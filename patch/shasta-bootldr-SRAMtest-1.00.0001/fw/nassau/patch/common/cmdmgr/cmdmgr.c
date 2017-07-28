/* -*- mode: c; tab-width: 4 -*- */
/*
 * INTERNAL USE ONLY
 *
 * Copyright (c) 2015-2016 Synaptics Incorporated.  All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated ("Synaptics"). The holder of this file shall treat all
 * information contained herein as confidential, shall use the
 * information only for its intended purpose, and shall not duplicate,
 * disclose, or disseminate any of this information in any manner unless
 * Synaptics has otherwise provided express, written permission.
 */

/*
 * Implement a command/reply mechanism for patches to
 *  take advantage of.  Note that, for later versions of
 *  bootldrs of IFVER=2 and later (Denali B0) this code is much simpler.
 *
 * Bjoren Davis, January 18, 2016.
 */

/*
 * The implementation that uses the new cmdset() function.
 */
#include "basetypes.h"
#include "hwdef.h"
#include "hwreg.h"
#include "reg.h"
#include "fwcmd.h"
#include "patch_common.h"
#include "cmdmgr.h"
#include "vscat.h"
#if defined(NASSAU_PATCH_TYPE) && NASSAU_PATCH_TYPE==NASSAU_PATCH_TYPE_BOOTLDR
#   include "bootldr.h"
#   define CMDMGR_BOOTLDR
#else
#   include "aterom.h"
#   include "atetbenv.h"
#   define CMDMGR_ATEROM
#endif

/*
 * Externally defined macros:
 *
 *     CMDMGR_OPT_DIRSPI        Boolean.  Allow use of direct SPI access.
 *                              This is really for Denali A0 and A1 parts.
 *
 *     CMDMGR_OPT_CMDSET        Boolean Use the CMDSET() mechanism, if it's
 *                              available.
 *
 * Both CMDMGR_OPT_DIRSPI and CMDMGR_OPT_CMDSET can be defined at the
 *  same time for bootldr patches.  For ATEROM patches only CMDMGR_OPT_DIRSPI
 *  can be defined.
 *
 *      CMDMGR_MAXREPLY         Integer.  Maximum number of bytes in the
 *                              reply buffer.  If not defined this
 *                              defaults to 32.
 *
 *      CMDMGR_MAXCMD           Integer.  Maximum number of bytes in the
 *                              command buffer.  If not defined this
 *                              defaults to 64.  It should be a multiple
 *                              of the cacheline size (32).
 */

#if !defined(CMDMGR_OPT_DIRSPI) && !defined(CMDMGR_OPT_CMDSET)
#   error "One or both of CMDMGR_OPT_DIRSPI and CMDMGR_OPT_CMDSET must be defined"
#endif

#if defined(CMDMGR_ATEROM)
#   if defined(CMDMGR_OPT_CMDSET)
#       error "CMDMGR_OPT_CMDSET can't be defined when building for ATEROM"
#   endif
#   if !defined(CMDMGR_OPT_DIRSPI)
#       error "CMDMGR_OPT_DIRSPI must be defined when building for ATEROM"
#   endif
#endif

#if !defined(CMDMGR_MAXREPLY)
#   define CMDMGR_MAXREPLY      32
#endif

#if !defined(CMDMGR_MAXCMD)
#   define CMDMGR_MAXCMD        64
#endif

/* -------------------------------------------------------------------------- */
/* New types and type-specific macros */

/* The minimum ifver that supports CMDSET: */
#define CMDMGR_CMDSET_IFVER_MIN     2

#define CMDMGR_ASSERT(x)

#if defined(CMDMGR_OPT_DIRSPI)
#   if defined(BOOTLDR_MEMMAP_INTPEND)
#       define CMDMGR_INTPEND  (*((volatile uint32_t *) BOOTLDR_MEMMAP_INTPEND))
#   else
#       define CMDMGR_INTPEND  (*((volatile uint32_t *) 0x6000ffe0))
#   endif

