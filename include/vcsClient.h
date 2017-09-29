/*! @file vcsClient.h
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
**  USDK VCS API Definitions
**
    This file contains the USDK VCS API functions available to the client. It
    include functions that relate to matching and database operations.

    The vcsClientCreate() function creates a handle to be used for this
    particular client. This function must be called to get client handle,
    and the handle must be passed back to other API functions where required.

    Most of the VCS API functions  are synchronous.

    There are some VCS API functions which are synchronous by nature and wait
    for their operation to complete before returning.

    Fingerprint capture function is asynchronous that begin an
    operation and then return immediately without waiting for the operation
    to complete. The passed "callback" function is called with an appropriate
    data when the asynchronous process completes.

    After finishing all the work with USDK, vcsClientDestroy() function must be called.
*/

#ifndef __vcsClient_h__
#define __vcsClient_h__

#define DBG_VCS_API_SYS                         0x00000010L
#define DBG_VCS_CLIENT_SYS                      0x00000020L
#define DBG_VCS_SERVICE_SYS                     0x00000080L
#define DBG_VCS_SYS                             0x10000000L

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "vcsBase.h"        /* Builds up the defines for what platform and os we are building on */
#include "vcsTypes.h"       /* Platform specific type definitions */
#include "vcsResults.h"
#include "vcsDebug.h"       /* Routines for printing out string versions of the result codes */
#include "vcsImage.h"
#include "vcsTemplate.h"
#include "vcsMatcher.h"
#include "vcsEvent.h"


/**
Capture Purpose Definitions.

Define whether capture is for enrollment or verification purpose.
*/
#define VCS_CAPTURE_PURPOSE_FINGERDETECT       3




    /*********************************************************************
     *                             Synchronous                           *
     *                         VCS API Functions                         *
     *                                                                   *
     * The following are VCS API functions that do not return until their*
     * operation is complete.  They do NOT result in the occurrence of a *
     * system event upon completion.                                     *
     *********************************************************************/

/**
    VCS Client specific parameter types
 */
typedef vcsUint32_t vcsParameterType_t;

#define VCS_PARAM_UNKNOWN               ((vcsParameterType_t)  0) /**< Zero is reserved; unknown parameter. */
#define VCS_PARAM_SAVE_BVS              ((vcsParameterType_t)  1) /**< Save raw fingerprint data as BVS binary file. */
#define VCS_PARAM_SAVE_VFS              ((vcsParameterType_t)  2) /**< VFS201 specific - unsupported and will be ignored. */
#define VCS_PARAM_SAVE_IR_BMP           ((vcsParameterType_t)  3) /**< Save reconstructed fingerprint image as BMP file. */
#define VCS_PARAM_SAVE_RAW_BMP          ((vcsParameterType_t)  4) /**< Save raw fingerprint image as BMP file. */
#define VCS_PARAM_SECURITY_LEVEL        ((vcsParameterType_t)  5) /**< Fingerprint security level. */
#define VCS_PARAM_MAX_ENROLL_ATTEMPTS   ((vcsParameterType_t)  6) /**< Set max attempts for enrollment. Does not include any short swipes prior to first acceptable one */
#define VCS_PARAM_TIME_WINDOW           ((vcsParameterType_t)  7) /**< Set time window to leave enrollment open in seconds */
#define VCS_PARAM_SWIPES_WINDOW         ((vcsParameterType_t)  8) /**< Set number of swipe attempts to leave enrollment open */
#define VCS_PARAM_MATCH_BEHAVIOR_WITHIN_WINDOW ((vcsParameterType_t) 9) /**< Set match while window open. See params below. If >0 ==> score thresh */
#define VCS_PARAM_TRANSFER_IMAGE        ((vcsParameterType_t) 10) /**< ExAPI only:default set to true to transfer fp image to client, not for VCS API which transfer image always  */
#define VCS_PARAM_ENROLL_MIN_HEIGHT     ((vcsParameterType_t) 11) /**< Allow only image with >= this many rows to enroll (EEM only) */
#define VCS_PARAM_MAX_ENROLL_SWIPES     ((vcsParameterType_t) 12) /**< Set max swipes total for enrollment (includes short swipes) */
#define VCS_PARAM_SUSPEND_RETRY_HANDLING ((vcsParameterType_t) 13) /**< Enables VCS_RESULT_SUSPEND_RETRY error handling in FPClient side (suspends request on client side 
                                                                        until system resumes). hVCSClient is ignored during setting this param via vcsSetParam function call,
                                                                        because it is not a client specific but a system wide parameter. */
