/* -*- mode: c; tab-width: 4 -*- */
/*
 * INTERNAL USE ONLY
 *
 * Copyright (c) 2015-2016 Synaptics Incorporated.  All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated ("Synaptics"). The holder of this file shall treat all
 * information contained herein as confidential, shall use the
 * information only for its intended purpose, and shall not duplicate,
 * disclose, or disseminate any of this information in any manner unless
 * Synaptics has otherwise provided express, written permission.
 */

/*
 * Common include file for the primary boot loader for the
 *  Nassau firmware family.  This is first implemented on
 *  the Denali (b1210) chip.
 *
 * Bjoren Davis, Synaptics, Inc., June 8, 2015.
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

#ifndef __BOOTLDR_H
#define __BOOTLDR_H

/*
 * Common (C/assembler/linker script) defines.
 */

/*
 * Potential product types.  These values are chosen to
 *  mirror the values of VCFW_PRODUCT_* defined in
 *  engineering/private/impl/micro/falcon/shared/vcsfw_v4.h.
 */
#if !defined(NASSAU_PRODUCT_DENALI)
/* This value taken from vcsfw_v4.h */
#   define NASSAU_PRODUCT_DENALI    53
#endif

#if !defined(NASSAU_PRODUCT_DENALIPBL)
/* This value taken from vcsfw_v4.h */
#   define NASSAU_PRODUCT_DENALIPBL 54
#endif

#if !defined(NASSAU_PRODUCT_HAYES)
/* This value taken from vcsfw_v4.h */
#   define NASSAU_PRODUCT_HAYES     56
#endif

#if !defined(NASSAU_PRODUCT_HAYESPBL)
/* This value taken from vcsfw_v4.h */
#   define NASSAU_PRODUCT_HAYESPBL  57
#endif

#if !defined(NASSAU_PRODUCT_SHASTA)
/* This value taken from vcsfw_v4.h */
#   define NASSAU_PRODUCT_SHASTA    60
#endif

#if !defined(NASSAU_PRODUCT_SHASTAPBL)
/* This value taken from vcsfw_v4.h */
#   define NASSAU_PRODUCT_SHASTAPBL 61
#endif

#if !defined(NASSAU_PRODUCT)
#   error "You must define NASSAU_PRODUCT"
#endif

/* TODO: When Denali B makes Denali A obsolete, we should remove the DENALIA
 * product below.  This is added to simplify patch building. MJW20160603 */
#if NASSAU_PRODUCT != NASSAU_PRODUCT_DENALI                                 \
    && NASSAU_PRODUCT != NASSAU_PRODUCT_HAYES                               \
    && NASSAU_PRODUCT != NASSAU_PRODUCT_DENALIA                             \
    && NASSAU_PRODUCT != NASSAU_PRODUCT_SHASTA
#   error "NASSAU_PRODUCT must be one of NASSAU_PRODUCT_{DENALI,HAYES,DENALIA,SHASTA}"
#endif


/*
 * Place the vectors right above the ATE code.
 */
#define BOOTLDR_MEMMAP_VECBASE                                              \
    (ATEROM_MEMMAP_ROM_BASE + ATEROM_MEMMAP_ROM_SIZE)

/*
 * Reset vector size.
 */
#define BOOTLDR_MEMMAP_RESETVEC_SIZE    0x280

/*
 * Allow for three cache-lines worth of static variables, including
 *  the failsave data (whic are 4*17 = 68 bytes long).
 */
#define BOOTLDR_MEMMAP_DATA_SIZE        (3*0x20)

/*
 * Stack size.
 */
#define BOOTLDR_MEMMAP_STACK_SIZE       (200*16)

/*
 * The size and location of the ROM-based bootldr version information.
 */
#define BOOTLDR_MEMMAP_VER_ROINFO_SIZE  14
#define BOOTLDR_MEMMAP_VER_ROINFO_BASE  0x50000300

/*
 * The size and location of the ROM-based function pointers table.
 */
#define BOOTLDR_MEMMAP_ROM_FUNCPS_SIZE  0x80
#define BOOTLDR_MEMMAP_ROM_FUNCPS_BASE  0x50000380

