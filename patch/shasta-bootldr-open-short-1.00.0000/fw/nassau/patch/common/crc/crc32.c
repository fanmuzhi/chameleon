/*
 * INTERNAL USE ONLY
 *
 * Copyright (c) 2006-2016 Synaptics Incorporated.  All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated ("Synaptics"). The holder of this file shall treat all
 * information contained herein as confidential, shall use the
 * information only for its intended purpose, and shall not duplicate,
 * disclose, or disseminate any of this information in any manner unless
 * Synaptics has otherwise provided express, written permission.
 */

#include "basetypes.h"
#include "bootldr.h"
#include "hwdef.h"
#include "hwreg.h"
#include "reg.h"

/* to use this function in a source file, include the extern below,
 * there is no associated header file.
 * extern uint32_t crc32(const uint32_t *ptr, unsigned int nbytes);
 */


uint32_t
crc32(const uint32_t *ptr, unsigned int nbytes)
{
    uint32_t    tmp;
    uint8_t     *tmpptr;
    uint8_t     *byteptr;

    REG_WRITE32(CRC32_ACCUM, 0xFFFFFFFF);
    while(nbytes>=4) {
        REG_WRITE32(CRC32_INPUT, *ptr);
        ptr++;
        nbytes-=4;
    }
    /* handle the last word a little differently to pad with 0's in
     * case nbytes%4!=0
     */
    if (nbytes) {
        tmp = 0;
        tmpptr = (uint8_t *)&tmp;
        byteptr = (uint8_t *) ptr;
        while (nbytes) {
            tmpptr[nbytes-1] = byteptr[nbytes-1];
            nbytes--;
        }
        REG_WRITE32(CRC32_INPUT, tmp);
    }
    return ~REG_READ32(CRC32_ACCUM);
}
