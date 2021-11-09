#!/bin/sh
set -e

default_dest=/opt/agbcc

if [ $# -ge 1 ]; then
    dest="$(realpath "$1")"
else
    echo "INFO: Missing parameter, defaulting to installing in $default_dest" 1>&2
    dest="$default_dest"
fi

# Check if we can access $dest
if [ -d "$dest" -a ! -w "$dest" ] || ! install -d "$dest" 2> /dev/null; then
   echo "ERROR: Can't access directory '$dest'. Are you sure you have sufficient permissions? (hint: use 'sudo ./install.sh')" 1>&2
   exit 1
fi

install -Dm755 -t "$dest/bin" agbcc old_agbcc agbcc_arm
install -Dm644 -t "$dest/lib" libgcc.a libc.a
install -Dm644 -t "$dest/include" ginclude/*
cp -R libc/include "$dest/" #drop include, because we don't want include/include
make -C binutils/build DESTDIR="$dest" install
