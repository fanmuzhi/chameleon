 /*! @file vcsDebugBaseEx.h
 *******************************************************************************
 ** Android Fingerprint HAL Implementation for Synaptics Fingerprint Sensors
 **
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
**  Debug Interface Definition
**
**  This file defines a set of debug macros that can be used to instrument
**  C/C++ source code. The macros defined here are only compiled in the code
**  when compiler defines DEBUG, _DEBUG, or DBG != 0. This allows you to
**  add as much instrumentation to your code as you like, and then have it
**  all compiled out for the final release.
**
*/

#ifndef __vcsDebugBaseEx_h__
#define __vcsDebugBaseEx_h__

#include "vcsBase.h"
#include "vcsUtil.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

//#define  DBG_FORCE_ENABLE_FILE 1

/* NT builds define DBG=0 or DBG=1, so we use that throughout */
#if (defined(DBG) && (DBG != 0)) || defined(DEBUG) || defined(_DEBUG)
    #undef  DBG
    #define DBG 1
#else
    #undef  DBG
    #define DBG 0
#endif

/* In case these aren't defined in the current environment. */
#ifndef IN
    #define IN
    #define OUT
#endif

#ifndef _PRINTF_FORMAT
    #ifdef __GNUC__
        #define _PRINTF_FORMAT(x, y) __attribute__ ((format(printf, x, y)))
    #else
        #define _PRINTF_FORMAT(x, y)
    #endif
#endif

#ifndef _DBG_VALUE
    #define _DBG_VALUE(x)      { x, #x }
#endif

/****************************************************************************/
#if DBG /* When (DBG != 0) we enable all the debug macros and functions. */

/* Generic debug filtering flags */
# define DBG_ALL_OFF        0x00000000L /* Disable all debug */
# define DBG_ERROR_ON       0x00000001L /* Should never happen */
# define DBG_WARNING_ON     0x00000002L /* Unusual event */
# define DBG_NOTICE_ON      0x00000004L /* Interesting information */
# define DBG_TRACE_ON       0x00000008L /* Function tracing */
# define DBG_PARAM_ON       0x00000010L /* Display interface parameters */
# define DBG_INFO_ON        0x00000020L /* Display informational messages */
# define DBG_DATA_ON        0x00000040L /* Data tracing */
# define DBG_VERBOSE_ON     0x00000080L /* Verbose data tracing */
# define DBG_STATUS_ON      0x00000100L /* Important status message */
# define DBG_EVENT_ON       0x00000200L /* Important event message */
# define DBG_BREAK_INIT_ON  0x00004000L /* Breakpoint on DBG_INIT */
# define DBG_BREAK_ON       0x00008000L /* Breakpoint on error */
# define DBG_FATAL_ON       0x00010000L /* Abnormal exit or termination */
# define DBG_ALL_ON         0xFFFFFFFFL /* Enable all debug */

/* Application specific filtering flags */
# define DBG_GEN_SYS        0x00000001L /* Generic subsystem */
# define DBG_HW_SYS         0x00000002L /* Hardware subsystem */
# define DBG_INIT_SYS       0x00000004L /* Initialization handler */
# define DBG_HALT_SYS       0x00000008L /* Termination handler */
# define DBG_ALL_SYS        0xFFFFFFFFL /* Enable all sub-systems */

# define DBG_DEBUGGER_ON    0x00000001L /* Output to debug console */
# define DBG_LOG_ON         0x00000002L /* Log to circular memory queue */
# define DBG_FILE_ON        0x00000004L /* Output to file (if supported) */
# define DBG_MESSAGE_ON     0x00000008L /* Output to MessageBox (if supported) */

# ifndef   DBG_ENABLE_DEBUGGER  /* Enable output to debug console */
#  if !defined(VCS_OS_WINCE) || (UNDER_CE > 200)
#   define DBG_ENABLE_DEBUGGER  DBG_DEBUGGER_ON
#  else
#   define DBG_ENABLE_DEBUGGER  0
#  endif
# endif

# ifndef   DBG_ENABLE_LOGGING   /* Enable output to circular log */
#  if defined(VXWORKS) || defined(VCS_OS_BCMUSH) || defined(VCS_EFI)
#   define DBG_ENABLE_LOGGING   0
#  elif defined(VCS_OS_QSEE) || defined(VCS_OS_MOBICORE) || defined(VCS_OS_SSTZ)
#   define DBG_ENABLE_LOGGING   0
#  else
#   define DBG_ENABLE_LOGGING   DBG_LOG_ON
#  endif
# endif

# ifndef   DBG_ENABLE_FILE      /* Enable output to file */
#  if defined(VCS_OS_WINCE) || defined(DBG_FORCE_ENABLE_FILE)
#   if (!VCS_FEATURE_LOG_TO_FILE)
#    define DBG_ENABLE_FILE      DBG_FILE_ON
#   elif
#    define DBG_ENABLE_FILE     0
#   endif
#  else
#   define DBG_ENABLE_FILE      0
#  endif
# endif

# ifndef  DBG_ENABLE_MESSAGE    /* Disable output to MessageBox by default */
#  define DBG_ENABLE_MESSAGE    0 /* DBG_MESSAGE_ON */
# endif

# ifndef  DBG_ENABLE_TIMESTAMP  /* Disable timestamp output by default */
#  define DBG_ENABLE_TIMESTAMP  1
# endif

