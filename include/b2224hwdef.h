/* -*- mode: c; tab-width: 4 -*- */
/*
 * INTERNAL USE ONLY
 *
 * Copyright (c) 2010-2017 Synaptics Incorporated.  All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated ("Synaptics"). The holder of this file shall treat all
 * information contained herein as confidential, shall use the
 * information only for its intended purpose, and shall not duplicate,
 * disclose, or disseminate any of this information in any manner unless
 * Synaptics has otherwise provided express, written permission.
 */

/*
 * Byron (a/k/a B2224 a/k/a SC24 Steller Controller)
 *  family hardware definitions.
 *
 * Note that this does not contain individual register/field/enum definitions,
 *  which are auto-generated into b2224reg.h
 *
 *  NOTE: This file may be included into either C or assembler code.
 *
 * Blatantly ripped off from b1212hwdef.h
 * Bjoren Davis, April 5, 2017
 */

#ifndef __B2224HWDEF_H
#define __B2224HWDEF_H


/* CPU speed, used by VFK_SPINWAITUS() to calculate ticks/microsecond */
#define HWDEF_CPU_CLOCK_MHZ       192

/*
 * Denali Interrupts.
 * Reference:
 *  from b2224 SOS: ./doc/digital/specification/b2224_Registers_and_Commands.xls
 *    Revision:      8
 *    By:            bmott
 *    At time:       2017/04/01 12:33:05
 */

#define HWDEF_INT_USB_B         0
#define HWDEF_INT_USB           (1 << HWDEF_INT_USB_B)
#define HWDEF_INT_USB_LEVEL     1

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

#define HWDEF_INT_SPI_MAS_B     5
#define HWDEF_INT_SPI_MAS       (1 << HWDEF_INT_SPI_MAS_B)
#define HWDEF_INT_SPI_MAS_LEVEL 1

#define HWDEF_INT_TIMER0_B      6
#define HWDEF_INT_TIMER0        (1 << HWDEF_INT_TIMER0_B)
#define HWDEF_INT_TIMER0_LEVEL  1

#define HWDEF_INT_SOFT0_B       7
#define HWDEF_INT_SOFT0         (1 << HWDEF_INT_SOFT0_B)
#define HWDEF_INT_SOFT0_LEVEL   1

#define HWDEF_INT_GPIO_B        8
#define HWDEF_INT_GPIO          (1 << HWDEF_INT_GPIO_B)
#define HWDEF_INT_GPIO_LEVEL    2

#define HWDEF_INT_WOE_SLEEP_B   9
#define HWDEF_INT_WOE_SLEEP     (1 << HWDEF_INT_WOE_SLEEP_B)
#define HWDEF_INT_WOE_SLEEP_LEVEL 3

#define HWDEF_INT_TIMER1_B      10
#define HWDEF_INT_TIMER1        (1 << HWDEF_INT_TIMER1_B)
#define HWDEF_INT_TIMER1_LEVEL  3

#define HWDEF_INT_SOFT1_B       11
#define HWDEF_INT_SOFT1         (1 << HWDEF_INT_SOFT1_B)
#define HWDEF_INT_SOFT1_LEVEL   3

#define HWDEF_INT_MASTER_B      12
#define HWDEF_INT_MASTER        (1 << HWDEF_INT_MASTER_B)
#define HWDEF_INT_MASTER_LEVEL  4

#define HWDEF_INT_TIMER2_B      13
#define HWDEF_INT_TIMER2        (1 << HWDEF_INT_TIMER2_B)
#define HWDEF_INT_TIMER2_LEVEL  5

#define HWDEF_INT_NMI_B         14
#define HWDEF_INT_NMI           (1 << HWDEF_INT_NMI_B)
#define HWDEF_INT_NMI_LEVEL     7

#define HWDEF_INT_HSYNC_B       15
#define HWDEF_INT_HSYNC         (1 << HWDEF_INT_HSYNC_B)
#define HWDEF_INT_HSYNC_LEVEL   1

#define HWDEF_INT_VSYNC_B       16
#define HWDEF_INT_VSYNC         (1 << HWDEF_INT_VSYNC_B)
#define HWDEF_INT_VSYNC_LEVEL   1

#define HWDEF_INT_TT4NAV_B      17
#define HWDEF_INT_TT4NAV        (1 << HWDEF_INT_TT4NAV_B)
#define HWDEF_INT_TT4NAV_LEVEL  1

#define HWDEF_INT_COUNT         (HWDEF_INT_TT4NAV_B+1)

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
 *  from b2224 SOS: ./doc/digital/specification/b2224_Registers_and_Commands.xls
 *    Revision:      8
 *    By:            bmott
 *    At time:       2017/04/01 12:33:05
 *
 * and
 *  (by default because Byron is derived from Hayes)
 *   Hayes Design Specification B1212
 *   Revision 1.1 5/6/2016
 *   b1212 SOS: ./doc/digital/Hayes_B1212_Spec.docx
 *    Revision:      28
 *    By:            jchilds
 *    At time:       2017/01/30 08:58:23
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
/*   (24KB = 0x6000 bytes) */
#define HWDEF_MEMMAP_ROM_SIZE               0x00006000

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
#define HWDEF_MEMMAP_SROM_FLASH_BASE        0x50800000
#define HWDEF_MEMMAP_SROM_FLASH_SIZE        HWDEF_MEMMAP_FLASH_SIZE

    /* Flash mapped into System ROM space, apportioned to mission firmware */
