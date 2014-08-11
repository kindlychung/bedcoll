#include "bedcoll.h"

using namespace std;

BedColl::BedColl(std::string fn, bool allinram) :
    allinram(allinram)
{
    try {
        boost::filesystem::path fpath(fn);
        fstem = fpath.stem();
        fbranch = fpath.branch_path();
        auto fbed = fbranch / (fstem.string() + ".bed");
        auto fbim = fbranch / (fstem.string() + ".bim");
        auto ffam = fbranch / (fstem.string() + ".fam");
        bedfn = fbed.string();
        bimfn = fbim.string();
        famfn = ffam.string();
        std::cout << "bed file path: " << bedfn << "\n";
        std::cout << "fam file path: " << famfn << "\n";
        std::cout << "bim file path: " << bimfn << "\n";

        nsnp = countlines(bimfn);
        nindiv = countlines(famfn);
        bytes_snp = ceil(nindiv / 4.0);
        std::cout << "Each SNP contains " << bytes_snp << " bytes of data\n";
        bytes_read = bytes_snp * nsnp;
        std::cout << "All SNPs use " << bytes_read << " bytes of data\n";
        bed_filesize = fileSize(bedfn);
        std::cout << "bed file use " << bed_filesize << " bytes of data\n";

        if (bed_filesize != (bytes_read + 3)) {
            throw conflict_bed_bim_fam;
        }

        file_in = fopen(bedfn.c_str(), "rb");
        if (!file_in) {
            throw file_open_error;
        }
    } catch (const string& e) {
        cerr << e << endl;
    } catch (...) {
    }
}

BedColl::~BedColl()
{
    try {
        cout << "..............\n";
        if (fclose(file_in) != 0) {
            throw file_close_error;
        }
        if (fclose(outfile) != 0) {
            throw file_close_error;
        }
    } catch (const string& e) {
        cerr << e << endl;
    } catch (...) {
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
    std::cout << "You requested to shift " << nshift << "SNPs\n";
    std::cout << "Each SNP contains " << bytes_snp << " bytes of data\n";
    std::cout << "Therefore I will shift " << bytes_shift << " bytes\n";
    std::cout << "After shifting there are " << bytes_left << " bytes of data left\n";
    std::cout << "which is to say, " << nsnp_left << " SNPs left\n";

    try {

        // linking bim file
        boost::format outbim_leaf("_shift_%04d.bim");
        outbim_leaf % (int)nshift;
        auto bimout = fbranch / (fstem.string() + outbim_leaf.str());
        auto bim_shift_fn = bimout.string();
        std::cout << "Creating a symlink for " << bim_shift_fn << "\n";
        int bimlink_ret = symlink(bimfn.c_str(), bim_shift_fn.c_str());
        if (bimlink_ret != 0) {
            throw "Failed to create symlink for bim file!";
        }

        // linking fam file
        boost::format outfam_leaf("_shift_%04d.fam");
        outfam_leaf % (int)nshift;
        auto famout = fbranch / (fstem.string() + outfam_leaf.str());
        auto fam_shift_fn = famout.string();
        std::cout << "Creating a symlink for " << fam_shift_fn << "\n";
        int famlink_ret = symlink(famfn.c_str(), fam_shift_fn.c_str());
        if (famlink_ret != 0) {
            throw "Failed to create symlink for fam file!";
        }

//        // output bim file path
//        boost::format outbim_leaf("_shift_%04d.bim");
//        outbim_leaf % (int)nshift;
//        auto bimout = fbranch / (fstem.string() + outbim_leaf.str());
//        auto bim_shift_fn = bimout.string();
//        std::string bim_line;
//        ifstream bim_fh;
//        bim_fh.open(bimfn);
//        ofstream bim_shift_fh;
//        bim_shift_fh.open(bim_shift_fn);
//        if (bim_fh.is_open() and bim_shift_fh.is_open()) {
//            for (off_t i = 0; i < nsnp_left; i++) {
//                getline(bim_fh, bim_line);
//                bim_shift_fh << bim_line << "\n";
//            }
//            bim_fh.close();
//            bim_shift_fh.close();
//        } else {
//            throw "Cannot open bim file and / or shift bim file!";
//        }
    } catch (const string& e) {
        cerr << e << endl;
    } catch (...) {
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
                throw file_open_error;
            }
            fseeko(file_in, 3, SEEK_SET);
            fread(buffer, bytes_read, 1, file_in);

            collres = (unsigned char *)malloc(bytes_read);
            std::fill_n(collres, bytes_read, 0x00);
            for (off_t i = 0; i < bytes_left; i++) {
                collres[i] = collgen[buffer[i] * 256 + buffer[i + bytes_shift]];
//                using namespace boost;
//                std::cout << format("%5d  %5d  %5d") % (int)buffer[i] % (int)buffer[i + bytes_shift] % (int)collres[i] << "\n";
            }

            outfile = fopen(outfn.c_str(), "w+");
            if (outfile) {
                fwrite(magicbits, 3, 1, outfile);
                fwrite(collres, bytes_read, 1, outfile);
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
            off_t bytes_remain_plus_shift = bytes_remain + bytes_shift;


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

            res_buffer_remain = (unsigned char *)malloc(bytes_remain_plus_shift);
            std::fill_n(res_buffer_remain, bytes_remain_plus_shift, 0x00);
            remain_buffer = (unsigned char *)malloc(bytes_remain_plus_shift);

            fseeko(file_in, 3 + bytes_all_iters, SEEK_SET);
            fread(remain_buffer, bytes_remain_plus_shift, 1, file_in);
            for (off_t i = 0; i < bytes_remain; i++) {
                res_buffer_remain[i] = collgen[remain_buffer[i] * 256 + remain_buffer[i + bytes_shift]];
            }
            fwrite(res_buffer_remain, bytes_remain_plus_shift, 1, outfile);

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
    try {
        if (nshift > nsnp - 1) {
            throw shift_too_large;
        }
        for (int i = 1; i <= nshift; i++) {
            collapseSingleShift(i);
        }
    } catch (const std::string& e_string) {
        cerr << e_string << endl;
    } catch (...) {
        cerr << "Unknown error..." << endl;
    }
}

void BedColl::collapseSeqShift(off_t nshift_start, off_t nshift_end)
{
    try {
        if (nshift_start < 1 or nshift_end > nsnp - 1) {
            throw shift_out_of_range;
        }
        for (int i = nshift_start; i <= nshift_end; i++) {
            collapseSingleShift(i);
        }
    } catch (const std::string& e_string) {
        cerr << e_string << endl;
    } catch (...) {
        cerr << "Unknown error..." << endl;
    }
}
