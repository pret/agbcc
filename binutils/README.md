#### GNU Binutils 2.31

This is a copy of GNU Binutils 2.31 with most of the contents removed
that are not directly related to ARM and pret.

The unmodified source tree is found http://ftp.gnu.org/gnu/binutils/binutils-2.31.tar.gz.

Don't try to configure this manually, it will probably fail. Instead, run
`make` from the parent directory.

If you really want to manually install, run this:
```
./configure --target=arm-none-eabi --program-prefix=arm-none-eabi- --disable-dependency-tracking --enable-gold=no --with-system-zlib --without-isl --without-bfd
make
```

Anything else is not guaranteed to work.

Eventually, this will be stripped down to the minimum requirements, including,
hopefully, the removal of autotools.

See README for the rest of the contents and licensing.
