#!/bin/bash

#
# Usage: ./scripts/branch-delete.sh <branch_name>
#

if [ "$1" == "" ]; then
    echo "Delete a branch both locally and on remote"
    echo "USAGE  : $0 <branch_name>"
    echo "example: $0 mt/feature/new_work_20170601"
	exit
fi

curr_branch=$(git rev-parse --abbrev-ref HEAD)

if [ "$1" == "$curr_branch" ]; then
    echo "Cannot delete current branch"
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

echo "About to delete branch: $1 !!!"
pause

# delete the branch in each submodule
git submodule foreach --recursive "git branch -d $1; git push origin :$1"

# delete local branch
git branch -d $1
# delete branch on remote
git push origin :$1