# ifndef  DBG_ENABLE_THREAD     /* Disable thread output for drivers */
#  if defined(__KERNEL__) || defined(_SYS_) || defined(_VXD_)
#   define DBG_ENABLE_THREAD    0
#  else
#   define DBG_ENABLE_THREAD    1
#  endif
# endif

# ifndef  DBG_ENABLE_PROC       /* Disable processor ID output by default */
#  define DBG_ENABLE_PROC       0
# endif

# ifndef  DBG_SYSTEMS
#  define DBG_SYSTEMS           DBG_ALL_SYS
# endif

# ifndef  DBG_DEFAULTS
#  define DBG_DEFAULTS          (DBG_BREAK_ON | DBG_ERROR_ON | DBG_WARNING_ON)
# endif

# ifndef  DBG_MESSAGES
#  define DBG_MESSAGES          (DBG_BREAK_ON | DBG_ERROR_ON)
# endif

# ifndef  DBG_OUTPUTS
#  define DBG_OUTPUTS   \
    (DBG_ENABLE_DEBUGGER | DBG_ENABLE_LOGGING | DBG_ENABLE_FILE | \
    DBG_ENABLE_MESSAGE)
# endif

# ifndef  DBG_UNUSED
#  define DBG_UNUSED            {(void)__FUNC__; (void)__MASK__;}
# endif

# ifndef  DBG_ENV_FLAGS
#  define DBG_ENV_FLAGS         "VCSDBG_FLAGS"
# endif

# ifndef  DBG_ENV_SYSTEMS
#  define DBG_ENV_SYSTEMS       "VCSDBG_SYSTEMS"
# endif

typedef unsigned int    DBG_UINT_PTR;

#if defined(_WIN64)
    typedef unsigned __int64    DBG_ULONG_PTR;
#else
    typedef unsigned long       DBG_ULONG_PTR;
#endif

#if defined(_NTDDK_) || defined(_SYS_) || defined(_NTDRIVER_) || defined(NDIS_MINIPORT_DRIVER)
    #define PROCESSOR_BREAKPOINT  DbgBreakPoint()
#elif defined(_M_IX86) || defined(_M_I86)
    #define PROCESSOR_BREAKPOINT  __asm{int 3}
#elif defined(_WIN64) || defined(_M_ARM)
    #if !defined(VCS_EFI) && !defined(VCS_OS_SGX)
        #define PROCESSOR_BREAKPOINT  DebugBreak()
    #else
        #define PROCESSOR_BREAKPOINT
    #endif
#elif defined(linux)
    #if defined(__KERNEL__)
        #if defined(i386) || defined(i486) || defined(i586) || defined(i686)
            #define KDB_BREAKPOINT       asm volatile ("int $129");
            #define PROCESSOR_BREAKPOINT KDB_BREAKPOINT
        #else
            #warning "WARNING: You need to define PROCESSOR_BREAKPOINT for this processor!"
            #define KDB_BREAKPOINT
            #define PROCESSOR_BREAKPOINT
        #endif
    #else /* __KERNEL__ */
        #include <stdlib.h>
        #define PROCESSOR_BREAKPOINT abort()
    #endif /* __KERNEL__ */
#elif defined(VXWORKS)
    #define PROCESSOR_BREAKPOINT  abort()
#elif defined(VCS_OS_MTK)
    #define PROCESSOR_BREAKPOINT
#else
    #include <stdlib.h>
    #define PROCESSOR_BREAKPOINT  abort()
# endif /* _NTDDK_ */

# ifndef  DBG_TRAP
#  define DBG_TRAP          PROCESSOR_BREAKPOINT
# endif

# ifndef  DBG_BREAKPOINT
#  define DBG_BREAKPOINT    PROCESSOR_BREAKPOINT
# endif

# ifndef  DBG_FILE_PATH
#  ifdef VCS_OS_ANDROID
#   define DBG_FILE_PATH     "/data/validity/%s.log"   /* %s replaced with DBG_NAME */

#   if VCS_FEATURE_LOG_TO_FILE
#     undef DBG_FILE_PATH
#     define DBG_FILE_PATH     				"/data/log/validity/%s.log"   /* %s replaced with DBG_NAME */
#     define DBG_FILE_ROTATE_PATH			"/data/log/validity/%s_%02d.log"   /* %s replaced with DBG_NAME, %d replaced with log rotate filenumber */
#     define DBG_FILE_ROTATE_COUNT_PATH		"/data/log/validity/logFileIndex"    /* stores index of the log file */
#     define DBG_MAX_BACKUP_FILE_CNT			10			/* Number of files to be generated*/
#     define DBG_MAX_LINES_PER_FILE				25000		/* Number of lines per fille*/
#     define DBG_FILE_PERMEATIONS				0660		/* File Permutations */
	  void setLogToFile(void);
#   endif

#  else
#    ifdef VCS_OS_LINUX
#     define DBG_FILE_PATH     "/var/run/%s.log"   /* %s replaced with DBG_NAME */
#    else
#     define DBG_FILE_PATH     "\\TEMP\\%s_%d.txt"   /* %s replaced with DBG_NAME */
#    endif
#  endif
# endif

# ifndef  DBG_BUFFER_LEN
#  ifdef VCS_OS_BCMUSH
#   define DBG_BUFFER_LEN    511            /* Max debug output string length. */
#  else
#   define DBG_BUFFER_LEN    1023           /* Max debug output string length. */
#  endif
# endif

# if (DBG_BUFFER_LEN < 511)
#  error("ERROR: DBG_BUFFER_LEN must be >= 511!")
# endif

