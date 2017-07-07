#!/bin/bash

#
# Usage: ./scripts/branch-atag.sh <tag_name> <tag_message>
#

if [ "$1" == "" ]; then
    echo "Create a new annotated tag on the current working branch"
    echo "USAGE  : $0 <tag_name> <tag_message>"
    echo "example: $0 feature/new_work_20170601 \"tagging message\""
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

echo "About to create tag: $1"
pause

# Tag each submodule
git submodule foreach --recursive "git tag -a $1 -m \\\"$2\\\"; git push"

# Tag the superproject
git tag -a $1 -m \"$2\"
git push

# Tag the cmakescripts repo
#git -C ../cmakescripts tag -a $1 -m \"$2\"
#git -C ../cmakescripts push

# Tag the include repo
#git -C ../../include tag -a $1 -m \"$2\"
#git -C ../../include push

# Tag the pubkeys repo
#git -C ../../pubkeys tag -a $1 -m \"$2\"
#git -C ../../pubkeys push

# Tag the external repo
#git -C ../external tag -a $1 -m \"$2\"
#git -C ../external push

# Tag the Linuxspidrv repo
#git -C ../drivers/Linuxspidrv tag -a $1 -m \"$2\"
#git -C ../drivers/Linuxspidrv push
