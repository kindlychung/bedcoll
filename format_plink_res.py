#!/usr/bin/python3

import subprocess

subprocess.call("""awk '{print $2, $3}' ssknEx.assoc.linear > snpbp.base""", shell=True, executable="/bin/bash")
subprocess.call("""sed -i "1s/SNP BP/SNP1 BP1/" snpbp.base""", shell=True, executable="/bin/bash")