# ifndef  DBG_THREAD_COUNT
#  if defined(WIN16) || defined(_SYS_) || defined(_VXD_) || defined(__KERNEL__) || defined(VXWORKS)
#   define DBG_THREAD_COUNT 4               /* Max number of active debug threads. */
#  elif defined(VCS_OS_BCMUSH) /* for BCM target  */
#   define DBG_THREAD_COUNT 1               /* BCMUSH is single threaded. */
#  elif defined(VCS_OS_QSEE) || defined(VCS_OS_MOBICORE) || defined(VCS_OS_SSTZ) || defined(VCS_EFI)
#   define DBG_THREAD_COUNT 1               /* TZ is single threaded. */
#  else
#   define DBG_THREAD_COUNT 32              /* Max number of active debug threads. */
#  endif
# endif

# ifndef DBG_BUFFER_COUNT
#  define DBG_BUFFER_COUNT   (DBG_THREAD_COUNT*2) /* 2 active buffers for each thread. */
# endif

# if (DBG_BUFFER_COUNT <= DBG_THREAD_COUNT)
#  error("ERROR: DBG_BUFFER_COUNT must be > DBG_THREAD_COUNT!")
# endif

# ifndef  DBG_LOG_SIZE
#  define DBG_LOG_SIZE      128-2*sizeof(short) /* Max number of bytes per entry. */
# endif

# ifndef  DBG_LOG_ENTRIES
#  ifdef WIN16
#   define DBG_LOG_ENTRIES   256            /* Maximum number of FIFO log entries. */
#  else
#   define DBG_LOG_ENTRIES   1024           /* Maximum number of FIFO log entries. */
#  endif
# endif

typedef struct DBG_STRUCT *PDBG_STRUCT;
typedef void (VCS_API * DBG_FUNC_TYPE)(char *buffer, unsigned long length);

typedef struct DBG_THREAD_BUFFER {
    char *                  Buffer;
    unsigned long           BufferIndex;
    long                    DbgLevel;
    unsigned long           ThreadId;
    PDBG_STRUCT             pDbgInfo;    /* context saved here before output */

}   DBG_THREAD_BUFFER, * PDBG_THREAD_BUFFER;

# if (DBG_ENABLE_LOGGING)
typedef struct DBG_LOG {
    unsigned short          Flags;
    unsigned short          NumBytes;
    unsigned char           Buffer[DBG_LOG_SIZE];

}   DBG_LOG, * PDBG_LOG;
# endif /* DBG_ENABLE_LOGGING */

/*
// Define an access macro for reading each debug field.
// Make sure you keep the access macros in sync with the DBG_FIELDS macro.
*/
# define DBG_FLAGS(A)       (A)->DbgFlags
# define DBG_SYSTEM(A)      (A)->DbgSystem
# define DBG_ID(A)          (A)->DbgId
# define DBG_NAME(A)        (A)->DbgName
# define DBG_NAME_BUFFER(A) (A)->DbgNameBuffer
# define DBG_FILE_HANDLE(A) (A)->DbgFileHandle
# define DBG_FILE_NAME(A)   (A)->DbgFileName
# define DBG_OUT_FLAGS(A)   (A)->DbgOutFlags
# define DBG_PRINT_FUNC(A)  (A)->DbgPrintFunc
# define DBG_ALT_FUNC(A)    (A)->DbgAltFunc

/*
// Always declare these debug fields at the top of your context specific data
// structure so your structure can be cast to a DBG_STRUCT when debugging.
// The global context structure pointed to by gDbgInfo can be used in those
// routines that don't have access to your context specific debug structure.
*/
# define DBG_FIELDS \
    unsigned long           DbgFlags;\
    unsigned long           DbgSystem;\
    unsigned long           DbgId;\
    const char *            DbgName;\
    const char *            DbgFileName;\
    void *                  DbgFileHandle;\

/*
// Pointer to the first field in the DBG_FIELDS structure
*/
# define DBG_STRUCT_PTR(A)  (PDBG_STRUCT)(&DBG_FLAGS(A))

/*
// This is the global debug structure which can be used when you have not
// created a context specific debug structure.  You can use pass gDbgInfo
// to any of the
*/
typedef struct DBG_STRUCT {
    DBG_FIELDS              /* Declare debug fields at top of structure. */

    unsigned long           DbgOutFlags;
    DBG_FUNC_TYPE           DbgPrintFunc;
    DBG_FUNC_TYPE           DbgAltFunc;

    /*
    // The global gDbgInfo structure also contains the other buffers and
    // variables used by the debug library and macros functions.  These
    // extra fields are only accessed via the gDbgInfo pointer and are
    // not part of the context specific data structure.
    // These fields should only be used by the debug libary and macros.
    */

    const char *            Systems[33];    /* 32 bits plus one for zero */

    /* Add an extra character so we know which buffers are in use. */
# if defined(VCS_OS_WINCE) || defined(UNICODE)
#  define  DBG_BUFFER_SIZEOF ((DBG_BUFFER_LEN+1)*2)
# else
#  define  DBG_BUFFER_SIZEOF (DBG_BUFFER_LEN+1)
# endif

    /* Allow for one extra thread so one is always available */
    DBG_THREAD_BUFFER       DbgThreadBuffers[DBG_THREAD_COUNT+1];

    /* Allow for one extra buffer so one is always available */
    unsigned char           DbgBuffers[DBG_BUFFER_COUNT+1][DBG_BUFFER_SIZEOF];
    unsigned long           DbgBufferCount;

# if (DBG_ENABLE_LOGGING)
    unsigned long           LogIndex;
    struct DBG_LOG          LogBuffer[DBG_LOG_ENTRIES];
# endif /* DBG_ENABLE_LOGGING */
#ifdef VCS_OS_ANDROID
    unsigned int             logPriority;
#endif/* VCS_OS_ANDROID */
}   DBG_STRUCT;

