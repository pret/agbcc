#!/bin/sh
set -e
if [ "$1" != "" ]; then
	if [ -d "$1" ]; then
		mkdir -p $1/tools/agbcc
		mkdir -p $1/tools/agbcc/bin
		mkdir -p $1/tools/agbcc/include
		mkdir -p $1/tools/agbcc/lib
		cp agbcc $1/tools/agbcc/bin/
		cp old_agbcc $1/tools/agbcc/bin/
		cp agbcc_arm $1/tools/agbcc/bin/
		cp -R libc/include $1/tools/agbcc/ #drop include, because we don't want include/include
		cp ginclude/* $1/tools/agbcc/include/
		cp libgcc.a $1/tools/agbcc/lib/
		cp libc.a $1/tools/agbcc/lib/
		echo "agbcc successfully installed!"
	else
		if [ -d "../$1" ]; then
			echo "Target directory does not exist. Did you mean to do \"./install.sh ../$1\"?"
		else
			if case $1 in ".."*) true;; *) false;; esac; then
				echo "Target directory does not exist. If you aren't familiar with relative paths, make sure that agbcc and the repository are in the same directory, and run \"./install.sh $1\" again."
			else
				if echo "$1" | grep -qE '^[^/]*.$'; then
					echo "Target directory does not exist. You probably meant to do \"./install.sh ../$1\", but agbcc and $1 do not exist in the same directory. Check your spelling, make sure that the repository has been cloned, ensure that agbcc and the repository are in the same directory, and run \"./install.sh ../$1\" again."
				else
					echo "Target directory does not exist. Check your spelling, re-read the instructions, and try again."
				fi
			fi
		fi
	fi
else
	echo "Usage: install.sh PATH"
fi
