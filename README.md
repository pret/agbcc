# agbcc

This repository contains the following subprojects, with their
directory trees changed from the original:

#### GCC/LibGCC 2.95

Known as agbcc, this is a stripped version of GCC 2.95 that is
designed to run only in cc1 mode and targeting arm-thumb-elf,
isolated from autotools.

GCC is licensed under the GPLv2+, with libgcc containing the GCC
linking exception. See gcc/COPYING.

#### newlib 1.8.2

Autoconf removed, only for arm thumb, and flattened some directories.

Mixed licensing, see libc/COPYING.NEWLIB.

#### GNU Binutils 2.31

This is a copy of GNU Binutils 2.31 with most of the contents removed
that are not directly related to ARM and pret.

The unmodified source tree is found http://ftp.gnu.org/gnu/binutils/binutils-2.31.tar.gz.

Reducing the code size is still a work in progress, with autotools
still required to be run.

See README.binutils for the original README.

See COPYING for licensing.


