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

#include "sramtest.h"

//#define DEBUG
#define AVOID_CACHE

#ifdef DEBUG
//#define ERROR_SIMULATION
#endif


#ifdef AVOID_CACHE
#define FW_BL_RAM_ADDR 		0x80080000
#define LNA_BL_RAM_ADDR 	0x80088000
//#define ADC_RAM0_ADDR 		0x80090000
#define ADC_RAM1_ADDR 		0x8008f000
#define ADC_RAM2_ADDR 		0x80090000
//#define ADC_RAM3_ADDR 		0x80096000
#define DM_RAM_ADDR 		0x8008c000 
#define CDM_RAM_ADDR 		0x8008e000
//#define SCRATCH_RAM_ADDR 	0x8009B000
#define IMG_RAM_ADDR 		0x80084000
/* PUF RAM */
#define PUF_RAM_ADDR   		0x80091000
//#define HWDEF_MEMMAP_PUF_RAM_SIZE   0x00000080

#define PATCH_SIZE_EST 		0x838				// check the sram.verihex file: line38 60000836
#define SYSTEM_RAM_ADDR 	0x60000000+PATCH_SIZE_EST  //0x60000838

#define SYSTEM_RAM_HIGH_ADDR 0x6000e000
#define SYSTEM_RAM_HAGH_SIZE 0x6000f000 - SYSTEM_RAM_HIGH_ADDR

#if 0
#define RAM_SIZE_16KB 		0x3E80	
#define RAM_SIZE_8KB 		0x1F40
#define RAM_SIZE_6KB 		0x1770
#define RAM_SIZE_4KB 		0xFA0
#define RAM_SIZE_2KB 		0x7D0
#else
#define RAM_SIZE_16KB 		0x4000	
#define RAM_SIZE_8KB 		0x2000
#define RAM_SIZE_6KB 		0x1800
#define RAM_SIZE_4KB 		0x1000
#define RAM_SIZE_2KB 		0x800
#endif
#define RAM_SIZE_TEST 		0xd000-PATCH_SIZE_EST      //skip stack and patch areas; 0xd000 // check the sram.verihex file: line35 6000d020

#define PATTERN1 			0x55555555	
#define PATTERN2 			0xAAAAAAAA
#define PATTERN3 			0x00000000
#define PATTERN4 			0xFFFFFFFF
#define LOOPCOUNT     		5
#define ERRORS 	 	  		4

#define wr_word(a, d)          (*((volatile unsigned int*)(a))) = (unsigned int)(d)
#define rd_word(a, d)          d = (*((unsigned int*)(a)))

