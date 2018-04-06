#!/bin/sh
set -e
make -C gcc clean
make -C gcc old
mv gcc/old_agbcc .
make -C gcc clean
make -C gcc
mv gcc/agbcc .
make -C libgcc clean
make -C libgcc
mv libgcc/libgcc.a .
make -C libc clean
make -C libc
mv libc/libc.a .