/*
 * The location and size of the bootldr_cpusave function.  bootldr_cpusave
 *  is meant to be jumped to, which is why it's at a fixed address.
 *  If it were not at a fixed address, then the CPU would have to
 *  use registers to calculate the function's address.  But we need
 *  the registers to (possibly) save.  So there.
 */
#define BOOTLDR_MEMMAP_ROM_CPUSAVE_SIZE 0x80
#define BOOTLDR_MEMMAP_ROM_CPUSAVE_BASE 0x50000280

/* The BOOTLDR_MEMMAP_CPUSAVE macro points to the function to jump to. */
#define BOOTLDR_MEMMAP_CPUSAVE          BOOTLDR_MEMMAP_ROM_CPUSAVE_BASE

/*
 * The bootldr_intpend word.  This shows pending interrupts.
 */
#define BOOTLDR_MEMMAP_INTPEND          0x6000ffe0

/*
 * The size of the patch headers.
 */
/* the size of vcsfw_cmd_bootldr_patch_plainhdr_t */
#define BOOTLDR_PATCH_PLAINHDR_SIZE     36

/* the size of vcsfw_cmd_bootldr_patch_enchdr_t */
#define BOOTLDR_PATCH_ENCHDR_SIZE       20

/*
 * Interrupt stack layout.
 */
/* The user mode vector exception save stack: */

    /*
     * Here's the stack layout:
     *
     *    Higher Addresses
     *  +-----------------+
     *  |                 | <- original a1
     *  -------------------  16-byte paragraph divider
     *  |    a3 overflow  |    original a1 - 0x04
     *  |    a2 overflow  |    original a1 - 0x08
     *  |    a1 overflow  |    original a1 - 0x0c
     *  |    a0 overflow  |    original a1 - 0x10
     *  -------------------  16-byte paragraph divider
     *  |    a11 overflow |    original a1 - 0x14
     *  |    a10 overflow |    original a1 - 0x18
     *  |    a9 overflow  |    original a1 - 0x1c
     *  |    a8 overflow  |    original a1 - 0x20
     *  -------------------  16-byte paragraph divider
     *  |    a7 overflow  |    original a1 - 0x24
     *  |    a6 overflow  |    original a1 - 0x28
     *  |    a5 overflow  |    original a1 - 0x2c
     *  |    a4 overflow  |    original a1 - 0x30
     *  -------------------  16-byte paragraph divider
     *  |    a4 save      |    original a1 - 0x34 (only used by scm_isr)
     *  |    a3 save      |    original a1 - 0x38
     *  |    a2 save      |    original a1 - 0x3c
     *  |    a0 save      |    original a1 - 0x40 <- a1 at interrupt
     *  -------------------  16-byte paragraph divider
     *  +-----------------+
     *   Lower Addresses
     *
     * Note that the overflow spaces are only used by C-callable
     *  interrupt handlers (e.g. scm_isr) and its descendants.
     *
     * Note also that the A4 register is only saved in the C-callable
     *  interrupt handlers as well.
     */
#define BOOTLDR_EXC_A0           0x00
#define BOOTLDR_EXC_A2           0x04
#define BOOTLDR_EXC_A3           0x08
#define BOOTLDR_EXC_A4           0x0c

#define BOOTLDR_EXC_FRAMESIZE    0x10
#define BOOTLDR_EXC_TOTALFRAMESIZE   (0x30 + BOOTLDR_EXC_FRAMESIZE)

/*
 * This version of this files contains the definition of the
 *  following funcps "interface" version:
 */
#define BOOTLDR_FUNCPS_IFVER                3

#if defined(NASSAU_PATCH_BOOTLDR_IFVER)                                     \
    && (NASSAU_PATCH_BOOTLDR_IFVER > BOOTLDR_FUNCPS_IFVER)
/*
 * We're being compiled into a bootldr patch and we're
 *  expecting an interface version that seems to be in the future.
 * Give a warning.
 */
#   warn "NASSAU_PATCH_BOOTLDR_IFVER seems to be ahead of BOOTLDR_FUNCPS_IFVER"
#endif

