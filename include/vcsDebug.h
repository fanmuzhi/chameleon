/*! @file vcsDebug.h
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
**  Validity Debug Interface Definitions
**
**  This file contains the Validity debug definitions.
**
**  
*/

#ifndef __vcsDebug_h__
#define __vcsDebug_h__

#include "vcsTypes.h"
#include "vcsResults.h"
#include "vcsDebugBase.h"

#ifdef WIN_WRAPPER
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(VCS_EFI) && defined(UNICODE)
#undef UNICODE
#endif


#if DBG

extern const char * VCS_API DbgBool(vcsBool_t value);
extern const char * VCS_API DbgLastError(vcsUint32_t status);
extern const char * VCS_API DbgVcsResult(vcsResult_t result);
extern vcsUint32_t  VCS_API DbgGetLastError(void);
extern const char * VCS_API DbgVcsFWStatus(vcsUint32_t FWStatus);

#define TABLE_INIT(_val) { _val, #_val }
#define TABLE_INIT_EX(_val, _str) { _val, _str }

typedef struct DbgInfo_s {
    vcsUint32_t     value;
    char           *string;
} DbgInfo_t;

extern void VCS_API
DbgValToString(char *      buffer,
               vcsUint32_t value,
               DbgInfo_t * table,
               vcsUint32_t tableSize);

#define DBG_BUFFER_CHECK(_a_,_b_)                                   \
if (dbgStrlen(_b_) >= sizeof(_b_))                                  \
{                                                                   \
    DBG_PRINT(_a_, ("************************************\n"));     \
    DBG_PRINT(_a_, ("**** Exceeded debug buffer size! "             \
        "Actual:%lu Maximum:%lu\n", dbgStrlen(_b_), sizeof(_b_)));   \
    DBG_PRINT(_a_, ("************************************\n"));     \
}                                                                   \

#endif /* DBG */

#if defined(VCS_EFI) 
	#if defined(EFI_DEBUG)
	  #pragma runtime_checks( "", off )
	#endif

  /* dont Define anything here, it causing double prints in UEFI*/
 #define DEBUG_PBA_STRING( s )
#endif /* defined(VCS_EFI)   */

/* BCM for target and win simulation  */
#if  VCS_FEATURE_SOC /* for BCM target/windows simulation  */
  #if DBG
  #define DEBUG_SOC 1 /*  one place to turn on/off SOC debug: windows, NOT for BCM target */
  #else
  #define DEBUG_SOC 0 /*  one place to turn on/off SOC debug: BCM target, set to 0 when release to BCM  */
  #endif
#else
  #define DEBUG_SOC 0
#endif

void VCS_API _socDebugString(vcsConstString_t str);
void VCS_API _socDebugData(vcsConstString_t str, vcsUint32_t data);
void VCS_API _socDebugLog(vcsConstString_t file, vcsUint32_t line);
void VCS_API _socDebugLogData(vcsConstString_t file, vcsUint32_t line, vcsUint32_t data);

void VCS_API _socDebugByte( vcsUint8_t bByte );
void VCS_API _socDebugDword ( vcsUint32_t dwDword );
void VCS_API _socDebugArray(vcsUint8_t *pbArray, vcsUint32_t dwLength);

/* Debug message macros for SOC */
#if DEBUG_SOC

#if   VCS_OS_WINDOWS  /* change it to be 1 to make both window/BCM target output debug string  */
#define socDebugString(s)           _socDebugString(s)
#define SOC_DBG_ERROR(s)            _socDebugString(s)
#define SOC_DBG_WARNING(s)          _socDebugString(s)
#define SOC_DBG_INFO(s)             _socDebugString(s)
#define SOC_DBG_ERROR_DATA(s,d)     _socDebugData(s,d)
#define SOC_DBG_WARNING_DATA(s,d)   _socDebugData(s,d)
#define SOC_DBG_INFO_DATA(s,d)      _socDebugData(s,d)
#else
#define socDebugString(s)           _socDebugLog(__MODULE__,__LINE__)
#define SOC_DBG_ERROR(s)            _socDebugLog(__MODULE__,__LINE__)
#define SOC_DBG_WARNING(s)          _socDebugLog(__MODULE__,__LINE__)
#define SOC_DBG_INFO(s)             _socDebugLog(__MODULE__,__LINE__)
#define SOC_DBG_ERROR_DATA(s,d)     _socDebugLogData(__MODULE__,__LINE__,d)
#define SOC_DBG_WARNING_DATA(s,d)   _socDebugLogData(__MODULE__,__LINE__,d)
#define SOC_DBG_INFO_DATA(s,d)      _socDebugLogData(__MODULE__,__LINE__,d)
#endif

#define socDebugByte(x)             _socDebugByte(x)
#define socDebugDword(x)            _socDebugDword(x)
#define socDebugArray(x,y)          _socDebugArray(x,y)

#else

#define SOC_DBG_ERROR(s)
#define SOC_DBG_WARNING(s)
#define SOC_DBG_INFO(s)
#define SOC_DBG_ERROR_DATA(s,d)
#define SOC_DBG_WARNING_DATA(s,d)
#define SOC_DBG_INFO_DATA(s,d)

#define socDebugByte(x)
#define socDebugDword(x)
#define socDebugArray(x,y)
#define socDebugString(s)

#endif /* DEBUG_SOC */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __vcsDebug_h__ */

