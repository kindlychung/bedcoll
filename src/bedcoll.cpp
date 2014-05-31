#include "bedcoll.h"

using namespace std;

BedColl::BedColl(std::string fn, bool allinram) :
    allinram(allinram)
{
    boost::filesystem::path fpath(fn);
    fstem = fpath.stem();
    fbranch = fpath.branch_path();
    auto fbed = fbranch / (fstem.string() + ".bed");
    auto fbim = fbranch / (fstem.string() + ".bim");
    auto ffam = fbranch / (fstem.string() + ".fam");
    bedfn = fbed.string();
    bimfn = fbim.string();
    famfn = ffam.string();

    nsnp = countlines(bimfn);
    nindiv = countlines(famfn);
    bytes_snp = ceil(nindiv / 4.0);
    bytes_read = bytes_snp * nsnp;
    bed_filesize = fileSize(bedfn);

    if (bed_filesize != (bytes_read + 3)) {
        cout << "error initialization" << endl;
        cout << bed_filesize << endl;
        cout << nsnp << endl;
        cout << nindiv << endl;
        cout << bytes_read << endl;
        throw conflict_bed_bim_fam;
    }

    file_in = fopen(bedfn.c_str(), "rb");
    if (!file_in) {
        throw file_open_error;
    }
}

BedColl::~BedColl()
{
    cout << "Destroyed!" << endl;
    if (fclose(file_in) != 0) {
        throw file_close_error;
    }
    if (fclose(outfile) != 0) {
        throw file_close_error;
    }
}

void BedColl::collapseSingleShift(off_t nshift)
{
    unsigned char *res_buffer = 0;
    unsigned char *buffer = 0;
    unsigned char *res_buffer_remain = 0;
    unsigned char *remain_buffer = 0;
    unsigned char *collres = 0;

    off_t bytes_shift = bytes_snp * nshift;
    off_t bytes_left = bytes_read - bytes_shift;
    off_t nsnp_left = nsnp - nshift;

    try {
        // output fam file path
        boost::format outfam_leaf("_shift_%04d.fam");
        outfam_leaf % (int)nshift;
        auto famout = fbranch / (fstem.string() + outfam_leaf.str());
        auto fam_shift_fn = famout.string();
        // create a symlink for fam file
        int famlink_ret = symlink(famfn.c_str(), fam_shift_fn.c_str());
        if (famlink_ret != 0) {
            throw "Failed to create symlink for fam file!";
        }

        // output bim file path
        boost::format outbim_leaf("_shift_%04d.bim");
        outbim_leaf % (int)nshift;
        auto bimout = fbranch / (fstem.string() + outbim_leaf.str());
        auto bim_shift_fn = bimout.string();
        std::string bim_line;
        ifstream bim_fh;
        bim_fh.open(bimfn);
        ofstream bim_shift_fh;
        bim_shift_fh.open(bim_shift_fn);
        if (bim_fh.is_open() and bim_shift_fh.is_open()) {
            for (off_t i = 0; i < nsnp_left; i++) {
                getline(bim_fh, bim_line);
                bim_shift_fh << bim_line << "\n";
            }
            bim_fh.close();
            bim_shift_fh.close();
        } else {
            throw "Cannot open bim file and / or shift bim file!";
        }
    }
    catch(const string& e) {
        cerr << e << endl;
    }
    catch(...) {
    }



    // output bed file path
    boost::format outf_leaf("_shift_%04d.bed");
    outf_leaf % (int)nshift;
    auto fout = fbranch / (fstem.string() + outf_leaf.str());
    auto outfn = fout.string();

    if (allinram == true) {
        try {
            // see the documentation picture on flickr
            buffer = (unsigned char *)malloc(bytes_read);
            if (!buffer) {
                fprintf(stderr, "Memory error!");
                fclose(file_in);
            }
            fseeko(file_in, 3, SEEK_SET);
            fread(buffer, bytes_read, 1, file_in);

            collres = (unsigned char *)malloc(bytes_left);
            for (off_t i = 0; i < bytes_left; i++) {
                collres[i] = collgen[buffer[i] * 256 + buffer[i + bytes_shift]];
            }

            outfile = fopen(outfn.c_str(), "w+");
            if (outfile) {
                fwrite(magicbits, 3, 1, outfile);
                fwrite(collres, bytes_left, 1, outfile);
            } else {
                throw file_open_error;
            }

            free(buffer);
            free(collres);
        } catch (const std::string& s) {
            cerr << s << endl;
        } catch (...) {
            cerr << "Unknown error..." << endl;
            free(buffer);
            free(collres);
        }
    } else {
        try {
            off_t nsnp_read = nsnp - nshift;
            off_t n_iter = (off_t) nsnp_read / snp_iter;
            off_t n_remain = nsnp_read % snp_iter;
            off_t bytes_iter = snp_iter * bytes_snp;
            off_t bytes_remain = n_remain * bytes_snp;
            off_t bytes_shift = nshift * bytes_snp;
            off_t bytes_all_iters = n_iter * bytes_iter;


            outfile = fopen(outfn.c_str(), "w+");
            if (outfile) {
                fwrite(magicbits, 3, 1, outfile);
            } else {
                throw file_open_error;
            }

            res_buffer = (unsigned char *)malloc(bytes_iter);
            buffer = (unsigned char *)malloc(bytes_iter + bytes_shift);

            for (off_t i = 0; i < n_iter; i++) {
                fseeko(file_in, 3 + i * bytes_iter, SEEK_SET);
                fread(buffer, bytes_iter + bytes_shift, 1, file_in);
                for (off_t j = 0; j < bytes_iter; j++) {
                    res_buffer[j] = collgen[buffer[j] * 256 + buffer[j + bytes_shift]];
                }
                fwrite(res_buffer, bytes_iter, 1, outfile);
            }

            res_buffer_remain = (unsigned char *)malloc(bytes_remain);
            remain_buffer = (unsigned char *)malloc(bytes_remain + bytes_shift);

            fseeko(file_in, 3 + bytes_all_iters, SEEK_SET);
            fread(remain_buffer, bytes_remain + bytes_shift, 1, file_in);
            for (off_t i = 0; i < bytes_remain; i++) {
                res_buffer_remain[i] = collgen[remain_buffer[i] * 256 + remain_buffer[i + bytes_shift]];
            }
            fwrite(res_buffer_remain, bytes_remain, 1, outfile);

            free(buffer);
            free(res_buffer);
            free(remain_buffer);
            free(res_buffer_remain);
        } catch (const std::string& e_string) {
            cerr << e_string << endl;
        } catch (...) {
            cerr << "Unknown error..." << endl;
            free(buffer);
            free(res_buffer);
            free(remain_buffer);
            free(res_buffer_remain);
        }
    }
}

void BedColl::collapseSeqShift(off_t nshift)
{
    if (nshift > nsnp - 1) {
        throw shift_too_large;
    }
    for (int i = 1; i <= nshift; i++) {
        collapseSingleShift(i);
    }
}

void BedColl::collapseSeqShift(off_t nshift_start, off_t nshift_end)
{
    if (nshift_start < 1 or nshift_end > nsnp - 1) {
        throw shift_out_of_range;
    }
    for (int i = nshift_start; i <= nshift_end; i++) {
        collapseSingleShift(i);
    }
}
