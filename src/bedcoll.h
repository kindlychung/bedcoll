#pragma once

#include <stdio.h>
#include <boost/filesystem.hpp>
#include <iostream>
#include <algorithm>
#include <QString>
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
    boost::filesystem::path fstem;
    boost::filesystem::path fbranch;
    std::string bedfn;
    std::string bimfn;
    std::string famfn;
    bool saveram = false;
    off_t ramlimit = 5e8;
    FILE *file_in;
    BedColl(std::string fn, bool);
    ~BedColl();
    void collapseSingleShift(int nshift);
    void collapseSeqShift(int nshift);
    void collapseSeqShift(int nshift_start, int nshift_end);
};