#   if defined(CMDMGR_BOOTLDR)
#       define CMDMGR_HOSTCOMM_SPI_INTEGRITY
/* Expected CRC-32 residual: */
#       define CMDMGR_SPICMD_CRC32_RESIDUAL       0xc704dd7b
#   endif
#endif      /* defined(CMDMGR_OPT_DIRSPI) */


#if defined(CMDMGR_OPT_DIRSPI)
#   define CMDMGR_BOOTLDR_INTLEVEL_IDLE       0
#   define CMDMGR_BOOTLDR_INTLEVEL_INTR       5


 /* CMDMGR_SPICMD command (EP1IN and EP1OUT) states.
 */
typedef enum __attribute__ ((__packed__)) {
    /*
     * CMDMGR_SPICMD_EP1STATE_OFF. Endpoints are both off.
     */
    CMDMGR_SPICMD_EP1STATE_OFF = 0,

    /*
     * CMDMGR_SPICMD_EP1STATE_CMDWAIT. EP1OUT is ON, EP1IN is set to OFF
     * We wait in this state for a command to arrive.
     */
    CMDMGR_SPICMD_EP1STATE_CMDWAIT = 1,

    /*
     * CMDMGR_SPICMD_EP1STATE_CMDPROC. EP1OUT and EP1IN are set to OFF.
     * We are executing a command.
     */
    CMDMGR_SPICMD_EP1STATE_CMDPROC = 2,

    /*
     * CMDMGR_SPICMD_EP1STATE_REPLY. EP1OUT is set to OFF, EP1IN is set to ON.
     * We have formatted up a reply and queued it for transmission
     * to the host.
     */
    CMDMGR_SPICMD_EP1STATE_REPLY = 3,

    /*
     * CMDMGR_SPICMD_EP1STATE_REPLYSENT.  EP1OUT and EP1IN will be both OFF.
     * We've sent a reply and received confirmation of such
     * from the host. We are waiting for the completion
     * function (cmd_replysent()).
     */
    CMDMGR_SPICMD_EP1STATE_REPLYSENT = 4
} cmdmgr_spicmd_ep1state_t;

#endif      /* defined(CMDMGR_OPT_DIRSPI) */

typedef struct _cmdmgr_state_s {
    uint8_t                     *cmdbufp;
    uint32_t                     cmdbuf[ROUNDUPDIV(CMDMGR_MAXCMD
                                                   + HWDEF_DCACHE_LINESIZE,
                                                   sizeof(uint32_t))];
    uint32_t                     replybuf[ROUNDUPDIV(CMDMGR_MAXREPLY,
                                                     sizeof(uint32_t))];
    const cmdmgr_cmdentry_t     *tablep;
    unsigned int                 nentries;
#if defined(CMDMGR_OPT_DIRSPI)
    cmdmgr_spicmd_ep1state_t     ep1state;
    membool_t                    unloadpending;
#endif
#if defined(CMDMGR_OPT_CMDSET) && defined(CMDMGR_OPT_DIRSPI)
    membool_t                    use_cmdset;
#endif
} cmdmgr_state_t;

/* -------------------------------------------------------------------------- */
/* Uninitialized static data */

#if defined(CMDMGR_OPT_CMDSET)
static cmdmgr_state_t cmdmgr_state;
#endif      /* defined(CMDMGR_OPT_DIRSPI) */

/* -------------------------------------------------------------------------- */
/* Static prototypes */

static void cmdmgr_dispatch(void *ctxp, const uint8_t *cmdbufp,
                            unsigned int cmdlen, bool_t overflow);
#if defined(CMDMGR_OPT_DIRSPI)
static void cmdmgr_spicmd_cmdreceive(cmdmgr_state_t *statep);
static void cmdmgr_spicmd_cmdreply(cmdmgr_state_t *statep,
                                   const uint8_t *replyp, unsigned int length);
static void cmdmgr_spicmd_ep1state(cmdmgr_state_t *statep, int ep1state);
static void cmdmgr_spicmd_ep1in_int(cmdmgr_state_t *statep);
static void cmdmgr_spicmd_ep1out_int(cmdmgr_state_t *statep);
static void cmdmgr_spicmd_int(cmdmgr_state_t *statep);
#endif      /* defined(CMDMGR_OPT_DIRSPI) */

