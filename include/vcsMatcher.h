/*! @file vcsMatcher.h
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
**  This header file contains definitions and APIs to work with vcsImage.
**
**
*/

#ifndef __vcsMatcher_h__
#define __vcsMatcher_h__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "vcsTypes.h"
#include "vcsResults.h"
#include "vcsImage.h"
#include "vcsTemplate.h"

/******************************************************************************
*                       Match Results Related APIs                            *
******************************************************************************/

/**< Handle to the matching result. */
typedef struct vcsMatchResults_s *  vcsMatchResultsHandle_t;

/*!
*******************************************************************************
**  Return index of matching template (if match).
**
**  @param[in]  hMatchResults   Handle to match results.
**  @param[out] pIndex          Pointer to store index of matching template.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsMatchResultsGetIndex(
    vcsMatchResultsHandle_t hMatchResults,
    vcsUint32_t *           pIndex);

/*!
*******************************************************************************
**  Return match Score, For Validity Use Only.
**
**  @param[in]  hMatchResults   Handle to match results.
**  @param[out] pScore          Pointer to store match score.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsMatchResultsGetScore(
    vcsMatchResultsHandle_t hMatchResults,
    vcsUint32_t *           pScore);

/*!
*******************************************************************************
**  Return liveness Score, For Validity Use Only.
**
**  @param[in]  hMatchResults   Handle to match results.
**  @param[out] pScore          Pointer to store liveness score.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsMatchResultsGetAspScore(
vcsMatchResultsHandle_t hMatchResults,
vcsInt32_t *           pScore);

    /*!
*******************************************************************************
**  Return liveness Score, For Validity Use Only.
**
**  @param[in]  hMatchResults   Handle to match results.
**  @param[out] pSplScore       Pointer to store spl sensor-based scores
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsMatchResultsGetSplAspScore(
vcsMatchResultsHandle_t hMatchResults,
vcsInt32_t *           pSplScore);

/*!
*******************************************************************************
**  Return spoof factor, For Validity Use Only.
**
**  @param[in]  hMatchResults   Handle to match results.
**  @param[out] pScore          Pointer to store spoof factor.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsMatchResultsGetSpoofFactor(
vcsMatchResultsHandle_t hMatchResults,
vcsInt32_t *           pSpoofFactor);

/*!
*******************************************************************************
**  Return match strength, irrespective of match threshold.
**
**  @param[in]  hMatchResults   Handle to match results.
**  @param[out] pStrength       Pointer to store match strength.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsMatchResultsGetMatchStrength(
    vcsMatchResultsHandle_t hMatchResults,
    vcsUint8_t *            pStrength);

/*!
*******************************************************************************
**  Return match status: VCS_TRUE if successful, VCS_FALSE otherwise.
**
**  @param[in]  hMatchResults   Handle to match results.
**  @param[out] pMatch          Pointer to store match status.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsMatchResultsGetStatus(
    vcsMatchResultsHandle_t hMatchResults,
    vcsBool_t *             pMatch);

/*!
*******************************************************************************
**  Return whether enroll template was updated and should be re-saved.
**
**  @param[in]  hMatchResults   Handle to match results.
**  @param[out] pUpdated        Pointer to store update status.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsMatchResultsGetLive(
    vcsMatchResultsHandle_t hMatchResults,
    vcsBool_t *             pLive);
extern vcsResult_t VCS_API
vcsMatchResultsGetTemplateId(
    vcsMatchResultsHandle_t hMatchResults,
    vcsTemplateId_t *       pId);

/*!
*******************************************************************************
**  Return id of the template.
**
**  @param[in]  hMatchResults   Handle to match results.
**  @param[out] pId             Pointer to store template ID.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsMatchResultsGetTemplateId(
    vcsMatchResultsHandle_t hMatchResults,
    vcsTemplateId_t *       pId);

/*!
*******************************************************************************
**  Return whether enroll template was updated and should be re-saved.
**
**  @param[in]  hMatchResults   Handle to match results.
**  @param[out] pUpdated        Pointer to store update status.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsMatchResultsGetEnrollUpdate(
    vcsMatchResultsHandle_t hMatchResults,
    vcsBool_t *             pUpdated);

/*!
*******************************************************************************
**  Return percent of sensor area covered by this verification image.
**
**  @param[in]  hMatchResults   Handle to match results.
**  @param[out] pFingerCoverage Pointer to store percent area.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsMatchResultsGetFingerCoverage(
vcsMatchResultsHandle_t hMatchResults,
vcsUint32_t *           pFingerCoverage);

/*!
*******************************************************************************
**  Return quality of this verification image on a scale of 0-100.
**
**  @param[in]  hMatchResults     Handle to match results.
**  @param[out] pVerifyImgQuality Pointer to store verification image quality.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsMatchResultsGetVerifyImgQuality(
vcsMatchResultsHandle_t hMatchResults,
vcsUint32_t *           pVerifyImgQuality);


/*!
*******************************************************************************
**  Match results status flag definitions
*/
/* Window is fully open */
#define VCS_MATCHER_TEMPLATE_STATUS_WINDOW_OPEN_FULLY             1
/* Window is partially open */
#define VCS_MATCHER_TEMPLATE_STATUS_WINDOW_OPEN_PARTIALLY         2
/* Enroll template was updated in some way and must be saved */
#define VCS_MATCHER_TEMPLATE_STATUS_TEMPLATE_UPDATED              4
/* Enroll template match stats were updated */
#define VCS_MATCHER_TEMPLATE_STATUS_TEMPLATE_STATS_UPDATED        8
/* Enroll template biometric data were updated (added/replaced/removed) */
#define VCS_MATCHER_TEMPLATE_STATUS_TEMPLATE_BIODATA_UPDATED     16
/* Enroll template biometric data were added */
#define VCS_MATCHER_TEMPLATE_STATUS_TEMPLATE_BIODATA_ADDED       32
/* Enroll template biometric data were replaced */
#define VCS_MATCHER_TEMPLATE_STATUS_TEMPLATE_BIODATA_REPLACED    64
/* Enroll template biometric data were removed */
#define VCS_MATCHER_TEMPLATE_STATUS_TEMPLATE_BIODATA_REMOVED    128
/* Enroll template biometric data were reordered */
#define VCS_MATCHER_TEMPLATE_STATUS_TEMPLATE_BIODATA_REORDERED  256
/* Enroll template biometric data is full. Next one will cause replacement. */
#define VCS_MATCHER_TEMPLATE_STATUS_TEMPLATE_BIODATA_FULL       512

