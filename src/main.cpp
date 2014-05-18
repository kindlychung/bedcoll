#include <QCoreApplication>
#include <QString>
#include <QDir>
#include <QDebug>
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
    int nshift_max;
    int nshift_min;
    desc.add_options()
    ("bfile,b", po::value<std::string>(&bfile)->required(), ".bed filepath")
    ("nshift_max,n", po::value<int>(&nshift_max)->required(), "Max number of SNPs to shift")
    ("nshift_min,m", po::value<int>(&nshift_min)->required(), "Min number of SNPs to shift")
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

    QString bfile_qstr(bfile.c_str());
    QDir bpath = QDir::currentPath();
    QString bpath_qstr = bpath.filePath(bfile_qstr);

    BedColl mColl(bpath_qstr.toStdString(), false);
    mColl.collapseSeqShift(nshift_min, nshift_max);
    return 0;
}