#define VCS_PARAM_ENROLL_TEMPLATE_TYPE  ((vcsParameterType_t) 14) /**< Sets the enroll template type. DP Matcher only. */
#define VCS_PARAM_ENROLL_VIEWS_COUNT    ((vcsParameterType_t) 15) /**< Sets the number of swipes/views in the enrollment.DP Matcher only.1 1*/
#define VCS_PARAM_ENROLL_METHOD         ((vcsParameterType_t) 16) /**< Sets the enrollment method within the matcher. DP Matcher only.*/
#define VCS_PARAM_ENROLL_PEM_THRESHOLD  ((vcsParameterType_t) 17) /**< Sets the PEM threshold. Relevant for PEM enroll method only. DP Matcher only.*/
#define VCS_PARAM_ENROLL_INITIAL_MATCH_THRESHOLD ((vcsParameterType_t) 18) /**< Sets the EEM threshold during initial enrollment. Can be used to prevent different fingers from enrolling initially. */
#define VCS_PARAM_MIN_ENROLL_ATTEMPTS   ((vcsParameterType_t) 19) /**< Set min attempts for enrollment.*/
#define VCS_PARAM_MATCH_TEMPLATE_UPDATE_THRESHOLD ((vcsParameterType_t) 20) /**< update EEM template only if match score is this value or above. */

/**< BEGIN OF OBSOLETED ridge matcher related parameters. */
#define VCS_PARAM_SZ                           ((vcsParameterType_t) 21)
#define VCS_PARAM_STEP_X                       ((vcsParameterType_t) 22)
#define VCS_PARAM_STEP_Y                       ((vcsParameterType_t) 23)
#define VCS_PARAM_MARGIN_X                     ((vcsParameterType_t) 24)            
#define VCS_PARAM_MARGIN_Y                     ((vcsParameterType_t) 25)
#define VCS_PARAM_HOP                          ((vcsParameterType_t) 26)
#define VCS_PARAM_THRESHPATCHMATCH             ((vcsParameterType_t) 27)
#define VCS_PARAM_EDRATION1                    ((vcsParameterType_t) 28)
#define VCS_PARAM_EDRATION2                    ((vcsParameterType_t) 29)
#define VCS_PARAM_EDRATIOD                     ((vcsParameterType_t) 30)
#define VCS_PARAM_HU1N1                        ((vcsParameterType_t) 31)
#define VCS_PARAM_HU1N2                        ((vcsParameterType_t) 32)
#define VCS_PARAM_HU1D                         ((vcsParameterType_t) 33)           
#define VCS_PARAM_RANSAC_THRESHOLD             ((vcsParameterType_t) 34)
#define VCS_PARAM_HOUGH_MAX_CLUSTERS           ((vcsParameterType_t) 35)
#define VCS_PARAM_CLUSTER_WARP_ERROR_THRESHOLD ((vcsParameterType_t) 36)
#define VCS_PARAM_RANSAC_THRESHOLD_PATCHMATCH  ((vcsParameterType_t) 37)
#define VCS_PARAM_MAX_NPATCHES                 ((vcsParameterType_t) 38)  
#define VCS_PARAM_BLOCAL_REFINE                ((vcsParameterType_t) 39)
#define VCS_PARAM_N_ITERATIONS                 ((vcsParameterType_t) 40)
#define VCS_PARAM_BUSEBESTPATCHES              ((vcsParameterType_t) 41)
#define VCS_PARAM_NO_RIGID_COMPONENTS          ((vcsParameterType_t) 42)
#define VCS_PARAM_MIN_CLUSTER_PATCHES          ((vcsParameterType_t) 43)
#define VCS_PARAM_MAX_DISTANCE_TO_CLUSTER      ((vcsParameterType_t) 44)
#define VCS_PARAM_ENABLE_GLOBAL_CLUSTERING     ((vcsParameterType_t) 45)
#define VCS_PARAM_ENABLE_ROTATION              ((vcsParameterType_t) 46)
#define VCS_PARAM_ENABLE_WIGGLE                ((vcsParameterType_t) 47)
/**< END OF OBSOLETED ridge matcher related parameters. */