#define HWDEF_MEMMAP_SROM_FLASH_MISSION_BASE    HWDEF_MEMMAP_SROM_FLASH_BASE
#define HWDEF_MEMMAP_SROM_FLASH_MISSION_SIZE    0x00020000

    /* Flash mapped into System ROM space, apportioned to the iota data area */
#define HWDEF_MEMMAP_SROM_FLASH_IOTA_BASE                                   \
    (HWDEF_MEMMAP_SROM_FLASH_MISSION_BASE                                   \
     + HWDEF_MEMMAP_SROM_FLASH_MISSION_SIZE)
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

/*
 * Reference:
 *  from b2224 SOS: ./doc/digital/specification/b2224_Registers_and_Commands.xls
 *    Revision:      8
 *    By:            bmott
 *    At time:       2017/04/01 12:33:05
 */

/* Firmware baseline RAM */
#define HWDEF_MEMMAP_FW_BL_RAM_BASE 0x80080000
#define HWDEF_MEMMAP_FW_BL_RAM_SIZE 0x00004000

/* Portion of FW baseline RAM used for scratch purposes.  The size of this area
 * is based on where we've chosen to start it... */
#define HWDEF_MEMMAP_SCRATCH_FW_BL_RAM_BASE  0x80083800
#define HWDEF_MEMMAP_SCRATCH_FW_BL_RAM_SIZE                                 \
    (HWDEF_MEMMAP_FW_BL_RAM_SIZE -                                          \
     (HWDEF_MEMMAP_SCRATCH_FW_BL_RAM_BASE - HWDEF_MEMMAP_FW_BL_RAM_BASE))

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

/* PUF RAM */
#define HWDEF_MEMMAP_PUF_RAM_BASE   0x80091000
#define HWDEF_MEMMAP_PUF_RAM_SIZE   0x00000080


/* MISSIONVALID word */
#define HWDEF_MEMMAP_MISSIONVALID   (HWDEF_MEMMAP_SROM_FLASH_BASE           \
                                     +  HWDEF_MEMMAP_SROM_FLASH_SIZE - 4)
#define HWDEF_MEMMAP_MISSIONVALID_PATTERN   0x495dc931

/*
 * Flash information block layout.
 */

/* ---- page[0]low (most security restrictions) ---- */
/* General product options (4 bytes = 32 bits; [0x5084_0000, 0x5084_0003] */
#define HWDEF_FIB_PRODOPT           (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0000)
#define HWDEF_FIB_PRODOPT_SIZE      4

/* SC24 is SPI only, so we don't support the HOSTCOMM field */


/* Placeholder (4 bytes = 32 bits; [0x5084_0008, 0x5084_000b] */
/* Holds USB VID/PID for Hayes */

/* Serial number (6 bytes = 48 bits; [0x5084_0010, 0x5084_0015] */
#define HWDEF_FIB_SERIALNO          (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0010)
#define HWDEF_FIB_SERIALNO_SIZE     6

/*
 * Supply and current calibration adjustments (4 bytes)
 * High bits are 2'b10 or 2'b01 to indicate validity
 * Bits[3:0] are fed into SCM_BANDGAP_ADJ
 * Bits[13:8] are assigned to SCM_REF_IADJ
 * Bits[20:16] are assigned to SCM_VDDCORE_ADJ
 */
/* Vdd trim (4 bytes = 32 bits; [0x5084_0018, 0x5084_001b] */
#define HWDEF_FIB_BRVTRIM           (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0018)
#define HWDEF_FIB_BRVTRIM_SIZE      4
#define HWDEF_FIB_BRVTRIM_BANDGAP_B     0
#define HWDEF_FIB_BRVTRIM_BANDGAP_N     4
#define HWDEF_FIB_BRVTRIM_REF_IADJ_B    8
#define HWDEF_FIB_BRVTRIM_REF_IADJ_N    6
#define HWDEF_FIB_BRVTRIM_VDDCORE_B     16
#define HWDEF_FIB_BRVTRIM_VDDCORE_N     5

/* High speed oscillator trim (4 bytes = 32 bits; [0x5084_0020, 0x5084_0023] */
/* High bits are 2'b10 or 2'b01 to indicate validity */
/* Other bits fed directly into the SIOSC block (SIOSC_TRIM) */
#define HWDEF_FIB_FASTCLK           (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0020)
#define HWDEF_FIB_FASTCLK_SIZE      4

/* Fingerprint module number (4 bytes = 32 bits; [0x5084_0028, 0x5084_002b] */
/*
 * High bits are 2'b10 or 2'b01 to indicate validity
 *  bits 29:10 are the "mantissa", 9:0 the "dash"
 */

