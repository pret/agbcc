#!/bin/sh
set -e

(cd binutils
    ./clean.sh
    ./build.sh
)
make -C gcc clean
make -C gcc old
mv gcc/old_agbcc .
make -C gcc clean
make -C gcc
mv gcc/agbcc .
# not sure if the ARM compiler is the old one or the new one (-DOLD_COMPILER)
( cd gcc_arm
    rm -f config.status config.cache
    ./configure --target=arm-elf --host=i386-linux-gnu
    make cc1
)
mv gcc_arm/cc1 agbcc_arm
make -C libgcc clean
make -C libgcc
mv libgcc/libgcc.a .
make -C libc clean
make -C libc
mv libc/libc.a .