extern PDBG_STRUCT          gDbgInfo;       /* Always a global debug context */

extern void VCS_API DbgInit(
    PDBG_STRUCT             pDbgInfo,
    const char *            pDbgName,
    const char *            pDbgFileName,
    unsigned long           DbgId,
    unsigned long           DbgFlags,
    unsigned long           DbgSystems
    );

extern void VCS_API DbgHalt(
    const PDBG_STRUCT       pDbgInfo
    );

extern void VCS_API DbgSystemDefine(
    const char *            name,
    unsigned long           mask
    );

extern const char * VCS_API DbgSystemName(
    unsigned long           mask
    );

extern void VCS_API DbgOpenFile(
    const PDBG_STRUCT       pDbgInfo
    );

extern void VCS_API DbgCloseFile(
    const PDBG_STRUCT       pDbgInfo
    );

extern void VCS_API DbgSetThreadInfo(
    const PDBG_STRUCT       pDbgInfo
    );

extern int VCS_API DbgGetLevel(void);

extern int VCS_API DbgIncLevel(void);

extern int VCS_API DbgDecLevel(void);

extern const char * VCS_API DbgGetEnv(
    const char *            pEnvName
    );

extern unsigned long VCS_API DbgGetEnvULong(
    const char *            pEnvName,
    unsigned long           envValue
    );

extern unsigned long VCS_API DbgSetFlags(
    const PDBG_STRUCT       pDbgInfo,
    unsigned long           newFlags
    );

extern unsigned long VCS_API DbgGetFlags(
    const PDBG_STRUCT       pDbgInfo
    );

extern unsigned long VCS_API DbgSetSystems(
    const PDBG_STRUCT       pDbgInfo,
    unsigned long           newSystems
    );

extern unsigned long VCS_API DbgGetSystems(
    const PDBG_STRUCT       pDbgInfo
    );

extern unsigned long VCS_API DbgTimeStamp(void);

extern unsigned long VCS_API DbgThreadId(void);

extern unsigned long VCS_API DbgProcessorId(void);

# if !defined(WINNT) && !defined(_DBGNT_) && !defined(NDIS_WIN) && !defined(_INC_OSCALLS) && !defined(_MINIDRV_H_) && !defined(VCS_OS_MACOSX) || (defined(VCS_OS_MACOSX) && !KERNEL)
extern unsigned long VCS_API DbgPrint(
    char *                  format,
    ...
    ) _PRINTF_FORMAT(1, 2);
# endif /* _DBGNT_ */

extern void VCS_API DbgMessageOut(
    char *                  buffer,
    unsigned long           length
    );

extern void VCS_API DbgPrintOut(
    char *                  buffer,
    unsigned long           length
    );

extern void VCS_API TPI_DbgPrintf(
    const char *            format,
    ...
    ) _PRINTF_FORMAT(1, 2);

extern void VCS_API DbgPrintData(
    const PDBG_STRUCT       pDbgInfo,
    const unsigned char *   pData,
    unsigned long           numBytes,
    unsigned long           offset
    );

extern void VCS_API DbgQueueData(
    const unsigned char *   pData,
    unsigned long           numBytes,
    unsigned short          flags
    );

extern char * VCS_API DbgDataToString(
    const unsigned char *   pData,
    unsigned long           offset,
    unsigned long           length,
    char *                  pBuffer
    );

extern char * VCS_API DbgDataToHexString(
    const unsigned char *   pData,
    unsigned long           offset,
    unsigned long           length,
    char *                  pBuffer
    );

extern vcsUint32_t DbgGetLogLevel(void);
extern void DbgSetLogLevel(vcsUint32_t level);

extern void VCS_API DbgSetPriority(
    const PDBG_STRUCT       pDbgInfo,
    unsigned int            priority
    );

extern const char *_DBG_ENTER_STR;
extern const char *_DBG_LEAVE_STR;


/* define VCS_FEATURE_ENABLE_MINIMAL_LOG to enable only DBG_ERROR, DBG_WARNING and DBG_STATUS */
#if VCS_FEATURE_ENABLE_MINIMAL_LOG
# define DBG_PREFIX(A,S)
# define DBG_NOTICE(A,S)
# define DBG_FILTER(A,M,S)
# define DBG_DATA(A,B,N,O)
# define DBG_QDATA(A,B,N,F)
# define VCSAPI_ENTER(A, S)
# define VCSAPI_LEAVE(A, S)
# define DBG_TRACE(A)
# define DBG_INFO(A,S)
//# define DBG_STATUS(A,S)
# define DBG_EVENT(A,S)
# define DBG_RETURN(A,S)    {(void)__MASK__;}
# define DBG_ENTER(A)           {(void)__MASK__;}
# define DBG_LEAVE(A)           {(void)__MASK__;}
#endif /*  VCS_FEATURE_ENABLE_MINIMAL_LOG */

# define DBG_DEF_SYSTEM(N,M) DbgSystemDefine(N,M)
# define DBG_SYSNAME()       DbgSystemName(__MASK__)

