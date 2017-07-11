#!/bin/bash

#
# Usage: ./scripts/branch-status.sh
#
branch=$(git rev-parse --abbrev-ref HEAD)


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

#TODO: Add to check wheather the branch path is same or not. 
git status
#git submodule foreach --recursive 'git status;branch="$(git rev-parse --abbrev-ref HEAD)";echo $branch; echo $strString;echo $path'
git submodule foreach --recursive 'git status;'