/*!
*******************************************************************************
**  Return enroll template status. Possible values are defined above.
**
**  @param[in]  hMatchResults   Handle to match results.
**  @param[out] pStatus         Pointer to store template status.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsMatchResultsGetEnrollTemplateStatus(
    vcsMatchResultsHandle_t hMatchResults,
    vcsUint32_t *           pStatus);

/*!
*******************************************************************************
**  Free the resource allocated for the vcsMatchResultsHandle.
**
**  @param[in]  hMatchResults   Handle to match results.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsMatchResultsDestroyHandle(
    vcsMatchResultsHandle_t hMatchResults);

/******************************************************************************
*                       Match Details Related APIs                            *
******************************************************************************/

/**< Handle to the matching details. */
typedef struct vcsMatchDetails_s * vcsMatchDetailsHandle_t;

/*!
*******************************************************************************
**  This function returns match details handle from provided match results
**  handle.
**
**  @param[in]  hMatchResults       Handle to match results.
**  @param[out] phMatchDetails      Pointer to store match details handle.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API VCS_API
vcsMatchResultsGetMatchDetails(
    vcsMatchResultsHandle_t     hMatchResults,
    vcsMatchDetailsHandle_t *   phMatchDetails);

/*!
*******************************************************************************
**  @param[in]  hMatchDetails       Handle to match details.
**  @param[out] pFMinuCount
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API VCS_API
vcsMatchDetailsGetFMinuCount(
    vcsMatchDetailsHandle_t hMatchDetails,
    vcsUint8_t *            pFMinuCount);

/*!
*******************************************************************************
**  @param[in]      hMatchDetails       Handle to match details.
**  @param[in/out]  pSize               
**  @param[out]     pCorrMinuCount      
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API VCS_API
vcsMatchDetailsGetCorrMinuCount(
    vcsMatchDetailsHandle_t hMatchDetails,
    vcsUint32_t *           pSize,
    vcsUint8_t *            pCorrMinuCount);

/*!
*******************************************************************************
**  @param[in]      hMatchDetails       Handle to match details.
**  @param[in/out]  pSize               
**  @param[out]     pVMinuCount         
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API VCS_API
vcsMatchDetailsGetVMinuCount(
    vcsMatchDetailsHandle_t hMatchDetails,
    vcsUint32_t *           pSize,
    vcsUint8_t *            pVMinuCount);

/*!
*******************************************************************************
**  Free the resource allocated for the vcsMatchResultsHandle.
**
**  @param[in]      hMatchDetails       Handle to match details.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsMatchDetailsDestroyHandle(
    vcsMatchDetailsHandle_t hMatchDetails);


/******************************************************************************
*                           Matcher Related APIs                              *
******************************************************************************/