#define VCS_PARAM_MATCHER_MAX_GOOD_ENROLL_IMAGES ((vcsParameterType_t) 48)
#define VCS_PARAM_REDUNDANCY_LEVEL               ((vcsParameterType_t) 49) 
#define VCS_PARAM_TEMPLATE_UPDATE                ((vcsParameterType_t) 50) 
#define VCS_PARAM_MAX_VALUE                      ((vcsParameterType_t) 50) /**< This value will be used for validation of passed VCS_PARAM_XXX argument. */

/** Mobile matcher behaviour params, values that can be taken by VCS_PARAM_MATCH_BEHAVIOR_WITHIN_WINDOW */
#define VCS_PVAL_MATCH_BEHAVIOR_WITHIN_WINDOW_NORMAL          0 /**< if VCS_PARAM_MATCH_BEHAVIOR_WITHIN_WINDOW set to this, match normally when window open */
#define VCS_PVAL_MATCH_BEHAVIOR_WITHIN_WINDOW_ALWAYS_MATCH   -1 /**< if VCS_PARAM_MATCH_BEHAVIOR_WITHIN_WINDOW set to this, always matchy when window open */
#define VCS_PVAL_MATCH_BEHAVIOR_WITHIN_WINDOW_DISABLE_UPDATE -2 /**< if VCS_PARAM_MATCH_BEHAVIOR_WITHIN_WINDOW set to this, mimic forced enrollment only (disable update) */


    /*********************************************************************
     *               VCS API Security Related Functions                  *
     *********************************************************************/

typedef enum vcsMechanism_e /**< Enumeration of cryptographic mechanisms*/
{
    /* Digest Algorithms */
    VCS_CRYPTO_SHA_256,    /**< SHA 256 hashing mechanism*/
    VCS_CRYPTO_SHA_1,      /**< SHA 1 hashing mechanism*/
    VCS_CRYPTO_MD_5,       /**< MD5 hashing mechanism*/
    VCS_CRYPTO_SSL3_SHAMD5,/**< Pair of SHA1 and MD5 hashes for 
                           **** SSL3 client authentication algorithm */

    /* Encryption/Decryption Algorithms */
    VCS_CRYPTO_AES_ECB,    /**< AES encryption/decryption mechanism in ECB mode*/
    VCS_CRYPTO_AES_CBC,    /**< AES encryption/decryption mechanism in CBC mode*/
    VCS_CRYPTO_AES_CTR,    /**< AES encryption/decryption mechanism in Counter (CTR) mode*/
    VCS_CRYPTO_TDES_ECB,   /**< Triple DES encryption/decryption mechanism in ECB mode*/
    VCS_CRYPTO_TDES_CBC,   /**< Triple DES encryption/decryption mechanism in CBC mode*/
    VCS_CRYPTO_DES_ECB,    /**< DES encryption/decryption mechanism in ECB mode*/
    VCS_CRYPTO_DES_CBC,    /**< DES encryption/decryption mechanism in CBC mode*/

    /* Checksum Algorithms */
    VCS_CRYPTO_CRC_32,     /**< CRC32 checksum mechanism*/
    VCS_CRYPTO_ADLER_32    /**< Alder32 checksum mechanism*/
} vcsMechanism_t;

typedef vcsMechanism_t vcsCryptoAlg_t; /**< Cryptographic mechanism type*/



    /*********************************************************************
     *  VCS Extended API for Enrollment, Identification and Verification  *
     *********************************************************************/

typedef enum vcsEnrollStatus_s
{
    VCS_ENROLL_STATUS,                  /* indicates if enrollment status incomp, complete or failure */
    VCS_ENROLL_PERCENT,                 /* percent of enrollment, should be 100 if complete */
    VCS_REJECT_REASON,                  /* if image rejected for enrollment, reason if any */
    VCS_FINGER_COVERAGE,                /* percent of sensor area covered in current scan */
    VCS_ENROLL_IMG_QUALITY,             /* quality of current scan */
    VCS_ENROLL_NEXT_POSITION_FEEDBACK,  /* decides prompt to user */
    VCS_CURRENT_COVERAGE_MAP,           /* Grids Covered by current Swipe */
    VCS_CUMMULATIVE_COVERAGE_MAP,       /* Grids Covered due to swipes so far */
    VCS_REPLACED_VIEW,
    VCS_ENROLL_QUALITY,                 /* indicates the enrollment quality */
    VCS_ENROLL_STATUS_NUM_ELEM          /* Ensure this remains at the end */
} vcsEnrollStatus_t;

