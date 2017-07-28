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
 * Common include file for NVM-stored iotas.
 *
 * Iotas a bits of data stored in a chain in the flash
 *  data area.
 *
 * Two kinds of firmware need this file:
 *  + the mission firmware (which reads the iotas)
 *  + the iota (probably bootldr) programming patches (which write iotas)
 *
 * Bjoren Davis, Synaptics, Inc., September 24, 2015.
 */

#ifndef __NVMIOTA_H
#define __NVMIOTA_H

/* -------------------------------------------------------------------------- */
/* New types and type-specific macros */

/*
 * The flash (a/k/a NVM) must be programmed in units of
 *  64-bits (8 bytes).  This macro defines that quantum.
 */
#define NVMIOTA_FLASHQUANT      (2 * sizeof(uint32_t))
/*
 * This is the in-flash representation of an iota header.
 *  Note that the 'itype' values are defined in vcsfw_v4.h
 *  We claim it must be aligned to 64-bit (8 byte) boundaries
 *  because that's the programming quantum of the flash.
 */
typedef struct nvmiota_header_s {
    uint16_t        nbytes;         /* number of bytes of iota payload */
    uint16_t        itype;          /* type of iota */
    uint32_t        crc32;          /* crc32 of iota data (excludes header!) */
} nvmiota_header_t ATTR_ALIGNED(NVMIOTA_FLASHQUANT);



/*
 * The iota chain starts immediately after the mission firmware.
 */

#define NVMIOTA_STARTADDR   HWDEF_MEMMAP_SROM_FLASH_IOTA_BASE
/*
 * and it extends all the way up to the penultimate QWORD (we reserve
 *  the end for the MISSIONVALID stuff)
 */
#define NVMIOTA_SIZE      HWDEF_MEMMAP_SROM_FLASH_IOTA_SIZE

#define NVMIOTA_ENDADDR   (NVMIOTA_STARTADDR + NVMIOTA_SIZE)


#endif      /* !defined(__NVMIOTA_H) */
