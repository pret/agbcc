#!/bin/sh
set -e
CCOPT=
CXXOPT=

[ ! -e gcc_arm/Makefile ] || make -C gcc_arm clean
CC="gcc -m32" cd gcc_arm && ./configure --target=arm-elf --host=i686-linux-gnu && make cc1 && cd ..
exit 0

if [ ! -z "$CC" ]; then CCOPT=CC=$CC; fi
if [ ! -z "$CXX" ]; then CXXOPT=CXX=$CXX; fi
make -C gcc clean
make -C gcc old $CCOPT $CXXOPT
mv gcc/old_agbcc .
make -C gcc clean
make -C gcc $CCOPT $CXXOPT
mv gcc/agbcc .
make -C libgcc clean
make -C libgcc $CCOPT $CXXOPT
mv libgcc/libgcc.a .
make -C libc clean
make -C libc $CCOPT $CXXOPT
mv libc/libc.a .
