/* -*- mode: c; tab-width: 4 -*- */
/*
 * INTERNAL USE ONLY
 *
 * Copyright (c) 2010-2016 Synaptics Incorporated.  All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated ("Synaptics"). The holder of this file shall treat all
 * information contained herein as confidential, shall use the
 * information only for its intended purpose, and shall not duplicate,
 * disclose, or disseminate any of this information in any manner unless
 * Synaptics has otherwise provided express, written permission.
 */

/*
 * Denali (a/k/a B1210) family hardware definitions.
 *
 * Note that this does not contain individual register/field/enum definitions,
 *  which are auto-generated into b1210reg.h
 *
 *  NOTE: This file may be included into either C or assembler code.
 *
 * Bjoren Davis, June 2, 2015.
 */

#ifndef __B1210HWDEF_H
#define __B1210HWDEF_H

/* CPU speed, used by VFK_SPINWAITUS() to calculate ticks/microsecond */
#define HWDEF_CPU_CLOCK_MHZ       144

/*
 * Denali Interrupts.
 * Reference:
 *  from b1210 SOS: doc#digital#B1210_CPU_Top_Spec.doc
 *    Revision:      2
 *    By:            hsridhar
 *    At time:       2015/06/15 11:09:32
 *  "CPU TOP Architecture Spec"
 *  "Ver: 1.0 June 14, 2015"
 */
/* Interrupt 0 currently unused */
#define HWDEF_INT_SPI_B         1
#define HWDEF_INT_SPI           (1 << HWDEF_INT_SPI_B)
#define HWDEF_INT_SPI_LEVEL     1

#define HWDEF_INT_SCM_B         2
#define HWDEF_INT_SCM           (1 << HWDEF_INT_SCM_B)
#define HWDEF_INT_SCM_LEVEL     1

#define HWDEF_INT_WOE_B         3
#define HWDEF_INT_WOE           (1 << HWDEF_INT_WOE_B)
#define HWDEF_INT_WOE_LEVEL     1

#define HWDEF_INT_BC_B          4
#define HWDEF_INT_BC            (1 << HWDEF_INT_BC_B)
#define HWDEF_INT_BC_LEVEL      1

#define HWDEF_INT_NVM_B         5
#define HWDEF_INT_NVM           (1 << HWDEF_INT_NVM_B)
#define HWDEF_INT_NVM_LEVEL     1

#define HWDEF_INT_TIMER0_B      6
#define HWDEF_INT_TIMER0        (1 << HWDEF_INT_TIMER0_B)
#define HWDEF_INT_TIMER0_LEVEL  1

#define HWDEF_INT_SOFT0_B       7
#define HWDEF_INT_SOFT0         (1 << HWDEF_INT_SOFT0_B)
#define HWDEF_INT_SOFT0_LEVEL   1

#define HWDEF_INT_TIMER1_B      10
#define HWDEF_INT_TIMER1        (1 << HWDEF_INT_TIMER1_B)
#define HWDEF_INT_TIMER1_LEVEL  3

#define HWDEF_INT_SOFT1_B       11
#define HWDEF_INT_SOFT1         (1 << HWDEF_INT_SOFT1_B)
#define HWDEF_INT_SOFT1_LEVEL   3

#define HWDEF_INT_TIMER2_B      13
#define HWDEF_INT_TIMER2        (1 << HWDEF_INT_TIMER2_B)
#define HWDEF_INT_TIMER2_LEVEL  5

#define HWDEF_INT_NMI_B         14
#define HWDEF_INT_NMI           (1 << HWDEF_INT_NMI_B)
#define HWDEF_INT_NMI_LEVEL     7

#define HWDEF_INT_COUNT         (HWDEF_INT_NMI_B+1)

/*
 * Now a set of interrupt masks by level.
 * Reference:
 *  "Processor Configuration Summary" for validity_d212 XPG Release RF-2014.1
 * C:\usr\xtensa\XtDevTools\install\builds\RF-2014.1-win32\validity_d212\config.html
 *  Built on XPG    2015-05-15 17:06:40 PDT
 *  XPG Build ID 354718
 */
