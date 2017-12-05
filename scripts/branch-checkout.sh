#!/bin/bash

#
# Usage: ./scripts/branch-checkout.sh <branch_name>
#

if [ "$1" == "" ]; then
    echo "Switch to an existing branch"
    echo "USAGE  : $0 <branch_name>"
    echo "example: $0 develop"
	exit
fi

# make sure current working directory is niteCore
cwd=$(basename $(pwd))

if [ "$cwd" != "chameleon" ]; then
    echo "Please run the scrip from chameleon directory"
	echo "Current working directory: $cwd"
	exit
fi

echo "Switching to branch: $1"
git checkout $1
git pull
git submodule foreach --recursive 'branch="$(git -C $toplevel symbolic-ref --short HEAD)"; git checkout $branch; git pull;'
