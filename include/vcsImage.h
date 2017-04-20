/*! @file vcsImage.h
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

#ifndef __vcsImage_h__
#define __vcsImage_h__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "vcsBase.h"        /* Builds up the defines for what platform and os we are building on */
#include "vcsTypes.h"       /* Platform specific type definitions */
#include "vcsImageQuality.h"

/**< Handle to the fingerprint image. */
typedef struct vcsImage_s *   vcsImageHandle_t;

#define VCS_HANDLE_TAG_VCSIMAGE         ('V' + ('I' << 8) + ('M' << 16) + ('G' << 24))

/*!
*******************************************************************************
**  Return number of rows in fingerprint image.
**
**  @param[in]  hImage      Fingerprint image handle.
**  @param[out] pNumRows    Pointer to store number of rows in image.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsImageGetRows(
    vcsImageHandle_t    hImage,
    vcsUint32_t *       pNumRows);

/*!
*******************************************************************************
**  Return number of columns in fingerprint image.
**
**  @param[in]  hImage      Fingerprint image handle.
**  @param[out] pNumCols    Pointer to store number of columns in image.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsImageGetColumns(
    vcsImageHandle_t    hImage,
    vcsUint32_t *       pNumCols);

/*!
*******************************************************************************
**  Return image quality; bits as defined above: VCS_IMAGE_QUALITY_*.
**
**  @param[in]  hImage      Fingerprint image handle.
**  @param[out] pQuality    Pointer to store the quality of the image.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsImageGetQuality(
    vcsImageHandle_t    hImage,
    vcsImageQuality_t * pQuality);


/*!
*******************************************************************************
**  Return xResolution of fingerprint image.
**
**  @param[in]  hImage          Fingerprint image handle.
**  @param[out] pXResolution    Pointer to store the xResolution of the image.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsImageGetXResolution(
    vcsImageHandle_t    hImage,
    vcsInt32_t *        pXResolution);

/*!
*******************************************************************************
**  Return yResolution of fingerprint image.
**
**  @param[in]  hImage          Fingerprint image handle.
**  @param[out] pYResolution    Pointer to store the yResolution of the image.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsImageGetYResolution(
    vcsImageHandle_t    hImage,
    vcsInt32_t *        pYResolution);

/*!
*******************************************************************************
**  Return true if image is usable for the specified operation.
**
**  @param[in]  hImage          Fingerprint image handle.
**  @param[in]  bEnrollment     Operation type (VCS_TRUE for enrollment,
**                              VCS_FALSE for identification/verification).
**  @param[out] pUsable         Pointer to store checking result.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsImageCheckUsability(
    vcsImageHandle_t    hImage,
    vcsBool_t           bEnrollment,
    vcsBool_t *         pUsable);

/*!
*******************************************************************************
**  Return maximum number of bytes allocated for fingerprint.
**
**  @param[in]  hImage      Fingerprint image handle.
**  @param[out] pDataSize   Pointer to store allocated data size.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsImageGetAllocSize(
    vcsImageHandle_t    hImage,
    vcsUint32_t *       pDataSize);

/*!
*******************************************************************************
**  Return swipe speed of fingerprint image.
**
**  @param[in]  hImage          Fingerprint image handle.
**  @param[out] pSwipeSpeed     Pointer to store swipe speed.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsImageGetSwipeSpeed(
    vcsImageHandle_t    hImage,
    vcsUint32_t *       pSwipeSpeed);

/*!
*******************************************************************************
**  Return size of fingerprint data array, in bytes.
**
**  @param[in]  hImage      Fingerprint image handle.
**  @param[out] pDataSize   Pointer to store fingerprint data size.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsImageGetDataSize(
    vcsImageHandle_t    hImage,
    vcsUint32_t *       pDataSize);

/*!
*******************************************************************************
**  Return actual fingerprint data itself. Size should be obtained by
**  vcsImageGetDataSize() function.
**
**  @param[in]  hImage      Fingerprint image handle.
**  @param[out] pData       Pointer to store image data.
**  @param[in]  dataSize    data size.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsImageGetData(
    vcsImageHandle_t    hImage,
    vcsUint8_t *        pData,
    vcsUint32_t         dataSize);

/**< Handle to the IR metrics. */
typedef struct vcsIrMetrics_s *   vcsIrMetricsHandle_t;

