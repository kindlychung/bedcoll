#!/usr/bin/env python3

from checkbed.test1 import sysutil

if __name__ == "__main__":
    print(sysutil.cwd())
    print(sysutil.modpath())
    sysutil.newsayhi()