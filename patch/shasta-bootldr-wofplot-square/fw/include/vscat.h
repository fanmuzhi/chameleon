/* -*- mode: c; tab-width: 4 -*- */
/*
 * Copyright (c) 2013-2015 Synaptics Incorporated.  All rights reserved.
 */

/*
 * Validity Static Code Analysis Tool (VSCAT) definitions.
 *
 * The Validity Static Code Analysis Tool (VSCAT) is a tool
 *  to analyze a linked firmware image.  It works by scanning
 *  through the code, looking for 'entry' instructions (to
 *  figure out stack depth) and 'call' instructions (to figure
 *  out the call graph).
 * Things get difficult with indirect function pointers.  The
 *  compiler doesn't provide any mechanism to hint as to what
 *  function a function pointer could point to.
 * This file includes the VSCAT_MAYCALL macro.  This macro allows
 *  us to annotate the source with information about what
 *  functions may be being called indirectly.
 *
 * Bjoren Davis, March 6, 2013.
 *
 * Adapted for general use from engineering/private/impl/micro/windsor/fwrom/src/vfk/vscat.h,v 1.1 2013/03/10 21:23:25 bdavis
 *  Bjoren Davis, July 14, 2015.
 */

#ifndef __VSCAT_H
#define __VSCAT_H
/*
 * Common (C/assembler) defines.
 */
#define _VSCAT_QUOTE(x)     #x
#define _VSCAT_QUOTE2(x)    _VSCAT_QUOTE(x)

#if defined(__ASSEMBLER__)
/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
/*
 * The assembler version of this header.
 */

    .macro  vscat_hint_maycall  filename, line, funcnames:vararg
    .pushsection    .vscat_hint_maycall
    .align  4
    .word   1f
    .word   \line
    .asciz  "\filename"
    .irp    funcname,\funcnames
    .asciz  "\funcname"
    .endr         
    .byte   0
    .popsection
1:
    .endm
    
         
#define VSCAT_HINT_MAYCALL(...)                                             \
    vscat_hint_maycall __FILE__, __LINE__, __VA_ARGS__


    .macro  vscat_hint_maycallpool  filename, line, poolnames:vararg
    .pushsection    .vscat_hint_maycallpool
    .align  4
    .word   1f
    .word   \line
    .asciz  "\filename"
    .irp    poolname,\poolnames
    .asciz  "\poolname"
    .endr         
    .byte   0
    .popsection
1:
    .endm
    
         
#define VSCAT_HINT_MAYCALLPOOL(...)                                         \
    vscat_hint_maycallpool  __FILE__, __LINE__, __VA_ARGS__

         
#elif defined(__STDC__)

/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */

/*
 * The C version of this header
 */

/* -------------------------------------------------------------------------- */
/* General macros */
#if defined(NASSAU_BEHAVIOR_MODEL)
void vs_vscat_inthreadtop(void);
#define VSCAT_HINT_MAYCALLPOOL(...)
#define VSCAT_INCALLPOOL(...)
#define VSCAT_HINT_MAYCALL(...)
#define VSCAT_INTHREADTOP() vs_vscat_inthreadtop()

#else  /* NASSAU_BEHAVIOR_MODEL */
/*
 * Inform VSCAT that the following indirect function call can call
 *  the given function(s).
 * Example use:
 *    VSCAT_MAYCALL("usb_init");
 * or
 *    VSCAT_MAYCALL("usb_init", "spicmd_init");
 *
 * This macro works by causing the assembler to tuck away some
 *  notes about this call in a section called '.vscat_maycall'.
 */
#define VSCAT_HINT_MAYCALL(...)                                             \
    __asm__ __volatile__ ("\n"                                              \
"   .pushsection    .vscat_hint_maycall\n"                                  \
"   .align  4\n"                                                            \
"   .word   1f\n"                                                           \
"   .word   " _VSCAT_QUOTE2(__LINE__) "\n"                                     \
"   .asciz  " _VSCAT_QUOTE2(__FILE__) "\n"                                     \
"   .asciz  " #__VA_ARGS__ "\n"                                             \
"   .byte   0\n"                                                            \
"   .popsection\n"                                                          \
"1:\n" )

