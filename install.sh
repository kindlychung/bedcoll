#!/bin/bash

# you need to install either qt5-qmake or qt4-qmake
sudo apt-get install libboost-filesystem-dev libboost-system-dev libboost-program-options-dev python3
qmake bedcollqt.pro -r -spec linux-clang && make

if [[ ! -d "$HOME/bin" ]]
then
    mkdir $HOME/bin
fi

chmod a+x    bedcoll bedinfo bimfamgen checkcoll compplink
sudo cp -avi bedcoll bedinfo bimfamgen checkcoll compplink /usr/local/bin/
sudo cp -avi checkbed /usr/lib/python3/dist-packages/
