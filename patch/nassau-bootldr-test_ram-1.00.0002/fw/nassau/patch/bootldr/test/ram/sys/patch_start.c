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
 * Test the system RAMs.  This works by preserving RAM state in
 *  the data cache.
 *
 * Here's the play-by-play.  Note that all of this happens within
 *  the 'sysramtest()' function, which is in assembler.
 *
 * RULES FOR THE TEST FUNCTION
 *
 * Because the test function runs entirely within the instruction cache,
 *  it must not refer to any statically-allocated data, nor may it
 *  call into the bootldr ROM because we must leave the data cache
 *  active over the SROM mappings of things and so using ROM functions
 *  could pull data into the data cache that could overwrite our
 *  isolated copy.
 *
 * Bjoren Davis, August 9, 2017 (IMYSJT).
 */

#include "basetypes.h"
#include "bootldr.h"
#include "hwdef.h"
#include "fwcmd.h"
#include "ramtest.h"

/* -------------------------------------------------------------------------- */
/* Macros */

#if !defined(PATCH_NPASSES)
#   define PATCH_NPASSES     1
#endif

/* -------------------------------------------------------------------------- */
/* External prototypes */

RAMTEST_PROTO(PATCH_RAMTEST);

/* sysramtest.s: */
extern bool_t sysramtest(ramtest_result_t *resultp, unsigned int npasses,
                         bool_t (*testfuncp)(uint32_t *blkbasep,
                                             unsigned int nwords,
                                             unsigned int passno,
                                             ramtest_result_t *resultp));

/* -------------------------------------------------------------------------- */
/* New types */

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
    if (sysramtest(&(patch_reply.result), PATCH_NPASSES, &PATCH_RAMTEST)) {
        /* No error found */
        return VCSFW_STATUS_OKAY;
    }
    else {
        patch_reply.status = VCSFW_STATUS_OKAY;       /* !!!?? */
        BOOTLDR_REPLY(ctxp, (uint8_t *) &(patch_reply.status),
                  sizeof(patch_reply) - OFFSETOF(struct myreply, status));
        return -1;
    }
}

/* -------------------------------------------------------------------------- */
/* Static functions */


