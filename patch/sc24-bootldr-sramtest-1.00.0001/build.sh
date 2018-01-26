#!/bin/sh
set -ex
unset TZ TERM LANG
pwd
date
## Remote repository for release:
#GITROOT=ssh://gitms@git.synaptics.com/git/biometrics
#RELTAG=fw/nassau_patch_bootldr_flasherase/sc24-1.02.0012
#REPS="b2224/common b2224/tools fw/include fw/nassau/patch/bootldr fw/nassau/patch/common fw/nassau/common include tools"
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
cd fw/nassau/patch/bootldr/sram
make envid
time make sc24
date 

cp "obj\sc24-rom\sramtest.blpbin" "..\..\..\..\..\..\sc24_sramtest.blpbin" 
cp "obj\sc24-rom\sramtest.blxepbin" "..\..\..\..\..\..\sc24_sramtest.blxepbin" 