/* -------------------------------------------------------------------------- */
/* Inline assembler function prototypes */

#if defined(CMDMGR_OPT_DIRSPI) && defined(CMDMGR_BOOTLDR)
extern void cmdmgr_cpu_wait(void);
#endif

/* -------------------------------------------------------------------------- */
/* Global functions */

/*
 * The main function.  A patch calls this function
 *  to start up command processing.  If we're using CMDSET
 *  then this function will immediately return.  If we're not
 *  using CMDSET it will loop until we receive a VCSFW_CMD_UNLOAD_PATCH.
 */
int
cmdmgr(void *ctxp, const cmdmgr_cmdentry_t *tablep, unsigned int nentries)
{
#if defined(CMDMGR_OPT_DIRSPI)
#   if defined(CMDMGR_BOOTLDR)
    uint32_t             intmask;
#   endif
    uint32_t             intpend;
#endif
#if !defined(CMDMGR_OPT_CMDSET)
    cmdmgr_state_t       state;
#endif
    cmdmgr_state_t      *csp;

#if !defined(CMDMGR_OPT_CMDSET)
    csp = &state;
#else
    csp = &cmdmgr_state;
#endif

    /*
     * Figure out where the nice new command buffer will be.
     */
    csp->cmdbufp = (uint8_t *) HWDEF_DCACHE_ALIGN(&(csp->cmdbuf[0])) + 3;
    csp->tablep = tablep;
    csp->nentries = nentries;

#if defined(CMDMGR_OPT_CMDSET) && defined(CMDMGR_OPT_DIRSPI)
    /*
     * Look at the bootldr interface version to figure out
     *  whether to use CMDSET() or direct SPI.
     */
    csp->use_cmdset = (bootldr_funcps.ifver >= CMDMGR_CMDSET_IFVER_MIN);

    if (csp->use_cmdset) {
        BOOTLDR_CMDSET(ctxp, csp->cmdbufp, (unsigned int)
                       (((uint8_t *) &(csp->cmdbuf[NELEM(csp->cmdbuf)]))
                        - csp->cmdbufp), &cmdmgr_dispatch);

        return VCSFW_STATUS_OKAY;
    }
    else {

#elif defined(CMDMGR_OPT_CMDSET)
    /*
     * Make sure the interface version supports CMDSET
     */
    if (bootldr_funcps.ifver < CMDMGR_CMDSET_IFVER_MIN) {
        return VCSFW_STATUS_ERR_BOOTLDR_PATCH_IFVER;
    }
    BOOTLDR_CMDSET(ctxp, csp->cmdbufp, (unsigned int)
                   (((uint8_t *) &(csp->cmdbuf[NELEM(csp->cmdbuf)]))
                    - csp->cmdbufp), &cmdmgr_dispatch);

    return VCSFW_STATUS_OKAY;
#endif

#if defined(CMDMGR_OPT_DIRSPI)
    csp->ep1state = CMDMGR_SPICMD_EP1STATE_CMDPROC;
    csp->unloadpending = FALSE;
    /*
     * Reply successfully to the patch load command.
     */
    *(((uint16_t *) &(csp->replybuf[0])) + 1) = VCSFW_STATUS_OKAY;
    cmdmgr_spicmd_cmdreply(csp,
                           (uint8_t *)(((uint16_t *) &(csp->replybuf[0]))+1),
                           sizeof(uint16_t));

    /*
     * We take over the entire part to do this.
     */
#if defined(CMDMGR_BOOTLDR)
    intmask = (HWDEF_INT_BC | HWDEF_INT_SPI);

    /*
     * Turn all the interrupts back on again.
     */
    __asm__ __volatile__("wsr.intenable     %0\n\t"
                         "rsync\n" : : "a" (intmask));
#endif      /* defined(CMDMGR_BOOTLDR) */

    while (!csp->unloadpending
#if defined(CMDMGR_ATEROM)
           /*
            * For the ATE ROM, we need to send the reply to
            *  VCSFW_CMD_UNLOAD_PATCH before returning.
            */
           || csp->ep1state != CMDMGR_SPICMD_EP1STATE_REPLYSENT
#endif      /* defined(CMDMGR_ATEROM) */
                                                                ) {
#if defined(CMDMGR_BOOTLDR)
        /*
         * Interrupts are off.  Wait for an interrupt.
         */
        cmdmgr_cpu_wait();

        intpend = CMDMGR_INTPEND;
        CMDMGR_INTPEND = 0;
#else
        __asm__ __volatile__("rsr.interrupt %0" : "=a" (intpend));
#endif

        if (REG_BITISON_B(intpend, HWDEF_INT_BC_B)) {
            /* Dispatch the buffer controller interrupt */
            uint32_t        intrmap;

            intrmap = REG_READ32(BC_INTR_MAP);

            if (REG_BITISON_B(intrmap, BC_INTR_MAP_EP1_IN_STS_INTR_B)) {
                cmdmgr_spicmd_ep1in_int(csp);
            }
            if (REG_BITISON_B(intrmap, BC_INTR_MAP_EP1_OUT_STS_INTR_B)) {
                cmdmgr_spicmd_ep1out_int(csp);
            }
        }

        if (REG_BITISON_B(intpend, HWDEF_INT_SPI_B)) {
            /* Dispatch the SPI interrupts */
            cmdmgr_spicmd_int(csp);
        }

#if defined(CMDMGR_BOOTLDR)
        /*
         * Turn all the interrupts back on again.
         */
        __asm__ __volatile__("wsr.intenable     %0\n\t"
                             "rsync\n" : : "a" (intmask));
#endif      /* defined(CMDMGR_BOOTLDR) */
    }

    /*
     * We're done.  If we're in the bootldr then tell the
     *  bootldr to reply to the VCSFW_CMD_UNLOAD_PATCH with
     *  success.
     */
    return VCSFW_STATUS_OKAY;

#endif      /* defined(CMDMGR_OPT_DIRSPI) */

#if defined(CMDMGR_OPT_CMDSET) && defined(CMDMGR_OPT_DIRSPI)
    }
#endif
}

