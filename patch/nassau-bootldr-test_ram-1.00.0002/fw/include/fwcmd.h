/* -*- mode: c; tab-width: 4 -*- */
/* $Header$ */
/*
 * INTERNAL USE ONLY
 *
 * Copyright (c) 2010-2018 Synaptics Incorporated.  All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated ("Synaptics"). The holder of this file shall treat all
 * information contained herein as confidential, shall use the
 * information only for its intended purpose, and shall not duplicate,
 * disclose, or disseminate any of this information in any manner unless
 * Synaptics has otherwise provided express, written permission.
 */

/*
 * Tensilica common ASIC firmware: command definitions.
 *
 * NOTE: As of August 31, 2017 the authoritative copy of this file is
 *  in the general biometric firmware include area:
 *    ssh://gitms@git.synaptics.com/git/biometrics/fw/include.git
 *  Copies of this file may be checked into other places, but they
 *  should only be manually synchronized with the authoritative copy.
 *
 * DO NOT MAKE EDITS TO THIS FILE WHERE IT'S CHECKED INTO ANY LOCATION
 * THAN THE ABOVE.  YOUR EDITS WILL BE LOST.
 *
 *
 * The vcsfw_v4.h file (in CVS as ../../../falcon/shared/vcsfw_v4.h)
 *  combined with the vcsfw_v4priv.h file (in CVS as
 *  ../../../falcon/shared/vcsfw_v4priv.h) describe the "v4" interface
 *  used to control the Falcon, Raptor, Eagle, Osprey, Metallica,
 *  Viper, Viper2, Palisade (B1203), Denali (B1210), Hayes (B1212),
 *  Shasta (B1216), SC24 (B2224), and Prometheus (B1422) ASICs.
 * In these files, Validity defines a byte-oriented command and reply
 *  architecture.  That is: no requirement is made for items to be
 *  aligned in any way.  This was made as a conscious choice to make
 *  sure that host-side code always has the same idea of structure
 *  layout (i.e. no padding is inserted) and to optimize the use of
 *  the communications channel.
 * The Falcon/Raptor firmware uses the structure definitions in these
 *  files directly.  Unfortunately, this generates excessive code as
 *  the firmware must assume that data are not aligned.
 * However, in reality, most structures in these files are "naturally"
 *  aligned, by design.
 * This file acts as a "translator".  Here we have parallel structures
 *  for all in the two defining files.  Because these structures are
 *  defined without the "VCS_PACKED" attribute, and because they use
 *  the "native" types, the code generated when they are used is minimal.
 *
 * There is a diagnostic preprocessor macro that may be selected:
 *  FWCMD_FAILTEST   - Perform parallel structure tests for those
 *                        structures that we know will fail.  Using
 *                        this symbol is a handy way to denote those
 *                        structures for which we know there are problems.
 *
 * Bjoren Davis, September 20, 2010.
 *  Adapted for Savoy from engineering/private/impl/micro/windsor/fwcmn/include/fwcmd.h,v 1.77 2015/01/05 22:34:18 cbellman
 *   by Bjoren Davis on January 31, 2015.
 */

#ifndef __FWCMD_H
#define __FWCMD_H

/* Avoid conflicting declarations of VCS data types defined in vcsTypes.h */
#ifndef __VCSTYPES_H

#define __FWCMD_VCSTYPES__

typedef uint8_t vcsUint8_t;
/* We purposefully rewrite these types to non-scalar types to avoid
 *  accidental in-code use of the VCS_PACKED structures.
 */
typedef uint8_t vcsUint16_t[2];
typedef uint8_t vcsUint32_t[4];
typedef uint8_t vcsUint128_t[16];

typedef int8_t vcsInt8_t;
typedef uint8_t vcsInt16_t[2];
typedef uint8_t vcsInt32_t[4];

typedef uint8_t vcsBool_t[4];

#ifndef VCS_PACKED
#   if defined(ATTR_PACKED_STRUCT)
#       define VCS_PACKED ATTR_PACKED_STRUCT
#   else
#       define VCS_PACKED __attribute__ ((__packed__))
#   endif  /* !defined(ATTR_PACKED_STRUCT) */
#endif   /* VCS_PACKED */

#endif  /* __VCSTYPES_H */

#include "vcsfw_v4.h"
#include "vcsfw_v4priv.h"

/* Now remove the temptation to use this dreadful macro: */
#undef VCS_PACKED

/*
 * This is a simple macro to check a compile-time assertion.  This
 *  is identical to VFK_ASSERT_CT in vfk/vfkassert.h, but I reproduce
 *  it here so that we don't need to include the vfk to compile
 *  with this header.
 */

#define FWCMD_ASSERT_CT(name, x)                                            \
    typedef char fwcmd_##name [(x) ? 1 : -1]

/*
 * Make sure that the given element for the given structure lines
 *  up in terms of offset and size.  This macro makes the assumption
 *  that the structure in question is of the form:
 *      vcsfw_XXXXXX_t and fwcmd_XXXXXX_t
 */

