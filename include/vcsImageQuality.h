/*! @file vcsImageQuality.h
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
**  This header file contains image quality definitions.
**
**  
*/

#ifndef __vcsImageQuality_h__
#define __vcsImageQuality_h__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "vcsTypes.h"

/*!
*******************************************************************************
**  When image quality flags are zero, this indicates a good swipe and good
**  image for the matcher. Otherwise, one or more quality flags may be set to
**  indicate warnings or errors.
**  If VCS_IMAGE_QUALITY_PROCESS_FAILED is set, we recommend that you pass the
**  image to the matcher for confirmation. You should also prompt the user to
**  help guide them toward proper swipe technique.
*/
typedef vcsUint32_t      vcsImageQuality_t;

#define VCS_IMAGE_QUALITY_GOOD                              ((vcsImageQuality_t) 0)

/*!
\b Problem: Finger got stuck or stopped one or more times during swipe. \n
\b Prompt: Please swipe your finger smoothly across the middle sensor with gentle pressure.
*/
#define VCS_IMAGE_QUALITY_STICTION                          ((vcsImageQuality_t) 0x00000001)

/*!
\b Problem: Finger swiped too quickly across the sensor. \n
\b Prompt: Please swipe your finger more slowly across the middle sensor with gentle pressure.
*/
#define VCS_IMAGE_QUALITY_TOO_FAST                          ((vcsImageQuality_t) 0x00000002)

/*!
\b Problem: Finger tip only swiped or it was swiped too quickly. \n
\b Prompt: Please swipe your finger from the knuckle to the tip across the middle sensor with gentle pressure.
*/
#define VCS_IMAGE_QUALITY_TOO_SHORT                         ((vcsImageQuality_t) 0x00000004)

/*!
\b Problem: Finger moved backwards during swipe. \n
\b Prompt: Please swipe your finger in one direction across the middle sensor with gentle pressure.
*/
#define VCS_IMAGE_QUALITY_REVERSE_MOTION                    ((vcsImageQuality_t) 0x00000008)

/*!
\b Problem: Finger moved too slowly during swipe. \n
\b Prompt: Please swipe your finger a little more quickly across the middle sensor with gentle pressure.
*/
#define VCS_IMAGE_QUALITY_TOO_SLOW                          ((vcsImageQuality_t) 0x00000010)

/*!
\b Problem: Finger was lifted from the sensor before swipe was complete. \n
\b Prompt: Please swipe your finger from the knuckle to the tip across the middle sensor with gentle pressure.
*/
#define VCS_IMAGE_QUALITY_LIFTED_FINGER                     ((vcsImageQuality_t) 0x00000020)

/*!
\b Problem: USB bus dropped a significant number of scan lines from the sensor. \n
\b This is informational message and should be ignored.
*/
#define VCS_IMAGE_QUALITY_DROPPED_LINES                     ((vcsImageQuality_t) 0x00000040)

/*!
\b Problem: Too many image columns are damaged on the sensor. \n
\b This is informational message and should be ignored.
*/
#define VCS_IMAGE_QUALITY_BAD_IMAGE_COLUMNS                 ((vcsImageQuality_t) 0x00000080)

/*!
\b Problem: Too many rate columns are damaged on the sensor (VFS131 only). \n
\b This is informational message and should be ignored.
*/
#define VCS_IMAGE_QUALITY_BAD_RATE_COLUMNS                  ((vcsImageQuality_t) 0x00000100)

/*!
\b Problem: Minor RFI noise was detected in the image. \n
\b This is informational message and should be ignored.
*/
#define VCS_IMAGE_QUALITY_SPECKLE_NOISE_DETECTED            ((vcsImageQuality_t) 0x00000200)

/*!
\b Problem: Major RFI noise was detected in the image. \n
\b This is informational message and should be ignored.
*/
#define VCS_IMAGE_QUALITY_EXCESSIVE_SPECKLE_NOISE_DETECTED  ((vcsImageQuality_t) 0x00000400)

/*!
\b Problem: Possible host buffer overrun detected (SPI sensor only). \n
\b This is informational message and should be ignored.
*/
#define VCS_IMAGE_QUALITY_BUFFER_OVERRUN                    ((vcsImageQuality_t) 0x00000800)

/*!
\b Problem: Finger was offset to the left or right of the sensor. \n
\b Prompt: Please swipe your finger smoothly across the middle of the sensor with gentle pressure.
*/
#define VCS_IMAGE_QUALITY_FINGER_OFFSET                     ((vcsImageQuality_t) 0x00001000)

/*!
\b Problem: There seems to be moisture or some kind of noise on the sensor. \n
\b Prompt: Please wipe the finger and/or the sensor clean.
*/
#define VCS_IMAGE_QUALITY_WATER_ON_SENSOR                   ((vcsImageQuality_t) 0x00002000)

/*!
\b Problem: Finger pressure was not even during the swipe. \n
\b Prompt: Please swipe your finger more smoothly across the middle of the sensor with constant, gentle pressure.
*/
#define VCS_IMAGE_QUALITY_CHANGE_PRESSURE                   ((vcsImageQuality_t) 0x00004000)