/*!
*******************************************************************************
**  Identifier which should be used to open matcher.
*/
typedef vcsUint32_t vcsMatcherId_t;

/**< A real matcher ID is always a number greater than 100. The first 2(or 1) digits
     will indicate the base matcher type and second 2 digits indicate the flavour.
     So overall 99 base matchers and 100 flavours of each are supported */

#define GetBaseMatcherId(matcherId) (((matcherId)/100)*100)

/**< Let VCS API select the default matcher internally */
#define VCS_MATCHER_DEFAULT             ((vcsMatcherId_t) 0)

#define VCS_MATCHER_DP_BASE             ((vcsMatcherId_t) 100)                       /* this is base id and not to be used as real matcher ID */
#define VCS_MATCHER_DP                  ((vcsMatcherId_t) VCS_MATCHER_DP_BASE + 1)   /* standard minutiae matcher */
#define VCS_MATCHER_EEM                 ((vcsMatcherId_t) VCS_MATCHER_DP_BASE + 2)   /* extended enrollment version of minutiae matcher */
#define VCS_MATCHER_MV                  ((vcsMatcherId_t) VCS_MATCHER_DP_BASE + 3)   /* multiview version of minutiae matcher */

#define VCS_MATCHER_M7_BASE             ((vcsMatcherId_t) 200)                       /* this is base id and not to be used as real matcher ID */
#define VCS_MATCHER_RM                  ((vcsMatcherId_t) VCS_MATCHER_M7_BASE + 1)   /* ridge matcher */
#define VCS_MATCHER_QM                  ((vcsMatcherId_t) VCS_MATCHER_M7_BASE + 2)   /* Quantum matcher */

