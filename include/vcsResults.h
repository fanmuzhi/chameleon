/*! @file vcsResults.h
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
**  Result Code Definitions
** 
**
**  Result codes are grouped into following categories, where each category has
**  its own base result definition and fixed range:
**      1. Generic (100-199).
**      2. Sensor Specific (200-299).
**      3. Matcher Specific (300-399).
**      4. Image Processing Specific (400-499).
**      5. Database Specific (500-599).
**      6. System Specific (600-699).
**
**  Result codes range 1-99 will never used and dedicated for other layers
**  (like wrappers and customer specific modules).
**
*/

#ifndef __vcsResults_h__
#define __vcsResults_h__

#include "vcsTypes.h"

/*!
*******************************************************************************
**  Type definition for result
*/
typedef vcsUint32_t vcsResult_t;


/**< No error; Operation successfuly completed. */
#define VCS_RESULT_OK   ((vcsResult_t) 0)

/*!
*******************************************************************************
**  Error codes in range 1-99 are not used. This range is reserved for other
**  layers like wrappers, customer specific libraries, etc.
*/

/*!
*******************************************************************************
**  Error codes in 100-199 range are used to indicate generic error codes.
*/

/**< Base definition for all generic results. DO NOT USE AS AN ERROR CODE. */
#define VCS_RESULT_GEN_BASE                             ((vcsResult_t) 100)

/**< The underlying core module wasn't initialized. */
#define VCS_RESULT_GEN_CORE_NOT_INITIALIZED             ((vcsResult_t) 101)

/**< Other operation is in progress and current request cannot be accomplished. */
#define VCS_RESULT_GEN_BUSY                             ((vcsResult_t) 102)

/**< Operation was canceled by the caller. */
#define VCS_RESULT_GEN_OPERATION_CANCELED               ((vcsResult_t) 103)

/**< Operation denied. */
#define VCS_RESULT_GEN_OPERATION_DENIED                 ((vcsResult_t) 104)

/**< Specified finger is already enrolled. */
#define VCS_RESULT_GEN_ALREADY_ENROLLED                 ((vcsResult_t) 105)

/**< User authentication is required before current operation is performed. */
#define VCS_RESULT_GEN_AUTHENTICATION_REQUIRED          ((vcsResult_t) 106)

/**< Unable to find matching fingerprint template in the database. */
#define VCS_RESULT_GEN_NO_MATCH                         ((vcsResult_t) 107)

/**< No matching record for the specified user ID and template ID in the database. */
#define VCS_RESULT_GEN_FINGER_NOT_ENROLLED              ((vcsResult_t) 108)

/**< Signature verification failed.*/
#define VCS_RESULT_GEN_SIGNATURE_VERIFICATION_FAILED    ((vcsResult_t) 109)

/**< Place-holder until a real return code can be thought up. This code is for
internal usage only and should never returned from any function. */
#define VCS_RESULT_GEN_INVALID                          ((vcsResult_t) 110)

/**< One of the parameters specified is outside the range of acceptable values. */
#define VCS_RESULT_GEN_BAD_PARAM                        ((vcsResult_t) 111)

/**< Required input pointer was VCS_NULL. */
#define VCS_RESULT_GEN_NULL_POINTER                     ((vcsResult_t) 112)

/**< The function has not been implemented. */
#define VCS_RESULT_GEN_NOT_IMPLEMENTED                  ((vcsResult_t) 113)

/**< Format of the provided data is unexpected. */
#define VCS_RESULT_GEN_UNEXPECTED_FORMAT                ((vcsResult_t) 114)

/**< No more entries in list being scanned. */
#define VCS_RESULT_GEN_NO_MORE_ENTRIES                  ((vcsResult_t) 115)

/**< Buffer passed is too small. */
#define VCS_RESULT_GEN_BUFFER_TOO_SMALL                 ((vcsResult_t) 116)

/**< Wait time out. */
#define VCS_RESULT_GEN_TIMEOUT                          ((vcsResult_t) 117)

/**< Specified object doesn't exists. */
#define VCS_RESULT_GEN_OBJECT_DOESNT_EXIST              ((vcsResult_t) 118)

/**< Generic error code. */
#define VCS_RESULT_GEN_ERROR                            ((vcsResult_t) 119)

/*!
*******************************************************************************
**  Error codes in 200-299 range are used to indicate sensor specific error
**  codes.
*/

/**< Base definition for all sensor specific results. DO NOT USE AS AN ERROR
CODE. */
#define VCS_RESULT_SENSOR_BASE                          ((vcsResult_t) 200)

