# Requirements

* OS: currently support Linux and OSX
* Some boost libraries

    * On Linux

    `sudo apt-get install libboost1.55-all-dev`

    * On Mac OSX

    `brew install boost`

* You need tup if you want to compile bedcoll:

    * On Linux

    `sudo apt-get install libfuse-dev libc6-dev-i386`

    `git clone git://github.com/gittup/tup.git`

    `cd tup`

    `./bootstrap.sh`

    * On Mac OSX

    `// not sure how I got libfuse and libc installed on my mac`

    `git clone git://github.com/gittup/tup.git`

    `cd tup`

    `./bootstrap.sh`

    * In either case you need to copy the executable `tup` to your path, for example

    `sudo cp -av tup /usr/local/bin`

# Compile and install

    git clone https://github.com/kindlychung/bedcoll
    cd bedcoll
    tup upd
    ./install.sh

# Changes

* more debug messages
* create symlinks in c++
* original bed file now seen as shift 0000