#define FWCMD_ELCHECK(typename, elname)                                     \
    FWCMD_ASSERT_CT( typename ## _ ## elname ## _offset,                    \
                        OFFSETOF(fwcmd_ ## typename ## _t, elname)          \
                       == OFFSETOF(vcsfw_ ## typename ## _t, elname));      \
    FWCMD_ASSERT_CT(typename ## _ ## elname ## _size,                       \
                       ELSIZEOF(fwcmd_ ## typename ## _t, elname)           \
                       == ELSIZEOF(vcsfw_ ## typename ## _t, elname));

/*
 * Do the same as FWCMD_ELCHECK, but use an explicitly given name
 *  instead of elname.  This is needed for substructures.
 */
#define FWCMD_ELCHECK_NAMED(typename, elname, checkname)                    \
    FWCMD_ASSERT_CT( typename ## _ ## checkname ## _offset,                 \
                        OFFSETOF(fwcmd_ ## typename ## _t, elname)          \
                       == OFFSETOF(vcsfw_ ## typename ## _t, elname));      \
    FWCMD_ASSERT_CT(typename ## _ ## checkname ## _size,                    \
                       ELSIZEOF(fwcmd_ ## typename ## _t, elname)           \
                       == ELSIZEOF(vcsfw_ ## typename ## _t, elname));


/*
 * Make sure that the overall size of the two types is the same.  Again,
 *  as with FWCMD_ELCHECK, this macro assumes two types:
 *      vcsfw_XXXXXX_t and fwcmd_XXXXXX_t
 */

#define FWCMD_OVERALLSIZECHECK(typename)                                    \
    FWCMD_ASSERT_CT(typename ## _overallsize,                               \
                       sizeof(fwcmd_ ## typename ## _t)                     \
                       == sizeof(vcsfw_ ## typename ## _t));

/*
 * VCSFW_CMD_GET_PRINT parameters must be n*4bytes in order to maintain
 *  proper alignment.  This assertion makes sure that's true.
 */

#define FWCMD_PARAMSIZEX4(typename)                                         \
    FWCMD_ASSERT_CT(typename ## _paramsizex4,                               \
                       (sizeof(fwcmd_ ## typename ## _t) % 4) == 0);

/* -------------------------------------------------------------------------- */
/* New types and type-specific macros */

typedef struct fwcmd_generic_command_s
{
    uint8_t command;
} fwcmd_generic_command_t;

FWCMD_ELCHECK(generic_command, command)
FWCMD_OVERALLSIZECHECK(generic_command)

typedef struct fwcmd_generic_reply_s
{
    uint16_t status;
} fwcmd_generic_reply_t;

FWCMD_ELCHECK(generic_reply, status)
FWCMD_OVERALLSIZECHECK(generic_reply)

/****************************************************************************/
/* VCSFW_CMD_NOOP (0 = 0x00)                                                */
/****************************************************************************/

/****************************************************************************/
/* VCSFW_CMD_GET_VERSION (1 = 0x01)                                         */
/****************************************************************************/

typedef struct fwcmd_reply_get_version_s
{
    uint32_t   buildtime;        /* Unix-style build time, in seconds   */
                                 /*  from 1/1/1970 12:00 AM GMT         */
    uint32_t   buildnum;         /* build number                        */
    uint8_t    vmajor;           /* major version                       */
    uint8_t    vminor;           /* minor version                       */
    uint8_t    target;           /* target, e.g. VCSFW_TARGET_ROM       */
    uint8_t    product;          /* product, e.g.  VCSFW_PRODUCT_FALCON */
    uint8_t    siliconrev;       /* silicon revision                    */
    uint8_t    formalrel;        /* boolean: non-zero -> formal release */
    uint8_t    platform;         /* Platform (PCB) revision             */
    uint8_t    patch;            /* patch level                         */
    uint8_t    serial_number[6]; /* 48-bit Serial Number                */
    uint8_t    security[2];      /* bytes 0 and 1 of OTP                */
    uint32_t   patchsig;         /* opaque patch signature              */
    uint8_t    iface;            /* interface type, see below           */
    uint8_t    otpsig[3];        /* OTP Patch Signature                 */
    uint16_t   otpspare1;        /* spare space                         */
    uint8_t    reserved;         /* reserved byte                       */
    uint8_t    device_type;      /* device type                         */
} fwcmd_reply_get_version_t;

FWCMD_ELCHECK(reply_get_version, buildtime)
FWCMD_ELCHECK(reply_get_version, buildnum)
FWCMD_ELCHECK(reply_get_version, vmajor)
FWCMD_ELCHECK(reply_get_version, vminor)
FWCMD_ELCHECK(reply_get_version, target)
FWCMD_ELCHECK(reply_get_version, product)
FWCMD_ELCHECK(reply_get_version, siliconrev)
FWCMD_ELCHECK(reply_get_version, formalrel)
FWCMD_ELCHECK(reply_get_version, platform)
FWCMD_ELCHECK(reply_get_version, patch)
FWCMD_ELCHECK(reply_get_version, serial_number)
FWCMD_ELCHECK(reply_get_version, security)
FWCMD_ELCHECK(reply_get_version, patchsig)
FWCMD_ELCHECK(reply_get_version, iface)
FWCMD_ELCHECK(reply_get_version, otpsig)
FWCMD_ELCHECK(reply_get_version, otpspare1)
FWCMD_ELCHECK(reply_get_version, reserved)
FWCMD_ELCHECK(reply_get_version, device_type)
FWCMD_OVERALLSIZECHECK(reply_get_version)


/****************************************************************************/
/* VCSFW_CMD_GET_PRINT (2 = 0x02)                                           */
/****************************************************************************/

typedef struct fwcmd_cmd_get_print_s
{
    uint16_t     linelen;    /* number of bytes/line */
    uint16_t     nlines;     /* number of lines: 0 = unlimited */
} fwcmd_cmd_get_print_t;

FWCMD_ELCHECK(cmd_get_print, linelen)
FWCMD_ELCHECK(cmd_get_print, nlines)
FWCMD_OVERALLSIZECHECK(cmd_get_print)

/****************************************************************************/
/* VCSFW_CMD_STOP_PRINT (4 = 0x04)                                          */
/****************************************************************************/

/****************************************************************************/
/* VCSFW_CMD_RESET (5 = 0x05)                                               */
/****************************************************************************/

typedef struct fwcmd_cmd_reset_s
{
    uint16_t     delaytime;    /* time (in ms) to delay after reply */
} fwcmd_cmd_reset_t;

FWCMD_ELCHECK(cmd_reset, delaytime)
FWCMD_OVERALLSIZECHECK(cmd_reset)

/****************************************************************************/
/* VCSFW_CMD_PATCH (6 = 0x06)                                               */
/****************************************************************************/

typedef struct fwcmd_cmd_patch_s
{
    uint16_t   tag;
    uint16_t   length;
} fwcmd_cmd_patch_t;

FWCMD_ELCHECK(cmd_patch, tag)
FWCMD_ELCHECK(cmd_patch, length)
FWCMD_OVERALLSIZECHECK(cmd_patch)

typedef struct fwcmd_cmd_patch_relochdr_s
{
    uint32_t     linkaddr;
    uint32_t     entryaddr;
    uint16_t     nfixups24;
    uint16_t     nfixups32;
} fwcmd_cmd_patch_relochdr_t;

FWCMD_ELCHECK(cmd_patch_relochdr, linkaddr)
FWCMD_ELCHECK(cmd_patch_relochdr, entryaddr)
FWCMD_ELCHECK(cmd_patch_relochdr, nfixups24)
FWCMD_ELCHECK(cmd_patch_relochdr, nfixups32)
FWCMD_OVERALLSIZECHECK(cmd_patch_relochdr)


/****************************************************************************/
/* VCSFW_CMD_PEEK (7 = 0x07)                                                */
/****************************************************************************/

typedef struct fwcmd_cmd_peek_s
{
    uint32_t   address;
    uint8_t    opsize;
} fwcmd_cmd_peek_t;

FWCMD_ELCHECK(cmd_peek, address)
FWCMD_ELCHECK(cmd_peek, opsize)
#if defined(FWCMD_FAILTEST)
/* Assertion fails because native structure adds end padding. Usually safe. */
FWCMD_OVERALLSIZECHECK(cmd_peek)
#endif      /* defined(FWCMD_FAILTEST) */

typedef struct fwcmd_reply_peek_s
{
    uint32_t   value;
} fwcmd_reply_peek_t;

FWCMD_ELCHECK(reply_peek, value)
FWCMD_OVERALLSIZECHECK(reply_peek)


/****************************************************************************/
/* VCSFW_CMD_POKE (8 = 0x08)                                                */
/****************************************************************************/

typedef struct fwcmd_cmd_poke_s
{
    uint32_t   address;
    uint32_t   value;
    uint8_t    opsize;
} fwcmd_cmd_poke_t;

FWCMD_ELCHECK(cmd_poke, address)
FWCMD_ELCHECK(cmd_poke, value)
FWCMD_ELCHECK(cmd_poke, opsize)
#if defined(FWCMD_FAILTEST)
/* Assertion fails because native structure adds end padding. Usually safe. */
FWCMD_OVERALLSIZECHECK(cmd_poke)
#endif      /* defined(FWCMD_FAILTEST) */

/****************************************************************************/
/* VCSFW_CMD_SET_REFCLK (9 = 0x09)                                          */
/****************************************************************************/

typedef struct fwcmd_cmd_set_refclk_s
{
    uint32_t refclock;
    uint32_t fastclock;
    uint32_t slowclock;
    uint32_t scdiv;
    uint32_t plldiv;
} fwcmd_cmd_set_refclk_t;

FWCMD_ELCHECK(cmd_set_refclk, refclock)
FWCMD_ELCHECK(cmd_set_refclk, fastclock)
FWCMD_ELCHECK(cmd_set_refclk, slowclock)
FWCMD_ELCHECK(cmd_set_refclk, scdiv)
FWCMD_ELCHECK(cmd_set_refclk, plldiv)
FWCMD_OVERALLSIZECHECK(cmd_set_refclk)

/****************************************************************************/
/* VCSFW_CMD_SET_SPIMODE (10 = 0x0a)                                        */
/****************************************************************************/

typedef struct fwcmd_cmd_set_spimode_s
{
    uint32_t   spiflags;
} fwcmd_cmd_set_spimode_t;

FWCMD_ELCHECK(cmd_set_spimode, spiflags)
FWCMD_OVERALLSIZECHECK(cmd_set_spimode)

/****************************************************************************/
/* VCSFW_CMD_SPI_TRANSFER (11 = 0x0b)                                       */
/****************************************************************************/

typedef struct fwcmd_cmd_spi_transfer_s
{
    uint8_t  iv[VCSFW_SECURITY_IV_SIZE];
    uint32_t enc_length;
    uint32_t data_length;
    uint16_t enc_offset;
    uint16_t mode;
    uint8_t  port;
    uint8_t  unused[5];
} fwcmd_cmd_spi_transfer_t;

FWCMD_ELCHECK(cmd_spi_transfer, iv)
FWCMD_ELCHECK(cmd_spi_transfer, enc_length)
FWCMD_ELCHECK(cmd_spi_transfer, data_length)
FWCMD_ELCHECK(cmd_spi_transfer, enc_offset)
FWCMD_ELCHECK(cmd_spi_transfer, mode)
FWCMD_ELCHECK(cmd_spi_transfer, port)
FWCMD_ELCHECK(cmd_spi_transfer, unused)
#if defined(FWCMD_FAILTEST)
/* Assertion fails because native structure adds end padding. Usually safe. */
FWCMD_OVERALLSIZECHECK(cmd_spi_transfer)
#endif      /* defined(FWCMD_FAILTEST) */


/****************************************************************************/
/* VCSFW_CMD_WOE_ENTER (12 = 0x0c)                                          */
/****************************************************************************/

typedef struct fwcmd_cmd_woe_enter_s
{
    uint16_t     delaytime;
} fwcmd_cmd_woe_enter_t;

FWCMD_ELCHECK(cmd_woe_enter, delaytime)
FWCMD_OVERALLSIZECHECK(cmd_woe_enter)

/****************************************************************************/
/* VCSFW_CMD_GPIO (13 = 0x0d)                                               */
/****************************************************************************/

typedef struct fwcmd_gpio_reg_mask_s
{
    uint32_t   newval;
    uint32_t   mask;
} fwcmd_gpio_reg_mask_t;

FWCMD_ELCHECK(gpio_reg_mask, newval)
FWCMD_ELCHECK(gpio_reg_mask, mask)
FWCMD_OVERALLSIZECHECK(gpio_reg_mask)

typedef struct fwcmd_cmd_gpio_s
{
    fwcmd_gpio_reg_mask_t   alt;
    fwcmd_gpio_reg_mask_t   dir;
    fwcmd_gpio_reg_mask_t   reserved;
    fwcmd_gpio_reg_mask_t   dout;
    fwcmd_gpio_reg_mask_t   wpu;
    fwcmd_gpio_reg_mask_t   hdrv;
    fwcmd_gpio_reg_mask_t   alts;
    fwcmd_gpio_reg_mask_t   ien;
    fwcmd_gpio_reg_mask_t   pdr;
} fwcmd_cmd_gpio_t;

FWCMD_ELCHECK(cmd_gpio, alt)
FWCMD_ELCHECK(cmd_gpio, dir)
FWCMD_ELCHECK(cmd_gpio, reserved)
FWCMD_ELCHECK(cmd_gpio, dout)
FWCMD_ELCHECK(cmd_gpio, wpu)
FWCMD_ELCHECK(cmd_gpio, hdrv)
FWCMD_ELCHECK(cmd_gpio, alts)
FWCMD_ELCHECK(cmd_gpio, ien)
FWCMD_ELCHECK(cmd_gpio, pdr)
FWCMD_OVERALLSIZECHECK(cmd_gpio)

typedef struct fwcmd_reply_gpio_s
{
    uint32_t   alt;
    uint32_t   dir;
    uint32_t   din;
    uint32_t   dout;
    uint32_t   wpu;
    uint32_t   hdrv;
    uint32_t   alts;
    uint32_t   ien;
    uint32_t   pdr;
} fwcmd_reply_gpio_t;

FWCMD_ELCHECK(reply_gpio, alt)
FWCMD_ELCHECK(reply_gpio, dir)
FWCMD_ELCHECK(reply_gpio, din)
FWCMD_ELCHECK(reply_gpio, dout)
FWCMD_ELCHECK(reply_gpio, wpu)
FWCMD_ELCHECK(reply_gpio, hdrv)
FWCMD_ELCHECK(reply_gpio, alts)
FWCMD_ELCHECK(reply_gpio, ien)
FWCMD_ELCHECK(reply_gpio, pdr)
FWCMD_OVERALLSIZECHECK(reply_gpio)

/****************************************************************************/
/* VCSFW_CMD_PROVISION (14 = 0x0e)                                          */
/****************************************************************************/

typedef struct fwcmd_cmd_provision_s
{
    uint32_t options;
} fwcmd_cmd_provision_t;

FWCMD_ELCHECK(cmd_provision, options)
FWCMD_OVERALLSIZECHECK(cmd_provision)


/****************************************************************************/
/* VCSFW_CMD_WOE_SETUP (18 = 0x12)                                          */
/****************************************************************************/

typedef struct fwcmd_cmd_woe_setup_s
{
    uint32_t Parameters[14];
} fwcmd_cmd_woe_setup_t;

FWCMD_ELCHECK(cmd_woe_setup, Parameters)
FWCMD_OVERALLSIZECHECK(cmd_woe_setup)

/****************************************************************************/
/* VCSFW_CMD_SET_CPUCLK (19 = 0x13)                                         */
/****************************************************************************/

typedef struct fwcmd_cmd_set_cpuclk_s
{
    uint32_t     run_cpuclkctrl;
    uint32_t     wait_cpuclkctrl;
} fwcmd_cmd_set_cpuclk_t;

FWCMD_ELCHECK(cmd_set_cpuclk, run_cpuclkctrl)
FWCMD_ELCHECK(cmd_set_cpuclk, wait_cpuclkctrl)
FWCMD_OVERALLSIZECHECK(cmd_set_cpuclk)

/****************************************************************************/
/* VCSFW_CMD_RESERVED_1 (20 = 0x14)                                         */
/****************************************************************************/

/****************************************************************************/
/* VCSFW_CMD_GET_CONFIGURATION (21 = 0x15)                                  */
/****************************************************************************/

#if defined(FWCMD_FAILTEST)
/* This structure is not naturally aligned, so I've purposefully excluded
 *  it. */
typedef struct fwcmd_reply_get_configuration_s
{
    uint16_t reserved;
    uint32_t configuration[VCSFW_CONFIG_SIZE];
} fwcmd_reply_get_configuration_t;

FWCMD_ELCHECK(reply_get_configuration, reserved)
FWCMD_ELCHECK(reply_get_configuration, configuration)
FWCMD_OVERALLSIZECHECK(reply_get_configuration)
#endif      /* defined(FWCMD_FAILTEST) */

/****************************************************************************/
/* VCSFW_CMD_GET_FINGER_STATUS (23 = 0x17)                                  */
/****************************************************************************/

#if defined(FWCMD_FAILTEST)
/* This structure is not naturally aligned, so I've purposefully excluded
 *  it. */
typedef struct fwcmd_reply_get_finger_status_s
{
    uint8_t      status;
    uint32_t     accumwoe;
} fwcmd_reply_get_finger_status_t;

FWCMD_ELCHECK(reply_get_finger_status, status)
FWCMD_ELCHECK(reply_get_finger_status, accumwoe)
FWCMD_OVERALLSIZECHECK(reply_get_finger_status)
#endif      /* defined(FWCMD_FAILTEST) */

/****************************************************************************/
/* VCSFW_CMD_RESERVED_2 (24 = 0x18)                                         */
/****************************************************************************/

/****************************************************************************/
/* VCSFW_CMD_GET_STARTINFO (25 = 0x19)                                      */
/****************************************************************************/

#if defined(FWCMD_FAILTEST)
/* This structure is not naturally aligned, so I've purposefully excluded
 *  it. */
typedef struct fwcmd_reply_get_startinfo_s
{
    uint8_t start_type;
    uint8_t reset_type;
    uint32_t start_status;
    uint32_t sanity_pc;
    uint32_t sanity_code;
    uint32_t reset_nvinfo[13];
} fwcmd_reply_get_startinfo_t;

FWCMD_ELCHECK(reply_get_startinfo, start_type)
FWCMD_ELCHECK(reply_get_startinfo, reset_type)
FWCMD_ELCHECK(reply_get_startinfo, start_status)
FWCMD_ELCHECK(reply_get_startinfo, sanity_pc)
FWCMD_ELCHECK(reply_get_startinfo, sanity_code)
FWCMD_ELCHECK(reply_get_startinfo, reset_nvinfo)
FWCMD_OVERALLSIZECHECK(reply_get_startinfo)
#endif      /* defined(FWCMD_FAILTEST) */

/****************************************************************************/
/* VCSFW_CMD_UNLOAD_PATCH (26 = 0x1a)                                       */
/****************************************************************************/

/* This is optional: */
typedef struct fwcmd_cmd_unload_patch_s
{
    uint32_t     patchidx;
} fwcmd_cmd_unload_patch_t;

FWCMD_ELCHECK(cmd_unload_patch, patchidx)
FWCMD_OVERALLSIZECHECK(cmd_unload_patch)

/****************************************************************************/
/* VCSFW_CMD_MATCH_VERIFY (28 = 0x1c)                                       */
/****************************************************************************/

/* Command */
typedef struct fwcmd_cmd_match_verify_s
{
    uint16_t size;
    uint16_t reserved;   /* Align to eliminate large allocation */
    uint8_t  cookie[VCSFW_SHA256_HASH_SIZE];
} fwcmd_cmd_match_verify_t;

FWCMD_ELCHECK(cmd_match_verify, size)
FWCMD_ELCHECK(cmd_match_verify, reserved)
FWCMD_ELCHECK(cmd_match_verify, cookie)
FWCMD_OVERALLSIZECHECK(cmd_match_verify)

/* Reply */
typedef struct fwcmd_reply_match_verify_s
{
    uint16_t size;
    uint8_t  err[16];
} fwcmd_reply_match_verify_t;

FWCMD_ELCHECK(reply_match_verify, size)
FWCMD_ELCHECK(reply_match_verify, err)
FWCMD_OVERALLSIZECHECK(reply_match_verify)

/****************************************************************************/
/* VCSFW_CMD_SIGN_ENC_TEMPLATE_DATA (29 = 0x1d)                             */
/****************************************************************************/

/* Command */
typedef struct fwcmd_cmd_sign_enc_template_data_s
{
    uint16_t     templateSize;
    uint16_t     payloadSize;
} fwcmd_cmd_sign_enc_template_data_t;

FWCMD_ELCHECK(cmd_sign_enc_template_data, templateSize)
FWCMD_ELCHECK(cmd_sign_enc_template_data, payloadSize)
FWCMD_OVERALLSIZECHECK(cmd_sign_enc_template_data)


/* Reply */
typedef struct fwcmd_reply_sign_enc_template_data_s
{
    uint16_t     dataSize;
} fwcmd_reply_sign_enc_template_data_t;

FWCMD_ELCHECK(reply_sign_enc_template_data, dataSize)
FWCMD_OVERALLSIZECHECK(reply_sign_enc_template_data)


/****************************************************************************/
/* VCSFW_CMD_DEC_VERIFY_TEMPLATE_DATA (30 = 0x1e)                           */
/****************************************************************************/

/* Command */
typedef struct fwcmd_cmd_dec_verify_template_data_s
{
    uint16_t     dataSize;
    uint16_t     passwordLength;
    uint8_t      unused[12]; /*  template data should start at  */
                            /*  16 byte aligned address */
    uint8_t      password[VCSFW_SECURITY_MAX_PASSWORD_SIZE];
    uint8_t      externalFile[VCSFW_SHA256_HASH_SIZE];
} fwcmd_cmd_dec_verify_template_data_t;

FWCMD_ELCHECK(cmd_dec_verify_template_data, dataSize)
FWCMD_ELCHECK(cmd_dec_verify_template_data, passwordLength)
FWCMD_ELCHECK(cmd_dec_verify_template_data, unused)
FWCMD_ELCHECK(cmd_dec_verify_template_data, password)
FWCMD_ELCHECK(cmd_dec_verify_template_data, externalFile)
FWCMD_OVERALLSIZECHECK(cmd_dec_verify_template_data)

/* Reply */
typedef struct fwcmd_reply_dec_verify_template_data_s
{
    uint16_t     templateSize;
    uint16_t     payloadSize;
} fwcmd_reply_dec_verify_template_data_t;

FWCMD_ELCHECK(reply_dec_verify_template_data, templateSize)
FWCMD_ELCHECK(reply_dec_verify_template_data, payloadSize)
FWCMD_OVERALLSIZECHECK(reply_dec_verify_template_data)

/****************************************************************************/
/* VCSFW_CMD_SPISLV_FAST_READ (31 = 0x1f)                                   */
/****************************************************************************/

typedef struct fwcmd_cmd_spislv_fast_read_s
{
    uint32_t write_length;
    uint32_t read_length;
    uint8_t  port;
    uint8_t  unused[105];
} fwcmd_cmd_spislv_fast_read_t;

FWCMD_ELCHECK(cmd_spislv_fast_read, write_length)
FWCMD_ELCHECK(cmd_spislv_fast_read, read_length)
FWCMD_ELCHECK(cmd_spislv_fast_read, port)
FWCMD_ELCHECK(cmd_spislv_fast_read, unused)
#if defined(FWCMD_FAILTEST)
/* Assertion fails because native structure adds end padding. Usually safe. */
FWCMD_OVERALLSIZECHECK(cmd_spislv_fast_read)
#endif      /* defined(FWCMD_FAILTEST) */

/****************************************************************************/
/* VCSFW_CMD_TEST_USB (32 = 0x20)                                           */
/****************************************************************************/
/* This is part of module test.  It is not used in the main firmware. */

/****************************************************************************/
/* VCSFW_CMD_TEST_ROM_CHECKSUM (33 = 0x21)                                  */
/****************************************************************************/
/* This is part of module test.  It is not used in the main firmware. */

/****************************************************************************/
/* VCSFW_CMD_TEST_OTP_READ (34 = 0x22)                                      */
/****************************************************************************/
/* This is part of module test.  It is not used in the main firmware. */

/****************************************************************************/
/* VCSFW_CMD_TEST_OTP_WRITE (35 = 0x23)                                     */
/****************************************************************************/
/* This is part of module test.  It is not used in the main firmware. */

/****************************************************************************/
/* VCSFW_CMD_LED (36 = 0x24)                                                */
/****************************************************************************/

typedef struct fwcmd_cmd_led_s
{
    fwcmd_cmd_gpio_t        gpio_reg;
    fwcmd_gpio_reg_mask_t   state_a;
    fwcmd_gpio_reg_mask_t   state_b;
    fwcmd_gpio_reg_mask_t   state_c;
    fwcmd_gpio_reg_mask_t   state_d;
    uint32_t             state_ab_duty_cycle;
    uint32_t             state_cd_duty_cycle;
    uint16_t             state_ab_time;
    uint16_t             state_cd_time;
    uint8_t              finger_down_state;
    uint8_t              timeout;
} fwcmd_cmd_led_t;

FWCMD_ELCHECK(cmd_led, gpio_reg)
FWCMD_ELCHECK(cmd_led, state_a)
FWCMD_ELCHECK(cmd_led, state_b)
FWCMD_ELCHECK(cmd_led, state_c)
FWCMD_ELCHECK(cmd_led, state_d)
FWCMD_ELCHECK(cmd_led, state_ab_duty_cycle)
FWCMD_ELCHECK(cmd_led, state_cd_duty_cycle)
FWCMD_ELCHECK(cmd_led, state_ab_time)
FWCMD_ELCHECK(cmd_led, state_cd_time)
FWCMD_ELCHECK(cmd_led, finger_down_state)
FWCMD_ELCHECK(cmd_led, timeout)
#if defined(FWCMD_FAILTEST)
/* Assertion fails because native structure adds end padding. Usually safe. */
FWCMD_OVERALLSIZECHECK(cmd_led)
#endif      /* defined(FWCMD_FAILTEST) */

/****************************************************************************/
/* VCSFW_CMD_TEST_PROVISION (37 = 0x25)                                     */
/****************************************************************************/
/* This is part of module test.  It is not used in the main firmware. */

/****************************************************************************/
/* VCSFW_CMD_GET_OWNERSHIP_INFO (38 = 0x26)                                 */
/****************************************************************************/

/* Command */
/*   - Uses fwcmd_generic_command_t                                         */

/* Reply */
typedef struct fwcmd_reply_get_ownership_info_s
{
    uint16_t TotalCycles;              /* how many cycles were possible at */
                                       /* manufacturing time               */
    uint16_t AvailableCycles;          /* how many cycles are still        */
                                       /* available                        */
} fwcmd_reply_get_ownership_info_t;

FWCMD_ELCHECK(reply_get_ownership_info, TotalCycles)
FWCMD_ELCHECK(reply_get_ownership_info, AvailableCycles)
FWCMD_OVERALLSIZECHECK(reply_get_ownership_info)

/****************************************************************************/
/* VCSFW_CMD_GET_UID (39 = 0x27)                                            */
/****************************************************************************/

typedef struct fwcmd_reply_get_uid_s
{
    uint16_t vid;
    uint16_t pid;
    uint8_t uid[VCSFW_RAPTOR_UID_SIZE];
} fwcmd_reply_get_uid_t;

FWCMD_ELCHECK(reply_get_uid, vid)
FWCMD_ELCHECK(reply_get_uid, pid)
FWCMD_ELCHECK(reply_get_uid, uid)
FWCMD_OVERALLSIZECHECK(reply_get_uid)

/****************************************************************************/
/* VCSFW_CMD_KEY_EXCHANGE_DH (40 = 0x28)                                    */
/****************************************************************************/

/* Command */
typedef struct fwcmd_cmd_key_exchange_dh_s
{
    uint8_t      generator[VCSFW_DH_BLOCK_LENGTH];
    uint8_t      host_pub_key[VCSFW_DH_BLOCK_LENGTH];
    uint8_t      prime[VCSFW_DH_BLOCK_LENGTH];
} fwcmd_cmd_key_exchange_dh_t;

FWCMD_ELCHECK(cmd_key_exchange_dh, generator)
FWCMD_ELCHECK(cmd_key_exchange_dh, host_pub_key)
FWCMD_ELCHECK(cmd_key_exchange_dh, prime)
FWCMD_OVERALLSIZECHECK(cmd_key_exchange_dh)

/* Reply */
typedef struct fwcmd_reply_key_exchange_dh_s
{
    uint8_t      sensor_pub_key[VCSFW_DH_BLOCK_LENGTH];
} fwcmd_reply_key_exchange_dh_t;

FWCMD_ELCHECK(reply_key_exchange_dh, sensor_pub_key)
FWCMD_OVERALLSIZECHECK(reply_key_exchange_dh)


/****************************************************************************/
/* VCSFW_CMD_GET_CERTIFICATE (41 = 0x2b)                                    */
/****************************************************************************/

#if defined(FWCMD_FAILTEST)
/* This entire structure is no good.  */
/* REPLY                                                                    */
typedef struct fwcmd_reply_get_certificate_s {
    uint16_t         key_length;     /* in bytes */
    struct {
        uint32_t     pub_exp;    /* it is always 65537 */
        uint8_t      modulus[VCSFW_MAX_RSA_KEY_LENGTH]; /* little endian */
    } pub_key;
    uint8_t          sign[VCSFW_MAX_RSA_KEY_LENGTH];    /* big endian */
} fwcmd_reply_get_certificate_t;

FWCMD_ELCHECK(reply_get_certificate, key_length)
FWCMD_ELCHECK(reply_get_certificate, pub_key)
FWCMD_ELCHECK(reply_get_certificate, sign)
FWCMD_OVERALLSIZECHECK(reply_get_certificate)
#endif      /* defined(FWCMD_FAILTEST) */

/****************************************************************************/
/* VCSFW_CMD_LED_EX (42 = 0x2a)                                             */
/****************************************************************************/

#if defined(FWCMD_FAILTEST)
/* This structure is not naturally aligned, so I've purposefully excluded
 *  it. */

/*
 * WARNING: The names of "newled_state_t" and "newled_control_t" in
 *  vcsfw_v4.h are non-standard, and so we can't use the FWCMD_ELCHECK
 *  and FWCMD_OVERALLSIZECHECK macros.
 */

typedef struct fwcmd_newled_state_s {
    uint8_t     onoff;
    uint8_t     startbrightness;
    int8_t      step;
    uint8_t     steptime;
    uint8_t     maxbrightness;
    uint16_t    stdwelltime;
} fwcmd_newled_state_t;

FWCMD_ASSERT_CT(newled_state_onoff_offset,
                   OFFSETOF(fwcmd_newled_state_t, onoff)
                   == OFFSETOF(newled_state_t, onoff));
FWCMD_ASSERT_CT(newled_state_onoff_size,
                   ELSIZEOF(fwcmd_newled_state_t, onoff)
                   == ELSIZEOF(newled_state_t, onoff));

FWCMD_ASSERT_CT(newled_state_startbrightness_offset,
                   OFFSETOF(fwcmd_newled_state_t, startbrightness)
                   == OFFSETOF(newled_state_t, startbrightness));
FWCMD_ASSERT_CT(newled_state_startbrightness_size,
                   ELSIZEOF(fwcmd_newled_state_t, startbrightness)
                   == ELSIZEOF(newled_state_t, startbrightness));

FWCMD_ASSERT_CT(newled_state_step_offset,
                   OFFSETOF(fwcmd_newled_state_t, step)
                   == OFFSETOF(newled_state_t, step));
FWCMD_ASSERT_CT(newled_state_step_size,
                   ELSIZEOF(fwcmd_newled_state_t, step)
                   == ELSIZEOF(newled_state_t, step));

FWCMD_ASSERT_CT(newled_state_steptime_offset,
                   OFFSETOF(fwcmd_newled_state_t, steptime)
                   == OFFSETOF(newled_state_t, steptime));
FWCMD_ASSERT_CT(newled_state_steptime_size,
                   ELSIZEOF(fwcmd_newled_state_t, steptime)
                   == ELSIZEOF(newled_state_t, steptime));

FWCMD_ASSERT_CT(newled_state_maxbrightness_offset,
                   OFFSETOF(fwcmd_newled_state_t, maxbrightness)
                   == OFFSETOF(newled_state_t, maxbrightness));
FWCMD_ASSERT_CT(newled_state_maxbrightness_size,
                   ELSIZEOF(fwcmd_newled_state_t, maxbrightness)
                   == ELSIZEOF(newled_state_t, maxbrightness));

FWCMD_ASSERT_CT(newled_state_stdwelltime_offset,
                   OFFSETOF(fwcmd_newled_state_t, stdwelltime)
                   == OFFSETOF(newled_state_t, stdwelltime));
FWCMD_ASSERT_CT(newled_state_stdwelltime_size,
                   ELSIZEOF(fwcmd_newled_state_t, stdwelltime)
                   == ELSIZEOF(newled_state_t, stdwelltime));

FWCMD_ASSERT_CT(newled_state_overallsize,
                   sizeof(fwcmd_newled_state_t) == sizeof(newled_state_t));

typedef struct fwcmd_newled_control_s {
    fwcmd_newled_state_t    led_state[MAXSTATES];
} fwcmd_newled_control_t;

FWCMD_ASSERT_CT(newled_control_led_state_offset,
                   OFFSETOF(fwcmd_newled_control_t, led_state)
                   == OFFSETOF(newled_control_t, led_state));
FWCMD_ASSERT_CT(newled_control_led_state_size,
                   ELSIZEOF(fwcmd_newled_control_t, led_state)
                   == ELSIZEOF(newled_control_t, led_state));

FWCMD_ASSERT_CT(newled_control_overallsize,
                   sizeof(fwcmd_newled_control_t)
                   == sizeof(newled_control_t));

typedef struct fwcmd_led_cmd_ex_s {
    fwcmd_newled_control_t  led_control[MAXLEDS];
    uint32_t    dwelltime;
} fwcmd_cmd_led_ex_t;

FWCMD_ELCHECK(cmd_led_ex, led_control)
FWCMD_ELCHECK(cmd_led_ex, dwelltime)
FWCMD_OVERALLSIZECHECK(cmd_led_ex)

#endif      /* defined(FWCMD_FAILTEST) */

/****************************************************************************/
/* VCSFW_CMD_GET_AUTH_DATA (43 = 0x2b)                                      */
/****************************************************************************/

/* Command */
typedef struct fwcmd_cmd_get_auth_data_s
{
    uint16_t auth_blob_size;
    uint16_t add_info_size;
} fwcmd_cmd_get_auth_data_t;

FWCMD_ELCHECK(cmd_get_auth_data, auth_blob_size)
FWCMD_ELCHECK(cmd_get_auth_data, add_info_size)
FWCMD_OVERALLSIZECHECK(cmd_get_auth_data)

/* Reply */
typedef struct fwcmd_reply_get_auth_data_s
{
    uint16_t auth_data_size;
} fwcmd_reply_get_auth_data_t;

FWCMD_ELCHECK(reply_get_auth_data, auth_data_size)
FWCMD_OVERALLSIZECHECK(reply_get_auth_data)

/****************************************************************************/
/* VCSFW_CMD_REWRITE_PM_SECTOR (45 = 0x2d)                                  */
/****************************************************************************/

/****************************************************************************/
/* VCSFW_CMD_GET_ECC_CORRECTION_INFO (48 = 0x30)                            */
/****************************************************************************/
/* This is part of module test.  It is not used in the main firmware. */

/****************************************************************************/
/* VCSFW_CMD_VIP_CREATE_CLIENT_AUTH_TOKEN (49 = 0x31)                       */
/****************************************************************************/
/* This is part of VeriSign VIP OTP support patch.
 *  It is not used in the main firmware. */

/****************************************************************************/
/* VCSFW_CMD_VIP_PROVISION_OTP_SEED (50 = 0x32)                             */
/****************************************************************************/
/* This is part of VeriSign VIP OTP support patch.
 *  It is not used in the main firmware. */

/****************************************************************************/
/* VCSFW_CMD_VIP_CREATE_TOTP_TOKEN (51 = 0x33)                              */
/****************************************************************************/
/* This is part of VeriSign VIP OTP support patch.
 *  It is not used in the main firmware. */

/****************************************************************************/
/* VCSFW_CMD_TEST_RESET_OWNERSHIP (52 = 0x34)                               */
/****************************************************************************/
/* This is part of module test.  It is not used in the main firmware. */

/****************************************************************************/
/* VCSFW_CMD_POKE32 (53 = 0x35)                                             */
/****************************************************************************/
/*
 * Poke with a n*4 byte command length.  This is really used as
 *  a GET_PRINT tag.
 */

typedef struct fwcmd_cmd_poke32_s
{
    uint32_t        address;
    uint32_t        value;
} fwcmd_cmd_poke32_t;

FWCMD_ELCHECK(cmd_poke32, address)
FWCMD_ELCHECK(cmd_poke32, value)
FWCMD_OVERALLSIZECHECK(cmd_poke32)

/****************************************************************************/
/* VCSFW_CMD_INPLACE_PATCH (54 = 0x36)                                      */
/****************************************************************************/

/*
 * Load a patch that lives forever in the command buffer into
 *  which it was received.
 */

/*
 * Note that this structure is designed to TRAIL the patch.
 *  Also note that, when in the command flow, the structure
 *  is not necessarily aligned.
 */

typedef struct fwcmd_cmd_inplace_patch_s
{
    uint32_t      loadaddr;      /* expected address of first word */
    uint32_t      startaddr;     /* jump here to start the patch */
} fwcmd_cmd_inplace_patch_t;

FWCMD_ELCHECK(cmd_inplace_patch, loadaddr)
FWCMD_ELCHECK(cmd_inplace_patch, startaddr)
FWCMD_OVERALLSIZECHECK(cmd_inplace_patch)


/****************************************************************************/
/* VCSFW_CMD_GEN_SEC_KEY (55 = 0x37)                                        */
/****************************************************************************/
/* This is part of 4_52s_preboot patch.
 *  It is not used in the main firmware. */

/****************************************************************************/
/* VCSFW_CMD_GET_CFGTSBL (56 = 0x38)                                        */
/****************************************************************************/
/* The command uses the standard form (no parameters), but the
 *  reply is more complicated.  It is too complicated for a C
 *  structure.
 */

/****************************************************************************/
/* VCSFW_CMD_LED_EX2 (57 = 0x39)                                        */
/****************************************************************************/
/* COMMAND                                                                  */
/*
 * This command allows blinking/dimming/ramping of LEDs on Eagle-based hardware
 * This differs from VCSFW_CMD_LED_EX in that it is organized by state (all
 * LEDs change state at the same time) as opposed to LED_EX which is organized
 * by LED (each LED can change states at different times).  This matches the
 * hardware implementations in Eagle and follow-on devices
 * Note:  For Eagle, LED0-2 are used to specify LED behavior during normal
 * operation.  LED3 of State 0 is used to specify LED behavior during suspend -
 *
*/
typedef struct fwcmd_cmd_led_ex2_led_s {
    uint8_t maxbrightness;   /* 0-255, current limit for the LED (WOE_LED)*/
    uint8_t startbrightness; /* Starting PWM Brightness          (PWM_LED)*/
    int8_t  reserved1;
    uint8_t reserved2;
} fwcmd_cmd_led_ex2_led_t;

FWCMD_ELCHECK(cmd_led_ex2_led, maxbrightness)
FWCMD_ELCHECK(cmd_led_ex2_led, startbrightness)
FWCMD_ELCHECK(cmd_led_ex2_led, reserved1)
FWCMD_ELCHECK(cmd_led_ex2_led, reserved2)
FWCMD_OVERALLSIZECHECK(cmd_led_ex2_led)

typedef struct fwcmd_cmd_led_ex2_state_s {
    uint16_t dwelltime;     /* Specifies how long to run this state, in ms */
    uint16_t reserved;
    uint8_t  active;        /* When is this state in use?                  */
    uint8_t  startbrightness;/* Start Brightness of the state (PWM_BRIGHT)  */
    int8_t   step;          /* Signed incremental step for startbrightness */
    uint8_t  steptime;      /* How often (in ms) to step startbrightness   */
    fwcmd_cmd_led_ex2_led_t  led[VCSFW_LED_EX2_LEDS];
                            /* Settings for the three LEDs in this state   */
} fwcmd_cmd_led_ex2_state_t;

FWCMD_ELCHECK(cmd_led_ex2_state, dwelltime)
FWCMD_ELCHECK(cmd_led_ex2_state, reserved)
FWCMD_ELCHECK(cmd_led_ex2_state, active)
FWCMD_ELCHECK(cmd_led_ex2_state, startbrightness)
FWCMD_ELCHECK(cmd_led_ex2_state, step)
FWCMD_ELCHECK(cmd_led_ex2_state, steptime)
FWCMD_ELCHECK(cmd_led_ex2_state, led)
FWCMD_OVERALLSIZECHECK(cmd_led_ex2_state)

typedef struct fwcmd_cmd_led_ex2_s
{
    uint32_t     dwelltime; /* How long to run this command (in ms) */
    fwcmd_cmd_led_ex2_state_t state[VCSFW_LED_EX2_STATES];
                            /* Description of each active state */
} fwcmd_cmd_led_ex2_t;

FWCMD_ELCHECK(cmd_led_ex2, dwelltime)
FWCMD_ELCHECK(cmd_led_ex2, state)
FWCMD_OVERALLSIZECHECK(cmd_led_ex2)

/****************************************************************************/
/* VCSFW_CMD_SPI_FLASH (58 = 0x3A)                                          */
/****************************************************************************/

typedef struct fwcmd_cmd_spi_flash_s
{
    uint32_t addr;        /* External SPI flash address.                 */
    uint32_t read_length; /* Read length from SPI flash (FLASH to EP2).  */
    uint8_t  dummy_bytes; /* No of Dummy Bytes in the SPI flash command. */
    uint8_t  cmdid;       /* external SPI flash instruction              */
} fwcmd_cmd_spi_flash_t;

FWCMD_ELCHECK(cmd_spi_flash, addr)
FWCMD_ELCHECK(cmd_spi_flash, read_length)
FWCMD_ELCHECK(cmd_spi_flash, dummy_bytes)
FWCMD_ELCHECK(cmd_spi_flash, cmdid)
#if defined(FWCMD_FAILTEST)
/* Assertion fails because native structure adds end padding.            */
FWCMD_OVERALLSIZECHECK(cmd_spi_flash)
#endif      /* defined(FWCMD_FAILTEST) */

/****************************************************************************/
/* VCSFW_CMD_STORAGE_INFO_GET (62 = 0x3E)                                   */
/****************************************************************************/
typedef struct fwcmd_storage_part_entry_s {
    uint8_t  id;            /* Partition ID */
    uint8_t  type;          /* Partition Type */
    uint16_t attrb;         /* Partition attributes */
    uint32_t offset;        /* Beginning offset of the partition in bytes */
    uint32_t size;          /* Size of the partition in bytes */
} fwcmd_storage_part_entry_t;

FWCMD_ELCHECK(storage_part_entry, id)
FWCMD_ELCHECK(storage_part_entry, type)
FWCMD_ELCHECK(storage_part_entry, attrb)
FWCMD_ELCHECK(storage_part_entry, offset)
FWCMD_ELCHECK(storage_part_entry, size)
FWCMD_OVERALLSIZECHECK(storage_part_entry)

typedef struct fwcmd_reply_storage_info_get_s
{
    uint16_t mfgid;         /* Manufacture ID of the Flash */
    uint16_t deviceid;      /* Device ID of the Flash */
    uint16_t blocksize;     /* Block size in bytes */
    uint16_t sectorsize;    /* Erase sector size in blocks */
    uint16_t capacity;      /* Capacity of the flash in blocks */
    uint16_t reservedsize;  /* Reserved space in blocks, including the
                               sensor security context and partition table */
    uint16_t partitions;    /* Number of partitions */
} fwcmd_reply_storage_info_get_t;

FWCMD_ELCHECK(reply_storage_info_get, mfgid)
FWCMD_ELCHECK(reply_storage_info_get, deviceid)
FWCMD_ELCHECK(reply_storage_info_get, blocksize)
FWCMD_ELCHECK(reply_storage_info_get, capacity)
FWCMD_ELCHECK(reply_storage_info_get, reservedsize)
FWCMD_ELCHECK(reply_storage_info_get, partitions)
FWCMD_OVERALLSIZECHECK(reply_storage_info_get)

/****************************************************************************/
/* VCSFW_CMD_STORAGE_PART_FORMAT (63 = 0x3F)                                */
/****************************************************************************/
typedef struct fwcmd_cmd_storage_part_format_s
{
    uint8_t partid;         /* Partition ID of the partition to format */
} fwcmd_cmd_storage_part_format_t;

FWCMD_ELCHECK(cmd_storage_part_format, partid)
FWCMD_OVERALLSIZECHECK(cmd_storage_part_format)

/****************************************************************************/
/* VCSFW_CMD_STORAGE_PART_READ (64 = 0x40)                                  */
/****************************************************************************/
typedef struct fwcmd_cmd_storage_part_read_s
{
    uint8_t  partid;        /* Partition ID of the partition to format */
    uint8_t  mode;          /* Read mode: 0 - Default, 1 - Raw */
    uint16_t reserved;      /* Unused */
    uint32_t offset;        /* Offset from the beginning of the partition */
    uint32_t size;          /* Number of bytes to read */
} fwcmd_cmd_storage_part_read_t;

FWCMD_ELCHECK(cmd_storage_part_read, partid)
FWCMD_ELCHECK(cmd_storage_part_read, mode)
FWCMD_ELCHECK(cmd_storage_part_read, reserved)
FWCMD_ELCHECK(cmd_storage_part_read, offset)
FWCMD_ELCHECK(cmd_storage_part_read, size)
FWCMD_OVERALLSIZECHECK(cmd_storage_part_read)

#if defined(FWCMD_FAILTEST)
typedef struct fwcmd_reply_storage_part_read_s
{
    uint32_t datasize;      /* Number of bytes returned */
    uint16_t reserved;      /* reserved bytes for alignment */
} fwcmd_reply_storage_part_read_t;

FWCMD_ELCHECK(reply_storage_part_read, datasize)
FWCMD_ELCHECK(reply_storage_part_read, reserved)
FWCMD_OVERALLSIZECHECK(reply_storage_part_read)
#endif      /* defined(FWCMD_FAILTEST) */

/****************************************************************************/
/* VCSFW_CMD_STORAGE_PART_WRITE (65 = 0x41)                                 */
/****************************************************************************/
typedef struct fwcmd_cmd_storage_part_write_s
{
    uint8_t  partid;        /* Partition ID of the partition to format */
    uint8_t  mode;          /* Read mode: 0 - Default, 1 - Raw */
    uint16_t reserved;      /* Unused */
    uint32_t offset;        /* Offset from the beginning of the partition */
    uint32_t size;          /* Number of bytes to write */
} fwcmd_cmd_storage_part_write_t;

FWCMD_ELCHECK(cmd_storage_part_write, partid)
FWCMD_ELCHECK(cmd_storage_part_write, mode)
FWCMD_ELCHECK(cmd_storage_part_write, reserved)
FWCMD_ELCHECK(cmd_storage_part_write, offset)
FWCMD_ELCHECK(cmd_storage_part_write, size)
FWCMD_OVERALLSIZECHECK(cmd_storage_part_write)

typedef struct fwcmd_reply_storage_part_write_s
{
    uint32_t datasize;       /* Number of bytes written */
} fwcmd_reply_storage_part_write_t;

FWCMD_ELCHECK(reply_storage_part_write, datasize)
FWCMD_OVERALLSIZECHECK(reply_storage_part_write)

/****************************************************************************/
/* VCSFW_CMD_STORAGE_FWPART_VALIDATE (66 = 0x42)                            */
/****************************************************************************/
typedef struct fwcmd_cmd_storage_fwpart_validate_s
{
    uint8_t  partid;        /* Partition ID of the FW Extension partition */
    uint8_t  signtype;      /* Type of signature */
    uint16_t signsize;      /* Size of the signature */
} fwcmd_cmd_storage_fwpart_validate_t;

FWCMD_ELCHECK(cmd_storage_fwpart_validate, partid)
FWCMD_ELCHECK(cmd_storage_fwpart_validate, signtype)
FWCMD_ELCHECK(cmd_storage_fwpart_validate, signsize)
FWCMD_OVERALLSIZECHECK(cmd_storage_fwpart_validate)

/****************************************************************************/
/* VCSFW_CMD_STORAGE_FWPART_INFO_GET  (67 = 0x43)                           */
/****************************************************************************/
typedef struct fwcmd_cmd_storage_fwpart_info_get_s
{
    uint8_t partid;         /* Partition ID of the FW Extension partition */
} fwcmd_cmd_storage_fwpart_info_get_t;

FWCMD_ELCHECK(cmd_storage_fwpart_info_get, partid)
FWCMD_OVERALLSIZECHECK(cmd_storage_fwpart_info_get)

typedef struct fwcmd_storage_fwpart_module_s
{
    uint16_t type;           /* Type of module */
    uint16_t subtype;        /* Subtype of module
                                For static and dynamic FW patch - patch phase
                                For FW extension - type of extension*/
    uint16_t majorver;       /* Major version of the FW module */
    uint16_t minorver;       /* Minor version of the FW module */
    uint32_t size;           /* Size of the module */
}fwcmd_storage_fwpart_module_t;

FWCMD_ELCHECK(storage_fwpart_module, type)
FWCMD_ELCHECK(storage_fwpart_module, subtype)
FWCMD_ELCHECK(storage_fwpart_module, majorver)
FWCMD_ELCHECK(storage_fwpart_module, minorver)
FWCMD_ELCHECK(storage_fwpart_module, size)
FWCMD_OVERALLSIZECHECK(storage_fwpart_module)

#if defined(FWCMD_FAILTEST)
typedef struct fwcmd_reply_storage_fwpart_info_get_s
{
    uint16_t vmajor;        /* Major version of the FW extension */
    uint16_t vminor;        /* Minor version of the FW extension */
    uint16_t modules;       /* Number of FW modules in the module list */
    uint32_t buildtime;     /* Build time of the FW extension in seconds
                               from 1/1/1970 12:00 AM GMT */
} fwcmd_reply_storage_fwpart_info_get_t;

FWCMD_ELCHECK(reply_storage_fwpart_info_get, vmajor)
FWCMD_ELCHECK(reply_storage_fwpart_info_get, vminor)
FWCMD_ELCHECK(reply_storage_fwpart_info_get, modules)
FWCMD_ELCHECK(reply_storage_fwpart_info_get, buildtime)
FWCMD_OVERALLSIZECHECK(reply_storage_fwpart_info_get)
#endif      /* defined(FWCMD_FAILTEST) */

/****************************************************************************/
/* VCSFW_CMD_TLS_HANDSHAKE (68 = 0x44)                                      */
/****************************************************************************/

typedef struct fwcmd_cmd_tls_handshake_s
{
    uint8_t dummy[3];        /* Added for data alignment. TLS record header
                              *  followed to the command structure has 5 byte
                              *  length. These 3 bytes assure alignment of
                              *  TLS handshake message data followed by record
                              *  header */
} fwcmd_cmd_tls_handshake_t;

FWCMD_ELCHECK(cmd_tls_handshake, dummy)
FWCMD_OVERALLSIZECHECK(cmd_tls_handshake)

/* Data, communicated to the sensor as a part of handshake messages. */

/* -------------------------------------------------------------------------- */
/* TLS Message/record headers. */

/* Handshake message header. */
typedef struct fwcmd_tls_handshake_header_s
{
    uint8_t              type;           /* vcsfw_tls_handshake_type_t */
    uint8_t              length[3];      /* handshake message length */
} fwcmd_tls_handshake_header_t;

FWCMD_ELCHECK(tls_handshake_header, type)
FWCMD_ELCHECK(tls_handshake_header, length)
FWCMD_OVERALLSIZECHECK(tls_handshake_header)

/* TLS version */
typedef struct fwcmd_tls_version_s
{
    uint8_t              major;          /* major version */
    uint8_t              minor;          /* minor version */
} fwcmd_tls_version_t;

FWCMD_ELCHECK(tls_version, major)
FWCMD_ELCHECK(tls_version, minor)
FWCMD_OVERALLSIZECHECK(tls_version)

/* Record header. */
typedef struct fwcmd_tls_record_header_s
{
    uint8_t                type;           /* vcsfw_tls_record_type_t */
    fwcmd_tls_version_t  version;        /* protocol version */
    uint8_t                length[2];      /* record length */
} fwcmd_tls_record_header_t;

FWCMD_ELCHECK(tls_record_header, type)
FWCMD_ELCHECK(tls_record_header, version)
FWCMD_ELCHECK(tls_record_header, length)
FWCMD_OVERALLSIZECHECK(tls_record_header)

/* -------------------------------------------------------------------------- */
/* Common for Client and Server messages. */

/* "Finished" message */
typedef struct fwcmd_tls_finished_s
{
    fwcmd_tls_handshake_header_t   hs_head;
    uint8_t                          hash[VCSFW_TLS_FINISHED_HASH_SIZE];
} fwcmd_tls_finished_t;

FWCMD_ELCHECK(tls_finished, hs_head)
FWCMD_ELCHECK(tls_finished, hash)
FWCMD_OVERALLSIZECHECK(tls_finished)

/* "ChangeCipherSpec" message */
typedef struct fwcmd_tls_change_cipher_spec_s
{
    fwcmd_tls_record_header_t    rec_head;
    uint8_t                        action;         /* always 1 */
} fwcmd_tls_change_cipher_spec_t;

FWCMD_ELCHECK(tls_change_cipher_spec, rec_head)
FWCMD_ELCHECK(tls_change_cipher_spec, action)
FWCMD_OVERALLSIZECHECK(tls_change_cipher_spec)

/* The "alert" message */
typedef struct fwcmd_tls_alert_s
{
    uint8_t             level;       /* vcsfw_tls_alert_level_t */
    uint8_t             description; /* vcsfw_tls_alert_description_t*/
} fwcmd_tls_alert_t;

FWCMD_ELCHECK(tls_alert, level)
FWCMD_ELCHECK(tls_alert, description)
FWCMD_OVERALLSIZECHECK(tls_alert)

/* -------------------------------------------------------------------------- */
/* Supported TLS extensions. */

/* Extension header. */
typedef struct fwcmd_tls_ext_header_s
{
    uint16_t ext_type;
    uint16_t ext_len;
} fwcmd_tls_ext_header_t;

FWCMD_ELCHECK(tls_ext_header, ext_type)
FWCMD_ELCHECK(tls_ext_header, ext_len)
FWCMD_OVERALLSIZECHECK(tls_ext_header)

/* Supported Elliptic Curve extension. */
typedef struct fwcmd_tls_ext_ecc_curve_s
{
    fwcmd_tls_ext_header_t header;          /* Extension header. */
    uint16_t               data_len;        /* Length of the data below. */
    uint16_t               iana_id_list[1]; /* IANA or custom ID of the curve
                                            ** or curve family */
} fwcmd_tls_ext_ecc_curve_t;

FWCMD_ELCHECK(tls_ext_ecc_curve, header)
FWCMD_ELCHECK(tls_ext_ecc_curve, data_len)
FWCMD_ELCHECK(tls_ext_ecc_curve, iana_id_list)
FWCMD_OVERALLSIZECHECK(tls_ext_ecc_curve)

/* Supported Point Formats extension. */
typedef struct fwcmd_tls_ext_ecc_point_formats_s
{
    fwcmd_tls_ext_header_t header;             /* Extension header. */
    uint8_t                  data_len;           /* Length of the data below. */
    uint8_t                  format_id_list[1];  /* List of supported format IDs */
} fwcmd_tls_ext_ecc_point_formats_t;

FWCMD_ELCHECK(tls_ext_ecc_point_formats, header)
FWCMD_ELCHECK(tls_ext_ecc_point_formats, data_len)
FWCMD_ELCHECK(tls_ext_ecc_point_formats, format_id_list)
FWCMD_OVERALLSIZECHECK(tls_ext_ecc_point_formats)

/* -------------------------------------------------------------------------- */
/* Custom, shared, public key structures. */

/* AES Encrypted RSA public key. */
typedef struct fwcmd_tls_rsa_pubk_s {
    uint32_t keylen;
    uint8_t  modulus[VCSFW_MAX_RSA_KEY_LENGTH];
    uint8_t  signature[VCSFW_SHA256_HASH_SIZE];
} fwcmd_tls_rsa_pubk_t;

FWCMD_ELCHECK(tls_rsa_pubk, keylen)
FWCMD_ELCHECK(tls_rsa_pubk, modulus)
FWCMD_ELCHECK(tls_rsa_pubk, signature)
FWCMD_OVERALLSIZECHECK(tls_rsa_pubk)

/* AES Encrypted ECC public key. */
typedef struct fwcmd_tls_ecc_pubk_s {
    uint32_t iana_id;
    uint32_t keylen;
    uint8_t  x[VCSFW_TAKEOWN_MAX_ECC_PARAM_LEN];
    uint8_t  y[VCSFW_TAKEOWN_MAX_ECC_PARAM_LEN];
    uint8_t  padding[8];
    uint8_t signature[VCSFW_SHA256_HASH_SIZE];
} fwcmd_tls_ecc_pubk_t;

FWCMD_ELCHECK(tls_ecc_pubk, iana_id)
FWCMD_ELCHECK(tls_ecc_pubk, keylen)
FWCMD_ELCHECK(tls_ecc_pubk, x)
FWCMD_ELCHECK(tls_ecc_pubk, y)
FWCMD_ELCHECK(tls_ecc_pubk, padding)
FWCMD_ELCHECK(tls_ecc_pubk, signature)
FWCMD_OVERALLSIZECHECK(tls_ecc_pubk)

/****************************************************************************/
/* VCSFW_CMD_DB_INFO_GET (69 = 0x45)                                        */
/****************************************************************************/
typedef struct fwcmd_reply_db_info_get_s
{
    uint16_t majorversion; /* Major version of the database */
    uint16_t minorversion; /* Minor version of the database */
    uint32_t attrib;       /* Attribute of the database */
    uint32_t capacity;     /* Capacity of the database in bytes */
    uint32_t usedspace;    /* Space consumed by valid data in bytes */
    uint32_t freespace;    /* Available free space in bytes */
    uint16_t revision;     /* Revision of the database */
                           /* Incremented on every database update */
    uint16_t usercontainers;/* Number of user containers in the database */
} fwcmd_reply_db_info_get_t;

FWCMD_ELCHECK(reply_db_info_get, majorversion)
FWCMD_ELCHECK(reply_db_info_get, minorversion)
FWCMD_ELCHECK(reply_db_info_get, attrib)
FWCMD_ELCHECK(reply_db_info_get, capacity)
FWCMD_ELCHECK(reply_db_info_get, usedspace)
FWCMD_ELCHECK(reply_db_info_get, freespace)
FWCMD_ELCHECK(reply_db_info_get, revision)
FWCMD_ELCHECK(reply_db_info_get, usercontainers)
FWCMD_OVERALLSIZECHECK(reply_db_info_get)

/****************************************************************************/
/* VCSFW_CMD_DB_OBJECT_INFO_GET (70 = 0x46)                                 */
/****************************************************************************/
typedef struct fwcmd_cmd_db_object_info_get_s
{
    uint16_t objectid;     /* Object id of the object to retrieve */
} fwcmd_cmd_db_object_info_get_t;

FWCMD_ELCHECK(cmd_db_object_info_get, objectid)
FWCMD_OVERALLSIZECHECK(cmd_db_object_info_get)

/* structure of object entry */
typedef struct fwcmd_db_object_entry_s {
    uint16_t id;     /* object id */
    uint16_t type;   /* object type */
} fwcmd_db_object_entry_t;

FWCMD_ELCHECK(db_object_entry, id)
FWCMD_ELCHECK(db_object_entry, type)
FWCMD_OVERALLSIZECHECK(db_object_entry)

/* structure of template object data */
typedef struct fwcmd_db_template_object_s {
    uint16_t fingerindex;    /* finger index */
    uint16_t attrib;         /* attribute of the template */
    uint16_t templatesize;   /* size of the template data */
    uint16_t peksize;        /* size of the payload encryption key (PEK) */
} fwcmd_db_template_object_t;

FWCMD_ELCHECK(db_template_object, fingerindex)
FWCMD_ELCHECK(db_template_object, attrib)
FWCMD_ELCHECK(db_template_object, templatesize)
FWCMD_ELCHECK(db_template_object, peksize)
FWCMD_OVERALLSIZECHECK(db_template_object)

/* structure of authentication data object data */
typedef struct fwcmd_db_auth_data_object_s {
    uint16_t datatype;   /* type of authentication data */
    uint16_t datasize;   /* size of the authentication data blob */
} fwcmd_db_auth_data_object_t;

FWCMD_ELCHECK(db_auth_data_object, datatype)
FWCMD_ELCHECK(db_auth_data_object, datasize)
FWCMD_OVERALLSIZECHECK(db_auth_data_object)

/* header of payload data blob inside the auth data. */
typedef struct fwcmd_db_auth_data_payload_header_s {
    uint16_t datatype;    /* type of authentication data */
    uint16_t payloadsize;
    uint16_t flags;       /* Various flags. User to control the use of
                           * the payload. */
    uint8_t  usused[2];   /* For making data 4-byte aligned. */
} fwcmd_db_auth_data_payload_header_t;
/* followed by the payload, padding optionally and encryption IV */

FWCMD_ELCHECK(db_auth_data_payload_header, datatype)
FWCMD_ELCHECK(db_auth_data_payload_header, payloadsize)
FWCMD_ELCHECK(db_auth_data_payload_header, flags)
FWCMD_ELCHECK(db_auth_data_payload_header, usused)
FWCMD_OVERALLSIZECHECK(db_auth_data_payload_header)

/* header of payload data blob inside the auth data. */
typedef struct fwcmd_db_auth_data_payload_header_ex_s {
    uint16_t datatype;    /* type of authentication data */
    uint16_t uuidsize;    /* The UUID size. */
    uint16_t payloadsize; /* The payload size. */
    uint16_t paddingsize; /* Various flags. User to control the use of
                              * the payload. */
} fwcmd_db_auth_data_payload_header_ex_t;
/* Followed by "uuidsize" bytes of UUID data. */
/* Followed by "payloadsize" bytes of payload data. */
/* Followed by "paddingsize" bytes of padding bytes. */

FWCMD_ELCHECK(db_auth_data_payload_header_ex, datatype)
FWCMD_ELCHECK(db_auth_data_payload_header_ex, uuidsize)
FWCMD_ELCHECK(db_auth_data_payload_header_ex, payloadsize)
FWCMD_ELCHECK(db_auth_data_payload_header_ex, paddingsize)
FWCMD_OVERALLSIZECHECK(db_auth_data_payload_header_ex)

/* structure of generic data object data */
typedef struct fwcmd_db_generic_data_object_s {
    uint16_t datatype;   /* type of generic data */
    uint16_t datasize;   /* size of the generic data blob */
} fwcmd_db_generic_data_object_t;

FWCMD_ELCHECK(db_generic_data_object, datatype)
FWCMD_ELCHECK(db_generic_data_object, datasize)
FWCMD_OVERALLSIZECHECK(db_generic_data_object)

typedef struct fwcmd_reply_db_object_info_get_s
{
    uint16_t id;     /* object id */
    uint16_t type;   /* object type */
    uint16_t attrib; /* object attributes */
    uint16_t size;   /* size of the object data in bytes */
    uint32_t nchild; /* number of child object */
} fwcmd_reply_db_object_info_get_t;

FWCMD_ELCHECK(reply_db_object_info_get, id)
FWCMD_ELCHECK(reply_db_object_info_get, type)
FWCMD_ELCHECK(reply_db_object_info_get, attrib)
FWCMD_ELCHECK(reply_db_object_info_get, size)
FWCMD_ELCHECK(reply_db_object_info_get, nchild)
FWCMD_OVERALLSIZECHECK(reply_db_object_info_get)

/****************************************************************************/
/* VCSFW_CMD_DB_OBJECT_CREATE (71 = 0x47)                                   */
/****************************************************************************/
typedef struct fwcmd_cmd_db_object_create_s
{
    uint16_t parentid;   /* Object id of the parent object */
    uint16_t type;       /* object type */
    uint16_t attrib;     /* attribute of the object */
    uint16_t size;       /* size of the object data in bytes */
} fwcmd_cmd_db_object_create_t;

FWCMD_ELCHECK(cmd_db_object_create, parentid)
FWCMD_ELCHECK(cmd_db_object_create, type)
FWCMD_ELCHECK(cmd_db_object_create, attrib)
FWCMD_ELCHECK(cmd_db_object_create, size)
FWCMD_OVERALLSIZECHECK(cmd_db_object_create)

typedef struct fwcmd_reply_db_object_create_s
{
    uint16_t objectid;   /* object id assigned */
} fwcmd_reply_db_object_create_t;

FWCMD_ELCHECK(reply_db_object_create, objectid)
FWCMD_OVERALLSIZECHECK(reply_db_object_create)

/****************************************************************************/
/* VCSFW_CMD_DB_OBJECT_DELETE (72 = 0x48)                                   */
/****************************************************************************/
typedef struct fwcmd_cmd_db_object_delete_s
{
    uint16_t objectid;   /* Object id of the object */
} fwcmd_cmd_db_object_delete_t;

FWCMD_ELCHECK(cmd_db_object_delete, objectid)
FWCMD_OVERALLSIZECHECK(cmd_db_object_delete)

/****************************************************************************/
/* VCSFW_CMD_DB_OBJECT_DATA_GET (73 = 0x49)                                 */
/****************************************************************************/
typedef struct fwcmd_cmd_db_object_data_get_s
{
    uint16_t objectid;   /* Object id of the object */
} fwcmd_cmd_db_object_data_get_t;

FWCMD_ELCHECK(cmd_db_object_data_get, objectid)
FWCMD_OVERALLSIZECHECK(cmd_db_object_data_get)

typedef struct fwcmd_reply_db_object_data_get_s
{
    uint16_t objectid;   /* object id */
    uint16_t type;       /* object type */
    uint16_t attrib;     /* object attributes */
    uint16_t size;       /* size of the object in bytes */
    uint16_t reserved;   /* reserved bytes for alignment */
} fwcmd_reply_db_object_data_get_t;

FWCMD_ELCHECK(reply_db_object_data_get, objectid)
FWCMD_ELCHECK(reply_db_object_data_get, type)
FWCMD_ELCHECK(reply_db_object_data_get, attrib)
FWCMD_ELCHECK(reply_db_object_data_get, size)
FWCMD_ELCHECK(reply_db_object_data_get, reserved)
FWCMD_OVERALLSIZECHECK(reply_db_object_data_get)

/****************************************************************************/
/* VCSFW_CMD_DB_USER_INFO_GET (74 = 0x4A)                                   */
/****************************************************************************/
typedef struct fwcmd_cmd_db_user_info_get_s
{
    uint16_t objectid;   /* Object id of the user object */
    uint16_t usercontid; /* Object id of the user container object */
    uint16_t uuidsize;   /* size of the UUID data follow */
} fwcmd_cmd_db_user_info_get_t;

FWCMD_ELCHECK(cmd_db_user_info_get, objectid)
FWCMD_ELCHECK(cmd_db_user_info_get, usercontid)
FWCMD_ELCHECK(cmd_db_user_info_get, uuidsize)
FWCMD_OVERALLSIZECHECK(cmd_db_user_info_get)

/* structure of template entry */
typedef struct fwcmd_db_template_entry_s {
    uint16_t objectid;    /* object id */
    uint16_t fingerindex; /* finger index */
    uint16_t attrib;      /* attribute */
    uint16_t size;        /* size of the template data */
} fwcmd_db_template_entry_t;

FWCMD_ELCHECK(db_template_entry, objectid)
FWCMD_ELCHECK(db_template_entry, fingerindex)
FWCMD_ELCHECK(db_template_entry, attrib)
FWCMD_ELCHECK(db_template_entry, size)
FWCMD_OVERALLSIZECHECK(db_template_entry)

/* structure of user data entry */
typedef struct fwcmd_db_user_data_entry_s {
    uint16_t objectid;    /* object id */
    uint16_t datatype;    /* type of user data */
    uint16_t size;        /* size of the data blob */
    uint16_t reserved;    /* reserved */
} fwcmd_db_user_data_entry_t;

FWCMD_ELCHECK(db_user_data_entry, objectid)
FWCMD_ELCHECK(db_user_data_entry, datatype)
FWCMD_ELCHECK(db_user_data_entry, size)
FWCMD_ELCHECK(db_user_data_entry, reserved)
FWCMD_OVERALLSIZECHECK(db_user_data_entry)

typedef struct fwcmd_reply_db_user_info_get_s
{
    uint16_t objectid;   /* object id */
    uint16_t templates;  /* number of templates enrolled by the user */
    uint16_t objects;    /* number of data objects owned by the user */
    uint16_t uuidsize;   /* size of UUID in bytes */
} fwcmd_reply_db_user_info_get_t;

FWCMD_ELCHECK(reply_db_user_info_get, objectid)
FWCMD_ELCHECK(reply_db_user_info_get, templates)
FWCMD_ELCHECK(reply_db_user_info_get, objects)
FWCMD_ELCHECK(reply_db_user_info_get, uuidsize)
FWCMD_OVERALLSIZECHECK(reply_db_user_info_get)

/****************************************************************************/
/* VCSFW_CMD_DB_USER_CONTAINER_INFO_GET (76 = 0x4B)                         */
/****************************************************************************/
/* COMMAND                                                                  */
typedef struct fwcmd_cmd_db_user_container_info_get_s
{
    uint16_t objectid;   /* Object id of the user container object */
    uint16_t ucuidsize;  /* size of the UCUID data follow */
} fwcmd_cmd_db_user_container_info_get_t;

FWCMD_ELCHECK(cmd_db_user_container_info_get, objectid)
FWCMD_ELCHECK(cmd_db_user_container_info_get, ucuidsize)
FWCMD_OVERALLSIZECHECK(cmd_db_user_container_info_get)

/* REPLY                                                                    */
typedef struct fwcmd_db_user_entry_s {
    uint16_t objectid;    /* object id */
    uint16_t size;        /* size of the user data */
} fwcmd_db_user_entry_t;

FWCMD_ELCHECK(db_user_entry, objectid)
FWCMD_ELCHECK(db_user_entry, size)
FWCMD_OVERALLSIZECHECK(db_user_entry)

typedef struct fwcmd_reply__db_user_container_info_get_s
{
    uint16_t objectid;   /* object id */
    uint16_t users;      /* number of users */
    uint16_t ucuidsize; /* size of UCUID in bytes */
    uint16_t reserved;   /* for data alignment */
} fwcmd_reply_db_user_container_info_get_t;

FWCMD_ELCHECK(reply_db_user_container_info_get, objectid)
FWCMD_ELCHECK(reply_db_user_container_info_get, users)
FWCMD_ELCHECK(reply_db_user_container_info_get, ucuidsize)
FWCMD_OVERALLSIZECHECK(reply_db_user_container_info_get)

/****************************************************************************/
/* VCSFW_CMD_DB_OBJECT_RESTORE (76 = 0x4C)                                  */
/****************************************************************************/
typedef struct fwcmd_cmd_db_object_restore_s
{
    uint32_t size;       /* size of data to restore */
    uint16_t parentid;   /* Object id of the parent object */
    uint16_t reserved;   /* reserved */
} fwcmd_cmd_db_object_restore_t;

FWCMD_ELCHECK(cmd_db_object_restore, size)
FWCMD_ELCHECK(cmd_db_object_restore, parentid)
FWCMD_ELCHECK(cmd_db_object_restore, reserved)
FWCMD_OVERALLSIZECHECK(cmd_db_object_restore)

typedef struct fwcmd_reply_db_object_restore_s
{
    uint16_t objectid;   /* object id assigned */
} fwcmd_reply_db_object_restore_t;

FWCMD_ELCHECK(reply_db_object_restore, objectid)
FWCMD_OVERALLSIZECHECK(reply_db_object_restore)

/****************************************************************************/
/* VCSFW_CMD_DB_SYSTEM_PARAM_GET (77 = 0x4D)                                */
/****************************************************************************/
typedef struct fwcmd_cmd_db_system_param_get_s
{
    uint16_t namesize;   /* size of the parameter name in bytes */
} fwcmd_cmd_db_system_param_get_t;

FWCMD_ELCHECK(cmd_db_system_param_get, namesize)
FWCMD_OVERALLSIZECHECK(cmd_db_system_param_get)

typedef struct fwcmd_reply_db_system_param_get_s
{
    uint16_t datasize;   /* size of the parameter value in bytes */
} fwcmd_reply_db_system_param_get_t;

FWCMD_ELCHECK(reply_db_system_param_get, datasize)
FWCMD_OVERALLSIZECHECK(reply_db_system_param_get)

/****************************************************************************/
/* VCSFW_CMD_DB_SYSTEM_PARAM_SET (78 = 0x4E)                                */
/****************************************************************************/
typedef struct fwcmd_cmd_db_system_param_set_s
{
    uint16_t namesize;   /* size of the parameter name in bytes */
    uint16_t valuesize;  /* size of the parameter value in bytes */
} fwcmd_cmd_db_system_param_set_t;

FWCMD_ELCHECK(cmd_db_system_param_set, namesize)
FWCMD_ELCHECK(cmd_db_system_param_set, valuesize)
FWCMD_OVERALLSIZECHECK(cmd_db_system_param_set)

/****************************************************************************/
/* VCSFW_CMD_TAKE_OWNERSHIP_EX2 (79 = 0x4f)                                 */
/****************************************************************************/
/* COMMAND                                                                  */

/*
 * The command consists of a set of sections in the form
 *  {tag, length, data} blobs.
 */
typedef struct fwcmd_cmd_takeownex2_section_s {
    uint16_t     tag;
    uint16_t     length;
} fwcmd_cmd_takeownex2_section_t;

FWCMD_ELCHECK(cmd_takeownex2_section, tag)
FWCMD_ELCHECK(cmd_takeownex2_section, length)
FWCMD_OVERALLSIZECHECK(cmd_takeownex2_section)

/* The command structure. */
typedef struct fwcmd_cmd_take_ownership_ex2_s {
    uint8_t      flags;             /* Flags, options. */
    uint8_t      reserved[3];       /* For data alignment. */
} fwcmd_cmd_take_ownership_ex2_t;

FWCMD_ELCHECK(cmd_take_ownership_ex2, flags)
FWCMD_ELCHECK(cmd_take_ownership_ex2, reserved)
FWCMD_OVERALLSIZECHECK(cmd_take_ownership_ex2)

/* VCSFW_TAKEOWN_TAG_FLASHINFO: General flash information. */
typedef struct fwcmd_takeown_data_flashinfo_s {
    uint32_t     flash_size;
    uint32_t     erasable_unit_size;
    uint16_t     dummy;
    uint8_t      erase_cmd_id;
    uint8_t      read_mode;
} fwcmd_takeown_data_flashinfo_t;

FWCMD_ELCHECK(takeown_data_flashinfo, flash_size)
FWCMD_ELCHECK(takeown_data_flashinfo, erasable_unit_size)
FWCMD_ELCHECK(takeown_data_flashinfo, dummy)
FWCMD_ELCHECK(takeown_data_flashinfo, erase_cmd_id)
FWCMD_ELCHECK(takeown_data_flashinfo, read_mode)
FWCMD_OVERALLSIZECHECK(takeown_data_flashinfo)

/* VCSFW_TAKEOWN_TAG_CI_SYMK: Customer-Injected Key / Seed */
typedef struct fwcmd_takeown_data_symk_s {
    uint8_t      cios[VCSFW_SHA256_HASH_SIZE];
    uint8_t      cik[VCSFW_SHA256_HASH_SIZE];
} fwcmd_takeown_data_symk_t;

FWCMD_ELCHECK(takeown_data_symk, cios)
FWCMD_ELCHECK(takeown_data_symk, cik)
FWCMD_OVERALLSIZECHECK(takeown_data_symk)

/* VCSFW_TAKEOWN_TAG_EC: Elliptic Curve parameters. */
typedef struct fwcmd_takeown_data_ec_s {
    uint32_t     iana_id;               /* Curve IANA ID */
    uint32_t     strength;              /* Strength in bits */
    uint32_t     h;                     /* Cofactor */
    uint8_t      a[VCSFW_TAKEOWN_MAX_ECC_PARAM_LEN]; /* Coefficient 'a' */
    uint8_t      b[VCSFW_TAKEOWN_MAX_ECC_PARAM_LEN]; /* Coefficient 'b' */
    uint8_t      g_x[VCSFW_TAKEOWN_MAX_ECC_PARAM_LEN]; /* X coordinate of BP */
    uint8_t      g_y[VCSFW_TAKEOWN_MAX_ECC_PARAM_LEN]; /* Y coordinate of BP */
    uint8_t      n[VCSFW_TAKEOWN_MAX_ECC_PARAM_LEN];   /* Order */
    uint8_t      p[VCSFW_TAKEOWN_MAX_ECC_PARAM_LEN];   /* Prime 'P' */
} fwcmd_takeown_data_ec_t;

FWCMD_ELCHECK(takeown_data_ec, iana_id)
FWCMD_ELCHECK(takeown_data_ec, strength)
FWCMD_ELCHECK(takeown_data_ec, h)
FWCMD_ELCHECK(takeown_data_ec, a)
FWCMD_ELCHECK(takeown_data_ec, b)
FWCMD_ELCHECK(takeown_data_ec, g_x)
FWCMD_ELCHECK(takeown_data_ec, g_y)
FWCMD_ELCHECK(takeown_data_ec, n)
FWCMD_ELCHECK(takeown_data_ec, p)
FWCMD_OVERALLSIZECHECK(takeown_data_ec)

typedef fwcmd_tls_rsa_pubk_t fwcmd_takeown_data_pubk_rsa_t;

typedef fwcmd_tls_ecc_pubk_t fwcmd_takeown_data_pubk_ecc_t;

/* ECC Signature block coming after. It can hold an RSA or ECC signature. */
typedef struct fwcmd_takeown_data_pubk_signature_s {
    uint32_t sign_len;
    uint8_t  signature[VCSFW_MAX_RSA_KEY_LENGTH];
} fwcmd_takeown_data_pubk_signature_t;
FWCMD_ELCHECK(takeown_data_pubk_signature, sign_len)
FWCMD_ELCHECK(takeown_data_pubk_signature, signature)
FWCMD_OVERALLSIZECHECK(takeown_data_pubk_signature)

/* VCSFW_TAKEOWN_TAG_CI_PK: PrivK/SPubK - RSA */
typedef struct fwcmd_takeown_data_ci_pk_rsa_s {
    fwcmd_takeown_data_pubk_rsa_t pubk;
    uint8_t      priv_exp[VCSFW_MAX_RSA_KEY_LENGTH];
} fwcmd_takeown_data_ci_pk_rsa_t;

FWCMD_ELCHECK(takeown_data_ci_pk_rsa, pubk)
FWCMD_ELCHECK(takeown_data_ci_pk_rsa, priv_exp)
FWCMD_OVERALLSIZECHECK(takeown_data_ci_pk_rsa)

/* SEPrivK/SEPubK - ECC */
typedef struct fwcmd_takeown_data_ci_pk_ecc_s {
    fwcmd_takeown_data_pubk_ecc_t pubk;
    uint8_t      privk[VCSFW_TAKEOWN_MAX_ECC_PARAM_LEN];
} fwcmd_takeown_data_ci_pk_ecc_t;

FWCMD_ELCHECK(takeown_data_ci_pk_ecc, pubk)
FWCMD_ELCHECK(takeown_data_ci_pk_ecc, privk)
FWCMD_OVERALLSIZECHECK(takeown_data_ci_pk_ecc)

/* REPLY                                                                    */
typedef struct fwcmd_reply_take_ownership_ex2_s {
    uint32_t    length;
    union {
        fwcmd_takeown_data_pubk_rsa_t  rsa_pk; /* 256 bytes - 2048 bit key */
        fwcmd_takeown_data_pubk_ecc_t  ecc_pk; /* 140 bytes - 521  bit key  */
    } hapubk_signed;  /* Encrypted with SSEK */
} fwcmd_reply_take_ownership_ex2_t;

FWCMD_ELCHECK(reply_take_ownership_ex2, length)
FWCMD_ELCHECK(reply_take_ownership_ex2, hapubk_signed)
FWCMD_OVERALLSIZECHECK(reply_take_ownership_ex2)

/****************************************************************************/
/* VCSFW_CMD_GET_CERTIFICATE_EX (80 = 0x50)                                 */
/****************************************************************************/


typedef struct fwcmd_rsa_certificate_s {
    uint32_t         keylen;                            /* In bytes */
    uint32_t         pub_exp;
    uint8_t          modulus[VCSFW_MAX_RSA_KEY_LENGTH]; /* Public key */
    uint8_t          sign[VCSFW_MAX_RSA_KEY_LENGTH];    /* RSA sign */
} fwcmd_rsa_certificate_t;

FWCMD_ELCHECK(rsa_certificate, keylen)
FWCMD_ELCHECK(rsa_certificate, pub_exp)
FWCMD_ELCHECK(rsa_certificate, modulus)
FWCMD_ELCHECK(rsa_certificate, sign)
FWCMD_OVERALLSIZECHECK(rsa_certificate)

typedef struct fwcmd_ecc_certificate_s {
    uint32_t         keylen;                                /* In bytes.  */
    uint32_t         curve_iana_id;                         /* Curve ID   */
    uint8_t          x[VCSFW_TAKEOWN_MAX_ECC_PARAM_LEN];    /* Public key */
    uint8_t          y[VCSFW_TAKEOWN_MAX_ECC_PARAM_LEN];
    uint8_t          sign[VCSFW_MAX_RSA_KEY_LENGTH];        /* RSA sign */
} fwcmd_ecc_certificate_t;

FWCMD_ELCHECK(ecc_certificate, keylen)
FWCMD_ELCHECK(ecc_certificate, curve_iana_id)
FWCMD_ELCHECK(ecc_certificate, x)
FWCMD_ELCHECK(ecc_certificate, y)
FWCMD_ELCHECK(ecc_certificate, sign)
FWCMD_OVERALLSIZECHECK(ecc_certificate)

/* The reply structure. */
typedef struct fwcmd_reply_get_certificate_ex_s {
    uint32_t         reply_length;
    fwcmd_rsa_certificate_t rsa_cert;
    fwcmd_ecc_certificate_t ecc_cert;
} fwcmd_reply_get_certificate_ex_t;

FWCMD_ELCHECK(reply_get_certificate_ex, reply_length)
FWCMD_ELCHECK(reply_get_certificate_ex, rsa_cert)
FWCMD_ELCHECK(reply_get_certificate_ex, ecc_cert)
FWCMD_OVERALLSIZECHECK(reply_get_certificate_ex)

/****************************************************************************/
/* VCSFW_CMD_READ_FPDATA (81 = 0x51)                                        */
/****************************************************************************/

/* header of the fpdata returned */
typedef struct fwcmd_fpdata_header_s
{
    uint16_t    rows;       /* number of rows in the fingerprint */
    uint16_t    cols;       /* number of columns in the fingerprint */
    uint16_t    dpi;        /* resolution in dpi */
    uint8_t     bitdepth;   /* bits per pixel */
    uint8_t     reserved;
    uint32_t    quality;    /* quality flags */
} fwcmd_fpdata_header_t;

FWCMD_ELCHECK(fpdata_header, rows)
FWCMD_ELCHECK(fpdata_header, cols)
FWCMD_ELCHECK(fpdata_header, dpi)
FWCMD_ELCHECK(fpdata_header, bitdepth)
FWCMD_ELCHECK(fpdata_header, reserved)
FWCMD_ELCHECK(fpdata_header, quality)
FWCMD_OVERALLSIZECHECK(fpdata_header)

typedef struct fwcmd_cmd_read_fpdata_s
{
    uint32_t    bufsize;    /* host buffer size */
} fwcmd_cmd_read_fpdata_t;

FWCMD_ELCHECK(cmd_read_fpdata, bufsize)
FWCMD_OVERALLSIZECHECK(cmd_read_fpdata)

typedef struct fwcmd_reply_read_fpdata_s
{
    uint32_t    datasize;   /* amount of data returned */
} fwcmd_reply_read_fpdata_t;

FWCMD_ELCHECK(reply_read_fpdata, datasize)
FWCMD_OVERALLSIZECHECK(reply_read_fpdata)


/****************************************************************************/
/* VCSFW_CMD_GET_PATCHINFO (91 = 0x5b)                                      */
/****************************************************************************/

/*
 * The reply consists of the standard 16-bit status code followed by
 *  an array of 0 or more of the following structures.
 */

typedef struct fwcmd_reply_get_patchinfo_item_s
{
    uint32_t          basep;
    uint32_t          size;
    uint32_t          flags;
    uint32_t          buildtime;
    uint32_t          sig;        /* signature */
    uint16_t          buildnum;
    uint8_t           majorver;
    uint8_t           minorver;
} fwcmd_reply_get_patchinfo_item_t;


FWCMD_ELCHECK(reply_get_patchinfo_item, basep)
FWCMD_ELCHECK(reply_get_patchinfo_item, size)
FWCMD_ELCHECK(reply_get_patchinfo_item, flags)
FWCMD_ELCHECK(reply_get_patchinfo_item, buildtime)
FWCMD_ELCHECK(reply_get_patchinfo_item, sig)
FWCMD_ELCHECK(reply_get_patchinfo_item, buildnum)
FWCMD_ELCHECK(reply_get_patchinfo_item, majorver)
FWCMD_ELCHECK(reply_get_patchinfo_item, minorver)
FWCMD_OVERALLSIZECHECK(reply_get_patchinfo_item)

/****************************************************************************/
/* VCSFW_CMD_GET_NAV_STATUS (93 = 0x5d)                                     */
/****************************************************************************/

#if defined(FWCMD_FAILTEST)
/* This structure is not aligned.  */
typedef struct fwcmd_reply_get_nav_status_s
{
    uint8_t      status;
    int16_t      delta_x;
    int16_t      delta_y;
    uint16_t     timestamp;
} fwcmd_reply_get_nav_status_t;

FWCMD_ELCHECK(reply_get_nav_status, status)
FWCMD_ELCHECK(reply_get_nav_status, delta_x)
FWCMD_ELCHECK(reply_get_nav_status, delta_y)
FWCMD_ELCHECK(reply_get_nav_status, timestamp)
FWCMD_OVERALLSIZECHECK(reply_get_nav_status)
#endif

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* VCSFW_CMD_GET_PRINT parameters                                             */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

/*
 * Parameters.  Note that the host must guarantee 32-bit alignment
 *  *relative to the beginning of fwcmd_cmd_get_print_t* (not the
 *  initial byte = VCSFW_CMD_GET_PRINT) for every parameter block.
 *
 * Note that these are located here at the end of the file because
 *  they make several back references (e.g. fwcmd_cmd_woe_setup_t).
 */
typedef struct fwcmd_param_s
{
    uint16_t parameter;     /* host guarantees alignment at n*4bytes
                             * (from &(fwcmd_cmd_get_print_t::linelen)) */
    uint16_t length;        /* host guarantees = n*4 */
    /* Followed by length bytes of data */
} fwcmd_param_t;

FWCMD_ELCHECK(param, parameter)
FWCMD_ELCHECK(param, length)
FWCMD_OVERALLSIZECHECK(param)


/* VCSFW_CMD_GET_PRINT/VCSFW_PARAM_NOOP (0 = 0x00) */
/* VCSFW_CMD_GET_PRINT/VCSFW_PARAM_TSCONFIG (2 = 0x02) */
typedef struct fwcmd_param_tscfg_s
{
    uint32_t SCM_TSCFG[352];
} fwcmd_param_tscfg_t;

FWCMD_ELCHECK(param_tscfg, SCM_TSCFG)
FWCMD_OVERALLSIZECHECK(param_tscfg)
FWCMD_PARAMSIZEX4(param_tscfg)

/* VCSFW_CMD_GET_PRINT/VCSFW_PARAM_REGISTER (3 = 0x03) */
/* Uses vcsfw_cmd_poke_t */
FWCMD_PARAMSIZEX4(cmd_poke)

/* VCSFW_CMD_GET_PRINT/VCSFW_PARAM_WOE (7 = 0x07) */
/* forward reference: */
typedef struct fwcmd_param_woe_s {
    uint32_t             trigev;
    fwcmd_cmd_woe_setup_t   woe_setup;
} fwcmd_param_woe_t;

FWCMD_ELCHECK(param_woe, trigev)
FWCMD_ELCHECK(param_woe, woe_setup)
FWCMD_OVERALLSIZECHECK(param_woe)
FWCMD_PARAMSIZEX4(param_woe)

/* VCSFW_CMD_GET_PRINT/VCSFW_UNKNOWN (9 = 0x09) */
/* VCSFW_CMD_GET_PRINT/VCSFW_PARAM_CPUCLK (10 = 0x0a) */
typedef struct fwcmd_param_cpuclk_s
{
    uint32_t     run_cpuclkctrl;
    uint32_t     wait_cpuclkctrl;
} fwcmd_param_cpuclk_t;

FWCMD_ELCHECK(param_cpuclk, run_cpuclkctrl)
FWCMD_ELCHECK(param_cpuclk, wait_cpuclkctrl)
FWCMD_OVERALLSIZECHECK(param_cpuclk)
FWCMD_PARAMSIZEX4(param_cpuclk)

/* Define a generic list structure */
typedef struct fwcmd_param_reglist_s
{
    uint32_t addr;
    uint32_t value;
    uint32_t mask;
} fwcmd_param_reglist_t;

FWCMD_ELCHECK(param_reglist, addr)
FWCMD_ELCHECK(param_reglist, value)
FWCMD_ELCHECK(param_reglist, mask)

/* VCSFW_CMD_GET_PRINT/VCSFW_PARAM_SWEEP (13 = 0x0d) */
typedef struct fwcmd_param_tworegsweep_s
{
    uint32_t reg1;
    uint32_t initial;
    uint32_t step;
    uint32_t stop;
    uint32_t mask;
    uint32_t iter;
    uint32_t reg2;
    uint32_t offset;
} fwcmd_param_tworegsweep_t;

FWCMD_ELCHECK(param_tworegsweep, reg1)
FWCMD_ELCHECK(param_tworegsweep, initial)
FWCMD_ELCHECK(param_tworegsweep, step)
FWCMD_ELCHECK(param_tworegsweep, stop)
FWCMD_ELCHECK(param_tworegsweep, mask)
FWCMD_ELCHECK(param_tworegsweep, iter)
FWCMD_ELCHECK(param_tworegsweep, reg2)
FWCMD_ELCHECK(param_tworegsweep, offset)
FWCMD_OVERALLSIZECHECK(param_tworegsweep)
FWCMD_PARAMSIZEX4(param_tworegsweep)

typedef struct fwcmd_param_tworangesweep_s
{
    uint32_t reg;
    uint32_t initial1;
    uint32_t step1;
    uint32_t stop1;
    uint32_t mask;
    uint32_t iter;

    uint32_t initial2;
    uint32_t step2;
    uint32_t stop2;
} fwcmd_param_tworangesweep_t;

FWCMD_ELCHECK(param_tworangesweep, reg)
FWCMD_ELCHECK(param_tworangesweep, initial1)
FWCMD_ELCHECK(param_tworangesweep, step1)
FWCMD_ELCHECK(param_tworangesweep, stop1)
FWCMD_ELCHECK(param_tworangesweep, mask)
FWCMD_ELCHECK(param_tworangesweep, iter)
FWCMD_ELCHECK(param_tworangesweep, initial2)
FWCMD_ELCHECK(param_tworangesweep, step2)
FWCMD_ELCHECK(param_tworangesweep, stop2)
FWCMD_OVERALLSIZECHECK(param_tworangesweep)
FWCMD_PARAMSIZEX4(param_tworangesweep)

typedef struct fwcmd_param_listsweep_s
{
    uint32_t size;
    uint32_t numvals;
    uint32_t regs;
} fwcmd_param_listsweep_t;

FWCMD_ELCHECK(param_listsweep, size)
FWCMD_ELCHECK(param_listsweep, numvals)
FWCMD_ELCHECK(param_listsweep, regs)
FWCMD_OVERALLSIZECHECK(param_listsweep)
FWCMD_PARAMSIZEX4(param_listsweep)

typedef struct fwcmd_param_listswap_s
{
    uint32_t size;
    uint32_t storeaddr;
    uint32_t numvals;
} fwcmd_param_listswap_t;

FWCMD_ELCHECK(param_listswap, size)
FWCMD_ELCHECK(param_listswap, storeaddr)
FWCMD_ELCHECK(param_listswap, numvals)
FWCMD_OVERALLSIZECHECK(param_listswap)
FWCMD_PARAMSIZEX4(param_listswap)

typedef struct fwcmd_param_sweep_s
{
    union {
        fwcmd_param_tworegsweep_t tworegs;
        fwcmd_param_tworangesweep_t tworanges;
        fwcmd_param_listsweep_t listsweep;
        fwcmd_param_listswap_t listswap;
    } data;
    uint32_t type;
} fwcmd_param_sweep_t;

FWCMD_ELCHECK(param_sweep, data)
FWCMD_ELCHECK(param_sweep, type)
FWCMD_OVERALLSIZECHECK(param_sweep)
FWCMD_PARAMSIZEX4(param_sweep)

/* VCSFW_CMD_GET_PRINT/VCSFW_PARAM_BW_REDUCTION (21 = 0x15) */
typedef struct fwcmd_param_bw_reduction_s
{
    uint32_t cull_flags;
    uint32_t cull_control;
} fwcmd_param_bw_reduction_t;

FWCMD_ELCHECK(param_bw_reduction, cull_flags)
FWCMD_ELCHECK(param_bw_reduction, cull_control)
FWCMD_OVERALLSIZECHECK(param_bw_reduction)
FWCMD_PARAMSIZEX4(param_bw_reduction)

/* VCSFW_CMD_GET_PRINT/VCSFW_PARAM_AGC (22 = 0x16) */
typedef struct fwcmd_param_agc_s
{
    uint32_t flags;
    uint32_t control;
    uint32_t Z0_thresh;
    uint32_t Z0_count;
    uint32_t Z1_thresh;
    uint32_t Z1_count;
    uint8_t Z0_finish_thresh;
    uint8_t Z1_finish_thresh;
    uint16_t finish_debounce;
} fwcmd_param_agc_t;

FWCMD_ELCHECK(param_agc, flags)
FWCMD_ELCHECK(param_agc, control)
FWCMD_ELCHECK(param_agc, Z0_thresh)
FWCMD_ELCHECK(param_agc, Z0_count)
FWCMD_ELCHECK(param_agc, Z1_thresh)
FWCMD_ELCHECK(param_agc, Z1_count)
FWCMD_ELCHECK(param_agc, Z0_finish_thresh)
FWCMD_ELCHECK(param_agc, Z1_finish_thresh)
FWCMD_ELCHECK(param_agc, finish_debounce)
FWCMD_OVERALLSIZECHECK(param_agc)
FWCMD_PARAMSIZEX4(param_agc)

/* VCSFW_CMD_GET_PRINT/VCSFW_PARAM_CONFIGREPLY (23 = 0x17) */

/* VCSFW_CMD_GET_PRINT/VCSFW_PARAM_WOVAR (25 = 0x19) */
/* This is part of the WOVAR patch, and is not used in the base firmware */

/* VCSFW_CMD_GET_PRINT/VCSFW_PARAM_BIT_REDUCTION (27 = 0x1b) */
typedef struct fwcmd_param_bit_reduction_s
{
    uint16_t ts_sense_start;
    uint16_t ts_sense_end;
    uint16_t mts_count;
    uint16_t max_image_count;
    uint8_t max_nbits;
    uint8_t max_bufs;
    uint16_t reserved;
} fwcmd_param_bit_reduction_t;

FWCMD_ELCHECK(param_bit_reduction, ts_sense_start)
FWCMD_ELCHECK(param_bit_reduction, ts_sense_end)
FWCMD_ELCHECK(param_bit_reduction, mts_count)
FWCMD_ELCHECK(param_bit_reduction, max_image_count)
FWCMD_ELCHECK(param_bit_reduction, max_nbits)
FWCMD_ELCHECK(param_bit_reduction, max_bufs)
FWCMD_ELCHECK(param_bit_reduction, reserved)
FWCMD_OVERALLSIZECHECK(param_bit_reduction)
FWCMD_PARAMSIZEX4(param_bit_reduction)


/* VCSFW_CMD_GET_PRINT/VCSFW_PARAM_KEEP_XLTRACE (0x8000) */
/* VCSFW_CMD_GET_PRINT/VCSFW_PARAM_TIMESLOTRANGES (0x8003) */

/* VCSFW_PARAM_CAL -- Calibration Setup
   Aligned to 4-byte boundary, for use beginning with Eagle */
typedef struct fwcmd_param_cal_s
{
    uint16_t   linespersweep;
    uint8_t    block;
    uint8_t    linespersweepiter;
} fwcmd_param_cal_t;

FWCMD_ELCHECK(param_cal, linespersweep)
FWCMD_ELCHECK(param_cal, block)
FWCMD_ELCHECK(param_cal, linespersweepiter)
FWCMD_OVERALLSIZECHECK(param_cal)
#if defined(FWCMD_FAILTEST)
FWCMD_PARAMSIZEX4(param_cal)
#endif      /* defined(FWCMD_FAILTEST) */

/* VCSFW_PARAM_ZONECFGA -- Nested register sweep parameters
   Aligned to 4-byte boundary, for use beginning with Eagle
   NOTE: Followed by (totalzones * sizeof(fwcmd_param_reglist_t)) bytes of
   address/value/and_mask sets */
typedef struct fwcmd_param_zone_cfga_s
{
    uint8_t  totalzones;     // Number of zones to plug
    uint8_t  zonespersweep;  // Zones to use for each loop
    uint8_t  reserved[2];    // Reserved
} fwcmd_param_zone_cfga_t;

FWCMD_ELCHECK(param_zone_cfga, totalzones)
FWCMD_ELCHECK(param_zone_cfga, zonespersweep)
FWCMD_OVERALLSIZECHECK(param_zone_cfga)
#if defined(FWCMD_FAILTEST)
FWCMD_PARAMSIZEX4(param_zone_cfga)
#endif      /* defined(FWCMD_FAILTEST) */

/* VCSFW_PARAM_REGISTER32 -- Unconditionally set any
   32-bit register in the sensor.
   Uses the vcsfw_cmd_poke32_t structure.
   Aligned to 4-byte boundary, for use beginning with Eagle */

/* VCSFW_PARAM_REG_OP32A -- Perform complex atomic operation
   on a register:
   *scmreg = (((*scmreg & andmask) ^ xormask) | ormask);
   Aligned to 4-byte boundary, for use beginning with Eagle */
typedef struct fwcmd_param_reg_op32a_s
{
    uint32_t addr;
    uint32_t andmask;
    uint32_t xormask;
    uint32_t ormask;
} fwcmd_param_reg_op32a_t;

FWCMD_ELCHECK(param_reg_op32a, addr)
FWCMD_ELCHECK(param_reg_op32a, andmask)
FWCMD_ELCHECK(param_reg_op32a, xormask)
FWCMD_ELCHECK(param_reg_op32a, ormask)
FWCMD_OVERALLSIZECHECK(param_reg_op32a)
#if defined(FWCMD_FAILTEST)
FWCMD_PARAMSIZEX4(param_reg_op32a)
#endif      /* defined(FWCMD_FAILTEST) */


/* VCSFW_PARAM_FPBUFFERING (34 = 0x22)*/
typedef struct fwcmd_param_fpbuffering_s
{
    uint32_t start_addr; // Start address of buffering area.
    uint32_t end_addr;   // End address of buffering area.
    uint32_t low_addr;   // Start address of FP buffering partition.
    uint32_t high_addr;  // End address of FP buffering partition.
} fwcmd_param_fpbuffering_t;

FWCMD_ELCHECK(param_fpbuffering, start_addr)
FWCMD_ELCHECK(param_fpbuffering, end_addr)
FWCMD_ELCHECK(param_fpbuffering, low_addr)
FWCMD_ELCHECK(param_fpbuffering, high_addr)
FWCMD_OVERALLSIZECHECK(param_fpbuffering)
FWCMD_PARAMSIZEX4(param_fpbuffering)

/* VCSFW_PARAM_CFGTSBLREPLY -- Reply to the GET_PRINT command
 *  with the configuration (register) data, timeslot table data, and
 *  baseline data.  This tag uses the vcsfw_cmd_get_cfgtsbl_t
 *  /fwcmd_cmd_get_cfgtsbl_t structure.
 */

/* VCSFW_CMD_GET_PRINT/VCSFW_PARAM_BASELINE (36 = 0x24)
 *    Uses the vcsfw_param_t structure to set a variable-length
 *    baseline, first used in Eagle. */

/* VCSFW_PARAM_SO_ALT
   Uses the vcsfw_param_so_alt_t structure to set
   multiple SO_ALT bits in the SCM.
   Bit 0 first value contains the enable value for
   so[0] and bit 3 of the last value contains
   the enable value for so[291]. */
typedef struct fwcmd_param_so_alt_s
{
    uint32_t so_alt[10];
} fwcmd_param_so_alt_t;
FWCMD_ELCHECK(param_so_alt, so_alt)
FWCMD_OVERALLSIZECHECK(param_so_alt)
FWCMD_PARAMSIZEX4(param_so_alt)

/* VCSFW_PARAM_FDETECT_SAMPLE_REGS
   Poke multiple registers with values prior to
   sampling for finger down
   First used in Eagle.
   NOTE: Followed by (num_regs * sizeof(vcsfw_cmd_poke32_t))
   bytes of address/value sets */

/* VCSFW_PARAM_FDETECT_SCAN_REGS
   Poke multiple registers with values prior to
   scanning after finger down
   First used in Eagle.
   NOTE: Followed by (num_regs * sizeof(vcsfw_cmd_poke32_t))
   bytes of address/value sets */

/* Define the results for a finger detection table.
   Use VCSFW_FDETECT_USE or VCSFW_FDETECT_INVALID
   in vcsfw_param_fdetect_table_t to use a particular
   detection method.
*/
typedef struct fwcmd_param_fdetect_table_s
{
   uint16_t outcomes; // Decision outcomes
   uint8_t use_wovar; // WOVAR, use {VCSFW_FDETECT_USE, VCSFW_FDETECT_DONTUSE}
   uint8_t use_asp;   // ASP, use {VCSFW_FDETECT_USE, VCSFW_FDETECT_DONTUSE}
} fwcmd_param_fdetect_table_t;

FWCMD_ELCHECK(param_fdetect_table, outcomes)
FWCMD_ELCHECK(param_fdetect_table, use_wovar)
FWCMD_ELCHECK(param_fdetect_table, use_asp)
FWCMD_OVERALLSIZECHECK(param_fdetect_table)
FWCMD_PARAMSIZEX4(param_fdetect_table)

/* VCSFW_PARAM_FDETECT -- Finger Detection setup,
   first used in Eagle. */
typedef struct fwcmd_param_fdetect_s
{
    fwcmd_param_fdetect_table_t dettbldown; // Detection table, finger-down
    fwcmd_param_fdetect_table_t dettblup;   // Detection table, finger-up
    uint32_t wovar_ctrl;      // WOVAR_CTRL register
    uint32_t wovar_fd_thresh; // WOVAR_FD_THRESH register
    uint32_t wovar_fu_thresh; // WOVAR_FU_THRESH register
    uint32_t asp_ctrl;        // ASP_CTRL register
    uint32_t asp_fd_thresh;   // ASP_FD_THRESH register
    uint32_t asp_fu_thresh;   // ASP_FU_THRESH register
    uint16_t polltimedown;    // Sampling poll time (ms), finger-down
    uint16_t polltimeup;      // Sampling poll time (ms), finger-up
    uint8_t  debouncedown;    // Debounce threshold, finger-down
    uint8_t  debounceup;      // Debounce threshold, finger-up
    uint8_t  pollsize;        // Sampling poll size (lines), finger-down only
    uint8_t  useflash;        // Buffer to flash
} fwcmd_param_fdetect_t;

FWCMD_ELCHECK(param_fdetect, dettbldown)
FWCMD_ELCHECK(param_fdetect, dettblup)
FWCMD_ELCHECK(param_fdetect, wovar_ctrl)
FWCMD_ELCHECK(param_fdetect, wovar_fd_thresh)
FWCMD_ELCHECK(param_fdetect, wovar_fu_thresh)
FWCMD_ELCHECK(param_fdetect, asp_ctrl)
FWCMD_ELCHECK(param_fdetect, asp_fd_thresh)
FWCMD_ELCHECK(param_fdetect, asp_fu_thresh)
FWCMD_ELCHECK(param_fdetect, polltimedown)
FWCMD_ELCHECK(param_fdetect, polltimeup)
FWCMD_ELCHECK(param_fdetect, debouncedown)
FWCMD_ELCHECK(param_fdetect, debounceup)
FWCMD_ELCHECK(param_fdetect, pollsize)
FWCMD_ELCHECK(param_fdetect, useflash)
FWCMD_OVERALLSIZECHECK(param_fdetect)
FWCMD_PARAMSIZEX4(param_fdetect)

/* VCSFW_PARAM_FDETECT_DEFER   (79 = 0x4f)*/
typedef struct fwcmd_param_fdetect_defer_s
{
    uint8_t       defer_type;          /* defer type to use */
    uint8_t       reserved[3];
} fwcmd_param_fdetect_defer_t;

FWCMD_ELCHECK(param_fdetect_defer, defer_type)
FWCMD_ELCHECK(param_fdetect_defer, reserved)
FWCMD_PARAMSIZEX4(param_fdetect_defer)

/* VCSFW_PARAM_FDETECT_NAV   (81 = 0x51)*/
typedef struct fwcmd_param_fdetect_nav_s
{
   uint16_t polltimedown;    // Sampling poll time (us), from the second finger detection during navigation
   uint16_t reserved;
} fwcmd_param_fdetect_nav_t;

FWCMD_ELCHECK(param_fdetect_nav, polltimedown)
FWCMD_ELCHECK(param_fdetect_nav, reserved)
FWCMD_PARAMSIZEX4(param_fdetect_nav)
FWCMD_OVERALLSIZECHECK(param_fdetect_nav)

/* VCSFW_PARAM_FDETECT_FU   (82 = 0x52)*/

/* VCSFW_PARAM_TSTBLOFF -- Adjusts the timeslot
 table after it is downloaded or adjusts the
 default (ROM) timeslot table, if so desired.
 If the timeslot table is set up appropriately,
 this parameter can adjust the swipespeed, for
 example. This is the number of 4-byte words of offset. */
typedef uint32_t fwcmd_param_tstbloff_t;
FWCMD_PARAMSIZEX4(param_tstbloff)

/* VCSFW_PARAM_ACM_CONFIG (42 = 0x2a) -- Advance Culling config */
typedef struct fwcmd_param_acm_config_s
{
    uint32_t      pixel_info_line;
    uint32_t      pixel_info_prim;
} fwcmd_param_acm_config_t;

FWCMD_ELCHECK(param_acm_config, pixel_info_line)
FWCMD_ELCHECK(param_acm_config, pixel_info_prim)
FWCMD_OVERALLSIZECHECK(param_acm_config)
FWCMD_PARAMSIZEX4(param_acm_config)

/* VCSFW_PARAM_ACM_CONTROL (43 = 0x2b) -- Advance Culling control */
typedef struct fwcmd_param_acm_control_s
{
    uint32_t      acm_ctrl;
    uint32_t      cull_pix_test1;
    uint32_t      cull_total_thresh_test1;
    uint32_t      params_test2;
    uint32_t      params_test3a;
    uint32_t      params_test3b;
    uint32_t      params_test3c;
    uint32_t      params_test5_md;
    uint32_t      params_test5_ncca;
    uint32_t      params_test5_nccb;
    uint32_t      params_test5_nccc;
    uint32_t      params_test5_nccc2;
} fwcmd_param_acm_control_t;

FWCMD_ELCHECK(param_acm_control, acm_ctrl)
FWCMD_ELCHECK(param_acm_control, cull_pix_test1)
FWCMD_ELCHECK(param_acm_control, cull_total_thresh_test1)
FWCMD_ELCHECK(param_acm_control, params_test2)
FWCMD_ELCHECK(param_acm_control, params_test3a)
FWCMD_ELCHECK(param_acm_control, params_test3b)
FWCMD_ELCHECK(param_acm_control, params_test3c)
FWCMD_ELCHECK(param_acm_control, params_test5_md)
FWCMD_ELCHECK(param_acm_control, params_test5_ncca)
FWCMD_ELCHECK(param_acm_control, params_test5_nccb)
FWCMD_ELCHECK(param_acm_control, params_test5_nccc)
FWCMD_ELCHECK(param_acm_control, params_test5_nccc2)
FWCMD_OVERALLSIZECHECK(param_acm_control)
FWCMD_PARAMSIZEX4(param_acm_control)

/* VCSFW_PARAM_CEM_CONFIG (44 = 0x2c) -- Correlation Engine config */
typedef struct fwcmd_param_cem_config_s
{
    uint32_t      ctl_Offsets_updown;
    uint32_t      ctl_offsets_sideab;
    uint32_t      ctl_offsets_sideab_acm;
    uint32_t      ctl_offsets_aux;
    uint32_t      ctl_yrange_updn;
    uint32_t      ctl_yrange_sideab;
    uint32_t      ctl_xrange_up;
    uint32_t      ctl_xrange_dn;
    uint32_t      ctl_xrange_sideab;
    uint32_t      ctl_xrange_aux;
} fwcmd_param_cem_config_t;

FWCMD_ELCHECK(param_cem_config, ctl_Offsets_updown)
FWCMD_ELCHECK(param_cem_config, ctl_offsets_sideab)
FWCMD_ELCHECK(param_cem_config, ctl_offsets_sideab_acm)
FWCMD_ELCHECK(param_cem_config, ctl_offsets_aux)
FWCMD_ELCHECK(param_cem_config, ctl_yrange_updn)
FWCMD_ELCHECK(param_cem_config, ctl_yrange_sideab)
FWCMD_ELCHECK(param_cem_config, ctl_xrange_up)
FWCMD_ELCHECK(param_cem_config, ctl_xrange_dn)
FWCMD_ELCHECK(param_cem_config, ctl_xrange_sideab)
FWCMD_ELCHECK(param_cem_config, ctl_xrange_aux)
FWCMD_OVERALLSIZECHECK(param_cem_config)
FWCMD_PARAMSIZEX4(param_cem_config)

/* VCSFW_PARAM_CEM_CONTROL (45 = 0x2d) -- Correlation Engine control */
typedef struct fwcmd_param_cem_control_s
{
    uint32_t      cem_ctrl;
    uint32_t      ctl_corr_params;
    uint32_t      ctl_opt_params_x1;
    uint32_t      ctl_opt_params_x2;
    uint32_t      ctl_opt_params_y;
    uint32_t      ctl_srchspc_lag;
    uint32_t      ctl_srchspc_win;
} fwcmd_param_cem_control_t;

FWCMD_ELCHECK(param_cem_control, cem_ctrl)
FWCMD_ELCHECK(param_cem_control, ctl_corr_params)
FWCMD_ELCHECK(param_cem_control, ctl_opt_params_x1)
FWCMD_ELCHECK(param_cem_control, ctl_opt_params_x2)
FWCMD_ELCHECK(param_cem_control, ctl_opt_params_y)
FWCMD_ELCHECK(param_cem_control, ctl_srchspc_lag)
FWCMD_ELCHECK(param_cem_control, ctl_srchspc_win)
FWCMD_OVERALLSIZECHECK(param_cem_control)
FWCMD_PARAMSIZEX4(param_cem_control)

/* VCSFW_PARAM_NAV (57 = 0x39) --Navigation params */
typedef struct fwcmd_param_nav_s
{
    uint16_t       navsendtime;
    uint16_t       navidletime;
    uint8_t        navmode;
    uint8_t        finger_det_thresh;
    uint8_t        finger_det_thresh2;
    uint8_t        updn_corr_thresh_low_snr;
    uint8_t        updn_corr_thresh_must_use;
    uint8_t        updn_corr_thresh;
    uint8_t        sd_corr_thresh;
    uint8_t        sd_corr_thresh_min;
    uint8_t        reli_corr_thresh;
    uint8_t        debounce_scan_up;
    uint8_t        debounce_scan_down;
    uint8_t        min_iter_time;
    uint8_t        event_qu_lat_proc;
    uint8_t        event_qu_lat_output;
    uint8_t        event_time_comb_eve_within;
    uint8_t        event_filter_stale_time;
} fwcmd_param_nav_t;

FWCMD_ELCHECK(param_nav,navsendtime)
FWCMD_ELCHECK(param_nav,navidletime)
FWCMD_ELCHECK(param_nav,navmode)
FWCMD_ELCHECK(param_nav,finger_det_thresh)
FWCMD_ELCHECK(param_nav,finger_det_thresh2)
FWCMD_ELCHECK(param_nav,updn_corr_thresh_low_snr)
FWCMD_ELCHECK(param_nav,updn_corr_thresh_must_use)
FWCMD_ELCHECK(param_nav,updn_corr_thresh)
FWCMD_ELCHECK(param_nav,sd_corr_thresh)
FWCMD_ELCHECK(param_nav,sd_corr_thresh_min)
FWCMD_ELCHECK(param_nav,reli_corr_thresh)
FWCMD_ELCHECK(param_nav,debounce_scan_up)
FWCMD_ELCHECK(param_nav,debounce_scan_down)
FWCMD_ELCHECK(param_nav,min_iter_time)
FWCMD_ELCHECK(param_nav,event_qu_lat_proc)
FWCMD_ELCHECK(param_nav,event_qu_lat_output)
FWCMD_ELCHECK(param_nav,event_time_comb_eve_within)
FWCMD_ELCHECK(param_nav,event_filter_stale_time)
FWCMD_OVERALLSIZECHECK(param_nav)
FWCMD_PARAMSIZEX4(param_nav)


/* VCSFW_PARAM_IMAGE_RECON (46 = 0x2e) -- Image reconstruction params */
typedef struct fwcmd_param_dlimager_s
{
    uint32_t        irflags;
    uint16_t        cols;
    uint16_t        rows;
    uint16_t        dpi;
    uint16_t        reserved;
    uint16_t        fpbuf_rows;
    uint16_t        reconimagesize;
    uint16_t        persistentmemsize;
    uint16_t        volatilememsize;

} fwcmd_param_dlimager_t;

FWCMD_ELCHECK(param_dlimager, irflags)
FWCMD_ELCHECK(param_dlimager, cols)
FWCMD_ELCHECK(param_dlimager, rows)
FWCMD_ELCHECK(param_dlimager, dpi)
FWCMD_ELCHECK(param_dlimager, fpbuf_rows)
FWCMD_ELCHECK(param_dlimager, reconimagesize)
FWCMD_ELCHECK(param_dlimager, persistentmemsize)
FWCMD_ELCHECK(param_dlimager, volatilememsize)
FWCMD_OVERALLSIZECHECK(param_dlimager)
FWCMD_PARAMSIZEX4(param_dlimager)


typedef struct fwcmd_param_ifsimager_s
{
    uint32_t        irflags;
    uint16_t        cols;
    uint16_t        rows;
    uint16_t        dpi;
    uint16_t        reserved1;
    uint8_t         stride;
    uint8_t         numDarkerThresh;
    uint8_t         numDarkerThreshPct;
    uint8_t         settleHistDiffThreshPct;
    uint8_t         settleHistDiffThreshPctHighconf;
    uint8_t         settleHistDiffThreshPctAvg;
    uint8_t         settleHistDiffThreshPctPrev;
    uint8_t         settleHistDarkerThreshPct;
    uint8_t         settleHistDarkerThreshPct2;
    uint8_t         settleHistLighterThreshPct;
    uint8_t         chooseHistLighterThreshPct;
    uint8_t         chooseHistLighterThreshPctExper;
    uint8_t         chooseHistLighterDiffThreshPctPrev;
    uint8_t         settleDarkCountThreshPct;
    uint8_t         fingerLiftNumLighterThreshPct;
    uint8_t         fingerLiftHistDiffThreshPct;
    uint8_t         fastChooseHistLighterThreshPct;
    uint8_t         fastChooseHistBelowAvgPct;
    uint8_t         fastChooseHistLighterThreshAltPct;
    uint8_t         fastChooseHistDiffThreshPct;
    uint8_t         fastChooseHistDiffThreshPctPrev;
    uint8_t         fastChooseHistBelowAvgCountDiff;
    uint8_t         fastChooseMaxFrameCount;
    uint8_t         baselineDiffThreshPct;
    uint8_t         maxFramesWithoutFinger;
    uint8_t         reserved2[3];
} fwcmd_param_ifsimager_t;

FWCMD_ELCHECK(param_ifsimager, irflags)
FWCMD_ELCHECK(param_ifsimager, cols)
FWCMD_ELCHECK(param_ifsimager, rows)
FWCMD_ELCHECK(param_ifsimager, dpi)
FWCMD_ELCHECK(param_ifsimager, stride)
FWCMD_ELCHECK(param_ifsimager, numDarkerThresh)
FWCMD_ELCHECK(param_ifsimager, numDarkerThreshPct)
FWCMD_ELCHECK(param_ifsimager, settleHistDiffThreshPct)
FWCMD_ELCHECK(param_ifsimager, settleHistDiffThreshPctHighconf)
FWCMD_ELCHECK(param_ifsimager, settleHistDiffThreshPctAvg)
FWCMD_ELCHECK(param_ifsimager, settleHistDiffThreshPctPrev)
FWCMD_ELCHECK(param_ifsimager, settleHistDarkerThreshPct)
FWCMD_ELCHECK(param_ifsimager, settleHistDarkerThreshPct2)
FWCMD_ELCHECK(param_ifsimager, settleHistLighterThreshPct)
FWCMD_ELCHECK(param_ifsimager, chooseHistLighterThreshPct)
FWCMD_ELCHECK(param_ifsimager, chooseHistLighterThreshPctExper)
FWCMD_ELCHECK(param_ifsimager, chooseHistLighterDiffThreshPctPrev)
FWCMD_ELCHECK(param_ifsimager, settleDarkCountThreshPct)
FWCMD_ELCHECK(param_ifsimager, fingerLiftNumLighterThreshPct)
FWCMD_ELCHECK(param_ifsimager, fingerLiftHistDiffThreshPct)
FWCMD_ELCHECK(param_ifsimager, fastChooseHistLighterThreshPct)
FWCMD_ELCHECK(param_ifsimager, fastChooseHistBelowAvgPct)
FWCMD_ELCHECK(param_ifsimager, fastChooseHistLighterThreshAltPct)
FWCMD_ELCHECK(param_ifsimager, fastChooseHistDiffThreshPct)
FWCMD_ELCHECK(param_ifsimager, fastChooseHistDiffThreshPctPrev)
FWCMD_ELCHECK(param_ifsimager, fastChooseHistBelowAvgCountDiff)
FWCMD_ELCHECK(param_ifsimager, fastChooseMaxFrameCount)
FWCMD_ELCHECK(param_ifsimager, baselineDiffThreshPct)
FWCMD_ELCHECK(param_ifsimager, maxFramesWithoutFinger)
FWCMD_OVERALLSIZECHECK(param_ifsimager)
FWCMD_PARAMSIZEX4(param_ifsimager)

typedef struct fwcmd_param_ifsimager_v2_s
{
    uint32_t     irflags;
    uint16_t     cols;               /* width of reconstructed image */
    uint16_t     rows;               /* height of reconstructed image */
    uint16_t     dpi;                /* resolution in dot per inch */
    union {
        uint16_t     reserved1;
        uint16_t     frameVarThresh;
    } details;

    /* various frame selection parameters */
    uint16_t         fingerPresentThresh;        /* avg line variance threshold for finger presence detect */
    uint16_t         fingerAbsentThresh;         /* avg line variance threshold for finger absent detect */
    uint8_t          fingerPresentThreshPct;     /* percent of lines with variance above threshold */
    uint8_t          fingerLiftThreshPct;        /* percent of lines with variance below threshold */
    uint8_t          fingerPresentVarThresh;     /* variance of the line variance threshold for finger detect */
    uint8_t          fingerFastSettleThresh;     /* frame delta threshold for fast settling */
    uint8_t          fingerSettleThresh;         /* frame delta threshold for normal settling */
    uint8_t          maxFramesWithoutFinger;     /* wait this long before giving up on seeing a finger */
    uint8_t          stride;
    uint8_t          rxRepeat;                   /* Rx repeat count to support line averaging */
}fwcmd_param_ifsimager_v2_t;

FWCMD_ELCHECK(param_ifsimager_v2, irflags)
FWCMD_ELCHECK(param_ifsimager_v2, cols)
FWCMD_ELCHECK(param_ifsimager_v2, rows)
FWCMD_ELCHECK(param_ifsimager_v2, dpi)
FWCMD_ELCHECK(param_ifsimager_v2, details)

FWCMD_ELCHECK(param_ifsimager_v2, fingerPresentThresh)
FWCMD_ELCHECK(param_ifsimager_v2, fingerAbsentThresh)
FWCMD_ELCHECK(param_ifsimager_v2, fingerPresentThreshPct)
FWCMD_ELCHECK(param_ifsimager_v2, fingerLiftThreshPct)
FWCMD_ELCHECK(param_ifsimager_v2, fingerPresentVarThresh)
FWCMD_ELCHECK(param_ifsimager_v2, fingerFastSettleThresh)
FWCMD_ELCHECK(param_ifsimager_v2, fingerSettleThresh)
FWCMD_ELCHECK(param_ifsimager_v2, maxFramesWithoutFinger)
FWCMD_ELCHECK(param_ifsimager_v2, stride)
FWCMD_ELCHECK(param_ifsimager_v2, rxRepeat)
FWCMD_OVERALLSIZECHECK(param_ifsimager_v2)
FWCMD_PARAMSIZEX4(param_ifsimager_v2)

typedef struct fwcmd_param_image_recon_s
{
    uint16_t       imagertype;          /* imager type to use */
    uint16_t       paramsize;           /* size of the imager parameters follow */
} fwcmd_param_image_recon_t;

FWCMD_ELCHECK(param_image_recon, imagertype)
FWCMD_ELCHECK(param_image_recon, paramsize)
FWCMD_OVERALLSIZECHECK(param_image_recon)
FWCMD_PARAMSIZEX4(param_image_recon)

/* VCSFW_CMD_GET_PRINT/VCSFW_PARAM_IFSIMAGER_FRAMESTATS (61 = 0x3d) */
typedef struct fwcmd_param_framestats_s
{
    uint16_t thresh;
    uint16_t margin;
    uint16_t pixCount;
    uint16_t aboveAvgCount;
    uint16_t belowAvgCount;
    int16_t  medianBin;      /* Use signed because subtracting */
    uint16_t histogram[16];  /* 4-bit data only */
    uint16_t diffPct;        /* As calculated, so we have some history */
    uint16_t diffPctBaseline;/* As calculated, so we have some history */
} fwcmd_param_framestats_t;

FWCMD_ELCHECK(param_framestats, thresh)
FWCMD_ELCHECK(param_framestats, margin)
FWCMD_ELCHECK(param_framestats, pixCount)
FWCMD_ELCHECK(param_framestats, aboveAvgCount)
FWCMD_ELCHECK(param_framestats, belowAvgCount)
FWCMD_ELCHECK(param_framestats, medianBin)
FWCMD_ELCHECK(param_framestats, histogram)
FWCMD_ELCHECK(param_framestats, diffPct)
FWCMD_ELCHECK(param_framestats, diffPctBaseline)
FWCMD_OVERALLSIZECHECK(param_framestats)
FWCMD_PARAMSIZEX4(param_framestats)

/* VCSFW_CMD_GET_PRINT/VCSFW_PARAM_2D (47 = 0x2f) */

typedef struct fwcmd_param_2d_s
{
    uint32_t      linesperframe;     /* number of lines per frame nom=160*/
} fwcmd_param_2d_t;

FWCMD_ELCHECK(param_2d, linesperframe)
FWCMD_OVERALLSIZECHECK(param_2d)
FWCMD_PARAMSIZEX4(param_2d)

/* VCSFW_CMD_GET_PRINT/VCSFW_PARAM_LINEUPD (48 = 0x30) */

typedef struct fwcmd_param_lineupd_s
{
    uint32_t      ngroups;           /* number of groups to follow */
} fwcmd_param_lineupd_t;

FWCMD_ELCHECK(param_lineupd, ngroups)
FWCMD_OVERALLSIZECHECK(param_lineupd)
FWCMD_PARAMSIZEX4(param_lineupd)

typedef struct fwcmd_param_lineupd_group_s
{
    uint32_t      mask;              /* mask to use for packing or r/m/w */
    uint32_t      flags;             /* flags and offset */
} fwcmd_param_lineupd_group_t;

FWCMD_ELCHECK(param_lineupd_group, mask)
FWCMD_ELCHECK(param_lineupd_group, flags)
FWCMD_OVERALLSIZECHECK(param_lineupd_group)
FWCMD_PARAMSIZEX4(param_lineupd_group)

/* VCSFW_CMD_GET_PRINT/VCSFW_PARAM_TSFDETECT (49 = 0x31) */
typedef struct fwcmd_param_tsfdetect_s
{
    uint32_t tscfg[352];
} fwcmd_param_tsfdetect_t;

FWCMD_ELCHECK(param_tsfdetect, tscfg)
FWCMD_OVERALLSIZECHECK(param_tsfdetect)
FWCMD_PARAMSIZEX4(param_tsfdetect)

/* VCSFW_PARAM_HWREG16 (50 = 0x32) */
typedef struct fwcmd_param_reg16_list_s
{
    uint16_t    regoffset;      // register offset from HW base
    uint16_t    value;          // value to poke (zero extended to 32-bit)
} fwcmd_param_reg16_list_t;

FWCMD_ELCHECK(param_reg16_list, regoffset)
FWCMD_ELCHECK(param_reg16_list, value)
FWCMD_OVERALLSIZECHECK(param_reg16_list)
FWCMD_PARAMSIZEX4(param_reg16_list)

typedef struct fwcmd_param_hwreg16_s
{
    uint32_t    hwreg_base;     // base address of HW registers
    fwcmd_param_reg16_list_t reglist[1]; // list of register to poke
} fwcmd_param_hwreg16_t;

FWCMD_ELCHECK(param_hwreg16, hwreg_base)
FWCMD_ELCHECK(param_hwreg16, reglist)
FWCMD_OVERALLSIZECHECK(param_hwreg16)
FWCMD_PARAMSIZEX4(param_hwreg16)

/* VCSFW_PARAM_FPSECURITY -- Fingerprint security parameters                 */
 typedef struct fwcmd_param_fpsecurity_t
{
    uint8_t     AES_key[256 / 8];    /*                                       */
    uint8_t     AES_IV[256 / 8];     /* 64 bits of Counter in Counter mode    */
    uint8_t     hash_key[256 / 8];   /*                                       */
    uint32_t    nonce;               /*                                       */
    uint32_t    clock;               /* AES clock frequency                   */
    uint8_t     keysize;             /* Applies to AESKey, IV, and HashKey    */
    uint8_t     mode;                /* 0 = passthru, 1 = CBC, 2 = Counter    */
    uint16_t    reserved;            /* reserved                              */
} fwcmd_param_fpsecurity_t;

FWCMD_ELCHECK(param_fpsecurity, AES_key)
FWCMD_ELCHECK(param_fpsecurity, AES_IV)
FWCMD_ELCHECK(param_fpsecurity, hash_key)
FWCMD_ELCHECK(param_fpsecurity, nonce)
FWCMD_ELCHECK(param_fpsecurity, clock)
FWCMD_ELCHECK(param_fpsecurity, keysize)
FWCMD_ELCHECK(param_fpsecurity, mode)
FWCMD_ELCHECK(param_fpsecurity, reserved)
FWCMD_OVERALLSIZECHECK(param_fpsecurity)
FWCMD_PARAMSIZEX4(param_fpsecurity)

/* VCSFW_PARAM_WOF2 (55 = 0x37) */
typedef struct fwcmd_param_wof2_s
{
    uint32_t wof2_ctrl2;     /* WOE_WOF2_CTRL2 register  */
    uint32_t wof2_thresh;    /* WOE_WOF2_THRESH register */
    uint16_t wof2_ctrl1;     /* WOE_WOF2_CTRL1 register  */
    uint16_t so_aiso_p;      /* SO line loaded AISO_P */
    uint16_t so_aiso_m;      /* SO line loaded AISO_M */
    uint16_t so_tr_cnt;      /* count of SO transmitters follow */
} fwcmd_param_wof2_t;

FWCMD_ELCHECK(param_wof2, wof2_ctrl1)
FWCMD_ELCHECK(param_wof2, wof2_ctrl2)
FWCMD_ELCHECK(param_wof2, wof2_thresh)
FWCMD_ELCHECK(param_wof2, so_aiso_p)
FWCMD_ELCHECK(param_wof2, so_aiso_m)
FWCMD_ELCHECK(param_wof2, so_tr_cnt)
FWCMD_OVERALLSIZECHECK(param_wof2)
FWCMD_PARAMSIZEX4(param_wof2)

/* VCSFW_PARAM_WOF2 (56 = 0x38) */
typedef struct fwcmd_param_wof_s
{
    uint32_t wof_thresh;     /* WOE_WOF_THREAH register */
    uint32_t wof_timing;     /* WOE_WOF_TIMING register */
    uint32_t wof_trim;       /* WOE_WOF_TRIM register */
    uint32_t so_driver;      /* SO line to be used as drivers for WOF */
} fwcmd_param_wof_t;

FWCMD_ELCHECK(param_wof, wof_thresh)
FWCMD_ELCHECK(param_wof, wof_timing)
FWCMD_ELCHECK(param_wof, wof_trim)
FWCMD_ELCHECK(param_wof, so_driver)
FWCMD_OVERALLSIZECHECK(param_wof)
FWCMD_PARAMSIZEX4(param_wof)

/* VCSFW_PARAM_WOF2_V2 (58 = 0x3a) */
typedef struct fwcmd_param_wof2_v2_s
{
    uint32_t wof2_ctrl2;     /* WOE_WOF2_CTRL2 register  */
    uint32_t wof2_thresh;    /* WOE_WOF2_THRESH register */
    uint16_t wof2_ctrl1;     /* WOE_WOF2_CTRL1 register  */
    uint16_t wof2_cfg_size;  /* Number of bytes of cfg data follow */
} fwcmd_param_wof2_v2_t;

FWCMD_ELCHECK(param_wof2_v2, wof2_ctrl1)
FWCMD_ELCHECK(param_wof2_v2, wof2_ctrl2)
FWCMD_ELCHECK(param_wof2_v2, wof2_thresh)
FWCMD_ELCHECK(param_wof2_v2, wof2_cfg_size)
FWCMD_OVERALLSIZECHECK(param_wof2_v2)
FWCMD_PARAMSIZEX4(param_wof2_v2)

/* WOF2 SO configuration parameters */
typedef struct fwcmd_param_wof2_config_s
{
    uint16_t so_aisop_config;    /* AISO_P receiver config */
    uint16_t so_aisom_config;    /* AISO_M receiver config */
    uint16_t so_trans_config;    /* Transmitter config */
    uint8_t  so_config[2];
} fwcmd_param_wof2_config_t;

FWCMD_ELCHECK(param_wof2_config, so_aisop_config)
FWCMD_ELCHECK(param_wof2_config, so_aisom_config)
FWCMD_ELCHECK(param_wof2_config, so_trans_config)
FWCMD_ELCHECK(param_wof2_config, so_config)
FWCMD_OVERALLSIZECHECK(param_wof2_config)
FWCMD_PARAMSIZEX4(param_wof2_config)

/* VCSFW_CMD_GET_PRINT/VCSFW_PARAM_CAL_WOF2 (59 = 0x3b) */
typedef struct fwcmd_param_cal_wof2_s
{
    uint16_t recal_period; /* Recalibration period (secs), max 11.93 hrs nom. */
    uint8_t  max_gain;     /* Highest gain allowed */
    uint8_t  min_gain;     /* Lowest gain allowed */
    uint8_t  start_gain;   /* Starting gain */
    uint8_t  step_gain;    /* Step size for gain */
    uint8_t  max_thresh;   /* Highest threshold allowed */
    uint8_t  min_thresh;   /* Lowest threshold allowed */
    uint8_t  polltime;     /* Wake-on-Timer definition (ms) */
    uint8_t reserved[3];
} fwcmd_param_cal_wof2_t;

FWCMD_ELCHECK(param_cal_wof2, recal_period)
FWCMD_ELCHECK(param_cal_wof2, max_gain)
FWCMD_ELCHECK(param_cal_wof2, min_gain)
FWCMD_ELCHECK(param_cal_wof2, start_gain)
FWCMD_ELCHECK(param_cal_wof2, step_gain)
FWCMD_ELCHECK(param_cal_wof2, max_thresh)
FWCMD_ELCHECK(param_cal_wof2, min_thresh)
FWCMD_ELCHECK(param_cal_wof2, polltime)
FWCMD_ELCHECK(param_cal_wof2, reserved)
FWCMD_OVERALLSIZECHECK(param_cal_wof2)
FWCMD_PARAMSIZEX4(param_cal_wof2)

/* VCSFW_CMD_GET_PRINT/VCSFW_PARAM_EVENTSIG (60 = 0x3c) */
typedef struct fwcmd_param_eventsig_s
{
    uint16_t    eventmask;
    uint8_t     gpionum;
    uint8_t     reserved;
} fwcmd_param_eventsig_t;

FWCMD_ELCHECK(param_eventsig, eventmask)
FWCMD_ELCHECK(param_eventsig, gpionum)
FWCMD_ELCHECK(param_eventsig, reserved)
FWCMD_OVERALLSIZECHECK(param_eventsig)
FWCMD_PARAMSIZEX4(param_eventsig)

/* VCSFW_PARAM_SNRMETHOD4 (62 = 0x3e) */
/* CWB Definition to follow soon. */

/* VCSFW_PARAM_WOF2_V3 (63 = 0x3f) -- Setup WOE WOF2 parameters,
   first used in Metallica B1 silicon. */
typedef struct fwcmd_param_wof2_v3_s
{
    uint32_t wof2_ctrl2;     /* WOE_WOF2_CTRL2 register  */
    uint32_t wof2_thresh;    /* WOE_WOF2_THRESH register, zone 0 */
    uint32_t wof2_threshz1;  /* WOE_WOF2_THRESH_Z1 register, zone 1 */
    uint16_t wof2_ctrl1;     /* WOE_WOF2_CTRL1 register  */
    uint16_t wof2_cfg_size;  /* Number of bytes of cfg data follow */
    uint8_t  wof2_flags;     /* Calibration directives defined in vcsfw_v4.h */
    uint8_t  reserved[3];
} fwcmd_param_wof2_v3_t;
FWCMD_ELCHECK(param_wof2_v3, wof2_ctrl2)
FWCMD_ELCHECK(param_wof2_v3, wof2_thresh)
FWCMD_ELCHECK(param_wof2_v3, wof2_threshz1)
FWCMD_ELCHECK(param_wof2_v3, wof2_ctrl1)
FWCMD_ELCHECK(param_wof2_v3, wof2_cfg_size)
FWCMD_ELCHECK(param_wof2_v3, wof2_flags)
FWCMD_ELCHECK(param_wof2_v3, reserved)
FWCMD_OVERALLSIZECHECK(param_wof2_v3)
FWCMD_PARAMSIZEX4(param_wof2_v3)

/* VCSFW_PARAM_CAL_WOF2_V3 (64 = 0x40) */
typedef struct fwcmd_param_cal_wof2_v3_s
{
    uint16_t recal_period; /* Recalibration period (secs), max 11.93 hrs nom. */
    uint8_t  max_gain;     /* Highest gain allowed */
    uint8_t  min_gain;     /* Lowest gain allowed */
    uint8_t  start_gain;   /* Starting gain */
    uint8_t  step_gain;    /* Step size for gain */
    uint8_t  max_thresh;   /* Highest threshold allowed */
    uint8_t  min_thresh;   /* Lowest threshold allowed */
    uint8_t  polltime;     /* Wake-on-Timer definition (ms) */
    uint8_t  reserved[3];
} fwcmd_param_cal_wof2_v3_t;
FWCMD_ELCHECK(param_cal_wof2_v3, recal_period)
FWCMD_ELCHECK(param_cal_wof2_v3, max_gain)
FWCMD_ELCHECK(param_cal_wof2_v3, min_gain)
FWCMD_ELCHECK(param_cal_wof2_v3, start_gain)
FWCMD_ELCHECK(param_cal_wof2_v3, step_gain)
FWCMD_ELCHECK(param_cal_wof2_v3, max_thresh)
FWCMD_ELCHECK(param_cal_wof2_v3, min_thresh)
FWCMD_ELCHECK(param_cal_wof2_v3, polltime)
FWCMD_ELCHECK(param_cal_wof2_v3, reserved)
FWCMD_OVERALLSIZECHECK(param_cal_wof2_v3)
FWCMD_PARAMSIZEX4(param_cal_wof2_v3)

/* VCSFW_PARAM_FDETECT_RATCHET (65 = 0x41) */
typedef struct fwcmd_param_fdetect_ratchet_s
{
    uint8_t  wof2_flag;      /* specify which thresh to ratchet */
    uint8_t  min_thresh;     /* min thresh allowed */
    uint8_t  max_thresh;     /* max thresh allowed */
    uint8_t  ftcount_thresh; /* false trigger count to trigger ratcheting */
} fwcmd_param_fdetect_ratchet_t;
FWCMD_ELCHECK(param_fdetect_ratchet, wof2_flag)
FWCMD_ELCHECK(param_fdetect_ratchet, min_thresh)
FWCMD_ELCHECK(param_fdetect_ratchet, max_thresh)
FWCMD_ELCHECK(param_fdetect_ratchet, ftcount_thresh)
FWCMD_OVERALLSIZECHECK(param_fdetect_ratchet)
FWCMD_PARAMSIZEX4(param_fdetect_ratchet)

/* VCSFW_PARAM_DATA_ENCODER (66 = 0x42) */
typedef struct fwcmd_param_data_encoder_s
{
    uint8_t  encoder;    /* encoder type */
    uint8_t  offset;     /* offset from the beginning of the line */
    uint16_t pixels;     /* number of pixels encoded */
    uint32_t param;      /* encoder specific parameter */
} fwcmd_param_data_encoder_t;

FWCMD_ELCHECK(param_data_encoder, encoder)
FWCMD_ELCHECK(param_data_encoder, offset)
FWCMD_ELCHECK(param_data_encoder, pixels)
FWCMD_ELCHECK(param_data_encoder, param)
FWCMD_OVERALLSIZECHECK(param_data_encoder)
FWCMD_PARAMSIZEX4(param_data_encoder)

/* VCSFW_PARAM_LINEUPD_TRANSFORM (67 = 0x43) */
typedef struct fwcmd_param_lineupd_transform_s
{
    uint8_t  flags;
    uint8_t  adjadd;
    uint16_t romoffset;
} fwcmd_param_lineupd_transform_t;

FWCMD_ELCHECK(param_lineupd_transform, flags)
FWCMD_ELCHECK(param_lineupd_transform, adjadd)
FWCMD_ELCHECK(param_lineupd_transform, romoffset)
FWCMD_OVERALLSIZECHECK(param_lineupd_transform)
FWCMD_PARAMSIZEX4(param_lineupd_transform)


/* VCSFW_PARAM_LINEUPD_INTERLEAVE (68 = 0x44) */
typedef struct fwcmd_param_lineupd_interleave_s
{
    uint32_t     interleavecount;
} fwcmd_param_lineupd_interleave_t;

FWCMD_ELCHECK(param_lineupd_interleave, interleavecount)
FWCMD_OVERALLSIZECHECK(param_lineupd_interleave)
FWCMD_PARAMSIZEX4(param_lineupd_interleave)

/* VCSFW_PARAM_SOTABLE (69 = 0x45) */
typedef struct fwcmd_param_sotable_s
{
    uint32_t      flags;
} fwcmd_param_sotable_t;
FWCMD_ELCHECK(param_sotable, flags)
FWCMD_OVERALLSIZECHECK(param_sotable)
FWCMD_PARAMSIZEX4(param_sotable)

/* VCSFW_PARAM_TSMACRO (70 = 0x46) */
typedef struct fwcmd_param_tsmacro_instance_s
{
    uint8_t      nbytes;
    int8_t       soidx_incr;
} fwcmd_param_tsmacro_instance_t;
FWCMD_ELCHECK(param_tsmacro_instance, nbytes)
FWCMD_ELCHECK(param_tsmacro_instance, soidx_incr)
FWCMD_OVERALLSIZECHECK(param_tsmacro_instance)

typedef struct fwcmd_param_tsmacro_s
{
    uint8_t       nmacros;
} fwcmd_param_tsmacro_t;
FWCMD_ELCHECK(param_tsmacro, nmacros)
FWCMD_OVERALLSIZECHECK(param_tsmacro)
#if defined(FWCMD_FAILTEST)
/* It's okay -- it can be padded */
FWCMD_PARAMSIZEX4(param_tsmacro)
#endif

/* VCSFW_PARAM_BASELINE_FRAME (72 = 0x48) */
typedef struct fwcmd_param_baseline_frame_s
{
    uint16_t      linelength;
    uint16_t      lines;
} fwcmd_param_baseline_frame_t;
FWCMD_ELCHECK(param_baseline_frame, linelength)
FWCMD_ELCHECK(param_baseline_frame, lines)
FWCMD_OVERALLSIZECHECK(param_baseline_frame)
FWCMD_PARAMSIZEX4(param_baseline_frame)

/* VCSFW_PARAM_RXSELECT (73 = 0x49) */
typedef struct fwcmd_param_rxselect_s
{
    uint8_t      flags;
    uint8_t      nrxs;
    uint8_t      firstrx;
} fwcmd_param_rxselect_t;

FWCMD_ELCHECK(param_rxselect, flags)
FWCMD_ELCHECK(param_rxselect, nrxs)
FWCMD_ELCHECK(param_rxselect, firstrx)
FWCMD_OVERALLSIZECHECK(param_rxselect)
#if defined(FWCMD_FAILTEST)
/* It's okay -- it can be and is padded */
FWCMD_PARAMSIZEX4(param_rxselect)
#endif

/* VCSFW_PARAM_TEMPERATURE_CAL (74 = 0x4a) */
typedef struct fwcmd_param_temperature_cal_s
{
    uint32_t cal_period;       /* Interval (ms) for firmware to check for recalibration. */
    uint16_t host_temperature; /* 9-bit ADC count. Compare against this if use_otprom is 0. */
    uint16_t min_deviation;    /* Deviation allowed if current temp < last temp, in ADC counts */
    uint16_t max_deviation;    /* Deviation allowed if current temp > last temp, in ADC counts */
    uint8_t  use_otprom;       /* 1: Compare against value from OTPROM; else host must specify
                                  0: Check against host_temperature. */
    uint8_t num_samples;       /* Number of temperature samples to average. Use {1, 2, 4, 8}. */
} fwcmd_param_temperature_cal_t;
FWCMD_ELCHECK(param_temperature_cal, cal_period)
FWCMD_ELCHECK(param_temperature_cal, host_temperature)
FWCMD_ELCHECK(param_temperature_cal, min_deviation)
FWCMD_ELCHECK(param_temperature_cal, max_deviation)
FWCMD_ELCHECK(param_temperature_cal, use_otprom)
FWCMD_ELCHECK(param_temperature_cal, num_samples)
FWCMD_OVERALLSIZECHECK(param_temperature_cal)
FWCMD_PARAMSIZEX4(param_temperature_cal)

/* VCSFW_PARAM_SCM_WOF (76 = 0x4c) -- Setup SCM WOF parameters */
typedef struct fwcmd_param_scm_wof_s
{
    uint32_t woe_wof2_thresh;   /* WOE_WOF2_THRESH register, zone 0 */
    uint32_t woe_wof2_threshz1; /* WOE_WOF2_THRESH_Z1 register, zone 1 */
    uint8_t  scm_wof_thresh;    /* SCM_WOF_THRESH register */
    uint8_t  reserved[3];
} fwcmd_param_scm_wof_t;
FWCMD_ELCHECK(param_scm_wof, woe_wof2_thresh)
FWCMD_ELCHECK(param_scm_wof, woe_wof2_threshz1)
FWCMD_ELCHECK(param_scm_wof, scm_wof_thresh)
FWCMD_ELCHECK(param_scm_wof, reserved)
FWCMD_OVERALLSIZECHECK(param_scm_wof)
FWCMD_PARAMSIZEX4(param_scm_wof)

/* VCSFW_PARAM_CAL_SCM_WOF */
typedef struct fwcmd_param_cal_scm_wof_s
{
    uint16_t max_replysize; /* Maximum bytes in reply, based on params */
    uint8_t  max_gain;     /* Highest gain allowed */
    uint8_t  min_gain;     /* Lowest gain allowed */
    uint8_t  start_gain;   /* Starting gain */
    uint8_t  step_gain;    /* Step size for gain */
    uint8_t  max_thresh;   /* Highest threshold allowed */
    uint8_t  min_thresh;   /* Lowest threshold allowed */
    uint8_t  wof_flags;    /* Calibration directives */
    uint8_t  reserved[3];
    fwcmd_param_scm_wof_t scm_wof; /* VCSFW_PARAM_SCM_WOF params */
} fwcmd_param_cal_scm_wof_t;
FWCMD_ELCHECK(param_cal_scm_wof, max_replysize)
FWCMD_ELCHECK(param_cal_scm_wof, max_gain)
FWCMD_ELCHECK(param_cal_scm_wof, min_gain)
FWCMD_ELCHECK(param_cal_scm_wof, start_gain)
FWCMD_ELCHECK(param_cal_scm_wof, step_gain)
FWCMD_ELCHECK(param_cal_scm_wof, max_thresh)
FWCMD_ELCHECK(param_cal_scm_wof, min_thresh)
FWCMD_ELCHECK(param_cal_scm_wof, wof_flags)
FWCMD_ELCHECK(param_cal_scm_wof, reserved)
FWCMD_OVERALLSIZECHECK(param_cal_scm_wof)
FWCMD_PARAMSIZEX4(param_cal_scm_wof)

typedef struct fwcmd_cal_scm_wof_status_s {
    uint32_t scm_status;     /* SCM_SEN_STATUS register value */
    uint32_t wof_status;     /* SCM_WOF_STATUS register value */
    uint8_t  current_state;  /* Calibration current state */
    uint8_t  current_gain;   /* Gain register field value */
    uint8_t  current_thresh; /* Threshold register field value */
    uint8_t  reserved;
} fwcmd_cal_scm_wof_status_t;
FWCMD_ELCHECK(cal_scm_wof_status, scm_status)
FWCMD_ELCHECK(cal_scm_wof_status, wof_status)
FWCMD_ELCHECK(cal_scm_wof_status, current_state)
FWCMD_ELCHECK(cal_scm_wof_status, current_gain)
FWCMD_ELCHECK(cal_scm_wof_status, current_thresh)
FWCMD_ELCHECK(cal_scm_wof_status, reserved)
FWCMD_OVERALLSIZECHECK(cal_scm_wof_status)

typedef struct fwcmd_reply_cal_scm_wof_s
{
    uint32_t   bytes;   /* Number of status bytes returned */
    /* This is followed by the calibration status data. */
} fwcmd_reply_cal_scm_wof_t;
FWCMD_ELCHECK(reply_cal_scm_wof, bytes)
FWCMD_OVERALLSIZECHECK(reply_cal_scm_wof)

/* VCSFW_PARAM_SCM_WOF_V2 (80 = 0x50) -- Setup SCM WOF V2 parameters */
typedef struct fwcmd_param_scm_wof_v2_s
{
    uint32_t woe_wof2_thresh;       /* WOE_WOF2_THRESH register, zone 0 */
    uint32_t woe_wof2_threshz1;     /* WOE_WOF2_THRESH_Z1 register, zone 1 */
    uint8_t  scm_wof_thresh;        /* SCM_WOF_THRESH register */
    uint8_t  scm_wof_debounceup;    /* Debounce count for FU detection */
    uint8_t  reserved[2];
} fwcmd_param_scm_wof_v2_t;
FWCMD_ELCHECK(param_scm_wof_v2, woe_wof2_thresh)
FWCMD_ELCHECK(param_scm_wof_v2, woe_wof2_threshz1)
FWCMD_ELCHECK(param_scm_wof_v2, scm_wof_thresh)
FWCMD_ELCHECK(param_scm_wof_v2, reserved)
FWCMD_OVERALLSIZECHECK(param_scm_wof_v2)
FWCMD_PARAMSIZEX4(param_scm_wof_v2)

/****************************************************************************/
/* VCSFW_CMD_GLOBAL_KEY_GEN (92 = 0x5C)                                     */
/****************************************************************************/

/* Operation parameters/reply for VCSFW_CMD_GLOBAL_KEYGEN_VCEKIV_SSPRIVK. */
typedef struct fwcmd_cmd_global_key_gen_vcekiv_s
{
    uint8_t vpubk[VCSFW_MAX_RSA_KEY_LENGTH];
} fwcmd_cmd_global_key_gen_vcekiv_t;
FWCMD_ELCHECK(cmd_global_key_gen_vcekiv, vpubk)
FWCMD_OVERALLSIZECHECK(cmd_global_key_gen_vcekiv)

typedef struct fwcmd_reply_global_key_gen_vcekiv_s
{
    uint8_t unused[2];
    uint8_t vcekiv_vcekiv0[VCSFW_SHA256_HASH_SIZE*2+16];
    uint8_t vcekiv_vpubk[VCSFW_MAX_RSA_KEY_LENGTH];
    uint8_t ssprivk_vcekiv[VCSFW_MAX_RSA_KEY_LENGTH];
    uint8_t sspubk_plain[VCSFW_MAX_RSA_KEY_LENGTH];
} fwcmd_reply_global_key_gen_vcekiv_t;
FWCMD_ELCHECK(reply_global_key_gen_vcekiv, unused)
FWCMD_ELCHECK(reply_global_key_gen_vcekiv, vcekiv_vcekiv0)
FWCMD_ELCHECK(reply_global_key_gen_vcekiv, vcekiv_vpubk)
FWCMD_ELCHECK(reply_global_key_gen_vcekiv, ssprivk_vcekiv)
FWCMD_ELCHECK(reply_global_key_gen_vcekiv, sspubk_plain)
FWCMD_OVERALLSIZECHECK(reply_global_key_gen_vcekiv)

/* Operation parameters/reply for VCSFW_CMD_GLOBAL_KEYGEN_SYM. */
typedef struct fwcmd_cmd_global_key_gen_sym_s
{
    uint32_t keylen;
    uint8_t  vpubk[VCSFW_MAX_RSA_KEY_LENGTH];
} fwcmd_cmd_global_key_gen_sym_t;
FWCMD_ELCHECK(cmd_global_key_gen_sym, keylen)
FWCMD_ELCHECK(cmd_global_key_gen_sym, vpubk)
FWCMD_OVERALLSIZECHECK(cmd_global_key_gen_sym)

typedef struct fwcmd_reply_global_key_gen_sym_s
{
    uint16_t datalen;
    uint8_t  symk_vcekiv[VCSFW_SHA256_HASH_SIZE*3];
    uint8_t  symk_vpubk[VCSFW_MAX_RSA_KEY_LENGTH];
} fwcmd_reply_global_key_gen_sym_t;
FWCMD_ELCHECK(reply_global_key_gen_sym, datalen)
FWCMD_ELCHECK(reply_global_key_gen_sym, symk_vcekiv)
FWCMD_ELCHECK(reply_global_key_gen_sym, symk_vpubk)
FWCMD_OVERALLSIZECHECK(reply_global_key_gen_sym)

/* COMMAND                                                                  */
typedef struct fwcmd_cmd_global_key_gen_s
{
    uint16_t  opid;   /* The operation ID. */
    uint16_t  unused; /* To make 'operation' 4-byte aligned for Metallica. */
    union
    {
        fwcmd_cmd_global_key_gen_vcekiv_t   vcekiv_ssprivk;
        fwcmd_cmd_global_key_gen_sym_t      sym;
    } operation;
} fwcmd_cmd_global_key_gen_t;
FWCMD_ELCHECK(cmd_global_key_gen, opid)
FWCMD_ELCHECK(cmd_global_key_gen, unused)
FWCMD_ELCHECK(cmd_global_key_gen, operation)

/****************************************************************************/
/* VCSFW_CMD_AUTH_SESSION_START (94 = 0x5E)                                 */
/****************************************************************************/

/* COMMAND                                                                  */
typedef struct fwcmd_cmd_auth_session_start_s
{
    uint8_t  auth_type;     /* Authentication type: SM, MOC, POA, etc.   */
    uint8_t  timeout;       /* Session timeout in seconds.               */
    uint16_t uc_objid;      /* User object container ID.                 */
    uint16_t usr_objid;     /* User object ID.                           */
    uint16_t auth_datatype; /* Aka Authentication algorithm ID - AlgID.  */
    uint32_t add_datalen;   /* Additional data length. This may be required
                             * for POA mode. Data will be appended to the
                             * command structure.                        */
} fwcmd_cmd_auth_session_start_t;
/* Optionally followed by {additional data}. */
FWCMD_ELCHECK(cmd_auth_session_start, auth_type)
FWCMD_ELCHECK(cmd_auth_session_start, timeout)
FWCMD_ELCHECK(cmd_auth_session_start, uc_objid)
FWCMD_ELCHECK(cmd_auth_session_start, usr_objid)
FWCMD_ELCHECK(cmd_auth_session_start, auth_datatype)
FWCMD_ELCHECK(cmd_auth_session_start, add_datalen)
FWCMD_OVERALLSIZECHECK(cmd_auth_session_start)

/* REPLY                                                                    */
typedef struct fwcmd_reply_auth_session_start_s
{
    uint32_t sessionid;     /* Session ID. */
} fwcmd_reply_auth_session_start_t;
FWCMD_ELCHECK(reply_auth_session_start, sessionid)
FWCMD_OVERALLSIZECHECK(reply_auth_session_start)

/****************************************************************************/
/* VCSFW_CMD_AUTH_SESSION_STATUS_GET (95 = 0x5F)                            */
/****************************************************************************/

/* COMMAND                                                                  */
typedef struct fwcmd_cmd_auth_session_status_get_s
{
    uint32_t  sessionid;     /* Session ID. */
} fwcmd_cmd_auth_session_status_get_t;
FWCMD_ELCHECK(cmd_auth_session_status_get, sessionid)
FWCMD_OVERALLSIZECHECK(cmd_auth_session_status_get)

/* REPLY                                                                    */
/* The original vcsfw_reply_auth_session_status_get_s structure will be used
 * since it has an odd length (it is made equal to EP3 buffer size). */

 /* Optionally it may be followed by the statistical informaiton collected
 * during POA. vcsfw_reply_auth_session_status_get_t::reserved will contain
 * data size in that case, instead of 0xdb */
typedef struct fwcmd_moc_stat_info_s
{
    uint16_t truly_triggered;             /* Truely triggered.             */
    uint16_t false_triggered;             /* False triggered.              */
    uint16_t bad_images;                  /* Bad image and other problems. */
    uint16_t min_capture_time;            /* Min capture time (ms)         */
    uint16_t max_capture_time;            /* Max capture time (ms)         */
    uint16_t min_auth_time;               /* Min auth time (ms)            */
    uint16_t max_auth_time;               /* Max auth time (ms)            */
    uint16_t reserved;                    /* Reserved                      */
} fwcmd_moc_stat_info_t;
FWCMD_ELCHECK(moc_stat_info, truly_triggered)
FWCMD_ELCHECK(moc_stat_info, false_triggered)
FWCMD_ELCHECK(moc_stat_info, bad_images)
FWCMD_ELCHECK(moc_stat_info, min_capture_time)
FWCMD_ELCHECK(moc_stat_info, max_capture_time)
FWCMD_ELCHECK(moc_stat_info, min_auth_time)
FWCMD_ELCHECK(moc_stat_info, max_auth_time)
FWCMD_ELCHECK(moc_stat_info, reserved)
FWCMD_OVERALLSIZECHECK(moc_stat_info)

/****************************************************************************/
/* VCSFW_CMD_AUTH_SESSION_RESULT_GET (96 = 0x60)                            */
/****************************************************************************/

/* TLV header structure. */
typedef struct fwcmd_tlv_hdr_s
{
    uint16_t  tagid;        /* Tag ID                                    */
    uint16_t  length;       /* Data length coming after the header.      */
} fwcmd_tlv_hdr_t;
FWCMD_ELCHECK(tlv_hdr, tagid)
FWCMD_ELCHECK(tlv_hdr, length)
FWCMD_OVERALLSIZECHECK(tlv_hdr)

/* COMMAND                                                                  */
typedef struct fwcmd_cmd_auth_session_result_get_s
{
    uint32_t sessionid;     /* Session ID. */
} fwcmd_cmd_auth_session_result_get_t;
FWCMD_ELCHECK(cmd_auth_session_result_get, sessionid)
FWCMD_OVERALLSIZECHECK(cmd_auth_session_result_get)

/* REPLY  (obsolete)                                                        */
typedef struct fwcmd_reply_auth_session_result_get_s
{
    uint32_t usr_objid;       /* Identified user onject ID.    */
    uint32_t score;           /* Match score.                  */
    uint32_t matcher_retcode; /* Matcher return code.          */
    uint32_t pek_size;        /* PEK is appended to the reply. */
    uint16_t template_index;  /* Matched template index.       */
    uint16_t finger_index;    /* Matched finger index.         */
} fwcmd_reply_auth_session_result_get_t;
/* Followed by {PEK} data. */
FWCMD_ELCHECK(reply_auth_session_result_get, usr_objid)
FWCMD_ELCHECK(reply_auth_session_result_get, score)
FWCMD_ELCHECK(reply_auth_session_result_get, matcher_retcode)
FWCMD_ELCHECK(reply_auth_session_result_get, template_index)
FWCMD_ELCHECK(reply_auth_session_result_get, finger_index)
FWCMD_ELCHECK(reply_auth_session_result_get, pek_size)
FWCMD_OVERALLSIZECHECK(reply_auth_session_result_get)

typedef struct fwcmd_reply_auth_session_result_get_ex_s
{
    uint16_t data_size;         /* Size of data to follow   */
} fwcmd_reply_auth_session_result_get_ex_t;
/* The structure is followed by data in a TLV list */
FWCMD_ELCHECK(reply_auth_session_result_get_ex, data_size)
FWCMD_OVERALLSIZECHECK(reply_auth_session_result_get_ex)

typedef struct fwcmd_session_status_timing_info_s
{
    uint32_t feature_extraction;
    uint32_t template_import;
    uint32_t matching;
    uint32_t selected_operation;
} fwcmd_session_status_timing_info_t;
FWCMD_ELCHECK(session_status_timing_info, feature_extraction)
FWCMD_ELCHECK(session_status_timing_info, template_import)
FWCMD_ELCHECK(session_status_timing_info, matching)
FWCMD_ELCHECK(session_status_timing_info, selected_operation)
FWCMD_OVERALLSIZECHECK(session_status_timing_info)

/****************************************************************************/
/* VCSFW_CMD_AUTH_SESSION_DATA_GET (97 = 0x61)                              */
/****************************************************************************/

/* COMMAND                                                                  */
typedef struct fwcmd_cmd_auth_session_data_get_s
{
    uint32_t sessionid;      /* Session ID. */
    uint16_t auth_datatype;  /* aka Authentication algorithm ID - AlgID.  */
    uint16_t param_datasize; /* Param data is appended to the command. This
                              * can be a "Challange" data in case of
                              * Challenge-Response authentication. */
} fwcmd_cmd_auth_session_data_get_t;
/* Folowed by {param_data}. */
FWCMD_ELCHECK(cmd_auth_session_data_get, sessionid)
FWCMD_ELCHECK(cmd_auth_session_data_get, auth_datatype)
FWCMD_ELCHECK(cmd_auth_session_data_get, param_datasize)
FWCMD_OVERALLSIZECHECK(cmd_auth_session_data_get)

/* REPLY                                                                  */
typedef struct fwcmd_reply_auth_session_data_get_s
{
    uint16_t auth_datasize;  /* Authentication data size (payload/token).
                              * The data is  appended to the reply. */
} fwcmd_reply_auth_session_data_get_t;
/* Followed by the {auth_data}. */
FWCMD_ELCHECK(reply_auth_session_data_get, auth_datasize)
FWCMD_OVERALLSIZECHECK(reply_auth_session_data_get)

/* Data structures for authentication results. */

/* Fingerprint ID. The identifier returned after successful identification. */
typedef struct fwcmd_auth_session_data_fid_s
{
    uint8_t userid[16];       /* Unique identifier for the group of templates
                               * (user ID). */
    uint8_t enrollid[15];     /* Unique identifier for matched enrollment
                               * (template). */
    uint8_t finger_index;     /* Index of matched finger. */
} fwcmd_auth_session_data_fid_t;
FWCMD_ELCHECK(auth_session_data_fid, userid)
FWCMD_ELCHECK(auth_session_data_fid, enrollid)
FWCMD_OVERALLSIZECHECK(auth_session_data_fid)

/* Auth data in case of VCSFW_AUTH_ALGID_PKCR algid. */
typedef struct fwcmd_auth_session_data_alg_pkcr_s
{
    fwcmd_auth_session_data_fid_t       id;
    uint32_t                            signature_size;
} fwcmd_auth_session_data_alg_pkcr_t;
FWCMD_ELCHECK(auth_session_data_alg_pkcr, id)
FWCMD_ELCHECK(auth_session_data_alg_pkcr, signature_size)
FWCMD_OVERALLSIZECHECK(auth_session_data_alg_pkcr)
/* Folowed by "signature_size" bytes of signature data.
 * ECDSA signature in DER, ASN.1 format or
 * RSA signature. Data is always in little endian byte format. */

/****************************************************************************/
/* VCSFW_CMD_AUTH_SESSION_FINISH (98 = 0x62)                                */
/****************************************************************************/

/* COMMAND                                                                  */
typedef struct fwcmd_cmd_auth_session_finish_s
{
    uint32_t sessionid;      /* Session ID. */
} fwcmd_cmd_auth_session_finish_t;
FWCMD_ELCHECK(cmd_auth_session_finish, sessionid)
FWCMD_OVERALLSIZECHECK(cmd_auth_session_finish)

/* REPLY                                                                    */
/* vcsfw_generic_reply_t structure is used. */

/****************************************************************************/
/* VCSFW_CMD_TIDLE_SET (87 = 0x57)  */
/****************************************************************************/

/* COMMAND                                                                  */
typedef struct fwcmd_cmd_tidle_set_s{
    uint16_t  idletime; /* time (in units of 80 us) */
    uint16_t  idletime_msw;
} fwcmd_cmd_tidle_set_t;

FWCMD_ELCHECK(cmd_tidle_set, idletime)
FWCMD_ELCHECK(cmd_tidle_set, idletime_msw)
FWCMD_OVERALLSIZECHECK(cmd_tidle_set)

/* REPLY                                                                    */
/* vcsfw_generic_reply_t structure is used. */

/****************************************************************************/
/* VCSFW_CMD_CAL_WOF2 Calibration of WOE_WOF2 (99 = 0x63) */
/****************************************************************************/
typedef struct fwcmd_cmd_cal_wof2_s
{
    uint16_t max_replysize; /* Maximum bytes in reply, based on params */
    uint8_t  max_gain;      /* Highest gain allowed */
    uint8_t  min_gain;      /* Lowest gain allowed */
    uint8_t  start_gain;    /* Starting gain */
    uint8_t  step_gain;     /* Step size for gain */
    uint8_t  max_thresh;    /* Highest threshold allowed */
    uint8_t  min_thresh;    /* Lowest threshold allowed (> 0) */
    uint8_t  polltime;      /* Wake-on-Timer definition (ms) */
    uint8_t  debug;         /* Verbose reply with debug data */
    uint8_t  reserved[2];   /* Padding */
    /*-------- NOTE: equivalent to vcsfw_param_wof2_t -------------- */
    uint32_t wof2_ctrl2;    /* WOE_WOF2_CTRL2 register  */
    uint32_t wof2_thresh;   /* WOE_WOF2_THRESH register */
    uint16_t wof2_ctrl1;    /* WOE_WOF2_CTRL1 register  */
    uint16_t wof2_cfg_size; /* Number of bytes of cfg data follow */
} fwcmd_cmd_cal_wof2_t;
    /* (followed by fwcmd_param_wof2_config_t + binary data) */
    /*-------- NOTE: Maintain equivalence to vcsfw_param_wof2_t ---- */
FWCMD_ELCHECK(cmd_cal_wof2, max_replysize)
FWCMD_ELCHECK(cmd_cal_wof2, max_gain)
FWCMD_ELCHECK(cmd_cal_wof2, min_gain)
FWCMD_ELCHECK(cmd_cal_wof2, start_gain)
FWCMD_ELCHECK(cmd_cal_wof2, step_gain)
FWCMD_ELCHECK(cmd_cal_wof2, max_thresh)
FWCMD_ELCHECK(cmd_cal_wof2, min_thresh)
FWCMD_ELCHECK(cmd_cal_wof2, polltime)
FWCMD_ELCHECK(cmd_cal_wof2, debug)
FWCMD_ELCHECK(cmd_cal_wof2, wof2_ctrl2)
FWCMD_ELCHECK(cmd_cal_wof2, wof2_thresh)
FWCMD_ELCHECK(cmd_cal_wof2, wof2_ctrl1)
FWCMD_ELCHECK(cmd_cal_wof2, wof2_cfg_size)

typedef struct fwcmd_reply_cal_wof2_s
{
    uint32_t   bytes;   /* Number of status bytes returned */
    /* This is followed by the calibration status data. */
} fwcmd_reply_cal_wof2_t;
FWCMD_ELCHECK(reply_cal_wof2, bytes)

/* VCSFW_CMD_CAL_WOF2 Reply data format:
   Bytecount is divisible by 4.
   Byte  Description
   ----  -----------
   0:3   WOE_EVENT Register value
   4:7   WOE_WOF2_STATUS Register value
   8     Current calibration state (start of sweep or end of cal)
   9     Current gain
   10    Current threshold
   11    Reserved
   12:X  Repeat values as in bytes 0:11, for each sweep iteration
   X+1   Final WOE_EVENT Register value
   X+2   Final calibration state
   X+3   Gain result, or current gain if status is non-zero
   X+4   Threshold result, or current threshold if status is non-zero */

/* VCSFW_CMD_CAL_WOF2_V3 Reply data format:
Debug mode: Same as VCSFW_CMD_CAL_WOF2 reply, but use with 1 zone only.
Non-debug mode:
   Bytecount is divisible by 4.
   Byte  Description
   ----  -----------
   0:3   WOE_EVENT Register value, zone 0
   4:7   WOE_WOF2_STATUS Register value, zone 0
   8     Current calibration state (start of sweep or end of cal), zone 0
   9     Current gain, zone 0
   10    Current threshold, zone 0
   11    Reserved
   12:15 WOE_EVENT Register value, zone 1
   16:19 WOE_WOF2_STATUS Register value, zone 1
   20    Current calibration state (start of sweep or end of cal), zone 1
   21    Current gain, zone 1
   22    Current threshold, zone 1
   23    Reserved
NOTE: If only 1 zone is calibrated, the other zone's data will be 0. */
typedef struct fwcmd_cal_wof2_status_s {
    uint32_t event_status;  /* WOE_EVENT register value */
    uint32_t wof2_status;   /* WOE_WOF2_STATUS register value */
    uint8_t current_state;  /* Calibration current state */
    uint8_t current_gain;   /* Gain register field value */
    uint8_t current_thresh; /* Threshold register field value */
    uint8_t count;          /* Calibration iteration count */
} fwcmd_cal_wof2_status_t;
FWCMD_ELCHECK(cal_wof2_status, event_status)
FWCMD_ELCHECK(cal_wof2_status, current_state)
FWCMD_ELCHECK(cal_wof2_status, current_gain)
FWCMD_ELCHECK(cal_wof2_status, current_thresh)
FWCMD_ELCHECK(cal_wof2_status,  count)

/****************************************************************************/
/* VCSFW_CMD_CAL_STATUS_GET (100 = 0x64) Retrieve calibration status */
/****************************************************************************/
/* VCSFW_CMD_CAL_STATUS_GET structure */
typedef struct fwcmd_reply_cal_status_s
{
    uint32_t   bytes;   /* Number of status bytes returned */
    /* This is followed by the calibration status data. */
    /* Instance of fwcmd_cal_wof2_status_t for finger-down calibration */
    /* (Optional) Instance of fwcmd_cal_wof2_status_t for finger-up calibration */
} fwcmd_reply_cal_status_t;
FWCMD_ELCHECK(reply_cal_status, bytes)

/****************************************************************************/
/* VCSFW_CMD_TEST_SPIFLASH_OP (101 = 0x65)                                  */
/****************************************************************************/

/* COMMAND                                                                  */
typedef struct fwcmd_cmd_test_spiflash_op_s
{
    uint8_t  opid;             /* Operation ID (see definitions above)  */
    uint8_t  mode;             /* mode: 0-NODMA 1-DMA                   */
    uint16_t data_size;        /* Read/Prog data size.                  */
    uint32_t address;          /* Flash memory address.                 */
} fwcmd_cmd_test_spiflash_op_t;
/* Followed by 'data_size'-byte data. */
FWCMD_ELCHECK(cmd_test_spiflash_op, opid)
FWCMD_ELCHECK(cmd_test_spiflash_op, mode)
FWCMD_ELCHECK(cmd_test_spiflash_op, data_size)
FWCMD_ELCHECK(cmd_test_spiflash_op, address)
FWCMD_OVERALLSIZECHECK(cmd_test_spiflash_op)

/* REPLY                                                                    */
typedef struct fwcmd_reply_test_spiflash_op_s
{
    uint16_t data_size;        /* Returned back data size.               */
} fwcmd_reply_test_spiflash_op_t;
/* Followed by 'data_size'-byte data. */
FWCMD_ELCHECK(reply_test_spiflash_op, data_size)
FWCMD_OVERALLSIZECHECK(reply_test_spiflash_op)


/****************************************************************************/
/* VCSFW_CMD_ENROLL_SESSION_START   (104 = 0x68)                            */
/****************************************************************************/

/* COMMAND                                                                  */
typedef struct fwcmd_cmd_enroll_session_start_s
{
    uint32_t    sessionid;                  /* 0 for new session, session ID
                                             * for session restore.         */
    uint8_t     flags;
    uint8_t     reserved[3];
} fwcmd_cmd_enroll_session_start_t;

FWCMD_ELCHECK(cmd_enroll_session_start, sessionid)
FWCMD_ELCHECK(cmd_enroll_session_start, flags)
FWCMD_ELCHECK(cmd_enroll_session_start, reserved)
FWCMD_OVERALLSIZECHECK(cmd_enroll_session_start)


/* REPLY                                                                   */
typedef struct fwcmd_reply_enroll_session_start_s
{
    uint32_t sessionid;                     /* Session ID.                 */
} fwcmd_reply_enroll_session_start_t;

FWCMD_ELCHECK(reply_enroll_session_start, sessionid)
FWCMD_OVERALLSIZECHECK(reply_enroll_session_start)

/****************************************************************************/
/* VCSFW_CMD_ENROLL_SESSION_FINISH (105 = 0x69)                             */
/****************************************************************************/

/* COMMAND                                                                  */
typedef struct fwcmd_cmd_enroll_session_finish_s
{
    bool_t session_ctx_erase;
} fwcmd_cmd_enroll_session_finish_t;
FWCMD_ELCHECK(cmd_enroll_session_finish, session_ctx_erase)
FWCMD_OVERALLSIZECHECK(cmd_enroll_session_finish)

/* REPLY                                                                    */
/* Generic reply. */

/****************************************************************************/
/* VCSFW_CMD_TEMPLATE_UPDATE   (107 = 0x6B)                                 */
/****************************************************************************/

/* COMMAND                                                                  */
/* Generic command, followed by the template blob. It is in TLV format.
 * same data received from the host.
 * <VCSFW_TEMPLATE_UPDATE_DATA_TAGID_TEMPLATE><LEN<DATA>. */

 /* TLV header structure. */
typedef struct fwcmd_template_update_tlv_hdr_s
{
    uint16_t  tagid;            /* Tag ID                                    */
    uint16_t  length;           /* Data length coming after the header.      */
    uint16_t  flags;            /* Flags, for FW internal use.               */
    uint16_t  reserved;         /* For alignment.                            */
    uint8_t   iv[VCSFW_SECURITY_IV_SIZE];        /* Encryption IV.           */
    uint8_t   hash[VCSFW_SHA256_HASH_SIZE];      /* Data hash.               */
} fwcmd_template_update_tlv_hdr_t;
/* <DATA> of length "length" comes after */
FWCMD_ELCHECK(template_update_tlv_hdr, tagid)
FWCMD_ELCHECK(template_update_tlv_hdr, length)
FWCMD_ELCHECK(template_update_tlv_hdr, flags)
FWCMD_ELCHECK(template_update_tlv_hdr, reserved)
FWCMD_ELCHECK(template_update_tlv_hdr, iv)
FWCMD_ELCHECK(template_update_tlv_hdr, hash)
FWCMD_OVERALLSIZECHECK(template_update_tlv_hdr)

/* Structure for debuginfo. */
typedef struct fwcmd_session_status_dbg_info_s
{
    uint32_t  wrapper_code;
    uint32_t  matcher_code;
    uint8_t   dbg_info[VCSFW_ENROLL_SESSION_STATUS_DBG_INFO_DATA_SIZE];
} fwcmd_session_status_dbg_info_t;
FWCMD_ELCHECK(session_status_dbg_info, wrapper_code)
FWCMD_ELCHECK(session_status_dbg_info, matcher_code)
FWCMD_ELCHECK(session_status_dbg_info, dbg_info)
FWCMD_OVERALLSIZECHECK(session_status_dbg_info)

/* REPLY                                                                    */
typedef struct fwcmd_reply_template_update_s
{
    uint16_t reply_len;              /* Reply data length.                  */
} fwcmd_reply_template_update_t;
/* Followed by the reply data blob in TLV format. */
/* <TAGID><LEN><DATA>-<TAGID><LEN><DATA>-... */

FWCMD_ELCHECK(reply_template_update, reply_len)
FWCMD_OVERALLSIZECHECK(reply_template_update)

/* COMMAND                                                                  */
/* Generic command, followed by the list of templates. host doesn't need to
 * know the list format. */


/****************************************************************************/
/* VCSFW_CMD_CAL_WOF2_V3 Calibration of WOE_WOF2 (108 = 0x6c) */
/****************************************************************************/
#if !defined(VCSFW_CAL_WOF2_Z0)
#define VCSFW_CAL_WOF2_Z0 0x1  /* Calibrate zone 0 */
#endif
#if !defined(VCSFW_CAL_WOF2_Z1)
#define VCSFW_CAL_WOF2_Z1 0x2  /* Calibrate zone 1 */
#endif
/* VCSFW_CMD_CAL_WOF2_V3 structure */
typedef struct fwcmd_cmd_cal_wof2_v3_s
{
    uint16_t max_replysize; /* Maximum bytes in reply, based on params */
    uint8_t  max_gain;      /* Highest gain allowed */
    uint8_t  min_gain;      /* Lowest gain allowed */
    uint8_t  start_gain;    /* Starting gain */
    uint8_t  step_gain;     /* Step size for gain */
    uint8_t  max_thresh;    /* Highest threshold allowed */
    uint8_t  min_thresh;    /* Lowest threshold allowed (> 0) */
    uint8_t  polltime;      /* Wake-on-Timer definition (ms) */
    uint8_t  debug;         /* Verbose reply with debug data (1 zone only) */
    uint8_t  reserved[2];   /* Padding */
    /*-------- NOTE: equivalent to vcsfw_param_wof2_v3_t -------------- */
    uint32_t wof2_ctrl2;    /* WOE_WOF2_CTRL2 register  */
    uint32_t wof2_thresh;   /* WOE_WOF2_THRESH register, zone 0 */
    uint32_t wof2_threshz1; /* WOE_WOF2_THRESH register, zone 1 */
    uint16_t wof2_ctrl1;    /* WOE_WOF2_CTRL1 register  */
    uint16_t wof2_cfg_size; /* Number of bytes of cfg data follow */
    uint8_t  wof2_flags;    /* Calibration directives */
    uint8_t  reserved1[3];
} fwcmd_cmd_cal_wof2_v3_t;
    /* (followed by fwcmd_param_wof2_config_t + binary data) */
    /*-------- NOTE: Maintain equivalence to vcsfw_param_wof2_t ---- */
FWCMD_ELCHECK(cmd_cal_wof2_v3, max_replysize)
FWCMD_ELCHECK(cmd_cal_wof2_v3, max_gain)
FWCMD_ELCHECK(cmd_cal_wof2_v3, min_gain)
FWCMD_ELCHECK(cmd_cal_wof2_v3, start_gain)
FWCMD_ELCHECK(cmd_cal_wof2_v3, step_gain)
FWCMD_ELCHECK(cmd_cal_wof2_v3, max_thresh)
FWCMD_ELCHECK(cmd_cal_wof2_v3, min_thresh)
FWCMD_ELCHECK(cmd_cal_wof2_v3, polltime)
FWCMD_ELCHECK(cmd_cal_wof2_v3, debug)
FWCMD_ELCHECK(cmd_cal_wof2_v3, wof2_ctrl2)
FWCMD_ELCHECK(cmd_cal_wof2_v3, wof2_thresh)
FWCMD_ELCHECK(cmd_cal_wof2_v3, wof2_ctrl1)
FWCMD_ELCHECK(cmd_cal_wof2_v3, wof2_cfg_size)
FWCMD_ELCHECK(cmd_cal_wof2_v3, wof2_flags)

/****************************************************************************/
/* VCSFW_CMD_SECURE_WRAP (110 = 0x6e)                                       */
/****************************************************************************/

/* COMMAND                                                                  */
typedef struct fwcmd_cmd_secure_wrap_s
{
    uint16_t flags;
    uint16_t data_len;
} fwcmd_cmd_secure_wrap_t;

FWCMD_ELCHECK(cmd_secure_wrap, flags)
FWCMD_ELCHECK(cmd_secure_wrap, data_len)
FWCMD_OVERALLSIZECHECK(cmd_secure_wrap)


/* REPLY                                                                  */
typedef struct fwcmd_reply_secure_wrap_s
{
    uint16_t data_len;
} fwcmd_reply_secure_wrap_t;

FWCMD_ELCHECK(reply_secure_wrap, data_len)
FWCMD_OVERALLSIZECHECK(reply_secure_wrap)

/****************************************************************************/
/* VCSFW_CMD_OTPROM_TAG_FIND (111 = 0x6)                                    */
/****************************************************************************/

/* Command structure */
typedef struct fwcmd_cmd_otprom_tag_find_s
{
    uint8_t      flags;
    uint8_t      tag;
    uint8_t      maxntags;
    uint8_t      firstidx;
    uint32_t     extended_tag;
} fwcmd_cmd_otprom_tag_find_t;

FWCMD_ELCHECK(cmd_otprom_tag_find, flags)
FWCMD_ELCHECK(cmd_otprom_tag_find, tag)
FWCMD_ELCHECK(cmd_otprom_tag_find, maxntags)
FWCMD_ELCHECK(cmd_otprom_tag_find, firstidx)
FWCMD_ELCHECK(cmd_otprom_tag_find, extended_tag)
FWCMD_OVERALLSIZECHECK(cmd_otprom_tag_find)

/* Reply */
typedef struct fwcmd_reply_otprom_tag_find_s
{
    uint32_t     freespace;
    uint32_t     ntags;
} fwcmd_reply_otprom_tag_find_t;

FWCMD_ELCHECK(reply_otprom_tag_find, freespace)
FWCMD_ELCHECK(reply_otprom_tag_find, ntags)
FWCMD_OVERALLSIZECHECK(reply_otprom_tag_find)

/* Header on each tag */
typedef struct fwcmd_reply_otprom_tag_find_header_s
{
    uint32_t     offset;
    uint16_t     length;
    uint8_t      tag;
    uint8_t      unused;
    uint32_t     extended_tag;
} fwcmd_reply_otprom_tag_find_header_t;


FWCMD_ELCHECK(reply_otprom_tag_find_header, offset)
FWCMD_ELCHECK(reply_otprom_tag_find_header, length)
FWCMD_ELCHECK(reply_otprom_tag_find_header, tag)
FWCMD_ELCHECK(reply_otprom_tag_find_header, unused)
FWCMD_ELCHECK(reply_otprom_tag_find_header, extended_tag)
FWCMD_OVERALLSIZECHECK(reply_otprom_tag_find_header)

/****************************************************************************/
/* Definitions regarding VCSFW_STORAGE_PART_PRINTCFG flash partition        */
/* (Metallica).                                                             */
/* Data in this partition is stored in                                      */
/* <2 byte TAGID><2 byte LEN><32byte HASH><data> format.                    */
/****************************************************************************/

/* The entry header. */
typedef struct fwcmd_storage_part_printcfg_entry_hdr_s
{
    uint16_t tagid;
                                    /* Entry type - tag ID.                 */
    uint16_t len;
                                    /* Data length coming after the header. */
    uint8_t  hash[VCSFW_SHA256_HASH_SIZE];
                                    /* Data hash coming after the header.   */
    uint8_t  mac[VCSFW_SHA256_HASH_SIZE];
                                    /* Data MAC coming after the header.
                                     * Maintained exclusively by firmware.  */
} fwcmd_storage_part_printcfg_entry_hdr_t;
/* Followed by the data of <len> length.
 * GetPrint command is in <4 byte length>+<data> format. */

FWCMD_ELCHECK(storage_part_printcfg_entry_hdr, tagid)
FWCMD_ELCHECK(storage_part_printcfg_entry_hdr, len)
FWCMD_ELCHECK(storage_part_printcfg_entry_hdr, hash)
FWCMD_ELCHECK(storage_part_printcfg_entry_hdr, mac)
FWCMD_OVERALLSIZECHECK(storage_part_printcfg_entry_hdr)

/****************************************************************************/
/* Definitions regarding VCSFW_STORAGE_PART_SYSTEMCFG flash partition        */
/* (Metallica).                                                             */
/* Data in this partition is stored in                                      */
/* <2 byte TAGID><2 byte LEN><32byte HASH><data> format.                    */
/****************************************************************************/

/* The entry header. */
typedef struct fwcmd_storage_part_systemcfg_entry_hdr_s
{
    uint16_t tagid;
                                    /* Entry type - tag ID.                 */
    uint16_t len;
                                    /* Data length coming after the header. */
    uint8_t  hash[VCSFW_SHA256_HASH_SIZE];
                                    /* Data hash coming after the header.   */
    uint8_t  mac[VCSFW_SHA256_HASH_SIZE];
                                    /* Data MAC coming after the header.
                                     * Maintained exclusively by firmware.  */
} fwcmd_storage_part_systemcfg_entry_hdr_t;
/* Followed by the data of <len> length.
 * GetPrint command is in <4 byte length>+<data> format. */

FWCMD_ELCHECK(storage_part_systemcfg_entry_hdr, tagid)
FWCMD_ELCHECK(storage_part_systemcfg_entry_hdr, len)
FWCMD_ELCHECK(storage_part_systemcfg_entry_hdr, hash)
FWCMD_ELCHECK(storage_part_systemcfg_entry_hdr, mac)
FWCMD_OVERALLSIZECHECK(storage_part_systemcfg_entry_hdr)

/****************************************************************************/
/* VCSFW_CMD_IFS_STATS_GET (114 = 0x72)                                    */
/****************************************************************************/

typedef struct fwcmd_reply_framevarstats_s
{
    uint16_t thresh;         /* threshold used */
    uint16_t margin;         /* margin around the threshold */
    uint16_t totalLines;     /* total number of lines */
    uint16_t aboveAvgCount;  /* number of lines above the threshold */
    uint16_t belowAvgCount;  /* number of lines below the threshold */
    uint16_t avgVariance;    /* average line variance */
    uint16_t lineVariance[64];   /* line variance data */
} fwcmd_reply_framevarstats_t;

FWCMD_ELCHECK(reply_framevarstats, thresh)
FWCMD_ELCHECK(reply_framevarstats, margin)
FWCMD_ELCHECK(reply_framevarstats, totalLines)
FWCMD_ELCHECK(reply_framevarstats, aboveAvgCount)
FWCMD_ELCHECK(reply_framevarstats, belowAvgCount)
FWCMD_ELCHECK(reply_framevarstats, avgVariance)
FWCMD_ELCHECK(reply_framevarstats, lineVariance)
FWCMD_OVERALLSIZECHECK(reply_framevarstats)


typedef struct fwcmd_reply_ifs_stats_get_s
{
    uint16_t             framesCaptured;         /* number of frames captured */
    uint16_t             frameWhenFirstSettled;  /* number of frames captured when finger first seems settled */
    uint16_t             framesSelectedCount;    /* number of frames captured */
    uint16_t             framesBestCount;        /* number of frames captured when found best quality one found */
    uint16_t             frameRows;              /* number of rows in each frame */
    uint16_t             frameCols;              /* number of cols in each frame */
    uint8_t              headerLen;              /* size of the line header */
    uint8_t              frameSelectStatus;      /* status of frame selection */
    uint8_t              lastSettledFrame;       /* frame number of last one that was settled */
    uint8_t              empty;

    fwcmd_reply_framevarstats_t   previousFrameVarStats;  /* line variance stats for the previous frame */
    fwcmd_reply_framevarstats_t   lastSelectedFrameVarStats;
    uint32_t             lastAvgLineDelta;       /* average line delta of the previous frame */
    uint32_t             bestFrameAvgVariance;   /* average variance of the best frame stored */
} fwcmd_reply_ifs_stats_get_t;


FWCMD_ELCHECK(reply_ifs_stats_get, framesCaptured)
FWCMD_ELCHECK(reply_ifs_stats_get, frameWhenFirstSettled)
FWCMD_ELCHECK(reply_ifs_stats_get, framesSelectedCount)
FWCMD_ELCHECK(reply_ifs_stats_get, framesBestCount)
FWCMD_ELCHECK(reply_ifs_stats_get, frameRows)
FWCMD_ELCHECK(reply_ifs_stats_get, frameCols)
FWCMD_ELCHECK(reply_ifs_stats_get, headerLen)
FWCMD_ELCHECK(reply_ifs_stats_get, frameSelectStatus)
FWCMD_ELCHECK(reply_ifs_stats_get, lastSettledFrame)
FWCMD_ELCHECK(reply_ifs_stats_get, empty)
FWCMD_ELCHECK(reply_ifs_stats_get, previousFrameVarStats)
FWCMD_ELCHECK(reply_ifs_stats_get, lastSelectedFrameVarStats)
FWCMD_ELCHECK(reply_ifs_stats_get, lastAvgLineDelta)
FWCMD_ELCHECK(reply_ifs_stats_get, bestFrameAvgVariance)
FWCMD_OVERALLSIZECHECK(reply_ifs_stats_get)

/****************************************************************************/
/* VCSFW_CMD_APPLICATION_REQUEST (115 = 0x73)                               */
/****************************************************************************/

/* Command structure */
typedef struct fwcmd_cmd_application_request_s
{
    uint16_t app_id;
    uint16_t request_len;
} fwcmd_cmd_application_request_t;

FWCMD_ELCHECK(cmd_application_request, app_id)
FWCMD_ELCHECK(cmd_application_request, request_len)
FWCMD_OVERALLSIZECHECK(cmd_application_request)

/* Reply */
typedef struct fwcmd_reply_application_request_s
{
    uint16_t     response_len;
} fwcmd_reply_application_request_t;

FWCMD_ELCHECK(reply_application_request, response_len)
FWCMD_OVERALLSIZECHECK(reply_application_request)

/****************************************************************************/
/* VCSFW_CMD_STORAGE_FWPART_INFO_GET_V2 (116 = 0x74)                        */
/****************************************************************************/

/* Command */
typedef struct fwcmd_cmd_storage_fwpart_info_get_v2_s
{
    uint8_t partid;
} fwcmd_cmd_storage_fwpart_info_get_v2_t;
FWCMD_ELCHECK(cmd_storage_fwpart_info_get_v2, partid)
FWCMD_OVERALLSIZECHECK(cmd_storage_fwpart_info_get_v2)

/* Reply */
typedef struct fwcmd_reply_storage_fwpart_info_get_v2_s
{
    uint8_t      majorver;
    uint8_t      minorver;
    uint16_t     buildnum;
    uint32_t     buildtime;
    uint32_t     romid;
    uint32_t     fwstart;
    uint32_t     fwsize;
    uint32_t     datasize;
    uint32_t     unused[3];
    uint16_t     type;
    uint16_t     nmodules;
} fwcmd_reply_storage_fwpart_info_get_v2_t;

FWCMD_ELCHECK(reply_storage_fwpart_info_get_v2, majorver)
FWCMD_ELCHECK(reply_storage_fwpart_info_get_v2, minorver)
FWCMD_ELCHECK(reply_storage_fwpart_info_get_v2, buildnum)
FWCMD_ELCHECK(reply_storage_fwpart_info_get_v2, buildtime)
FWCMD_ELCHECK(reply_storage_fwpart_info_get_v2, romid)
FWCMD_ELCHECK(reply_storage_fwpart_info_get_v2, fwstart)
FWCMD_ELCHECK(reply_storage_fwpart_info_get_v2, fwsize)
FWCMD_ELCHECK(reply_storage_fwpart_info_get_v2, datasize)
FWCMD_ELCHECK(reply_storage_fwpart_info_get_v2, unused)
FWCMD_ELCHECK(reply_storage_fwpart_info_get_v2, type)
FWCMD_ELCHECK(reply_storage_fwpart_info_get_v2, nmodules)
FWCMD_OVERALLSIZECHECK(reply_storage_fwpart_info_get_v2)

typedef struct fwcmd_storage_fwpart_module_v2_s
{
    uint32_t     flags;
    uint32_t     refcount;
    uint32_t     size;
    uint32_t     unused[2];
    uint16_t     type;
    uint16_t     subtype;
    uint16_t     dynsize;
    uint16_t     bsslen;
    uint16_t     datalen;
    uint8_t      majorver;
    uint8_t      minorver;
} fwcmd_storage_fwpart_module_v2_t;

FWCMD_ELCHECK(storage_fwpart_module_v2, flags)
FWCMD_ELCHECK(storage_fwpart_module_v2, refcount)
FWCMD_ELCHECK(storage_fwpart_module_v2, size)
FWCMD_ELCHECK(storage_fwpart_module_v2, unused)
FWCMD_ELCHECK(storage_fwpart_module_v2, type)
FWCMD_ELCHECK(storage_fwpart_module_v2, subtype)
FWCMD_ELCHECK(storage_fwpart_module_v2, dynsize)
FWCMD_ELCHECK(storage_fwpart_module_v2, bsslen)
FWCMD_ELCHECK(storage_fwpart_module_v2, datalen)
FWCMD_ELCHECK(storage_fwpart_module_v2, majorver)
FWCMD_ELCHECK(storage_fwpart_module_v2, minorver)
FWCMD_OVERALLSIZECHECK(storage_fwpart_module_v2)

/****************************************************************************/
/* VCSFW_CMD_CUSTID_GET (117 = 0x75)                                        */
/****************************************************************************/

/* Command -- no arguments */

/* Reply */
typedef struct fwcmd_reply_custid_get_s {
    uint32_t     custid;
    uint32_t     projid;
} fwcmd_reply_custid_get_t;

FWCMD_ELCHECK(reply_custid_get, custid)
FWCMD_ELCHECK(reply_custid_get, projid)
FWCMD_OVERALLSIZECHECK(reply_custid_get)

/****************************************************************************/
/* VCSFW_CMD_WOF2_CONFIGURE (118 = 0x76)                                    */
/****************************************************************************/

/* COMMAND                                                                  */
/*   - Uses vcsfw_generic_command_t                                         */
typedef struct fwcmd_cmd_wof2_config_s {
    uint8_t              mode;           /* finger detection mode */
    uint8_t              flags;
    uint8_t              reserved[2];
    fwcmd_param_wof2_v3_t   wof2_param;     /* WOF2 parameters */
} fwcmd_cmd_wof2_config_t;

FWCMD_ELCHECK(cmd_wof2_config, mode)
FWCMD_ELCHECK(cmd_wof2_config, flags)
FWCMD_ELCHECK(cmd_wof2_config, wof2_param)
FWCMD_OVERALLSIZECHECK(cmd_wof2_config)

/* REPLY                                                                    */
/*   - Uses vcsfw_generic_reply_t                                           */


/****************************************************************************/
/* VCSFW_CMD_WOF2_STATUS_GET (118 = x076)                                   */
/****************************************************************************/

/* COMMAND                                                                  */
/*   - Uses vcsfw_generic_command_t                                         */

/* REPLY                                                                    */
typedef struct fwcmd_reply_wof2_status_s {
    uint8_t         mode;       /* finger detection mode */
    uint8_t         fstate;     /* finger state */
    uint8_t         event;      /* FU/FD events */
    uint8_t         reserved;
    uint16_t        fd_count;   /* finger down sample count */
    uint16_t        fu_count;   /* finger up sample count */
} fwcmd_reply_wof2_status_t;

FWCMD_ELCHECK(reply_wof2_status, fstate)
FWCMD_ELCHECK(reply_wof2_status, mode)
FWCMD_ELCHECK(reply_wof2_status, event)
FWCMD_ELCHECK(reply_wof2_status, fd_count)
FWCMD_ELCHECK(reply_wof2_status, fu_count)
FWCMD_OVERALLSIZECHECK(reply_wof2_status)

/****************************************************************************/
/* VCSFW_CMD_READ_RAW_FPDATA (120 = 0x78)                                   */
/****************************************************************************/

typedef struct fwcmd_cmd_read_raw_fpdata_s
{
    uint32_t bufsize;    /* host buffer size */
} fwcmd_cmd_read_raw_fpdata_t;

FWCMD_ELCHECK(cmd_read_raw_fpdata, bufsize)
FWCMD_OVERALLSIZECHECK(cmd_read_raw_fpdata)

typedef struct fwcmd_reply_read_raw_fpdata_s
{
    uint32_t datasize;   /* amount of data returned */
} fwcmd_reply_read_raw_fpdata_t;

FWCMD_ELCHECK(reply_read_raw_fpdata, datasize)
FWCMD_OVERALLSIZECHECK(reply_read_raw_fpdata)

/****************************************************************************/
/* VCSFW_CMD_TEMPERATURE_READ reply (122 = 0x7A)                            */
/****************************************************************************/

typedef struct fwcmd_reply_temperature_read_s
{
    uint16_t   adccount;   /* ADC counts from sensing register */
    uint16_t   loopcount;  /* Debug: Number of iterations before settling */
} fwcmd_reply_temperature_read_t;
FWCMD_OVERALLSIZECHECK(reply_temperature_read)

/****************************************************************************/
/* VCSFW_CMD_AUTORESTART_STAT_GET (123 = x07B)                              */
/****************************************************************************/

/* REPLY                                                                    */
typedef struct fwcmd_reply_autorestart_stat_get_s {
    uint8_t         ff_count;       /* false finger count */
    uint8_t         reserved[3];
} fwcmd_reply_autorestart_stat_get_t;

FWCMD_ELCHECK(reply_autorestart_stat_get, ff_count)
FWCMD_OVERALLSIZECHECK(reply_autorestart_stat_get)

/****************************************************************************/
/* VCSFW_CMD_ECHO (124 = 0x7c)                                              */
/****************************************************************************/

/****************************************************************************/
/* VCSFW_CMD_BOOTLDR_PATCH (125 = 0x7d)                                     */
/****************************************************************************/

/* This header is always transmitted in plaintext */
typedef struct fwcmd_cmd_bootldr_patch_plainhdr_s {
    uint32_t     flags;
    uint32_t     id;
    uint32_t     buildnum;
    uint32_t     payload_id;
    uint32_t     payload_buildtime;
    uint32_t     payload_buildnum;
    uint8_t      payload_vmajor;
    uint8_t      payload_vminor;
    uint8_t      payload_target;
    uint8_t      payload_product;
    uint8_t      payload_siliconrev;
    uint8_t      payload_formalrel;
    uint8_t      seq;
    uint8_t      vmajor;
    uint8_t      vminor;
    uint8_t      unused[3];
} fwcmd_cmd_bootldr_patch_plainhdr_t;

FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr, flags)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr, id)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr, buildnum)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr, payload_id)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr, payload_buildtime)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr, payload_buildnum)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr, payload_vmajor)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr, payload_vminor)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr, payload_target)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr, payload_product)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr, payload_siliconrev)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr, payload_formalrel)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr, seq)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr, vmajor)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr, vminor)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr, unused)
FWCMD_OVERALLSIZECHECK(cmd_bootldr_patch_plainhdr)

/* This header is always transmitted in plaintext */
typedef struct fwcmd_cmd_bootldr_patch_plainhdr_type1_s {
    uint32_t     flags;
    uint32_t     id;
    uint32_t     buildnum;
    uint32_t     restrict_fwflavor;
    uint32_t     payload_buildtime;
    uint32_t     payload_buildnum;
    uint8_t      payload_vmajor;
    uint8_t      payload_vminor;
    uint8_t      restrict_seclevel;
    uint8_t      padding[3];
    uint8_t      seq;
    uint8_t      vmajor;
    uint8_t      vminor;
    uint8_t      unused[3];
} fwcmd_cmd_bootldr_patch_plainhdr_type1_t;
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr_type1, flags)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr_type1, id)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr_type1, buildnum)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr_type1, restrict_fwflavor)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr_type1, payload_buildtime)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr_type1, payload_buildnum)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr_type1, payload_vmajor)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr_type1, payload_vminor)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr_type1, restrict_seclevel)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr_type1, padding)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr_type1, seq)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr_type1, vmajor)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr_type1, vminor)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr_type1, unused)
FWCMD_OVERALLSIZECHECK(cmd_bootldr_patch_plainhdr_type1)