/**< Sensor is reset while operation is performed. */
#define VCS_RESULT_SENSOR_RESET                         ((vcsResult_t) 201)

/**< Sensor is malfunctioned while operation is performed. */
#define VCS_RESULT_SENSOR_MALFUNCTIONED                 ((vcsResult_t) 202)

/**< The sensor is removed or was not physically present or disabled. */
#define VCS_RESULT_SENSOR_UNAVAILABLE                   ((vcsResult_t) 203)

/**< Error occurred during secure sensor initialization and sensor is not
completely initialized. As a result most of operations can not be performed with
this sensor. To recover from this situation re-set ownership operation should be
performed. */
#define VCS_RESULT_SENSOR_NEED_TO_RESET_OWNER           ((vcsResult_t) 204)

/**< Operation can not be performed because the sensor is currently in
autonomous mode. Need to bring out the sensor from this mode to perform the
operation. */
#define VCS_RESULT_SENSOR_IN_AUTONOMOUS_MODE            ((vcsResult_t) 205)

/**< Operation can't be performed because the sensor is not initialized yet. */
#define VCS_RESULT_SENSOR_NOT_READY_FOR_USE             ((vcsResult_t) 206)

/**< The number of re-set ownership operations is exceed the limits. This is
applicable for flash-less secure sensors only and should be use as a warning
that from this point sensor will remain in secure owned state but sensor
specific keys will never re-generated. */
#define VCS_RESULT_SENSOR_OUT_OF_OTP_OWNERSHIP          ((vcsResult_t) 207)

/**< Failed to program the FW extension. */
#define VCS_RESULT_SENSOR_FW_EXT_PROGRAM_FAILED         ((vcsResult_t) 208)

/**< The command was not recognized or is not implemented . */
#define VCS_RESULT_SENSOR_BAD_CMD                       ((vcsResult_t) 209)

/**< Failed to open sensor driver. */
#define VCS_RESULT_SENSOR_DRV_OPEN_FAILED               ((vcsResult_t) 210)

/**< Unexpected error occurred during TLS session establishment. This error
should be reviewed and probably removed. VCS_RESULT_SENSOR_MALFUNCTIONED or
VCS_RESULT_SENSOR_NEED_TO_RESET_OWNER error should be used instead. */
#define VCS_RESULT_SENSOR_TLS_INTERNAL_FAILURE          ((vcsResult_t) 211)

/**< Sensor partition is not opened. */
#define VCS_RESULT_SENSOR_PARTITION_NOT_OPENED          ((vcsResult_t) 212)

/**< Sensor partition is full. */
#define VCS_RESULT_SENSOR_PARTITION_IS_FULL             ((vcsResult_t) 213)

/**<  Sensor Calibration Fail */
#define VCS_RESULT_SENSOR_CALIBRATION_FAIL              ((vcsResult_t) 214)


#define VCS_RESULT_SENSOR_CALIBRATION_RETRY             ((vcsResult_t) 215)

#define VCS_RESULT_SENSOR_INVALID_BASELINE              ((vcsResult_t) 216)
#define VCS_RESULT_SENSOR_CAPTURE_RESET                 ((vcsResult_t) 217)


#define VCS_RESULT_SENSOR_FRAME_NOT_READY               ((vcsResult_t) 218)
#define VCS_RESULT_SENSOR_FINGER_REMOVED                ((vcsResult_t) 219)


/*!
*******************************************************************************
**  Error codes in 300-399 range are used to indicate matcher specific error
**  codes.
*/

/**< Base definition for all matcher specific errors. DO NOT USE AS AN ERROR
CODE. */
#define VCS_RESULT_MATCHER_BASE                         ((vcsResult_t) 300)

/**< Failed to open matcher. */
#define VCS_RESULT_MATCHER_OPEN_FAILED                  ((vcsResult_t) 301)

/**< Failed to match provided image with provided template(s). */
#define VCS_RESULT_MATCHER_MATCH_FAILED                 ((vcsResult_t) 302)

/**< Failed to extract features from the template. */
#define VCS_RESULT_MATCHER_EXTRACT_FAILED               ((vcsResult_t) 303)

/**< Enrollment operation failed. */
#define VCS_RESULT_MATCHER_ENROLL_FAILED                ((vcsResult_t) 304)

/**< Failed to add provided image to the new creating template. */
#define VCS_RESULT_MATCHER_ADD_IMAGE_FAILED             ((vcsResult_t) 305)

/**< Failed to add provided image to the new creating template as fp area is lessthan 65% */
#define VCS_RESULT_MATCHER_FP_AREA_IMAGE_DISCARDED      ((vcsResult_t) 306)

