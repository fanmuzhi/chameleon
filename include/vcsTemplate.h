/*! @file vcsTemplate.h
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
**  This header file contains definitions and APIs to work with vcsTemplate.
**
** 
*/

#ifndef __vcsTemplate_h__
#define __vcsTemplate_h__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "vcsTypes.h"
#include "vcsResults.h"
#include "vcsDefs.h"

#define VCS_HANDLE_TAG_VCSTEMPLATE      ('V' + ('T' << 8) + ('M' << 16) + ('L' << 24))

typedef vcsUint32_t vcsEemParameterType_t;
/* for use with the vcsTemplateSetParamEEM() function only */
#define  VCS_EEM_PARAM_TYPE_SWIPES_SEEN    ((vcsEemParameterType_t)  1) /**< set number of swipes seen. Window closes after this increments to VCS_PARAM_SWIPES_WINDOW count. */
#define  VCS_EEM_PARAM_TYPE_TIME_CREATED   ((vcsEemParameterType_t)  2) /**< set amount of time in secs when template created. 0 sets to current time. Window closes after VCS_PARAM_TIME_WINDOW time has passed since this value.*/
#define  VCS_EEM_PARAM_TYPE_RESET_WINDOW   ((vcsEemParameterType_t)  3) /**< Resets num swipes seen to 0 and time created to current time. Window will definitely be open after this call.*/

/* vcs template pack bit flags */
#define VCS_TEMPLATE_PACK_ENCRYPTED         0x00000001  /**< Encrypted packed template */

/**< Template handle. */
typedef struct vcsEnrollTemplate_s *   vcsTemplateHandle_t;

/*!
*******************************************************************************
**  Create template from given data and data size.
**
**  @param[out] phTemplate      Pointer to store template handle to match results.
**  @param[in]  templateData    Template data blob.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsTemplateCreate(
    vcsTemplateHandle_t *   phTemplate,
    vcsBlobData_t           templateData);

/*!
*******************************************************************************
**  Destroy template by given handle.
**
**  @param[in]  hTemplate      Template handle.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsTemplateDestroy(
    vcsTemplateHandle_t hTemplate);

/*!
*******************************************************************************
**  Clone provided template.
**
**  @param[in]  hTemplate           Template handle to be cloned.
**  @param[out] phClonedTemplate    Pointer to store cloned template handle.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsTemplateClone(
    vcsTemplateHandle_t     hTemplate,
    vcsTemplateHandle_t *   phCloneTemplate);

/*!
*******************************************************************************
**  Return size of template data.
**
**  @param[in]  hTemplate   Template handle.
**  @param[out] pSize       Pointer to store template data size.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsTemplateGetDataSize(
    vcsTemplateHandle_t hTemplate,
    vcsUint32_t *       pSize);

/*!
*******************************************************************************
**  Return array of actual template data itself (size must be taken from
**  vcsTemplateGetDataSize function).
**
**  @param[in]  hTemplate       Template handle.
**  @param[out] templateData    Blob store template data.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsTemplateGetData(
    vcsTemplateHandle_t hTemplate,
    vcsBlobData_t       templateData);

/*!
*******************************************************************************
**  Return template Type, Vendor + Template Rev.
**
**  @param[in]  hTemplate   Template handle.
**  @param[out] pType       Pointer to store template type.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsTemplateGetType(
    vcsTemplateHandle_t hTemplate,
    vcsUint32_t *       pType);

/*!
*******************************************************************************
**  Return number of features in the template.
**
**  @param[in]  hTemplate       Template handle.
**  @param[out] pNumFeatures    Pointer to store number of features.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsTemplateGetFeatures(
    vcsTemplateHandle_t hTemplate,
    vcsUint32_t *       pNumFeatures);

/*!
*******************************************************************************
**  Set number of features in the template.
**
**  @param[in]  hTemplate       Template handle.
**  @param[in]  numFeatures     Number of features to be set.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsTemplateSetFeatures(
    vcsTemplateHandle_t hTemplate,
    vcsUint32_t         numFeatures);

/*!
*******************************************************************************
**   Return size of the ASP profile data in bytes
**
**  @param[in]  hTemplate   Template handle.
**  @param[out] pAspSize    Pointer to store ASP profile data size.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsTemplateGetAspProfileSize(
    vcsTemplateHandle_t hTemplate,
    vcsUint32_t *       pAspSize);

/*!
*******************************************************************************
**  Return ASP profile data associated with the template. Size of the profile
**  data should be determined using vcsTemplateGetAspProfileSize().
**
**  @param[in]  hTemplate   Template handle.
**  @param[out] profile     Pointer to store ASP profile data.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsTemplateGetAspProfile(
    vcsTemplateHandle_t hTemplate,
    vcsBlobData_t       profile);

/** Store ASP profile data to the template */
/*!
*******************************************************************************
**  Store ASP profile data to the template.
**
**  @param[in]  hTemplate   Template handle.
**  @param[in]  profile     ASP profile data to be set.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsTemplateSetAspProfile(
    vcsTemplateHandle_t hTemplate,
    vcsBlobData_t       profile);

/*!
*******************************************************************************
**  Pack provided template handle to byte array.
**
**  @param[in]  hTemplate           Template handle.
**  @param[out] pPackedTemplate     Pointer to store packed fingerprint data.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsTemplatePack(
    vcsTemplateHandle_t     hTemplate,
    vcsBlobData_t *         pPackedTemplate);

/*!
*******************************************************************************
**  Unpack provided blob to template and return the handle on Host.
**
**  @param[in]  packedTemplate  Pointer where packed template data is stored.
**  @param[out] phTemplate      Pointer to store template handle.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
vcsResult_t VCS_API
vcsTemplateUnPack(
    const vcsBlobData_t *  pPackedTemplate,
    vcsTemplateHandle_t *  phTemplate);

/*!
*******************************************************************************
**  Set params for an EEM template. Possible param types are defined above.
**
**  @param[in]  hTemplate       Template handle.
**  @param[in]  paramType       Type of the parameter to be set. One of
**                              VCS_EEM_PARAM_TYPE_XXX.
**  @param[in]  paramValue      Parameter value.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsTemplateSetParamEem(
    vcsTemplateHandle_t     hTemplate,
    vcsEemParameterType_t   paramType,
    vcsInt32_t              paramValue);

/*!
*******************************************************************************
**  Computes SHA256 hash of template data
**
**  @param[in]    hTemplate         Template handle.
**  @param[out]   tplDataHashBlob   Hash of template data as blob
**
**  @return VCS return code, VCS_RESULT_OK if no error.
**
**  Note: Caller should allocate enough memory for tplDataHashBlob
*/
extern vcsResult_t VCS_API
vcsTemplateGetDataSHA256(
    vcsTemplateHandle_t hTemplate,
    vcsBlobData_t       tplDataHashBlob);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __vcsTemplate_h__ */