/* This header is always transmitted in plaintext */
typedef struct fwcmd_cmd_bootldr_patch_plainhdr_type2_s {
    uint32_t     flags;
    uint32_t     id;
    uint32_t     buildnum;
    uint32_t     restrict_configid1;
    uint32_t     restrict_configid2;
    uint16_t     restrict_configver;
    uint8_t      padding[8];
    uint8_t      seq;
    uint8_t      vmajor;
    uint8_t      vminor;
    uint8_t      unused[3];
} fwcmd_cmd_bootldr_patch_plainhdr_type2_t;
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr_type2, flags)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr_type2, id)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr_type2, buildnum)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr_type2, restrict_configid1)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr_type2, restrict_configid2)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr_type2, restrict_configver)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr_type2, padding)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr_type2, seq)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr_type2, vmajor)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr_type2, vminor)
FWCMD_ELCHECK(cmd_bootldr_patch_plainhdr_type2, unused)
FWCMD_OVERALLSIZECHECK(cmd_bootldr_patch_plainhdr_type2)

/* This header is sometimes transmitted in an encrypted form. */
typedef struct fwcmd_cmd_bootldr_patch_enchdr_s {
    uint32_t     buildtime;
    uint32_t     loadaddr;
    uint32_t     startaddr;
    uint32_t     bssaddr;
    uint32_t     bsslen;
} fwcmd_cmd_bootldr_patch_enchdr_t;

