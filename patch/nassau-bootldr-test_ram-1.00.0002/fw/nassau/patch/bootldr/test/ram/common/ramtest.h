/* -*- mode: c; tab-width: 4 -*- */

/*
 * INTERNAL USE ONLY
 *
 * Copyright (c) 2017-2018 Synaptics Incorporated.  All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated ("Synaptics"). The holder of this file shall treat all
 * information contained herein as confidential, shall use the
 * information only for its intended purpose, and shall not duplicate,
 * disclose, or disseminate any of this information in any manner unless
 * Synaptics has otherwise provided express, written permission.
 */

/*
 * Common include defines for all RAM tests.
 *
 *
 * Bjoren Davis, August 7, 2017.
 */

#if !defined(__RAMTEST_H)
#define __RAMTEST_H
/*
 * Common (C/assembler) defines.
 */

/*
 * The highest valid interrupt.  When we set the CPU to this level
 *  we turn off interrupts.
 */
#define RAMTEST_INTLEVEL_INTR       5

/*
 * Pseudo addressing ranges.
 */
/* Instruction cache data */
#define RAMTEST_ICD_PSEUDOADDR      0xe0000000

/* Instruction cache tag */
#define RAMTEST_ICT_PSEUDOADDR      0xf0000000

/* Data cache tag mapping. */
#define RAMTEST_DCT_PSEUDOADDR      0xd0000000

/*
 * Cache data mapping addresses.  These are addresses into which
 *  we lock the instruction and data caches so we can use them
 *  as local RAM.
 */

/* Data cache data mapping.  We will actually map the data cache RAM here */
#define RAMTEST_DCD_MAPADDR         0x20000000

/*
 * Cache attributes.
 */
#define RAMTEST_ICACHE_ATTR_OFF     0x2222222f
                                            /* bypass cache for all ranges
                                             * except ([0000_0000], [1fff_ffff])
                                             * (_memmap_cacheattr_bp_trapnull)
                                             */
#define RAMTEST_ICACHE_ATTR_ON      0xfff2444f
                                            /* _memmap_cacheattr_wb_strict */

#define RAMTEST_DCACHE_ATTR_ISO     0xfff224ef
                                            /* Keep cache functional
                                             *  [2000_0000, 3fff_ffff] is
                                             *   "isolated" and
                                             *  [4000_0000, 5fff_ffff] is
                                             *   normal (will become locked)
                                             *  [6000_0000, 7fff_ffff] is
                                             *   bypass
                                             * everything else is normal
                                             */
#define RAMTEST_DCACHE_ATTR_ON      0xfff2444f
                                            /* _memmap_cacheattr_wb_strict */

#if defined(__ASSEMBLER__)
/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
/*
 * The assembler version of this header.
 */

#elif defined(__LDSCRIPT__)
/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */

/*
 * The linker script version of this header
 */

#elif defined(__STDC__)
/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */

/*
 * The C version of this header
 */

#include "basetypes.h"
/* -------------------------------------------------------------------------- */
/* General macros */

#if defined(RAMTEST_OPT_ICDATA)
#   define RAMTEST_WRITE32(p, o, v)    ({                                   \
    uint32_t    _ptr, _val;                                                 \
    _val = (v);                                                             \
    _ptr = (uint32_t) (p) + (uint32_t) (o);                                 \
    __asm__ __volatile__ ("sicw %0, %1" : : "a" (_val), "a" (_ptr));        \
    _val; })
#   define RAMTEST_READ32(p, o)    ({                                       \
    uint32_t    _ptr, _val;                                                 \
    _ptr = (uint32_t) (p) + (uint32_t) (o);                                 \
    __asm__ __volatile__ ("licw %0, %1" : "=a" (_val) : "a" (_ptr));        \
    _val; })
