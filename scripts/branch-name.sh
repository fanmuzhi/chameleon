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

echo $branch
git submodule foreach --recursive "branch=$(git rev-parse --abbrev-ref HEAD); echo $branch;"