#ifdef LOW_SYSRAM_CHECK
uint32_t errcount = 0;    //error count variable;
uint32_t relbuff[] = {
	0x6000a768, 0x6000a950, 0x00000002, 0x50000b60, 0x80000004, 0x80000008, 0x80000230, 0x80000234, 0x80000238, 0x8000023c, 
	0x8000022c, 0x80003ff0, 0x80000000, 0x80001600, 0x8000160c, 0x80001610, 0x80002818, 0x8000030c, 0x6000a28c, 0x6000a970, 
	0x6000a954, 0x55555555, 0xaaaaaaaa, 0x80000344, 0x80000374, 0x50820808, 0x80000324, 0x3fffffff, 0x80000328, 0x8000493c, 
	0x6000a99c, 0x60000000, 0x00002000, 0x6000a9a0, 0x6000a7b4, 0x00026789, 0x80002800, 0x6000a970, 0x000000b0, 0xfff2444f, 
	0x00050020, 0x6000a7cd, 0x6000a758, 0x6000a83c, 0x6000a847, 0x50000b74, 0x29008136, 0x68513941, 0x69066841, 0x0020c001, 
	0x680020c0, 0x67515801, 0x20c01f15, 0x0020c000, 0xc00020c0, 0x20c00020, 0x0020c000, 0xc00020c0, 0xc6810020, 0x72170cff, 
	0xf01d0048, 0x29008136, 0x51513941, 0xe507ffc3, 0xa851b810, 0x11bbe041, 0xc0811c0c, 0xe01888ff, 0xbf710008, 0xc0160cff, 
	0x07690020, 0x0cffbe51, 0x0020c004, 0xbc310549, 0x390338ff, 0xffbaf101, 0x01e8220c, 0xc020ee20, 0x0fe90020, 0x7cffb8d1, 
	0x0020c0fc, 0xb6c10dc9, 0xc041b8ff, 0x0cb90020, 0xb4b151a8, 0xc041c8ff, 0x20c0a0aa, 0x910ba900, 0x180cffb2, 0x890020c0, 
	0xffb07109, 0xc0c2a062, 0x07690020, 0x0cffaf51, 0x0020c024, 0xab310549, 0xc3a022ff, 0x290020c0, 0xffa8f103, 0xc0f3a0e2, 
	0x0fe90020, 0xd8ffa1d1, 0xc801d90d, 0x12ec1701, 0xb8ff9eb1, 0x0c01b90b, 0x882a0c09, 0x1088a001, 0x31ec1897, 0xa022ff9e, 
	0x0020c0c4, 0x9cf10329, 0xc00e0cff, 0x0fe90020, 0xc2ff99d1, 0x20c0c5a0, 0x1d0dc900, 0x000000f0, 0x29008136, 0x69060c41, 
	0x48415801, 0x0eb45701, 0x0198f03d, 0x0199991b, 0x01784188, 0x1df03787, 0x000000f0, 0x29006136, 0x655a0c01, 0x8961fffd, 
	0xfea052ff, 0x590020c0, 0x48050c06, 0x0c945701, 0x0cff8781, 0x0020c017, 0x02460879, 0xff83a100, 0x20c0090c, 0x0c0a9900, 
	0xfffa655a, 0xb2ff7ec1, 0x20c0ffa0, 0x1d0cb900, 0x000000f0, 0x29006136, 0xff7b5101, 0x20c00148, 0x0c054900, 0xfff8255a, 
	0x52ff7561, 0x20c0fda0, 0x0c065900, 0xfff7255a, 0x0000f01d, 0x0c004136, 0xfff8a50a, 0xfce51a0c, 0xa50a0cff, 0x1a0cfffc, 
	0x0cfffc25, 0xfff7651a, 0x0000f01d, 0x31004136, 0x024cff6b, 0x290020c0, 0x00f01d03, 0x71006136, 0x0778ff68, 0x66510179, 
	0x48260cff, 0x20446001, 0x490020c0, 0xff633105, 0x20c0320c, 0x0c032900, 0xff61b18a, 0x0c0045a5, 0xfff0e55a, 0xb9ff60b1, 
	0xa90a0c11, 0x0c219821, 0x183897a8, 0x020c1138, 0x11f80329, 0x11f9ff4b, 0xee1b21e8, 0x21d821e9, 0xbcd7ac0c, 0xff55a1e6, 
	0x510033e5, 0xa042ff4c, 0x0020c0a5, 0x2a3c0549, 0x61ffece5, 0x1169ff50, 0xbb0c11a8, 0x78ffdde5, 0x66077811, 0x43910d17, 
	0xaaa082ff, 0x890020c0, 0x00024609, 0x5cff40b1, 0x0020c05a, 0xf01d0ba9, 0x29008136, 0x68513941, 0x69066841, 0x0020c001, 
	0x680020c0, 0x67515801, 0x20c01f15, 0x0020c000, 0xc00020c0, 0x20c00020, 0x0020c000, 0xc00020c0, 0x39810020, 0x72170cff, 
	0xf01d0048, 0x29008136, 0x49513941, 0x99090c61, 0x89080c01, 0x78018801, 0x41727051, 0x0602b787, 0x5c0c002f, 0xbbc001b8, 
	0xa85c0ce2, 0xe2aac061, 0xd8129ab7, 0xf801e801, 0xa0eef041, 0xfa11fd00, 0x0020c0dd, 0x540c0ed9, 0x331b6138, 0x0ce23340, 
	0x40012854, 0x9237e222, 0x6801580e, 0xa0556041, 0xc0ff2141, 0x05490020, 0x6188590c, 0x8890882b, 0x78590ce2, 0xe2779001, 
	0xa80e9787, 0xb041b801, 0x1991a0aa, 0x0020c0ff, 0x5e0c0a99, 0xdd3b61d8, 0x0ce2dde0, 0xe001c85e, 0x9cd7e2cc, 0x2801f80d, 
	0xa0ff2041, 0x20c00e0c, 0x0c0fe900, 0x4b614855, 0xe2445044, 0x0138550c, 0x47e23350, 0x01680d93, 0x66704178, 0xc0f57ca0, 
	0x06590020, 0x991b0198, 0x01980199, 0x82805188, 0x02389741, 0x1dffd086, 0x000000f0, 0x29008136, 0x49513941, 0x0c715961, 
	0x0c01b90b, 0x9801a90a, 0x41929051, 0x38970188, 0x006fc602, 0x412801f8, 0xf8a0ff20, 0x0c11f90f, 0xe001d85e, 0x5e0ce2dd, 
	0xcce061c8, 0x449cd7e2, 0x54000148, 0x38445a11, 0x38134711, 0x68feed61, 0x2446f616, 0xeaa10198, 0xb81aa8fe, 0xb0aab071, 
	0x41b8aa8b, 0x99a099b0, 0xfee5810a, 0x71981888, 0xcbb08890, 0x79117888, 0xfee1c108, 0xb8fee1b1, 0xb9bb1b1b, 0xd85e0c1c, 
	0xe0dd1b61, 0x5e0ce2dd, 0xcce001c8, 0x409cd7e2, 0xe8fedbf1, 0x381ef711, 0x28fed821, 0x2442f612, 0xd5610158, 0x781668fe, 
	0xb0667071, 0x4178668b, 0x59a05570, 0xfed04106, 0x71581448, 0xcbb04450, 0x39113844, 0xfecc8104, 0x78fecc71, 0x79771b17, 
	0x985a0c18, 0xa0992b61, 0x5a0ce299, 0x88a00188, 0x409897e2, 0xa8fec7b1, 0x381ab711, 0xc8fec3c1, 0x244cf61c, 0xc02101f8, 
	0x381228fe, 0xb0223071, 0x4138228b, 0xf9a0ff30, 0xfebbe102, 0x71f81ee8, 0xcbb0eef0, 0xd911d8ee, 0xfeb7410e, 0x38feb731, 
	0x39331b13, 0x58560c14, 0x60553b61, 0x560ce255, 0x44600148, 0x3f9457e2, 0x1168070c, 0x81381677, 0x1888feae, 0xb82448f6, 
	0xfeabc101, 0x71d81cc8, 0x8bb0ccd0, 0xd041d8cc, 0x0cb9a0bb, 0xa8fea7a1, 0xb071b81a, 0xaacbb0aa, 0x0a991198, 0xd1fea3e1, 
	0x1dd8fea2, 0x1ed9dd1b, 0x61f8520c, 0xff20ff4b, 0xe8520ce2, 0xe2ee2001, 0x283d9ef7, 0x38022611, 0x38fe9a31, 0x2443f613, 
	0x97710168, 0x881778fe, 0xb0778071, 0x4188778b, 0x69a06680, 0xfe925107, 0x71681558, 0xcbb05560, 0x49114855, 0xfe8e9105, 
	0x88fe8e81, 0x89881b18, 0x1b01b819, 0xa801b9bb, 0x41a2a051, 0xb9a70198, 0xff8fc602, 0x0000f01d, 0x29008136, 0xfe87b141, 
	0x01b90bb8, 0x82a101b8, 0xb02aa8fe, 0x01a920aa, 0x88fe8391, 0x0020c001, 0x81710989, 0xc0160cfe, 0x07690020, 0x58fe8051, 
	0x48115905, 0x154e4011, 0xc807b426, 0x15cec011, 0xe1121c66, 0x7bf1fe7b, 0xf011d8fe, 0x20c010dd, 0x060ed900, 0x76310003, 
	0xfe6f21fe, 0x20c03228, 0xa1032900, 0x6c91fe75, 0xc04998fe, 0x0a990020, 0x81fe7391, 0x5888fe69, 0x890020c0, 0xfe707109, 
	0x0769060c, 0x2159050c, 0x2149040c, 0x53f62138, 0xfe6ca122, 0xc8fe6db1, 0xffca2521, 0xb1fe6aa1, 0x21c8fe6a, 0xd76541d8, 
	0x1b21f8ff, 0xe821f9ff, 0xdc5eb621, 0x5821030c, 0x371228fe, 0x41480f92, 0x4138150c, 0x33500338, 0x06043920, 0x41680003, 
	0x4158270c, 0x55700558, 0x88065920, 0x71884b41, 0x1778fe4e, 0xf01d0879, 0x29006136, 0xc8113901, 0xfe56d101, 0xb8a0ccd0, 
	0xa80cb911, 0xfe54b101, 0x88fe3581, 0x0008e048, 0x0000f01d, 0x29006136, 0x8101a801, 0x5888fe30, 0x1d0008e0, 0x000000f0, 
	0x29006136, 0xfe2b8101, 0x08e03888, 0x00f01d00, 0x21004136, 0xe227fe27, 0xfe3a6114, 0xc0fe4651, 0x06590020, 0x0cfe4541, 
	0x0020c003, 0x20710439, 0x0ae717fe, 0x0cfe42a1, 0xfe41c10b, 0x810012e5, 0xe807fe1c, 0xfe3fa10a, 0x88fe1b81, 0x0008e008, 
	0x0b0c0a0c, 0x1dffaee5, 0x000000f0, 0x20e1c222, 0x31496020, 0x59f0c112, 0xfe370111, 0x00200030, 0x010961e6, 0x00fe3601, 
	0x00a0b003, 0x03b10000, 0x00000c46, 0x03b20000, 0x00000a46, 0x03b30000, 0x00000846, 0x03b40000, 0x00000646, 0x03b50000, 
	0x00000446, 0x03b60000, 0x00000246, 0x03b70000, 0x00000046, 0x12210900, 0x201050c1, 0x36024d00, 0x03c000a1, 0xe131c903, 
	0x18f1fe22, 0xa0f0f0fe, 0xfef00ff8, 0xf000ed83, 0x31e8000f, 0x011303e0, 0xf01dfe1d, 0x25080528, 0x1013e620, 0x1a210020, 
	0xb02320fe, 0x000002a0, 0x0c4613b1, 0x00000000, 0x0a4613b2, 0x00000000, 0x084613b3, 0x00000000, 0x064613b4, 0x00000000, 
	0x044613b5, 0x00000000, 0x024613b6, 0x00000000, 0x004613b7, 0x12000000, 0x155810c5, 0x09213148, 0xa02320fe, 0x02a00228, 
	0x00000000, 0x04947600, 0x1b004532, 0x00f01d55, 0x32f184b6, 0x440b0045, 0x6517551b, 0xe484b628, 0x42005532, 0x552bfec4, 
	0x00000686, 0x30002136, 0x025d7430, 0x70117380, 0x73002033, 0x20337011, 0x17cce207, 0x7440d6e2, 0x0a977641, 0x15390539, 
	0x35392539, 0x3710c552, 0x05390664, 0xc5521539, 0x03642708, 0x554b0539, 0x32046417, 0x552b0055, 0x32026407, 0xf01d0045, 
	0x20002136, 0x73402052, 0x12977641, 0x15390539, 0x35392539, 0x55394539, 0x75396539, 0x2720c552, 0x05390a64, 0x25391539, 
	0xc5523539, 0x07641710, 0x39006532, 0x08c55215, 0x39016407, 0x00f01d05, 0x00000000, 0x00000000, 0x00000000, 0x00020000, 
	0x000013eb, 0x00000012, 0x00002568};
