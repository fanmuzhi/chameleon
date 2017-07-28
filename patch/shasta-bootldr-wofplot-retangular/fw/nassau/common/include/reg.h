/* -*- mode: c; tab-width: 4 -*- */
/*
 * INTERNAL USE ONLY
 *
 * Copyright (c) 2006-2016 Synaptics Incorporated.  All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated ("Synaptics"). The holder of this file shall treat all
 * information contained herein as confidential, shall use the
 * information only for its intended purpose, and shall not duplicate,
 * disclose, or disseminate any of this information in any manner unless
 * Synaptics has otherwise provided express, written permission.
 */

/*
 * Register operations.  All register operations should come through
 *  these macros.
 *
 * Bjoren Davis, November 14, 2006.
 *  Reworked for Eagle: Bjoren Davis, September 22, 2010.
 *  Reworked for Cosworth: Bjoren Davis, June 9, 2011.
 *  Reworked for Windsor: Bjoren Davis, July 28, 2011.
 *  Adapted for Nassau/Denali from  engineering/private/impl/micro/windsor/fwrom/src/reg.h,v 1.10 2013/09/25 14:38:41
 *   by Bjoren Davis, June 6, 2015.
 */


#ifndef __REG_H
#define __REG_H

/*
 * Common (C/assembler) defines.
 */

#if defined(__ASSEMBLER__)
/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */

/*
 * The assembler version of this header.
 */

#define REG_STUFFINT32(val, shift, mask)                                    \
    ((((val)) << (shift)) & (mask))

#elif defined(__STDC__)

/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */

/*
 * The C version of this header
 */

/* ------------------------------------------------------------------------- */
/* General macros */

#if defined NASSAU_BEHAVIOR_MODEL
extern void reg_write32(uint32_t *ptr, uint32_t val);
extern uint32_t reg_read32(uint32_t *ptr);
extern void reg_or32(uint32_t *ptr, uint32_t val);
extern void reg_and32(uint32_t *ptr, uint32_t val);
extern uint32_t reg_rdwr32(uint32_t *ptr);
extern void reg_andor32(uint32_t *ptr, uint32_t andmask, uint32_t ormask);
extern void reg_memcpy(void *dst, const void *src, unsigned len);
extern uint32_t reg_stuffInt32(uint32_t *ptr, uint32_t shift, uint32_t mask);
extern uint16_t reg_read16(uint16_t *arrp);
extern void reg_write16(uint16_t *arrp, uint16_t val);

#define REG_READ16(arrp)         reg_read16((uint16_t*)arrp)
#define REG_WRITE16(arrp, val)   reg_write16((uint16_t*)(arrp), val)
#define REG_READ32(ptr)          reg_read32((uint32_t *)(ptr))
#define REG_WRITE32(ptr, val)    reg_write32((uint32_t *)(ptr),val)
#define REG_VARWRITE32(ptr, val) reg_write32((uint32_t *)(ptr),val)
#define REG_OR32(ptr, val)       reg_or32((uint32_t *)(ptr),val)
#define REG_VAROR32(ptr, val)    reg_or32((uint32_t *)(ptr),val)
#define REG_XOR32(ptr, val)      reg_or32((uint32_t *)(ptr),val)
#define REG_VARXOR32(ptr, val)   reg_or32((uint32_t *)(ptr),val)
#define REG_AND32(ptr, val)      reg_and32((uint32_t *)(ptr),val)
#define REG_VARAND32(ptr, val)   reg_and32((uint32_t *)(ptr),val)
#define REG_RDWR32(ptr)          reg_rdwr32((uint32_t *)(ptr))
#define REG_ANDOR32(ptr, andmask, ormask)                                   \
    reg_andor32((uint32_t *)(ptr), andmask, ormask)
#define REG_BLKWRITE32(regbasep, srcp, nwords)                              \
    reg_memcpy((uint32_t *)(regbasep), srcp, nwords)
#define REG_BLKREAD32(destp, regbasep, nwords)                              \
    reg_memcpy(destp, (uint32_t *)(regbasep), nwords)

#define REG_STUFFINT32(val, shift, mask)                                    \
    reg_stuffInt32((uint32_t *)(val),shift,mask)

#else  /* NASSAU_BEHAVIOR_MODEL */
/*
 * This macro creates an assertion that the argument is a constant.
 */

#define REG_CONSTASSERT(expr)                                               \
    typedef struct { char _el[(expr) >> 1]; } __reg_constassert

#define  REG_READ16(arrp)         (*((volatile uint16_t *) (arrp)))

#define  REG_WRITE16(arrp, val)   (*((volatile uint16_t *) (arrp)) = (val))

#define REG_READ32(ptr)                                                     \
    (*((volatile uint32_t *) (ptr))) // Doesn't need checking