/*
 * We can define "pools" of indirect function calls (e.g.
 *  DPCs, timer callbacks, etc.).
 * Functions can declare themselves to be in one or more
 *  pools using the VSCAT_INCALLPOOL() macro.
 * Code that actually calls functions from a pool or pools
 *  can use the VSCAT_MAYCALLPOOL() macro.
 */

#define VSCAT_HINT_MAYCALLPOOL(...)                                         \
    __asm__ __volatile__ ("\n"                                              \
"   .pushsection    .vscat_hint_maycallpool\n"                              \
"   .align  4\n"                                                            \
"   .word   1f\n"                                                           \
"   .word   " _VSCAT_QUOTE2(__LINE__) "\n"                                     \
"   .asciz  " _VSCAT_QUOTE2(__FILE__) "\n"                                     \
"   .asciz  " #__VA_ARGS__ "\n"                                             \
"   .byte   0\n"                                                            \
"   .popsection\n"                                                          \
"1:\n" )

/*
 * Provide a limit to the level of recursion that this function
 *  can make.
 */

#define VSCAT_RECURSELIMIT(val)                                             \
    __asm__ __volatile__ ("\n"                                              \
"   .pushsection    .vscat_recurselimit\n"                                  \
"   .align  4\n"                                                            \
"   .word   1f\n"                                                           \
"   .word   " #val "\n"                                                     \
"   .word   " _VSCAT_QUOTE2(__LINE__) "\n"                                     \
"   .asciz  " _VSCAT_QUOTE2(__FILE__) "\n"                                     \
"   .popsection\n"                                                          \
"1:\n" )

/*
 * Add the given function to a given pool or pools.
 */

#define VSCAT_INCALLPOOL(...)                                               \
    __asm__ __volatile__ ("\n"                                              \
"   .pushsection    .vscat_callpool\n"                                      \
"   .align  4\n"                                                            \
"   .word   1f\n"                                                           \
"   .word   " _VSCAT_QUOTE2(__LINE__) "\n"                                     \
"   .asciz  " _VSCAT_QUOTE2(__FILE__) "\n"                                     \
"   .asciz  " #__VA_ARGS__ "\n"                                             \
"   .byte   0\n"                                                            \
"   .popsection\n"                                                          \
"1:\n" )

/*
 * Add an arbitrary function to a given pool or pools.
 *  This macro is meant to be used outside of a function definition.
 */

#define VSCAT_ADDTOCALLPOOL(func, ...)                                      \
    __asm__ ("\n"                                                           \
"   .pushsection    .vscat_callpool\n"                                      \
"   .align  4\n"                                                            \
"   .word   " #func "\n"                                                    \
"   .word   " _VSCAT_QUOTE2(__LINE__) "\n"                                     \
"   .asciz  " _VSCAT_QUOTE2(__FILE__) "\n"                                     \
"   .asciz  " #__VA_ARGS__ "\n"                                             \
"   .byte   0\n"                                                            \
"   .popsection\n")

/*
 * Tell vscat that we're in a function at the top of a thread.
 */

#define VSCAT_INTHREADTOP()                                                 \
    __asm__ ("\n"                                                           \
"   .pushsection    .vscat_threadtop\n"                                     \
"   .align  4\n"                                                            \
"   .word   1f\n"                                                           \
"   .word   " _VSCAT_QUOTE2(__LINE__) "\n"                                     \
"   .asciz  " _VSCAT_QUOTE2(__FILE__) "\n"                                     \
"   .popsection\n"                                                          \
"1:\n")

#endif      /* NASSAU_BEHAVIOR_MODEL */



#endif      /* defined(__STDC__) */

#endif      /* !__VSCAT_H */