FWCMD_ELCHECK(cmd_bootldr_patch_enchdr, buildtime)
FWCMD_ELCHECK(cmd_bootldr_patch_enchdr, loadaddr)
FWCMD_ELCHECK(cmd_bootldr_patch_enchdr, startaddr)
FWCMD_ELCHECK(cmd_bootldr_patch_enchdr, bssaddr)
FWCMD_ELCHECK(cmd_bootldr_patch_enchdr, bsslen)
FWCMD_OVERALLSIZECHECK(cmd_bootldr_patch_enchdr)


/****************************************************************************/
/* VCSFW_CMD_GET_NAV_STATUS_V2 (126 = 0x7E)                                 */
/****************************************************************************/

/* COMMAND                                                                  */
/*   - Uses vcsfw_generic_command_t                                         */

/* REPLY                                                                    */
typedef struct fwcmd_reply_get_nav_status_v2_s
{
    uint8_t      statusCount;
    uint8_t      reserved[3];
} fwcmd_reply_get_nav_status_v2_t;

FWCMD_ELCHECK(reply_get_nav_status_v2, statusCount)
FWCMD_OVERALLSIZECHECK(reply_get_nav_status_v2)

/* Optionally (when statusCount is non-zero), above reply bytes will be
followed by statusCount * 12 bytes. Each of those 12 bytes will follow
below structure */
typedef struct fwcmd_nav_status_info_s
{
    uint8_t      status; /* finger status */
    uint8_t      reserved;
    int16_t      posdelta_x;
    int16_t      negdelta_x;
    int16_t      posdelta_y;
    int16_t      negdelta_y;
    uint16_t     timestamp;
} fwcmd_nav_status_info_t;

