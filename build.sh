#!/bin/sh

set -e
CCOPT=
CXXOPT=

if [ ! -z "$CC" ]; then CCOPT=CC=$CC; fi
if [ ! -z "$CXX" ]; then CXXOPT=CXX=$CXX; fi

mkdir -p build_old_agbcc
srcdir=../gcc make -C build_old_agbcc -f ../gcc/Makefile clean
srcdir=../gcc make -C build_old_agbcc -f ../gcc/Makefile old $CCOPT $CXXOPT

mkdir -p build_agbcc
srcdir=../gcc make -C build_agbcc -f ../gcc/Makefile clean
srcdir=../gcc make -C build_agbcc -f ../gcc/Makefile normal $CCOPT $CXXOPT

# not sure if the ARM compiler is the old one or the new one (-DOLD_COMPILER)
# || true is needed to keep going if the distclean fails, such as when no configure has been done before
make -C gcc_arm distclean || true
cd gcc_arm && ./configure --target=arm-elf --host=i386-linux-gnu && cd ..
make -C gcc_arm cc1

make -C libgcc clean
make -C libgcc $CCOPT $CXXOPT

make -C libc clean
make -C libc $CCOPT $CXXOPT
