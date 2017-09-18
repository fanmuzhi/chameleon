/* -*- mode: c; tab-width: 4 -*- */
/*
 * INTERNAL USE ONLY
 *
 * Copyright (c) 2006-2017 Synaptics Incorporated.  All rights reserved.
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

#if defined(NASSAU_BEHAVIOR_MODEL)
extern void reg_write32(uint32_t *ptr, uint32_t val);
extern uint32_t reg_read32(uint32_t *ptr);
extern void reg_or32(uint32_t *ptr, uint32_t val);
extern void reg_xor32(uint32_t *ptr, uint32_t val);
extern void reg_and32(uint32_t *ptr, uint32_t val);
extern uint32_t reg_rdwr32(uint32_t *ptr);
extern void reg_andor32(uint32_t *ptr, uint32_t andmask, uint32_t ormask);
extern void reg_memcpy(void *dst, const void *src, unsigned len);
extern uint32_t reg_stuffInt32(uint32_t *ptr, uint32_t shift, uint32_t mask);
extern uint16_t reg_read16(uint16_t *arrp);
extern void reg_write16(uint16_t *arrp, uint16_t val);
extern int reg_untilsetpoll32(uint32_t *ptr, uint32_t mask,
                              unsigned int maxcount);
extern int reg_untilclrpoll32(uint32_t *ptr, uint32_t mask,
                              unsigned int maxcount);


#define REG_READ16(arrp)         reg_read16((uint16_t*)arrp)
#define REG_WRITE16(arrp, val)   reg_write16((uint16_t*)(arrp), val)
#define REG_READ32(ptr)          reg_read32((uint32_t *)(ptr))
#define REG_WRITE32(ptr, val)    reg_write32((uint32_t *)(ptr),val)
#define REG_VARWRITE32(ptr, val) reg_write32((uint32_t *)(ptr),val)
#define REG_OR32(ptr, val)       reg_or32((uint32_t *)(ptr),val)
#define REG_VAROR32(ptr, val)    reg_or32((uint32_t *)(ptr),val)
#define REG_XOR32(ptr, val)      reg_xor32((uint32_t *)(ptr),val)
#define REG_VARXOR32(ptr, val)   reg_xor32((uint32_t *)(ptr),val)
#define REG_AND32(ptr, val)      reg_and32((uint32_t *)(ptr),val)
#define REG_VARAND32(ptr, val)   reg_and32((uint32_t *)(ptr),val)
#define REG_RDWR32(ptr)          reg_rdwr32((uint32_t *)(ptr))
#define REG_ANDOR32(ptr, andmask, ormask)                                   \
    reg_andor32((uint32_t *)(ptr), andmask, ormask)
#define REG_BLKWRITE32(regbasep, srcp, nwords)                              \
    reg_memcpy((uint32_t *)(regbasep), srcp, nwords)
#define REG_BLKREAD32(destp, regbasep, nwords)                              \
    reg_memcpy(destp, (uint32_t *)(regbasep), nwords)

#define REG_UNTILSETPOLL32(ptr, mask, maxcount)                             \
    reg_untilsetpoll32((uint32_t *) (ptr), (mask), (maxcount))

#define REG_UNTILCLRPOLL32(ptr, mask, maxcount)                             \
    reg_untilclrpoll32((uint32_t *) (ptr), (mask), (maxcount))

#define REG_STUFFINT32(val, shift, mask)                                    \
    reg_stuffInt32((uint32_t *)(val),shift,mask)

#elif defined(NASSAUOPT_DEBUG_REGREC)                                       \
    && ((defined(NASSAUOPT_DEBUG_REGREC_OPT_IN)                             \
         && defined(REG_REGREC_ON))                                         \
        || (!defined(NASSAUOPT_DEBUG_REGREC_OPT_IN)                         \
            && !defined(REG_REGREC_OFF)))
    /* defined(NASSAU_BEHAVIOR_MODEL) */

/*
 * Use "REGREC" (the register operation recording mechanism).
 *
 * Truth table
 * NASSAUOPT_DEBUG_REGREC              REG_REGREG_ON
 * |     NASSAUOPT_DEBUG_REGREC_OPT_IN |     REG_REGREC_OFF   Result
 * undef N/A                           N/A   N/A              no REGREC
 * def   undef                         N/A   undef            REGREC
 * def   undef                         N/A   def              no REGREC
 * def   def                           undef N/A              no REGREC
 * def   def                           def   N/A              REGREC
 *
 */

/*
 * This macro creates an assertion that the argument is a constant.
 */

