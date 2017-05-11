# Note: You must have the environment variable DEVKITARM properly defined in your path.
# This is a basic script that uses the build.sh and install.sh files already in the repository.
# To execute it, simply run `make` in this directory.
# OS X users: The default gcc suite will fail to build libgcc.a.  Use the Hombrew version instead.

all: bin/agbcc bin/old_agbcc lib/libgcc.a
	sh build.sh
	sh install.sh ../..
	${DEVKITARM}/arm-none-eabi/bin/ranlib lib/libgcc.a