#define HWDEF_INT_MASK_LEVEL1       0x001f80ff
#define HWDEF_INT_MASK_LEVEL2       0x00000100
#define HWDEF_INT_MASK_LEVEL3       0x00200e00
#define HWDEF_INT_MASK_LEVEL4       0x00001000
#define HWDEF_INT_MASK_LEVEL5       0x00002000
#define HWDEF_INT_MASK_LEVEL6       0x00000000
#define HWDEF_INT_MASK_LEVEL7       0x00004000

/*
 * Some memory map defines.
 * Reference:
 *  "Denali Design Specification B1210" "Rev 0.3 05/28/2015"
 *  denali_b1210_spec_rev_0_3.docx
 *  http://usuv-conf.synaptics.com:8090/download/attachments/47809214/denali_b1210_spec_rev_0_3.docx?api=v2
 *  checked into Confluence on May 28, 2015 by Syed Mahmud
 *
 * and
 *
 *  "Processor Configuration Summary" for validity_d212 XPG Release RF-2014.1
 * C:\usr\xtensa\XtDevTools\install\builds\RF-2014.1-win32\validity_d212\config.html
 *  Built on XPG    2015-05-15 17:06:40 PDT
 *  XPG Build ID 354718
 */

/* The spaces (as derived from "Processor Configuration Summary"): */


    /* System RAM, 64 MB */
#define HWDEF_MEMMAP_SRAM_SPACE_BASE        0x60000000
#define HWDEF_MEMMAP_SRAM_SPACE_SIZE        0x04000000

    /* System ROM, 64 MB */
#define HWDEF_MEMMAP_SROM_SPACE_BASE        0x50000000
#define HWDEF_MEMMAP_SROM_SPACE_SIZE        0x04000000

    /* Local Instruction RAM, 128 KB */
#define HWDEF_MEMMAP_IRAM_SPACE_BASE        0x40000000
#define HWDEF_MEMMAP_IRAM_SPACE_SIZE        0x00020000

    /* Local Instruction ROM is not used */

    /* Local Data RAM, 128 KB */
#define HWDEF_MEMMAP_DRAM_SPACE_BASE        0x3ffe0000
#define HWDEF_MEMMAP_DRAM_SPACE_SIZE        0x00020000

    /* Local Data ROM is not used */

    /* Local XLMI, 128 KB */
#define HWDEF_MEMMAP_XLMI_SPACE_BASE        0x3ffc0000
#define HWDEF_MEMMAP_XLMI_SPACE_SIZE        0x00020000


/* The hardware in the different spaces: */

/* The ROM */
/*   (16KB = 0x4000 bytes) */
#define HWDEF_MEMMAP_ROM_SIZE               0x00004000

    /* ROM mapped into System ROM space */
#define HWDEF_MEMMAP_SROM_ROM_BASE          0x50000000
#define HWDEF_MEMMAP_SROM_ROM_SIZE          HWDEF_MEMMAP_ROM_SIZE

/* The RAM */
/* (64KB = 0x1_0000 bytes) */
#define HWDEF_MEMMAP_RAM_SIZE               0x00010000

    /* RAM mapped into read/execute space */
#define HWDEF_MEMMAP_SROM_RAM_BASE          0x507f0000
#define HWDEF_MEMMAP_SROM_RAM_SIZE          HWDEF_MEMMAP_RAM_SIZE

    /* RAM mapped into read/write space */
#define HWDEF_MEMMAP_SRAM_RAM_BASE          0x60000000
#define HWDEF_MEMMAP_SRAM_RAM_SIZE          HWDEF_MEMMAP_RAM_SIZE

/* The flash memory */
/*  (192 KB = 0x3_0000 bytes) */
#define HWDEF_MEMMAP_FLASH_SIZE             0x00030000

/* The size of a flash page (2 KB = 0x800) */
#define HWDEF_MEMMAP_FLASH_PAGE_SIZE        0x00000800

