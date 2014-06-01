#!/usr/bin/env python3

from checkbed.Checkcoll import Checkcoll
import sys
import subprocess

# read ~/.plinkcoll.rc
# if not exist, create it and ask for plink path, store
# if exists, read and check if it is executable

if "--bfile" not in sys.argv:
    raise Exception("--bfile option must be specified")

bfile_index = sys.argv.index("--bfile") + 1
bfile = sys.argv[bfile_index]
bfile_coll = Checkcoll(bfile)
# feed the original bed file to plink
# check if there are collapsed bed files
# if there is, modify argv and feed each of them to plink
