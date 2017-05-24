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
 * Shasta (a/k/a B1216) family hardware definitions.
 *
 * Note that this does not contain individual register/field/enum definitions,
 *  which are auto-generated into b1216reg.h
 *
 *  NOTE: This file may be included into either C or assembler code.
 *
 * Bjoren Davis, September 30, 2016.
 */

#ifndef __B1216HWDEF_H
#define __B1216HWDEF_H


/* CPU speed, used by VFK_SPINWAITUS() to calculate ticks/microsecond */
#define HWDEF_CPU_CLOCK_MHZ       144

/*
 * Shasta Interrupts.
 * Reference:
 *  from b1216 SOS: ./doc/digital/B1216_CPU_Top_Spec.doc
 *  Revision:      1.4
 *  By:            bbignell
 *  At time:       2016/11/29 17:24:49 PDT
 */

/* Interrupt 0 unused */

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

#define HWDEF_INT_CDM_B         5
#define HWDEF_INT_CDM           (1 << HWDEF_INT_CDM_B)
#define HWDEF_INT_CDM_LEVEL     1

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
#define HWDEF_INT_WOE_SLEEP_LEVEL   3

#define HWDEF_INT_TIMER1_B      10
#define HWDEF_INT_TIMER1        (1 << HWDEF_INT_TIMER1_B)
#define HWDEF_INT_TIMER1_LEVEL  3

#define HWDEF_INT_SOFT1_B       11
#define HWDEF_INT_SOFT1         (1 << HWDEF_INT_SOFT1_B)
#define HWDEF_INT_SOFT1_LEVEL   3

#define HWDEF_INT_SPI_MAS_B     12
#define HWDEF_INT_SPI_MAS       (1 << HWDEF_INT_SPI_MAS_B)
#define HWDEF_INT_SPI_MAS_LEVEL 4

#define HWDEF_INT_TIMER2_B      13
#define HWDEF_INT_TIMER2        (1 << HWDEF_INT_TIMER2_B)
#define HWDEF_INT_TIMER2_LEVEL  5

#define HWDEF_INT_NMI_B         14
#define HWDEF_INT_NMI           (1 << HWDEF_INT_NMI_B)
#define HWDEF_INT_NMI_LEVEL     7

#define HWDEF_INT_NVM_B         15
#define HWDEF_INT_NVM           (1 << HWDEF_INT_NVM_B)
#define HWDEF_INT_NVM_LEVEL     1

/* 16, 17, 18, 19, 20, 21 are unused */
#define HWDEF_INT_COUNT         (HWDEF_INT_NVM_B+1)

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
 *  "Shasta (B1216)Design Specification", Rev 0.4 09/19/2016
 *  shasta_b1216_spec_rev_0_4.pdf
 *  from SOS in doc/digital/shasta_b1216_spec_rev_0_4.pdf
 *  Revision:      1
 *  By:            smahmud
 *  At time:       2016/09/19 15:07:17
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
/*  (96 KB = 0x1_8000 bytes) */
#define HWDEF_MEMMAP_FLASH_SIZE             0x00018000

/* The size of a flash page (2 KB = 0x800) */
#define HWDEF_MEMMAP_FLASH_PAGE_SIZE        0x00000800

/* The size of a programmable quantum of flash (8 bytes) */
#define HWDEF_MEMMAP_FLASH_PROGQUANT        8

    /* Flash mapped into System ROM space */
#define HWDEF_MEMMAP_SROM_FLASH_BASE        0x50800000
#define HWDEF_MEMMAP_SROM_FLASH_SIZE        HWDEF_MEMMAP_FLASH_SIZE

    /* Flash mapped into System ROM space, apportioned to mission firmware */
#define HWDEF_MEMMAP_SROM_FLASH_MISSION_BASE    HWDEF_MEMMAP_SROM_FLASH_BASE
#define HWDEF_MEMMAP_SROM_FLASH_MISSION_SIZE    0x00010000

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
#define HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE     0x50820000
#define HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_SIZE                             \
    HWDEF_MEMMAP_FLASH_INFO_BLOCK_SIZE


/*
 * Address mapping macros.  These are addends added to addresses
 *  to convert them from pointing to something in one space
 *  to pointing to that same thing in another space.
 */

/* RAM: from a SROM (read/executed) address to SRAM (read/write) address */
/*      E.g. 507f_xxxx -> 6000_xxxx for Shasta */
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
 *  "Shasta (B1216)Design Specification", Rev 0.4 09/19/2016
 *  shasta_b1216_spec_rev_0_4.pdf
 *  from SOS in doc/digital/shasta_b1216_spec_rev_0_4.pdf
 *  Revision:      1
 *  By:            smahmud
 *  At time:       2016/09/19 15:07:17
 */

