/*
 *****************************************************************************
 **
 **                           NDA AND NEED-TO-KNOW REQUIRED
 **
 *****************************************************************************
 **
 **  Copyright (C) 2017 Synaptics Incorporated. All rights reserved.
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

/**
 * @file ssiWhitneyCmd.h
 *
 * @brief. This file defines types for the externally accessible 
 * command mechanism.
 *
 */

#ifndef _ssiWhitneyCmd_h__
#define __ssiWhitneyCmd_h__

/*!
*******************************************************************************
**  Unsigned 8-bit integer
**
**  Unsigned integer type of at least 8 bits.
**
*/
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
typedef uint8_t wfwUint8_t;
#else
typedef unsigned char wfwUint8_t;
#endif

/*!
*******************************************************************************
**  Unsigned 16-bit integer
**
**  Unsigned integer type of at least 16 bits.
*/
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
typedef uint16_t wfwUint16_t;
typedef int16_t  wfwSint16_t;
#else
typedef unsigned short wfwUint16_t;
typedef signed   short wfwSint16_t;
#endif

/*!
*******************************************************************************
**  Unsigned 32-bit integer
**
**  Unsigned integer type of at least 32 bits.
*/
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)

typedef uint32_t wfwUint32_t;

#else /* defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) */

#if defined(VCS_OS_WINDOWS)
typedef UINT32 wfwUint32_t;
#else /* VCS_OS_WINDOWS */
#if defined(VCS_SIZEOF_INT) && (VCS_SIZEOF_INT < 4)
typedef unsigned long wfwUint32_t;
#else /* defined(VCS_SIZEOF_INT) && (VCS_SIZEOF_INT < 4) */
typedef unsigned int wfwUint32_t;
#endif /* defined(VCS_SIZEOF_INT) && (VCS_SIZEOF_INT < 4) */
#endif /* VCS_OS_WINDOWS */

#endif /* defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) */

/*!
*******************************************************************************
**  Packed type qualifier
*/
#define WFW_PACKED

#include "whitney_commands.h"

#endif /* __ssiWhitneyCmd_h__ */
