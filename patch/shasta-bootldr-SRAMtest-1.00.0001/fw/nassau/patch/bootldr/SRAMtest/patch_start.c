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
 * This patch is loaded to implement the VCSFW_CMD_SRAM_TEST command.
 */


#include "basetypes.h"
#include "bootldr.h"
#include "fwcmd.h"
#include "hwdef.h"
#include "reg.h"
#include "hwreg.h"

#include "cmdmgr.h"

#include "sramtest.h"

//#define DEBUG

/* -------------------------------------------------------------------------- */
/* Static prototypes */

static int patch_cmd_handler_sram_test(void *ctxp, const uint8_t *cmdbufp,
                                         unsigned int cmdlen, uint8_t *replyp);

static int iSRAMTest( unsigned long * ulStartAddr, unsigned long ulLength, unsigned long *ulErrAddr );


/* -------------------------------------------------------------------------- */
/* Static initialized data  */

static cmdmgr_cmdentry_t patch_cmdtable[] = {
    { VCSFW_CMD_SRAM_TEST, &patch_cmd_handler_sram_test }
};

/* -------------------------------------------------------------------------- */
/* Global functions */

static int iSRAMTest( unsigned long * ulStartAddr, unsigned long ulLength, unsigned long *ulErrAddr )
{
	unsigned long i, temp;

	// both of them qualified
#if 1 // write 0xAA then ox55 one by one
	for( i = 0; i < ulLength / 4; i++ )
	{
		temp = *( ulStartAddr + i );				// backup
		
		*( ulStartAddr + i ) = 0x0AAAAAAAA;
		if( *( ulStartAddr + i ) != 0x0AAAAAAAA )
		{
			*ulErrAddr = ( unsigned long )ulStartAddr + i * 4;
			*( ulStartAddr + i ) = temp;			// restore
			return 0xAA;
		}

		*( ulStartAddr + i ) = 0x055555555;
		if( *( ulStartAddr + i ) != 0x055555555 )
		{
			*ulErrAddr = ( unsigned long )ulStartAddr + i * 4;
			*( ulStartAddr + i ) = temp;			// restore
			return 0x55;
		}
		
		*( ulStartAddr + i ) = temp;				// restore
	}
#else // write 0xAA then 0x55 block by block
	// write 0xAA
	for( i = 0; i < ulLength / 4; i++ )
	{
#if 0	// avoid the address of used area: .rodata .text .stack .bss etc of sramtest itself and .bss and .stack of bootldr
		// boot loader stack:0x6000f320 bss:0x6000ffa0 size:0x44
		if( ulStartAddr + i >= 0x6000ce68 && ulStartAddr + i <= 0x6000cf00 || ulStartAddr + i >= 0x6000f320 )
			continue;
#endif	
		temp = *( ulStartAddr + i );
		*( ulStartAddr + i ) = 0x0AAAAAAAA;
		if( *( ulStartAddr + i ) != 0x0AAAAAAAA )
		{
			*ulErrAddr = ( unsigned long )ulStartAddr + i * 4;
			*( ulStartAddr + i ) = temp;
			return 0xAA;
		}
		*( ulStartAddr + i ) = temp;
	}

	// write 0x55
	for( i = 0; i < ulLength / 4; i++ )
	{
#if 0
		if( ulStartAddr + i >= 0x6000ce68 && ulStartAddr + i <= 0x6000cf00 || ulStartAddr + i >= 0x6000f320 )
			continue;
#endif
		temp = *( ulStartAddr + i );
		*( ulStartAddr + i ) = 0x055555555;
		if( *( ulStartAddr + i ) != 0x055555555 )
		{
			*ulErrAddr = ( unsigned long )ulStartAddr + i * 4;
			*( ulStartAddr + i ) = temp;
			return 0x55;
		}
		*( ulStartAddr + i ) = temp;
	}
#endif

#ifdef DEBUG
	// just for simulate sram error occur
	*ulErrAddr = ( unsigned long )ulStartAddr + 2 * 4;	
	return 1;
#endif

	return RAM_TEST_SUCCESS;
}

int
patch_start(void *ctxp, uint8_t *replyp)
{
    return cmdmgr(ctxp, &(patch_cmdtable[0]), NELEM(patch_cmdtable));
}

/* -------------------------------------------------------------------------- */
/* Static functions */

/*
 * Handler for VCSFW_CMD_SRAM_TEST.
 */