#define VCS_MATCHER_PB_BASE                         ((vcsMatcherId_t) 400)                     /* this is base id and not to be used as real matcher ID */
#define VCS_MATCHER_PB_EHM_SQ_XL                    ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 1) /* 10x10 mm - full blown best biometric performance matcher */
#define VCS_MATCHER_PB_EHM_SQ_XL_SPEED              ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 2) /* 10x10 mm - 'optimized for speed' trades off accuracy */
#define VCS_MATCHER_PB_EHM_SQ_XL_SPEED_MEM          ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 3) /* 10x10 mm - 'optimized for speed and memory', trades off accuracy */
#define VCS_MATCHER_PB_EHM_SQ_XL_SLICE              ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 4) /* 10x10 mm - slice extractor, lesser memory than full blown with speed traded off */
#define VCS_MATCHER_PB_EHM_SQ_XL_SLICE_SPEED        ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 5) /* 10x10 mm - 'optimized for speed' trades off accuracy */
#define VCS_MATCHER_PB_EHM_SQ_XL_SLICE_SPEED_MEM    ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 6) /* 10x10 mm - 'optimized for speed and memory', trades off accuracy */
#define VCS_MATCHER_PB_EHM_SQ_L                     ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 7)  /* 8x8 mm - full blown best biometric performance matcher */
#define VCS_MATCHER_PB_EHM_SQ_L_SPEED               ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 8)  /* 8x8 mm - 'optimized for speed' trades off accuracy */
#define VCS_MATCHER_PB_EHM_SQ_L_SPEED_MEM           ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 9)  /* 8x8 mm - 'optimized for speed and memory', trades off accuracy */
#define VCS_MATCHER_PB_EHM_SQ_L_SLICE               ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 10) /* 8x8 mm - slice extractor, lesser memory than full blown with speed traded off */
#define VCS_MATCHER_PB_EHM_SQ_L_SLICE_SPEED         ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 11) /* 8x8 mm - 'optimized for speed' trades off accuracy */
#define VCS_MATCHER_PB_EHM_SQ_L_SLICE_SPEED_MEM     ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 12) /* 8x8 mm - 'optimized for speed and memory', trades off accuracy */
#define VCS_MATCHER_PB_EHM_SQ_S                     ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 13) /* 5.8x5.8 mm - full blown best biometric performance matcher */
#define VCS_MATCHER_PB_EHM_SQ_S_SPEED               ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 14) /* 5.8x5.8 mm - 'optimized for speed' trades off accuracy */
#define VCS_MATCHER_PB_EHM_SQ_S_SPEED_MEM           ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 15) /* 5.8x5.8 mm - 'optimized for speed and memory', trades off accuracy */
#define VCS_MATCHER_PB_EHM_SQ_XS                    ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 16) /* 5x5 mm - full blown best biometric performance matcher */
#define VCS_MATCHER_PB_EHM_REC_M                    ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 17) /* 10x4 mm - full blown best biometric performance matcher */
#define VCS_MATCHER_PB_EHM_REC_M_SPEED              ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 18) /* 10x4 mm - 'optimized for speed' trades off accuracy */
#define VCS_MATCHER_PB_EHM_REC_M_SPEED_MEM          ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 19) /* 10x4 mm - 'optimized for speed and memory', trades off accuracy */
#define VCS_MATCHER_PB_EHM_REC_M_NON_HR             ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 20) /* 10x4 mm - no high resolution, smaller templates than full blown with accuracy traded off */
#define VCS_MATCHER_PB_EHM_REC_M_NON_HR_SPEED       ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 21) /* 10x4 mm - 'optimized for speed' trades off accuracy */
#define VCS_MATCHER_PB_EHM_REC_M_NON_HR_SPEED_MEM   ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 22) /* 10x4 mm - 'optimized for speed and memory', trades off accuracy */
#define VCS_MATCHER_PB_EHM_REC_S                    ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 23) /* 10x3 mm - full blown best biometric performance matcher */
#define VCS_MATCHER_PB_EHM_SWIPE                    ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 24) /* Swipe - full blown best biometric performance matcher */
#define VCS_MATCHER_PB_EHM_SWIPE_SPEED              ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 25) /* Swipe - 'optimized for speed' trades off accuracy */
#define VCS_MATCHER_PB_EHM_SWIPE_SPEED_MEM          ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 26) /* Swipe - 'optimized for speed and memory', trades off accuracy */
#define VCS_MATCHER_PB_EHM_SWIPE_SLICE              ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 27) /* Swipe - slice extractor, lesser memory than full blown with speed traded off */
#define VCS_MATCHER_PB_EHM_SWIPE_SLICE_SPEED        ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 28) /* Swipe - 'optimized for speed' trades off accuracy */
#define VCS_MATCHER_PB_EHM_SWIPE_SLICE_SPEED_MEM    ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 29) /* Swipe - 'optimized for speed and memory', trades off accuracy */
/* IMPORTANT IMPORTANT IMPORTANT IMPORTANT - ensure the ids above and below this line remain equal */
#define VCS_MATCHER_PB_SPEC_BASE                    ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 29) /* base id for spectral matchers which are for low resource environments like on-chip, expect lower biometric performance */
#define VCS_MATCHER_PB_SPEC_SQ_XL                   ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 30) /* 10x10 mm */
#define VCS_MATCHER_PB_SPEC_SQ_XL_SLICE             ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 31) /* 10x10 mm - slice extractor, lesser memory than full blown with speed traded off */
#define VCS_MATCHER_PB_SPEC_SQ_L                    ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 32) /* 8x8 mm */
#define VCS_MATCHER_PB_SPEC_SQ_L_SLICE              ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 33) /* 8x8 mm - slice extractor, lesser memory than full blown with speed traded off */
#define VCS_MATCHER_PB_SPEC_SQ_S                    ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 34) /* 5.8x5.8 mm */
#define VCS_MATCHER_PB_SPEC_SWIPE                   ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 35) /* Swipe */
#define VCS_MATCHER_PB_SPEC_SWIPE_SLICE             ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 36) /* Swipe - slice extractor, lesser memory than full blown with speed traded off */
#define VCS_MATCHER_PB_EHM_SQ_M                     ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 37)  /* 6.5x6.5 mm - full blown best biometric performance matcher */
#define VCS_MATCHER_PB_EHM_SQ_M_SPEED               ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 38)  /* 6.5x6.5 mm - 'optimized for speed' trades off accuracy */
#define VCS_MATCHER_PB_EHM_SQ_M_SPEED_MEM           ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 39)  /* 6.5x6.5 mm - 'optimized for speed and memory', trades off accuracy */
#define VCS_MATCHER_PB_SQ_S_NON_HR                  ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 40)  /* 5.8x5.8 mm - full blown best biometric performance matcher */
#define VCS_MATCHER_PB_SQ_S_NON_HR_SPEED            ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 41)  /* 5.8x5.8 mm - 'optimized for speed' trades off accuracy */
#define VCS_MATCHER_PB_SQ_S_NON_HR_SPEED_MEM        ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 42)  /* 5.8x5.8 mm - 'optimized for speed and memory', trades off accuracy */
#define VCS_MATCHER_PB_EHM_SQ_M_NON_HR              ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 43)  /* 6.5x6.5 mm - no high resolution, smaller templates than full blown with accuracy traded off */
#define VCS_MATCHER_PB_EHM_SQ_M_NON_HR_SPEED        ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 44)  /* 6.5x6.5 mm - 'optimized for speed' trades off accuracy */
#define VCS_MATCHER_PB_EHM_SQ_M_NON_HR_SPEED_MEM    ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 45)  /* 6.5x6.5 mm - 'optimized for speed and memory', trades off accuracy */
#define VCS_MATCHER_PB_TOP                          ((vcsMatcherId_t) VCS_MATCHER_PB_BASE + 46) /* this is summit id and not to be used as real matcher ID */

