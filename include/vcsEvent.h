/*! @file vcsEvent.h
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
**  This header file contains definitions and APIs to work with vcs events.
**
*/

#ifndef __vcsEvent_h__
#define __vcsEvent_h__

#define DBG_VCS_EVENT_SYS   0x00000040L

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "vcsTypes.h"
#include "vcsResults.h"

/*!
*******************************************************************************
**  Handle to event information.
*/
typedef struct vcsEvent_s *   vcsEventHandle_t;

/*!
*******************************************************************************
**  Event ID type definition.
*/
typedef vcsUint32_t vcsEventId_t;


extern vcsResult_t VCS_API
vcsEventCreate(
    vcsEventHandle_t *  phEvent,
    vcsUint32_t         eventId,
    vcsUint32_t         status,
    vcsUint32_t         eventDataSize,
    void *              pEventData);

extern vcsResult_t VCS_API
vcsEventDestroy(
    vcsEventHandle_t hEvent);

/** Return event ID. 0 in case of error. */
extern vcsEventId_t VCS_API
vcsEventGetId(
    vcsEventHandle_t hEvent);

/** Return status of event. */
extern vcsUint32_t VCS_API
vcsEventGetStatus(
    vcsEventHandle_t hEvent);

/** Return event specific data. */
extern void * VCS_API
vcsEventGetData(
    vcsEventHandle_t hEvent);

/** Return event specific data size. */
extern vcsUint32_t VCS_API
vcsEventGetDataSize(
    vcsEventHandle_t hEvent);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __vcsEvent_h__ */