#endif
#endif

/* -------------------------------------------------------------------------- */
/* Static prototypes */

static int patch_cmd_handler_sram_test(void *ctxp, const uint8_t *cmdbufp,
                                         unsigned int cmdlen, uint8_t *replyp);

static int iSRAMTest( unsigned long * ulStartAddr, unsigned long ulLength, unsigned long *ulErrAddr );

/* -------------------------------------------------------------------------- */
/* Static uninitialized data  */




/* -------------------------------------------------------------------------- */
/* Static initialized data  */

static cmdmgr_cmdentry_t patch_cmdtable[] = {
    { VCSFW_CMD_SRAM_TEST, &patch_cmd_handler_sram_test }
};

/* -------------------------------------------------------------------------- */
/* Global functions */

#ifdef AVOID_CACHE
//memory fill function 
void Mem_Fill(uint32_t address, uint32_t size, uint32_t j){
	uint32_t i = 0;   	
	for (i=0; i<= size/4; i++){
		if (i%LOOPCOUNT == (0+j)%LOOPCOUNT) wr_word(address+i*4, i+i*0x10000); // i*0x10000 covers the higher 16 bits, i covers the lower 16 bits
		if (i%LOOPCOUNT == (1+j)%LOOPCOUNT) wr_word(address+i*4, PATTERN1);  // 0101 0101 0101 0101 0101 0101 0101 0101
		if (i%LOOPCOUNT == (2+j)%LOOPCOUNT) wr_word(address+i*4, PATTERN2);  // 1010 1010 1010 1010 1010 1010 1010 1010
		if (i%LOOPCOUNT == (3+j)%LOOPCOUNT) wr_word(address+i*4, PATTERN3);  // 0000 0000 0000 0000 0000 0000 0000 0000
		if (i%LOOPCOUNT == (4+j)%LOOPCOUNT) wr_word(address+i*4, PATTERN4);  // 1111 1111 1111 1111 1111 1111 1111 1111
	}
}

//memory check function 
int Mem_Check(uint32_t address, uint32_t size, uint32_t j, uint32_t *err_addr)
{
	uint32_t i = 0;
	uint32_t mem;  

	for (i=0; i< size/4; i++)
	{
		rd_word(address+i*4, mem);
		
		if (i%LOOPCOUNT == (0+j)%LOOPCOUNT) // check increase number
		{
			if( mem != i + ( i << 16 ) ) 
			{
				*err_addr = address + i * 4;
				return RAM_TEST_FAIL;
			}
		}
		
		if (i%LOOPCOUNT == (1+j)%LOOPCOUNT) // check PATTERN1
		{            	
			if (mem != PATTERN1) 
			{
				*err_addr = address + i * 4;
				return RAM_TEST_FAIL;
			}
		}
		
		if (i%LOOPCOUNT == (2+j)%LOOPCOUNT)  // check PATTERN2
		{            	
			if (mem != PATTERN2) 
			{
				*err_addr = address + i * 4;
				return RAM_TEST_FAIL;
			}
		}
		
		if (i%LOOPCOUNT == (3+j)%LOOPCOUNT)  // check PATTERN3
		{            	
			if (mem != PATTERN3) 
			{
				*err_addr = address + i * 4;
				return RAM_TEST_FAIL;
			}
		}
		
		if (i%LOOPCOUNT == (4+j)%LOOPCOUNT)  // check PATTERN4
		{            	
			if (mem != PATTERN4) 
			{
				*err_addr = address + i * 4;
				return RAM_TEST_FAIL;
			}
		}
	}

	return 0;

} 
#endif