/*
 * Send a reply to the host.
 *
 * ctxp points to &cmdmgr_state if
 *   defined(CMDMGR_OPT_DIRSPI) && !defined(CMDMGR_OPT_CMDSET) or
 *   defined(CMDMGR_OPT_DIRSPI) && !cmdmgr_state.use_cmdset
 *
 * otherwise it points to the bootldr's context
 */
void
cmdmgr_reply(void *ctxp, const uint8_t *replyp, unsigned int length)
{
#if defined(CMDMGR_OPT_CMDSET) && defined(CMDMGR_OPT_DIRSPI)
    if (cmdmgr_state.use_cmdset) {
        BOOTLDR_REPLY(ctxp, replyp, length);
    }
    else {
        cmdmgr_spicmd_cmdreply((cmdmgr_state_t *) ctxp, replyp, length);
    }
#elif defined(CMDMGR_OPT_CMDSET)
    BOOTLDR_REPLY(ctxp, replyp, length);
#else
    cmdmgr_spicmd_cmdreply((cmdmgr_state_t *) ctxp, replyp, length);
#endif
}



/* -------------------------------------------------------------------------- */
/* Static functions */

/*
 * Actually dispatch a command.
 *
 * ctxp points to &cmdmgr_state if
 *   defined(CMDMGR_OPT_DIRSPI) && !defined(CMDMGR_OPT_CMDSET) or
 *   defined(CMDMGR_OPT_DIRSPI) && !cmdmgr_state.use_cmdset
 *
 * otherwise it points to the bootldr's context
 */
