#!/bin/sh
set -e
if [ "$1" != "" ]; then
    make install prefix="$1"
else
    echo "Usage: install.sh PATH"
fi