#ifndef AVOID_CACHE
static int iSRAMTest( unsigned long * ulStartAddr, unsigned long ulLength, unsigned long *ulErrAddr )
{
	unsigned long i,j, temp;

	// both of them qualified
#if 0 // write 0xAA then ox55 one by one, UNUSABLE BECAUSE OF DATA CACHE
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

		*( ulStartAddr + i ) = i + i << 16;
		if( *( ulStartAddr + i ) != i + i << 16 )
		{
			*ulErrAddr = ( unsigned long )ulStartAddr + i * 4;
			*( ulStartAddr + i ) = temp;			// restore
			return 0x12;
		}


#ifdef TEST_FROM_0X00000000_TO_0XFFFFFFFF
		*( ulStartAddr + i ) = 0x00000000;
		if( *( ulStartAddr + i ) != 0x00000000 )
		{
			*ulErrAddr = ( unsigned long )ulStartAddr + i * 4;
			*( ulStartAddr + i ) = temp;			// restore
			return 0x10;
		}

		*( ulStartAddr + i ) = 0x11111111;
		if( *( ulStartAddr + i ) != 0x11111111 )
		{
			*ulErrAddr = ( unsigned long )ulStartAddr + i * 4;
			*( ulStartAddr + i ) = temp;			// restore
			return 0x11;
		}

		*( ulStartAddr + i ) = 0x22222222;
		if( *( ulStartAddr + i ) != 0x22222222 )
		{
			*ulErrAddr = ( unsigned long )ulStartAddr + i * 4;
			*( ulStartAddr + i ) = temp;			// restore
			return 0x22;
		}

		*( ulStartAddr + i ) = 0x33333333;
		if( *( ulStartAddr + i ) != 0x33333333 )
		{
			*ulErrAddr = ( unsigned long )ulStartAddr + i * 4;
			*( ulStartAddr + i ) = temp;			// restore
			return 0x33;
		}

		*( ulStartAddr + i ) = 0x44444444;
		if( *( ulStartAddr + i ) != 0x44444444 )
		{
			*ulErrAddr = ( unsigned long )ulStartAddr + i * 4;
			*( ulStartAddr + i ) = temp;			// restore
			return 0x44;
		}

		*( ulStartAddr + i ) = 0x66666666;
		if( *( ulStartAddr + i ) != 0x66666666 )
		{
			*ulErrAddr = ( unsigned long )ulStartAddr + i * 4;
			*( ulStartAddr + i ) = temp;			// restore
			return 0x66;
		}

		*( ulStartAddr + i ) = 0x77777777;
		if( *( ulStartAddr + i ) != 0x77777777 )
		{
			*ulErrAddr = ( unsigned long )ulStartAddr + i * 4;
			*( ulStartAddr + i ) = temp;			// restore
			return 0x77;
		}

		*( ulStartAddr + i ) = 0x88888888;
		if( *( ulStartAddr + i ) != 0x88888888 )
		{
			*ulErrAddr = ( unsigned long )ulStartAddr + i * 4;
			*( ulStartAddr + i ) = temp;			// restore
			return 0x88;
		}

		*( ulStartAddr + i ) = 0x99999999;
		if( *( ulStartAddr + i ) != 0x99999999 )
		{
			*ulErrAddr = ( unsigned long )ulStartAddr + i * 4;
			*( ulStartAddr + i ) = temp;			// restore
			return 0x99;
		}

		*( ulStartAddr + i ) = 0x0bbbbbbbb;
		if( *( ulStartAddr + i ) != 0x0bbbbbbbb )
		{
			*ulErrAddr = ( unsigned long )ulStartAddr + i * 4;
			*( ulStartAddr + i ) = temp;			// restore
			return 0x0bb;
		}

		*( ulStartAddr + i ) = 0x0cccccccc;
		if( *( ulStartAddr + i ) != 0x0cccccccc )
		{
			*ulErrAddr = ( unsigned long )ulStartAddr + i * 4;
			*( ulStartAddr + i ) = temp;			// restore
			return 0x0cc;
		}

		*( ulStartAddr + i ) = 0x0dddddddd;
		if( *( ulStartAddr + i ) != 0x0dddddddd )
		{
			*ulErrAddr = ( unsigned long )ulStartAddr + i * 4;
			*( ulStartAddr + i ) = temp;			// restore
			return 0x0dd;
		}

		*( ulStartAddr + i ) = 0x0eeeeeeee;
		if( *( ulStartAddr + i ) != 0x0eeeeeeee )
		{
			*ulErrAddr = ( unsigned long )ulStartAddr + i * 4;
			*( ulStartAddr + i ) = temp;			// restore
			return 0x0ee;
		}

		*( ulStartAddr + i ) = 0x0ffffffff;
		if( *( ulStartAddr + i ) != 0x0ffffffff )
		{
			*ulErrAddr = ( unsigned long )ulStartAddr + i * 4;
			*( ulStartAddr + i ) = temp;			// restore
			return 0x0ff;
		}

		*( ulStartAddr + i ) = 0x5a5a5a5a;
		if( *( ulStartAddr + i ) != 0x5a5a5a5a )
		{
			*ulErrAddr = ( unsigned long )ulStartAddr + i * 4;
			*( ulStartAddr + i ) = temp;			// restore
			return 0x0ff;
		}
		
		*( ulStartAddr + i ) = 0x12488421;
		if( *( ulStartAddr + i ) != 0x12488421 )
		{
			*ulErrAddr = ( unsigned long )ulStartAddr + i * 4;
			*( ulStartAddr + i ) = temp;			// restore
			return 0x0ff;
		}

		*( ulStartAddr + i ) = 0x12345678;
		if( *( ulStartAddr + i ) != 0x12345678 )
		{
			*ulErrAddr = ( unsigned long )ulStartAddr + i * 4;
			*( ulStartAddr + i ) = temp;			// restore
			return 0x0ff;
		}

#endif
		
		*( ulStartAddr + i ) = temp;				// restore
	}
