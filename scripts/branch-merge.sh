#!/bin/bash

#
# Usage: ./scripts/branch-merge.sh
#
branch=$(git rev-parse --abbrev-ref HEAD)


if [ "$1" == "" ]; then
    echo "Merge local code from existing branch"
    echo "USAGE  : $0 <branch_name>"
    echo "example: $0 mt/feature/new_work_20170601"
	exit
fi

# make sure current working directory is chameleon
cwd=$(basename $(pwd))

if [ "$cwd" != "chameleon" ]; then
    echo "Please run the scrip from chameleon directory"
	echo "Current working directory: $cwd"
	exit
fi

git merge --no-ff $1
git submodule foreach --recursive "git merge --no-ff $1"