static int
patch_cmd_handler_sram_test(void *ctxp, const uint8_t *cmdbufp,
                              unsigned int cmdlen, uint8_t *replyp)
{
    int                              status_fw_bl_ram   = RAM_TEST_SUCCESS, 
									 status_lna_bl_ram  = RAM_TEST_SUCCESS,
									 status_adc_ram0    = RAM_TEST_SUCCESS, 
									 status_adc_ram1    = RAM_TEST_SUCCESS, 
									 status_adc_ram2    = RAM_TEST_SUCCESS, 
									 status_adc_ram3    = RAM_TEST_SUCCESS,
									 status_dm_ram      = RAM_TEST_SUCCESS,
									 status_cdm_ram     = RAM_TEST_SUCCESS,
									 status_scratch_ram = RAM_TEST_SUCCESS,
									 status_image_ram   = RAM_TEST_SUCCESS,
									 status_system_ram  = RAM_TEST_SUCCESS;

	unsigned long                    err_addr_fw_bl_ram   = 0,
									 err_addr_lna_bl_ram  = 0,
									 err_addr_adc_ram0    = 0,
									 err_addr_adc_ram1    = 0,
									 err_addr_adc_ram2    = 0,
									 err_addr_adc_ram3    = 0,
									 err_addr_dm_ram      = 0,
									 err_addr_cdm_ram     = 0,
									 err_addr_scratch_ram = 0,
									 err_addr_image_ram   = 0,
									 err_addr_system_ram  = 0;
		
	uint16_t *rp = ( uint16_t * ) replyp + 2;

	*(((uint16_t *) replyp) + 1 ) = VCSFW_STATUS_OKAY;

	status_fw_bl_ram   = iSRAMTest( ( unsigned long * )HWDEF_MEMMAP_FW_BL_RAM_BASE, HWDEF_MEMMAP_FW_BL_RAM_SIZE,     &err_addr_fw_bl_ram   );
	status_lna_bl_ram  = iSRAMTest( ( unsigned long * )HWDEF_MEMMAP_LNA_BL_RAM_BASE, HWDEF_MEMMAP_LNA_BL_RAM_BASE,   &err_addr_lna_bl_ram  );
	status_adc_ram0    = iSRAMTest( ( unsigned long * )HWDEF_MEMMAP_ADCRAM0_BASE, HWDEF_MEMMAP_ADCRAM0_SIZE,         &err_addr_adc_ram0    );
	status_adc_ram1    = iSRAMTest( ( unsigned long * )HWDEF_MEMMAP_ADCRAM1_BASE, HWDEF_MEMMAP_ADCRAM1_SIZE,         &err_addr_adc_ram1    );
	status_adc_ram2    = iSRAMTest( ( unsigned long * )HWDEF_MEMMAP_ADCRAM2_BASE, HWDEF_MEMMAP_ADCRAM2_SIZE,         &err_addr_adc_ram2    );
	status_adc_ram3    = iSRAMTest( ( unsigned long * )HWDEF_MEMMAP_ADCRAM3_BASE, HWDEF_MEMMAP_ADCRAM3_SIZE,         &err_addr_adc_ram3    );
	status_dm_ram      = iSRAMTest( ( unsigned long * )HWDEF_MEMMAP_DMRAM_BASE, HWDEF_MEMMAP_DMRAM_SIZE,             &err_addr_dm_ram      );
	status_cdm_ram     = iSRAMTest( ( unsigned long * )HWDEF_MEMMAP_CDMRAM_BASE, HWDEF_MEMMAP_CDMRAM_SIZE,           &err_addr_cdm_ram     );
	status_scratch_ram = iSRAMTest( ( unsigned long * )HWDEF_MEMMAP_SCRATCH_RAM_BASE, HWDEF_MEMMAP_SCRATCH_RAM_SIZE, &err_addr_scratch_ram );	
	status_image_ram   = iSRAMTest( ( unsigned long * )HWDEF_MEMMAP_IMGRAM_BASE, HWDEF_MEMMAP_IMGRAM_SIZE,           &err_addr_image_ram   );	
	status_system_ram  = iSRAMTest(	( unsigned long * )HWDEF_MEMMAP_SRAM_RAM_BASE, HWDEF_MEMMAP_SRAM_RAM_SIZE,       &err_addr_system_ram  );	
//	status_system_ram  = iSRAMTest(	( unsigned long * )(HWDEF_MEMMAP_SRAM_RAM_BASE + 0x800), HWDEF_MEMMAP_SRAM_RAM_SIZE - 0x800, &err_addr_system_ram );	

	if( status_fw_bl_ram != RAM_TEST_SUCCESS )
	{
		status_fw_bl_ram = FW_BL_RAM_TEST_ERROR;
	}
	if( status_lna_bl_ram != RAM_TEST_SUCCESS )
	{
		status_lna_bl_ram = LNA_BL_RAM_TEST_ERROR;
	}
	if( status_adc_ram0 != RAM_TEST_SUCCESS )
	{
		status_adc_ram0 = ADC_RAM0_TEST_ERROR;
	}
	if( status_adc_ram1 != RAM_TEST_SUCCESS )
	{
		status_adc_ram1 = ADC_RAM1_TEST_ERROR;
	}
	if( status_adc_ram2 != RAM_TEST_SUCCESS )
	{
		status_adc_ram2 = ADC_RAM2_TEST_ERROR;
	}
	if( status_adc_ram3 != RAM_TEST_SUCCESS )
	{
		status_adc_ram3 = ADC_RAM3_TEST_ERROR;
	}
	if( status_dm_ram != RAM_TEST_SUCCESS )
	{
		status_dm_ram = DM_RAM_TEST_ERROR;
	}
	if( status_cdm_ram != RAM_TEST_SUCCESS )
	{
		status_cdm_ram = CDM_RAM_TEST_ERROR;
	}
	if( status_scratch_ram != RAM_TEST_SUCCESS )
	{
		status_scratch_ram = SCRATCH_RAM_TEST_ERROR;
	}
	if( status_image_ram != RAM_TEST_SUCCESS )
	{
		status_image_ram = IMAGE_RAM_TEST_ERROR;
	}
	if( status_system_ram != RAM_TEST_SUCCESS )
	{
		status_system_ram = SYSTEM_RAM_TEST_ERROR;
	}

	*rp++ = status_fw_bl_ram;
	*rp++ = err_addr_fw_bl_ram;
	*rp++ = err_addr_fw_bl_ram >> 16;
	
	*rp++ = status_lna_bl_ram;
	*rp++ = err_addr_lna_bl_ram;
	*rp++ = err_addr_lna_bl_ram >> 16;
	
	*rp++ = status_adc_ram0;
	*rp++ = err_addr_adc_ram0;
	*rp++ = err_addr_adc_ram0 >> 16;
	
	*rp++ = status_adc_ram1;
	*rp++ = err_addr_adc_ram1;
	*rp++ = err_addr_adc_ram1 >> 16;
	
	*rp++ = status_adc_ram2;
	*rp++ = err_addr_adc_ram2;
	*rp++ = err_addr_adc_ram2 >> 16;
	
	*rp++ = status_adc_ram3;
	*rp++ = err_addr_adc_ram3;
	*rp++ = err_addr_adc_ram3 >> 16;
	
	*rp++ = status_dm_ram;
	*rp++ = err_addr_dm_ram;
	*rp++ = err_addr_dm_ram >> 16;
	
	*rp++ = status_cdm_ram;
	*rp++ = err_addr_cdm_ram;
	*rp++ = err_addr_cdm_ram >> 16;
	
	*rp++ = status_scratch_ram;
	*rp++ = err_addr_scratch_ram;
	*rp++ = err_addr_scratch_ram >> 16;
	
	*rp++ = status_image_ram;
	*rp++ = err_addr_image_ram;
	*rp++ = err_addr_image_ram >> 16;	

	*rp++ = status_system_ram;
	*rp++ = err_addr_system_ram;
	*rp++ = err_addr_system_ram >> 16;	

#ifndef DEBUG
	cmdmgr_reply( ctxp, (uint8_t *)(((uint16_t *) replyp)+1), sizeof(uint16_t) + 66 );
#else	// just for debug
	unsigned long                    i;

	for( i = 0; i < 16 * 1024 / 2; i++ )
	{
//		*rp++ = *(( unsigned short * )HWDEF_MEMMAP_FW_BL_RAM_BASE + i );
//		*rp++ = *(( unsigned short * )HWDEF_MEMMAP_LNA_BL_RAM_BASE + i );
//		*rp++ = *(( unsigned short * )HWDEF_MEMMAP_ADCRAM0_BASE + i );
//		*rp++ = *(( unsigned short * )HWDEF_MEMMAP_ADCRAM1_BASE + i );
//		*rp++ = *(( unsigned short * )HWDEF_MEMMAP_ADCRAM2_BASE + i );
//		*rp++ = *(( unsigned short * )HWDEF_MEMMAP_ADCRAM3_BASE + i );
//		*rp++ = *(( unsigned short * )HWDEF_MEMMAP_DMRAM_BASE + i );
//		*rp++ = *(( unsigned short * )HWDEF_MEMMAP_CDMRAM_BASE + i );
//		*rp++ = *(( unsigned short * )HWDEF_MEMMAP_SCRATCH_RAM_BASE + i );
//		*rp++ = *(( unsigned short * )HWDEF_MEMMAP_IMGRAM_BASE + i );
		*rp++ = *(( unsigned short * )( HWDEF_MEMMAP_SROM_RAM_BASE + 48 * 1024 ) + i );

		
	}
	cmdmgr_reply( ctxp, (uint8_t *)(((uint16_t *) replyp)+1), sizeof(uint16_t) + 66 + 16 * 1024 );	
#endif

    return -1;
}