/*
 * Patches can check against this value.  If the value they
 *  were compiled against is bigger than the value in the ROM
 *  then they can issue an error.
 * This ensures back-compatibility.  That is: write your patch
 *  once and it will always work (assuming no one breaks this
 *  interface).
 */

/*
 * Exported functions.  There is a dispatch table in the ROM
 *  that RAM code can use to call useful functions.  These
 *  macros are the indices in the table.
 */
#if !defined(NASSAU_PATCH_BOOTLDR_IFVER) || NASSAU_PATCH_BOOTLDR_IFVER >= 1
/* ifver=1: */
#define BOOTLDR_FUNCPS_INDEX_IFVER              0
#define BOOTLDR_FUNCPS_INDEX_CPU_RESET          1
#define BOOTLDR_FUNCPS_INDEX_REPLY              2
#define BOOTLDR_FUNCPS_INDEX_MEMCPY             3
#define BOOTLDR_FUNCPS_INDEX_MEMSET             4
#define BOOTLDR_FUNCPS_INDEX_CACHE_DFLUSH       5
#define BOOTLDR_FUNCPS_INDEX_CACHE_IFLUSH       6
#define BOOTLDR_FUNCPS_INDEX_CACHE_DATTR_SET    7
#define BOOTLDR_FUNCPS_INDEX_CACHE_IATTR_SET    8
#define BOOTLDR_FUNCPS_INDEX_SHA256_INIT        9
#define BOOTLDR_FUNCPS_INDEX_SHA256_UPDATE      10
#define BOOTLDR_FUNCPS_INDEX_SHA256_FINALIZE    11
#define BOOTLDR_FUNCPS_INDEX_AESCBC_KEY_INIT    12
#define BOOTLDR_FUNCPS_INDEX_AESCBC_CBC_DECRYPT 13
#endif      /* !defined(NASSAU_PATCH_BOOTLDR_IFVER)
             * || NASSAU_PATCH_BOOTLDR_IFVER >= 1 */

#if !defined(NASSAU_PATCH_BOOTLDR_IFVER) || NASSAU_PATCH_BOOTLDR_IFVER >= 2
/* ifver=2: */
#define BOOTLDR_FUNCPS_INDEX_CMDSET             14
#define BOOTLDR_FUNCPS_INDEX_SANITY_PANIC       15
#endif      /* !defined(NASSAU_PATCH_BOOTLDR_IFVER)
             * || NASSAU_PATCH_BOOTLDR_IFVER >= 2 */

#if !defined(NASSAU_PATCH_BOOTLDR_IFVER) || NASSAU_PATCH_BOOTLDR_IFVER >= 3
/* ifver=3: */
#define BOOTLDR_FUNCPS_INDEX_IDLESET            16
#define BOOTLDR_FUNCPS_INDEX_INTRSET            17
#endif      /* !defined(NASSAU_PATCH_BOOTLDR_IFVER)
             * || NASSAU_PATCH_BOOTLDR_IFVER >= 3 */


/* Cache flushing options.  These are for the  */
    /* invalidate (remove cachelines from cache) */
#define BOOTLDR_CACHE_FLUSH_INV         0
    /* write-back (flush out pending writes) */
#define BOOTLDR_CACHE_FLUSH_WB          1
    /* write-back and invalidate */
#define BOOTLDR_CACHE_FLUSH_WBINV       2

/* Length of SHA-256 hash result (in bytes) */
#define BOOTLDR_SHA256_SIZE             32

/* SHA-256 Hash block size (in bytes) */
#define BOOTLDR_SHA256_BLOCK_SIZE       64

/* AES block size and IV size (in bytes) */
#define BOOTLDR_AESCBC_BLOCK_SIZE       16

/* Supported key strengths (in bytes) */
#define BOOTLDR_AESCBC_KEYLEN_256       32

/* RSA key length (in bytes) */
#define BOOTLDR_RSA_KEYLEN_2048         256


/*
 * These codes define why we did not boot to the mission firmware.
 *  We return this value via vcsfw_reply_get_version_t::patch.
 */