static void
cmdmgr_dispatch(void *ctxp, const uint8_t *cmdbufp,
                unsigned int cmdlen, bool_t overflow)
{
    int              status;
    unsigned int     idx;
    cmdmgr_state_t  *csp;

    CMDMGR_ASSERT(cmdlen >= 1);

#if defined(CMDMGR_OPT_CMDSET)
    csp = &cmdmgr_state;
#else
    csp = (cmdmgr_state_t *) ctxp;
#endif

    if (overflow) {
        status = VCSFW_STATUS_ERR_CMDOVERFLOW;
    }
    else if (*cmdbufp == VCSFW_CMD_UNLOAD_PATCH) {
        /*
         * Special handling for the unload patch command.
         */
#if defined(CMDMGR_OPT_CMDSET) && defined(CMDMGR_OPT_DIRSPI)
        if (csp->use_cmdset) {
            BOOTLDR_CMDSET(ctxp, NULL, 0, NULL);
            status = VCSFW_STATUS_OKAY;
        }
        else {
#elif defined(CMDMGR_OPT_CMDSET)
        BOOTLDR_CMDSET(ctxp, NULL, 0, NULL);
        status = VCSFW_STATUS_OKAY;
#endif

#if defined(CMDMGR_OPT_DIRSPI)
        csp->unloadpending = TRUE;
#   if defined(CMDMGR_ATEROM)
        status = VCSFW_STATUS_OKAY;
#   else
        status = -1;
#   endif
#endif
#if defined(CMDMGR_OPT_CMDSET) && defined(CMDMGR_OPT_DIRSPI)
        }
#endif
    }
    else {
        /*
         * Go through our table of handlers and find the one
         *  that handles the command we just received.
         * If we fail to find a handler then we return INVALID_CMD.
         */
        status = VCSFW_STATUS_ERR_INVALID_CMD;

        /*
         * Go through our table of handlers and find the one
         *  that handles the command we just received.
         * If we fail to find a handler then we return INVALID_CMD.
         */
        for (idx = 0; idx < csp->nentries; idx++) {
            if (csp->tablep[idx].id == (uint32_t) *cmdbufp) {
                VSCAT_HINT_MAYCALLPOOL("bootldr_cmdhandler");
                status = (*csp->tablep[idx].handlerp)(
                    ctxp, cmdbufp+1, cmdlen-1, (uint8_t *) &(csp->replybuf[0]));
            }
        }
    }

    /*
     * Did the handler take care of forming a response?
     */
    if (status >= 0) {
        /* No.  Send the status back to the host */
        *((uint16_t *) &(csp->replybuf[0])) = status;
        cmdmgr_reply(ctxp, (const uint8_t *) &(csp->replybuf[0]),
                     sizeof(uint16_t));
    }
}

#if defined(CMDMGR_OPT_DIRSPI)
/*
 * Arrange to start receiving a new command.
 */

static void
cmdmgr_spicmd_cmdreceive(cmdmgr_state_t *statep)
{
    /* Flush the cache and invalidate it */
#   if defined(CMDMGR_BOOTLDR)
    BOOTLDR_CACHE_DFLUSH(statep->cmdbufp, (uint32_t) CMDMGR_MAXCMD,
                         BOOTLDR_CACHE_FLUSH_WBINV);
#else
    ATEROM_CACHE_DFLUSH(statep->cmdbufp, (uint32_t) CMDMGR_MAXCMD,
                        ATEROM_CACHE_FLUSH_WBINV);
#endif
    REG_WRITE32(BC_EP1_OUT_STRT_ADDR, (uint32_t) statep->cmdbufp);
    REG_WRITE32(BC_EP1_OUT_END_ADDR, (uint32_t) CMDMGR_MAXCMD);
    REG_OR32(BC_EP1_OUT_CTRL, BC_EP1_OUT_CTRL_CHANL);
    cmdmgr_spicmd_ep1state(statep, CMDMGR_SPICMD_EP1STATE_CMDWAIT);
}

/*
 * Arrange for the given command reply to be sent.
 */