/*!
*******************************************************************************
**  Return Ir metrics handle.
**
**  @param[in]  hImage          Fingerprint image handle.
**  @param[out] phIrMetrics     Pointer to store IR metrics handle.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsImageGetIrMetrics(
    vcsImageHandle_t        hImage,
    vcsIrMetricsHandle_t *  phIrMetrics);

/**< Handle to the IR metrics. */
typedef struct vcsAspMetrics_s *   vcsAspMetricsHandle_t;
/*!
*******************************************************************************
**  Return ASP metrics handle.
**
**  @param[in]  hImage          Fingerprint image handle.
**  @param[out] phAspMetrics    Pointer to store ASP metrics handle.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsImageGetAspMetrics(
    vcsImageHandle_t        hImage,
    vcsAspMetricsHandle_t * phAspMetrics);

/*!
*******************************************************************************
**  Flipping fingerprint image vertically.
**
**  @param[in/out]  hImage      Fingerprint image handle.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsImageFlipVertical(
    vcsImageHandle_t hImage);

/*!
*******************************************************************************
**  Flipping fingerprint image horizontally.
**
**  @param[in/out]  hImage      Fingerprint image handle.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsImageFlipHorizontal(
    vcsImageHandle_t hImage);

/*!
*******************************************************************************
**  This function convert fingerprint to the gray scaled image
**  (invert pixel values (black<->white)).
**
**  @param[in/out]  hImage      Fingerprint image handle.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsImageInvertGreyscale(
    vcsImageHandle_t hImage);

/*!
*******************************************************************************
**  Convert vcsImaget_t to ANSI-381 image format.
**
**  @param[in]      hImage          Fingerprint image handle.
**  @param[out]     pAnsi381Image   Pointer to buffer to store the ANSI-381
**                                  image. If VCS_NULL, pSize will be set to
**                                  the required size.
**  @param[in/out]  pSize           Size of buffer pointed by pAnsi381Image.
**                                  Actual size of the image is returned.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsImageToAnsi381(
    vcsImageHandle_t    hImage,
    vcsUint8_t *        pAnsi381Image,
    vcsUint32_t *       pSize);

/*!
*******************************************************************************
**  Convert vcsImaget_t to ANSI-381 image format with optional convert to big-endian.
**
**  @param[in]      hImage          Fingerprint image handle.
**  @param[out]     pAnsi381Image   Pointer to buffer to store the ANSI-381
**                                  image. If VCS_NULL, pSize will be set to
**                                  the required size.
**  @param[in/out]  pSize           Size of buffer pointed by pAnsi381Image.
**                                  Actual size of the image is returned.
**  @param[in] bConvertToBigEndian  Convert headers to big-endian if true.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsImageToAnsi381Ex(
    vcsImageHandle_t    hImage,
    vcsUint8_t *        pAnsi381Image,
    vcsUint32_t *       pSize,
    vcsBool_t           bConvertToBigEndian);

// added for synaFpStubTest
/*!
*******************************************************************************
**  Convert  ANSI-381 image format to vcsImaget_t with optional convert to big-endian.
**
**  @param[in]        pAnsi381Image   Pointer to ANSI-381 image. 
**  @param[in]        size                  Size of buffer pointed by pAnsi381Image.
**  @param[out]      hImage             Fingerprint image handle.
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API vcsImageAnsi381TohImage(
    vcsUint8_t*    pAnsi381Image, 
    vcsUint32_t   size, 
    vcsImageHandle_t* phImage);

/*!
*******************************************************************************
**  Return pointer to the bits of bitmap file created from the hImage.
**
**  @param[in]      hImage      Fingerprint image handle.
**  @param[out]     pBits       Pointer to buffer to retrieve bitmap file bits.
**                              Bitmap Bits have the following structure:
**                              BITMAPFILEHEADER +
**                              BITMAPINFOHEADER +
**                              RGBQUAD table +
**                              fingerprint image bits.
**  @param[in/out]  pSize       Size of buffer pointed by ppBits.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsImageToBitmapBits(
    vcsImageHandle_t    hImage,
    vcsUint8_t  *       pBits,
    vcsUint32_t *       pSize);

/*!
*******************************************************************************
**  Create image handle from provided data.
**
**  @param[out] phImage         Pointer to store fingerprint image handle.
**  @param[in]  pData           Pointer fingerprint data.
**  @param[in]  dataSize        Fingerprint data size.
**  @param[in]  rows            Number of rows in fingerprint image.
**  @param[in]  columns         Number of columns in fingerprint image.
**  @param[in]  quality         Fingerprint image quality.
**  @param[in]  xResolution     xResolution of the image.
**  @param[in]  yResolution     yResolution of the image.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsImageCreate(
    vcsImageHandle_t *  phImage,
    vcsUint8_t *        data,
    vcsUint32_t         dataSize,
    vcsUint32_t         rows,
    vcsUint32_t         columns,
    vcsImageQuality_t   quality,
    vcsInt32_t          xResolution,
    vcsInt32_t          yResolution);

/*!
*******************************************************************************
**  Destroy an image handle
**
**  @param[in]  hImage      Fingerprint data handle.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsImageDestroy(
    vcsImageHandle_t hImage);

/*!
*******************************************************************************
**  Clone given image.
**
**  @param[in]  hOriginalImage  Handle to original fingerprint data.
**  @param[out] phClonedImage   Pointer to store cloned fingerprint image handle.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsImageClone(
    vcsImageHandle_t   hOriginalImage,
    vcsImageHandle_t * phClonedImage);

/*!
*******************************************************************************
**  Pack provided fingerprint image handle to byte array.
**
**  @param[in]      hImage          Fingerprint image handle.
**  @param[in/out]  packedImage     Pointer to store packed fingerprint data.
**                                  Set packedImage.pData to VCS_NULL to get
**                                  packed image size in packedImage.length.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsImagePack(
    vcsImageHandle_t    hImage,
    vcsBlobData_t *     pPackedImage);

/*!
*******************************************************************************
**  Unpack provided blob to fingerprint image and return the handle.
**
**  @param[in]  packedImage     Pointer where packed fingerprint data is stored.
**  @param[out] phImage         Pointer to store fingerprint image handle.
**
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsImageUnPack(
    const vcsBlobData_t *   pPackedImage,
    vcsImageHandle_t *      phImage);


/*!
*******************************************************************************
**  Pack provided fingerprint image header to byte array.
**
**  @param[in]      hImage              Fingerprint image handle.
**  @param[in/out]  packedImageHeader   Pointer to store packed fingerprint data.
**                                  Set packedImageHeader.pData to VCS_NULL to get
**                                  packed image header size in packedImage.length.
**  @return VCS return code, VCS_RESULT_OK if no error.
*/
extern vcsResult_t VCS_API
vcsImageHeaderPack(
    vcsImageHandle_t    hImage,
    vcsBlobData_t *     pPackedImage);

/*!
*******************************************************************************
**  Unpacks provided blob to fingerprint image header set image data and return the handle.
**
**  @param[in]  pPackedImageHeader  Pointer where packed image data header is stored.
**  @param[in] pData                Image data pointer.
**  @param[out] phImage             Pointer to store fingerprint image handle.
**  @return VCS return code, VCS_RESULT_OK if no error.
**
*/
extern vcsResult_t VCS_API
vcsImageHeaderUnPack(
    const vcsBlobData_t *   pPackedImageHeader,
    vcsUint8_t*             pData,
    vcsUint32_t             dataSize,
    vcsImageHandle_t *      phImage);

/*!
*******************************************************************************
**  Retrieves max packed image size excluding image data.
*/
extern vcsUint32_t VCS_API
vcsImageHeaderMaxSize(void);

/*!
*******************************************************************************
**  Retrieves vcsImage_t struct size.
*/
extern vcsUint32_t VCS_API
vcsImageHeaderSize(void);

extern vcsResult_t VCS_API
vcsBitmapBitsToImage(
    const vcsConstBlobData_t * pBmp,
    vcsImageHandle_t *         phImage);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __vcsImage_h__ */
