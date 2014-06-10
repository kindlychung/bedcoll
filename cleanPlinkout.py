#!/usr/bin/python3

import sys
import re
import errno

if len(sys.argv) != 3:
    raise Exception("Commandline arguments incorrect")

n_predictors = int(sys.argv[1])
filepath = sys.argv[2]
try: 
    with open(filepath) as fh:
        # line counter
        n_line = 0
        # read the first line
        sys.stdout.write(fh.readline())
        n_line += 1
        # filter the rest 
        for line in fh:
            # only take results of the first predictor, as the others are covariates
            if (n_line % n_predictors) == 1:
                # merged_spaces = re.sub(r' {2,}', r' ', line)
                sys.stdout.write(line)
            n_line = n_line + 1
except IOError as e:
    if e.errno == errno.EPIPE:
        pass
