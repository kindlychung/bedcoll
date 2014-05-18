#include "bedcoll.h"

using namespace std;

BedColl::BedColl(std::string fn, bool saveram) : saveram(saveram) {
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

    if(bed_filesize != (bytes_read + 3)) {
        cout << "error initialization" << endl;
        cout << bed_filesize << endl;
        cout << nsnp << endl;
        cout << nindiv << endl;
        cout << bytes_read << endl;
        throw conflict_bed_bim_fam;
    };
    if(bytes_read > ramlimit) { saveram = true; }


    file_in = fopen(bedfn.c_str(), "rb");
    if (!file_in)
    {
        cout << "file_open_error" << endl;
        throw file_open_error;
    }
}

BedColl::~BedColl() {
    cout << "Destroyed!" << endl;
    if(fclose(file_in) != 0)
    {
        cout << "file_close_error" << endl;
        throw file_close_error;
    }
}

void BedColl::collapseSingleShift(int nshift)
{
    QString outf_leaf;
    outf_leaf.sprintf("_shift_%04d.bed", nshift);
    auto fout = fbranch / (fstem.string() + outf_leaf.toStdString());
    auto outfn = fout.string();

    off_t bytes_shift = bytes_snp * nshift;
    if(saveram == false) {
        // see the documentation picture on flickr
        off_t bytes_left = bytes_read - bytes_shift;
        unsigned char buffer[bytes_read]; if (!buffer) { fprintf(stderr, "Memory error!"); fclose(file_in); }
        fseeko(file_in, 3, SEEK_SET);
        fread(buffer, bytes_read, 1, file_in);

        unsigned char collres[bytes_read];
        // initialize with NA
        std::fill_n(collres, bytes_read, 0x55);
        for(off_t i=0; i<bytes_left; i++)
        {
            collres[i] = collgen[buffer[i] * 256 + buffer[i+bytes_shift]];
        }

        FILE *outfile = fopen(outfn.c_str(), "w+");
        if(outfile)
        {
            fwrite(magicbits, 3, 1, outfile);
            fwrite(collres, bytes_read, 1, outfile);
        } else {
            cout << "file_open_error: " << outfn << outfile << endl;
            throw file_open_error;
        }
        if(fclose(outfile) != 0)
        {
            cout << "file_close_error" << endl;
            throw file_close_error;
        }
    } else {
    }
}

void BedColl::collapseSeqShift(int nshift)
{
    if(nshift > nsnp-1)
    {
        cout << "shift_too_large" << endl;
        throw shift_too_large;
    }
    for(int i=1; i<=nshift; i++)
    {
        collapseSingleShift(i);
    }
}

void BedColl::collapseSeqShift(int nshift_start, int nshift_end)
{
    if(nshift_start < 1 or nshift_end > nsnp-1)
    {
        cout << "shift_out_of_range" << endl;
        throw shift_out_of_range;
    }
    for(int i=nshift_start; i<=nshift_end; i++)
    {
        collapseSingleShift(i);
    }
}
