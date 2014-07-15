#!/usr/bin/python3

import sys
import re
import errno

helpmsg = """
Description:
    Clean up plink result files:
    1. Merge redundant delimiters (usually spaces)
    2. Skip covariates
Usage:
    cleanPlinkout.py -h             print this msg
    cleanPlinkout.py --help         print this msg
    cleanPlinkout.py N FILE         N is the number of variates
                                    FILE is the plink result file path
"""

if "-h" in sys.argv or "--help" in sys.argv:
    print(helpmsg)
    exit(0)
if len(sys.argv) != 3:
    raise Exception("Commandline arguments incorrect")


n_predictors = int(sys.argv[1])
filepath = sys.argv[2]
try:
    with open(filepath) as fh:
        # line counter
        n_line = 0
        # read the first line
        firstline = fh.readline().lstrip()
        firstline = re.sub(r' {2,}', r' ', firstline)
        sys.stdout.write(firstline)
        n_line += 1
        # filter the rest
        for line in fh:
            line = line.lstrip()
            # only take results of the first predictor, as the others are covariates
            if (n_line % n_predictors) == 1:
                line = re.sub(r' {2,}', r' ', line)
                sys.stdout.write(line)
            n_line = n_line + 1
except IOError as e:
    if e.errno == errno.EPIPE:
        pass
