/*! @file vcsUtil.h
 *******************************************************************************
 **
 **                           NDA AND NEED-TO-KNOW REQUIRED
 **
 *****************************************************************************
 **
 **  Copyright (C) 2006-2016 Synaptics Incorporated. All rights reserved.
 **
 **
 ** This file contains information that is proprietary to Synaptics
 ** Incorporated ("Synaptics"). The holder of this file shall treat all
 ** information contained herein as confidential, shall use the
 ** information only for its intended purpose, and shall not duplicate,
 ** disclose, or disseminate any of this information in any manner unless
 ** Synaptics has otherwise provided express, written permission.
 ** Use of the materials may require a license of intellectual property
 ** from a third party or from Synaptics. Receipt or possession of this
 ** file conveys no express or implied licenses to any intellectual
 ** property rights belonging to Synaptics.
 **
 **
 ** INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS," AND
 ** SYNAPTICS EXPRESSLY DISCLAIMS ALL EXPRESS AND IMPLIED WARRANTIES,
 ** INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 ** PARTICULAR PURPOSE, AND ANY WARRANTIES OF NON-INFRINGEMENT OF ANY
 ** INTELLECTUAL PROPERTY RIGHTS. IN NO EVENT SHALL SYNAPTICS BE LIABLE
 ** FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE, OR
 ** CONSEQUENTIAL DAMAGES ARISING OUT OF OR IN CONNECTION WITH THE USE OF
 ** THE INFORMATION CONTAINED IN THIS DOCUMENT, HOWEVER CAUSED AND BASED
 ** ON ANY THEORY OF LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 ** NEGLIGENCE OR OTHER TORTIOUS ACTION, AND EVEN IF SYNAPTICS WAS ADVISED
 ** OF THE POSSIBILITY OF SUCH DAMAGE. IF A TRIBUNAL OF COMPETENT
 ** JURISDICTION DOES NOT PERMIT THE DISCLAIMER OF DIRECT DAMAGES OR ANY
 ** OTHER DAMAGES, SYNAPTICS' TOTAL CUMULATIVE LIABILITY TO ANY PARTY
 ** SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.
*/


/*!
*******************************************************************************
**  C Library Interface Definition
**
**  This file defines some of the basic C library functions. This makes
**  porting to other platforms easier, where the C run-time libraries
**  are not available or fully functional.
**
*/

#ifndef __vcsUtil_h__
#define __vcsUtil_h__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if defined(VCS_EFI) && defined(EFI_DEBUG)
#pragma runtime_checks( "", off )
#endif

#if defined(macintosh)
  # define TPI_OVERRIDE_LIBC
  # include <size_t.h>
#elif defined(linux)
  #include <stdio.h> 
#elif defined(LINUX_DRIVER)
 #include <linux/types.h>
#elif defined(__MACH__) && defined(__APPLE__)
  # include <sys/types.h>
#else
  # if !defined(_SIZE_T_DEFINED) && !defined(_SIZE_T_DEFINED_)
    #  if defined(_WIN64) 
      #if !defined(VCS_OS_WINBIO)  /*  avoid linking error when generate win bio driver */
      typedef unsigned __int64 size_t;
      #endif
    #  else
          #if defined(VCS_OS_QSEE_LLVM) || defined(VCS_OS_MTK_LLVM)
          typedef unsigned long size_t;
          #else
          typedef unsigned int size_t;
          #endif
    #  endif
  #  define _SIZE_T_DEFINED
  #  define _SIZE_T_DEFINED_
  # endif  /* _SIZE_T_DEFINED */
#endif

#if !defined(_PRINTF_FORMAT)
# if defined(__GNUC__)
#  define _PRINTF_FORMAT(x, y) __attribute__ ((format(printf, x, y)))
#  define _PRINTF_DECL
#elif defined(_WIN32)
#  define _PRINTF_FORMAT(x, y)
#  define _PRINTF_DECL  __cdecl
# else
#  define _PRINTF_FORMAT(x, y)
#  define _PRINTF_DECL
# endif
#else
# define _PRINTF_DECL
#endif

#define TPI_SIZE_OF_ARRAY(arr) (sizeof(arr) / sizeof(arr[0]))