#define isPbMatcher(MatcherId) ((MatcherId > VCS_MATCHER_PB_BASE) && (MatcherId < VCS_MATCHER_PB_TOP))
#define isPbSpectralMatcher(MatcherId) ((MatcherId > VCS_MATCHER_PB_SPEC_BASE) && (MatcherId < VCS_MATCHER_PB_TOP))
#define isPbSpeedMemMatcher(MatcherId) ((MatcherId == VCS_MATCHER_PB_EHM_SQ_XL_SPEED_MEM) || \
                                        (MatcherId == VCS_MATCHER_PB_EHM_SQ_XL_SLICE_SPEED_MEM) || \
                                        (MatcherId == VCS_MATCHER_PB_EHM_SQ_L_SPEED_MEM) || \
                                        (MatcherId == VCS_MATCHER_PB_EHM_SQ_L_SLICE_SPEED_MEM) || \
                                        (MatcherId == VCS_MATCHER_PB_EHM_SQ_M_SPEED_MEM) || \
                                        (MatcherId == VCS_MATCHER_PB_EHM_SQ_S_SPEED_MEM) || \
										(MatcherId == VCS_MATCHER_PB_SQ_S_NON_HR_SPEED_MEM) || \
                                        (MatcherId == VCS_MATCHER_PB_EHM_REC_M_SPEED_MEM) || \
                                        (MatcherId == VCS_MATCHER_PB_EHM_REC_M_NON_HR_SPEED_MEM) || \
                                        (MatcherId == VCS_MATCHER_PB_EHM_SWIPE_SPEED_MEM) || \
                                        (MatcherId == VCS_MATCHER_PB_EHM_SWIPE_SLICE_SPEED_MEM) || \
                                        (MatcherId == VCS_MATCHER_PB_EHM_SQ_M_NON_HR_SPEED_MEM))

#define VCS_MATCHER_EGIS_BASE           ((vcsMatcherId_t) 500)                       /* this is base id and not to be used as real matcher ID */
#define VCS_MATCHER_EGIS                ((vcsMatcherId_t) VCS_MATCHER_EGIS_BASE + 1) /* standard Egis matcher */
#define VCS_RESULT_MATCHER_NONCRITICAL_ENROLL_ERROR                 ((vcsResult_t) 200)
#define VCS_RESULT_MATCHER_NONCRITICAL_ENROLL_ERROR_REDUNDANT_IMAGE ((vcsResult_t) 201)
#define VCS_NONCRITICAL_ENROLL_ERROR(_status) ((_status) == VCS_RESULT_MATCHER_NONCRITICAL_ENROLL_ERROR)
#define VCS_NONCRITICAL_ENROLL_ERROR_REDUNDANCY(_status) ((_status) == VCS_RESULT_MATCHER_NONCRITICAL_ENROLL_ERROR_REDUNDANT_IMAGE)

/*!
*******************************************************************************
**  Match Return Policy
**  For functions that do matching, this enumeration determines whether
**  the function is to return when the first matching template is found,
**  or it is to return only when the best matching template has been
**  found (i.e. the entire template list has been searched).
*/
typedef vcsUint32_t vcsMatchReturnPolicy_t;

/**< Match with all templates and return all results and updated template too. */
#define VCS_ALL_MATCH ((vcsMatchReturnPolicy_t)  1)
/**< Return on first matching template. */
#define VCS_ANY_MATCH  ((vcsMatchReturnPolicy_t)  2)

/**< Matcher accuracy definitions. */
/**< Increasing order of 4 security levels, absolute values will vary with implementations */
#define VCS_MATCHER_ACCURACY_LOW            0
#define VCS_MATCHER_ACCURACY_REGULAR        1
#define VCS_MATCHER_ACCURACY_HIGH           2
#define VCS_MATCHER_ACCURACY_EXTRAHIGH      3

