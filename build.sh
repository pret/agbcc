#!/bin/sh
make -C gcc clean
make -C gcc old
make -C libgcc clean
make -C libgcc
make -C gcc clean
make -C gcc
