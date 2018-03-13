#!/bin/sh
set -ex
unset TZ TERM LANG
pwd
date
GITROOT=ssh://gitms@git.synaptics.com/git/biometrics
BRTAG=fw/nassau_patch_bootldr_test_ram/nassau-1.00.0002
RELTAG=origin/fw/nassau_patch_bootldr_test_ram/nassau-1.00.0002
REPS="b1210/common b1210/tools b1212/common b1212/tools b1216/common b1216/tools b2224/common b2224/tools fw/include fw/nassau/common fw/nassau/patch/bootldr include tools"
rm -rf $REPS
for d in $REPS ; do
    mkdir -p `dirname $d`
    (cd `dirname $d` ; git --no-pager clone --depth 1 --branch $BRTAG -q $GITROOT/${d}.git)&
done
wait
for d in $REPS ; do
    (cd $d ; git --no-pager checkout -q $RELTAG)
done
for d in $REPS ; do
    (cd $d ; echo -n `git config --get remote.origin.url` ; echo -n ' ' ; git rev-parse HEAD) | tee -a repos_commits.txt
done
for d in $REPS ; do
    (cd $d ; rm -rf .git)
done
find . -depth -type d -exec rmdir {} \; 2>/dev/null
cd fw/nassau/patch/bootldr/test/ram
make -f ../../../../common/makefile.envid
time make
date
