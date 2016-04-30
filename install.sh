#!/bin/sh
if [ "$1" != "" ]; then
    mkdir -p $1/tools/agbcc
    mkdir -p $1/tools/agbcc/bin
    mkdir -p $1/tools/agbcc/include
    mkdir -p $1/tools/agbcc/lib
    cp gcc/agbcc $1/tools/agbcc/bin/
    cp ginclude/* $1/tools/agbcc/include/
    cp libgcc/libgcc.a $1/tools/agbcc/lib/
else
    echo "Usage: install.sh PATH"
fi
