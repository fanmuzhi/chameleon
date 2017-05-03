/*! @file vcsPalProdTrace.h
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
**
**  This file contains the USDK Event Tracing interface.
**
** 
*/

#ifndef __vcsPalProdTrace_h__
#define __vcsPalProdTrace_h__

#if VCS_FEATURE_PRODTRACE && !DBG

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef VCS_OS_QSEE
extern void qsee_log(unsigned char pri, const char* fmt, ...);
#define __VAL_PRD_LOG__(...)   qsee_log(5, __VA_ARGS__)
/*qsee_log() function is responsible to switch to new line */
#define NEW_LINE_STR " "

# elif defined(VCS_OS_MOBICORE)
extern void tlApiLogPrintf(const char *fmt, ...);
#define __VAL_PRD_LOG__(...)   tlApiLogPrintf(__VA_ARGS__)
#define NEW_LINE_STR "\n"

# elif defined(VCS_OS_GP)
extern void SLog(const char *fmt, ...);
#define __VAL_PRD_LOG__(...)   SLog(__VA_ARGS__)
#define NEW_LINE_STR "\n"

# elif defined(VCS_OS_SSTZ)
extern void LOG_MSG(unsigned char pri, const char * format, ...);
#define __VAL_PRD_LOG__(...)   LOG_MSG(5, __VA_ARGS__)
#define NEW_LINE_STR " "

#elif defined(VCS_EFI)
/* Printing on console is taking considerable delays for USB, hence not having any function for prod traces for now
TODO: evalute the dealy with serial logs, and provide API*/
#define __VAL_PRD_LOG__(...)
#define NEW_LINE_STR "\n"

#elif defined(VCS_OS_ANDROID)
#define ADR_LOG_TAG "SynapticsLIB"
extern int __android_log_print(int prio, const char *tag,  const char *fmt, ...);
#define __VAL_PRD_LOG__(...)    __android_log_print(2, ADR_LOG_TAG, __VA_ARGS__);

#else
#define __VAL_PRD_LOG__(...)
#endif /* VCS_OS_QSEE */

    extern void VCS_API palProdTraceInit(void);
    extern void VCS_API palProdTraceDeinit(void);
    extern void  VCS_API palProdTraceStart(void);
    extern void  VCS_API palProdTraceStop(void);
    extern void  VCS_API palTraceEvent(
        const char*       fileName,
        vcsUint32_t lineNumber,
        vcsUint8_t  level
        );
    extern void VCS_API palProdTraceSetLevel(vcsUint32_t level);

# define PAL_TRACE_EVENT(L) {                                           \
    palTraceEvent(__FILE__, __LINE__, L);                               \
    }

/*
*   Interface to trace events.
*/

#define PAL_PRDTRACE_LEVEL_REG_VAL_NAME              "evTraceEnableLevel"

#define PAL_TRACE_LEVEL_NONE        0   // Tracing is not on
#define PAL_TRACE_LEVEL_FATAL       1   // Abnormal exit or termination
#define PAL_TRACE_LEVEL_ERROR       2   // Severe errors that need logging
#define PAL_TRACE_LEVEL_WARNING     3   // Warnings such as allocation failure
#define PAL_TRACE_LEVEL_INFORMATION 4   // Includes non-error cases(e.g.,Entry-Exit)
#define PAL_TRACE_LEVEL_VERBOSE     5   // Detailed traces from intermediate steps


# define PAL_PRODTRACE_INIT()         palProdTraceInit()
# define PAL_PRODTRACE_DEINIT()       palProdTraceDeinit()

# define PAL_PRODTRACE_START()        palProdTraceStart()
# define PAL_PRODTRACE_STOP()         palProdTraceStop()

/* Setting trace level */
#define PAL_PRODTRACE_SET_LEVEL(L)    palProdTraceSetLevel(L)

/* Trace error messages */
# define PAL_PRODTRACE_ERROR()        PAL_TRACE_EVENT(PAL_TRACE_LEVEL_ERROR)

/* Trace warning messages */
# define PAL_PRODTRACE_WARNING()      PAL_TRACE_EVENT(PAL_TRACE_LEVEL_WARNING)

/* Trace informational messages */
# define PAL_PRODTRACE_INFO()         PAL_TRACE_EVENT(PAL_TRACE_LEVEL_INFORMATION)

/* Trace detailed information messages */
# define PAL_PRODTRACE_VERBOSE()      PAL_TRACE_EVENT(PAL_TRACE_LEVEL_VERBOSE)

#if VCS_FEATURE_PROFILING
# define PAL_PRODLOG_PROFILING(S)     __VAL_PRD_LOG__ S;
#endif /* VCS_FEATURE_PROFILING */

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#else /*VCS_FEATURE_PRODTRACE&& !DBG*/

/*
*   Event Tracing dummy interface.
*/
# define PAL_PRODTRACE_INIT()
# define PAL_PRODTRACE_DEINIT()
# define PAL_PRODTRACE_START()
# define PAL_PRODTRACE_STOP()
# define PAL_PRODTRACE_ERROR()
# define PAL_PRODTRACE_WARNING()
# define PAL_PRODTRACE_INFO()
# define PAL_PRODTRACE_VERBOSE()

#if VCS_FEATURE_PROFILING
# define PAL_PRODLOG_PROFILING(S)
#endif /* VCS_FEATURE_PROFILING */

#endif /* VCS_FEATURE_PRODTRACE && !DBG*/

#if !VCS_FEATURE_PRODTRACE && !DBG
#define PAL_PRODTRACE_SET_LEVEL(L)
#endif /* !VCS_FEATURE_PRODTRACE && !DBG*/



#endif /* __vcsPalProdTrace_h__ */

