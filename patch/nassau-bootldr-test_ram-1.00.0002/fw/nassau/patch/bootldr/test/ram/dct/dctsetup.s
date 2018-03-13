/* -*- mode: asm; tab-width: 4 -*- */
/*
 * INTERNAL USE ONLY
 *
 * Copyright (c) 2013-2017 Synaptics Incorporated.  All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated ("Synaptics"). The holder of this file shall treat all
 * information contained herein as confidential, shall use the
 * information only for its intended purpose, and shall not duplicate,
 * disclose, or disseminate any of this information in any manner unless
 * Synaptics has otherwise provided express, written permission.
 */

/*
 * Setup to do data cache tag RAM test.
 * Adapted from /test/engineering/private/impl/micro/windsor/patch/common/diag/dcram/diag_dcram_asm.s,v 1.4 2015/03/03 04:12:22 bdavis
 *
 * Bjoren Davis, August 9, 2017 (SMYSJT).
 */

#include "ramtest.h"
#include <xtensa/cacheasm.h>
#include <xtensa/cacheattrasm.h>


    /*
     * dctsetup.  This is a C-callable function which
     *  sets up the CPU to do a RAM test, does the test, and then
     *  restores the CPU to its proper configuration.
     *
     * bool_t dctsetup(ramtest_result_t *resultp, unsigned int npasses,
     *               bool_t (*testfuncp)(uint32_t *blkbasep,
     *                                   unsigned int nwords,
     *                                   unsigned int passno,
     *                                   ramtest_result_t *resultp)) ;
     *
     * a2 = pointer to response
     * a3 = number of passes
     * a4 = pointer to C-callable memory test function
     * a5 = old PS
     * a6 = pass number
     */
    .text
    .global dctsetup
    .type   dctsetup, @function
    .align  4
dctsetup:
    .frame  a1, 48
    entry       a1, 48

    /*
     * Next, cause a windowed flush to bring all registers
     *  active.
     */
    call12      dctsetup_windowspill20

    /*
     * Now raise the processor priority level so that all
     *  interrupts are off.
     */
    rsil        a5, RAMTEST_INTLEVEL_INTR

    /*
     * Flush the data cache and0xfffff80f turn off all data caching.
     *  Register use at this point:
     *      a0 = return address (R/O)
     *      a1 = stack pointer (R/O)
     *      a2 = pointer to storage block (R/O)
     *      a3 = number of passes (R/O)
     *      a4 = pointer to C-callable memory test function (R/O)
     *      a5 = old PS (R/O)
     */
    /* Load literals (from memory) before invalidating cache */
    movi        a9, 14      /* cache isolated */
    movi        a6, 2       /* bypass cache */
    movi        a10, 0
    movi        a7, 0x20000000      /* the increment */
    movi        a8, RAMTEST_DCT_PSEUDOADDR
                                    /* where we'll play with the dcache */
    memw

dctsetup_dcache_flush:
    /* Flush and invalidate the data caches.  Trashes registers a11 and a12 */
    dcache_writeback_inv_all    a11, a12

dctsetup_dcache_off:
    /*
     * Turn off ALL data caching, including for reads from ROM.
     */
1:
    wdtlb       a6, a10
    add         a10, a10, a7
    bnez        a10, 1b
    dsync

    /*
     * At this point we should be able to test the entire data cache.
     */

    movi        a6, 0   /* zero out the pass number */
dctsetup_test_looptop:
    movi        a10, RAMTEST_DCT_PSEUDOADDR
    movi        a11, (XCHAL_DCACHE_SIZE / XCHAL_DCACHE_LINESIZE)
    mov         a12, a6
    mov         a13, a2

    /* Call the test function */
    callx8      a4

    /* Did the test fail? */
    beqz        a10, dctsetup_test_done

    /* Increment the pass number */
    addi        a6, a6, 1
    bltu        a6, a3, dctsetup_test_looptop

dctsetup_test_done:
    /* Unlock & Invalidate the data caches.  Trashes a11 and a12 */
    dcache_reset    a11, a12

    /* Preserve the PS */
    mov         a11, a5

    /*
     * Re-activate the data cache the way they're supposed to be.
     */

    movi        a2, RAMTEST_DCACHE_ATTR_ON
    dcacheattr_set      /* clobbers a3-a8 */

    /*
     * Turn interrupts back on again.
     */
    wsr.ps      a11
    rsync

    /* Return the status */
    mov         a2, a10

    retw
    .size   dctsetup, . - dctsetup


    /* ------------------------------------------------------------ */
    /*
     * dctsetup_windowspill20
     * Spill 20 registers.
     */

    .type dctsetup_windowspill20, @function
    .align  4
dctsetup_windowspill20:
    entry   a1, 16
    call4   dctsetup_windowspill16
    retw
    .size dctsetup_windowspill20, . - dctsetup_windowspill20

    /* ------------------------------------------------------------ */
    /*
     * dctsetup_windowspill16
     * Spill 16 registers.
     */

    .type dctsetup_windowspill16, @function
    .align  4
dctsetup_windowspill16:
    entry   a1, 16
    movi    a15, 0
    retw
    .size dctsetup_windowspill16, . - dctsetup_windowspill16