# ifndef  DBG_INIT
#  define DBG_INIT2(A,N,F,I,M,S) {\
	DbgInit(DBG_STRUCT_PTR(A), N, F, I, M, S); \
	DBG_PREFIX(A, "DBG_INIT"); \
	DBG_PRINT(A, ("DebugFlags @%p=0x%lX  ", \
	&(DBG_FLAGS(A)), DBG_FLAGS(A))); \
	DBG_PRINT(A, ("DebugSystems @%p=0x%lX\n", \
	&(DBG_SYSTEM(A)), DBG_SYSTEM(A))); \
if (_DBG_ON(A, DBG_BREAK_INIT_ON)) DBG_BREAK(A); \
}
#  define DBG_INIT(A,N,I,M,S)  DBG_INIT2(A,N,(char *)0,I,M,S)
# endif

# ifndef  DBG_HALT
#  define DBG_HALT(A)       {\
	DBG_PREFIX(A, "DBG_HALT"); \
	DBG_PRINT(A, ("\n")); \
	DbgHalt(DBG_STRUCT_PTR(A)); \
	DBG_OUT_FLAGS(A) = DBG_OUTPUTS; \
	DBG_FLAGS(A) = DBG_ALL_ON; \
	DBG_SYSTEM(A) = DBG_ALL_SYS; \
}
# endif


#if VCS_FEATURE_ENABLE_MINIMAL_LOG

#ifndef DBG_FUNC
# define DBG_FUNC(F)        static unsigned long __MASK__ = DBG_GEN_SYS;
#endif
#ifndef DBG_FUNCX
# define DBG_FUNCX(F,M)     static unsigned long __MASK__ = M;
#endif
#ifdef  DBG_UNUSED
    #undef DBG_UNUSED
# endif
#define DBG_UNUSED {(void)__MASK__;}

#else   /* VCS_FEATURE_ENABLE_MINIMAL_LOG */

#ifndef DBG_FUNC
# define DBG_FUNC(F)        static const char __FUNC__[] = F; \
                            static unsigned long __MASK__ = DBG_GEN_SYS;

#endif
#ifndef DBG_FUNCX
    /* Windows arm target will have to be checked!! */
#if (defined (VCS_OS_WINDOWS) || defined (VCS_OS_LINUX)) && (defined (_X64_) || defined (_X86_))
# define DBG_FUNCX(F,M)     static const char __FUNC__[] = F;\
    static unsigned long __MASK__ = M;
#else /* for non-windows/linux targets on non-x86/x64 architechtures */
# define DBG_FUNCX(F,M)     static const char __FUNC__[] = F;	\
    static unsigned long __MASK__ = M;					\
    __MASK__ = __MASK__ + 0; //prevent unused variable warnings rodzzz
#endif /* (defined (VCS_OS_WINDOWS) || defined (VCS_OS_LINUX)) && !(defined (_X64_) || defined (_X86_)) */
#endif
#endif  /* DBG_MINIMAL */

# ifndef  DBG_PRINT

# if defined(VCS_OS_QSEE)

/* Intermediate macro with reduced first parameter. */
/* FIXME: the qsee log level is fixed to 5 (QSEE_LOG_MSG_DEBUG) which is
defined in qsee_log.h Need to review implementation of DBG_PRINT macro
to use palQSEE function instead of direct call of qsee_log. */
extern void qsee_log(unsigned char pri, const char* fmt, ...);

#define __VAL_QSEE_LOG__(...)   qsee_log(5, __VA_ARGS__)

#  define DBG_PRINT(A,S)    {DbgSetThreadInfo(DBG_STRUCT_PTR(A)); __VAL_QSEE_LOG__ S;}

# elif defined(VCS_OS_SGX)

extern void VCS_API SgxDbgPrint(char * format, ...);

#  define DBG_PRINT(A,S)    {DbgSetThreadInfo(DBG_STRUCT_PTR(A)); SgxDbgPrint S;}

# elif defined(VCS_OS_GP)

/* Intermediate macro with reduced first parameter. */
/* FIXME: the qsee log level is fixed to 5 (QSEE_LOG_MSG_DEBUG) which is
defined in qsee_log.h Need to review implementation of DBG_PRINT macro
to use palQSEE function instead of direct call of qsee_log. */
extern void SLog(const char *fmt, ...);


#define __VAL_GP_LOG__(...)   SLog(__VA_ARGS__)

#define DBG_PRINT(A,S)    {DbgSetThreadInfo(DBG_STRUCT_PTR(A)); __VAL_GP_LOG__ S;}
# elif defined(VCS_OS_MTK)

/* Intermediate macro with reduced first parameter. */
#define __PRINTFLIKE(__fmt,__varargs) __attribute__((__format__ (__printf__, __fmt, __varargs)))
extern int printf(const char *fmt, ...) __PRINTFLIKE(1,2);

#define __VAL_MTK_LOG__(...)   printf(__VA_ARGS__)

#define DBG_PRINT(A,S)    {DbgSetThreadInfo(DBG_STRUCT_PTR(A)); __VAL_MTK_LOG__ S;}

# elif defined(VCS_OS_MOBICORE)

extern void tlApiLogPrintf(
                           const char *fmt,
                           ...);

// NOTE: logging and debug printing are now moved to separate header file.
#define __VAL_MOBICORE_LOG__(...)   tlApiLogPrintf(__VA_ARGS__)