FWCMD_ELCHECK(nav_status_info, status)
FWCMD_ELCHECK(nav_status_info, posdelta_x)
FWCMD_ELCHECK(nav_status_info, negdelta_x)
FWCMD_ELCHECK(nav_status_info, posdelta_y)
FWCMD_ELCHECK(nav_status_info, negdelta_y)
FWCMD_ELCHECK(nav_status_info, timestamp)
FWCMD_OVERALLSIZECHECK(nav_status_info)

/****************************************************************************/
/* VCSFW_CMD_FRAME_READ (127 = 0x7f)                                        */
/****************************************************************************/
typedef struct fwcmd_cmd_frame_read_s {
    uint16_t  xfernum;
    uint16_t  offset;
    uint16_t  nbytes;
    uint16_t  flags;
} fwcmd_cmd_frame_read_t;

FWCMD_ELCHECK(cmd_frame_read, xfernum)
FWCMD_ELCHECK(cmd_frame_read, offset)
FWCMD_ELCHECK(cmd_frame_read, nbytes)
FWCMD_ELCHECK(cmd_frame_read, flags)
FWCMD_OVERALLSIZECHECK(cmd_frame_read)

typedef struct fwcmd_reply_frame_read_s {
    uint16_t  flags;
    uint16_t  xfernum;
    uint16_t  acqnum; /* number of frame as received from SCM */
    uint16_t  validbytes;
} fwcmd_reply_frame_read_t;

