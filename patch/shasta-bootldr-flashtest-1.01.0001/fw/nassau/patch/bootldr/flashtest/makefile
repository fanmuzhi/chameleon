# -*- mode: makefile -*-
#
# Makefile for building patches to test flash
#
# Matt Waldron, December 1, 2015 - adapted from Bjoren's noop makefile
#

BOOTLDRPATCHNAME=flashtest
SOURCES_C=patch_start.c cmdmgr.c nvm_prog.c
NASSAU_ROOT=../../..
PATCH_VMAJOR=1
PATCH_VMINOR=0
PATCH_BUILDNUM=1

include $(NASSAU_ROOT)/patch/bootldr/makefile.bootldrpatch

CFLAGS+=-DPATCH_VMAJOR=$(PATCH_VMAJOR) -DPATCH_VMINOR=$(PATCH_VMINOR) \
     -DPATCH_BUILDNUM=$(PATCH_BUILDNUM)

vpath cmdmgr.c $(NASSAU_ROOT)/patch/common/cmdmgr
INCLUDES += -I$(NASSAU_ROOT)/patch/common/cmdmgr

# Provide support for older parts with CMDMGR_OPT_DIRSPI
cmdmgr.o: CFLAGS += -DCMDMGR_OPT_CMDSET -DCMDMGR_OPT_DIRSPI \
	-DCMDMGR_MAXREPLY='((50*1024)+10)' -DCMDMGR_MAXCMD=64

vpath nvm_prog.c $(NASSAU_ROOT)/patch/common/nvm_prog

INCLUDES += -I$(NASSAU_ROOT)/patch/common/nvm_prog
