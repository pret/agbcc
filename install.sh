#!/bin/sh
set -e

if [ "$1" = "-h" ]; then
	echo "Usage: install.sh PATH"
	exit 0
fi

if [ "$1" = "" ]; then
	INSTALL_DIRECTORY=/opt/pret/
	mkdir -p $INSTALL_DIRECTORY
else
	INSTALL_DIRECTORY=$1
fi

if [ -d "$INSTALL_DIRECTORY" ]; then
	mkdir -p $INSTALL_DIRECTORY/tools/agbcc
	mkdir -p $INSTALL_DIRECTORY/tools/agbcc/bin
	mkdir -p $INSTALL_DIRECTORY/tools/agbcc/include
	mkdir -p $INSTALL_DIRECTORY/tools/agbcc/lib
	cp agbcc $INSTALL_DIRECTORY/tools/agbcc/bin/
	cp old_agbcc $INSTALL_DIRECTORY/tools/agbcc/bin/
	cp agbcc_arm $INSTALL_DIRECTORY/tools/agbcc/bin/
	cp -R libc/include $INSTALL_DIRECTORY/tools/agbcc/ #drop include, because we don't want include/include
	cp ginclude/* $INSTALL_DIRECTORY/tools/agbcc/include/
	cp libgcc.a $INSTALL_DIRECTORY/tools/agbcc/lib/
	cp libc.a $INSTALL_DIRECTORY/tools/agbcc/lib/
	echo "agbcc successfully installed!"
else
	if [ -d "../$INSTALL_DIRECTORY" ]; then
		echo "Target directory does not exist. Did you mean to do \"./install.sh ../$1\"?"
	else
		if case $INSTALL_DIRECTORY in ".."*) true;; *) false;; esac; then
			echo "Target directory does not exist. If you aren't familiar with relative paths, make sure that agbcc and the repository are in the same directory, and run \"./install.sh $1\" again."
		else
			if echo "$INSTALL_DIRECTORY" | grep -qE '^[^/]*.$'; then
				echo "Target directory does not exist. You probably meant to do \"./install.sh ../$1\", but agbcc and $1 do not exist in the same directory. Check your spelling, make sure that the repository has been cloned, ensure that agbcc and the repository are in the same directory, and run \"./install.sh ../$1\" again."
			else
				echo "Target directory does not exist. Check your spelling, re-read the instructions, and try again."
			fi
		fi
	fi
fi
