/*! @file vcsDebugBase.h
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


/*! @file
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

#ifndef __vcsDebugBase_h__
#define __vcsDebugBase_h__

#include "vcsDebugBaseEx.h"
#include "vcsPalProdTrace.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/****************************************************************************/
#if !DBG /* When (DBG == 0) Event Tracing replaces some macroses. */
# ifdef DBG_ERROR
#  undef DBG_ERROR
# endif
# define DBG_ERROR(A,S)         PAL_PRODTRACE_ERROR()

# ifdef DBG_WARNING
#  undef DBG_WARNING
# endif
# define DBG_WARNING(A,S)       PAL_PRODTRACE_WARNING()
#ifdef VCS_OS_LINUX
# define DbgLastError(A)              "unavailable" /* causes problems under linux */
#endif

# ifdef DBG_STATUS
#  undef DBG_STATUS
# endif
# define DBG_STATUS(A,S)        PAL_PRODTRACE_INFO()

# ifdef DBG_NOTICE
#  undef DBG_NOTICE
# endif
# define DBG_NOTICE(A,S)        PAL_PRODTRACE_VERBOSE()

# ifdef DBG_SET_LEVEL
#  undef DBG_SET_LEVEL
# endif
# define DBG_SET_LEVEL(L)        PAL_PRODTRACE_SET_LEVEL(L)

#if VCS_FEATURE_PROFILING
# ifdef LOG_PROFILING
#  undef LOG_PROFILING
# endif
# define LOG_PROFILING(A,S)      PAL_PRODLOG_PROFILING(S)
#endif /* VCS_FEATURE_PROFILING */

/* define DBG_MINIMAL to enable only DBG_ERROR, DBG_WARNING and DBG_STATUS */
#if  !VCS_FEATURE_ENABLE_MINIMAL_LOG

# ifdef VCSAPI_ENTER
#  undef VCSAPI_ENTER
# endif
# define VCSAPI_ENTER(A, S)        PAL_PRODTRACE_INFO()

# ifdef VCSAPI_LEAVE
#  undef VCSAPI_LEAVE
# endif
# define VCSAPI_LEAVE(A, S)        PAL_PRODTRACE_INFO()

#endif /* VCS_FEATURE_ENABLE_MINIMAL_LOG */

#endif /* !DBG */
/****************************************************************************/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __vcsDebugBase_h__ */
