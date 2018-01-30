/* -*- mode: c; tab-width: 4 -*- */
/*
 * INTERNAL USE ONLY
 *
 * Copyright (c) 2015-2017 Synaptics Incorporated.  All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated ("Synaptics"). The holder of this file shall treat all
 * information contained herein as confidential, shall use the
 * information only for its intended purpose, and shall not duplicate,
 * disclose, or disseminate any of this information in any manner unless
 * Synaptics has otherwise provided express, written permission.
 */

/*
 * Nassau general hardware definitions.
 *
 * This file includes the correct file for the particular chip
 *  we're building for.
 *
 * It is used in the boot loader, ATE ROM, mission firmware, and
 *  patches.
 *
 * Note that this file can be included into assembler, C, and linker
 *  script source.  As such, please do not use any C expressions
 *  (e.g., sizeof) and don't put comments inside of definitions.
 */

#ifndef __HWDEF_H
#define __HWDEF_H

#if !defined(NASSAU_PRODUCT_DENALI)
/* This value taken from vcsfw_v4.h */
#   define NASSAU_PRODUCT_DENALI    53
#endif

#if !defined(NASSAU_PRODUCT_HAYES)
/* This value taken from vcsfw_v4.h */
#   define NASSAU_PRODUCT_HAYES     56
#endif

#if !defined(NASSAU_PRODUCT_HAYESCP)
/* This value taken from vcsfw_v4.h */
#   define NASSAU_PRODUCT_HAYESCP   58
#endif

#if !defined(NASSAU_PRODUCT_SHASTA)
/* This value taken from vcsfw_v4.h */
#   define NASSAU_PRODUCT_SHASTA    60
#endif

#if !defined(NASSAU_PRODUCT_SC24)
/* This value taken from vcsfw_v4.h */
#   define NASSAU_PRODUCT_SC24      63
#endif


#if !defined(NASSAU_PRODUCT_DENALIA)
/*
 * Note: we make up a fake DENALIA definition to take advantage that the
 *  platform stored in the version structure (and returned by
 *  VCSFW_CMD_GET_VERSION) is only one byte, so it will report the
 *  same as Denali B0 and later.
 */
#   define NASSAU_PRODUCT_DENALIA           (0x80000000 | NASSAU_PRODUCT_DENALI)
#endif

#if NASSAU_PRODUCT == NASSAU_PRODUCT_DENALI                                 \
    || NASSAU_PRODUCT == NASSAU_PRODUCT_DENALIA
#   include "b1210hwdef.h"
#elif NASSAU_PRODUCT == NASSAU_PRODUCT_HAYES                                \
    || NASSAU_PRODUCT == NASSAU_PRODUCT_HAYESCP
#   include "b1212hwdef.h"
#elif NASSAU_PRODUCT == NASSAU_PRODUCT_SHASTA
#   include "b1216hwdef.h"
#elif NASSAU_PRODUCT == NASSAU_PRODUCT_SC24
#   include "b2224hwdef.h"
#else
#   error "NASSAU_PRODUCT is not one of NASSAU_PRODUCT_{DENALI,DENALIA,HAYES,HAYESCP,SC24}"
#endif

#endif      /* __HWDEF_H */
