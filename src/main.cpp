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
    std::string bpath_str = bpath.string();

    if(vm.count("allinram"))
    {
        BedColl mColl(bpath_str, true);
        mColl.collapseSeqShift(nshift_min, nshift_max);
    } else
    {
        BedColl mColl(bpath_str, false);
        mColl.collapseSeqShift(nshift_min, nshift_max);
    }

    return 0;
}