#   define REG_CONSTASSERT(expr)                                            \
    typedef struct { char _el[(expr) >> 1]; } __reg_constassert

/*
 * REG_READ16() and REG_WRITE16() do not belong in this file.
 *  We have no true 16-bit registers in any Nassau-generation device.
 *  These macros are being used to do 16-bit reads and writes to RAM,
 *  but in a very silly way (there is no reason to separate even device
 *  RAM writes with 'memw' instructions).
 */
#   define REG_READ16(arrp)         (*((volatile uint16_t *) (arrp)))

#   define REG_WRITE16(arrp, val)   (*((volatile uint16_t *) (arrp)) = (val))

#   define REG_READ32(ptr)                                                  \
    reg_read32((volatile uint32_t *) (ptr))

#   define REG_WRITE32(ptr, val)                                            \
    ({                                                                      \
        REG_CONSTASSERT(ptr);                                               \
        reg_write32((volatile uint32_t *) (ptr), (val));                    \
    })

#   define REG_VARWRITE32(ptr, val)                                         \
    reg_write32((volatile uint32_t *) (ptr), (val))

#   define REG_OR32(ptr, val)                                               \
    ({                                                                      \
        REG_CONSTASSERT(ptr);                                               \
        reg_or32((volatile uint32_t *) (ptr), (val));                       \
    })

#   define REG_XOR32(ptr, val)                                              \
    ({                                                                      \
        REG_CONSTASSERT(ptr);                                               \
        reg_xor32((volatile uint32_t *) (ptr), (val));                      \
    })

#   define REG_AND32(ptr, val)                                              \
    ({                                                                      \
        REG_CONSTASSERT(ptr);                                               \
        reg_and32((volatile uint32_t *) (ptr), (val));                      \
    })

#   define REG_VAROR32(ptr, val)                                            \
    reg_or32((volatile uint32_t *) (ptr), (val))

#   define REG_VARXOR32(ptr, val)                                           \
    reg_xor32((volatile uint32_t *) (ptr), (val))

#   define REG_VARAND32(ptr, val)                                           \
    reg_and32((volatile uint32_t *) (ptr), (val))

#   define REG_RDWR32(ptr)                                                  \
    ({                                                                      \
        REG_CONSTASSERT(ptr);                                               \
        reg_rdwr32((volatile uint32_t *) (ptr));                            \
    })

#   define REG_ANDOR32(ptr, andmask, ormask)                                \
    ({                                                                      \
        REG_CONSTASSERT(ptr);                                               \
        reg_andor32((volatile uint32_t *) (ptr), (andmask), (ormask));      \
    })

#   define REG_UNTILSETPOLL32(ptr, mask, maxcount)                          \
    ({                                                                      \
        REG_CONSTASSERT(ptr);                                               \
        reg_untilsetpoll32((volatile uint32_t *) (ptr),                     \
                           (mask), (maxcount));                             \
    })

#   define REG_UNTILCLRPOLL32(ptr, mask, maxcount)                          \
    ({                                                                      \
        REG_CONSTASSERT(ptr);                                               \
        reg_untilclrpoll32((volatile uint32_t *) (ptr),                     \
                           (mask), (maxcount));                             \
    })

#   if defined(NASSAU_MISSION)
#       define REG_BLKWRITE32(regbasep, srcp, nwords)                       \
    vfk_memcopy_copyreg32((volatile uint32_t *) (regbasep),                 \
                          (volatile const uint32_t *) (srcp),               \
                          (nwords))

#       define REG_BLKREAD32(destp, regbasep, nwords)                       \
    vfk_memcopy_copyreg32((volatile uint32_t *) (destp),                    \
                          (volatile const uint32_t *) (regbasep),           \
                          (nwords))

#   endif       /* defined(NASSAU_MISSION) */

/*
 * Field arithmetic: shift an integer into a field.
 */
#   define REG_STUFFINT32(val, shift, mask)                                 \
    ((uint32_t) ((((uint32_t) (val)) << (shift)) & (mask)))


#else   /*  defined(NASSAUOPT_DEBUG_REGREC)
         *   && ((defined(NASSAUOPT_DEBUG_REGREC_OPT_IN)
         *        && defined(REG_REGREC_ON))
         *       || (!defined(NASSAUOPT_DEBUG_REGREC_OPT_IN)
         *           && !defined(REG_REGREC_OFF)))
         */

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

/*
 * Looping constructs.  I'm making these so that we don't
 *  fill the 'regrec' record with a whole bunch of identical reads.
 */
