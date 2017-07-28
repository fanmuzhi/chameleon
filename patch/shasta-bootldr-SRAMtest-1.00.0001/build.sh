#!/bin/sh
set -ex
unset TZ TERM LANG
pwd
date
## Remote repository for release:
#GITROOT=ssh://gitms@git.synaptics.com/git/biometrics
#RELTAG=fw/nassau_patch_bootldr_flasherase/shasta-1.01.0006
#REPS="b1216/common b1216/tools fw/include fw/nassau/patch/bootldr fw/nassau/patch/common fw/nassau/common include tools"
#rm -rf $REPS
#for d in $REPS ; do
#    mkdir -p `dirname $d`
#    # using the .git suffix here for remote branch
#    (cd `dirname $d` ; git --no-pager clone -q $GITROOT/${d}.git)&
#done
#wait
#for d in $REPS ; do
#   (cd $d ; git --no-pager checkout -q $RELTAG)
#done
#find . -depth -type d -name .git -exec rm -rf {} \;
#find . -depth -type d -exec rmdir {} \; 2>/dev/null
cd fw/nassau/patch/bootldr/SRAMtest
make envid
time make shasta
date 

cp "obj\shasta-rom\sramtest.blpbin" "..\..\..\..\..\..\shasta_ramtest.blpbin" 
cp "obj\shasta-rom\sramtest.blxepbin" "..\..\..\..\..\..\shasta_ramtest.blxepbin" 