#define VCS_ENROLL_STATUS_EX_MAGIC 0xecce
/**< Template successfully created. */
#define VCS_TEMPLATE_CREATION_SUCCESS   ((vcsTemplateCreationStatus_t)  400)
/**< Template creation failed. */
#define VCS_TEMPLATE_CREATION_FAILURE   ((vcsTemplateCreationStatus_t)  401)
/**< Template creation incomplete; need another image. */
#define VCS_TEMPLATE_CREATION_INCOMP    ((vcsTemplateCreationStatus_t)  402)

typedef enum vcsFingerFeedBack_e /**< Enumeration of finger position feedback codes.*/
{
    VCS_FINGER_POSITION_FEEDBACK_SWIPE_MIDDLE = 11,            /** Swipe middle part of the Finger.**/
    VCS_FINGER_POSITION_FEEDBACK_SWIPE_MIDDLE_FULL = 12,       /** Middle part is swiped short please swipe with full finger.*/
    VCS_FINGER_POSITION_FEEDBACK_SWIPE_LEFT   = 21,            /** Swipe left part of the finger   **/
    VCS_FINGER_POSITION_FEEDBACK_SWIPE_RIGHT  = 31,            /** Swipe right part of the finger  **/
    VCS_FINGER_POSITION_FEEDBACK_SWIPE_ANY    = 41,            /** Grids Filled, Swipe Any part of finger To Increase Confidence */
    VCS_FINGER_POSITION_FEEDBACK_PLACE_MIDDLE = 61,            /** Place middle part of the finger **/
    VCS_FINGER_POSITION_FEEDBACK_PLACE_TIP    = 62,            /** Place Tip of the finger         **/
    VCS_FINGER_POSITION_FEEDBACK_PLACE_BOTTOM = 63,            /** Place bottom part of the finger **/
    VCS_FINGER_POSITION_FEEDBACK_PLACE_LEFT   = 64,            /** Place left part of the finger  **/
    VCS_FINGER_POSITION_FEEDBACK_PLACE_RIGHT  = 65,            /** Place right part of the finger **/
    VCS_FINGER_POSITION_FEEDBACK_PLACE_ANY    = 66             /** Place any part of the finger   **/
} vcsFingerFeedBack_t;

typedef enum vcsRejectReason_e /**< Enumeration of finger position feedback codes.*/
{
    VCS_FINGER_REJECT_REASON_FEATURE_EX_FAILURE = 67,    /** Feature extraction failed on this image.**/
    VCS_FINGER_REJECT_REASON_SAME_AS_PREVIOUS,           /** Current image very similar to one of previous.**/
    VCS_FINGER_REJECT_REASON_BAD_QUALITY,                /** Current image doesnt notcover sensor sufficiently **/
    VCS_FINGER_REJECT_REASON_LOW_COVERAGE,                /** Current image doesnt cover sensor sufficiently **/
    VCS_FINGER_REJECT_REASON_FIXED_PATTERN_LIMIT_EXCEEDED /** Too many images exhibit fixed pattern **/
} vcsRejectReason_t;

/**< Enrollment quality flags */
typedef vcsUint32_t      vcsEnrollQuality_t;

#define VCS_ENROLL_QUALITY_SCORE                 ((vcsEnrollQuality_t) 0x0000007f) /** first 8 bits indicate completeness of enrollment, score range 0-100. further bits are flags for error situations **/
#define VCS_ENROLL_QUALITY_CALIBRATION_ERROR     ((vcsEnrollQuality_t) 0x00000100) /** bit indicating calibration error in enrollment images */
#define VCS_ENROLL_QUALITY_NORMALISED_ENROLLMENT ((vcsEnrollQuality_t) 0x00000200) /** bit indicating normalised enrollment images */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __vcsClient_h__ */
