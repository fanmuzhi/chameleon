# -*- mode: makefile -*-
#
# Makefile for building a sample boot loader patch
#
# Bjoren Davis, June 19, 2015.
#

BOOTLDRPATCHNAME=test_ram_ict
SOURCES_C=patch_start.c ramtest_$(RAMTEST_ALGORITHM).c
NASSAU_ROOT=../../../../..
PATCH_SRCDIR=.

include $(PATCH_SRCDIR)/../makefile.common

include $(NASSAU_ROOT)/patch/bootldr/makefile.bootldrpatch

CFLAGS+=$(RAMTEST_CFLAGS)
INCLUDES+=$(RAMTEST_INCLUDES)

XCHAL_ICACHE_LINESIZE = 32

ramtest_$(RAMTEST_ALGORITHM).o: CFLAGS += -DRAMTEST_OPT_ICTAG \
	-DRAMTEST_VALIDMASK=0xfffff007 \
	-DRAMTEST_STRIDE=$(XCHAL_ICACHE_LINESIZE)








