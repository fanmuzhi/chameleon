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
 * Setup to do data cache RAM test.
 * Adapted from /test/engineering/private/impl/micro/windsor/patch/common/diag/dcram/diag_dcram_asm.s,v 1.4 2015/03/03 04:12:22 bdavis
 *
 * Bjoren Davis, August 9, 2017 (SMYSJT).
 */

#include "ramtest.h"
#include <xtensa/cacheasm.h>
#include <xtensa/cacheattrasm.h>



    /*
     * dcdsetup.  This is a C-callable function which
     *  sets up the CPU to do a RAM test, does the test, and then
     *  restores the CPU to its proper configuration.
     *
     * bool_t dcdsetup(ramtest_result_t *resultp, unsigned int npasses,
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
    .global dcdsetup
    .type   dcdsetup, @function
    .align  4
dcdsetup:
    .frame  a1, 48
    entry       a1, 48

    /*
     * Next, cause a windowed flush to bring all registers
     *  active.
     */
    call12      dcdsetup_windowspill20

    /*
     * Now raise the processor priority level so that all
     *  interrupts are off.
     */
    rsil        a5, RAMTEST_INTLEVEL_INTR

    /*
     * Flush the data cache and turn off all data caching.
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
    movi        a8, RAMTEST_DCD_MAPADDR
                                    /* where we'll play with the dcache */
    memw

dcdsetup_dcache_flush:
    /* Flush and invalidate the data caches.  Trashes registers a11 and a12 */
    dcache_writeback_inv_all    a11, a12

dcdsetup_dcache_off:
    /*
     * Turn off ALL data caching, including for reads from ROM.
     */
1:
    wdtlb       a6, a10
    add         a10, a10, a7
    bnez        a10, 1b
    dsync

    /*
     * Turn on the data cache ONLY for the area [2000_0000, 3fff_ffff]
     * We'll use that mapping in "isolated" mode.
     */
    wdtlb       a9, a8
    dsync

    /*
     * At this point we should be able to test the entire
     *  data cache RAM (4 KB), aliased through the entire
     *  [2000_0000, 3fff_ffff] space.
     */

    movi        a6, 0   /* zero out the pass number */
dcdsetup_test_looptop:
    movi        a10, RAMTEST_DCD_MAPADDR
    movi        a11, (XCHAL_DCACHE_SIZE/4)
    mov         a12, a6
    mov         a13, a2

    /* Call the test function */
    callx8      a4

    /* Did the test fail? */
    beqz        a10, dcdsetup_test_done

    /* Increment the pass number */
    addi        a6, a6, 1
    bltu        a6, a3, dcdsetup_test_looptop

dcdsetup_test_done:
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
    .size   dcdsetup, . - dcdsetup


    /* ------------------------------------------------------------ */
    /*
     * dcdsetup_windowspill20
     * Spill 20 registers.
     */

    .type dcdsetup_windowspill20, @function
    .align  4
dcdsetup_windowspill20:
    entry   a1, 16
    call4   dcdsetup_windowspill16
    retw
    .size dcdsetup_windowspill20, . - dcdsetup_windowspill20

    /* ------------------------------------------------------------ */
    /*
     * dcdsetup_windowspill16
     * Spill 16 registers.
     */

    .type dcdsetup_windowspill16, @function
    .align  4
dcdsetup_windowspill16:
    entry   a1, 16
    movi    a15, 0
    retw
    .size dcdsetup_windowspill16, . - dcdsetup_windowspill16