/**< Handle to the opened matcher. */
typedef struct mssMatcher_s *  vcsMatcherHandle_t;

/*!
*******************************************************************************
**  Open matcher by specified ID.
**
**  @param[in]  matcherId     ID of matcher to be opened.
**  @param[in]  pInData       Pointer to matcher input data.
**  @param[out] phMatcher     Pointer to the matcher handle.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsMatcherOpen(
    vcsMatcherId_t          matcherId,
    const vcsBlobData_t *   pInData,
    vcsMatcherHandle_t *    phMatcher);

/*!
*******************************************************************************
**  Close open matcher by handle. This function also destroys a matcher handle
**  previously allocated as the result of a call to vcsMatcherOpen. Since the
**  mentioned function create a matcher handle and passes it back, it is up to
**  the caller to call vcsMatcherClose() when he/she is done with it.
**
**  @param[in]  hMatcher      Handle to VCS matcher information.
**  @param[out] pOutData      Pointer to store matcher out data.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsMatcherClose(
    vcsMatcherHandle_t  hMatcher,
    vcsBlobData_t *     pOutData);

/*!
*******************************************************************************
**  This function set matcher parameters.
**
**  @param[in]  hMatcher    Handle to VCS matcher information.
**  @param[in]  id          Parameter ID.
**  @param[in]  value       Parameter value.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
**/
extern vcsResult_t VCS_API
vcsMatcherSetParameter(
    vcsMatcherHandle_t  hMatcher,
    vcsUint32_t         id,
    vcsUint32_t         value);

/*!
*******************************************************************************
**  This function set matcher parameters.
**
**  @param[in]  hMatcher    Handle to VCS matcher information.
**  @param[in]  id          Parameter ID.
**  @param[out] pValue      Pointer to store parameter value.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
**/
extern vcsResult_t VCS_API
vcsMatcherGetParameter(
    vcsMatcherHandle_t  hMatcher,
    vcsUint32_t         id,
    vcsUint32_t *       pValue);

/*!
*******************************************************************************
**  Return unique identifier for the matcher.
**
**  @param[in]  hMatcher      Handle to VCS matcher information.
**  @param[out] pMatcherId    Pointer to store matcher ID.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsMatcherGetId(
    vcsMatcherHandle_t  hMatcher,
    vcsMatcherId_t *    pMatcherId);

/*!
*******************************************************************************
**  Return template type supported by this matcher.
**
**  @param[in]  hMatcher        Handle to VCS matcher information.
**  @param[out] pTemplateType   Pointer to store template type.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsMatcherGetTemplateType(
    vcsMatcherHandle_t  hMatcher,
    vcsUint32_t *       pTemplateType);

/*!
*******************************************************************************
**  Return the size of the matcher identification string.
**
**  @param[in]  hMatcher    Handle to VCS matcher information.
**  @param[out] pSize       Pointer to store matcher id string size.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsMatcherGetIdStringSize(
    vcsMatcherHandle_t  hMatcher,
    vcsUint32_t *       pSize);

/*!
*******************************************************************************
**  Return matcher identification string (for convenience).
**
**  @param[in]  hMatcher            Handle to VCS matcher information.
**  @param[out] sMatcherIDString    Pointer to store matcher id in string format.
**  @param[in]  size                Matcher id string size.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsMatcherGetIdStringW(
    vcsMatcherHandle_t  hMatcher,
    vcsUnicodeChar_t *  sMatcherIDString,
    vcsUint32_t         size);
extern vcsResult_t VCS_API
vcsMatcherGetIdStringA(
    vcsMatcherHandle_t  hMatcher,
    vcsAsciiChar_t *    sMatcherIDString,
    vcsUint32_t         size);
#ifdef UNICODE
#define vcsMatcherGetIdString vcsMatcherGetIdStringW
#else /* UNICODE */
#define vcsMatcherGetIdString vcsMatcherGetIdStringA
#endif /* UNICODE */

typedef vcsUint32_t vcsMatcherImageQuality_t;

/**< Image quality is good for matching operations. */
#define VCS_MATCHER_IMAGE_QUALITY_GOOD ((vcsMatcherImageQuality_t) 0)

/**< Image quality is not acceptable for matching operations. */
#define VCS_MATCHER_IMAGE_QUALITY_POOR ((vcsMatcherImageQuality_t) 1)