/* The size of a programmable quantum of flash (8 bytes) */
#define HWDEF_MEMMAP_FLASH_PROGQUANT        8

    /* Flash mapped into System ROM space */
#if defined(NASSAU_BEHAVIOR_MODEL)
extern unsigned char *nvm;
#define HWDEF_MEMMAP_SROM_FLASH_BASE        nvm
#else
#define HWDEF_MEMMAP_SROM_FLASH_BASE        0x50800000
#endif

#define HWDEF_MEMMAP_SROM_FLASH_SIZE        HWDEF_MEMMAP_FLASH_SIZE

/*
 * This works out to:
 * [0x5080_0000, 0x5081_f7ff] mission firmware (size = 0x1f800 = 126 KiB)
 * [0x5081_f800, 0x5081_ffff] SDB (size = 0x800 = 2 KiB)
 * [0x5082_0000, 0x5082_fff7] iota (data) space (size = 0xfff8 = (64 KiB - 8 B))
 * [0x5082_fff8, 0x5082_ffff] MISSIONVALID area (size = 8 B)
 */

    /* Flash mapped into System ROM space, apportioned to mission firmware */
#define HWDEF_MEMMAP_SROM_FLASH_MISSION_BASE    HWDEF_MEMMAP_SROM_FLASH_BASE
#define HWDEF_MEMMAP_SROM_FLASH_MISSION_SIZE                                \
    (0x00020000 - HWDEF_MEMMAP_FLASH_PAGE_SIZE)

    /* Flash mapped into System ROM space, apportioned to the security page */
#define HWDEF_MEMMAP_SROM_FLASH_SDB_BASE                                    \
    (HWDEF_MEMMAP_SROM_FLASH_MISSION_BASE                                   \
     + HWDEF_MEMMAP_SROM_FLASH_MISSION_SIZE)
#define HWDEF_MEMMAP_SROM_FLASH_SDB_SIZE        HWDEF_MEMMAP_FLASH_PAGE_SIZE

    /* Flash mapped into System ROM space, apportioned to the iota data area */
#define HWDEF_MEMMAP_SROM_FLASH_IOTA_BASE                                   \
    (HWDEF_MEMMAP_SROM_FLASH_SDB_BASE + HWDEF_MEMMAP_SROM_FLASH_SDB_SIZE)
#define HWDEF_MEMMAP_SROM_FLASH_IOTA_SIZE                                   \
    (HWDEF_MEMMAP_SROM_FLASH_BASE + HWDEF_MEMMAP_SROM_FLASH_SIZE            \
     - HWDEF_MEMMAP_SROM_FLASH_IOTA_BASE - HWDEF_MEMMAP_FLASH_PROGQUANT)

/* The flash info block */
/*  (4 KB = 0x1000 bytes) */
#define HWDEF_MEMMAP_FLASH_INFO_BLOCK_SIZE  0x00001000


    /* Flash info block mapped into System ROM space */
#define HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE     0x50840000
#define HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_SIZE                             \
    HWDEF_MEMMAP_FLASH_INFO_BLOCK_SIZE


/*
 * Address mapping macros.  These are addends added to addresses
 *  to convert them from pointing to something in one space
 *  to pointing to that same thing in another space.
 */

/* RAM: from a SROM (read/executed) address to SRAM (read/write) address */
/*      E.g. 507f_xxxx -> 6000_xxxx for Denali */
#define HWDEF_MEMMAP_SROM_TO_SRAM_RAM_ADDEND 0x0f810000

/*
 * The vectors.  Note that the sizes are informational only; the CPU
 *  does not, as far as I know, enforce these sizes in any way.
 */

/*
 * Static vectors
 */

/* reset vector vectors */
#define HWDEF_MEMMAP_RESETVEC       0x50000000
#define HWDEF_MEMMAP_RESETVEC_SIZE  0x00000300

/*
 * Dynamic vectors
 */

/* window overflow/underflow exception vectors */
#define HWDEF_MEMMAP_WINVEC_OFF     0x00000000
#define HWDEF_MEMMAP_WINVEC_SIZE    0x00000178