#  define DBG_PRINT(A,S)    {DbgSetThreadInfo(DBG_STRUCT_PTR(A)); __VAL_MOBICORE_LOG__ S;}
# elif defined(VCS_OS_SSTZ)

typedef void (*fpTZ_LOG) (vcsUint8_t pri, const char * format, ...);
extern fpTZ_LOG LOG_MSG;

#define __VAL_SSTZ_LOG__(...)   (*LOG_MSG)(5, __VA_ARGS__)

#  define DBG_PRINT(A,S)    {DbgSetThreadInfo(DBG_STRUCT_PTR(A)); __VAL_SSTZ_LOG__ S;}

# else
#  define DBG_PRINT(A,S)    {DbgSetThreadInfo(DBG_STRUCT_PTR(A)); TPI_DbgPrintf S;}
# endif
# endif

# if DBG_ENABLE_PROC
#  define _DBG_PROC_VAL()   ,DbgProcessorId()
#  define _DBG_PROC_FMT     "[%lu]"
# else
#  define _DBG_PROC_VAL()
#  define _DBG_PROC_FMT
# endif

# if DBG_ENABLE_TIMESTAMP
#  define _DBG_TIME_VAL()   ,DbgTimeStamp()
#  define _DBG_TIME_FMT     "%08lu:"
# else
#  define _DBG_TIME_VAL()
#  define _DBG_TIME_FMT
# endif

# if DBG_ENABLE_THREAD
#  define _DBG_THREAD_VAL() ,DbgThreadId()
#  define _DBG_THREAD_FMT   "%lX:"
# else
#  define _DBG_THREAD_VAL()
#  define _DBG_THREAD_FMT
# endif

# ifndef  DBG_MODULE
#if defined(VCS_OS_QSEE) || defined(VCS_OS_MOBICORE) || defined(VCS_OS_SSTZ) || defined(VCS_EFI) || defined(VCS_OS_SGX)
#   define DBG_MODULE(A)     DBG_PRINT(A,("%s:%lu:", DBG_NAME(A), DBG_ID(A)))
#  else /* VCS_OS_QSEE */
#   define DBG_MODULE(A)     DBG_PRINT(A,(_DBG_PROC_FMT _DBG_TIME_FMT _DBG_THREAD_FMT "%s:%lu:" _DBG_PROC_VAL() _DBG_TIME_VAL() _DBG_THREAD_VAL(), DBG_NAME(A), DBG_ID(A)))
# endif /* !VCS_OS_QSEE */
#endif

# ifndef  DBG_PREFIX
#  define DBG_PREFIX(A,S)   {DBG_MODULE(A);\
                             DBG_PRINT(A,("%s:%s ",S,__FUNC__));}
# endif

/* Disabling dynamic log level set for TZ builds */
# if !VCS_FEATURE_FP_FUNC_PACK && !VCS_FEATURE_FP_FUNC_UNPACK
#   define DBG_SET_LEVEL(L)    DbgSetLogLevel(L)
#   define DBG_IS_LEVEL_ENABLED(A)  (DbgGetLogLevel() & A)
#   define DBG_SET_PRIORITY(A,P)    DbgSetPriority(DBG_STRUCT_PTR(A),P)
# else /*!VCS_FEATURE_FP_FUNC_PACK && !VCS_FEATURE_FP_FUNC_UNPACK*/
#   define DBG_SET_LEVEL(L)
#   define DBG_IS_LEVEL_ENABLED(A)  VCS_TRUE
#   define DBG_SET_PRIORITY(A,P)
# endif /*!VCS_FEATURE_FP_FUNC_PACK && !VCS_FEATURE_FP_FUNC_UNPACK*/

# define DBG_LEVEL(A)       DbgGetLevel()

# define _DBG_INC_LEVEL(A)  DbgIncLevel()

# define _DBG_DEC_LEVEL(A)  DbgDecLevel()

# define _DBG_ENTER(A)      {DBG_MODULE(A);\
                             DBG_PRINT(A,("%.*s:%s",(int)DBG_LEVEL(A),_DBG_ENTER_STR,__FUNC__));}
# define _DBG_LEAVE(A)      {DBG_MODULE(A);\
                             DBG_PRINT(A,("%.*s:%s",(int)DBG_LEVEL(A),_DBG_LEAVE_STR,__FUNC__));}

# define _DBG_ON(A,M)       (((DBG_FLAGS(A) & (M)) == (M)) && (__MASK__ == __MASK__))
# define _DBG_OUT_ON(A,M)   ((DBG_OUT_FLAGS(A) & (M)) == (M))

# define _DBG_SYS(A,M)      (_DBG_ON(A,M) && (DBG_SYSTEM(A) & __MASK__))

# define DBG_DISPLAY(A,S)   {if ((DBG_SYSTEM(A) & __MASK__) && DBG_IS_LEVEL_ENABLED(DBG_VERBOSE_ON)) \
                                {DBG_MODULE(A);DBG_PRINT(A,S);}}

# define DBG_PRINTSYS(A,S)  {if ((DBG_SYSTEM(A) & __MASK__) && DBG_IS_LEVEL_ENABLED(DBG_VERBOSE_ON)) \
                                {DBG_PRINT(A,S);}}

#ifndef DBG_BREAK
# define DBG_BREAK(A)       {if (_DBG_ON(A,DBG_BREAK_ON))            \
                                {DBG_PREFIX(A,"BREAK");              \
                                DBG_PRINT(A,("LINE=%d\n",__LINE__)); \
                                DBG_BREAKPOINT;}}
