/* -*- mode: asm; tab-width: 4 -*- */
/*
 * INTERNAL USE ONLY
 *
 * Copyright (c) 2013-2018 Synaptics Incorporated.  All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated ("Synaptics"). The holder of this file shall treat all
 * information contained herein as confidential, shall use the
 * information only for its intended purpose, and shall not duplicate,
 * disclose, or disseminate any of this information in any manner unless
 * Synaptics has otherwise provided express, written permission.
 */

/*
 * Setup to do main system RAM tests.
 *
 * Bjoren Davis, August 9, 2017 (SMYSJT).
 *
 * Potential improvements:
 *  + add checks to make sure everything actually fits into the caches
 *  + isolate-map the instruction cache (in addition to its normal
 *    mapping for execution) and put the stack there.  The advantage is
 *    that our instruction cache is 8 KiB and our data cache is only
 *    4 KiB, so there's a better chance of things fitting)
 *  + split out RAM copying into a subroutine to save a few bytes of code
 *    space.
 *  + experiement with running the code from the data cache.  Can
 *    that work? or will instructions only be fetched from the instruction
 *    cache.
 */

#include "ramtest.h"
#include "hwdef.h"
#include <xtensa/cacheasm.h>
#include <xtensa/cacheattrasm.h>

#define FRAME_SIZE      32
#define FRAME_PS        0
#define FRAME_RESULTP   4
#define FRAME_OLDSTACK  8
#define FRAME_TESTFUNCP 12
#define FRAME_RESULT    16      /* 12 bytes long */

#define BASEFRAME_SIZE  16      /* how big of a size to reserve *before*
                                 *  the current stack pointer for our
                                 *  caller's a0-a3 */

    /*
     * Copy data from one place to another 16-bytes at a time.
     */
    .macro  copy16  destp, srcp, term, tmp1, tmp2, termisdest=0
1:
    l32i    \tmp1, \srcp, 0
    l32i    \tmp2, \srcp, 4
    s32i    \tmp1, \destp, 0
    l32i    \tmp1, \srcp, 8
    s32i    \tmp2, \destp, 4
    l32i    \tmp2, \srcp, 12
    s32i    \tmp1, \destp, 8
    addi    \srcp, \srcp, 16
    s32i    \tmp2, \destp, 12
    addi    \destp, \destp, 16
    .if \termisdest
    bltu    \destp, \term, 1b
    .else
    bltu    \srcp, \term, 1b
    .endif

    .endm


    /*
     * sysramtest.  This is a C-callable function which
     *  sets up the CPU to do a RAM test, does the test, and then
     *  restores the CPU to its proper configuration.
     *
     * bool_t sysramtest(ramtest_result_t *resultp, unsigned int npasses,
     *                  bool_t (*testfuncp)(uint32_t *blkbasep,
     *                                      unsigned int nwords,
     *                                      unsigned int passno,
     *                                      ramtest_result_t *resultp));
     *
     * a2 = pointer to response
     * a3 = number of passes
     * a4 = pointer to C-callable memory test function
     *---
     * a5 = old PS
     * a6 = pass number
     * a7 = old stack pointer
     */
    .text
    .global sysramtest
    .type   sysramtest, @function
    .align  4