/* level 2 interrupt exception vector */
#define HWDEF_MEMMAP_L2INTVEC_OFF   0x00000180
#define HWDEF_MEMMAP_L2INTVEC_SIZE  0x00000038

/* level 3 interrupt exception vector */
#define HWDEF_MEMMAP_L3INTVEC_OFF   0x000001c0
#define HWDEF_MEMMAP_L3INTVEC_SIZE  0x00000038

/* level 4 interrupt exception vector */
#define HWDEF_MEMMAP_L4INTVEC_OFF   0x00000200
#define HWDEF_MEMMAP_L4INTVEC_SIZE  0x00000038

/* level 5 interrupt exception vector */
#define HWDEF_MEMMAP_L5INTVEC_OFF   0x00000240
#define HWDEF_MEMMAP_L5INTVEC_SIZE  0x00000038

/* level 6 interrupt exception vector */
#define HWDEF_MEMMAP_L6INTVEC_OFF   0x00000280
#define HWDEF_MEMMAP_L6INTVEC_SIZE  0x00000038

/* level 7 interrupt exception (NMI) vector */
#define HWDEF_MEMMAP_L7INTVEC_OFF   0x000002c0
#define HWDEF_MEMMAP_L7INTVEC_SIZE  0x00000038

/* kernel-mode exception vector (which we don't use) */
#define HWDEF_MEMMAP_KEXCVEC_OFF    0x00000300
#define HWDEF_MEMMAP_KEXCVEC_SIZE   0x00000038

/* user-mode exception vector */
#define HWDEF_MEMMAP_UEXCVEC_OFF    0x00000340
#define HWDEF_MEMMAP_UEXCVEC_SIZE   0x00000038

/* double exception vector */
#define HWDEF_MEMMAP_DBLEXCVEC_OFF  0x000003c0
#define HWDEF_MEMMAP_DBLEXCVEC_SIZE 0x00000040

/*
 * There is an alternate reset vector which we use for ATE code.
 */

#define HWDEF_MEMMAP_ALTRESETVEC    0x50000400
#define HWDEF_MEMMAP_ALTRESETVEC_SIZE  0x00000300

/*
 * Here we make the allocation for ATE code: they get exact 2 KB
 *  of space in the ROM, starting at 0x50000400.
 */
#define HWDEF_MEMMAP_SROM_ATE_BASE  0x50000400
#define HWDEF_MEMMAP_SROM_ATE_SIZE  0x00000800


/* Register base */
#define HWDEF_MEMMAP_REGISTER_BASE  0x80000000

/* Register space definitions that are not in the XML: */

/* Firmware baseline RAM */
#define HWDEF_MEMMAP_FW_BL_RAM_BASE 0x80080000
#define HWDEF_MEMMAP_FW_BL_RAM_SIZE 0x00004000

/* Image RAM */
#define HWDEF_MEMMAP_IMGRAM_BASE    0x80084000
#define HWDEF_MEMMAP_IMGRAM_SIZE    0x00004000

/* LNA baseline RAM */
#define HWDEF_MEMMAP_LNA_BL_RAM_BASE 0x80088000
#define HWDEF_MEMMAP_LNA_BL_RAM_SIZE 0x00004000

/* Drive Matrix RAM */
#define HWDEF_MEMMAP_DMRAM_BASE     0x8008c000
#define HWDEF_MEMMAP_DMRAM_SIZE     0x00001800

/* Code division multiplexing RAM */
#define HWDEF_MEMMAP_CDMRAM_BASE    0x8008e000
#define HWDEF_MEMMAP_CDMRAM_SIZE    0x00001000

/* ADC RAM 1 */
#define HWDEF_MEMMAP_ADCRAM1_BASE   0x8008f000
#define HWDEF_MEMMAP_ADCRAM1_SIZE   0x00001000

/* ADC RAM 2 */
#define HWDEF_MEMMAP_ADCRAM2_BASE   0x80090000
#define HWDEF_MEMMAP_ADCRAM2_SIZE   0x00001000