#endif

#ifndef DBG_ERROR
# define DBG_ERROR(A,S)     {if (_DBG_ON(A,DBG_ERROR_ON) && DBG_IS_LEVEL_ENABLED(DBG_ERROR_ON))           \
                                {DBG_SET_PRIORITY(A,DBG_ERROR_ON);   \
                                DBG_PREFIX(A,"ERROR");              \
                                DBG_PRINT(A,S);                     \
                                if (_DBG_ON(A,DBG_BREAK_ON))        \
                                    DBG_BREAKPOINT;}}
#endif

#ifndef DBG_WARNING
# define DBG_WARNING(A,S)   {if (_DBG_ON(A,DBG_WARNING_ON) && DBG_IS_LEVEL_ENABLED(DBG_WARNING_ON))         \
                                {DBG_SET_PRIORITY(A,DBG_WARNING_ON);   \
                                DBG_PREFIX(A,"WARNING");            \
                                DBG_PRINT(A,S);}}
#endif

#ifndef DBG_NOTICE
# define DBG_NOTICE(A,S)    {if (_DBG_SYS(A,DBG_NOTICE_ON) && DBG_IS_LEVEL_ENABLED(DBG_NOTICE_ON))         \
                                {DBG_SET_PRIORITY(A,DBG_NOTICE_ON);   \
                                DBG_PREFIX(A,"NOTICE");             \
                                DBG_PRINT(A,S);}}
#endif

#ifndef DBG_FILTER
# define DBG_FILTER(A,M,S)  {if (_DBG_SYS(A,M) && DBG_IS_LEVEL_ENABLED(M))                     \
                                {DBG_SET_PRIORITY(A,M);   \
                                DBG_PREFIX(A,DBG_SYSNAME());       \
                                DBG_PRINT(A,S);}}
#endif

#ifndef DBG_DATA
# define DBG_DATA(A,B,N,O)  {if (_DBG_SYS(A,DBG_DATA_ON) && DBG_IS_LEVEL_ENABLED(DBG_DATA_ON))           \
                                {DBG_SET_PRIORITY(A,DBG_DATA_ON);   \
                                DbgPrintData(DBG_STRUCT_PTR(A),    \
                                (const unsigned char*)(B),N,O);}}
#endif

#ifndef DBG_QDATA
# define DBG_QDATA(A,B,N,F) {if (_DBG_SYS(A,DBG_DATA_ON) && DBG_IS_LEVEL_ENABLED(DBG_DATA_ON)) \
                                {DBG_SET_PRIORITY(A,DBG_DATA_ON);   \
                                DbgQueueData((const unsigned char*)(B),N,F);}}
#endif

#ifndef DBG_ENTER
# define DBG_ENTER(A)       {if (_DBG_SYS(A,DBG_TRACE_ON) && DBG_IS_LEVEL_ENABLED(DBG_TRACE_ON))              \
                                {DBG_SET_PRIORITY(A,DBG_TRACE_ON);  \
                                _DBG_INC_LEVEL(A);    \
                                _DBG_ENTER(A);DBG_PRINT(A,("\n"));}}
#endif

#ifndef DBG_LEAVE
# define DBG_LEAVE(A)       {if (_DBG_SYS(A,DBG_TRACE_ON) && DBG_IS_LEVEL_ENABLED(DBG_TRACE_ON))              \
                                {DBG_SET_PRIORITY(A,DBG_TRACE_ON);  \
                                _DBG_LEAVE(A);DBG_PRINT(A,("\n"));}    \
                                _DBG_DEC_LEVEL(A);}
#endif

#ifndef VCSAPI_ENTER
# define VCSAPI_ENTER(A, S)    if (_DBG_SYS(A,DBG_STATUS_ON) && DBG_IS_LEVEL_ENABLED(DBG_STATUS_ON))     \
                                  {DBG_SET_PRIORITY(A,DBG_STATUS_ON);  \
                                  DBG_PREFIX(A,"VCS_API");      \
                                  DBG_PRINT(A,("%s", _DBG_ENTER_STR)); \
                                  DBG_PRINT(A,("\n"));}
#endif

#ifndef VCSAPI_LEAVE
# define VCSAPI_LEAVE(A, S)    if (_DBG_SYS(A,DBG_STATUS_ON) && DBG_IS_LEVEL_ENABLED(DBG_STATUS_ON))     \
                                  {DBG_SET_PRIORITY(A,DBG_STATUS_ON);  \
                                  DBG_PREFIX(A,"VCS_API");      \
                                  DBG_PRINT(A,("%s", _DBG_LEAVE_STR)); \
                                  DBG_PRINT(A,("\n"));}
#endif

#ifndef DBG_RETURN
# define DBG_RETURN(A,S)    {if (_DBG_SYS(A,DBG_TRACE_ON) && DBG_IS_LEVEL_ENABLED(DBG_TRACE_ON))              \
                                {DBG_SET_PRIORITY(A,DBG_TRACE_ON);  \
                                _DBG_LEAVE(A);DBG_PRINT(A,(" -- "));   \
                                DBG_PRINT(A,S);DBG_PRINT(A,("\n"));}    \
                                _DBG_DEC_LEVEL(A);}
#endif

