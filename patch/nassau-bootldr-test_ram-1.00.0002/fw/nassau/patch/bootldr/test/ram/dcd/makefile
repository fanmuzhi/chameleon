# -*- mode: makefile -*-
#
# Makefile for building a sample boot loader patch
#
# Bjoren Davis, June 19, 2015.
#

BOOTLDRPATCHNAME=test_ram_dcd
SOURCES_C=patch_start.c ramtest_$(RAMTEST_ALGORITHM).c
SOURCES_S=dcdsetup.s
NASSAU_ROOT=../../../../..
PATCH_SRCDIR=.

include $(PATCH_SRCDIR)/../makefile.common

include $(NASSAU_ROOT)/patch/bootldr/makefile.bootldrpatch

CFLAGS+=$(RAMTEST_CFLAGS)
INCLUDES+=$(RAMTEST_INCLUDES)


#
# We can't use the sdcw/ldcw instructions because they're not
#  implemented on validity_d212.
#ramtest_$(RAMTEST_ALGORITHM).o: CFLAGS += -DRAMTEST_OPT_DCDATA