FWCMD_ELCHECK(reply_frame_read, flags)
FWCMD_ELCHECK(reply_frame_read, xfernum)
FWCMD_ELCHECK(reply_frame_read, acqnum)
FWCMD_ELCHECK(reply_frame_read, validbytes)
FWCMD_OVERALLSIZECHECK(reply_frame_read)

/****************************************************************************/
/* VCSFW_CMD_FRAME_ACQ (128 = 0x80)                                         */
/****************************************************************************/
typedef struct fwcmd_cmd_frame_acq_s {
    uint32_t flags;
    uint32_t nframes;  /* 0 means that no frame is to be acquired. */
} fwcmd_cmd_frame_acq_t;

FWCMD_ELCHECK(cmd_frame_acq, flags)
FWCMD_ELCHECK(cmd_frame_acq, nframes)
FWCMD_OVERALLSIZECHECK(cmd_frame_acq)

/* Reply from VCSFW_CMD_FRAME_ACQ */
typedef struct fwcmd_reply_frame_acq_s {
    uint32_t     pixpitch;
    uint32_t     linepitch;
    uint32_t     bitinfo;
    uint16_t     pixperline;
    uint16_t     linesperframe;
} fwcmd_reply_frame_acq_t;

FWCMD_ELCHECK(reply_frame_acq, pixpitch)
FWCMD_ELCHECK(reply_frame_acq, linepitch)
FWCMD_ELCHECK(reply_frame_acq, bitinfo)
FWCMD_ELCHECK(reply_frame_acq, pixperline)
FWCMD_ELCHECK(reply_frame_acq, linesperframe)
FWCMD_OVERALLSIZECHECK(reply_frame_acq)


