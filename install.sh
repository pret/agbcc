#!/bin/sh
set -e

if [ "$1" = "-h" ]; then
	echo "Usage: install.sh PATH"
	exit 0
fi

# If there's no installation directory specified, the install script will default to 
# installing agbcc into /opt/pret/agbcc
if [ "$1" = "" ]; then
	BASE_INSTALL_DIRECTORY=/opt/pret
	INSTALL_DIRECTORY=$BASE_INSTALL_DIRECTORY
	mkdir -p $INSTALL_DIRECTORY
else
	BASE_INSTALL_DIRECTORY=$1
	INSTALL_DIRECTORY=$BASE_INSTALL_DIRECTORY/tools
fi

# The BASE_INSTALL_DIRECTORY nomenclature is so that the check for the existence of the 
# directory doesn't fail just because there's no tools directory yet.
if [ -d "$BASE_INSTALL_DIRECTORY" ]; then
	mkdir -p $INSTALL_DIRECTORY/agbcc
	mkdir -p $INSTALL_DIRECTORY/agbcc/bin
	mkdir -p $INSTALL_DIRECTORY/agbcc/include
	mkdir -p $INSTALL_DIRECTORY/agbcc/lib
	cp agbcc $INSTALL_DIRECTORY/agbcc/bin/
	cp old_agbcc $INSTALL_DIRECTORY/agbcc/bin/
	cp agbcc_arm $INSTALL_DIRECTORY/agbcc/bin/
	cp -R libc/include $INSTALL_DIRECTORY/agbcc/ #drop include, because we don't want include/include
	cp ginclude/* $INSTALL_DIRECTORY/agbcc/include/
	cp libgcc.a $INSTALL_DIRECTORY/agbcc/lib/
	cp libc.a $INSTALL_DIRECTORY/agbcc/lib/
	echo "agbcc successfully installed!"
else
	if [ -d "../$BASE_INSTALL_DIRECTORY" ]; then
		echo "Target directory does not exist. Did you mean to do \"./install.sh ../$1\"?"
	else
		if case $BASE_INSTALL_DIRECTORY in ".."*) true;; *) false;; esac; then
			echo "Target directory does not exist. If you aren't familiar with relative paths, make sure that agbcc and the repository are in the same directory, and run \"./install.sh $1\" again."
		else
			if echo "$BASE_INSTALL_DIRECTORY" | grep -qE '^[^/]*.$'; then
				echo "Target directory does not exist. You probably meant to do \"./install.sh ../$1\", but agbcc and $1 do not exist in the same directory. Check your spelling, make sure that the repository has been cloned, ensure that agbcc and the repository are in the same directory, and run \"./install.sh ../$1\" again."
			else
				echo "Target directory does not exist. Check your spelling, re-read the instructions, and try again."
			fi
		fi
	fi
fi