/* Variable argument list definitions */
#ifdef _WIN64 /* Win64 always pushes 64-bit arguments on the stack */
# define TPI_INTSIZEOF(n) ((sizeof(n) + sizeof(int*) - 1) & ~(sizeof(int*) - 1))
#else
# define TPI_INTSIZEOF(n) ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))
#endif

/*
// Include TPI routines in all debug builds, but not the release build
// if they are explicitly excluded by defining NO_TPI_UTILS
*/
#if (defined(DBG) && (DBG != 0)) || defined(DEBUG) || defined(_DEBUG) || !defined(NO_TPI_UTILS)

#if defined(__GNUC__) && (defined(__x86_64__) || defined(__aarch64__))
/* This is to fix crashes under linux x86_64 with gcc.
 * May also be necessary with 32-bit too. -- APR
 */
#include <stdarg.h>
typedef va_list tpi_va_list;
#define tpi_va_start(ap,v)  va_start(ap,v)
#define tpi_va_arg(ap,t)    va_arg(ap,t)
#define tpi_va_end(ap)      va_end(ap)
#else
typedef char * tpi_va_list;
#define tpi_va_start(ap,v)  (ap = (tpi_va_list)&v + TPI_INTSIZEOF(v))
#define tpi_va_arg(ap,t)    (*(t *)((ap += TPI_INTSIZEOF(t)) - TPI_INTSIZEOF(t)))
#define tpi_va_end(ap)      (ap = (tpi_va_list)0)
#endif

#if defined(TPI_OVERRIDE_LIBC)
# if !defined(isalnum)
#  define isalnum        tpi_isalnum
#  define isalpha        tpi_isalpha
#  define iscntrl        tpi_iscntrl
#  define isdigit        tpi_isdigit
#  define isgraph        tpi_isgraph
#  define islower        tpi_islower
#  define isprint        tpi_isprint
#  define ispunct        tpi_ispunct
#  define isspace        tpi_isspace
#  define isupper        tpi_isupper
#  define isxdigit       tpi_isxdigit
#  define isascii        tpi_isascii
#  define toascii        tpi_toascii
# endif

# define tpi_memcpy      memcpy
# define tpi_memset      memset
# define tpi_memcmp      memcmp
# define tpi_memchr      memchr
# define tpi_strlen      strlen
# define tpi_strcpy      strcpy
# define tpi_strncpy     strncpy
# define tpi_strcat      strcat
# define tpi_strncat     strncat
# define tpi_strcmp      strcmp
# define tpi_strncmp     strncmp
# define tpi_stricmp     stricmp
# define tpi_strnicmp    strnicmp
# define tpi_strchr      strchr
# define tpi_strrchr     strrchr
# define tpi_strstr      strstr
# ifdef sprintf
#  undef sprintf
# endif
# define tpi_sprintf     sprintf
# define tpi_vsprintf    vsprintf
# ifndef  SIZE_OF_ARRAY
#  define SIZE_OF_ARRAY  TPI_SIZE_OF_ARRAY
# endif
#ifndef  _INTSIZEOF
# define _INTSIZEOF(n)   TPI_INTSIZEOF(n)
#endif

#ifndef va_start
typedef tpi_va_list     va_list;
# define va_start       tpi_va_start
# define va_arg         tpi_va_arg
# define va_end         tpi_va_end
#endif
#endif /* TPI_OVERRIDE_LIBC */

/****************************************************************************
 * Memory handling functions
 ****************************************************************************/
void *tpi_memcpy(void *dst, const void *src, size_t n);
void *tpi_memmove(void *dst, const void *src, size_t n);
void *tpi_memchr(const void *ptr, int val, size_t n);
int tpi_memcmp(const void *ptr1, const void *ptr2, size_t n);
void *tpi_memset(void *dst, int val, size_t n);

/****************************************************************************
 * Misc functions
 ****************************************************************************/
unsigned long tpi_min(unsigned long x, unsigned long y);
unsigned long tpi_max(unsigned long x, unsigned long y);
unsigned short tpi_swaps(unsigned short x);
unsigned long tpi_swapl(unsigned long x);

/****************************************************************************
 * Character handling functions
 ****************************************************************************/