/* VCSFW_FRAME_TAG */
typedef struct fwcmd_frame_tag_s {
    uint16_t   nwords;    /* number of 32-bit to follow */
    uint8_t    flags;
    uint8_t    tagid;
} fwcmd_frame_tag_t;

FWCMD_ELCHECK(frame_tag, nwords)
FWCMD_ELCHECK(frame_tag, flags)
FWCMD_ELCHECK(frame_tag, tagid)
FWCMD_OVERALLSIZECHECK(frame_tag)

/* VCSFW_FRAME_TAG_REG32BLK */
typedef struct fwcmd_frame_tag_reg32blk_s {
    uint16_t  regbase;
    uint16_t  nregs;
} fwcmd_frame_tag_reg32blk_t;

FWCMD_ELCHECK(frame_tag_reg32blk, regbase)
FWCMD_ELCHECK(frame_tag_reg32blk, nregs)
FWCMD_OVERALLSIZECHECK(frame_tag_reg32blk)


/* VCSFW_FRAME_TAG_REG16BLK */
typedef struct fwcmd_frame_tag_reg16blk_s {
    uint16_t  regbase;
    uint16_t  nregs;
} fwcmd_frame_tag_reg16blk_t;

FWCMD_ELCHECK(frame_tag_reg16blk, regbase)
FWCMD_ELCHECK(frame_tag_reg16blk, nregs)
FWCMD_OVERALLSIZECHECK(frame_tag_reg16blk)

/* VCSFW_FRAME_TAG_DIMS */
typedef struct fwcmd_frame_tag_dims_s {
    uint16_t     bitdepth;
    uint16_t     frame_nlines;
    uint16_t     frame_header;
    uint16_t     frame_lineskip;
    uint16_t     frame_linesusable;
    uint16_t     line_npix;
    uint16_t     line_header;
    uint16_t     line_pixskip;
    uint16_t     line_pixusable;
    uint16_t     unused;
} fwcmd_frame_tag_dims_t;

FWCMD_ELCHECK(frame_tag_dims, bitdepth)
FWCMD_ELCHECK(frame_tag_dims, frame_nlines)
FWCMD_ELCHECK(frame_tag_dims, frame_header)
FWCMD_ELCHECK(frame_tag_dims, frame_lineskip)
FWCMD_ELCHECK(frame_tag_dims, frame_linesusable)
FWCMD_ELCHECK(frame_tag_dims, line_npix)
FWCMD_ELCHECK(frame_tag_dims, line_header)
FWCMD_ELCHECK(frame_tag_dims, line_pixskip)
FWCMD_ELCHECK(frame_tag_dims, line_pixusable)
FWCMD_ELCHECK(frame_tag_dims, unused)
FWCMD_OVERALLSIZECHECK(frame_tag_dims)



/* VCSFW_FRAME_TAG_ACQOPT */
typedef struct fwcmd_frame_tag_acqopt_s {
    uint8_t      mode;
    uint8_t      trigger;
    uint8_t      filter;
    uint8_t      trigger_event;
} fwcmd_frame_tag_acqopt_t;

FWCMD_ELCHECK(frame_tag_acqopt, mode)
FWCMD_ELCHECK(frame_tag_acqopt, trigger)
FWCMD_ELCHECK(frame_tag_acqopt, filter)
FWCMD_ELCHECK(frame_tag_acqopt, trigger_event)
FWCMD_OVERALLSIZECHECK(frame_tag_acqopt)

/* VCSFW_FRAME_TAG_ACQCFG */
typedef struct fwcmd_frame_tag_acqcfg_s {
    uint16_t     min_interframe_gap_ms;
    uint8_t      max_buffered_frames;
    uint8_t      unused;
} fwcmd_frame_tag_acqcfg_t;

FWCMD_ELCHECK(frame_tag_acqcfg, min_interframe_gap_ms)
FWCMD_ELCHECK(frame_tag_acqcfg, max_buffered_frames)
FWCMD_ELCHECK(frame_tag_acqcfg, unused)
FWCMD_OVERALLSIZECHECK(frame_tag_acqcfg)


/* VCSFW_FRAME_TAG_XSREG8BLK */
typedef struct fwcmd_frame_tag_xsreg8blk_s {
    uint32_t     flags;
} fwcmd_frame_tag_xsreg8blk_t;

FWCMD_ELCHECK(frame_tag_xsreg8blk, flags)
FWCMD_OVERALLSIZECHECK(frame_tag_xsreg8blk)

/* VCSFW_FRAME_TAG_STRIDEBLK */
typedef struct fwcmd_frame_tag_strideblk_s {
    uint32_t    regbase;
    uint16_t    nregs;
    uint16_t    stride;
    uint32_t    val;
} fwcmd_frame_tag_strideblk_t;

FWCMD_ELCHECK(frame_tag_strideblk, regbase)
FWCMD_ELCHECK(frame_tag_strideblk, nregs)
FWCMD_ELCHECK(frame_tag_strideblk, stride)
FWCMD_ELCHECK(frame_tag_strideblk, val)
FWCMD_OVERALLSIZECHECK(frame_tag_strideblk)

/* VCSFW_FRAME_TAG_STATS */
typedef struct fwcmd_frame_tag_stats_s {
    uint32_t    types;
    uint16_t    flags;
    uint16_t    skipnframes;
} fwcmd_frame_tag_stats_t;

FWCMD_ELCHECK(frame_tag_stats, types)
FWCMD_ELCHECK(frame_tag_stats, flags)
FWCMD_ELCHECK(frame_tag_stats, skipnframes)
FWCMD_OVERALLSIZECHECK(frame_tag_stats)

/* VCSFW_FRAME_TAG_ORIENTPITCH */
typedef struct fwcmd_frame_tag_orientpitch_s {
    uint32_t        pixpitch;
    uint32_t        linepitch;
    uint8_t         orient;
    uint8_t         unused[3];
} fwcmd_frame_tag_orientpitch_t;

FWCMD_ELCHECK(frame_tag_orientpitch, pixpitch)
FWCMD_ELCHECK(frame_tag_orientpitch, linepitch)
FWCMD_ELCHECK(frame_tag_orientpitch, orient)
FWCMD_ELCHECK(frame_tag_orientpitch, unused)
FWCMD_OVERALLSIZECHECK(frame_tag_orientpitch)

/* VCSFW_FRAME_TAG_IMGPROC_CROP */
typedef struct fwcmd_frame_tag_imgproc_crop_s {
    uint16_t        top;
    uint16_t        bottom;
    uint16_t        left;
    uint16_t        right;
} fwcmd_frame_tag_imgproc_crop_t;

FWCMD_ELCHECK(frame_tag_imgproc_crop, top)
FWCMD_ELCHECK(frame_tag_imgproc_crop, bottom)
FWCMD_ELCHECK(frame_tag_imgproc_crop, left)
FWCMD_ELCHECK(frame_tag_imgproc_crop, right)
FWCMD_OVERALLSIZECHECK(frame_tag_imgproc_crop)

/* VCSFW_FRAME_TAG_IMGPROC_DYNBPDET */
typedef struct fwcmd_frame_tag_imgproc_dynbpdet_s {
    uint16_t        threshold;
} fwcmd_frame_tag_imgproc_dynbpdet_t;

FWCMD_ELCHECK(frame_tag_imgproc_dynbpdet, threshold)
FWCMD_OVERALLSIZECHECK(frame_tag_imgproc_dynbpdet)

/* VCSFW_FRAME_TAG_IMGPROC_3X3SEPSQ */
typedef struct fwcmd_frame_tag_imgproc_3x3sepsq_s {
     uint16_t       vector[3];
     uint16_t       normal;
} fwcmd_frame_tag_imgproc_3x3sepsq_t;

FWCMD_ELCHECK(frame_tag_imgproc_3x3sepsq, vector)
FWCMD_ELCHECK(frame_tag_imgproc_3x3sepsq, normal)
FWCMD_OVERALLSIZECHECK(frame_tag_imgproc_3x3sepsq)

/* VCSFW_FRAME_TAG_IMGPROC_DISABLE */
typedef struct fwcmd_frame_tag_imgproc_disable_s {
    uint32_t        filters;
} fwcmd_frame_tag_imgproc_disable_t;

FWCMD_ELCHECK(frame_tag_imgproc_disable, filters)
FWCMD_OVERALLSIZECHECK(frame_tag_imgproc_disable)

/* VCSFW_FRAME_TAG_EXTFPS_OTP_TEMPCORRECT */
typedef struct fwcmd_frame_tag_extfps_otp_tempcorrect_s {
    uint16_t     tempcorrect60;
    uint16_t     tempcorrect25;
} fwcmd_frame_tag_extfps_otp_tempcorrect_t;

FWCMD_ELCHECK(frame_tag_extfps_otp_tempcorrect, tempcorrect60)
FWCMD_ELCHECK(frame_tag_extfps_otp_tempcorrect, tempcorrect25)
FWCMD_OVERALLSIZECHECK(frame_tag_extfps_otp_tempcorrect)

/* VCSFW_FRAME_TAG_EXTFPS_OTP_DIEIMPRINT */
typedef struct fwcmd_frame_tag_extfps_otp_dieimprint_s {
    uint8_t       waferlot[8];
    uint8_t       xcoord;
    uint8_t       ycoord;
    uint8_t       fabno;
    uint8_t       waferno;
} fwcmd_frame_tag_extfps_otp_dieimprint_t;

FWCMD_ELCHECK(frame_tag_extfps_otp_dieimprint, waferlot)
FWCMD_ELCHECK(frame_tag_extfps_otp_dieimprint, xcoord)
FWCMD_ELCHECK(frame_tag_extfps_otp_dieimprint, ycoord)
FWCMD_ELCHECK(frame_tag_extfps_otp_dieimprint, fabno)
FWCMD_ELCHECK(frame_tag_extfps_otp_dieimprint, waferno)
FWCMD_OVERALLSIZECHECK(frame_tag_extfps_otp_dieimprint)

/* VCSFW_FRAME_TAG_EXTFPS_SIGCTRL */

typedef struct fwcmd_frame_tag_extfps_sigctrl_s {
    uint8_t      hbm;
    uint8_t      hbmreq;
    uint8_t      wake;
    uint8_t      unused;
} fwcmd_frame_tag_extfps_sigctrl_t;

FWCMD_ELCHECK(frame_tag_extfps_sigctrl, hbm)
FWCMD_ELCHECK(frame_tag_extfps_sigctrl, hbmreq)
FWCMD_ELCHECK(frame_tag_extfps_sigctrl, wake)
FWCMD_ELCHECK(frame_tag_extfps_sigctrl, unused)
FWCMD_OVERALLSIZECHECK(frame_tag_extfps_sigctrl)

/* VCSFW_FRAME_TAG_EXTFPS_SUNDRYREGS */

typedef struct fwcmd_frame_tag_extfps_sundryregs_regset_s {
    uint16_t     shutter;
    uint16_t     vblank;
    uint16_t     gain;
    uint16_t     unused;
} fwcmd_frame_tag_extfps_sundryregs_regset_t;

FWCMD_ELCHECK(frame_tag_extfps_sundryregs_regset, shutter)
FWCMD_ELCHECK(frame_tag_extfps_sundryregs_regset, vblank)
FWCMD_ELCHECK(frame_tag_extfps_sundryregs_regset, gain)
FWCMD_ELCHECK(frame_tag_extfps_sundryregs_regset, unused)
FWCMD_OVERALLSIZECHECK(frame_tag_extfps_sundryregs_regset)

typedef struct fwcmd_frame_tag_extfps_sundryregs_s {
    fwcmd_frame_tag_extfps_sundryregs_regset_t  sunsense;
    fwcmd_frame_tag_extfps_sundryregs_regset_t  sunlight;
    fwcmd_frame_tag_extfps_sundryregs_regset_t  dryfinger;
    uint32_t     sunsense_threshold;
} fwcmd_frame_tag_extfps_sundryregs_t;

FWCMD_ELCHECK(frame_tag_extfps_sundryregs, sunsense)
FWCMD_ELCHECK(frame_tag_extfps_sundryregs, sunlight)
FWCMD_ELCHECK(frame_tag_extfps_sundryregs, dryfinger)
FWCMD_ELCHECK(frame_tag_extfps_sundryregs, sunsense_threshold)
FWCMD_OVERALLSIZECHECK(frame_tag_extfps_sundryregs)

typedef struct fwcmd_frame_tag_extfps_otp_tempcorrect_v2_s {
    uint32_t     tempcorrect60;
    uint32_t     tempcorrect25;
} fwcmd_frame_tag_extfps_otp_tempcorrect_v2_t;

FWCMD_ELCHECK(frame_tag_extfps_otp_tempcorrect_v2, tempcorrect60)
FWCMD_ELCHECK(frame_tag_extfps_otp_tempcorrect_v2, tempcorrect25)
FWCMD_OVERALLSIZECHECK(frame_tag_extfps_otp_tempcorrect_v2)

typedef struct fwcmd_frame_tag_extfps_otp_raw_s {
    uint32_t     base;
    uint8_t      data[VCSFW_FRAME_TAG_EXTFPS_OTP_RAW_DATA_MAX];
} fwcmd_frame_tag_extfps_otp_raw_t;

FWCMD_ELCHECK(frame_tag_extfps_otp_raw, base)
FWCMD_ELCHECK(frame_tag_extfps_otp_raw, data)
FWCMD_OVERALLSIZECHECK(frame_tag_extfps_otp_raw)

/* VCSFW_FRAME_TAG_BURSTCFG */
typedef struct fwcmd_frame_tag_burstcfg_step_s {
    uint8_t     burstsize;
    uint8_t     nframes;
} fwcmd_frame_tag_burstcfg_step_t;

FWCMD_ELCHECK(frame_tag_burstcfg_step, burstsize)
FWCMD_ELCHECK(frame_tag_burstcfg_step, nframes)
FWCMD_OVERALLSIZECHECK(frame_tag_burstcfg_step)

typedef struct fwcmd_frame_tag_burstcfg_s {
    uint8_t     nsteps;
    uint8_t     looptop;
    fwcmd_frame_tag_burstcfg_step_t steps[VCSFW_FRAME_TAG_BURSTCFG_NSTEPSMAX];
} fwcmd_frame_tag_burstcfg_t;

FWCMD_ELCHECK(frame_tag_burstcfg, nsteps)
FWCMD_ELCHECK(frame_tag_burstcfg, looptop)
FWCMD_ELCHECK(frame_tag_burstcfg, steps)
FWCMD_OVERALLSIZECHECK(frame_tag_burstcfg)

/****************************************************************************/
/* VCSFW_CMD_FRAME_FINISH (129 = 0x81)                                      */
/****************************************************************************/



/****************************************************************************/
/* VCSFW_CMD_FRAME_STATE_GET (130 = 0x82)                                   */
/****************************************************************************/
typedef struct fwcmd_cmd_frame_state_get_s {
    uint32_t  offset;
    uint16_t  nbytes;
    uint8_t   flags;
    uint8_t   tag;
} fwcmd_cmd_frame_state_get_t;

FWCMD_ELCHECK(cmd_frame_state_get, offset)
FWCMD_ELCHECK(cmd_frame_state_get, nbytes)
FWCMD_ELCHECK(cmd_frame_state_get, flags)
FWCMD_ELCHECK(cmd_frame_state_get, tag)
FWCMD_OVERALLSIZECHECK(cmd_frame_state_get)

typedef struct fwcmd_reply_frame_state_get_s {
    uint32_t   fullsize;
    uint16_t   nbytes;
    uint8_t    unused[2];
} fwcmd_reply_frame_state_get_t;

FWCMD_ELCHECK(reply_frame_state_get, fullsize)
FWCMD_ELCHECK(reply_frame_state_get, nbytes)
FWCMD_ELCHECK(reply_frame_state_get, unused)
FWCMD_OVERALLSIZECHECK(reply_frame_state_get)


/****************************************************************************/
/* VCSFW_CMD_IOTA_WRITE     (131 = 0x83)                                    */
/****************************************************************************/
typedef struct fwcmd_cmd_iota_write_s {
     uint16_t     itype;
     uint8_t      unused[2];
} fwcmd_cmd_iota_write_t;
FWCMD_ELCHECK(cmd_iota_write, itype)
FWCMD_OVERALLSIZECHECK(cmd_iota_write)

typedef struct fwcmd_cmd_iota_write_alt1_s {
     uint16_t     itype;
     uint16_t     flags;
} fwcmd_cmd_iota_write_alt1_t;
FWCMD_ELCHECK(cmd_iota_write_alt1, itype)
FWCMD_ELCHECK(cmd_iota_write_alt1, flags)
FWCMD_OVERALLSIZECHECK(cmd_iota_write_alt1)

/* non-frame_tag iota definitions */


/*
 * Frame Selection algorithm configuration tag.
 */

typedef struct fwcmd_config_frame_select_s{
    uint16_t  cnt_thresh;
    uint8_t   diff_thresh;
    uint8_t   delay_interval_msec;
    uint8_t   max_frames;
    uint8_t   pscan_col_size;
    uint8_t   pscan_row_size;
    uint8_t   pscan_start_row;
    uint8_t   state_check_delay_msec;
    uint8_t   flags;
    uint8_t   unused[2];
} fwcmd_config_frame_select_t;

FWCMD_ELCHECK(config_frame_select, cnt_thresh)
FWCMD_ELCHECK(config_frame_select, diff_thresh)
FWCMD_ELCHECK(config_frame_select, delay_interval_msec)
FWCMD_ELCHECK(config_frame_select, max_frames)
FWCMD_ELCHECK(config_frame_select, pscan_col_size)
FWCMD_ELCHECK(config_frame_select, pscan_row_size)
FWCMD_ELCHECK(config_frame_select, pscan_start_row)
FWCMD_ELCHECK(config_frame_select, state_check_delay_msec)
FWCMD_ELCHECK(config_frame_select, flags)
FWCMD_ELCHECK(config_frame_select, unused)
FWCMD_OVERALLSIZECHECK(config_frame_select)


/*
 * Partial Scan Frame Selection algorithm configuration tag.
 */
typedef struct fwcmd_config_tag_pselect_s {
    uint8_t diff_thresh;
    uint8_t cnt_thresh;
    uint8_t delay_interval_msec;
    uint8_t unused;
    uint8_t pscan_col_size;
    uint8_t pscan_row_size;
    uint8_t state_check_delay_msec;
    uint8_t flags;
} fwcmd_config_tag_pselect_t;

FWCMD_ELCHECK(config_tag_pselect, diff_thresh)
FWCMD_ELCHECK(config_tag_pselect, cnt_thresh)
FWCMD_ELCHECK(config_tag_pselect, delay_interval_msec)
FWCMD_ELCHECK(config_tag_pselect, unused)
FWCMD_ELCHECK(config_tag_pselect, pscan_col_size)
FWCMD_ELCHECK(config_tag_pselect, pscan_row_size)
FWCMD_ELCHECK(config_tag_pselect, state_check_delay_msec)
FWCMD_ELCHECK(config_tag_pselect, flags)
FWCMD_OVERALLSIZECHECK(config_tag_pselect)

/*
 * This struct contains configuration settings for the frame averaging algorithm.
 */

typedef struct fwcmd_config_frame_avg_s {
    uint8_t  frame_avg;
    uint8_t  avg_cnt;
    uint8_t  unused[2];
} fwcmd_config_frame_avg_t;

/*
 * WOF threshold iota definition
 */
typedef struct fwcmd_config_wof_thresholds_s {
    uint32_t      signal;
    uint32_t      hyst;
    uint16_t      reduction;
    uint16_t      unused;
} fwcmd_config_wof_thresholds_t;

FWCMD_ELCHECK(config_wof_thresholds, signal)
FWCMD_ELCHECK(config_wof_thresholds, hyst)
FWCMD_ELCHECK(config_wof_thresholds, reduction)
FWCMD_ELCHECK(config_wof_thresholds, unused)
FWCMD_OVERALLSIZECHECK(config_wof_thresholds)


/*
 * Prometheus WOF threshold iota definition
 */

typedef struct fwcmd_config_prometheus_wof_thresholds_s {
    int16_t     fp_fu_abs_thresh;
    int16_t     fp_fd_abs_thresh;
    int16_t     fp_fu_deriv_thresh;
    int16_t     fp_fd_deriv_thresh;
    int16_t     sb_fu_abs_thresh;
    int16_t     sb_fd_abs_thresh;
    int16_t     sb_fu_deriv_thresh;
    int16_t     sb_fd_deriv_thresh;
    uint32_t    unused;
} fwcmd_config_prometheus_wof_thresholds_t;

FWCMD_ELCHECK(config_prometheus_wof_thresholds, fp_fu_abs_thresh)
FWCMD_ELCHECK(config_prometheus_wof_thresholds, fp_fd_abs_thresh)
FWCMD_ELCHECK(config_prometheus_wof_thresholds, fp_fu_deriv_thresh)
FWCMD_ELCHECK(config_prometheus_wof_thresholds, fp_fd_deriv_thresh)
FWCMD_ELCHECK(config_prometheus_wof_thresholds, sb_fu_abs_thresh)
FWCMD_ELCHECK(config_prometheus_wof_thresholds, sb_fd_abs_thresh)
FWCMD_ELCHECK(config_prometheus_wof_thresholds, sb_fu_deriv_thresh)
FWCMD_ELCHECK(config_prometheus_wof_thresholds, sb_fd_deriv_thresh)
FWCMD_ELCHECK(config_prometheus_wof_thresholds, unused)
FWCMD_OVERALLSIZECHECK(config_prometheus_wof_thresholds)


/*
 * VCSFW_IOTA_ITYPE_CONFIG_WOE
 */

typedef struct fwcmd_config_woe_s {
    uint8_t      flags;
    uint8_t      unused[3];
} fwcmd_config_woe_t;
FWCMD_ELCHECK(config_woe, flags)
FWCMD_ELCHECK(config_woe, unused)
FWCMD_OVERALLSIZECHECK(config_woe)


/* VCSFW_IOTA_ITYPE_CONFIG_VERSION */
/*
 * This iota contains the configuration id and version.
 */
typedef struct fwcmd_config_version_s{
    uint32_t      config_id1;    /* YYMMDD */
    uint32_t      config_id2;    /* HHMMSS */
    uint16_t      version;
    uint16_t      unused[3];
} fwcmd_config_version_t;

/*
 * NAV swipe iota definition
 */
typedef struct fwcmd_config_nav_swipe_s {
    uint8_t       h_regions;
    uint8_t       h_flags;
    uint16_t      h_req_dist_posdir;
    uint16_t      h_req_dist_negdir;
    uint16_t      h_min_exit_pos;
    uint16_t      h_max_exit_pos;
    uint16_t      h_min_fwd_movement;
    uint16_t      h_max_rev_movement;
    uint16_t      h_threshold;
    uint8_t       h_xstride;
    uint8_t       h_ystride;
    uint8_t       v_regions;
    uint8_t       v_flags;
    uint16_t      v_req_dist_posdir;
    uint16_t      v_req_dist_negdir;
    uint16_t      v_min_exit_pos;
    uint16_t      v_max_exit_pos;
    uint16_t      v_min_fwd_movement;
    uint16_t      v_max_rev_movement;
    uint16_t      v_threshold;
    uint8_t       v_xstride;
    uint8_t       v_ystride;
} fwcmd_config_nav_swipe_t;

FWCMD_ELCHECK(config_nav_swipe, h_regions)
FWCMD_ELCHECK(config_nav_swipe, h_flags)
FWCMD_ELCHECK(config_nav_swipe, h_req_dist_posdir)
FWCMD_ELCHECK(config_nav_swipe, h_req_dist_negdir)
FWCMD_ELCHECK(config_nav_swipe, h_min_exit_pos)
FWCMD_ELCHECK(config_nav_swipe, h_max_exit_pos)
FWCMD_ELCHECK(config_nav_swipe, h_min_fwd_movement)
FWCMD_ELCHECK(config_nav_swipe, h_max_rev_movement)
FWCMD_ELCHECK(config_nav_swipe, h_threshold)
FWCMD_ELCHECK(config_nav_swipe, h_xstride)
FWCMD_ELCHECK(config_nav_swipe, h_ystride)
FWCMD_ELCHECK(config_nav_swipe, v_regions)
FWCMD_ELCHECK(config_nav_swipe, v_flags)
FWCMD_ELCHECK(config_nav_swipe, v_req_dist_posdir)
FWCMD_ELCHECK(config_nav_swipe, v_req_dist_negdir)
FWCMD_ELCHECK(config_nav_swipe, v_min_exit_pos)
FWCMD_ELCHECK(config_nav_swipe, v_max_exit_pos)
FWCMD_ELCHECK(config_nav_swipe, v_min_fwd_movement)
FWCMD_ELCHECK(config_nav_swipe, v_max_rev_movement)
FWCMD_ELCHECK(config_nav_swipe, v_threshold)
FWCMD_ELCHECK(config_nav_swipe, v_xstride)
FWCMD_ELCHECK(config_nav_swipe, v_ystride)
FWCMD_OVERALLSIZECHECK(config_nav_swipe)

/*
 * NAV tap iota definition
 */
typedef struct fwcmd_config_nav_tap_s {
    uint32_t      quiescent_time;
    uint32_t      min_tap_time;
    uint32_t      max_tap_time;
    uint32_t      min_gap_time;
    uint32_t      max_gap_time;
    uint32_t      min_press_time;
    uint32_t      max_press_time;
    uint32_t      quiescent_time3;
} fwcmd_config_nav_tap_t;

FWCMD_ELCHECK(config_nav_tap, quiescent_time)
FWCMD_ELCHECK(config_nav_tap, min_tap_time)
FWCMD_ELCHECK(config_nav_tap, max_tap_time)
FWCMD_ELCHECK(config_nav_tap, min_gap_time)
FWCMD_ELCHECK(config_nav_tap, max_gap_time)
FWCMD_ELCHECK(config_nav_tap, min_press_time)
FWCMD_ELCHECK(config_nav_tap, max_press_time)
FWCMD_ELCHECK(config_nav_tap, quiescent_time3)
FWCMD_OVERALLSIZECHECK(config_nav_tap)

typedef struct fwcmd_fppresent_params_s {
    uint32_t variance_thresh;
    uint16_t coverage_thresh;
    uint16_t unused;
    uint16_t blocksizex;
    uint16_t blocksizey;
    uint16_t marginx;
    uint16_t marginy;
} fwcmd_fppresent_params_t;

