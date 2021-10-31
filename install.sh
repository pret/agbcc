#!/bin/sh
set -e
if [ "$1" != "" ]; then
    dest="$(realpath "$1/tools/agbcc")"
    install -Dm755 -t "$dest/bin" agbcc old_agbcc agbcc_arm
    install -Dm644 -t "$dest/lib" libgcc.a libc.a
    install -Dm644 -t "$dest/include" ginclude/*
    cp -R libc/include "$dest/" #drop include, because we don't want include/include
    make -C binutils/build DESTDIR="$dest" install
else
    echo "Usage: install.sh PATH"
fi