#else // write 0xAA then 0x55 block by block
	// write 0xAA
	for( i = 0; i < ulLength / 4; i++ )
	{
#if 1	// avoid the address of used area: .rodata .text .stack .bss etc of sramtest itself and .bss and .stack of bootldr
		// boot loader stack:0x6000f320 bss:0x6000ffa0 size:0x44
		//if( ulStartAddr + i >= 0x6000ce68 && ulStartAddr + i <= 0x6000cf00 || ulStartAddr + i >= 0x6000f320 )
		if(  ( ( ( ulStartAddr + i ) >= 0x60000000 ) && ( ( ulStartAddr + i ) < 0x60000900 )) || \
			 ( (( ulStartAddr + i ) >= 0x6000cf00 ) && ( ( ulStartAddr + i ) <= 0x6000d100 )) || \
			 ( (( ulStartAddr + i ) >= 0x6000fe00 ) && ( ( ulStartAddr + i ) <= 0x60010000 )))
			continue;
#endif	
		//if(  ( ( ulStartAddr + i ) >= 0x60000900 ) && ( ( ulStartAddr + i ) <= 0x6000cf00 ) || ( ( ulStartAddr + i ) >= 0x80000000 ))
		{
			*( ulStartAddr + i ) = 0x0AAAAAAAA;
		}
	}

	for( i = 0; i < ulLength / 4; i++ )
	{
#if 1	// avoid the address of used area: .rodata .text .stack .bss etc of sramtest itself and .bss and .stack of bootldr
		// boot loader stack:0x6000f320 bss:0x6000ffa0 size:0x44
		//if( ulStartAddr + i >= 0x6000ce68 && ulStartAddr + i <= 0x6000cf00 || ulStartAddr + i >= 0x6000f320 )
		if(  ( ( ( ulStartAddr + i ) >= 0x60000000 ) && ( ( ulStartAddr + i ) < 0x60000900 )) || \
			 ( (( ulStartAddr + i ) >= 0x6000cf00 ) && ( ( ulStartAddr + i ) <= 0x6000d100 )) || \
			 ( (( ulStartAddr + i ) >= 0x6000fe00 ) && ( ( ulStartAddr + i ) <= 0x60010000 )))
			continue;
#endif	
		if( *( ulStartAddr + i ) != 0x0AAAAAAAA )
		{
			*ulErrAddr = ulStartAddr + i;
			return RAM_TEST_FAIL;
		}
	}

	
	// write 0x55
		for( i = 0; i < ulLength / 4; i++ )
		{
#if 1	// avoid the address of used area: .rodata .text .stack .bss etc of sramtest itself and .bss and .stack of bootldr
			// boot loader stack:0x6000f320 bss:0x6000ffa0 size:0x44
			//if( ulStartAddr + i >= 0x6000ce68 && ulStartAddr + i <= 0x6000cf00 || ulStartAddr + i >= 0x6000f320 )
			if(  ( ( ( ulStartAddr + i ) >= 0x60000000 ) && ( ( ulStartAddr + i ) < 0x60000900 )) || \
			 ( (( ulStartAddr + i ) >= 0x6000cf00 ) && ( ( ulStartAddr + i ) <= 0x6000d100 )) || \
			 ( (( ulStartAddr + i ) >= 0x6000fe00 ) && ( ( ulStartAddr + i ) <= 0x60010000 )))
			continue;
#endif	
			
			*( ulStartAddr + i ) = 0x55555555;
		}
	
		for( i = 0; i < ulLength / 4; i++ )
		{
#if 1	// avoid the address of used area: .rodata .text .stack .bss etc of sramtest itself and .bss and .stack of bootldr
			// boot loader stack:0x6000f320 bss:0x6000ffa0 size:0x44
			//if( ulStartAddr + i >= 0x6000ce68 && ulStartAddr + i <= 0x6000cf00 || ulStartAddr + i >= 0x6000f320 )
			if(  ( ( ( ulStartAddr + i ) >= 0x60000000 ) && ( ( ulStartAddr + i ) < 0x60000900 )) || \
			 ( (( ulStartAddr + i ) >= 0x6000cf00 ) && ( ( ulStartAddr + i ) <= 0x6000d100 )) || \
			 ( (( ulStartAddr + i ) >= 0x6000fe00 ) && ( ( ulStartAddr + i ) <= 0x60010000 )))
			continue;
#endif	
			if( *( ulStartAddr + i ) != 0x55555555 )
			{
				*ulErrAddr = ulStartAddr + i;
				return RAM_TEST_FAIL;
			}
		}

	// write 0x00
		for( i = 0; i < ulLength / 4; i++ )
		{
#if 1	// avoid the address of used area: .rodata .text .stack .bss etc of sramtest itself and .bss and .stack of bootldr
			// boot loader stack:0x6000f320 bss:0x6000ffa0 size:0x44
			//if( ulStartAddr + i >= 0x6000ce68 && ulStartAddr + i <= 0x6000cf00 || ulStartAddr + i >= 0x6000f320 )
			if(  ( ( ( ulStartAddr + i ) >= 0x60000000 ) && ( ( ulStartAddr + i ) < 0x60000900 )) || \
			 ( (( ulStartAddr + i ) >= 0x6000cf00 ) && ( ( ulStartAddr + i ) <= 0x6000d100 )) || \
			 ( (( ulStartAddr + i ) >= 0x6000fe00 ) && ( ( ulStartAddr + i ) <= 0x60010000 )))
			continue;
#endif	
			
			*( ulStartAddr + i ) = 0x00000000;
		}
	
		for( i = 0; i < ulLength / 4; i++ )
		{
#if 1	// avoid the address of used area: .rodata .text .stack .bss etc of sramtest itself and .bss and .stack of bootldr
			// boot loader stack:0x6000f320 bss:0x6000ffa0 size:0x44
			//if( ulStartAddr + i >= 0x6000ce68 && ulStartAddr + i <= 0x6000cf00 || ulStartAddr + i >= 0x6000f320 )
			if(  ( ( ( ulStartAddr + i ) >= 0x60000000 ) && ( ( ulStartAddr + i ) < 0x60000900 )) || \
			 ( (( ulStartAddr + i ) >= 0x6000cf00 ) && ( ( ulStartAddr + i ) <= 0x6000d100 )) || \
			 ( (( ulStartAddr + i ) >= 0x6000fe00 ) && ( ( ulStartAddr + i ) <= 0x60010000 )))
			continue;
#endif	
			if( *( ulStartAddr + i ) != 0x00000000 )
			{
				*ulErrAddr = ulStartAddr + i;
				return RAM_TEST_FAIL;
			}
		}

		// write 0x00
		for( i = 0; i < ulLength / 4; i++ )
		{
#if 1	// avoid the address of used area: .rodata .text .stack .bss etc of sramtest itself and .bss and .stack of bootldr
			// boot loader stack:0x6000f320 bss:0x6000ffa0 size:0x44
			//if( ulStartAddr + i >= 0x6000ce68 && ulStartAddr + i <= 0x6000cf00 || ulStartAddr + i >= 0x6000f320 )
			if(  ( ( ( ulStartAddr + i ) >= 0x60000000 ) && ( ( ulStartAddr + i ) < 0x60000900 )) || \
			 ( (( ulStartAddr + i ) >= 0x6000cf00 ) && ( ( ulStartAddr + i ) <= 0x6000d100 )) || \
			 ( (( ulStartAddr + i ) >= 0x6000fe00 ) && ( ( ulStartAddr + i ) <= 0x60010000 )))
			continue;
#endif	
			
			*( ulStartAddr + i ) = 0x0ffffffff;
		}
	
		for( i = 0; i < ulLength / 4; i++ )
		{
#if 1	// avoid the address of used area: .rodata .text .stack .bss etc of sramtest itself and .bss and .stack of bootldr
			// boot loader stack:0x6000f320 bss:0x6000ffa0 size:0x44
			//if( ulStartAddr + i >= 0x6000ce68 && ulStartAddr + i <= 0x6000cf00 || ulStartAddr + i >= 0x6000f320 )
			if(  ( ( ( ulStartAddr + i ) >= 0x60000000 ) && ( ( ulStartAddr + i ) < 0x60000900 )) || \
			 ( (( ulStartAddr + i ) >= 0x6000cf00 ) && ( ( ulStartAddr + i ) <= 0x6000d100 )) || \
			 ( (( ulStartAddr + i ) >= 0x6000fe00 ) && ( ( ulStartAddr + i ) <= 0x60010000 )))
			continue;
#endif	
			if( *( ulStartAddr + i ) != 0x0ffffffff )
			{
				*ulErrAddr = ulStartAddr + i;
				return RAM_TEST_FAIL;
			}
		}
		