sysramtest:
    /*
     * The frame size is pedantic.  I'm allocating 48 + FRAME_SIZE because
     *  we're doing a call12.  However, the call12 should be guaranteed
     *  not to take our own registers out of currency, so we don't need
     *  to have space allocated for them to overflow into.  So really
     *  we should be able to do (16 + FRAME_SIZE).  But I just don't
     *  feel that confident in my understanding of Tensilica Windowed
     *  Registers.  Yes, and that's after 12 years of using the thing.
     */
    .frame  a1, (48 + FRAME_SIZE)
    entry       a1, (48 + FRAME_SIZE)

    /*
     * Now raise the processor priority level so that all
     *  interrupts are off.
     */
    rsil        a5, RAMTEST_INTLEVEL_INTR

    /*
     * Before we get going, save some things to the old stack.
     *  This will get copied into the new stack, too.
     */

    s32i        a5, a1, FRAME_PS
    s32i        a2, a1, FRAME_RESULTP
    s32i        a1, a1, FRAME_OLDSTACK
    s32i        a4, a1, FRAME_TESTFUNCP

    /*
     * Cause a windowed flush to bring all registers
     *  active.
     */
    call12      sysramtest_windowspill20

    /*
     * Register use at this point:
     *      a0 = return address (R/O)
     *      a1 = stack pointer (R/O)
     *      a2 = unused
     *      a3 = number of passes (R/O)
     */
    /*
     *
     * WARNING WARNING WARNING
     *
     * We're doing something really dangerous here.  We're turning
     *  on the isolated mapping of the DCACHE RAM *and* we're leaving
     *  the data cache active in the SROM space.  We need to leave
     *  the SROM caching on because we need to be able to fetch
     *  literals in our patch.  The danger is that the two uses
     *  will interfere with each other.
     *
     * Turn on the isolated DCACHE data to allow us to preserve our
     *  stack.  Note that we don't use the ROM code here because that
     *  would pollute the data cache with ROM-located literals that
     *  could mean that we could interfere with our isolated stack
     *  copy, below.
     */
#if ((RAMTEST_DCACHE_ATTR_ISO ^ RAMTEST_DCACHE_ATTR_ON) & 0xffff0f0f) != 0
    /*
     * This logic test checks to see if our attributes are changing
     *  in any other place besides the [2000_0000, 3fff_ffff] and
     *  [6000_0000, 7fff_ffff] mappings.
     *  This case is they are.
     */
    movi        a2, RAMTEST_DCACHE_ATTR_ISO
    movi        a4, 0xe0000000      /* the decrement */
    movi        a5, XCHAL_SPANNING_WAY      /* start at region 0 */
    memw

    /* Flush and invalidate the data caches.  Trashes registers a14, a15 */
    dcache_writeback_inv_all    a14, a15

    /* Loop through setting the data TLB attributes */
1:
    extui       a6, a2, 0, 4
    wdtlb       a6, a5
    sub         a5, a5, a4
    srli        a2, a2, 4
    bgeui       a5, 16, 1b
    dsync

#else
    /*
     * We're only changing the TLB for [2000_0000, 3fff_ffff] and
     *  the main mapping of [6000_0000, 7fff_ffff].  Much
     *  simpler.
     */
    movi        a5, (RAMTEST_DCD_MAPADDR + XCHAL_SPANNING_WAY)
    movi        a7, HWDEF_MEMMAP_SRAM_RAM_BASE
    movi        a6, ((RAMTEST_DCACHE_ATTR_ISO & 0x000000f0) >> 4)
    memw

    /* Flush and invalidate the data caches.  Trashes registers a14, a15 */
    dcache_writeback_inv_all    a14, a15

    wdtlb       a6, a5
    movi        a6, ((RAMTEST_DCACHE_ATTR_ISO & 0x0000f000) >> 12)
    wdtlb       a6, a7
    dsync

#endif

    /*
     * Now copy the entirety of the existing call stack, including
     *  the space for our immediate caller's a0-a3,
     *  into the high end of the isolated data cache.  This assumes
     *  the stack is a multiple of 16 bytes long.
     *  Note that we're copying from a now-*uncached* mapping of
     *  RAM into the isolated data cache RAM.
     */
    movi        a6, (HWDEF_MEMMAP_SRAM_RAM_BASE + HWDEF_MEMMAP_SRAM_RAM_SIZE)
                        /* a6 = terminus in source */
    movi        a4, (RAMTEST_DCD_MAPADDR + XCHAL_DCACHE_SIZE)
    sub         a7, a6, a1      /* Calculate the stack size */
    addi        a5, a1, -BASEFRAME_SIZE /* The source */
    sub         a1, a4, a7              /* Create the new stack pointer */
    addi        a4, a1, -BASEFRAME_SIZE /* And the destination */

