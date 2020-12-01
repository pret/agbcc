#!/bin/sh
set -e
if [ "$1" != "" ]; then
    mkdir -p $1/tools/agbcc
    mkdir -p $1/tools/agbcc/bin
    mkdir -p $1/tools/agbcc/include
    mkdir -p $1/tools/agbcc/lib
    cp build_agbcc/agbcc $1/tools/agbcc/bin/
    cp build_old_agbcc/old_agbcc $1/tools/agbcc/bin/
    cp gcc_arm/cc1 $1/tools/agbcc/bin/agbcc_arm
    cp -R libc/include $1/tools/agbcc/ #drop include, because we don't want include/include
    cp ginclude/* $1/tools/agbcc/include/
    cp libgcc/libgcc.a $1/tools/agbcc/lib/
    cp libc/libc.a $1/tools/agbcc/lib/
else
    echo "Usage: install.sh PATH"
fi