/* Firmware baseline RAM */
#define HWDEF_MEMMAP_FW_BL_RAM_BASE     0x80080000
#define HWDEF_MEMMAP_FW_BL_RAM_SIZE     0x00004000

/* LNA baseline RAM */
#define HWDEF_MEMMAP_LNA_BL_RAM_BASE    0x80088000
#define HWDEF_MEMMAP_LNA_BL_RAM_SIZE    0x00004000

/* ADC RAM 0 */
#define HWDEF_MEMMAP_ADCRAM0_BASE       0x80090000
#define HWDEF_MEMMAP_ADCRAM0_SIZE       0x00001000

/* ADC RAM 1 */
#define HWDEF_MEMMAP_ADCRAM1_BASE       0x80092000
#define HWDEF_MEMMAP_ADCRAM1_SIZE       0x00001000

/* ADC RAM 2 */
#define HWDEF_MEMMAP_ADCRAM2_BASE       0x80094000
#define HWDEF_MEMMAP_ADCRAM2_SIZE       0x00001000

/* ADC RAM 3 */
#define HWDEF_MEMMAP_ADCRAM3_BASE       0x80096000
#define HWDEF_MEMMAP_ADCRAM3_SIZE       0x00001000

/* Drive Matrix RAM */
#define HWDEF_MEMMAP_DMRAM_BASE         0x80098000
#define HWDEF_MEMMAP_DMRAM_SIZE         0x00001000

/* Code division multiplexing RAM */
#define HWDEF_MEMMAP_CDMRAM_BASE        0x8009a000
#define HWDEF_MEMMAP_CDMRAM_SIZE        0x00000800

/* SCRATCH RAM */
#define HWDEF_MEMMAP_SCRATCH_RAM_BASE   0x8009b000
#define HWDEF_MEMMAP_SCRATCH_RAM_SIZE   0x00001000

/* Image RAM */
#define HWDEF_MEMMAP_IMGRAM_BASE        0x800a0000
#define HWDEF_MEMMAP_IMGRAM_SIZE        0x00004000

/* MISSIONVALID word */
#define HWDEF_MEMMAP_MISSIONVALID   (HWDEF_MEMMAP_SROM_FLASH_BASE           \
                                     +  HWDEF_MEMMAP_SROM_FLASH_SIZE - 4)
#define HWDEF_MEMMAP_MISSIONVALID_PATTERN   0x495dc931

/*
 * Flash information block layout.
 */

/* -------------------------------------------------------------------------- */
/* page[0] (write/page-erasable in OPEN/SECURE/FA states) */
/* Security data block (2048 bytes; [0x5082_0000, 0x5082_07ff] */
#define HWDEF_FIB_SDB               (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0000)
#define HWDEF_FIB_SDB_SIZE          2048

#define HWDEF_MEMMAP_SROM_FLASH_SDB_BASE HWDEF_FIB_SDB   

/* -------------------------------------------------------------------------- */
/* page[1]low (never page-erasable, writable only in OPEN/SECURE/FA states)  */

/* Serial number (6 bytes = 48 bits; 8 bytes alloc [0x5082_0800, 0x5082_0805] */
#define HWDEF_FIB_SERIALNO          (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0800)
#define HWDEF_FIB_SERIALNO_SIZE     6

/* High speed oscillator trim (4 bytes = 32 bits; [0x5082_0808, 0x5082_080b] */
/* High bits are 2'b10 or 2'b01 to indicate validity */
/* Other bits fed directly into WOE_HS_OSC_TRIM */
#define HWDEF_FIB_FASTCLK           (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0808)
#define HWDEF_FIB_FASTCLK_SIZE      4

/* Bandgap bias trim.  (4 bytes = 32 bits; [0x5082_0810, 0x5082_0817]) */
/* High bits are 2'b10 or 2'b01 to indicate validity */
/* Other bits fed directly into SCM_BG_BIAS_TRIM */
#define HWDEF_FIB_BGTRIM            (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0810)
#define HWDEF_FIB_BGTRIM_SIZE       4

/* Voltage reference trims.  (4 bytes = 32 bits; [0x5082_0818, 0x5082_081f]) */
/* High bits are 2'b10 or 2'b01 to indicate validity */
/* Other bits fed directly into SCM_LDO_REF_TRIM */
#define HWDEF_FIB_REFTRIM           (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x818)
#define HWDEF_FIB_REFTRIM_SIZE      4