/* SCRATCH RAM (Denali B0 and later only) */
#define HWDEF_MEMMAP_SCRATCH_RAM_BASE   0x80094000
#define HWDEF_MEMMAP_SCRATCH_RAM_SIZE   0x00001000


/* MISSIONVALID word */
#define HWDEF_MEMMAP_MISSIONVALID   (HWDEF_MEMMAP_SROM_FLASH_BASE           \
                                     +  HWDEF_MEMMAP_SROM_FLASH_SIZE - 4)
#define HWDEF_MEMMAP_MISSIONVALID_PATTERN   0x495dc931

/*
 * Flash information block layout.
 */
/* Serial number (6 bytes = 48 bits; [0x5084_0010, 0x5084_0015] */
#define HWDEF_FIB_SERIALNO          (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0010)
#define HWDEF_FIB_SERIALNO_SIZE     6

/* High speed oscillator trim (4 bytes = 32 bits; [0x5084_0020, 0x5084_0023] */
/* High bits are 2'b10 or 2'b01 to indicate validity */
/* Other bits fed directly into WOE_HS_OSC_TRIM */
#define HWDEF_FIB_FASTCLK           (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0020)
#define HWDEF_FIB_FASTCLK_SIZE      4

/* Low speed oscillator trim (4 bytes = 32 bits; [0x5084_0024, 0x5084_0027] */
/* High bits are 2'b10 or 2'b01 to indicate validity */
/* Other bits fed directly into SCM_SOSC_BIAS_TRIM */
#define HWDEF_FIB_SLOWCLK           (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0024)
#define HWDEF_FIB_SLOWCLK_SIZE      4

/* Voltage trim (4 bytes = 32 bits; [0x5084_0028, 0x5084_002b] */
/* High bits are 2'b10 or 2'b01 to indicate validity */
/* Other bits fed directly into WOE_DVREG */
#define HWDEF_FIB_VTRIM             (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0028)
#define HWDEF_FIB_VTRIM_SIZE        4

/* "Soft" security word (4 bytes = 32bits; [0x5084_002c, 0x5084_002f] */
#define HWDEF_FIB_SOFTSECW          (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x002c)
/* Soft security word is used to determine engineering vs. production sensor */


/* Bandgap bias trim.  (4 bytes = 32 bits; [0x5084_0070, 0x5084_0073]) */
/* High bits are 2'b10 or 2'b01 to indicate validity */
/* Other bits fed directly into SCM_BG_BIAS_TRIM */
#define HWDEF_FIB_BGTRIM            (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0070)
#define HWDEF_FIB_BGTRIM_SIZE       4

/* Production test record.  (112 bytes; [0x5084_0080, 0x5084_00ef] */
#define HWDEF_FIB_TESTREC           (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0080)
#define HWDEF_FIB_TESTREC_SIZE      112

/* WOF DAC thresholds. */
/* High bits are 2'b10 or 2'b01 to indicate validity */
/* Three separate 8-bit values in 23:16, 15:8, 7:0. */
/* To be programmed at ATE.  Requested by Gordon Shen on January 19, 2017. */
#define HWDEF_FIB_WOFDACOFF         (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0f70)
#define HWDEF_FIB_WOFDACOFF_SIZE    4


/* Mission firmware Flavor Selector: 4 bytes = 32 bits; [0x5084_0f78, 0x5084_07f]
 * Since project A and project B may differ by features supported and 
 * integration models, a feature for project A may become a security hole 
 * for project B.  The flavor selector value gets compared with the flavor 
 * selector value for a new image before proceeding with programming.
 */
#define HWDEF_FIB_FWFLAVOR           (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0f78)
#define HWDEF_FIB_FWFLAVOR_SIZE      8
                                     
/* Security level (16 bytes = 128 bits; [0x5084_0f80, 0x5084_0f9f] */
#define HWDEF_FIB_SECLEVEL          (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0f80)
#define HWDEF_FIB_SECLEVEL_SIZE     16

