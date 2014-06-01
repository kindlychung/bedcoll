#!/usr/bin/env python3

from checkbed.Checkcoll import Checkcoll
import sys

helpmsg = """
Description:
Generates bim and fam files for collapsed bed files

Usage:
bimfamgen BED_FILE (original)
"""

if "-h" in sys.argv or "--help" in sys.argv:
    print(helpmsg)
    exit(0)

if len(sys.argv) == 2:
    x = Checkcoll(sys.argv[1])
    x.bimfamgen()
else:
    print(helpmsg)
    raise Exception("Arguments invalid!")