/* Voltage trim (4 bytes = 32 bits; [0x5082_0820, 0x5082_0827] */
/* High bits are 2'b10 or 2'b01 to indicate validity */
/* Other bits fed directly into WOE_DVREG */
#define HWDEF_FIB_VTRIM             (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0820)
#define HWDEF_FIB_VTRIM_SIZE        4

/* Fingerprint module number (4 bytes = 32 bits; [0x5082_0828, 0x5081_082b] */
/*
 * High bits are 2'b10 or 2'b01 to indicate validity
 *  bits 29:10 are the "mantissa", 9:0 the "dash"
 */

#define HWDEF_FIB_FMNO              (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0828)
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


/* "Soft" security word (4 bytes = 32bits; [0x5082_0830, 0x5082_0833] */
#define HWDEF_FIB_SOFTSECW          (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0830)
/* Soft security word is used to determine engineering vs. production sensor */

/* Customer key (912 bytes = 6528 bits; [0x5082_0838, 0x5082_0b67] */
#define HWDEF_FIB_CUSTKEY           (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0838)
#define HWDEF_FIB_CUSTKEY_SIZE      816

/* --GAP-- 872 bytes [0x5082_0b68, 0x5082_0ecf] */

/* Mission firmware Flavor Selector: (4 bytes = 32 bits; [0x5082_0ed0, 0x5082_0ed3]
 * Since project A and project B may differ by features supported and 
 * integration models, a feature for project A may become a security hole 
 * for project B. The flavor selector value gets compared with the flavor 
 * selector value for a new image before proceeding with programming.
 */
#define HWDEF_FIB_FWFLAVOR          (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0ed0)

#define HWDEF_FIB_FWFLAVOR_SIZE    4

/* Security level (16 bytes = 128 bits; [0x5084_0ed8, 0x5084_0ee7] */
#define HWDEF_FIB_SECLEVEL          (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0ed8)
#define HWDEF_FIB_SECLEVEL_SIZE     16

/* WOF DAC thresholds. */
/* High bits are 2'b10 or 2'b01 to indicate validity */
/* Three separate 8-bit values in 23:16, 15:8, 7:0. */
/* To be programmed at ATE.  Requested by Gordon Shen on January 19, 2017. */
#define HWDEF_FIB_WOFDACOFF         (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0ee8)
#define HWDEF_FIB_WOFDACOFF_SIZE    4

/* Analog (AVCC) voltage trim (4 bytes = 32 bits; [0x5082_0ef0, 0x5082_0ef3] */
/* High bits are 2'b10 or 2'b01 to indicate validity */
/* If valid bits 11:0 are written to low 12 bits of SCM_LNA_AVREG0 and
 *  bits 23:12 are written to low 12 bits of SCM_LNA_AVREG1 */
#define HWDEF_FIB_AVCCTRIM          (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0xef0)
#define HWDEF_FIB_AVCCTRIM_SIZE     4

#define HWDEF_FIB_AVCCTRIM_AVREG_VALID 0xc0000000

/* bits 11:0 */
#define HWDEF_FIB_AVCCTRIM_AVREG0    0x00000fff
#define HWDEF_FIB_AVCCTRIM_AVREG0_B  0
#define HWDEF_FIB_AVCCTRIM_AVREG0_N  12

/* bits 23:12 */
#define HWDEF_FIB_AVCCTRIM_AVREG1    0x00fff000
#define HWDEF_FIB_AVCCTRIM_AVREG1_B  12
#define HWDEF_FIB_AVCCTRIM_AVREG1_N  12

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

/* Mission firmware size: 0x5082_0ef8 */
#define HWDEF_FIB_MISSIONSIZE       (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0ef8)
/* Iota start offset: 0x5082_0f00 */
#define HWDEF_FIB_IOTAOFF           (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0f00)
/* Iota size: 0x5082_0f04 */
#define HWDEF_FIB_IOTASIZE          (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0f04)

/* Low speed oscillator trim (4 bytes = 32 bits; [0x5082_0f08, 0x5082_0f0b] */
/* High bits are 2'b10 or 2'b01 to indicate validity */
/* Other bits fed directly into SCM_SOSC_BIAS_TRIM */
#define HWDEF_FIB_SLOWCLK           (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0f08)
#define HWDEF_FIB_SLOWCLK_SIZE      4

/* Firmware version (16 bytes = 128 bits; [0x5082_0f10, 0x5082_0f1f] */
#define HWDEF_FIB_FWVER             (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0f10)
#define HWDEF_FIB_FWVER_SIZE        16

