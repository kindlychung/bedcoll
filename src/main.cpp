#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include "bedcoll.h"

using namespace std;


int main(int argc, char *argv[])
{
    namespace po = boost::program_options;
    po::options_description desc("Calculate collapsed genotype from plink .bed file.");

    std::string bfile;
    int nshift_min;
    int nshift_max;
    desc.add_options()
    ("allinram,a", "Read the whole bed file into RAM, faster, but might not be a good idea unless you have a lot of RAM")
    ("bfile,b", po::value<std::string>(&bfile)->required(), ".bed filepath")
    ("nshift_min,m", po::value<int>(&nshift_min)->required(), "Min number of SNPs to shift")
    ("nshift_max,n", po::value<int>(&nshift_max)->required(), "Max number of SNPs to shift")
    ("help,h", "print help")
    ;

    po::variables_map vm;
    po::store(parse_command_line(argc, argv, desc), vm);
    // this goes before notify, so -h is effective no matter
    // whether other options are provided
    if(vm.count("help")) {
        std::cout << desc << std::endl;
        return 0;
    }
    po::notify(vm);

    using namespace boost::filesystem;
    path bfile_path(bfile);
    path bpath = absolute(bfile_path);
    std::string bpath_str_stem = bpath.string();
    std::cout << "bed file stem: " << bpath_str_stem << "\n";

    std::cout << "Creating links to unshifted files, naming them as _shift_0000 ... \n";
    // linking bed file
    std::string bedfn = bpath_str_stem + ".bed";
    std::string bed_shift_fn = bpath_str_stem + "_shift_0000.bed";
    std::cout << "Creating a symlink for " << bed_shift_fn << "\n";
    int bedlink_ret = symlink(bedfn.c_str(), bed_shift_fn.c_str());
    if (bedlink_ret != 0) {
        throw "Failed to create symlink for bed file!";
    }

    // linking fam file
    std::string famfn = bpath_str_stem + ".fam";
    std::string fam_shift_fn = bpath_str_stem + "_shift_0000.fam";
    std::cout << "Creating a symlink for " << fam_shift_fn << "\n";
    int famlink_ret = symlink(famfn.c_str(), fam_shift_fn.c_str());
    if (famlink_ret != 0) {
        throw "Failed to create symlink for fam file!";
    }

    // linking bim file
    std::string bimfn = bpath_str_stem + ".bim";
    std::string bim_shift_fn = bpath_str_stem + "_shift_0000.bim";
    std::cout << "Creating a symlink for " << bim_shift_fn << "\n";
    int bimlink_ret = symlink(bimfn.c_str(), bim_shift_fn.c_str());
    if (bimlink_ret != 0) {
        throw "Failed to create symlink for bim file!";
    }


    if(vm.count("allinram"))
    {
        BedColl mColl(bpath_str_stem, true);
        mColl.collapseSeqShift(nshift_min, nshift_max);
    } else
    {
        BedColl mColl(bpath_str_stem, false);
        mColl.collapseSeqShift(nshift_min, nshift_max);
    }

    return 0;
}


