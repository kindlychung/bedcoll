#!/bin/bash

# you need to install either qt5-qmake or qt4-qmake
sudo apt-get install libboost-filesystem-dev \
    libboost-system-dev libboost-program-options-dev \
    python3 clang-3.5

# tup upd

# if [[ ! -d "$HOME/bin" ]]
# then
#     mkdir $HOME/bin
# fi

chmod a+x   \
    bedcoll bedcoll32 bedcoll64 bedinfo.py bimfamgen.py \
    checkcoll.py compplink.py plinkcoll.py \
    cleanPlinkout.py removeAllLinks.sh

if [[ $(uname -m) == "x86_64" ]]; then
    echo "64bit OS, using the 64bit version of bedcoll..."
    sudo cp -av bedcoll64 /usr/local/bin/bedcoll
else
    echo "32bit OS, using the 32bit version of bedcoll..."
    sudo cp -av bedcoll32 /usr/local/bin/bedcoll
fi

sudo cp -av \
    bedinfo.py bimfamgen.py \
    checkcoll.py compplink.py plinkcoll.py \
    cleanPlinkout.py removeAllLinks.sh \
    /usr/local/bin/

sudo cp -av checkbed /usr/lib/python3/dist-packages/

# plink and plinkcoll autocompletion
sudo compplink.py
