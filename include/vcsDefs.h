/*! @file vcsDefs.h
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
**  Common Definitions Between different layers.
**
**
*/
#ifndef __vcsDefs_h__
#define __vcsDefs_h__

#include "vcsTypes.h"
#include "vcsPushPack1.h"

#define VCS_PARAM_VALIDATE(_param_, _message_)  \
if (VCS_ISNULL(_param_))                        \
{                                               \
    DBG_WARNING(DBG_CTX, (_message_));          \
    result = VCS_RESULT_GEN_BAD_PARAM;          \
    goto cleanup;                               \
}                                               \

/* Capture Purpose Definitions. */
#define VCS_CAPTURE_PURPOSE_VERIFY      ((vcsUint32_t) 0)
#define VCS_CAPTURE_PURPOSE_ENROLL      ((vcsUint32_t) 1)

typedef vcsUint8_t  vcsCommIface_t;
#define VCS_COMM_IFACE_UNKNOWN          ((vcsCommIface_t) 0)
#define VCS_COMM_IFACE_USB              ((vcsCommIface_t) 1)
#define VCS_COMM_IFACE_SPI              ((vcsCommIface_t) 2)

/* Device info capabilities bit flags definitions */
#define VCS_CAP_UNKNOWN                 ((vcsUint32_t) 0x00000000)
#define VCS_CAP_PRODUCT_TYPE_TOUCH      ((vcsUint32_t) 0x00000001)
#define VCS_CAP_DEVICE_OWNED            ((vcsUint32_t) 0x00000002)
#define VCS_CAP_DEVICE_PROVISIONED      ((vcsUint32_t) 0x00000004)
#define VCS_CAP_DEVICE_HAS_FLASH        ((vcsUint32_t) 0x00000008)
#define VCS_CAP_DEVICE_EVENT_STREAMING  ((vcsUint32_t) 0x00000010)
#define VCS_CAP_DEVICE_FINGER_DETECT    ((vcsUint32_t) 0x00000020)
#define VCS_CAP_DEVICE_HAS_DRDY         ((vcsUint32_t) 0x00000040)
#define VCS_CAP_DEVICE_FINGER_DETECT_HBM ((vcsUint32_t) 0x00000080)

/* Silicon Version */
#define VCS_SILICON_VERSION_UNKNOWN         ((vcsUint32_t) -1)
#define VCS_SILICON_VERSION_METALLICA_A3    ((vcsUint32_t) 3)
#define VCS_SILICON_VERSION_METALLICA_A4    ((vcsUint32_t) 4)
#define VCS_SILICON_VERSION_METALLICA_B1    ((vcsUint32_t) 5)
#define VCS_SILICON_VERSION_METALLICA_B2    ((vcsUint32_t) 6)
#define VCS_SILICON_VERSION_METALLICA_B3    ((vcsUint32_t) 7)
#define VCS_SILICON_VERSION_METALLICA_C1    ((vcsUint32_t) 8)
#define VCS_SILICON_VERSION_METALLICA_C2    ((vcsUint32_t) 9)

#define VCS_SILICON_VERSION_VIPER_A0        ((vcsUint32_t) 0)
#define VCS_SILICON_VERSION_VIPER_A1        ((vcsUint32_t) 1)
#define VCS_SILICON_VERSION_VIPER_B0        ((vcsUint32_t) 2)

#define VCS_SILICON_VERSION_VIPER_2        ((vcsUint32_t) 10)


typedef struct VCS_PACKED vcsFwVersion_s
{
    vcsUint32_t   buildTime;        /* Unix-style build time, in seconds from
                                    1/1/1970 12:00 AM GMT */
    vcsUint32_t   buildNum;         /* build Number */
    vcsUint8_t    vMajor;           /* major version */
    vcsUint8_t    vMinor;           /* minor version */
    vcsUint8_t    target;           /* target, e.g. VCSFW_TARGET_ASIC      */
    vcsUint8_t    product;          /* product, e.g.  VCSFW_PRODUCT_FALCON */
    vcsUint8_t    patch;            /* patch level */
    vcsUint8_t    filler[3];        /* filler for alignment and future */
    vcsUint8_t    serial_number[6]; /* 48-bit Serial Number. (Falcon only) */
} vcsFwVersion_t;

typedef struct vcsFwExtVersion_s
{
    vcsUint16_t vMajor;     /* Major version of the FW extension */
    vcsUint16_t vMinor;     /* Minor version of the FW extension */
    vcsUint32_t buildTime;  /* Unix-style build time, in seconds from
                            1/1/1970 12:00 AM GMT */
} vcsFwExtVersion_t;

typedef struct VCS_PACKED vcsDeviceInfo_s
{
    vcsCommIface_t      commIface;
    vcsFwVersion_t      fwVersion;
    vcsFwExtVersion_t   fwExtVersion;
    vcsBool32_t         isProduction;
    vcsUint32_t         flexId;
    vcsUint32_t         projId;
    vcsUint32_t         siliconVersion; /* One of VCS_SILICON_VERSION_XXX */
    vcsUint16_t         pid;
    vcsUint16_t         vid;
    vcsUint32_t         capabilities; /* VCS_CAP_XXX bit flags. */
    vcsUint32_t         reserved;
} vcsDeviceInfo_t;

/* Version information structure */
typedef struct vcsVersion_s
{
    vcsUint32_t majorNum;            /** VCS product version major number.*/
    vcsUint32_t minorNum;            /** VCS product version minor number.*/
    vcsUint32_t subMinorNum;         /** VCS product version sub minor number.*/
    vcsUint32_t buildNum;            /** VCS product version build number.*/
} vcsVersion_t;

#include "vcsPopPack.h"

#endif /* __vcsDefs_h__ */
