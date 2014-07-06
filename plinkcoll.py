#!/usr/bin/env python3

from checkbed.Checkcoll import Checkcoll
import sys
import subprocess
import os
import datetime

# now = datetime.datetime.now()
# nowstring = "_%04d%02d%02d-%02d%02d%02d" % (now.year, now.month, now.day, now.hour, now.minute, now.second)
# print(nowstring)
helpmsg = """
Apply plink on a series of collapsed genotype files:

    plinkcoll.py [options] [options for plink]

    --help                     show this help message
    --plinkcoll-range [m:n]    which files to analyze, largest range is 0:N,
                               where N is the max shift, suppose the original bed file
                               is x.bed, then:
                               0:3 includes x.bed x_shift_0001.bed ... x_shift_0003.bed
                               1:2 includes x_shift_0001.bed ... x_shift_0002.bed
                               3:  includes x_shift_0003.bed ... x_shift_000N.bed
                               :1  includes x.bed x_shift_0001.bed
                               :   includes x.bed x_shift_0001.bed ... x_shift_000N.bed

"""

# get plink binary path
homedir = os.environ["HOME"]
rcfile = os.path.join(homedir, ".plinkcoll.rc")
if os.path.exists(rcfile):
    with open(rcfile) as rcfh:
        plink_bin = rcfh.readline().strip()
else:
    raise Exception("You should give path to plink binary in " + rcfile + " !")

if not os.path.exists(plink_bin) or not os.access(plink_bin, os.X_OK):
    raise Exception("Sorry, the plink path either does not exist or is not executable!")

# validate commandline options
if "--help" in sys.argv:
    print(helpmsg)
    exit(0)
if "--bfile" not in sys.argv:
    raise Exception("--bfile option must be specified!")
if "--out" in sys.argv:
    raise Exception("--out shouldn't be here, it's automatically taken care of!")
if "--plinkcoll-range" not in sys.argv:
    raise Exception("You must give me a range of files to operate on!")
    
# get original bed file, remove extension for plink
bfile_index = sys.argv.index("--bfile") + 1
bfile = sys.argv[bfile_index]
bfile, ext = os.path.splitext(bfile)

# get shift bed files, combine with the original into a list
bfile_coll = Checkcoll(bfile)
bedfiles = [bfile + ".bed"] + bfile_coll.shift_files 

# get range option
rangeIdx = sys.argv.index("--plinkcoll-range")
plinkcollRage = sys.argv[rangeIdx + 1]

# validate range
if ":" not in plinkcollRage:
    raise Exception("Range is malformatted!")

fromN, toN = plinkcollRage.split(":")

if not fromN:
    fromN = 0
elif int(fromN) < 0 or int(fromN) >= len(bedfiles):
    raise Exception("Range invalid!")
else:
    fromN = int(fromN)

if not toN:
    toN = len(bedfiles)
elif int(toN) < fromN or int(toN) >= len(bedfiles):
    raise Exception("Range invalid!")
else:
    toN = int(toN) + 1 # need to plus 1 because of list indexing rules in python
del(sys.argv[rangeIdx:(rangeIdx + 2)])

print(sys.argv)
# print("Selected bedfiles: ", bedfiles[fromN:toN])

bedfiles = [os.path.splitext(f)[0] for f in bedfiles]
for bedfile in bedfiles[fromN:toN]:
    out_opt = ["--out", bedfile]
    sys.argv[bfile_index] = bedfile
    cmd = [plink_bin] + sys.argv[1:] + out_opt
    print("\n\nExec: ", " ".join(cmd))
    retcode = subprocess.call(cmd)
    if retcode == 0:
        # on success, remove log files
        os.remove(bedfile + ".log")
    else:
        raise Exception("plink analysis failed, check %s" % bedfile + ".log")
