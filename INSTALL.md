By following these instructions, you should be able to install a
working copy of agbcc and friends.

In order to build this, you need zlib as well as your usual build
tools:

Examples for installing zlib:

Linux and WSL:

    sudo apt-get install zlib-dev

Mac OS X with [Homebrew](https://brew.sh/)

    brew install zlib

Then run the following:

    make -j<number_of_cpus>
    make install prefix=install_prefix

A deprecated version of building is still supported, but only for
backwards compatibility:

    ./build.sh
    ./install.sh install_prefix

with install_prefix being the place where you want to place the
built binaries.

If the prefix is not set or is set to a suspicious location, it
will fail to install.

It is designed to be installed into the root of a pret GBA 
disassembly tree, and because of this, it will install itself
into the following hierarchy:

    install_prefix
    └── tools
        ├── agbcc
        │   ├── bin
        │   ├── lib
        │   └── include
        └── binutils
            ├── bin
            └── armv4tl-none-eabi
                ├── bin
                └── lib
                    └── ldscripts