/* The NOMISSION bit was set in the WOE_DFT word */
#define BOOTLDR_MISSIONWHYNOT_NOMISSION       1
/* The MISSIONVALID word in flash was not valid */
#define BOOTLDR_MISSIONWHYNOT_INVALID         2
/* The CRC-32 value calculated over the mission firmware was not valid */
#define BOOTLDR_MISSIONWHYNOT_BADCRC          3

/*
 * DFT word bits for the FW_DEFINED field.
 */
#define BOOTLDR_FW_DEFINED_NOMISSION        0x01
#define BOOTLDR_FW_DEFINED_NOMISSION_B          0
#define BOOTLDR_FW_DEFINED_NOMISSION_N          1
#define BOOTLDR_FW_DEFINED_HOSTCOMMFORCE    0x06
#define BOOTLDR_FW_DEFINED_HOSTCOMMFORCE_B      1
#define BOOTLDR_FW_DEFINED_HOSTCOMMFORCE_N      2
#define BOOTLDR_FW_DEFINED_HOSTCOMMFORCE_SPI        1
#define BOOTLDR_FW_DEFINED_HOSTCOMMFORCE_USB        2

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

/*
 * The bootldr version information structure.
 *
 * This always lives at 0x5000_0300 (BOOTLDR_MEMMAP_VER_ROINFO_BASE).
 */
/* a structure containing the read-only bootldrver information */
typedef struct _bootldr_ver_roinfo_t {
    uint32_t    buildtime;
    uint32_t    buildnum;
    uint8_t     vmajor;
    uint8_t     vminor;
    uint8_t     target;     /* FPGA, ISS, RTLSIM, ROM */
    uint8_t     product;    /* Hornet, Falcon, etc. */
    uint8_t     siliconrev;
    uint8_t     formalrel;  /* non-zero -> formal release. */
} bootldr_ver_roinfo_t;

/*
 * This is a special "compile time" assert.  If it fails, it
 *  will create a compile-time error.
 */
#define BOOTLDR_ASSERT_CT(name, x)                                          \
    typedef char _##name [(x) ? 1 : -1]

#if !defined(NASSAU_PATCH_BOOTLDR_IFVER) || NASSAU_PATCH_BOOTLDR_IFVER >= 1
/* SHA256 context */
/* The length is 112 bytes. */
typedef struct bootldr_sha256_ctx_s
{
    uint32_t  state[8];                         /* state */
    uint8_t   buffer[BOOTLDR_SHA256_BLOCK_SIZE]; /* input buffer */
    uint32_t  hbits;                            /* number of high bits */
    uint32_t  lbits;                            /* number of low bits */
    uint32_t  msglen;                           /* message length */
} bootldr_sha256_ctx_t;

/* The AESCBC expanded key context. */
typedef struct bootldr_aescbc_key_s {
    uint32_t     nrnd;                         /* Cipher round count  */
    uint32_t     key[4 * BOOTLDR_AESCBC_BLOCK_SIZE];   /* Expanded key blob */
} bootldr_aescbc_key_t;
#endif      /* !defined(NASSAU_PATCH_BOOTLDR_IFVER)
             * || NASSAU_PATCH_BOOTLDR_IFVER >= 1 */

/*
 * There is a function table in the ROM that points to these
 *  support functions.  Downloaded C code can call into these
 *  functions to get things done.
 */
