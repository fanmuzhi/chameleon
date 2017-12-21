#!/bin/bash

#
# Usage: ./scripts/branch-status.sh
#



if [ "$1" != "" ]; then
    echo "reset merging"
    echo "USAGE  : $0"
    echo "example: $0"
	exit
fi

# make sure current working directory is chameleon
cwd=$(basename $(pwd))

if [ "$cwd" != "chameleon" ]; then
    echo "Please run the scrip from chameleon directory"
	echo "Current working directory: $cwd"
	exit
fi

branch=$(git rev-parse --abbrev-ref HEAD)
echo ----------------------------------
echo "Branch:"$branch
echo ----------------------------------
echo

git submodule foreach --recursive '
	branchMain="$(git -C $toplevel symbolic-ref --short HEAD)"
	branchSubmodule="$(git rev-parse --abbrev-ref HEAD)"
	echo ----------------------------------
	echo "Branch:"$branchSubmodule
	echo ----------------------------------	
	if [ $branchMain != $branchSubmodule ]; then
		echo "Warning!! Submodule is different branch now!" 
		echo 
	fi
	echo 
'