#ifndef DBG_TRACE
# define DBG_TRACE(A)       {if (_DBG_SYS(A,DBG_TRACE_ON) && DBG_IS_LEVEL_ENABLED(DBG_TRACE_ON))              \
                                {DBG_SET_PRIORITY(A,DBG_TRACE_ON);  \
                                DBG_PREFIX(A,DBG_SYSNAME());           \
                                DBG_PRINT(A,("LINE=%d\n",__LINE__));}}
#endif

#ifndef DBG_INFO
# define DBG_INFO(A,S)      {if (_DBG_SYS(A,DBG_INFO_ON) && DBG_IS_LEVEL_ENABLED(DBG_INFO_ON))       \
                                {DBG_SET_PRIORITY(A,DBG_INFO_ON);  \
                                DBG_PREFIX(A,DBG_SYSNAME());   \
                                DBG_PRINT(A,S);}}
#endif

#ifndef DBG_STATUS
# define DBG_STATUS(A,S)      {if (_DBG_SYS(A,DBG_STATUS_ON) && DBG_IS_LEVEL_ENABLED(DBG_STATUS_ON))   \
                                {DBG_SET_PRIORITY(A,DBG_STATUS_ON);  \
                                DBG_PREFIX(A,"STATUS");        \
                                DBG_PRINT(A,S);}}
#endif

#ifndef DBG_EVENT
# define DBG_EVENT(A,S)      {if (_DBG_SYS(A,DBG_EVENT_ON) && DBG_IS_LEVEL_ENABLED(DBG_EVENT_ON))     \
                                {DBG_SET_PRIORITY(A,DBG_EVENT_ON);  \
                                DBG_PREFIX(A,"EVENT");         \
                                DBG_PRINT(A,S);}}
#endif

#if VCS_FEATURE_ENABLE_MINIMAL_LOG
# define DBG_ASSERT(C)      if (!(C)) { \
                                DBG_PRINT(gDbgInfo,("ASSERT(%s) -- %s#%lu\n", \
                                          #C, __FILE__, __LINE__)); \
                                DBG_TRAP;\
                                {(void)__MASK__;}}
#else /* VCS_FEATURE_ENABLE_MINIMAL_LOG */
# define DBG_ASSERT(C)      if (!(C)) { \
                                DBG_PRINT(gDbgInfo,("ASSERT(%s) -- %s#%d (%s)(%lu)\n", \
                                          #C, __FILE__, __LINE__, __FUNC__, __MASK__)); \
                                DBG_TRAP;}
#endif /* VCS_FEATURE_ENABLE_MINIMAL_LOG */

# if !defined(ASSERT)
#  define ASSERT(C)         if (!(C)) { \
                                DBG_PRINT(gDbgInfo,("ASSERT(%s) -- %s#%lu\n", \
                                          #C, __FILE__, __LINE__)); \
                                DBG_TRAP;}
# endif

#if VCS_FEATURE_PROFILING
#ifndef LOG_PROFILING
# define LOG_PROFILING(A,S)   DBG_PRINT(A, S);
#endif
#endif /* VCS_FEATURE_PROFILING */

# define DBG_STATIC

#define dbgSprintf          tpi_sprintf
#define dbgStrlen(s)        tpi_strlen(s)
#define dbgStrcat(d,s)      tpi_strcat(d,s)
#define dbgStrcpy(d,s)      tpi_strcpy(d,s)

/****************************************************************************/
#else /* !DBG -- When (DBG == 0) we disable all the debug macros. */
/****************************************************************************/

# define DBG_FIELDS
# define DBG_FLAGS(A)           0
# define DBG_OUT_FLAGS(A)       0
# define DBG_ID(A)              0
# define DBG_NAME(A)            ""
# define DBG_NAME_BUFFER(A)
# define DBG_FIELD_NAME_BUFFER(L)
# define DBG_SYSTEM(A)          0
# define DBG_LEVEL(A)           0
# define DBG_FILE_HANDLE(A)     0
# define DBG_PRINT_FUNC(A)
# define DBG_ALT_FUNC(A)

# define DBG_SET_LEVEL(L)
# define DBG_DEF_SYSTEM(N,M)
# define DBG_SYSNAME()          ""
# define DBG_INIT_SYSTEMS()
# define DBG_INIT(A,N,I,M,S)
# define DBG_INIT2(A,N,F,I,M,S)
# define DBG_HALT(A)
# define DBG_FUNC(F)
# define DBG_FUNCX(F,M)
# define DBG_PRINT(A,S)

# define DBG_MODULE(A)
# define DBG_PREFIX(A,S)
# define DBG_DISPLAY(A,S)
# define DBG_PRINTSYS(A,S)
# define DBG_BREAK(A)
# define DBG_ERROR(A,S)
# define DBG_WARNING(A,S)
# define DBG_NOTICE(A,S)
# define DBG_FILTER(A,M,S)
# define DBG_DATA(A,B,N,O)
# define DBG_QDATA(A,B,N,F)
# define DBG_ENTER(A)
# define DBG_LEAVE(A)
# define VCSAPI_ENTER(A, S)
# define VCSAPI_LEAVE(A, S)
# define DBG_RETURN(A,S)
# define DBG_TRACE(A)
# define DBG_INFO(A,S)
# define DBG_STATUS(A,S)
# define DBG_EVENT(A,S)
# define DBG_UNUSED

#define DBG_ASSERT(C)

# if !defined(ASSERT)
#  define ASSERT(C)         DBG_ASSERT(C)
# endif

# define DBG_STATIC         static

#endif /* DBG */

/****************************************************************************/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __vcsDebugBaseEx_h__ */

