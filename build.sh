#!/bin/sh
set -e
CCOPT=
CXXOPT=

if [ ! -z "$CC" ]; then CCOPT=CC=$CC; fi
if [ ! -z "$CXX" ]; then CXXOPT=CXX=$CXX; fi
make -C gcc clean
make -C gcc -j$(nproc) old $CCOPT $CXXOPT
mv gcc/old_agbcc .
make -C gcc clean
make -C gcc -j$(nproc) normal $CCOPT $CXXOPT
mv gcc/agbcc .
# not sure if the ARM compiler is the old one or the new one (-DOLD_COMPILER)
make -C gcc_arm distclean
cd gcc_arm && ./configure --target=arm-elf --host=i386-linux-gnu && cd ..
make -C gcc_arm -j$(nproc) cc1
mv gcc_arm/cc1 agbcc_arm
make -C libgcc clean
make -C libgcc -j$(nproc) $CCOPT $CXXOPT
mv libgcc/libgcc.a .
make -C libc clean
make -C libc -j$(nproc) $CCOPT $CXXOPT
mv libc/libc.a .
