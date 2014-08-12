# Requirements

* OS: currently support Linux and OSX
* Some boost libraries

    * On Linux

    sudo apt-get install libboost1.55-all-dev

    * On Mac OSX

    brew install boost

* You need tup if you want to compile bedcoll:

    * On Linux

    `sudo apt-get install libfuse-dev libc6-dev-i386`

    `git clone git://github.com/gittup/tup.git`

    * On Mac OSX

    `// not sure how I got libfuse and libc installed on my mac`

    `git clone git://github.com/gittup/tup.git`

# Compile and install

    tup upd
    ./install.sh

# Changes

* more debug messages
* create symlinks in c++
* original bed file now seen as shift 0000