#endif

#ifdef DEBUG
	// just for simulate sram error occur
	*ulErrAddr = ( unsigned long )ulStartAddr + 2 * 4;	
	return 1;
#endif

	return RAM_TEST_SUCCESS;
}
#endif


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
									 //status_adc_ram0    = RAM_TEST_SUCCESS, 
									 status_adc_ram1    = RAM_TEST_SUCCESS, 
									 status_adc_ram2    = RAM_TEST_SUCCESS, 
									 //status_adc_ram3    = RAM_TEST_SUCCESS,
									 status_dm_ram      = RAM_TEST_SUCCESS,
									 status_cdm_ram     = RAM_TEST_SUCCESS,
									 //status_scratch_ram = RAM_TEST_SUCCESS,
									 status_image_ram   = RAM_TEST_SUCCESS,
									 status_system_ram  = RAM_TEST_SUCCESS,
									 status_puf_ram		= RAM_TEST_SUCCESS;

	unsigned long                    err_addr_fw_bl_ram   = 0,
									 err_addr_lna_bl_ram  = 0,
									 //err_addr_adc_ram0    = 0,
									 err_addr_adc_ram1    = 0,
									 err_addr_adc_ram2    = 0,
									 //err_addr_adc_ram3    = 0,
									 err_addr_dm_ram      = 0,
									 err_addr_cdm_ram     = 0,
									 //err_addr_scratch_ram = 0,
									 err_addr_image_ram   = 0,
									 err_addr_system_ram  = 0,
									 err_addr_puf_ram  = 0;
		
	uint16_t *rp = ( uint16_t * ) replyp + 2;

	*(((uint16_t *) replyp) + 1 ) = VCSFW_STATUS_OKAY;
	*rp++ = 9;	// 9 block of SRAM, 2byte error code and 4byte error address 

#ifndef AVOID_CACHE
	status_fw_bl_ram   = iSRAMTest( ( unsigned long * )HWDEF_MEMMAP_FW_BL_RAM_BASE, HWDEF_MEMMAP_FW_BL_RAM_SIZE,     &err_addr_fw_bl_ram   );
	status_lna_bl_ram  = iSRAMTest( ( unsigned long * )HWDEF_MEMMAP_LNA_BL_RAM_BASE, HWDEF_MEMMAP_LNA_BL_RAM_BASE,   &err_addr_lna_bl_ram  );
	//status_adc_ram0    = iSRAMTest( ( unsigned long * )HWDEF_MEMMAP_ADCRAM0_BASE, HWDEF_MEMMAP_ADCRAM0_SIZE,         &err_addr_adc_ram0    );
	status_adc_ram1    = iSRAMTest( ( unsigned long * )HWDEF_MEMMAP_ADCRAM1_BASE, HWDEF_MEMMAP_ADCRAM1_SIZE,         &err_addr_adc_ram1    );
	status_adc_ram2    = iSRAMTest( ( unsigned long * )HWDEF_MEMMAP_ADCRAM2_BASE, HWDEF_MEMMAP_ADCRAM2_SIZE,         &err_addr_adc_ram2    );
	//status_adc_ram3    = iSRAMTest( ( unsigned long * )HWDEF_MEMMAP_ADCRAM3_BASE, HWDEF_MEMMAP_ADCRAM3_SIZE,         &err_addr_adc_ram3    );
	status_dm_ram      = iSRAMTest( ( unsigned long * )HWDEF_MEMMAP_DMRAM_BASE, HWDEF_MEMMAP_DMRAM_SIZE,             &err_addr_dm_ram      );
	status_cdm_ram     = iSRAMTest( ( unsigned long * )HWDEF_MEMMAP_CDMRAM_BASE, HWDEF_MEMMAP_CDMRAM_SIZE,           &err_addr_cdm_ram     );
	//status_scratch_ram = iSRAMTest( ( unsigned long * )HWDEF_MEMMAP_SCRATCH_RAM_BASE, HWDEF_MEMMAP_SCRATCH_RAM_SIZE, &err_addr_scratch_ram );	
	status_image_ram   = iSRAMTest( ( unsigned long * )HWDEF_MEMMAP_IMGRAM_BASE, HWDEF_MEMMAP_IMGRAM_SIZE,           &err_addr_image_ram   );	
	status_system_ram  = iSRAMTest(	( unsigned long * )HWDEF_MEMMAP_SRAM_RAM_BASE, HWDEF_MEMMAP_SRAM_RAM_SIZE,       &err_addr_system_ram  );	
//	status_system_ram  = iSRAMTest(	( unsigned long * )(HWDEF_MEMMAP_SRAM_RAM_BASE + 0x800), HWDEF_MEMMAP_SRAM_RAM_SIZE - 0x800, &err_addr_system_ram );	

	status_puf_ram 	   = iSRAMTest( ( unsigned long * )HWDEF_MEMMAP_PUF_RAM_BASE, HWDEF_MEMMAP_PUF_RAM_SIZE, &err_addr_puf_ram ); 