typedef struct _bootldr_funcps_s {
    uint32_t      ifver;        /* interface version */
#if !defined(NASSAU_PATCH_BOOTLDR_IFVER) || NASSAU_PATCH_BOOTLDR_IFVER >= 1
    /*
     * This is ifver=1:
     */
    void        (*cpu_reset)(void) __attribute__ ((noreturn));
    void        (*reply)(void *ctxp, const uint8_t *replyp, unsigned int len);
    void       *(*memcpy)(void *dst, const void *src, unsigned int len);
    void       *(*memset)(void *dst, int c, unsigned int length);
    void        (*cache_dflush)(void *ptr, uint32_t len, unsigned int howflush);
    void        (*cache_iflush)(void *ptr, uint32_t len, unsigned int howflush);
    void        (*cache_dattr_set)(uint32_t cacheattr);
    void        (*cache_iattr_set)(uint32_t cacheattr);
    void        (*sha256_init)(bootldr_sha256_ctx_t *ctxp);
    void        (*sha256_update)(bootldr_sha256_ctx_t *ctxp,
                                 const uint8_t *msgp,
                                 uint32_t msg_len, uint32_t *wp);
    void        (*sha256_finalize)(bootldr_sha256_ctx_t *ctxp, uint8_t *hashp,
                                   uint32_t *wp);
    void        (*aescbc_key_init)(bootldr_aescbc_key_t *keyp,
                                   bootldr_aescbc_key_t *tmpkeyp);
    void        (*aescbc_cbc_decrypt)(bootldr_aescbc_key_t *keyp,
                                      uint8_t *inoutp,
                                      unsigned int length, uint8_t *ivp);
#endif      /* !defined(NASSAU_PATCH_BOOTLDR_IFVER)
             * || NASSAU_PATCH_BOOTLDR_IFVER >= 1 */

#if !defined(NASSAU_PATCH_BOOTLDR_IFVER) || NASSAU_PATCH_BOOTLDR_IFVER >= 2
    /*
     * This is additional for ifver=2:
     */
    void        (*cmdset)(void *ctxp, uint8_t *cmdbufp, unsigned int cmdbuflen,
                          void (*dispatchp)(void *ctxp, const uint8_t *cmdbufp,
                                            unsigned int cmdlen,
                                            bool_t overflow));
    void        (*sanity_panic)(uint32_t code) __attribute__ ((noreturn));
#endif      /* !defined(NASSAU_PATCH_BOOTLDR_IFVER)
             * || NASSAU_PATCH_BOOTLDR_IFVER >= 2 */

#if !defined(NASSAU_PATCH_BOOTLDR_IFVER) || NASSAU_PATCH_BOOTLDR_IFVER >= 3
    /*
     * This is additional for ifver=3:
     */
    void        (*idleset)(void *ctxp, void (*idlep)(void *ctxp));
    void        (*intrset)(void *ctxp, uint32_t userintrmask,
                           uint32_t (*intrp)(void *ctxp, uint32_t intron));
#endif      /* !defined(NASSAU_PATCH_BOOTLDR_IFVER)
             * || NASSAU_PATCH_BOOTLDR_IFVER >= 3 */

} bootldr_funcps_t;

/* Make sure this structure lines up with the common indexes. */
#if !defined(NASSAU_PATCH_BOOTLDR_IFVER) || NASSAU_PATCH_BOOTLDR_IFVER >= 1
BOOTLDR_ASSERT_CT(bootldr_funcps_index_cpu_reset_valid,
                 OFFSETOF(bootldr_funcps_t, cpu_reset)
                  == (BOOTLDR_FUNCPS_INDEX_CPU_RESET * sizeof(void *)));
BOOTLDR_ASSERT_CT(bootldr_funcps_index_reply_valid,
                 OFFSETOF(bootldr_funcps_t, reply)
                 == (BOOTLDR_FUNCPS_INDEX_REPLY * sizeof(void *)));
BOOTLDR_ASSERT_CT(bootldr_funcps_index_memcpy_valid,
                 OFFSETOF(bootldr_funcps_t, memcpy)
                 == (BOOTLDR_FUNCPS_INDEX_MEMCPY * sizeof(void *)));
BOOTLDR_ASSERT_CT(bootldr_funcps_index_memset_valid,
                 OFFSETOF(bootldr_funcps_t, memset)
                 == (BOOTLDR_FUNCPS_INDEX_MEMSET * sizeof(void *)));
BOOTLDR_ASSERT_CT(bootldr_funcps_index_cache_dflush_valid,
                  OFFSETOF(bootldr_funcps_t, cache_dflush)
                  == (BOOTLDR_FUNCPS_INDEX_CACHE_DFLUSH * sizeof(void *)));
BOOTLDR_ASSERT_CT(bootldr_funcps_index_cache_iflush_valid,
                  OFFSETOF(bootldr_funcps_t, cache_iflush)
                  == (BOOTLDR_FUNCPS_INDEX_CACHE_IFLUSH * sizeof(void *)));
