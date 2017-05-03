/*! @file vcsEccCurveParam.h
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
**  Define ECC curve param data structure.
**
** 
*/

#ifndef __vcsEccCurveParam_h__
#define __vcsEccCurveParam_h__

#include "vcsTypes.h"

/**************ECC Related Definitions and Types*****************/
/* 
*  The length of the buffer holding 
*  521-bit long integer, increased to
*  be multiple to 32 bit. 
*/
#define VCS_ECC_MAX_BUF_LEN 68 

/* 
*  Elliptic curve parameters.
*  EC formula on Prime field of order n: y^2 mod p = x^3 + ax + b mod p 
*  NOTE: Parameters are in little endian byte order. 
*/

typedef struct vcsEccCurveParams_s
{
    vcsUint32_t ianaId;                     /* Curve IANA (or custom) ID            */
    vcsUint32_t strength;                   /* Strength in bits: 256, 348 or 521    */
    vcsUint32_t h;                          /* Cofactor                             */
    vcsUint8_t  a[VCS_ECC_MAX_BUF_LEN];     /* Coefficient 'a'                      */
    vcsUint8_t  b[VCS_ECC_MAX_BUF_LEN];     /* Coefficient 'b'                      */
    vcsUint8_t  gX[VCS_ECC_MAX_BUF_LEN];    /* X coordinate of Base Point           */
    vcsUint8_t  gY[VCS_ECC_MAX_BUF_LEN];    /* Y coordinate of Base Point           */
    vcsUint8_t  n[VCS_ECC_MAX_BUF_LEN];     /* Order                                */
    vcsUint8_t  p[VCS_ECC_MAX_BUF_LEN];     /* Prime 'P'                            */
} vcsEccCurveParams_t;

#endif /* __vcsEccCurveParam_h__ */
