#!/bin/sh
set -e
CCOPT=
CXXOPT=

if [ ! -z "$CC" ]; then CCOPT=CC=$CC; fi
if [ ! -z "$CXX" ]; then CXXOPT=CXX=$CXX; fi
make -C gcc clean
make -C gcc old $CCOPT $CXXOPT
mv gcc/old_agbcc .
make -C gcc clean
make -C gcc $CCOPT $CXXOPT
mv gcc/agbcc .
# not sure if the ARM compiler is the old one or the new one (-DOLD_COMPILER)
[ ! -e gcc_arm/Makefile ] || make -C gcc_arm clean
cd gcc_arm && ./configure --target=arm-elf --host=i686-linux-gnu && make cc1 && cd ..
mv gcc_arm/cc1 agbcc_arm
make -C libgcc clean
make -C libgcc $CCOPT $CXXOPT
mv libgcc/libgcc.a .
make -C libc clean
make -C libc $CCOPT $CXXOPT
mv libc/libc.a .