#define REG_WRITE32(ptr, val)                                               \
    ({                                                                      \
        REG_CONSTASSERT(ptr);                                               \
        (*((volatile uint32_t *) (ptr))) = (val);                           \
    })

#define REG_VARWRITE32(ptr, val)                                            \
    ((*((volatile uint32_t *) (ptr))) = (val))

#define REG_OR32(ptr, val)                                                  \
    ({                                                                      \
        REG_CONSTASSERT(ptr);                                               \
        (*((volatile uint32_t *) (ptr))) |= (val);                          \
    })

#define REG_VAROR32(ptr, val)                                               \
    ({                                                                      \
        (*((volatile uint32_t *) (ptr))) |= (val);                          \
    })

#define REG_XOR32(ptr, val)                                                 \
    ({                                                                      \
        REG_CONSTASSERT(ptr);                                               \
        (*((volatile uint32_t *) (ptr))) ^= (val);                          \
    })

#define REG_VARXOR32(ptr, val)                                                 \
    ({                                                                      \
    (*((volatile uint32_t *) (ptr))) ^= (val);                          \
    })

#define REG_AND32(ptr, val)                                                 \
    ({                                                                      \
        REG_CONSTASSERT(ptr);                                               \
        (*((volatile uint32_t *) (ptr))) &= (val);                          \
    })

#define REG_VARAND32(ptr, val)                                                 \
    ({                                                                      \
    (*((volatile uint32_t *) (ptr))) &= (val);                          \
    })

#define REG_RDWR32(ptr)                                                     \
    ({                                                                      \
        REG_CONSTASSERT(ptr);                                               \
        (*((volatile uint32_t *) (ptr)))                                    \
            = (*((volatile uint32_t *) (ptr)));                             \
    })

#define REG_ANDOR32(ptr, andmask, ormask)                                   \
    ({                                                                      \
        REG_CONSTASSERT(ptr);                                               \
        (*((volatile uint32_t *) (ptr)))                                    \
            = (((*((volatile uint32_t *) (ptr)))                            \
                & (andmask))                                                \
               | (ormask));                                                 \
    })

#if defined(NASSAU_MISSION)
#   define REG_BLKWRITE32(regbasep, srcp, nwords)                           \
    vfk_memcopy_copyreg32((volatile uint32_t *) (regbasep),                 \
                          (volatile const uint32_t *) (srcp),               \
                          (nwords))

#   define REG_BLKREAD32(destp, regbasep, nwords)                           \
    vfk_memcopy_copyreg32((volatile uint32_t *) (destp),                    \
                          (volatile const uint32_t *) (regbasep),           \
                          (nwords))

#endif

/*
 * Pointer arithmetic: return a pointer to an offset register
 */
#define REG_REGOFFP32(blkptr, regoff)                                       \
    ((volatile uint32_t *)                                                  \
     (((volatile uint8_t *) (blkptr)) + (regoff)))

/*
 * Field arithmetic: shift an integer into a field.
 */
#define REG_STUFFINT32(val, shift, mask)                                    \
    ((uint32_t) ((((uint32_t) (val)) << (shift)) & (mask)))


#endif /* NASSAU_BEHAVIOR_MODEL */

#define REG_UNSTUFFINT32(val, shift, mask)                                  \
    ((((uint32_t) (val)) >> (shift)) & (((uint32_t) (mask)) >> (shift)))

/*
 * These next two macros take advantage of the fact that a field
 *  mask is name X and the bit number of the LSB of that field is
 *  names X_B.
 */
