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
 * Test the device RAMs.
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

/* -------------------------------------------------------------------------- */
/* New types */

typedef struct patch_testrange_s {
    uint32_t        base;
    uint32_t        nwords;
} patch_testrange_t;

/* -------------------------------------------------------------------------- */
/* Static prototypes */

/* -------------------------------------------------------------------------- */
/* Static initialized data  */

static const patch_testrange_t patch_testranges[] = {
    { HWDEF_MEMMAP_FW_BL_RAM_BASE,
      HWDEF_MEMMAP_FW_BL_RAM_SIZE / sizeof(uint32_t)  },
    { HWDEF_MEMMAP_IMGRAM_BASE,
      HWDEF_MEMMAP_IMGRAM_SIZE / sizeof(uint32_t) },
    { HWDEF_MEMMAP_LNA_BL_RAM_BASE,
      HWDEF_MEMMAP_LNA_BL_RAM_SIZE / sizeof(uint32_t) },
    { HWDEF_MEMMAP_DMRAM_BASE,
      HWDEF_MEMMAP_DMRAM_SIZE / sizeof(uint32_t) },
    { HWDEF_MEMMAP_CDMRAM_BASE,
      HWDEF_MEMMAP_CDMRAM_SIZE / sizeof(uint32_t) },
#if defined(HWDEF_MEMMAP_ADCRAM0_BASE)
    { HWDEF_MEMMAP_ADCRAM0_BASE,
      HWDEF_MEMMAP_ADCRAM0_SIZE / sizeof(uint32_t) },
#endif        
    { HWDEF_MEMMAP_ADCRAM1_BASE,
      HWDEF_MEMMAP_ADCRAM1_SIZE / sizeof(uint32_t) },
    { HWDEF_MEMMAP_ADCRAM2_BASE,
      HWDEF_MEMMAP_ADCRAM2_SIZE / sizeof(uint32_t) },
#if defined(HWDEF_MEMMAP_ADCRAM3_BASE)
    { HWDEF_MEMMAP_ADCRAM3_BASE,
      HWDEF_MEMMAP_ADCRAM3_SIZE / sizeof(uint32_t) },
#endif        
#if defined(HWDEF_MEMMAP_PUF_RAM_BASE)
    { HWDEF_MEMMAP_PUF_RAM_BASE,
      HWDEF_MEMMAP_PUF_RAM_SIZE / sizeof(uint32_t) },
#endif
#if defined(HWDEF_MEMMAP_SCRATCH_RAM_BASE)
    { HWDEF_MEMMAP_SCRATCH_RAM_BASE,
      HWDEF_MEMMAP_SCRATCH_RAM_SIZE / sizeof(uint32_t) },
#endif
};
        

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
    const patch_testrange_t *trp;
    unsigned int    passno, trleft;
    int             status;
    uint32_t        oldps;

    status = -1;

    /* Turn off interrupts */
    __asm__ __volatile__("rsil  %0, "
                         RAMTEST_QUOTE2(RAMTEST_INTLEVEL_INTR) : "=a" (oldps));

    /* Do the test */
    for (passno = 0; passno < PATCH_NPASSES; passno++) {
        trleft = NELEM(patch_testranges);
        trp = &(patch_testranges[0]);
        while (trleft != 0) {
            if (!PATCH_RAMTEST((uint32_t *) trp->base, trp->nwords,
                               passno, &(patch_reply.result))) {
                status = VCSFW_STATUS_OKAY;       /* !!!? */
                goto finish;
            }
            trp++;
            trleft--;
        }
    }

finish:

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


