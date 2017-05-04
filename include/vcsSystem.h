/*! @file vcsSystem.h
 *******************************************************************************
 **
 **                           NDA AND NEED-TO-KNOW REQUIRED
 **
 *****************************************************************************
 **
 **  Copyright (C) 2008-2016 Synaptics Incorporated. All rights reserved.
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


/*
    vcsStartup() function must be the first API function called. This function
    startup the lower subsystems, allocats storage for VCS subsystem.

    The vcsClientCreateSys() function creates a handle to be used for this
    particular client. This function must be called to get client handle,
    and the handle must be passed back to other API functions where required.

*/

#ifndef __vcsSystem_h__
#define __vcsSystem_h__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "vcsClient.h"

typedef struct vcsImage_s *            vcsImagePointer_t;
typedef struct vcsEnrollTemplate_s *   vcsTemplatePointer_t;
typedef struct vcsMatchResults_s *     vcsMatchResultsPointer_t;
typedef struct mssMatcher_s *          vcsMatcherPointer_t;


typedef vcsResult_t (VCS_API * vcsHASignBySerNumCB_t)(vcsUint8_t * pSerialNumber, vcsUint32_t snLen, vcsUint8_t * pHash, vcsUint32_t hashSize, vcsMechanism_t hashType,
                                              vcsUint8_t * pSignature, vcsUint32_t * pSignatureLen);

typedef vcsResult_t (VCS_API * vcsGetSPubKeyBySerNumCB_t)(vcsUint8_t * pSerialNumber, vcsUint32_t snLen, vcsUint8_t * pSPubKeyMod,
                                                  vcsUint32_t * pSPubKeyModLen);

typedef enum vcsSwipeSpeed_e
{
    VCS_SWIPE_SPEED_DEFAULT  = 0,
    VCS_SWIPE_SPEED_10       = 10,
    VCS_SWIPE_SPEED_15       = 15,
    VCS_SWIPE_SPEED_25       = 25
} vcsSwipeSpeed_t;

typedef enum vcsSOClock_e{
    VCS_SO_CLOCK_DEFAULT = 0,
    VCS_SO_CLOCK_10      = 10,
    VCS_SO_CLOCK_18      = 18,
    VCS_SO_CLOCK_21      = 21,
    VCS_SO_CLOCK_24      = 24
} vcsSOClock_t;

/**
Capture Mode Definitions.

Defines whether capture is immediate, should wait for motion, or based on WOE.

Application can set this via a call to vcsSetParam, which is
why these are pound-defines instead of enumerations.
*/

#define VCS_CAPTURE_MODE_DEFAULT                   ((vcsUint32_t) ~0) /**<  Use sensor specific default capture mode. Actual capture mode will be determined internally during sensor initialization. */
#define VCS_CAPTURE_MODE_IMMEDIATE                 ((vcsUint32_t) 0) /**<  Capture immediately, don't wait for motion. */
#define VCS_CAPTURE_MODE_WAIT_MOTION               ((vcsUint32_t) 1) /**<  Wait for motion before capturing. */
#define VCS_CAPTURE_MODE_WOE_BASED                 ((vcsUint32_t) 2) /**<  Send sensor to WOE mode and start capturing after sensor wakeup. */
#define VCS_CAPTURE_MODE_WOE_FINGER_POLLING        ((vcsUint32_t) 3) /**<  Send sensor to WOE mode, polling finger presence and send normal GetPrint command. */
#define VCS_CAPTURE_MODE_SENSOR_SIDE_RECONSTRUCT   ((vcsUint32_t) 4) /**<  Send GetPrint command with FULL_IR parameter, polling finger presence and read reconstructed image data by VCSFW_CMD_READ_FPDATA command. */
#define VCS_CAPTURE_MODE_SENSOR_SIDE_NAV           ((vcsUint32_t) 5) /**<  Send GetPrint command with nav parameter and read nav results. */
#define VCS_CAPTURE_MODE_FW_LINEAVG                ((vcsUint32_t) 6) /**<  Send Getprint command with ifs irflag indicating only line averaging to be done in firmware */
#define VCS_CAPTURE_MODE_LAST                      ((vcsUint32_t) 7)

/**
Settable VCS System Parameter Type

This is a parameter that can be set and gotten for each client.
*/
#define VCS_PARAM_INTERNAL_                         VCS_PARAM_MAX_VALUE /**< Start internal parameter definitions. */

#define VCS_PARAM_FAR                               (VCS_PARAM_INTERNAL_ + (vcsParameterType_t) 2) /**< False accept rate, as a numerator over one billion. */
#define VCS_PARAM_CAPTURE_MODE                      (VCS_PARAM_INTERNAL_ + (vcsParameterType_t) 3) /**< Capture mode, either VCS_CAPTURE_MODE_WAIT_MOTION or VCS_CAPTURE_MODE_IMMEDIATE. */
#define VCS_PARAM_FINGER_WALK_MATCH_THRESH          (VCS_PARAM_INTERNAL_ + (vcsParameterType_t) 12) /**< Stores IR flags to use */
#define VCS_PARAM_REGULAR_THRESHOLD                 (VCS_PARAM_INTERNAL_ + (vcsParameterType_t) 13)
#define VCS_MAX_PARAMS                              (VCS_PARAM_INTERNAL_ + (vcsParameterType_t) 14) /**< Number of parameters. */

#define VCS_INVALID_UINT_VAL ((vcsUint32_t)-1)


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __vcsSystem_h__ */