/**< Failed to set the parameter. */
#define VCS_RESULT_MATCHER_SET_PARAMETER_FAILED         ((vcsResult_t) 307)

#define VCS_RESULT_MATCHER_CLOSE_FAILED                 ((vcsResult_t) 308)

/**< Failed to set the parameter. */
#define VCS_RESULT_MATCHER_GET_PARAMETER_FAILED         ((vcsResult_t) 309)

/*!
*******************************************************************************
**  Error codes in 400-499 range are used to indicate image processing
**  specific error codes.
*/

/**< Base definition for all image processing specific errors. DO NOT USE AS AN
ERROR CODE. */
#define VCS_RESULT_IMGPROC_BASE                         ((vcsResult_t) 400)

/**< Image processing is completed. This is a status which is returned by
image processing module to indicate that processing is complete. This error
should be reviewed and probably removed. */
#define VCS_RESULT_IMGPROC_COMPLETE                     ((vcsResult_t) 401)

/**< Image frame selection is in progress. This is a status which is returned by
IFS module to indicate that frame selection is in progress. */
#define VCS_RESULT_IMGPROC_IN_PROGRESS                  ((vcsResult_t) 402)

/**< Image frame selection is completed. This is a status which is returned by 
IFS module to indicate that frame selection was done. */
#define VCS_RESULT_IMGPROC_FINGER_SETTLED               ((vcsResult_t) 403)

/**< Image frame selection is failed. This is a status which is returned by 
IFS module to indicate that frame wasn't selected. */
#define VCS_RESULT_IMGPROC_NO_IMAGE_CAPTURED            ((vcsResult_t) 404)

#define VCS_RESULT_IMGPROC_FINGER_REMOVED               ((vcsResult_t) 405)

#define VCS_RESULT_IMGPROC_FINGER_ON_SENSOR             ((vcsResult_t) 406)

#define VCS_RESULT_IMGPROC_FINGER_DETECTED              ((vcsResult_t) 407)

#define VCS_RESULT_IMGPROC_ACQUISITION_NO_IMAGE         ((vcsResult_t) 408)

/*!
*******************************************************************************
**  Error codes in 500-599 range are used to indicate database specific error
**  codes.
*/

/**< Base definition for all database specific errors. DO NOT USE AS AN ERROR
CODE. */
#define VCS_RESULT_DB_BASE                              ((vcsResult_t) 500)

/**< Database is full. */
#define VCS_RESULT_DB_FULL                              ((vcsResult_t) 501)

/**< Database is empty. */
#define VCS_RESULT_DB_EMPTY                             ((vcsResult_t) 502)


/*!
*******************************************************************************
**  Error codes in 600-699 range are used to indicate system specific error
**  codes.
*/

/**< Base definition for all system specific errors. DO NOT USE AS AN ERROR
CODE. */
#define VCS_RESULT_SYS_BASE                             ((vcsResult_t) 600)

/**< System call failed. */
#define VCS_RESULT_SYS_CALL_FAILED                      ((vcsResult_t) 601)

/**< Out of heap memory. */
#define VCS_RESULT_SYS_OUT_OF_MEMORY                    ((vcsResult_t) 602)

/**< Failed to open file. */
#define VCS_RESULT_SYS_FILE_OPEN_FAILED                 ((vcsResult_t) 603)

/**< Failed to write to the file. */
#define VCS_RESULT_SYS_FILE_WRITE_FAILED                ((vcsResult_t) 604)

/**< Failed to read from the file. */
#define VCS_RESULT_SYS_FILE_READ_FAILED                 ((vcsResult_t) 605)


/*!
*******************************************************************************
**  Error codes in 700-799 range is dedicated for transport protocol result codes.
*/

/**< Base definition for all system specific errors. DO NOT USE AS AN ERROR
CODE. */
#define VCS_RESULT_VRC_BASE                             ((vcsResult_t) 700)

/**< Specified operation ID is invalid. */
#define VCS_RESULT_VRC_INVALID_OP_ID                    ((vcsResult_t) 701)

/**< Provided data size is greater than data transition size limit in transport module. */
#define VCS_RESULT_VRC_BUFER_TO_BIG                     ((vcsResult_t) 702)

/**< Provided data parameter is invalid. */
#define VCS_RESULT_VRC_INVALID_PARAM                     ((vcsResult_t) 703)


/**< Macro to determine if result code indicates success. */
#define VCS_SUCCESS(_status_) ((_status_) == VCS_RESULT_OK)

/**< Macro to determine if result code indicates failure. */
#define VCS_FAILURE(_status_) ((_status_) != VCS_RESULT_OK)

#endif /* __vcsResults_h__ */