#elif defined(RAMTEST_OPT_ICTAG)
#   define RAMTEST_WRITE32(p, o, v)    ({                                   \
    uint32_t    _ptr, _val;                                                 \
    _val = (v);                                                             \
    _ptr = (uint32_t) (p) + (uint32_t) (o);                                 \
    __asm__ __volatile__ ("sict %0, %1" : : "a" (_val), "a" (_ptr));        \
    _val; })
#   define RAMTEST_READ32(p, o)    ({                                       \
    uint32_t    _ptr, _val;                                                 \
    _ptr = (uint32_t) (p) + (uint32_t) (o);                                 \
    __asm__ __volatile__ ("lict %0, %1" : "=a" (_val) : "a" (_ptr));        \
    _val; })
#elif defined(RAMTEST_OPT_DCDATA)
/* Warning: validity_d212, at least, does not support sdcw/ldcw */
#   define RAMTEST_WRITE32(p, o, v)    ({                                   \
    uint32_t    _ptr, _val;                                                 \
    _val = (v);                                                             \
    _ptr = (uint32_t) (p) + (uint32_t) (o);                                 \
    __asm__ __volatile__ ("sdcw %0, %1" : : "a" (_val), "a" (_ptr));        \
    _val; })
#   define RAMTEST_READ32(p, o)    ({                                       \
    uint32_t    _ptr, _val;                                                 \
    _ptr = (uint32_t) (p) + (uint32_t) (o);                                 \
    __asm__ __volatile__ ("ldcw %0, %1" : "=a" (_val) : "a" (_ptr));        \
    _val; })
#elif defined(RAMTEST_OPT_DCTAG)
#   define RAMTEST_WRITE32(p, o, v)    ({                                   \
    uint32_t    _ptr, _val;                                                 \
    _val = (v);                                                             \
    _ptr = (uint32_t) (p) + (uint32_t) (o);                                 \
    __asm__ __volatile__ ("sdct %0, %1" : : "a" (_val), "a" (_ptr));        \
    _val; })
#   define RAMTEST_READ32(p, o)    ({                                       \
    uint32_t    _ptr, _val;                                                 \
    _ptr = (uint32_t) (p) + (uint32_t) (o);                                 \
    __asm__ __volatile__ ("ldct %0, %1" : "=a" (_val) : "a" (_ptr));        \
    _val; })
#else
/* Normal RAM or device RAM */
#   define RAMTEST_NEED_FLUSH_MEMORY
#   define RAMTEST_LOOPUNROLL8      /* a cacheline's size */
#   define RAMTEST_WRITE32(p, o, v)    ({                                   \
    uint32_t    _ptr, _val;                                                 \
    _val = (v);                                                             \
    _ptr = (uint32_t) (p);                                 \
    __asm__ __volatile__                                                    \
        ("s32i %0, %1, %2" : : "a" (_val), "a" (_ptr), "n" (o));            \
    _val; })
#   define RAMTEST_READ32(p, o)    ({                                       \
    uint32_t    _ptr, _val;                                                 \
    _ptr = (uint32_t) (p) + (uint32_t) (o);                                 \
    __asm__ __volatile__                                                    \
        ("l32i %0, %1, %2" : "=a" (_val) : "a" (_ptr), "n" (o));            \
    _val; })
#endif

/* A macro to form a prototype for a RAM testing function */
#define RAMTEST_PROTO(name)                                                 \
    extern bool_t name(uint32_t *blkbasep, unsigned int nwords,             \
                       unsigned int passno, ramtest_result_t *resultp)


/* Quoting macros.  C is a nightmare. */
#define RAMTEST_QUOTE1(x)       #x
#define RAMTEST_QUOTE2(x)       RAMTEST_QUOTE1(x)

/* -------------------------------------------------------------------------- */
/* New types and type-specific macros */

typedef struct ramtest_result_s {
    void        *failptr;           /* by convention, NULL means no error */
    uint32_t     expected;          /* what we expected to see */
    uint32_t     actual;            /* what we actually read */
} ramtest_result_t;


/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */

#endif      /* defined(__STDC__) */

#endif          /* !defined(__RAMTEST_H) */

