# -*- mode: makefile -*-
#
# Makefile for building a sample boot loader patch
#
# Bjoren Davis, June 19, 2015.
#
BOOTLDRPATCHNAME=wofplot
SOURCES_C=patch_start.c cmdmgr.c crc32.c
NASSAU_ROOT=../../..
PATCH_VMAJOR=1
PATCH_VMINOR=0
PATCH_BUILDNUM=3
PATCH_ID=2

ALL_TARGETS=rom fpga

include $(NASSAU_ROOT)/patch/bootldr/makefile.bootldrpatch

CFLAGS+=-DPATCH_VMAJOR=$(PATCH_VMAJOR) -DPATCH_VMINOR=$(PATCH_VMINOR) \
     -DPATCH_BUILDNUM=$(PATCH_BUILDNUM) -DPATCH_ID=$(PATCH_ID)

vpath cmdmgr.c $(NASSAU_ROOT)/patch/common/cmdmgr
INCLUDES += -I$(NASSAU_ROOT)/patch/common/cmdmgr

# Provide support for older parts with CMDMGR_OPT_DIRSPI
cmdmgr.o: CFLAGS += -DCMDMGR_OPT_CMDSET -DCMDMGR_OPT_DIRSPI \
	-DCMDMGR_MAXREPLY=32 -DCMDMGR_MAXCMD='((50*1024)+64)'

vpath nvm_prog.c $(NASSAU_ROOT)/patch/common/nvm_prog/
INCLUDES += -I$(NASSAU_ROOT)/patch/common/nvm_prog/

vpath crc32.c $(NASSAU_ROOT)/patch/common/crc/
INCLUDES += -I$(NASSAU_ROOT)/patch/common/crc/