#else
		uint32_t loop = 0;	
		//memory fill at designated RAM locations:	  
		for (loop=0; loop<LOOPCOUNT; loop++){	//LOOPCOUNT
			Mem_Fill(FW_BL_RAM_ADDR, RAM_SIZE_16KB,loop);
			Mem_Fill(LNA_BL_RAM_ADDR + 4, RAM_SIZE_16KB - 4,loop);
			//Mem_Fill(ADC_RAM0_ADDR, RAM_SIZE_4KB,loop);
			Mem_Fill(ADC_RAM1_ADDR + 4, RAM_SIZE_4KB - 4,loop); 
			Mem_Fill(ADC_RAM2_ADDR, RAM_SIZE_4KB,loop);
			//Mem_Fill(ADC_RAM3_ADDR, RAM_SIZE_4KB,loop); 		  
			Mem_Fill(DM_RAM_ADDR, RAM_SIZE_6KB,loop);
			Mem_Fill(CDM_RAM_ADDR, RAM_SIZE_4KB,loop);
			//Mem_Fill(SCRATCH_RAM_ADDR, RAM_SIZE_4KB,loop);
			Mem_Fill(IMG_RAM_ADDR, RAM_SIZE_16KB,loop);
			Mem_Fill(SYSTEM_RAM_ADDR, RAM_SIZE_TEST,loop);
			Mem_Fill(SYSTEM_RAM_HIGH_ADDR, SYSTEM_RAM_HAGH_SIZE,loop);
			Mem_Fill(PUF_RAM_ADDR + 4, HWDEF_MEMMAP_PUF_RAM_SIZE - 4,loop);
			
		//memory Check at designated locations: 
			if( status_fw_bl_ram == RAM_TEST_SUCCESS )
			{
				status_fw_bl_ram   = Mem_Check( FW_BL_RAM_ADDR, RAM_SIZE_16KB,loop, &err_addr_fw_bl_ram   );
			}
	
			if( status_lna_bl_ram == RAM_TEST_SUCCESS )
			{
				status_lna_bl_ram  = Mem_Check( LNA_BL_RAM_ADDR, RAM_SIZE_16KB,loop,&err_addr_lna_bl_ram  );
				//status_lna_bl_ram  = Mem_Check( LNA_BL_RAM_ADDR + 4, RAM_SIZE_16KB - 4,loop,&err_addr_lna_bl_ram  );
			}
	
			//if( status_adc_ram0 == RAM_TEST_SUCCESS )
			//{
			//		status_adc_ram0    = Mem_Check( ADC_RAM0_ADDR, RAM_SIZE_4KB,loop,	&err_addr_adc_ram0	  );
			//}
			
			if( status_adc_ram1 == RAM_TEST_SUCCESS )
			{
				status_adc_ram1    = Mem_Check( ADC_RAM1_ADDR, RAM_SIZE_4KB,loop,	&err_addr_adc_ram1	  );
				//status_adc_ram1    = Mem_Check( ADC_RAM1_ADDR + 4, RAM_SIZE_4KB - 4,loop,	&err_addr_adc_ram1	  );
			}
			
			if( status_adc_ram2 == RAM_TEST_SUCCESS )
			{
				status_adc_ram2    = Mem_Check( ADC_RAM2_ADDR, RAM_SIZE_4KB,loop,	&err_addr_adc_ram2	  );
			}
			
			//if( status_adc_ram3 == RAM_TEST_SUCCESS )
			//{
			//	status_adc_ram3    = Mem_Check( ADC_RAM3_ADDR, RAM_SIZE_4KB,loop,	&err_addr_adc_ram3	  );	
			//}
			
			if( status_dm_ram == RAM_TEST_SUCCESS )
			{
				status_dm_ram	   = Mem_Check( DM_RAM_ADDR, RAM_SIZE_6KB,loop, 	&err_addr_dm_ram	  );
			}
			
			if( status_cdm_ram == RAM_TEST_SUCCESS )
			{
				status_cdm_ram	   = Mem_Check( CDM_RAM_ADDR, RAM_SIZE_4KB,loop,	&err_addr_cdm_ram	  );
			}
			
			//if( status_scratch_ram == RAM_TEST_SUCCESS )
			//{
			//	status_scratch_ram = Mem_Check( SCRATCH_RAM_ADDR, RAM_SIZE_4KB,loop,&err_addr_scratch_ram );
			//}
	
				
			if( status_image_ram == RAM_TEST_SUCCESS )
			{
				status_image_ram   = Mem_Check( IMG_RAM_ADDR, RAM_SIZE_16KB,loop,	&err_addr_image_ram   );
			}
			
			if( status_system_ram == RAM_TEST_SUCCESS )
			{
				status_system_ram  = Mem_Check( SYSTEM_RAM_ADDR, RAM_SIZE_TEST,loop,&err_addr_system_ram  ); 
			}
	
			if( status_system_ram == RAM_TEST_SUCCESS )
			{
				status_system_ram  = Mem_Check( SYSTEM_RAM_HIGH_ADDR, SYSTEM_RAM_HAGH_SIZE,loop,&err_addr_system_ram  ); 
			}

			if( status_puf_ram == RAM_TEST_SUCCESS )
			{
				status_puf_ram  = Mem_Check( PUF_RAM_ADDR, HWDEF_MEMMAP_PUF_RAM_SIZE,loop,&err_addr_puf_ram  ); 
				//status_puf_ram  = Mem_Check( PUF_RAM_ADDR + 4, HWDEF_MEMMAP_PUF_RAM_SIZE - 4,loop,&err_addr_puf_ram  ); 
			}
	
#ifdef LOW_SYSRAM_CHECK
			if( errcount == 0 )
			{
				uint32_t desti = 0x6000A000;
				uint32_t i;
	
				for (i=0; i< sizeof(relbuff); i++)
				{
					wr_word(desti+i*4, relbuff[i]);
				}
				asm("movi a2, 0x6000A000");
				asm("l32i a4, a2, 0");
				asm("callx4 a4");
			}
	
			if(errcount != 0 )
			{
				status_system_ram = SYSTEM_RAM_TEST_ERROR;
			}
#endif
		}
#endif	



	if( status_fw_bl_ram != RAM_TEST_SUCCESS )
	{
		status_fw_bl_ram = FW_BL_RAM_TEST_ERROR;
	}
	if( status_lna_bl_ram != RAM_TEST_SUCCESS )
	{
		status_lna_bl_ram = LNA_BL_RAM_TEST_ERROR;
	}
//	if( status_adc_ram0 != RAM_TEST_SUCCESS )
//	{
//		status_adc_ram0 = ADC_RAM0_TEST_ERROR;
//	}
	if( status_adc_ram1 != RAM_TEST_SUCCESS )
	{
		status_adc_ram1 = ADC_RAM1_TEST_ERROR;
	}
	if( status_adc_ram2 != RAM_TEST_SUCCESS )
	{
		status_adc_ram2 = ADC_RAM2_TEST_ERROR;
	}
//	if( status_adc_ram3 != RAM_TEST_SUCCESS )
//	{
//		status_adc_ram3 = ADC_RAM3_TEST_ERROR;
//	}
	if( status_dm_ram != RAM_TEST_SUCCESS )
	{
		status_dm_ram = DM_RAM_TEST_ERROR;
	}
	if( status_cdm_ram != RAM_TEST_SUCCESS )
	{
		status_cdm_ram = CDM_RAM_TEST_ERROR;
	}