#define HWDEF_FIB_FMNO              (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0028)
#define HWDEF_FIB_FMNO_SIZE         4
#define HWDEF_FIB_FMNO_VALID            0xc0000000
#define HWDEF_FIB_FMNO_VALID_B              30
#define HWDEF_FIB_FMNO_VALID_N              2
#define HWDEF_FIB_FMNO_MANT             0x3ffffc00
#define HWDEF_FIB_FMNO_MANT_B               10
#define HWDEF_FIB_FMNO_MANT_N               20
#define HWDEF_FIB_FMNO_DASH             0x000003ff
#define HWDEF_FIB_FMNO_DASH_B               0
#define HWDEF_FIB_FMNO_DASH_N               10

/* --GAP-- 864 bytes [0x5084_0030, 0x5084_038f] */


/* Mission firmware Flavor Selector: 0x5084_0390 Since project A and
 * project B may differ by features supported and integration models, a
 * feature for project A may become a security hole for project B.  The
 * flavor selector value gets compared with the flavor selector value
 * for a new image before proceeding with programming.
 */
#define HWDEF_FIB_FWFLAVOR          (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0390)

/*
 * Main flash layout information.
 *
 * This is optional information, mainly for use with Diamond Peak (which
 *  needs much larger iota storage and much smaller mission firmware
 *  storage).
 * These values are marked as valid with their high 2 bits.  If they
 *  are invalid then the HWDEF_MEMMAP_SROM_FLASH_MISSION_SIZE,
 *  HWDEF_MEMMAP_SROM_FLASH_IOTA_BASE, and HWDEF_MEMMAP_SROM_FLASH_IOTA_SIZE
 *  predominate.
 */

/* Mission firmware size: 0x5084_0398 */
#define HWDEF_FIB_MISSIONSIZE       (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0398)
/* Iota start offset: 0x5084_03a0 */
#define HWDEF_FIB_IOTAOFF           (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x03a0)
/* Iota size: 0x5084_03a4 */
#define HWDEF_FIB_IOTASIZE          (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x03a4)
/*
 * High speed oscillator trim (4 bytes = 32 bits; [0x5084_03a8, 0x5084_03ab]
 * specific to 170Mhz applications.
 *
 * High bits are 2'b10 or 2'b01 to indicate validity
 * Other bits fed directly into the SIOSC block (SIOSC_TRIM) by the mission
 * firmware for systems requiring a slower HSO.
 *
 * This is targeted towards the Crystal Peak application.  For Crystal Peak, it
 * is necessary to write this value into the SIOSC_TRIM register prior to enabling
 * the DP_MCLK output to the Diamond Peak ASIC.
 */
#define HWDEF_FIB_FASTCLK_170MHZ      (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                       + 0x03a8)
#define HWDEF_FIB_FASTCLK_170MHZ_SIZE 4

/* Placeholder: 0x5084_03b0 */
/* Placeholder: 0x5084_03b8 */


/* Low speed oscillator trim (4 bytes = 32 bits; [0x5084_03c0, 0x5084_03c3] */
/* High bits are 2'b10 or 2'b01 to indicate validity */
/* Other bits fed directly into SCM_SOSC_BIAS_TRIM */
#define HWDEF_FIB_SLOWCLK           (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x03c0)
#define HWDEF_FIB_SLOWCLK_SIZE      4

/* PUF BCH parity (52 bytes = 416 bits; [0x5084_03c8, 0x5084_03fb] */
#define HWDEF_FIB_SEC_BCH_PARITY    (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x03c8)
#define HWDEF_FIB_SEC_BCH_PARITY_SIZE   52

/* ---- page[0]high (medium security restrictions) ---- */

/* Test record (144 bytes = 1152 bits; [0x5084_0400, 0x5084_048f] */
#define HWDEF_FIB_TESTREC           (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0400)
#define HWDEF_FIB_TESTREC_SIZE      144

/* Customer key (816 bytes = 6528 bits; [0x5084_0490, 0x5084_07bf] */
#define HWDEF_FIB_CUSTKEY           (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0490)
#define HWDEF_FIB_CUSTKEY_SIZE      816

/* --GAP-- 48 bytes [0x5084_07c0, 0x5084_07e7] */

/* Security level (16 bytes = 128 bits; [0x5084_07e0, 0x5084_07f7] */
#define HWDEF_FIB_SECLEVEL          (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x07e0)
#define HWDEF_FIB_SECLEVEL_SIZE     16

/* ---- page[1] (fewest security restrictions) ---- */
/* Security data block (2048 bytes = 16384 bits; [0x5084_0800, 0x5084_0fff] */
#define HWDEF_FIB_SDB               (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0800)
#define HWDEF_FIB_SDB_SIZE          2048

#define HWDEF_MEMMAP_SROM_FLASH_SDB_BASE HWDEF_FIB_SDB

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




#endif      /* __B2224HWDEF_H */