/*
 * REG_UNTILSETPOLL32 -- keep reading the given register until
 *  one or more of the bits in mask turns on.  If maxcount == 0 then
 *  we loop forever.
 * Returns FALSE if none of the bit(s) ever turned on
 */
#   define REG_UNTILSETPOLL32(ptr, mask, maxcount)                          \
    ({                                                                      \
        uint32_t   __val;                                                   \
        bool_t     __ret;                                                   \
        REG_CONSTASSERT(ptr);                                               \
        if ((maxcount) != 0) {                                              \
            unsigned int __countdown;                                       \
            __countdown = (maxcount);                                       \
            do {                                                            \
                __countdown--;                                              \
                __val = REG_READ32(ptr);                                    \
            } while (__countdown != 0 && (__val & (mask)) == 0);            \
            __ret = (__countdown != 0);                                     \
        }                                                                   \
        else {                                                              \
            do {                                                            \
                __val = REG_READ32(ptr);                                    \
            } while ((__val & (mask)) == 0);                                \
            __ret = TRUE;                                                   \
        }                                                                   \
        __ret;                                                              \
    })

/*
 * REG_UNTILCLRPOLL32 -- keep reading the given register until
 *  all of the bits in a mask turn off.  If maxcount == 0 then
 *  we loop forever.
 * Returns FALSE if some of the bit(s) never turned off.
 */
#   define REG_UNTILCLRPOLL32(ptr, mask, maxcount)                          \
    ({                                                                      \
        uint32_t   __val;                                                   \
        bool_t     __ret;                                                   \
        REG_CONSTASSERT(ptr);                                               \
        if ((maxcount) != 0) {                                              \
            unsigned int __countdown;                                       \
            __countdown = (maxcount);                                       \
            do {                                                            \
                __countdown--;                                              \
                __val = REG_READ32(ptr);                                    \
            } while (__countdown != 0 && (__val & (mask)) != 0);            \
            __ret = (__countdown != 0);                                     \
        }                                                                   \
        else {                                                              \
            do {                                                            \
                __val = REG_READ32(ptr);                                    \
            } while ((__val & (mask)) != 0);                                \
            __ret = TRUE;                                                   \
        }                                                                   \
        __ret;                                                              \
    })

#   if defined(NASSAU_MISSION)
#       define REG_BLKWRITE32(regbasep, srcp, nwords)                       \
    vfk_memcopy_copyreg32((volatile uint32_t *) (regbasep),                 \
                          (volatile const uint32_t *) (srcp),               \
                          (nwords))

#       define REG_BLKREAD32(destp, regbasep, nwords)                       \
    vfk_memcopy_copyreg32((volatile uint32_t *) (destp),                    \
                          (volatile const uint32_t *) (regbasep),           \
                          (nwords))
#   endif       /* defined(NASSAU_MISSION) */

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


#endif  /*  !defined(NASSAU_BEHAVIOR_MODEL)
         *   && (!defined(NASSAUOPT_DEBUG_REGREC)
         *       || ((!defined(NASSAUOPT_DEBUG_REGREC_OPT_IN)
         *             || !defined(REG_REGREC_ON))
         *           && (defined(NASSAUOPT_DEBUG_REGREC_OPT_IN)
         *               || defined(REG_REGREC_OFF))))
         */

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


/* ------------------------------------------------------------------------- */
/* Prototypes */
/* Use the REG_XXXX macros above rather than calling these directly         */

#if defined(NASSAUOPT_DEBUG_REGREC)
extern uint32_t reg_read32(volatile uint32_t *ptr);
extern uint32_t reg_write32(volatile uint32_t *ptr, uint32_t val);
extern uint32_t reg_or32(volatile uint32_t *ptr, uint32_t val);
extern uint32_t reg_and32(volatile uint32_t *ptr, uint32_t val);
extern uint32_t reg_xor32(volatile uint32_t *ptr, uint32_t val);
extern uint32_t reg_andor32(volatile uint32_t *ptr,
                            uint32_t andmask, uint32_t ormask);
extern uint32_t reg_rdwr32(volatile uint32_t *ptr);
extern bool_t reg_untilsetpoll32(volatile uint32_t *ptr, uint32_t mask,
                                 unsigned int maxcount);
extern bool_t reg_untilclrpoll32(volatile uint32_t *ptr, uint32_t mask,
                                 unsigned int maxcount);
#endif      /* defined(NASSAUOPT_DEBUG_REGREC) */

/* ------------------------------------------------------------------------- */
/* Variables */

/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */

#endif      /* !defined(__STDC__) */
#endif      /* !defined(__REG_H) */
