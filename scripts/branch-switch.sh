#!/bin/bash

#
# Usage: ./scripts/branch-switch.sh <branch_name>
#

if [ "$1" == "" ]; then
    echo "Switch to an existing branch without update"
    echo "USAGE  : $0 <branch_name>"
    echo "example: $0 develop"
	exit
fi

# make sure current working directory is niseCore
cwd=$(basename $(pwd))
echo "Current working directory: $cwd"

if [ "$cwd" != "chameleon" ]; then
    echo "Please run the scrip from chameleon directory"
	echo "Current working directory: $cwd"
	exit
fi

echo "Switching to branch: $1"
git checkout $1
git submodule foreach --recursive 'branch="$(git -C $toplevel symbolic-ref --short HEAD)"; git checkout $branch; : ;'