static void
cmdmgr_spicmd_cmdreply(cmdmgr_state_t *statep,
                       const uint8_t *replyp, unsigned int length)
{
    CMDMGR_ASSERT(statep->ep1state == CMDMGR_SPICMD_EP1STATE_CMDPROC);

    /* Flush the cache, but don't necessarily invalidate */
#   if defined(CMDMGR_BOOTLDR)
    BOOTLDR_CACHE_DFLUSH((void *) replyp, length, BOOTLDR_CACHE_FLUSH_WB);
#   else
    ATEROM_CACHE_DFLUSH((void *) replyp, length, ATEROM_CACHE_FLUSH_WB);
#endif
    REG_WRITE32(BC_EP1_IN_STRT_ADDR, (uint32_t) replyp);
    REG_WRITE32(BC_EP1_IN_END_ADDR, (uint32_t) (replyp + length));

    REG_OR32(BC_EP1_IN_CTRL, BC_EP1_IN_CTRL_CHANL);

    cmdmgr_spicmd_ep1state(statep, CMDMGR_SPICMD_EP1STATE_REPLY);
}

/*
 * Set the ep1 state to the given value.  Also updates the
 *  SOFTSTATE field in the EP0IN register.
 */
static void
cmdmgr_spicmd_ep1state(cmdmgr_state_t *statep, int ep1state)
{
    uint32_t         ep0sts;

    ep0sts = REG_READ32(BC_EP0_IN_SPI_I2C_STS);
    ep0sts &= ~BC_EP0_IN_SPI_I2C_STS_SOFTSTATE;
    ep0sts |= ep1state;
    ep0sts |= BC_EP0_IN_SPI_I2C_STS_STATECHANGED;
    REG_WRITE32(BC_EP0_IN_SPI_I2C_STS, ep0sts);
    statep->ep1state = ep1state;
}

/*
 * Process an BC:EP1IN interrupt.
 *
 * This function is called from the buffer controller interrupt
 *  dispatcher.
 */

static void
cmdmgr_spicmd_ep1in_int(cmdmgr_state_t *statep)
{
    uint32_t        intsts;

    /* REG_RDWR32 will clear the active interrupts */
    intsts = REG_RDWR32(BC_EP1_IN_STS);
    intsts &= REG_READ32(BC_EP1_IN_STS_INTR_MASK);

    if (REG_BITISON_B(intsts, BC_EP1_IN_STS_XFER_SENT_B)) {
        /* Host has received the reply.  Wrap it up. */
        CMDMGR_ASSERT(statep->ep1state == CMDMGR_SPICMD_EP1STATE_REPLY);
#if defined(CMDMGR_ATEROM)
        if (statep->unloadpending) {
            cmdmgr_spicmd_ep1state(statep, CMDMGR_SPICMD_EP1STATE_REPLYSENT);
        }
        else {
            /* Setup to receive another command */
            cmdmgr_spicmd_cmdreceive(statep);
        }
#else
        /* Setup to receive another command */
        cmdmgr_spicmd_cmdreceive(statep);
#endif
    }
}

/*
 * Proces an BC:EP1OUT interrupt.
 *
 * This function si called from the buffer controller
 *  interrupt dispatcher.
 */

static void
cmdmgr_spicmd_ep1out_int(cmdmgr_state_t *statep)
{
    uint32_t        intsts, endaddr;
    unsigned int    cmdlen;
    bool_t          overflow;

    /* REG_RDWR32 will clear the active interrupts */
    intsts = REG_RDWR32(BC_EP1_OUT_STS);
    intsts &= REG_READ32(BC_EP1_OUT_STS_INTR_MASK);

    /*
     * What are we looking at?
     */

    if (REG_BITISON_B(intsts, BC_EP1_OUT_STS_XFER_RCVD_B)) {

        if (statep->ep1state != CMDMGR_SPICMD_EP1STATE_CMDWAIT) {
            /* This might be a spurious interrupt. Simply ignore */
            return;
        }
        /*
         * We got a command.  First, return the spare space to
         *  the pool.
         */
        endaddr = (REG_READ32(BC_EP1_OUT_MEM_WR_ADDR)
                   + HWDEF_MEMMAP_SRAM_RAM_BASE);

        overflow = (endaddr >= (uint32_t) (statep->cmdbufp + CMDMGR_MAXCMD));

        cmdlen = ((uint8_t *) endaddr - statep->cmdbufp);

#if defined(CMDMGR_HOSTCOMM_SPI_INTEGRITY)
        if (cmdlen <= sizeof(uint32_t)) {
            goto rearm;
        }
        if (REG_READ32(SPI_EP1_CRC) != CMDMGR_SPICMD_CRC32_RESIDUAL) {
            goto rearm;
        }
        /*
         * Free up the space at the tail of the command.
         */
        cmdlen -= sizeof(uint32_t);
#endif      /* defined(CMDMGR_HOSTCOMM_SPI_INTEGRITY) */

        if (cmdlen == 0) {
            goto rearm;
        }

        cmdmgr_spicmd_ep1state(statep, CMDMGR_SPICMD_EP1STATE_CMDPROC);

        cmdmgr_dispatch((void *) statep, (const uint8_t *) statep->cmdbufp,
                        cmdlen, overflow);
    }

    return;
rearm:
    /*
     * This is a void transaction.
     *  Ignore it and re-enable the channel.
     */

    REG_OR32(BC_EP1_OUT_CTRL, BC_EP1_OUT_CTRL_CHANL);
}