/* Test record (128 bytes = 1024 bits; [0x5082_0f20, 0x5082_0f9f] */
#define HWDEF_FIB_TESTREC           (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0f20)
#define HWDEF_FIB_TESTREC_SIZE      128

/* Details of the test record: */
#define HWDEF_FIB_TESTREC_CP1_TEST_DATA                                      \
                                    (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0f34)
#define HWDEF_FIB_TESTREC_CP1_TEST_DATA_SIZE    12

#define HWDEF_FIB_TESTREC_CP2_TEST_DATA                                      \
                                    (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0f40)
#define HWDEF_FIB_TESTREC_CP2_TEST_DATA_SIZE    12

#define HWDEF_FIB_TESTREC_FT_TEST_DATA                                      \
                                    (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0f4c)
#define HWDEF_FIB_TESTREC_FT_TEST_DATA_SIZE 12

#define HWDEF_FIB_TESTREC_MT1_TEST_DATA                                      \
                                    (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0f58)
#define HWDEF_FIB_TESTREC_MT1_TEST_DATA_SIZE    12

#define HWDEF_FIB_TESTREC_MT2_TEST_DATA                                      \
                                    (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0f64)
#define HWDEF_FIB_TESTREC_MT2_TEST_DATA_SIZE    12

#define HWDEF_FIB_TESTREC_IST_TEST_DATA                                      \
                                    (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0f70)
#define HWDEF_FIB_TESTREC_IST_TEST_DATA_SIZE    12

#define HWDEF_FIB_TESTREC_ENGJ570_TEST_DATA                                  \
                                    (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0f7c)
#define HWDEF_FIB_TESTREC_ENGJ570_TEST_DATA_SIZE    12

#define HWDEF_FIB_TESTREC_BENCH_TEST_DATA                                   \
                                    (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0f88)
#define HWDEF_FIB_TESTREC_BENCH_TEST_DATA_SIZE  12

/* General product options (4 bytes = 32 bits; [0x5084_0fa0, 0x5084_0fa8]
 * Reserved 8bytes. */
#define HWDEF_FIB_PRODOPT           (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0fa0)
#define HWDEF_FIB_PRODOPT_SIZE      8

/*
 * "Master" key material.  This replaces the PUF.
 * (64 bytes; [0x5082_0fa8, 0x5082_00fe7])
 */
#define HWDEF_FIB_SEC_MKM           (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0fa8)
#define HWDEF_FIB_SEC_MKM_SIZE      64

/* -------------------------------------------------------------------------- */
/* page[1]high (never page-erasable, writable only in OPEN/SECURE/FA states)  */
/* These are hardware-defined locations */

/* Debug password (16 bytes = 128 bits; [0x5082_0fe8, 0x5082_0ff7] */
#define HWDEF_FIB_DEBUGPW           (HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE \
                                     + 0x0fe8)
#define HWDEF_FIB_DEBUGPW_SIZE      16

/* Security word (8 bytes = 64 bits; [0x5082_0ff8, 0x5082_0fff] */
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

/* Used with NASSAUOPT_CONFIGURABLE_RXTX.
 * These are all MAXIMUMS. */
#define HWDEF_AFE_NUM_RX_RECTANGLE  56
#define HWDEF_AFE_NUM_TX_RECTANGLE  112
#define HWDEF_AFE_NUM_RX_SQUARE  80
#define HWDEF_AFE_NUM_TX_SQUARE  88
/* NOTE: For Shasta, these values are also MAXIMUMS.
 * The values from VCSFW_FRAME_TAG_DIMS should be used
 * if the specific number of RX or TX is required.
 */
#define HWDEF_AFE_NUM_RX  (MAX((HWDEF_AFE_NUM_RX_RECTANGLE), (HWDEF_AFE_NUM_RX_SQUARE)))
#define HWDEF_AFE_NUM_TX  (MAX((HWDEF_AFE_NUM_TX_RECTANGLE), (HWDEF_AFE_NUM_TX_SQUARE)))
#define HWDEF_AFE_MAX_PIXELS  ((HWDEF_AFE_NUM_RX) * (HWDEF_AFE_NUM_TX))

/* Used with BOTH configurable and fixed setups */
#define HWDEF_AFE_NUM_SUBFRAMES 4

/* NOT used with NASSAUOPT_CONFIGURABLE_RXTX */
/* Nav frame acqusition needs Total Pixel Number for SQUARE Sensor*/
#define HWDEF_AFE_TOTAL_PIXELS  ((HWDEF_AFE_NUM_RX_SQUARE) * (HWDEF_AFE_NUM_TX_SQUARE))
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


#endif      /* __B1216HWDEF_H */
