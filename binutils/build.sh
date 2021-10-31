#!/bin/sh
set -e

version=2.37
shasum=820d9724f020a3e69cb337893a0b63c2db161dadcb0e06fc11dc29eb1e84a32c
src=binutils-$version
tarball=binutils-$version.tar.xz

checksum() {
    test -f "$1" || return 1
    sha="$(sha256sum "$1" | cut -d ' ' -f 1)"
    test "$sha" = "$2" || return 1
    return 0
}

checksum "$tarball" "$shasum" || wget -c "https://ftp.gnu.org/gnu/binutils/$tarball"
if ! checksum "$tarball" "$shasum"; then 
    sha256sum "$tarball" 1>&2
    echo "Checksum failed!" 1>&2
    rm -f "$tarball"
fi

test -d "$src" || tar xf "$tarball"
mkdir -p build
cd build
MAKEINFO=false \
"../$src/configure" \
    --prefix= \
    --target=arm-none-eabi \
    --disable-nls
make -j "${NPROC:-$(nproc)}"
