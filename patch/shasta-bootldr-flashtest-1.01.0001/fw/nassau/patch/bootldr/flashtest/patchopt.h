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
 * Options selection for Nassau.  The idea is that this is the
 *  "switching center" for options within the Nassau firmware.  We
 *  specify a very small set of command-line options (e.g.
 *  "NASSAU_TARGET_ROM"), and these are translated here into
 *  smaller options that are use to select pieces of firmware to build,
 * etc.
 *
 * NOTE:
 *  This file is used in C, assembly, and linker script contexts, so:
 *
 *  1. Do not use any language-specific definitions (e.g. "sizeof(uint32_t)").
 *  2. Use ANSI C-style comments '/' + '*' and '*' + '/'.  C++ style
 *     comments break when this file is included in linker scripts.
 *  3. Do not put comments on the end of a macro definition line (e.g.
 *        define  MACRO   99 / *   this is a comment   * /.  This
 *     can break the assembler.
 *
 * Copied and stripped-down, greatly, from mission firmware's nassauopt.h
 *
 */

#ifndef __PATCHOPT_H
#define __PATCHOPT_H

/* ------------------------------------------------------------------------- */
/* General macros */

/*
 * Perform validation on the product and target macros defined on the
 *  command line.
 */

#if !defined(NASSAU_PRODUCT)
#   error You must define NASSAU_PRODUCT before including patchopt.h
#endif

#define NASSAU_PRODUCT_DENALI               53
#define NASSAU_PRODUCT_HAYES                56
#define NASSAU_PRODUCT_SHASTA               60

#if NASSAU_PRODUCT != NASSAU_PRODUCT_DENALI                                 \
    && NASSAU_PRODUCT != NASSAU_PRODUCT_HAYES                               \
    && NASSAU_PRODUCT != NASSAU_PRODUCT_SHASTA

#   error NASSAU_PRODUCT is not defined to a known value
#endif

#if NASSAU_PRODUCT==NASSAU_PRODUCT_HAYES

#define PATCHOPT_SEC_FUSE

#endif

#if NASSAU_PRODUCT==NASSAU_PRODUCT_HAYES
#   define PATCHOPT_PBL_PRODUCT NASSAU_PRODUCT_HAYESPBL
#   define PATCHOPT_FLASH_SOFTPART
#elif NASSAU_PRODUCT==NASSAU_PRODUCT_DENALI
#   define PATCHOPT_PBL_PRODUCT NASSAU_PRODUCT_DENALIPBL
#   define PATCHOPT_HAS_SDB
#elif NASSAU_PRODUCT==NASSAU_PRODUCT_SHASTA
#   define PATCHOPT_PBL_PRODUCT NASSAU_PRODUCT_SHASTAPBL
#   define PATCHOPT_FLASH_SOFTPART
#endif

#endif      /* __PATCHOPT_H */
