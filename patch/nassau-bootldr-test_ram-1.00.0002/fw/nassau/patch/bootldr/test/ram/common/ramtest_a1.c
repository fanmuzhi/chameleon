/* -*- mode: c; tab-width: 4 -*- */

/*
 * INTERNAL USE ONLY
 *
 * Copyright (c) 2017-2018 Synaptics Incorporated.  All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated ("Synaptics"). The holder of this file shall treat all
 * information contained herein as confidential, shall use the
 * information only for its intended purpose, and shall not duplicate,
 * disclose, or disseminate any of this information in any manner unless
 * Synaptics has otherwise provided express, written permission.
 */

/*
 * RAM test, algorithm 1.
 *
 * This exercises a block of RAM using the algorithm derived
 *  from the simulation of the RAM BIST back in the Metallica/Viper days.
 *
 * The algorithm used to test each RAM is derived from the BIST RAM test
 * used on Falcon/Raptor.
 *
 * Here is the original algorithm (described in an email from Greg Dean
 * "RE: Calibration Failure and Rescreening", Wed 10/2/2013 4:12 PM EDT):
 *
 *     I ran the Raptor Memory BIST simulation and confirmed that it
 *     follows the same basic algorithm as Falcon, which I sent earlier.
 *
 *     Here's the actual algorithm for the main memory.  The algorithm
 *     for the USB and SCM memories is equivalent.
 *
 *     for each word (starting at addr=0)
 *     write to all 0 (2 times)
 *
 *     for each word (starting at addr=0)
 *     read all 0 (3 times)
 *     write to all 1 (2 times)
 *     read all 1 (3 times)
 *
 *     for each word (starting at addr=0)
 *     read all 1 (3 times)
 *     write to all 0 (2 times)
 *     read all 0 (3 times)
 *
 *     for each word (starting at addr=12k)
 *     read all 0 (3 times)
 *     write to all 1 (2 times)
 *     read all 1 (3 times)
 *
 *     for each word (starting at addr=12k)
 *     read all 1 (3 times)
 *     write to all 0 (2 times)
 *     read all 0 (3 times)
 *
 *     for each word (starting at addr=12k)
 *     read all 0 (3 times)
 *
 *     The address sequences are slightly swizzled.  Instead
 *     0,1,2,3,etc., it uses 0,1,3,2,etc.
 *
 *
 * For our purposes, we don't bother with the "address swizzling"
 * described at the bottom, and the high address depends on the RAM being
 * tested.
 *
 * Bjoren Davis, August 7, 2017.
 */

/*
 * General rules for RAM tests functions
 *
 * Because the test function may have to run  entirely within the
 *  instruction cache, it must not refer to any statically-allocated
 *  data, nor may it call into the bootldr ROM because we must leave
 *  the data cache active over the SROM mappings of things and so
 *  using ROM functions could pull data into the data cache that
 *  could overwrite our isolated copy.
 */
#include "basetypes.h"
#include "ramtest.h"

#if defined(RAMTEST_LOOPUNROLL8) && defined(RAMTEST_STRIDE)
#   error "Can't defined both RAMTEST_LOOPUNROLL8 and RAMTEST_STRIDE"
#endif

/*
 * Perform a RAM test on the given block.  Returns TRUE if the
 *  test is successful, FALSE otherwise.  On failure it also
 *  updates the structure pointed to by resultp.
 */
