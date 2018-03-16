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
 * Test the instruction cache tag RAM.
 *
 * Bjoren Davis, August 9, 2017.
 */

#include "basetypes.h"
#include "bootldr.h"
#include "hwdef.h"
#include "fwcmd.h"
#include "ramtest.h"

#include <xtensa/config/core-isa.h>

/* -------------------------------------------------------------------------- */
/* Macros */

#if !defined(PATCH_NPASSES)
#   define PATCH_NPASSES     1
#endif

/* -------------------------------------------------------------------------- */
/* External prototypes */

RAMTEST_PROTO(PATCH_RAMTEST);

/* -------------------------------------------------------------------------- */
/* Static prototypes */

/* -------------------------------------------------------------------------- */
/* Static initialized data  */

/* -------------------------------------------------------------------------- */
/* Static uninitialized data  */

/*
 * Note: we place the reply in statically-allocated RAM (as opposed to
 *  stack-located RAM) in order to avoid any possibility of having
 *  a call stack get deep enough to clobber our reply before the
 *  host can read it.
 */
static struct myreply {
    uint16_t        padding;
    uint16_t        status;
    ramtest_result_t    result;
} patch_reply;

/* -------------------------------------------------------------------------- */
/* Global functions */

int
patch_start(void *ctxp, uint8_t *replyp)
{
    unsigned int    passno;
    int             status;
    uint32_t        oldps, cacheaddr, limit;

    status = -1;

    /* Turn off interrupts */
    __asm__ __volatile__("rsil  %0, "
                         RAMTEST_QUOTE2(RAMTEST_INTLEVEL_INTR) : "=a" (oldps));

    /* Turn off cache */
    BOOTLDR_CACHE_IATTR_SET(RAMTEST_ICACHE_ATTR_OFF);

    /* Do the test */
    for (passno = 0; passno < PATCH_NPASSES; passno++) {
        if (!PATCH_RAMTEST((uint32_t *) RAMTEST_ICT_PSEUDOADDR,
                           (XCHAL_ICACHE_SIZE / XCHAL_ICACHE_LINESIZE),
                           passno, &(patch_reply.result))) {
            status = VCSFW_STATUS_OKAY;       /* !!!? */
            break;
        }
    }

    /* Reset the instruction cache.  This recreates the icache_reset macro */
    /* First, unlock everything */
    limit = XCHAL_ICACHE_SIZE;

    for (cacheaddr = 0; cacheaddr < limit; cacheaddr += XCHAL_ICACHE_LINESIZE) {
        __asm__ __volatile__("iiu   %0, 0" : : "a" (cacheaddr));
    }
    __asm__ __volatile__("isync");

    /* Second, invalidate everything */
    limit = XCHAL_ICACHE_SIZE * XCHAL_ICACHE_WAYS;

    for (cacheaddr = 0; cacheaddr < limit; cacheaddr += XCHAL_ICACHE_LINESIZE) {
        __asm__ __volatile__("iii   %0, 0" : : "a" (cacheaddr));
    }
    __asm__ __volatile__("isync");


    /* Turn the cache on again */
    BOOTLDR_CACHE_IATTR_SET(RAMTEST_ICACHE_ATTR_ON);

    /* Restore interrupts */
    __asm__ __volatile__("wsr.ps  %0\n    rsync\n" : : "a" (oldps));

    /* Return results. */
    if (status == -1) {
        /* No error found */
        return VCSFW_STATUS_OKAY;
    }
    else {
        patch_reply.status = status;
        BOOTLDR_REPLY(ctxp, (uint8_t *) &(patch_reply.status),
                  sizeof(patch_reply) - OFFSETOF(struct myreply, status));
        return -1;
    }
}

/* -------------------------------------------------------------------------- */
/* Static functions */