BOOTLDR_ASSERT_CT(bootldr_funcps_index_cache_dattr_set_valid,
                  OFFSETOF(bootldr_funcps_t, cache_dattr_set)
                  == (BOOTLDR_FUNCPS_INDEX_CACHE_DATTR_SET * sizeof(void *)));
BOOTLDR_ASSERT_CT(bootldr_funcps_index_cache_iattr_set_valid,
                  OFFSETOF(bootldr_funcps_t, cache_iattr_set)
                  == (BOOTLDR_FUNCPS_INDEX_CACHE_IATTR_SET * sizeof(void *)));
BOOTLDR_ASSERT_CT(bootldr_funcps_index_sha256_init_valid,
                  OFFSETOF(bootldr_funcps_t, sha256_init)
                  == (BOOTLDR_FUNCPS_INDEX_SHA256_INIT * sizeof(void *)));
BOOTLDR_ASSERT_CT(bootldr_funcps_index_sha256_update_valid,
                  OFFSETOF(bootldr_funcps_t, sha256_update)
                  == (BOOTLDR_FUNCPS_INDEX_SHA256_UPDATE * sizeof(void *)));
BOOTLDR_ASSERT_CT(bootldr_funcps_index_sha256_finalize_valid,
                  OFFSETOF(bootldr_funcps_t, sha256_finalize)
                  == (BOOTLDR_FUNCPS_INDEX_SHA256_FINALIZE * sizeof(void *)));
BOOTLDR_ASSERT_CT(bootldr_funcps_index_aescbc_key_init_valid,
                  OFFSETOF(bootldr_funcps_t, aescbc_key_init)
                  == (BOOTLDR_FUNCPS_INDEX_AESCBC_KEY_INIT * sizeof(void *)));
BOOTLDR_ASSERT_CT(bootldr_funcps_index_aescbc_cbc_decrypt_valid,
                  OFFSETOF(bootldr_funcps_t, aescbc_cbc_decrypt)
                  == (BOOTLDR_FUNCPS_INDEX_AESCBC_CBC_DECRYPT
                      * sizeof(void *)));
#endif      /* !defined(NASSAU_PATCH_BOOTLDR_IFVER)
             * || NASSAU_PATCH_BOOTLDR_IFVER >= 1 */


#if !defined(NASSAU_PATCH_BOOTLDR_IFVER) || NASSAU_PATCH_BOOTLDR_IFVER >= 2
BOOTLDR_ASSERT_CT(bootldr_funcps_index_cmdset_valid,
                  OFFSETOF(bootldr_funcps_t, cmdset)
                  == (BOOTLDR_FUNCPS_INDEX_CMDSET
                      * sizeof(void *)));
BOOTLDR_ASSERT_CT(bootldr_funcps_index_sanity_panic_valid,
                  OFFSETOF(bootldr_funcps_t, sanity_panic)
                  == (BOOTLDR_FUNCPS_INDEX_SANITY_PANIC
                      * sizeof(void *)));
#endif      /* !defined(NASSAU_PATCH_BOOTLDR_IFVER)
             * || NASSAU_PATCH_BOOTLDR_IFVER >= 2 */

#if !defined(NASSAU_PATCH_BOOTLDR_IFVER) || NASSAU_PATCH_BOOTLDR_IFVER >= 3
BOOTLDR_ASSERT_CT(bootldr_funcps_index_idleset_valid,
                  OFFSETOF(bootldr_funcps_t, idleset)
                  == (BOOTLDR_FUNCPS_INDEX_IDLESET
                      * sizeof(void *)));
BOOTLDR_ASSERT_CT(bootldr_funcps_index_intrset_valid,
                  OFFSETOF(bootldr_funcps_t, intrset)
                  == (BOOTLDR_FUNCPS_INDEX_INTRSET
                      * sizeof(void *)));
#endif      /* !defined(NASSAU_PATCH_BOOTLDR_IFVER)
             * || NASSAU_PATCH_BOOTLDR_IFVER >= 3 */