bool_t
ramtest_a1(uint32_t *blkbasep, unsigned int nwords, unsigned int passno,
           ramtest_result_t *resultp)
{
    unsigned int wordsleft, loopcount;
    uint32_t    *testp, expectval, writeval, val1, val2, val3;
#if defined(RAMTEST_VALIDMASK)
    uint32_t    validmask;
#endif

    /* Write all locations to 0, twice */
#if defined(RAMTEST_LOOPUNROLL8)
    wordsleft = nwords / 8;
#else
    wordsleft = nwords;
#endif
    testp = blkbasep;

    writeval = 0;
    while (wordsleft != 0) {
#if defined(RAMTEST_LOOPUNROLL8)
        /*
         * We blindly assume the overall size is a multiple of
         *  the unrolled loop size (8).
         */
        RAMTEST_WRITE32(testp, 0*sizeof(uint32_t), writeval);
        RAMTEST_WRITE32(testp, 0*sizeof(uint32_t), writeval);
        RAMTEST_WRITE32(testp, 1*sizeof(uint32_t), writeval);
        RAMTEST_WRITE32(testp, 1*sizeof(uint32_t), writeval);
        RAMTEST_WRITE32(testp, 2*sizeof(uint32_t), writeval);
        RAMTEST_WRITE32(testp, 2*sizeof(uint32_t), writeval);
        RAMTEST_WRITE32(testp, 3*sizeof(uint32_t), writeval);
        RAMTEST_WRITE32(testp, 3*sizeof(uint32_t), writeval);
        RAMTEST_WRITE32(testp, 4*sizeof(uint32_t), writeval);
        RAMTEST_WRITE32(testp, 4*sizeof(uint32_t), writeval);
        RAMTEST_WRITE32(testp, 5*sizeof(uint32_t), writeval);
        RAMTEST_WRITE32(testp, 5*sizeof(uint32_t), writeval);
        RAMTEST_WRITE32(testp, 6*sizeof(uint32_t), writeval);
        RAMTEST_WRITE32(testp, 6*sizeof(uint32_t), writeval);
        RAMTEST_WRITE32(testp, 7*sizeof(uint32_t), writeval);
        RAMTEST_WRITE32(testp, 7*sizeof(uint32_t), writeval);

        testp += 8;
#else       /* defined(RAMTEST_LOOPUNROLL8) */
        RAMTEST_WRITE32(testp, 0, writeval);
        RAMTEST_WRITE32(testp, 0, writeval);
#   if defined(RAMTEST_STRIDE)
        testp += (RAMTEST_STRIDE / sizeof(uint32_t));
#   else
        testp++;
#   endif
#endif  /* !defined(RAMTEST_LOOPUNROLL8) */
        wordsleft--;
    }

#if defined(RAMTEST_NEED_FLUSH_MEMORY)
#   pragma flush_memory
#endif

#if defined(RAMTEST_VALIDMASK)
    validmask = RAMTEST_VALIDMASK;
#endif

    for (loopcount = 4; loopcount != 0; loopcount--) {
        wordsleft = nwords;
        testp = blkbasep;
        expectval = writeval;
#if defined(RAMTEST_VALIDMASK)
        writeval ^= validmask;
#else
        writeval = ~writeval;
#endif

        while (wordsleft != 0) {
            /*
             * Unroll the read so as to maximize the speed.
             */
            val1 = RAMTEST_READ32(testp, 0);
            val2 = RAMTEST_READ32(testp, 0);
            val3 = RAMTEST_READ32(testp, 0);

            if (val1 != expectval) {
                resultp->failptr = (void *) testp;
                resultp->expected = expectval;
                resultp->actual = val1;
                return FALSE;
            }

            if (val2 != expectval) {
                resultp->failptr = (void *) testp;
                resultp->expected = expectval;
                resultp->actual = val2;
                return FALSE;
            }

            if (val3 != expectval) {
                resultp->failptr = (void *) testp;
                resultp->expected = expectval;
                resultp->actual = val3;
                return FALSE;
            }

            RAMTEST_WRITE32(testp, 0, writeval);
            RAMTEST_WRITE32(testp, 0, writeval);

#if defined(RAMTEST_NEED_FLUSH_MEMORY)
#   pragma flush_memory
#endif

            val1 = RAMTEST_READ32(testp, 0);
            val2 = RAMTEST_READ32(testp, 0);
            val3 = RAMTEST_READ32(testp, 0);

            if (val1 != writeval) {
                resultp->failptr = (void *) testp;
                resultp->expected = writeval;
                resultp->actual = val1;
                return FALSE;
            }

            if (val2 != writeval) {
                resultp->failptr = (void *) testp;
                resultp->expected = writeval;
                resultp->actual = val2;
                return FALSE;
            }

            if (val3 != writeval) {
                resultp->failptr = (void *) testp;
                resultp->expected = writeval;
                resultp->actual = val3;
                return FALSE;
            }

#if defined(RAMTEST_STRIDE)
            testp += (RAMTEST_STRIDE / sizeof(uint32_t));
#else
            testp++;
#endif
            wordsleft--;
        }
    }

    return TRUE;
}