/* General product options (4 bytes = 32 bits; [0x5084_0fa0, 0x5084_0fa8]
 * Reserved 8bytes. */
#define HWDEF_FIB_PRODOPT           (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0fa0)
#define HWDEF_FIB_PRODOPT_SIZE      8

/*
 * "Master" key material.  This replaces the PUF.
 * (64 bytes; [0x5084_0fa8, 0x5084_00fe7])
 */
#define HWDEF_FIB_SEC_MKM           (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0fa8)
#define HWDEF_FIB_SEC_MKM_SIZE      64

/* Security word (8 bytes = 64 bits; [0x5084_0ff8, 0x5084_0fff] */
/* (actually, only 4 bytes are used */
#define HWDEF_FIB_SECW              (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0ff8)
#define HWDEF_FIB_SECW_SIZE         8

/* Security word possible magic values: */
#define HWDEF_FIB_SECW_MAGIC_OPEN   0x63733a2c
#define HWDEF_FIB_SECW_MAGIC_SECURE 0x20410a08

/*
 * Security word lifecycle:
 *                 
 *  random     -> erased     -> OPEN       -> SECURE
 *             ->  0 bits on -> 16 bits on -> 6 bits on
 *  0xXXXXXXXX -> 0xffffffff -> 0x63733a2c -> 0x20410a08
 * clear bits:           0x9c8cc5d3    0xbccdcfdb
 * bits left on:         0x63733a2c    0x20410a08
 */

/* Debug password (16 bytes = 128 bits; [0x5084_0fe8, 0x5084_0ff7] */
#define HWDEF_FIB_DEBUGPW           (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0fe8)
#define HWDEF_FIB_DEBUGPW_SIZE      16

/*
 * Cache configuration
 */
#define HWDEF_ICACHE_SIZE              8192
#define HWDEF_DCACHE_SIZE              4096

#define HWDEF_DCACHE_LINESIZE          32
#define HWDEF_ICACHE_LINESIZE          32

/* Align the pointer to dcache line boundary */
#define HWDEF_DCACHE_ALIGN(ptr) ((((uint32_t)(ptr))                     \
                                      + (HWDEF_DCACHE_LINESIZE - 1))    \
                                     & ~(HWDEF_DCACHE_LINESIZE - 1))

/*
 * AFE configuration
 */
#define HWDEF_AFE_NUM_RX  56
#define HWDEF_AFE_NUM_TX  144
#define HWDEF_AFE_TOTAL_PIXELS  (HWDEF_AFE_NUM_RX) * (HWDEF_AFE_NUM_TX)
#define HWDEF_AFE_NUM_SUBFRAMES 4
#define HWDEF_AFE_SUBFRAME_PIXELS                                      \
    (HWDEF_AFE_TOTAL_PIXELS / HWDEF_AFE_NUM_SUBFRAMES)
#define HWDEF_AFE_SUBFRAME_NUM_RX (HWDEF_AFE_NUM_RX / HWDEF_AFE_NUM_SUBFRAMES)
#define HWDEF_AFE_SUBFRAME1_OFFSET HWDEF_AFE_SUBFRAME_PIXELS
#define HWDEF_AFE_SUBFRAME2_OFFSET HWDEF_AFE_SUBFRAME_PIXELS*2
#define HWDEF_AFE_SUBFRAME3_OFFSET HWDEF_AFE_SUBFRAME_PIXELS*3


/* 12 bits, plus a 13th bit for sign.  The upper 4 bits are all part of the SIGN
 * macro in order to leave the B1210 code unchanged from where it was prior to
 * the integration of support for other chips. */
#define HWDEF_AFE_SIGN              0xf000
#define HWDEF_AFE_SIGN_N            12
#define HWDEF_AFE_SIGN_B            4
#define HWDEF_AFE_MAGNITUDE         0x0fff
#define HWDEF_AFE_MAGNITUDE_N       12
#define HWDEF_AFE_MAGNITUDE_B       0


#endif      /* __B1210HWDEF_H */
