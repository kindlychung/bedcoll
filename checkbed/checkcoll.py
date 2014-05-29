#!/usr/bin/env python3

import sys
import os
import subprocess
import shutil



bitsdict = {
  "0000":"00",
  "0001":"01",
  "0010":"00",
  "0011":"00",
  "0100":"01",
  "0101":"01",
  "0110":"01",
  "0111":"01",
  "1000":"00",
  "1001":"01",
  "1010":"00",
  "1011":"11",
  "1100":"00",
  "1101":"01",
  "1110":"11",
  "1111":"11",
  }


def countlines(fname):
    i = -1
    with open(fname) as f:
        for i, l in enumerate(f):
            pass
    return i + 1


shiftstem, ext = os.path.splitext(shiftpath)
stempath, nshift = shiftstem.split("_shift_")
nshift = int(nshift)
bedpath = stempath + ".bed"
bimpath = stempath + ".bim"
fampath = stempath + ".fam"
shutil.copy(bedpath, "/tmp/")
bedpath1 = os.path.join("/tmp", os.path.basename(bedpath))

nsnp = countlines(bimpath)
nindiv = countlines(fampath)
bytes_snp = nindiv // 4 + 1
bytes_skip = bytes_snp * nskip
bytes_total = bytes_snp * nsnp
bytes_shift = bytes_snp * nshift
bytes_left = bytes_total - bytes_shift - bytes_skip

greet = """
Collapsed bed file: {}
bed file:           {}
bim file:           {}
fam file:           {}
Shift width:           {}
Number of SNPs:        {}
Number of obs:         {}
Bytes / SNP:           {}
SNPs skipped:          {}
""".format(
    os.path.basename(shiftpath),
    os.path.basename(bedpath),
    os.path.basename(bimpath),
    os.path.basename(fampath),
    nshift, nsnp, nindiv, bytes_snp, nskip
    )

logpath = "/tmp/checkcoll.log"
try:
    with open(bedpath, "rb") as fh1, open(bedpath1, "rb") as fh2, open(shiftpath, "rb") as fh3, open(logpath, "w+") as logfh:
        fh1.seek(3 + bytes_skip)
        fh2.seek(3 + bytes_skip + bytes_shift)
        fh3.seek(3 + bytes_skip)
        n_right = 0
        n_wrong = 0
        for i in range(bytes_left):
            error_occurred = False
            b1 = fh1.read(1)
            b2 = fh2.read(1)
            b3 = fh3.read(1)
            b1str = "{:08b}".format(ord(b1))
            b2str = "{:08b}".format(ord(b2))
            b3str = "{:08b}".format(ord(b3))
            b1bits = [b1str[i:(i+2)] for i in range(0, 8, 2)]
            b2bits = [b2str[i:(i+2)] for i in range(0, 8, 2)]
            b3bits = [b3str[i:(i+2)] for i in range(0, 8, 2)]
            for j in range(4):
                if bitsdict[b1bits[j] + b2bits[j]] == b3bits[j]:
                    n_right = n_right + 1
                else:
                    n_wrong = n_wrong + 1
                    logfh.write("\n=============== byte index: {}, error index: {} ==================\n".format(str(i), str(j)))
                    logfh.write("byte1 integer: {}\n".format(ord(b1)))
                    logfh.write("byte2 integer: {}\n".format(ord(b2)))
                    logfh.write("byte3 integer: {}\n".format(ord(b3)))
                    logfh.write("{}\n".format(str(b1bits)))
                    logfh.write("{}\n".format(str(b2bits)))
                    logfh.write("{}\n".format(str(b3bits)))

            printout = """
            Right results count: {}
            Wrong results count: {}
            """.format(n_right, n_wrong)
            tmp = subprocess.call("clear", shell=True)
            print(greet)
            print(printout)
except KeyboardInterrupt:
    print("\n\n\nChecking process ended by your request. \nHave a nice day, :-)")
except:
    print("Unknown error ocurred...")
finally:
    os.remove(bedpath1)
