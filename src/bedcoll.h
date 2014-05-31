#pragma once

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "util.h"
#include "error_enum.h"
#include "collgen_memo.h"

class BedColl
{
public:
    off_t nsnp = 0;
    off_t nindiv = 0;
    off_t bytes_snp = 0;
    off_t bytes_read = 0;
    off_t bed_filesize = 0;
    off_t snp_iter = 5000;

    boost::filesystem::path fstem;
    boost::filesystem::path fbranch;
    std::string bedfn;
    std::string bimfn;
    std::string famfn;
    bool allinram = false;

    FILE *file_in = 0;
    FILE *outfile = 0;

    BedColl(std::string fn, bool);
    ~BedColl();
    void collapseSingleShift(off_t nshift);
    void collapseSeqShift(off_t nshift);
    void collapseSeqShift(off_t nshift_start, off_t nshift_end);
};