BOOTLDR_ASSERT_CT(bootldr_funcps_table_toobig,
                 sizeof(bootldr_funcps_t) <= BOOTLDR_MEMMAP_ROM_FUNCPS_SIZE);


extern const bootldr_funcps_t bootldr_funcps;

#if !defined(NASSAU_PATCH_BOOTLDR_IFVER) || NASSAU_PATCH_BOOTLDR_IFVER >= 1
#define BOOTLDR_CPU_RESET()                                                 \
    (*bootldr_funcps.cpu_reset)()
#define BOOTLDR_REPLY(ctxp, replyp, len)                                    \
    (*bootldr_funcps.reply)((ctxp), (replyp), (len))
#define BOOTLDR_MEMCPY(dst, src, len)                                       \
    (*bootldr_funcps.memcpy)((dst), (src), (len))
#define BOOTLDR_MEMSET(dst, c, length)                                      \
    (*bootldr_funcps.memset)((dst), (c), (length))
#define BOOTLDR_CACHE_DFLUSH(ptr, len, howflush)                            \
    (*bootldr_funcps.cache_dflush)((ptr), (len), (howflush))
#define BOOTLDR_CACHE_IFLUSH(ptr, len, howflush)                            \
    (*bootldr_funcps.cache_iflush)((ptr), (len), (howflush))
#define BOOTLDR_CACHE_DATTR_SET(cacheattr)                                  \
    (*bootldr_funcps.cache_dattr_set)((cacheattr))
#define BOOTLDR_CACHE_IATTR_SET(cacheattr)                                  \
    (*bootldr_funcps.cache_iattr_set)((cacheattr))
#define BOOTLDR_SHA256_INIT(ctxp)                                           \
    (*bootldr_funcps.sha256_init)((ctxp))
#define BOOTLDR_SHA256_UPDATE(ctxp, msgp, msg_len, wp)                      \
    (*bootldr_funcps.sha256_update)((ctxp), (msgp), (msg_len), (wp))
#define BOOTLDR_SHA256_FINALIZE(ctxp, hashp, wp)                            \
    (*bootldr_funcps.sha256_finalize)((ctxp), (hashp), (wp))
#define BOOTLDR_AESCBC_KEY_INIT(keyp, tmpkeyp)                              \
    (*bootldr_funcps.aescbc_key_init)((keyp), (tmpkeyp))
#define BOOTLDR_AESCBC_CBC_DECRYPT(keyp, inoutp, length, ivp)               \
    (*bootldr_funcps.aescbc_cbc_decrypt)((keyp), (inoutp), (length), (ivp))
#endif      /* !defined(NASSAU_PATCH_BOOTLDR_IFVER)
             * || NASSAU_PATCH_BOOTLDR_IFVER >= 1 */

#if !defined(NASSAU_PATCH_BOOTLDR_IFVER) || NASSAU_PATCH_BOOTLDR_IFVER >= 2
#define BOOTLDR_CMDSET(ctxp, cmdbufp, cmdbuflen, dispatchp)                 \
    (*bootldr_funcps.cmdset)((ctxp), (cmdbufp), (cmdbuflen), (dispatchp))
#define BOOTLDR_SANITY_PANIC(code)                                          \
    (*bootldr_funcps.sanity_panic)((code))
#endif      /* !defined(NASSAU_PATCH_BOOTLDR_IFVER)
             * || NASSAU_PATCH_BOOTLDR_IFVER >= 2 */

#if !defined(NASSAU_PATCH_BOOTLDR_IFVER) || NASSAU_PATCH_BOOTLDR_IFVER >= 3
#define BOOTLDR_IDLSET(ctxp, idlep)                                         \
    (*bootldr_funcps.idleset)((ctxp), (idlep))
#define BOOTLDR_INTRSET(ctxp, userintrmask, intrp)                          \
    (*bootldr_funcps.intrset)((ctxp), (userintrmask), (intrp))
#endif      /* !defined(NASSAU_PATCH_BOOTLDR_IFVER)
             * || NASSAU_PATCH_BOOTLDR_IFVER >= 3 */


#endif      /* !defined(__STDC__) */
#endif      /* !defined(__BOOTLDR_H) */




