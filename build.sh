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
make -C libgcc clean
make -C libgcc $CCOPT $CXXOPT
mv libgcc/libgcc.a .
make -C libc clean
make -C libc $CCOPT $CXXOPT
mv libc/libc.a .