#define _VCS_FMT_U   0x01  /* upper */
#define _VCS_FMT_L   0x02  /* lower */
#define _VCS_FMT_D   0x04  /* digit */
#define _VCS_FMT_C   0x08  /* cntrl */
#define _VCS_FMT_P   0x10  /* punct */
#define _VCS_FMT_S   0x20  /* white space (space/lf/tab) */
#define _VCS_FMT_X   0x40  /* hex digit */
#define _VCS_FMT_SP  0x80  /* hard space (0x20) */

extern unsigned char const _tpi_ctype[];

#define __tpi_ismask(x)     (_tpi_ctype[(int)(unsigned char)(x)])
#define tpi_isalnum(c)  ((__tpi_ismask(c)&(_VCS_FMT_U|_VCS_FMT_L|_VCS_FMT_D)) != 0)
#define tpi_isalpha(c)  ((__tpi_ismask(c)&(_VCS_FMT_U|_VCS_FMT_L)) != 0)
#define tpi_iscntrl(c)  ((__tpi_ismask(c)&(_VCS_FMT_C)) != 0)
#define tpi_isdigit(c)  ((__tpi_ismask(c)&(_VCS_FMT_D)) != 0)
#define tpi_isgraph(c)  ((__tpi_ismask(c)&(_VCS_FMT_P|_VCS_FMT_U|_VCS_FMT_L|_VCS_FMT_D)) != 0)
#define tpi_islower(c)  ((__tpi_ismask(c)&(_VCS_FMT_L)) != 0)
#define tpi_isprint(c)  ((__tpi_ismask(c)&(_VCS_FMT_P|_VCS_FMT_U|_VCS_FMT_L|_VCS_FMT_D|_VCS_FMT_SP)) != 0)
#define tpi_ispunct(c)  ((__tpi_ismask(c)&(_VCS_FMT_P)) != 0)
#define tpi_isspace(c)  ((__tpi_ismask(c)&(_VCS_FMT_S)) != 0)
#define tpi_isupper(c)  ((__tpi_ismask(c)&(_VCS_FMT_U)) != 0)
#define tpi_isxdigit(c) ((__tpi_ismask(c)&(_VCS_FMT_D|_VCS_FMT_X)) != 0)
#define tpi_isascii(c)  (((unsigned char)(c))<=0x7f)
#define tpi_toascii(c)  (((unsigned char)(c))&0x7f)

char tpi_toupper(char cp);
char tpi_tolower(char cp);

/****************************************************************************
 * String handling functions
 ****************************************************************************/
unsigned char tpi_basedigit(char digit, unsigned char base, unsigned char *pValue);
unsigned long tpi_strtoul(const char *cp, char **endp, unsigned char base);
char *tpi_strcat(char *dst, const char *src);
char *tpi_strncat(char *dst, const char *src, size_t n);
char *tpi_strcpy(char *dst, const char *src);
char *tpi_strncpy(char *dst, const char *src, size_t n);
char *tpi_strlwr(char *s);
char *tpi_strupr(char *s);
size_t tpi_strlen(const char *s);
size_t tpi_strnlen(const char *s, size_t n);
int tpi_strcmp(const char *s1, const char *s2);
int tpi_strncmp(const char *s1, const char *s2, size_t n);
int tpi_strcasecmp(const char *s1, const char *s2);
int tpi_strncasecmp(const char *s1, const char *s2, size_t n);
size_t tpi_strspn(const char *s, const char *set);
char *tpi_strpbrk(const char *s, const char *set);
char *tpi_strchr(const char *s, int c);
char *tpi_strrchr(const char *s, int c);
char *tpi_strstr(const char *s1, const char *s2);
int tpi_vsprintf(char *buf, const char *fmt, tpi_va_list args);
int _PRINTF_DECL tpi_sprintf(char *buffer, const char *fmtString, ...) _PRINTF_FORMAT(2, 3);
int _PRINTF_DECL tpi_sprintf255(unsigned char *buf255, /* 255 bytes max! */const char *fmt, ...) _PRINTF_FORMAT(2, 3);

#endif /* (DBG != 0) || defined(DEBUG) || defined(_DEBUG) || !defined(NO_TPI_UTILS) */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __vcsUtil_h__ */

