#!/bin/bash

# you need to install either qt5-qmake or qt4-qmake
# sudo apt-get install libboost-filesystem-dev \
#     libboost-system-dev libboost-program-options-dev \
#     python3 clang-3.5

# tup upd

# if [[ ! -d "$HOME/bin" ]]
# then
#     mkdir $HOME/bin
# fi

chmod a+x   \
    bedcoll bedinfo.py bimfamgen.py \
    checkcoll.py compplink.py plinkcoll.py \
    cleanPlinkout.py removeAllLinks.sh

# if [[ $(uname -m) == "x86_64" ]]; then
#     echo "64bit OS, using the 64bit version of bedcoll..."
#     sudo cp -av bedcoll64 /usr/local/bin/bedcoll
# else
#     echo "32bit OS, using the 32bit version of bedcoll..."
#     sudo cp -av bedcoll32 /usr/local/bin/bedcoll
# fi

echo 'Copying the main binary bedcoll to /usr/local/bin ...'
sudo cp -a bedcoll /usr/local/bin/

echo 'Copying scripts to /usr/local/bin ...'
sudo cp -a \
    bedinfo.py bimfamgen.py \
    checkcoll.py compplink.py plinkcoll.py \
    cleanPlinkout.py removeAllLinks.sh \
    /usr/local/bin/

echo 'Copying python module checkbed into /usr/local/lib/python3.4/site-packages/'
sudo cp -a checkbed /usr/local/lib/python3.4/site-packages/

# plink and plinkcoll autocompletion
sudo compplink.py
