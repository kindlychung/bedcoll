#!/usr/bin/env python3

from checkbed.Checkcoll import Checkcoll
import sys
import subprocess
import os

homedir = os.environ["HOME"]
rcfile = os.path.join(homedir, ".plinkcoll.rc")
if os.path.exists(rcfile):
    with open(rcfile) as rcfh:
        plink_bin = rcfh.readline().strip()
else:
    raise Exception("You should give path to plink binary in " + rcfile + " !")

if not os.path.exists(plink_bin) or not os.access(plink_bin, os.X_OK):
    raise Exception("Sorry, the plink path either does not exist or is not executable!")

if "--bfile" not in sys.argv:
    raise Exception("--bfile option must be specified!")
if "--out" in sys.argv:
    raise Exception("--out shouldn't be here, it's automatically taken care of!")

bfile_index = sys.argv.index("--bfile") + 1
bfile = sys.argv[bfile_index]
bfile, ext = os.path.splitext(bfile)
sys.argv[bfile_index] = bfile
out_opt = ["--out", bfile]
# print([plink_bin] + sys.argv[1:] + out_opt)
subprocess.Popen([plink_bin] + sys.argv[1:] + out_opt)


bfile_coll = Checkcoll(bfile)
shift_bedfiles = bfile_coll.shift_files
if shift_bedfiles:
    shift_bedfiles = [os.path.splitext(f)[0] for f in shift_bedfiles]
    for i in range(len(shift_bedfiles)):
        out_opt = ["--out", shift_bedfiles[i]]
        sys.argv[bfile_index] = shift_bedfiles[i]
#         print([plink_bin] + sys.argv[1:] + out_opt)
        subprocess.Popen([plink_bin] + sys.argv[1:] + out_opt)
    

# print(bfile)
# print(bfile_out)
# print(shift_bedfiles)
# print(shift_bedfiles_out)



# feed the original bed file to plink
# check if there are collapsed bed files
# if there is, modify argv and feed each of them to plink


# for clean_tree
# set an rc file:
# samtools path
# hg19 path
# preliminaries:
# R packages
