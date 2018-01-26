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
 * This patch is loaded to implement the FLASH_ERASE command.  The
 * flags sent with the command indicate what section is to be erased.
 * The sections include mission firmware (first 126KiB on Denali,
 *  first 128KiB on others), SDB (2 KiB only on Denali), iota (last
 * 64 KiB of the main 192 KiB), or the FIB (strongly unrecommended).
 * The valid combinations for Denali are:
 *     Flags  |  FIB  |  Iota  |  Mission  |  SDB  |
 *       1    |       |        |     X     |       |
 *       2    |       |    X   |           |       |
 *       3    |       |    X   |     X     |       |
 *       8    |       |        |           |   X   |
 *       9    |       |        |     X     |   X   |
 *      10    |       |    X   |           |   X   |
 *      11    |       |    X   |     X     |   X   |
 *      15    |   X   |    X   |     X     |   X   |
 *
 * The valid combinations for others are:
 *     Flags  |  FIB  |  Iota  |  Mission  |
 *       1    |       |        |     X     |
 *       2    |       |    X   |           |
 *       3    |       |    X   |     X     |
 *       7    |   X   |    X   |     X     |
 */


#include "basetypes.h"
#include "bootldr.h"
#include "fwcmd.h"
#include "hwdef.h"
//#include "patchopt.h"
#include "reg.h"
#include "hwreg.h"

#include "cmdmgr.h"

#include "romtest.h"

//#define DEBUG
#ifdef DEBUG
#define ERROR_SIMULATION
#endif

/* -------------------------------------------------------------------------- */
/* Static prototypes */

static int patch_cmd_handler_rom_test(void *ctxp, const uint8_t *cmdbufp,
                                         unsigned int cmdlen, uint8_t *replyp);

static uint8_t iROMTest( uint8_t * ulStartAddr, unsigned long ulLength, unsigned long *ulErrAddr );

extern uint32_t crc32(const uint32_t *ptr, unsigned int nbytes);

/* -------------------------------------------------------------------------- */
/* Static uninitialized data  */




/* -------------------------------------------------------------------------- */
/* Static initialized data  */

static cmdmgr_cmdentry_t patch_cmdtable[] = {
    { VCSFW_CMD_ROM_TEST, &patch_cmd_handler_rom_test }
};

/* -------------------------------------------------------------------------- */
/* Global functions */

static uint32_t
patch_crc32_calc_soft(const uint32_t *datap, unsigned int nwords, uint32_t crc)
{
#define CRC32_POLY  0x04c11db7
    uint32_t            tmpword;
    unsigned int        bitnum, highbit, bytenum;
    uint32_t            val;

    while (nwords != 0) 
	{
        tmpword = *datap++;
        for (bytenum = 4; bytenum != 0; bytenum--) 
		{
            val = tmpword & 0x000000ff;
            bitnum = 8;
            while (bitnum != 0) 
			{
                highbit = (crc & 0x80000000) >> 31;
                crc <<= 1;
                if ((highbit ^ ((val>>7) & 1)) == 1) 
				{
                    crc ^= CRC32_POLY;
                }
                val <<= 1;

                bitnum--;
            }
            tmpword >>= 8;
        }
        nwords--;
    }
    return ~crc;
}

static uint8_t iROMTest( uint8_t * ulStartAddr, uint32_t ulLength, uint32_t *ulErrAddr )
{
	uint32_t i, xor = 0;

	for( i = 0; i < ulLength; i++ )
	{
		xor ^= ulStartAddr[i];
	}

#ifdef ERROR_SIMULATION
	// just for simulate sram error occur	
	return 0x55 ^ 0x0ff;
#endif

	return xor;
}


int
patch_start(void *ctxp, uint8_t *replyp)
{
    return cmdmgr(ctxp, &(patch_cmdtable[0]), NELEM(patch_cmdtable));
}

/* -------------------------------------------------------------------------- */
/* Static functions */

/*
 * Handler for VCSFW_CMD_ROM_TEST.
 */
static int
patch_cmd_handler_rom_test(void *ctxp, const uint8_t *cmdbufp,
                              unsigned int cmdlen, uint8_t *replyp)
{
	uint8_t status = 0;

	uint32_t uicrc32 = 0;

	unsigned long err_addr = 0;
		
	uint16_t *rp = ( uint16_t * ) replyp + 2;

#if 1
	uint32_t uiData = 0x31323334;
#endif

	*(((uint16_t *) replyp) + 1 ) = VCSFW_STATUS_OKAY;

	status = iROMTest( (uint8_t *)HWDEF_MEMMAP_SROM_SPACE_BASE, (uint32_t)HWDEF_MEMMAP_ROM_SIZE, &err_addr );

	*rp++ = status;

	//uicrc32 = crc32( (uint32_t *)HWDEF_MEMMAP_SROM_SPACE_BASE, (unsigned int)HWDEF_MEMMAP_ROM_SIZE );
	uicrc32 = crc32( (uint32_t *)(&uiData), 4 );

	*rp++ = uicrc32;
	*rp++ = uicrc32 >> 16;

	uiData = 0x31;
	uicrc32 = crc32( (uint32_t *)(&uiData), 4 );
	*rp++ = uicrc32;
	*rp++ = uicrc32 >> 16;

	uiData = 0x12345678;
	uicrc32 = crc32( (uint32_t *)(&uiData), 4 );
	*rp++ = uicrc32;
	*rp++ = uicrc32 >> 16;

	uicrc32 = patch_crc32_calc_soft( (uint32_t *)(&uiData), 1, ~0 );
	*rp++ = uicrc32;
	*rp++ = uicrc32 >> 16;
	
#ifndef DEBUG
	cmdmgr_reply( ctxp, (uint8_t *)(((uint16_t *) replyp)+1), sizeof(uint16_t) + 26 );
#else	// just for debug, send all 16KB rom data to PC
	unsigned long                    i;

	for( i = 0; i < 16 * 1024 / 2; i++ )
	{
		*rp++ = *(( unsigned short * )( HWDEF_MEMMAP_SROM_SPACE_BASE ) + i );
	}
	cmdmgr_reply( ctxp, (uint8_t *)(((uint16_t *) replyp)+1), sizeof(uint16_t) + 6 + 16 * 1024 );	
#endif

    return -1;
}

