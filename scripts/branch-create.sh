#!/bin/bash

#
# Usage: ./scripts/branch-create.sh <branch_name>
#

if [ "$1" == "" ]; then
    echo "Create a new branch from the current working tree"
    echo "USAGE  : $0 <branch_name>"
    echo "example: $0 feature/new_work_20170601"
	exit
fi

# make sure current working directory is chameleon
cwd=$(basename $(pwd))

if [ "$cwd" != "chameleon" ]; then
    echo "Please run the scrip from chameleon directory"
	echo "Current working directory: $cwd"
	exit
fi

pause(){
 read -n1 -rsp $'Press any key to continue or Ctrl+C to exit...\n'
}

echo "About to create branch: $1"
pause

git checkout -b $1 $2
git submodule foreach --recursive 'branch="$(git -C $toplevel symbolic-ref --short HEAD)"; git checkout -b $branch $2; git push --set-upstream origin $branch'
git push --set-upstream origin $1