sysramtest_stackpreserve_loop:
    copy16      a4, a5, a6, a7, a8

    /*
     * Once this copying is done we must be VERY careful not
     *  to pull anything into the data cache we don't need.  That's
     *  why calls to the ROM are prohibited (because they would
     *  pull ROM-based literals into the data cache) because we
     *  risk corrupting the isolated RAM we've just written to.
     */

    /*
     * Pull the entire patch from RAM (in the SROM mapping only)
     *  into the instruction and data caches.  The instruction cache
     *  is needed for instructions and the data cache is needed for
     *  literals.  Sadly, there does not appear to be any other
     *  way to allow us to execute from the instruction cache RAM.
     *  "Cache Isolated"  is 'rw-', not 'rwx'.
     */
    movi    a4, HWDEF_MEMMAP_SROM_RAM_BASE
    movi    a5, (_bss_end - HWDEF_MEMMAP_SROM_TO_SRAM_RAM_ADDEND)

#if XCHAL_ICACHE_LINESIZE == XCHAL_DCACHE_LINESIZE
sysramtest_idpfl_looptop:
    ipfl    a4, 0
    dpfl    a4, 0
    addi    a4, a4, XCHAL_DCACHE_LINESIZE
    bltu    a4, a5, sysramtest_idpfl_looptop

#else

    mov     a2, a4
sysramtest_ipfl_looptop:
    ipfl    a2, 0
    addi    a2, a2, XCHAL_ICACHE_LINESIZE
    bltu    a2, a5, sysramtest_ipfl_looptop

sysramtest_dpfl_looptop:
    dpfl    a4, 0
    addi    a4, a4, XCHAL_DCACHE_LINESIZE
    bltu    a4, a5, sysramtest_dpfl_looptop

#endif

    /*
     * We're all set to go.  Call back into the C code using the
     *  new stack.
     * We must use callx4 for this function call because
     *  we are at the top of a call stack.  If we used call8
     *  or call12 the WindowOverflow code would attempt to use
     *  our caller's stack frame, which isn't available.
     * Well, actually, the caller's stack frame *is* available, but
     *  the stack pointers in it point to the currently-under-test
     *  address range, so effectively it's not available.
     */

    movi        a2, 0   /* zero out the pass number */
sysramtest_test_looptop:
    movi        a6, HWDEF_MEMMAP_SRAM_RAM_BASE     /* blkbasep */
    movi        a7, (HWDEF_MEMMAP_SRAM_RAM_SIZE/4) /* nwords */
    mov         a8, a2                             /* passno */
    addi        a9, a1, FRAME_RESULT               /* result pointer */
    l32i        a4, a1, FRAME_TESTFUNCP

    /* Call the test function */
    callx4      a4

    /* Did the test fail? */
    beqz        a6, sysramtest_test_done

    /* Increment the pass number */
    addi        a2, a2, 1
    bltu        a2, a3, sysramtest_test_looptop

sysramtest_test_done:
    mov         a2, a6          /* preserve the return value */
    /*
     * Registers we care about at this point:
     *      a1  - pointer to bottom of stack in mapped DCACHE RAM
     *      a2  - return value from final call to test function
     * All other registers are clobberable.
     */

    /*
     * Copy the patch back into main RAM.  We do this before
     *  we turn on the data cache so as not to destroy our
     *  stack in the isolated DCACHE.
     */
    movi        a4, HWDEF_MEMMAP_SRAM_RAM_BASE
                    /* destination: beginning of DRAM-mapped RAM */
    movi        a5, HWDEF_MEMMAP_SROM_RAM_BASE
                    /* source: DCACHE-located SROM-mapping of RAM */
    movi        a6, (_bss_end - HWDEF_MEMMAP_SROM_TO_SRAM_RAM_ADDEND)
                    /* terminus: in source space */

sysramtest_patchrestore_loop:
    copy16      a4, a5, a6, a7, a8

    /*
     * Copy the stack back into RAM.  This assumes the stack is a
     *  multiple of 16 bytes.
     */

    addi        a5, a1, -BASEFRAME_SIZE /* the source */
    l32i        a1, a1, FRAME_OLDSTACK  /* the base pointer of the old stack */
    movi        a6, (RAMTEST_DCD_MAPADDR + XCHAL_DCACHE_SIZE)
                                /* the terminus in the source */
    addi        a4, a1, -BASEFRAME_SIZE /* the destination */