//	if( status_scratch_ram != RAM_TEST_SUCCESS )
//	{
//		status_scratch_ram = SCRATCH_RAM_TEST_ERROR;
//	}
	if( status_image_ram != RAM_TEST_SUCCESS )
	{
		status_image_ram = IMAGE_RAM_TEST_ERROR;
	}
	if( status_system_ram != RAM_TEST_SUCCESS )
	{
		status_system_ram = SYSTEM_RAM_TEST_ERROR;
	}
	if( status_puf_ram != RAM_TEST_SUCCESS )
	{
		status_puf_ram = PUF_RAM_TEST_ERROR;
	}

#ifdef ERROR_SIMULATION // only for strip test error simulation
	err_addr_fw_bl_ram = 0x12345678;
	err_addr_lna_bl_ram = 0x23456789;
	//err_addr_adc_ram0 = 0x34567890;
	err_addr_adc_ram1 = 0x13572468;
	err_addr_adc_ram2 = 0x24687531;
	//err_addr_adc_ram3 = 0x87654321;
	err_addr_dm_ram = 0x98765432;
	err_addr_cdm_ram = 0x67678989;
	//err_addr_scratch_ram = 0x09876543;
	err_addr_image_ram = 0x14792589;
	err_addr_system_ram = 0x25836914;
	err_addr_puf_ram = 0x34567890;


	*rp++ = FW_BL_RAM_TEST_ERROR;
	*rp++ = err_addr_fw_bl_ram;
	*rp++ = err_addr_fw_bl_ram >> 16;

	*rp++ = LNA_BL_RAM_TEST_ERROR;
	*rp++ = err_addr_lna_bl_ram;
	*rp++ = err_addr_lna_bl_ram >> 16;
	
	//*rp++ = ADC_RAM0_TEST_ERROR;
	//*rp++ = err_addr_adc_ram0;
	//*rp++ = err_addr_adc_ram0 >> 16;
	
	*rp++ = ADC_RAM1_TEST_ERROR;
	*rp++ = err_addr_adc_ram1;
	*rp++ = err_addr_adc_ram1 >> 16;
	
	*rp++ = ADC_RAM2_TEST_ERROR;
	*rp++ = err_addr_adc_ram2;
	*rp++ = err_addr_adc_ram2 >> 16;
	
	//*rp++ = ADC_RAM3_TEST_ERROR;
	//*rp++ = err_addr_adc_ram3;
	//*rp++ = err_addr_adc_ram3 >> 16;
	
	*rp++ = DM_RAM_TEST_ERROR;
	*rp++ = err_addr_dm_ram;
	*rp++ = err_addr_dm_ram >> 16;
	
	*rp++ = CDM_RAM_TEST_ERROR;
	*rp++ = err_addr_cdm_ram;
	*rp++ = err_addr_cdm_ram >> 16;
	
	//*rp++ = SCRATCH_RAM_TEST_ERROR;
	//*rp++ = err_addr_scratch_ram;
	//*rp++ = err_addr_scratch_ram >> 16;
	
	*rp++ = IMAGE_RAM_TEST_ERROR;
	*rp++ = err_addr_image_ram;
	*rp++ = err_addr_image_ram >> 16;	

	*rp++ = SYSTEM_RAM_TEST_ERROR;
	*rp++ = err_addr_system_ram;
	*rp++ = err_addr_system_ram >> 16;	

	*rp++ = PUF_RAM_TEST_ERROR;
	*rp++ = err_addr_puf_ram;
	*rp++ = err_addr_puf_ram >> 16;	
#else
	*rp++ = status_fw_bl_ram;
	*rp++ = err_addr_fw_bl_ram;
	*rp++ = err_addr_fw_bl_ram >> 16;

	*rp++ = status_lna_bl_ram;
	*rp++ = err_addr_lna_bl_ram;
	*rp++ = err_addr_lna_bl_ram >> 16;
	
//	*rp++ = status_adc_ram0;
//	*rp++ = err_addr_adc_ram0;
//	*rp++ = err_addr_adc_ram0 >> 16;
	
	*rp++ = status_adc_ram1;
	*rp++ = err_addr_adc_ram1;
	*rp++ = err_addr_adc_ram1 >> 16;
	
	*rp++ = status_adc_ram2;
	*rp++ = err_addr_adc_ram2;
	*rp++ = err_addr_adc_ram2 >> 16;
	
//	*rp++ = status_adc_ram3;
//	*rp++ = err_addr_adc_ram3;
//	*rp++ = err_addr_adc_ram3 >> 16;
	
	*rp++ = status_dm_ram;
	*rp++ = err_addr_dm_ram;
	*rp++ = err_addr_dm_ram >> 16;
	
	*rp++ = status_cdm_ram;
	*rp++ = err_addr_cdm_ram;
	*rp++ = err_addr_cdm_ram >> 16;
	
//	*rp++ = status_scratch_ram;
//	*rp++ = err_addr_scratch_ram;
//	*rp++ = err_addr_scratch_ram >> 16;
	
	*rp++ = status_image_ram;
	*rp++ = err_addr_image_ram;
	*rp++ = err_addr_image_ram >> 16;	

	*rp++ = status_system_ram;
	*rp++ = err_addr_system_ram;
	*rp++ = err_addr_system_ram >> 16;	

	*rp++ = status_puf_ram;
	*rp++ = err_addr_puf_ram;
	*rp++ = err_addr_puf_ram >> 16;	
#endif

#ifndef DEBUG
	cmdmgr_reply( ctxp, (uint8_t *)(((uint16_t *) replyp)+1), sizeof(uint16_t) + 2 + 9 * 6 );
#else	// just for debug
	unsigned long                    i;

#if 0
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
//		*rp++ = *(( unsigned short * )( HWDEF_MEMMAP_SROM_RAM_BASE + 48 * 1024 ) + i );
		*rp++ = *(( unsigned short * )HWDEF_MEMMAP_PUF_RAM_BASE + i );
		
	}
	cmdmgr_reply( ctxp, (uint8_t *)(((uint16_t *) replyp)+1), sizeof(uint16_t) + 9 * 6 + 16 * 1024 );	
#else
	rp = ( uint16_t * ) replyp + 2;

	for( i = 0; i < 32; i++ )
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
//		*rp++ = *(( unsigned short * )( HWDEF_MEMMAP_SROM_RAM_BASE + 48 * 1024 ) + i );
		*rp++ = *(( unsigned short * )HWDEF_MEMMAP_PUF_RAM_BASE + i );
		
	}
	cmdmgr_reply( ctxp, (uint8_t *)(((uint16_t *) replyp)+1), sizeof(uint16_t) + 9 * 6 + 8 );

#endif
#endif

    return -1;
}


