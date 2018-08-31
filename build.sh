#!/bin/sh
# for legacy purposes.
set -e

if command -v nproc >/dev/null 2>/dev/null; then
    JOBS="${JOBS:-$(nproc)}"
else
    JOBS="${JOBS:-2}"
fi

make clean
make -j $JOBS