sysramtest_stackrestore_loop:
    copy16      a4, a5, a6, a7, a8

    /*
     * Now copy the result from the beginning of the mapped DCACHE
     *  RAM into the pointer we've been handed.  This is hard-coded
     *  to know the result is 3 DWORDs long.  Note that we do this
     *  after having restored the patch RAM and after having restored
     *  the stack because the pointer we're writing to could be in
     *  either.
     */
    l32i        a3, a1, FRAME_RESULTP
    l32i        a4, a1, (FRAME_RESULT+0)
    l32i        a5, a1, (FRAME_RESULT+4)
    s32i        a4, a3, 0
    l32i        a4, a1, (FRAME_RESULT+8)
    s32i        a5, a3, 4
    s32i        a4, a3, 8

    /*
     * Unlock the instruction cache lines.  We do this so
     *  the instruction cache is returned to its former operation.
     */

    icache_unlock_all   a4, a5      /* unlock the instruction cache */
    /*
     * Re-activate the data cache the way it's supposed to be.
     *  Note that the data cache should have nothing in it we care
     *  about.  All data should have been written to the stack
     *  located in the isolated DCACHE RAM.  The only stuff
     *  in the data cache should be the SROM mapping of the RAM
     *  (for patch literals).  So we're fine doing dcache_reset.
     *  And then using the ROM's code to set the DTLB.
     */

#if ((RAMTEST_DCACHE_ATTR_ISO ^ RAMTEST_DCACHE_ATTR_ON) & 0xffff0f0f) != 0
    /*
     * This logic test checks to see if our attributes are changing
     *  in any other place besides the [2000_0000, 3fff_ffff] and
     *  [6000_0000, 7fff_ffff] mappings.
     *  This case is they are.
     */
    movi        a3, RAMTEST_DCACHE_ATTR_ON
    movi        a4, 0xe0000000      /* the decrement */
    movi        a5, XCHAL_SPANNING_WAY      /* start at region 0 */
    memw

    dcache_reset    a6, a7

    /* Loop through setting the data TLB attributes */
1:
    extui       a6, a3, 0, 4
    wdtlb       a6, a5
    sub         a5, a5, a4
    srli        a3, a3, 4
    bgeui       a5, 16, 1b
    dsync

#else
    /*
     * We're only changing the TLB for [2000_0000, 3fff_ffff] and
     *  the main mapping of [6000_0000, 7fff_ffff].  Much
     *  simpler.
     */
    movi        a3, (RAMTEST_DCD_MAPADDR + XCHAL_SPANNING_WAY)
    movi        a4, HWDEF_MEMMAP_SRAM_RAM_BASE
    movi        a5, ((RAMTEST_DCACHE_ATTR_ISO & 0x000000f0) >> 4)
    memw

    dcache_reset    a6, a7

    wdtlb       a5, a3
    movi        a5, ((RAMTEST_DCACHE_ATTR_ISO & 0x0000f000) >> 12)
    wdtlb       a5, a4
    dsync

#endif

    /*
     * Turn interrupts back on again.
     */
    l32i        a5, a1, FRAME_PS
    wsr.ps      a5
    rsync

    retw
    .size   sysramtest, . - sysramtest


    /* ------------------------------------------------------------ */
    /*
     * sysramtest_windowspill20
     * Spill 20 registers.
     */

    .type sysramtest_windowspill20, @function
    .align  4
sysramtest_windowspill20:
    entry   a1, 16
    call4   sysramtest_windowspill16
    retw
    .size sysramtest_windowspill20, . - sysramtest_windowspill20

    /* ------------------------------------------------------------ */
    /*
     * sysramtest_windowspill16
     * Spill 16 registers.
     */

    .type sysramtest_windowspill16, @function
    .align  4
sysramtest_windowspill16:
    entry   a1, 16
    movi    a15, 0
    retw
    .size sysramtest_windowspill16, . - sysramtest_windowspill16