/*!
\b Problem: Finger swipe angle was too large. \n
\b Prompt: Please swipe your finger more straight across the middle of the sensor with gentle pressure.
*/
#define VCS_IMAGE_QUALITY_SKEW_TOO_LARGE                    ((vcsImageQuality_t) 0x00008000)

/*!
\b Problem: Finger pressure was too light during the swipe. \n
\b Prompt: Please swipe your finger smoothly across the middle of the sensor with a little more pressure.
*/
#define VCS_IMAGE_QUALITY_PRESSURE_TOO_LIGHT                ((vcsImageQuality_t) 0x00010000)

/*!
\b Problem: Finger was offset to the left of the sensor. \n
\b Prompt: Please swipe your finger smoothly across the middle of the sensor with gentle pressure.
*/
#define VCS_IMAGE_QUALITY_FINGER_OFFSET_TOO_FAR_LEFT        ((vcsImageQuality_t) 0x00020000)

/*!
\b Problem: Finger was offset to the right of the sensor. \n
\b Prompt: Please swipe your finger smoothly across the middle of the sensor with gentle pressure.
*/
#define VCS_IMAGE_QUALITY_FINGER_OFFSET_TOO_FAR_RIGHT       ((vcsImageQuality_t) 0x00040000)

/*!
\b Problem: Finger pressure was too heavy during the swipe. \n
\b Prompt: Please swipe your finger smoothly across the middle of the sensor with a little less pressure.
*/
#define VCS_IMAGE_QUALITY_PRESSURE_TOO_HARD                 ((vcsImageQuality_t) 0x00080000)

/*!
\b Problem: Finger swipe is not from a live finger (probably a fake finger or spoof image). \n
\b Prompt: Please swipe your finger smoothly across the middle of the sensor with gentle pressure.
*/
#define VCS_IMAGE_QUALITY_ASP_DATA_INVALID                  ((vcsImageQuality_t) 0x00100000)

/*!
\b Problem: Finger swipe is not from a live finger (probably a fake finger or spoof image). \n
\b Prompt: Please swipe your finger smoothly across the middle of the sensor with gentle pressure.
*/
#define VCS_IMAGE_QUALITY_NOT_A_FINGER_SWIPE                ((vcsImageQuality_t) 0x00200000)

/*!
\b Problem: The baseline data is invalid. \n
\b This is informational message and should be ignored.
*/
#define VCS_IMAGE_QUALITY_BASELINE_DATA_INVALID             ((vcsImageQuality_t) 0x00400000)

/*!
\b Problem: Swipe speed could not be accurately determined from available data. \n
\b Prompt: Please swipe your finger smoothly across the middle of the sensor with gentle pressure.
*/
#define VCS_IMAGE_QUALITY_CORRELATION_FAILURE               ((vcsImageQuality_t) 0x00800000)

/*!
\b Problem: Image data is obstructed by excessive moisture on the finger or water on the sensor. \n
\b Prompt: Please dry your finger and the sensor before swiping your finger across the sensor with gentle pressure.
*/
#define VCS_IMAGE_QUALITY_WET_FINGER                        ((vcsImageQuality_t) 0x01000000)

/*!
\b Problem: Finger appears to be too thin and may not be strong enough. \n
\b Prompt: Please use a larger finger, perhaps thumb.
*/
#define VCS_IMAGE_QUALITY_FINGER_TOO_THIN                   ((vcsImageQuality_t) 0x02000000)

/*!
\b Problem: The baseline data seems to be unavailable. \n
\b Prompt: This should only be seen during file playback due to BVS lacking this data.
*/
#define VCS_IMAGE_QUALITY_BASELINE_DATA_UNAVAILABLE         ((vcsImageQuality_t) 0x04000000)

/*!
\b Problem: Placement sensors only: The finger only partially touched the sensor. \n
\b Prompt: please try to touch in the middle of the sensor and fully.
*/
#define VCS_IMAGE_QUALITY_PARTIAL_TOUCH                     ((vcsImageQuality_t) 0x08000000)

/*!
\b Problem: Placement sensors only: The finger did not touch the sensor but an image was captured anyway. \n
\b Prompt: please try to touch in the middle of the sensor and fully.
*/
#define VCS_IMAGE_QUALITY_EMPTY_TOUCH                       ((vcsImageQuality_t) 0x10000000)

/*!
\b Problem: During enrollment, it looks like different fingers were used. (EEM may allow) \n
\b Prompt: please use only one finger at a time to enroll.
*/
#define VCS_IMAGE_QUALITY_WRONG_FINGER                      ((vcsImageQuality_t) 0x20000000)

/*!
\b Problem: This is not a finger print image but only a gesture, no matching attempt should be made with this image. \n
\b Prompt:
*/
#define VCS_IMAGE_QUALITY_GESTURE                           ((vcsImageQuality_t) 0x40000000)

/*!
\b Problem: This flag indicates the image quality is very poor and should not be used. \n
\b Notes: There will be one or more quality flags set whenever this error flag is set.
    If this flag is not set, and other flags are set, then those should be considered warnings, not errors.
    In that case, we recommend that you pass the image to the matcher for confirmation.
    You might also prompt the user to gently guide them toward proper swipe technique.
*/
#define VCS_IMAGE_QUALITY_PROCESS_FAILED                    ((vcsImageQuality_t) 0x80000000)


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __vcsImageQuality_h__ */
