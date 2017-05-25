/* -*- mode: c; tab-width: 4 -*- */
/*
 * INTERNAL USE ONLY
 *
 * Copyright (c) 2016 Synaptics Incorporated.  All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated ("Synaptics"). The holder of this file shall treat all
 * information contained herein as confidential, shall use the
 * information only for its intended purpose, and shall not duplicate,
 * disclose, or disseminate any of this information in any manner unless
 * Synaptics has otherwise provided express, written permission.
 */

/*
 * Definitions, structures and prototypes for a SPI command loop.
 *  This allows patches, both ATE ROM and bootldr, to handle
 *  commands.
 * Note that this code is not needed with bootldrs of IFVER=2 and
 *  later (e.g., Denali B0).
 *
 * Bjoren Davis, Synaptics, Inc., January 18, 2016
 */


#ifndef __CMDMGR_H
#define __CMDMGR_H

/* ------------------------------------------------------------------------- */
/* General macros */

/* ------------------------------------------------------------------------- */
/* New types and type-specific macros */

typedef struct _cmdmgr_cmdentry_t {
    uint32_t         id;
    int             (*handlerp)(void *ctxp, const uint8_t *cmdbufp,
                                unsigned int cmdlen, uint8_t *replyp);
} cmdmgr_cmdentry_t;

/* ------------------------------------------------------------------------- */
/* New types and type-specific macros */

/* cmdmgr.c: */
extern int cmdmgr(void *ctxp, const cmdmgr_cmdentry_t *tablep,
                  unsigned int nentries);
extern void cmdmgr_reply(void *ctxp, const uint8_t *replyp,
                         unsigned int length);

#endif      /* !defined(__CMDMGR_H) */