#define REG_FIELDUNSTUFF(val, fieldname)                                    \
    (((val) & fieldname) >> fieldname ## _B)
#define REG_FIELDSTUFF(val, fieldname)                                      \
    ((((uint32_t) (val)) << fieldname ## _B) & fieldname)

/*
 * Handy logical testing macros.
 */
#define REG_BITISON(val, mask)                                              \
    (((val) & (mask)) == (mask))
#define REG_BITISOFF(val, mask)                                             \
    (((val) & (mask)) == 0)
#define REG_BITISON_B(val, bitnum)                                          \
    ((((val) >> (bitnum)) & 0x1) != 0)
#define REG_BITISOFF_B(val, bitnum)                                         \
    ((((val) >> (bitnum)) & 0x1) == 0)
#define REG_ALLBITSAREON(val, mask)                                         \
    (((val) & (mask)) == (mask))
#define REG_ANYBITSAREON(val, mask)                                         \
    (((val) & (mask)) != 0)
#define REG_ALLBITSAREOFF(val, mask)                                        \
    (((val) & (mask)) == 0)
#define REG_ANYBITSAREOFF(val, mask)                                        \
    (((val) & (mask)) != (mask))

#if defined(REG_PRIV)
/*
 * Macros to force direct reading and writing of registers
 *  without ever being caught by recording or register checking
 *  functions.  These macros are for use inside of the reg.c
 *  code.
 */
#   define REG_DIRECT_WRITE32(ptr, val)                                     \
    (*((volatile uint32_t *) (ptr))) = (val)
#   define REG_DIRECT_OR32(ptr, val)                                        \
    (*((volatile uint32_t *) (ptr))) |= (val)
#   define REG_DIRECT_XOR32(ptr, val)                                       \
    (*((volatile uint32_t *) (ptr))) ^= (val)
#   define REG_DIRECT_AND32(ptr, val)                                       \
    (*((volatile uint32_t *) (ptr))) &= (val)
#   define REG_DIRECT_READ32(ptr)                                           \
    (*((volatile uint32_t *) (ptr)))
#endif      /* defined(REG_PRIV) */

/* ------------------------------------------------------------------------- */
/* New types */

#if defined(NASSAUOPT_REGREC) 

#   define REG_NOPRECS  80

#if defined(REG_PRIV)
/*
 * The record of an operation.
 *  We assume that the address of the operation is in the PIF
 *  "System RAM" space [0x3000_0000, 0x3fff_ffff], so we take over the
 *  high 4 bits for the operation.
 */
                                               /* nassau-regop-dump abbrev. */
#define REG_OPREC_OPMASK        0xf0000000
#define REG_OPREC_ADDRMASK      0x0fffffff
#define REG_OPREC_OPREAD        0x10000000      /* R for read */
#define REG_OPREC_OPWRITE       0x20000000      /* W for write */
#define REG_OPREC_OPOR          0x30000000      /* O for or */
#define REG_OPREC_OPAND         0x40000000      /* A for and */
#define REG_OPREC_OPXOR         0x50000000      /* X for xor */
#define REG_OPREC_OPANDOR       0x60000000      /* C for combination */
#define REG_OPREC_OPRDWR        0x70000000      /* B for writeback */

typedef struct _reg_oprec_t {
    uint32_t    time;
    uint32_t    addr;
    uint32_t    callerpc;
    uint32_t    val;
} reg_oprec_t;

#endif      /* defined(REG_PRIV)            */
#endif      /* defined(NASSAUOPT_REGREC)     */

/* ------------------------------------------------------------------------- */
/* Prototypes */
/* Use the REG_XXXX macros above rather than calling these directly         */

#if defined(NASSAUOPT_REGREC) || defined(NASSAUOPT_REGCHECK)
extern uint32_t reg_read32(volatile uint32_t *ptr);
extern uint32_t reg_write32(volatile uint32_t *ptr, uint32_t val);
extern uint32_t reg_or32(volatile uint32_t *ptr, uint32_t val);
extern uint32_t reg_and32(volatile uint32_t *ptr, uint32_t val);
extern uint32_t reg_xor32(volatile uint32_t *ptr, uint32_t val);
extern uint32_t reg_andor32(volatile uint32_t *ptr,
                            uint32_t andmask, uint32_t ormask);
extern uint32_t reg_rdwr32(volatile uint32_t *ptr);
#endif      /* defined(NASSAUOPT_REGREC) || defined(NASSAUOPT_REGCHECK) */

#if defined(NASSAUOPT_REGCHECK)
extern void reg_regcheck_disable(void);
extern void reg_regcheck_enable(bool_t assumezeroscratch);
#endif      /* defined(NASSAUOPT_REGCHECK) */

extern void reg_woescratchzero(void);

#if defined(REG_PRIV)
#   if defined(NASSAUOPT_REGREC)

REG_PRIV void reg_oprecord(uint32_t callerpc, uint32_t addr,
                           uint32_t val, uint32_t op);

#   endif       /* defined(NASSAUOPT_REGREC) */

#   if defined(NASSAUOPT_REGCHECK)
REG_PRIV uint32_t reg_regcheck_calc(bool_t assumezeroscratch);
REG_PRIV void reg_regcheck_write32(volatile uint32_t *ptr, uint32_t val);
#   endif       /* defined(NASSAUOPT_REGCHECK) */

#endif          /* defined(REG_PRIV)        */

/* ------------------------------------------------------------------------- */
/* Variables */

#if defined(REG_PRIV)
#   if defined(NASSAUOPT_REGREC)

REG_PRIV reg_oprec_t reg_oprecs[REG_NOPRECS];
REG_PRIV uint32_t reg_oprec_last;
REG_PRIV bool_t reg_oprec_overflow;
#   endif       /* defined(NASSAUOPT_REGREC) */

#   if defined(NASSAUOPT_REGCHECK)
REG_PRIV membool_t reg_check_enabled;
#   endif       /* defined(NASSAUOPT_REGCHECK) */

#endif  /* defined(REG_PRIV) */

/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */

#endif      /* !defined(__STDC__) */
#endif      /* !defined(__REG_H) */
