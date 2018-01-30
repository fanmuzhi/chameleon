/* -*- mode: c; tab-width: 4 -*- */
/*
 * INTERNAL USE ONLY
 *
 * Copyright (c) 2016 Synaptics Incorporated.  All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated ("Synaptics"). The holder of this file shall treat all
 * information contained herein as confidential, shall use the
 * information only for its intended purpose, and shall not duplicate,
 * disclose, or disseminate any of this information in any manner unless
 * Synaptics has otherwise provided express, written permission.
 */

/*
 * Common patch definitions.
 *  Nassau firmware family.  This is first implemented on
 *  the Denali (b1210) chip.
 *
 * Bjoren Davis, Synaptics, Inc., January 18, 2016
 */

/*
 * NOTE:
 *
 * This file is included into assembler, linker, and C source code.
 *  To make sure it works in all those locations, there are some
 *  rules:
 *
 *  1) Use only simple syntax to define numbers (e.g. '(1 << 8)' is fine,
 *     but 'sizeof(char)' is not.
 *  2) For those items that have parallel definitions (e.g. structure
 *     offsets for assembler use and a corresponding C structure), make
 *     sure there are compile-time assertions to make sure that the
 *     things stay concurrent.
 *  3) Do no put comments at the ends of macro definitions.
 *  4) Make sure you put definitions into the correct sections of
 *     this file.
 */

#ifndef __PATCH_COMMON_H
#define __PATCH_COMMON_H

/*
 * Common (C/assembler/linker script) defines.
 */

#define NASSAU_PATCH_TYPE_BOOTLDR       0
#define NASSAU_PATCH_TYPE_ATEROM        1
#define NASSAU_PATCH_TYPE_MISSION       2

#if defined(__LDSCRIPT__)

/*
 * The linker script version of this file.
 */

#elif defined(__ASSEMBLER__)

/*
 * The assembler version of this header.
 */

#elif defined(__STDC__)

/*
 * The C version of this header
 */

/* ------------------------------------------------------------------------- */
/* New types and type-specific macros */

#endif      /* !defined(__STDC__) */
#endif      /* !defined(__PATCH_COMMON_H) */