/*
 * The CMDMGR_SPICMD interrupt handler.  This is where we actually
 *  do the work of handling interrupts.
 */

static void
cmdmgr_spicmd_int(cmdmgr_state_t *statep)
{
    uint32_t         intsts, regval;

    /* REG_RDWR32 will clear the active interrupts */
    intsts = REG_RDWR32(SPI_SYS_STATUS);
    intsts &= REG_READ32(SPI_INTR_MASK);

    if (REG_BITISON_B(intsts, SPI_SYS_STATUS_TXFR_ERR_B)) {
        /*
         * Don't do anything if we see a non-n*8 bit transfer.
         * I believe we are seeing these in the real world
         *  when the part goes in and out of low power mode.
         */

        /*
         * Bugfix: it appears that we may sometimes shut off the
         *  EP1OUT channel due to this interrupt.  And we don't
         *  seem to get a BC_EP1_OUT_STS[XFER_RCVD] interrupt.
         *  So rearm it if necessary.
         *
         * Strictly speaking, this is a race condition (because
         *  the BC_EP1_OUT_STS[XFER_RCVD] could be pending).
         *
         * However, it would be hard for this TXFR_ERR interrupt
         *  to be delayed so long that it corresponds with a
         *  different transfer.
         *
         * Ideally, the hardware would change to say: if
         *  the hardware ever clears BC_EP1_OUT_CTRL[CHANL] it
         *  delivers a single type of interrupt.  There can be
         *  a status delivered then to indicate success or failure,
         *  but you would always get the interrupt.
         */
        if (statep->ep1state == CMDMGR_SPICMD_EP1STATE_CMDWAIT) {
            regval = REG_READ32(BC_EP1_OUT_CTRL);
            if (REG_BITISOFF_B(regval, BC_EP1_OUT_CTRL_CHANL_B)) {
                REG_WRITE32(BC_EP1_OUT_CTRL, regval | BC_EP1_OUT_CTRL_CHANL);
            }
        }
    }
}




/* -------------------------------------------------------------------------- */
/* Assembler functions */

#if defined(CMDMGR_BOOTLDR)
#define _Q1(x)  #x
#define _Q2(x)  _Q1(x)
#define CMDMGR_CPU_WAIT_ASM(intlevel_idle, intlevel_intr)                    \
    "\n\
    .text\n\
    .global cmdmgr_cpu_wait\n\
    .type   cmdmgr_cpu_wait, @function\n\
    .align 4\n\
cmdmgr_cpu_wait:\n\
    entry   a1, 16\n\
    extw\n\
    waiti " _Q2(intlevel_idle) "\n                                          \
    rsil    a3, " _Q2(intlevel_intr) "\n                                    \
    retw\n\
    .size   cmdmgr_cpu_wait, . - cmdmgr_cpu_wait\n"

asm(CMDMGR_CPU_WAIT_ASM(CMDMGR_BOOTLDR_INTLEVEL_IDLE,
                            CMDMGR_BOOTLDR_INTLEVEL_INTR));

#endif      /* defined(CMDMGR_BOOTLDR) */

#endif      /* defined(CMDMGR_OPT_DIRSPI) */