/*!
*******************************************************************************
**  This function checks if the image has good quality for matching operations.
**
**  @param[in]  hMatcher    Handle to VCS matcher information.
**  @param[in]  hImage      Handle to a fingerprint Image to be checked.
**  @param[out] pQuality    Pointer to the quality to be returned.
**                          Can be one of these two values:
**                              VCS_MATCHER_IMAGE_QUALITY_GOOD
**                              VCS_MATCHER_IMAGE_QUALITY_POOR
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsMatcherCheckImageQuality(
    vcsMatcherHandle_t                  hMatcher,
    vcsImageHandle_t                    hImage,
    vcsMatcherImageQuality_t *          pQuality);

typedef vcsInt32_t vcsTemplateCreationStatus_t;

/**< Template successfully created. */
#define VCS_TEMPLATE_CREATION_SUCCESS   ((vcsTemplateCreationStatus_t)  400)
/**< Template creation failed. */
#define VCS_TEMPLATE_CREATION_FAILURE   ((vcsTemplateCreationStatus_t)  401)
/**< Template creation incomplete; need another image. */
#define VCS_TEMPLATE_CREATION_INCOMP    ((vcsTemplateCreationStatus_t)  402)

/*!
*******************************************************************************
**  Adds the information from a fingerprint image to an enroll template. The
**  first time this is called, the template must be empty. It is up to this
**  function how to handle the addition of the image information, and pass back
**  an indication when it is done forming a template. The function should be
**  called with a new image until this "template complete" indication is given.
**
**  @param[in]  hMatcher    Handle to VCS matcher information.
**  @param[in]  hImage      Handle to a fingerprint Image to be used to update
**                          the enroll template.
**  @param[out] pStatus     Pointer to the template creation status handle.
**                          Can be one of these three values.
**                              VCS_TEMPLATE_CREATION_SUCCESS
**                              VCS_TEMPLATE_CREATION_FAILED
**                              VCS_TEMPLATE_CREATION_INCOMPLETE
**  @param[out] phTemplate  Handle to a template to be updated with the
**                          information from the image. The contents of this
**                          should initially be a VCS_NULL pointer to indicate
**                          to the function that it needs to do the allocation.
**                          The called function is responsible for the
**                          allocation of the template being built.
**
**  @return VCS return code, VCS_RESULT_OK if no error,
**  VCS_RESULT_MATCHER_ADD_IMAGE_FAILED if error occurs but enrollment can proceed,
**  VCS_RESULT_MATCHER_ENROLL_FAILED if error occurs and enrollment can not proceed
**/
extern vcsResult_t VCS_API
vcsMatcherAddImageToTemplate(
    vcsMatcherHandle_t              hMatcher,
    vcsImageHandle_t                hImage,
    vcsTemplateCreationStatus_t *   pStatus,
    vcsTemplateHandle_t *           phTemplate);

#if VCS_FEATURE_ENROLL_PROGRESS_IMAGE
/*!
*******************************************************************************
**  Retrieves enrollment progress image from the matcher, if present
**
**  @param[in]  hMatcher    Handle to VCS matcher information.
**  @param[out] phImage     Pointer to Handle to a progress Image.
**
**  @return VCS return code, VCS_RESULT_OK if no error,
**  VCS_RESULT_MATCHER_ADD_IMAGE_FAILED if error occurs but enrollment can proceed,
**  VCS_RESULT_MATCHER_ENROLL_FAILED if error occurs and enrollment can not proceed
**/
extern vcsResult_t VCS_API
vcsMatcherGetEnrollProgressImage(
    vcsMatcherHandle_t            pMatcher,
    vcsImageHandle_t *            phImage);
#endif

/*!
*******************************************************************************
**  This function matches a given fingerprint image to a population of one or
**  more enroll templates.
**
**  @param[in]  hMatcher            Handle to VCS matcher information.
**  @param[in]  hImage              Handle to a fingerprint Image to be matched
**                                  against the list of templates.
**  @param[in]  phTemplates         An array of pointers to enroll templates to
**                                  be matched against.
**  @param[in]  numTemplates        The number of template pointers in the
**                                  phTemplates array.
**  @param[in]  matchReturnPolicy   An enumeration of whether to return on any
**                                  match or return on the best match.
**  @param[out] phMatchResults      Pointer to the match result data handle.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
**/
/* FIXME FIXME FIXME - THIS FUNCTION IS SCHEDULED FOR DEPRECATION - Vijay 20140801 */
extern vcsResult_t VCS_API
vcsMatcherMatchImageToTemplates(
    vcsMatcherHandle_t          hMatcher,
    vcsImageHandle_t            hImage,
    vcsTemplateHandle_t         phTemplates[],
    vcsUint32_t                 numTemplates,
    vcsMatchReturnPolicy_t      matchReturnPolicy,
    vcsMatchResultsHandle_t *   phMatchResults);