FWCMD_ELCHECK(fppresent_params, variance_thresh)
FWCMD_ELCHECK(fppresent_params, coverage_thresh)
FWCMD_ELCHECK(fppresent_params, blocksizex)
FWCMD_ELCHECK(fppresent_params, blocksizey)
FWCMD_ELCHECK(fppresent_params, marginx)
FWCMD_ELCHECK(fppresent_params, marginy)
FWCMD_OVERALLSIZECHECK(fppresent_params)


/*
 * BL_MGT config iota definition
 */



typedef struct fwcmd_config_bl_mgt_s{
    uint32_t            period_MS;
    uint16_t            partial_scan_threshold;
    int16_t             pos_threshold;
    int16_t             neg_threshold;
    int16_t             max_positive;
    int16_t             min_negative;
    uint16_t            partial_scan_steps;
    uint8_t             enable_mode;
    uint8_t             unused;
    uint16_t            min_rezero_pixels;
} fwcmd_config_bl_mgt_t;

FWCMD_ELCHECK(config_bl_mgt, period_MS)
FWCMD_ELCHECK(config_bl_mgt, partial_scan_threshold)
FWCMD_ELCHECK(config_bl_mgt, pos_threshold)
FWCMD_ELCHECK(config_bl_mgt, neg_threshold)
FWCMD_ELCHECK(config_bl_mgt, max_positive)
FWCMD_ELCHECK(config_bl_mgt, min_negative)
FWCMD_ELCHECK(config_bl_mgt, min_rezero_pixels)
FWCMD_ELCHECK(config_bl_mgt, partial_scan_steps)
FWCMD_ELCHECK(config_bl_mgt, enable_mode)
FWCMD_ELCHECK(config_bl_mgt, unused)
FWCMD_OVERALLSIZECHECK(config_bl_mgt)


/*
 * BLM config iota definition
 */

typedef struct fwcmd_config_blm_s {
  uint16_t mode;
  uint16_t img_temp_threshold;
  uint16_t wof_temp_threshold;
  uint16_t nav_temp_threshold;
  uint16_t pixel_cal_err_tol;
  uint16_t activity_delay_ms;
  uint32_t wof_fast_sample_rate;
  uint32_t  unused;
} fwcmd_config_blm_t;

FWCMD_ELCHECK(config_blm, mode);
FWCMD_ELCHECK(config_blm, img_temp_threshold)
FWCMD_ELCHECK(config_blm, wof_temp_threshold)
FWCMD_ELCHECK(config_blm, nav_temp_threshold)
FWCMD_ELCHECK(config_blm, pixel_cal_err_tol)
FWCMD_ELCHECK(config_blm, activity_delay_ms)
FWCMD_ELCHECK(config_blm, wof_fast_sample_rate)
FWCMD_ELCHECK(config_blm, unused)
FWCMD_OVERALLSIZECHECK(config_blm)

/* DTM Iota configuation */
typedef struct fwcmd_config_dtm_s {
  uint16_t flags;
  uint16_t fd_cycle_rate;
  uint16_t fast_cycle_rate;
  uint16_t nom_cnts_per_degree_c;
  uint16_t fast_cycle_temperature;
  uint16_t fu_defer_count;
  uint16_t fd_defer_count;
  uint16_t reserved;
} fwcmd_config_dtm_t;

FWCMD_ELCHECK(config_dtm, flags);
FWCMD_ELCHECK(config_dtm, fd_cycle_rate)
FWCMD_ELCHECK(config_dtm, fast_cycle_rate)
FWCMD_ELCHECK(config_dtm, nom_cnts_per_degree_c)
FWCMD_ELCHECK(config_dtm, fast_cycle_temperature)
FWCMD_ELCHECK(config_dtm, fu_defer_count)
FWCMD_ELCHECK(config_dtm, fd_defer_count)
FWCMD_ELCHECK(config_dtm, reserved)
FWCMD_OVERALLSIZECHECK(config_dtm)

/*
 * temp_sense config iota definition
 */

typedef struct fwcmd_config_temp_sense_s {
  uint16_t interval_ms;
  uint8_t  threshold;
  uint8_t  hw_sample_period;
  uint32_t unused;
} fwcmd_config_temp_sense_t;



FWCMD_ELCHECK(config_temp_sense, interval_ms);
FWCMD_ELCHECK(config_temp_sense, threshold);
FWCMD_ELCHECK(config_temp_sense, hw_sample_period);
FWCMD_ELCHECK(config_temp_sense, unused);
FWCMD_OVERALLSIZECHECK(config_temp_sense)

typedef struct fwcmd_config_testhistory_s {
    uint32_t date;        /* seconds since 1/1/2000 */
    uint16_t stage;       /* For example: 1=CP1, 2=CP2, 3=FT, 4=MT, 5=IST.*/
    uint16_t error_code;  /* 0  == pass */
    /* MT core api part number (null-term string) */
    uint8_t  part_MT[VCSFW_TEST_HISTORY_IOTA_PART_NUM_STRING_LEN];
    /* MT config file part number (null-term string) */
    uint8_t  part_config[VCSFW_TEST_HISTORY_IOTA_PART_NUM_STRING_LEN];
} fwcmd_config_testhistory_t;
FWCMD_ELCHECK(config_testhistory, date)
FWCMD_ELCHECK(config_testhistory, stage)
FWCMD_ELCHECK(config_testhistory, error_code)
FWCMD_ELCHECK(config_testhistory, part_MT)
FWCMD_ELCHECK(config_testhistory, part_config)
FWCMD_OVERALLSIZECHECK(config_testhistory)

/****************************************************************************/
/* VCSFW_CMD_FLASH_ERASE (132 = 0x84)                                       */
/****************************************************************************/
typedef struct fwcmd_cmd_flash_erase_s {
     uint32_t     flags;
} fwcmd_cmd_flash_erase_t;
FWCMD_ELCHECK(cmd_flash_erase, flags)
FWCMD_OVERALLSIZECHECK(cmd_flash_erase)

/****************************************************************************/
/* VCSFW_CMD_EVENT_CONFIG (133 = 0x85)                                      */
/****************************************************************************/

typedef uint32_t fwcmd_event_mask_t[VCSFW_EVENT_NTYPES/32];
FWCMD_OVERALLSIZECHECK(event_mask)

typedef struct fwcmd_cmd_event_config_s {
    fwcmd_event_mask_t  active;
    fwcmd_event_mask_t  signal;
    uint32_t            flags;
} fwcmd_cmd_event_config_t;
FWCMD_ELCHECK(cmd_event_config, active)
FWCMD_ELCHECK(cmd_event_config, signal)
FWCMD_ELCHECK(cmd_event_config, flags)
FWCMD_OVERALLSIZECHECK(cmd_event_config)

typedef struct fwcmd_reply_event_config_s {
    fwcmd_event_mask_t  active;
    fwcmd_event_mask_t  signal;
    uint32_t            flags;

    fwcmd_event_mask_t  supported;
    uint32_t            timescale;
    uint32_t            nmax;
    uint16_t            npending;
    uint16_t            nextnum;
} fwcmd_reply_event_config_t;
FWCMD_ELCHECK(reply_event_config, active)
FWCMD_ELCHECK(reply_event_config, signal)
FWCMD_ELCHECK(reply_event_config, flags)
FWCMD_ELCHECK(reply_event_config, supported)
FWCMD_ELCHECK(reply_event_config, timescale)
FWCMD_ELCHECK(reply_event_config, nmax)
FWCMD_ELCHECK(reply_event_config, npending)
FWCMD_ELCHECK(reply_event_config, nextnum)
FWCMD_OVERALLSIZECHECK(reply_event_config)

/****************************************************************************/
/* VCSFW_CMD_EVENT_READ (134 = 0x86)                                        */
/****************************************************************************/

typedef struct fwcmd_event_s {
    uint8_t          type;
    uint8_t          unused1;
    uint16_t         num;
    uint32_t         time;
    union {
        uint32_t            dummy;
        struct {
            uint16_t     nmissed;
        } overflow;
    } details;
} fwcmd_event_t;

FWCMD_ELCHECK(event, type)
FWCMD_ELCHECK(event, unused1)
FWCMD_ELCHECK(event, num)
FWCMD_ELCHECK(event, time)
FWCMD_ELCHECK(event, details)
FWCMD_ELCHECK_NAMED(event, details.dummy, details_dummy)
FWCMD_ELCHECK_NAMED(event, details.overflow, details_overflow)
FWCMD_ELCHECK_NAMED(event, details.overflow.nmissed, details_overflow_nmissed)
FWCMD_OVERALLSIZECHECK(event)

typedef struct fwcmd_cmd_event_read_s {
    uint16_t         firstnum;
    uint16_t         nevents;
} fwcmd_cmd_event_read_t;

FWCMD_ELCHECK(cmd_event_read, firstnum)
FWCMD_ELCHECK(cmd_event_read, nevents)
FWCMD_OVERALLSIZECHECK(cmd_event_read)

typedef struct fwcmd_cmd_event_read_v2_s {
    uint16_t         firstnum;
    uint16_t         nevents;
    uint32_t         flags;
} fwcmd_cmd_event_read_v2_t;

FWCMD_ELCHECK(cmd_event_read_v2, firstnum)
FWCMD_ELCHECK(cmd_event_read_v2, nevents)
FWCMD_ELCHECK(cmd_event_read_v2, flags)
FWCMD_OVERALLSIZECHECK(cmd_event_read_v2)


typedef struct fwcmd_reply_event_read_s {
    uint16_t         nevents;
    uint16_t         npending;
} fwcmd_reply_event_read_t;

FWCMD_ELCHECK(reply_event_read, nevents)
FWCMD_ELCHECK(reply_event_read, npending)
FWCMD_OVERALLSIZECHECK(reply_event_read)

/****************************************************************************/
/* VCSFW_CMD_FMNO_GET (135 = 0x87)                                          */
/****************************************************************************/

typedef struct fwcmd_reply_fmno_get_s {
    uint32_t         fmno;
} fwcmd_reply_fmno_get_t;

FWCMD_ELCHECK(reply_fmno_get, fmno)
FWCMD_OVERALLSIZECHECK(reply_fmno_get)

/****************************************************************************/
/* VCSFW_CMD_FLASH_READ (138 = 0x8a)                                        */
/****************************************************************************/
typedef struct fwcmd_cmd_flash_read_s {
    uint32_t    offset;   /* offset (in bytes) into section at which to start reading */
    uint32_t    nbytes;   /* size (in bytes) of payload to be returned */
    uint16_t    flags;
    uint8_t     section;  /* specifies what section to read */
    uint8_t     reserved; /* undefined byte that may be repurposed later */
} fwcmd_cmd_flash_read_t;
FWCMD_ELCHECK(cmd_flash_read, offset)
FWCMD_ELCHECK(cmd_flash_read, nbytes)
FWCMD_ELCHECK(cmd_flash_read, flags)
FWCMD_ELCHECK(cmd_flash_read, section)
FWCMD_ELCHECK(cmd_flash_read, reserved)
FWCMD_OVERALLSIZECHECK(cmd_flash_read)

typedef struct fwcmd_flash_read_reply_s {
    uint32_t  sectionbytes; /* total size of the section read from */
    uint32_t  nbytes;       /* size of the data payload following this struct */
} fwcmd_flash_read_reply_t;
FWCMD_ELCHECK(flash_read_reply, sectionbytes)
FWCMD_ELCHECK(flash_read_reply, nbytes)
FWCMD_OVERALLSIZECHECK(flash_read_reply)

/****************************************************************************/
/* VCSFW_CMD_FRAME_STREAM (139 = 0x8b)                                      */
/****************************************************************************/

typedef struct fwcmd_cmd_frame_stream_s {
    uint16_t  nbytes;
} fwcmd_cmd_frame_stream_t;

FWCMD_ELCHECK(cmd_frame_stream, nbytes)
FWCMD_OVERALLSIZECHECK(cmd_frame_stream)

typedef struct fwcmd_reply_frame_stream_s {
    uint32_t  nlost;
    uint32_t  nqueued;
    uint16_t  flags;
    uint16_t  acqnum;
} fwcmd_reply_frame_stream_t;

FWCMD_ELCHECK(reply_frame_stream, nlost)
FWCMD_ELCHECK(reply_frame_stream, nqueued)
FWCMD_ELCHECK(reply_frame_stream, flags)
FWCMD_ELCHECK(reply_frame_stream, acqnum)
FWCMD_OVERALLSIZECHECK(reply_frame_stream)


/****************************************************************************/
/* VCSFW_CMD_PRNG (140 = 0x8c)                                              */
/****************************************************************************/

typedef struct fwcmd_cmd_prng_s
{
    uint16_t   bytes;    /* Bytes requested in the reply */
} fwcmd_cmd_prng_t;
FWCMD_ELCHECK(cmd_prng, bytes)
FWCMD_OVERALLSIZECHECK(cmd_prng)

/****************************************************************************/
/* VCSFW_CMD_IOTA_FIND (142 = 0x8e)                                         */
/****************************************************************************/

typedef struct fwcmd_cmd_iota_find_s
{
    uint16_t     itype;
    uint16_t     flags;
    uint8_t      maxniotas;
    uint8_t      firstidx;
    uint8_t      dummy[2];
    uint32_t     offset;
    uint32_t     nbytes;
} fwcmd_cmd_iota_find_t;

FWCMD_ELCHECK(cmd_iota_find, itype)
FWCMD_ELCHECK(cmd_iota_find, flags)
FWCMD_ELCHECK(cmd_iota_find, maxniotas)
FWCMD_ELCHECK(cmd_iota_find, firstidx)
FWCMD_ELCHECK(cmd_iota_find, dummy)
FWCMD_ELCHECK(cmd_iota_find, offset)
FWCMD_ELCHECK(cmd_iota_find, nbytes)
FWCMD_OVERALLSIZECHECK(cmd_iota_find)

typedef struct fwcmd_reply_iota_find_s
{
    uint32_t      fullsize;
} fwcmd_reply_iota_find_t;

FWCMD_ELCHECK(reply_iota_find, fullsize)
FWCMD_OVERALLSIZECHECK(reply_iota_find)

typedef struct fwcmd_reply_iota_find_hdr_s
{
    uint16_t      nbytes;
    uint16_t      itype;
} fwcmd_reply_iota_find_hdr_t;

FWCMD_ELCHECK(reply_iota_find_hdr, nbytes)
FWCMD_ELCHECK(reply_iota_find_hdr, itype)
FWCMD_OVERALLSIZECHECK(reply_iota_find_hdr)

/****************************************************************************/
/* VCSFW_CMD_FRAME_STATS_GET (143 = 0x8f)                                   */
/****************************************************************************/

typedef struct fwcmd_cmd_frame_stats_get_s {
    uint16_t     flags;
    uint8_t      type;
    uint8_t      unused;
} fwcmd_cmd_frame_stats_get_t;

FWCMD_ELCHECK(cmd_frame_stats_get, flags)
FWCMD_ELCHECK(cmd_frame_stats_get, type)
FWCMD_ELCHECK(cmd_frame_stats_get, unused)
FWCMD_OVERALLSIZECHECK(cmd_frame_stats_get)

/****************************************************************************/
/* VCSFW_CMD_IOTA_INSINUATE (144 = 0x90)                                   */
/****************************************************************************/

typedef struct fwcmd_cmd_iota_insinuate_s {
    uint16_t    itype;
    uint16_t    flags;
} fwcmd_cmd_iota_insinuate_t;

FWCMD_ELCHECK(cmd_iota_insinuate, itype)
FWCMD_ELCHECK(cmd_iota_insinuate, flags)
FWCMD_OVERALLSIZECHECK(cmd_iota_insinuate)


/****************************************************************************/
/* VCSFW_CMD_PUBK_GET (145 = 0x91)                                          */
/****************************************************************************/

typedef struct fwcmd_cmd_pubk_get_s {
    uint8_t     key_id;             /* Identifier of the requested keys. */
} fwcmd_cmd_pubk_get_t;
FWCMD_ELCHECK(cmd_pubk_get, key_id)
FWCMD_OVERALLSIZECHECK(cmd_pubk_get)

/* ECC public key. */
typedef struct fwcmd_ecc_pubkey_s
{
    uint32_t iana_id;
    uint32_t key_len;
    uint8_t  sign;
    uint8_t  unused[3];
    uint8_t  x[VCSFW_TAKEOWN_MAX_ECC_PARAM_LEN];
    uint8_t  y[VCSFW_TAKEOWN_MAX_ECC_PARAM_LEN];
} fwcmd_ecc_pubkey_t;
FWCMD_ELCHECK(ecc_pubkey, iana_id)
FWCMD_ELCHECK(ecc_pubkey, key_len)
FWCMD_ELCHECK(ecc_pubkey, sign)
FWCMD_ELCHECK(ecc_pubkey, unused)
FWCMD_ELCHECK(ecc_pubkey, x)
FWCMD_ELCHECK(ecc_pubkey, y)
FWCMD_OVERALLSIZECHECK(ecc_pubkey)

typedef struct fwcmd_reply_pubk_get_s {
    uint16_t     key_len;   /* Length of the key followed to the field. */
    union
    {
        fwcmd_ecc_pubkey_t ecc_key;
    } key;
} fwcmd_reply_pubk_get_t;
FWCMD_ELCHECK(reply_pubk_get, key_len)

/****************************************************************************/
/* VCSFW_CMD_PAIR (147 = 0x93)                                              */
/****************************************************************************/

typedef struct fwcmd_ecc_certificate2_s
{
    /* === Start data to sign === */
    uint16_t cert_type;                           /* Certificate Type   */
    uint16_t curve_iana_id;                       /* Curve ID   */
    uint8_t  x[VCSFW_TAKEOWN_MAX_ECC_PARAM_LEN];  /* Public key */
    uint8_t  y[VCSFW_TAKEOWN_MAX_ECC_PARAM_LEN];
    uint8_t  reserved;
    uint8_t  signature_type;                      /* Type of the signature. */
    /* === End data to sign === */
    uint16_t signature_length;                    /* Length of the signature
                                                      * in the buffer below. */
    uint8_t  signature[VCSFW_MAX_RSA_KEY_LENGTH]; /* Signature - big enough
                                                       buffer to accommodate an
                                                       RSA signature in worst
                                                       case. */
} fwcmd_ecc_certificate2_t; /* 400 Bytes total. */
FWCMD_ELCHECK(ecc_certificate2, cert_type)
FWCMD_ELCHECK(ecc_certificate2, curve_iana_id)
FWCMD_ELCHECK(ecc_certificate2, x)
FWCMD_ELCHECK(ecc_certificate2, y)
FWCMD_ELCHECK(ecc_certificate2, reserved)
FWCMD_ELCHECK(ecc_certificate2, signature_type)
FWCMD_OVERALLSIZECHECK(ecc_certificate2)

typedef struct fwcmd_cmd_pair_s
{
    fwcmd_ecc_certificate2_t host_cert;
} fwcmd_cmd_pair_t;

/* Reply. */
typedef struct fwcmd_reply_pair_s
{
    fwcmd_ecc_certificate2_t host_cert;
    fwcmd_ecc_certificate2_t sensor_cert;
} fwcmd_reply_pair_t;

/****************************************************************************/
/* VCSFW_CMD_SELFTEST_RUN (148 = 0x94)                                      */
/****************************************************************************/

typedef struct
{
        uint8_t     flags;               /* what test to run */
        uint8_t     reserved[3];
} fwcmd_cmd_selftest_run_t;

/****************************************************************************/
/* VCSFW_CMD_SELFTEST_QUERY -- Request last device self-test results        */
/****************************************************************************/

typedef struct fwcmd_reply_selftest_query_s
{
        uint8_t  ran;         /* For each bit, 1=Test ran; 0=Test did not run */
        uint8_t  result;      /* For each bit, if ran=1, 1=PASS; 0=FAIL */
        uint8_t  reserved[2];
} fwcmd_reply_selftest_query_t;

typedef struct fwcmd_soft_button_config_s {
    uint16_t bframe_index;       /* The index of the bscan frame that this button corresponds to */
    int16_t  press_threshold;    /* When exceeded, indicates a button press */
    int16_t  recal_threshold;    /* When exceeded, forces button recalibration (a la FINGER_HIGH in WOF) */
    uint16_t reserved[3];
} fwcmd_soft_button_config_t;
FWCMD_ELCHECK(soft_button_config, bframe_index)
FWCMD_ELCHECK(soft_button_config, press_threshold)
FWCMD_ELCHECK(soft_button_config, recal_threshold)

#if defined(TUDOROPT_ENROLL_WBF_ENABLE)
/****************************************************************************/
/* VCSFW_CMD_ENROLL_WBF (150 = 0x96)                                        */
/****************************************************************************/
typedef struct fwcmd_cmd_enroll_wbf_s
{
  uint32_t SubCommand;
} fwcmd_cmd_enroll_wbf_t;
FWCMD_ELCHECK(cmd_enroll_wbf, SubCommand)

typedef struct fwcmd_cmd_enroll_wbf_begin_s
{
  uint32_t Flags;
  uint32_t NonceSize;     /* How big should the returned Nonce be (bytes)? */
} fwcmd_cmd_enroll_wbf_begin_t;

FWCMD_ELCHECK(cmd_enroll_wbf_begin, Flags)
FWCMD_ELCHECK(cmd_enroll_wbf_begin, NonceSize)

typedef struct fwcmd_reply_enroll_wbf_begin_s
{
  uint32_t NonceSize;
} fwcmd_reply_enroll_wbf_begin_t;
FWCMD_ELCHECK(reply_enroll_wbf_begin, NonceSize)

typedef struct fwcmd_reply_enroll_wbf_update_s
{
    uint128_t TemplateID;
    uint32_t EnrollStatSize;
} fwcmd_reply_enroll_wbf_update_t;
FWCMD_ELCHECK(reply_enroll_wbf_update, TemplateID)
FWCMD_ELCHECK(reply_enroll_wbf_update, EnrollStatSize)

/* VCSFW_CMD_ENROLL_WBF_COMMIT                                              */
typedef struct fwcmd_cmd_enroll_wbf_commit_s
{
    uint32_t Flags;
    uint32_t payloadSize;
} fwcmd_cmd_enroll_wbf_commit_t;
FWCMD_ELCHECK(cmd_enroll_wbf_commit, Flags)
FWCMD_ELCHECK(cmd_enroll_wbf_commit, payloadSize)

#endif

#if defined(TUDOROPT_IDENTIFY_WBF_ENABLE)
/****************************************************************************/
/* VCSFW_CMD_IDENTIFY_WBF (153 = 0x99)                                      */
/****************************************************************************/
typedef struct fwcmd_cmd_identify_wbf_s
{
  uint32_t SubCommand;
} fwcmd_cmd_identify_wbf_t;
FWCMD_ELCHECK(cmd_identify_wbf, SubCommand)



/* VCSFW_CMD_IDENTIFY_WBF_MATCH                                             */
typedef struct fwcmd_cmd_identify_wbf_match_s
{
   uint32_t TuidSize;
   uint32_t NonceSize;
} fwcmd_cmd_identify_wbf_match_t;
FWCMD_ELCHECK(cmd_identify_wbf_match, TuidSize)
FWCMD_ELCHECK(cmd_identify_wbf_match, NonceSize)
/* REPLY                                                                    */
typedef struct fwcmd_reply_identify_wbf_match_s
{
	uint128_t  tuid;
    uint32_t   matchStatSize;
	uint32_t   NonceSize;
	uint32_t   PayloadSize;
} fwcmd_reply_identify_wbf_match_t;

FWCMD_ELCHECK(reply_identify_wbf_match, tuid)
FWCMD_ELCHECK(reply_identify_wbf_match, matchStatSize)
FWCMD_ELCHECK(reply_identify_wbf_match, NonceSize)
FWCMD_ELCHECK(reply_identify_wbf_match, PayloadSize)

/* VCSFW_CMD_IDENTIFY_WBF_FINISH                                            */
/*   - Uses fwcmd_generic_command_t                                         */
/* REPLY                                                                    */
/* -  uses fwcmd_reply_generic reply                                        */
#endif

#if defined(TUDOROPT_HAVE_SSFS)

/****************************************************************************/
/* VCSFW_CMD_DB2_GET_DB_INFO (158 = 0x9E)                                   */
/****************************************************************************/
typedef struct fwcmd_cmd_get_db_info_s {
    uint8_t     type;             //Type = 1:  File System Info, Type = 2: File System Slot Info(only for engineer mode)
} fwcmd_cmd_get_db_info_t;

FWCMD_ELCHECK(cmd_get_db_info, type)
FWCMD_OVERALLSIZECHECK(cmd_get_db_info)

/****************************************************************************/
/* VCSFW_CMD_DB2_GET_OBJECT_LIST (159 = 0x9F)                               */
/****************************************************************************/
typedef struct fwcmd_cmd_get_object_list_s {
    uint8_t     type;             //(1 = User, 2 = Template, 3 = Payload)
    uint8_t     dummy[3];         //for alignment
    uint128_t   parent_id;        //only for Type 2, 3
} fwcmd_cmd_get_object_list_t;

FWCMD_ELCHECK(cmd_get_object_list, type)
FWCMD_ELCHECK(cmd_get_object_list, parent_id)
FWCMD_OVERALLSIZECHECK(cmd_get_object_list)

/****************************************************************************/
/* VCSFW_CMD_DB2_GET_OBJECT_INFO (160 = 0xA0)                               */
/****************************************************************************/
typedef struct fwcmd_cmd_get_object_info_s {
    uint8_t     type;             //(1 = User, 2 = Template, 3 = Payload)
    uint8_t     dummy[3];         //for alignment
    uint128_t   id;               //uuid / tuid / puid
} fwcmd_cmd_get_object_info_t;

FWCMD_ELCHECK(cmd_get_object_info, type)
FWCMD_ELCHECK(cmd_get_object_info, id)
FWCMD_OVERALLSIZECHECK(cmd_get_object_info)

/****************************************************************************/
/* VCSFW_CMD_DB2_GET_OBJECT_DATA (161= 0xA1)                                */
/****************************************************************************/
typedef struct fwcmd_cmd_get_object_data_s {
    uint8_t     type;             //(1 = User, 2 = Template, 3 = Payload)
    uint128_t   id;               //uuid / tuid / puid
} fwcmd_cmd_get_object_data_t;

FWCMD_ELCHECK(cmd_get_object_data, type)
FWCMD_ELCHECK(cmd_get_object_data, id)
FWCMD_OVERALLSIZECHECK(cmd_get_object_data)

/****************************************************************************/
/* VCSFW_CMD_DB2_WRITE_OBJECT (162 = 0xA2)                                  */
/****************************************************************************/
typedef struct fwcmd_cmd_write_object_s {
   uint8_t     type;             //(1 = Add User, 2 = Add Template, 3 = Add Payload, 18 = Update Template)
   uint8_t     Flag;             //Bit[0] = 1 to indicate if host specific object and only valid for type 2 and typ3 3 command
   uint8_t     dummy[2];         //for alignment
   union {
    struct {
        uint32_t    gid;              //group id for type1
        uint32_t    attributes;       //for type 1,2,3
    }cmd_type1;
    struct {
        uint128_t   parent_id;        //for type 2,3
        uint32_t    attributes;       //for type 1,2,3
        uint32_t    fingerIndex;      //for type 2
        uint32_t    data_len;         //for type 2,3,18
        uint8_t     data[4];          //for type 2,3,18
    }cmd_type2;
    struct {
        uint128_t   parent_id;        //for type 2,3
        uint32_t    attributes;       //for type 1,2,3
        uint32_t    payloadtype;      //for type 3
        uint32_t    data_len;         //for type 2,3,18
        uint8_t     data[4];          //for type 2,3,18
    }cmd_type3;
    struct {
        uint128_t   tuid;             //for type 18
        uint32_t    data_len;         //for type 2,3,18
        uint8_t     data[4];          //for type 2,3,18
    }cmd_type18;
   }details;
} fwcmd_cmd_write_object_t;


FWCMD_ELCHECK(cmd_write_object, type)
FWCMD_ELCHECK(cmd_write_object, details)

FWCMD_OVERALLSIZECHECK(cmd_write_object)

/****************************************************************************/
/* VCSFW_CMD_DB2_DELETE_OBJECT (163 = 0xA3)                                 */
/****************************************************************************/
typedef struct fwcmd_cmd_delete_object_s {
    uint8_t     type;             //(1 = User, 2 = Template, 3 = Payload)
    uint8_t     dummy[3];         //for alignment
    uint128_t   id;               //uuid / tuid / puid
}fwcmd_cmd_delete_object_t;

FWCMD_ELCHECK(cmd_delete_object, type)
FWCMD_ELCHECK(cmd_delete_object, id)

FWCMD_OVERALLSIZECHECK(cmd_delete_object)

/****************************************************************************/
/* VCSFW_CMD_DB2_CLEANUP (164 = 0xA4)                                       */
/****************************************************************************/
typedef struct fwcmd_cmd_cleanup_s {
    uint8_t     type;             //(1 = User, 2 = Template, 3 = Payload)
}fwcmd_cmd_cleanup_t;

FWCMD_ELCHECK(cmd_cleanup, type)

FWCMD_OVERALLSIZECHECK(cmd_cleanup)

/****************************************************************************/
/* VCSFW_CMD_DB2_FORMAT (165 = 0xA5)                                        */
/****************************************************************************/
typedef struct fwcmd_cmd_format_s {
    uint8_t  type;                   //(1 = Format with Default sizes, 2 = Format with supplied sizes)
    uint8_t  dummy[1];               //for alignment
    uint16_t UOP_len;                //Length of the UOP in Kbytes for type2
    uint16_t TOP_len;                //Length of the TOP in Kbytes for type2
    uint16_t POP_len;                //Length of the POP in Kbytes for type2
    uint16_t Template_obj_slot_size; //Template object slot Ksize for type2
    uint16_t Payload_obj_slot_size;  //Payload object slot Ksize for type2
}fwcmd_cmd_format_t;

FWCMD_ELCHECK(cmd_format, type)
FWCMD_ELCHECK(cmd_format, UOP_len)
FWCMD_ELCHECK(cmd_format, TOP_len)
FWCMD_ELCHECK(cmd_format, POP_len)
FWCMD_ELCHECK(cmd_format, Template_obj_slot_size)
FWCMD_ELCHECK(cmd_format, Payload_obj_slot_size)

FWCMD_OVERALLSIZECHECK(cmd_format)

/****************************************************************************/
/* VCSFW_CMD_MSBM_CONNECT (166 = 0xA6)                                      */
/****************************************************************************/

/* The command structure. */
typedef struct fwcmd_cmd_msbm_connect_s
{
    uint8_t   r_h[VCSFW_CMD_MSBM_RANDOM_LEN];
    uint32_t  pkh_len;                               /* =0 means re-connect */
    uint8_t   pk_h[VCSFW_CMD_MSBM_ECC_PUBK_BUF_LEN];  /* OCT string format   */
} fwcmd_cmd_msbm_connect_t;
FWCMD_ELCHECK(cmd_msbm_connect, r_h)
FWCMD_ELCHECK(cmd_msbm_connect, pkh_len)
FWCMD_ELCHECK(cmd_msbm_connect, pk_h)
FWCMD_OVERALLSIZECHECK(cmd_msbm_connect)

/* The reply structure. */
typedef struct fwcmd_reply_msbm_connect_s
{
    uint16_t status;
    uint16_t payload_size; /* The size of the payload coming after 
                            * this stucture.
                            * =0 means reconnect. */
    uint8_t  m[VCSFW_SHA256_HASH_SIZE];
} fwcmd_reply_msbm_connect_t;
FWCMD_ELCHECK(reply_msbm_connect, status)
FWCMD_ELCHECK(reply_msbm_connect, payload_size)
FWCMD_ELCHECK(reply_msbm_connect, m)
FWCMD_OVERALLSIZECHECK(reply_msbm_connect)

/* The payload returned on successful connect. */
typedef struct fwcmd_reply_msbm_connect_payload_s
{
    uint8_t   r_d[VCSFW_CMD_MSBM_RANDOM_LEN];
    uint32_t  cert_m_len;
    uint8_t   cert_m[VCSFW_CMD_MSBM_CERT_M_MAXLEN];
    uint32_t  pkd_len;
    uint8_t   pk_d[VCSFW_CMD_MSBM_ECC_PUBK_BUF_LEN]; /* OCT string fmt */
    uint32_t  pkf_len;
    uint8_t   pk_f[VCSFW_CMD_MSBM_ECC_PUBK_BUF_LEN]; /* OCT string fmt */
    uint8_t   h_f[VCSFW_SHA256_HASH_SIZE];
    uint32_t  sm_len;
    uint8_t   s_m[VCSFW_CMD_MSBM_ECDSA_SIGN_BUF_LEN];
    uint32_t  sd_len;
    uint8_t   s_d[VCSFW_CMD_MSBM_ECDSA_SIGN_BUF_LEN];
} fwcmd_reply_msbm_connect_paytload_t;
FWCMD_ELCHECK(reply_msbm_connect_paytload, r_d)
FWCMD_ELCHECK(reply_msbm_connect_paytload, cert_m_len)
FWCMD_ELCHECK(reply_msbm_connect_paytload, cert_m)
FWCMD_ELCHECK(reply_msbm_connect_paytload, pkd_len)
FWCMD_ELCHECK(reply_msbm_connect_paytload, pk_d)
FWCMD_ELCHECK(reply_msbm_connect_paytload, pkf_len)
FWCMD_ELCHECK(reply_msbm_connect_paytload, pk_f)
FWCMD_ELCHECK(reply_msbm_connect_paytload, h_f)
FWCMD_ELCHECK(reply_msbm_connect_paytload, sm_len)
FWCMD_ELCHECK(reply_msbm_connect_paytload, s_m)
FWCMD_ELCHECK(reply_msbm_connect_paytload, sd_len)
FWCMD_ELCHECK(reply_msbm_connect_paytload, s_d)
FWCMD_OVERALLSIZECHECK(reply_msbm_connect_paytload)

#endif
#endif      /* __FWCMD_H */