/*!
*******************************************************************************
**  This function matches a given fingerprint image to a population of one or
**  more enroll templates. This function is similar to
**  vcsMatcherMatchImageToTemplates but has extra capability of returning more
**  than one match score.
**
**  @param[in]      hMatcher            Handle to VCS matcher information.
**  @param[in]      hImage              Handle to a fingerprint Image to be
**                                      matched against the list of templates.
**  @param[in]      phTemplates         An array of pointers to enroll templates
**                                      to be matched against.
**  @param[in]      numTemplates        The number of template pointers in the
**                                      phTemplates array.
**  @param[in]      matchReturnPolicy   An enumeration of whether to return on any
**                                      match or return on the best match.
**  @param[out]     phMatchResults      Pointer to the match result data handle.
**                                      If matchReturnPolicy is VCS_ANY_MATCH
**                                      this will be a pointer to one
**                                      vcsMatchResultsHandle_t. If matchReturnPolicy
**                                      is VCS_ALL_MATCH this will be a pointer to
**                                      numTemplates vcsMatchResultsHandle_t(s).
**                                      It is callers responsibility to destroy the
**                                      output handle(s).
**
**    @return VCS return code, VCS_RESULT_OK if no error.
**/
extern vcsResult_t VCS_API
vcsMatcherMatchImageToTemplatesEx(
    vcsMatcherHandle_t          hMatcher,
    vcsImageHandle_t            hImage,
    vcsTemplateHandle_t         phTemplates[],
    vcsUint32_t                 numTemplates,
    vcsMatchReturnPolicy_t      matchReturnPolicy,
    vcsMatchResultsHandle_t *   phMatchResults);

/*!
*******************************************************************************
**  After a call to vcsMatcherMatchImageToTemplatesEx with match return policy
**  VCS_ANY_MATCH this function will be called to fetch the updated template
**
**  @param[in]  hMatcher    Handle to VCS matcher information.
**  @param[out] phTemplate  Handle to updated template, VCS_NULL if no update happened
**
**  @return VCS return code, VCS_RESULT_OK if no error,
**/
extern vcsResult_t VCS_API
vcsMatcherGetUpdatedTemplate(
    vcsMatcherHandle_t              hMatcher,
    vcsTemplateHandle_t *           phTemplate);

/*!
*******************************************************************************
**  This function extract the template from a fingerprint image.
**
**  @param[in]  hMatcher        Handle to VCS matcher information.
**  @param[in]  hImage          Handle to a fingerprint Image to be used to
**                              create the enroll template.
**  @param[out] phTemplate      Pointer to the template handle.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
**/
extern vcsResult_t VCS_API
vcsMatcherExtractTemplate(
    vcsMatcherHandle_t      hMatcher,
    vcsImageHandle_t        hImage,
    vcsTemplateHandle_t *   phTemplate);

/*!
*******************************************************************************
**  This function extract the template from a fingerprint image.
**
**  @param[in]  hMatcher        Handle to VCS matcher information.
**  @param[in]  hImages         Handles to fingerprint Images to be used to
**                              force create the enroll template.
**  @param[in]  numImages       Number of images.
**  @param[out] phTemplate      Pointer to the template handle.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
**/
extern vcsResult_t VCS_API
vcsMatcherExtractTemplateEx(
    vcsMatcherHandle_t      hMatcher,
    vcsImageHandle_t        hImages[],
    vcsUint32_t             numImages,
    vcsTemplateHandle_t *   phTemplate);

/*!
*******************************************************************************
**  This function matches two given templates.
**
**  @param[in]  hMatcher           Handle to VCS matcher information.
**  @param[in]  hTemplate1         Handle to a template to be matched against
**                                 the hTemplate2.
**  @param[in]  hTemplate2         An handle to enroll template to be matched
**                                  against.
**  @param[out] phMatchResults     Pointer to the match result data handle.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
**  VCS_RESULT_GEN_NOT_IMPLEMENTED if not supported by matcher.
**/
extern vcsResult_t VCS_API
vcsMatcherMatchTemplates(
    vcsMatcherHandle_t          hMatcher,
    vcsTemplateHandle_t         hTemplate1,
    vcsTemplateHandle_t         hTemplate2,
    vcsMatchResultsHandle_t *   phMatchResults);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __vcsMatcher_h__ */
