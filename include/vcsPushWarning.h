/*! @file vcsPushWarning.h
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
**  USDK Disabled Compiler Warnings
**
**  This file disables compiler warnings.
**
**  
*/

#ifdef _MSC_VER

/******TODO START******/
/*Below warnings left in windows build. For now we are suppressing these warnings to make warnings count to 0 in windows builds and 
enabling /WX (Treat warning as error). These warnings needs to be fixed and below code needs to be removed */

/*falconConfig.h*/
/* warning C4132: 'flcn_patch******' : const object should be initialized*/
/* cfgBuilder tool modified to fix warnings in windsorPatchConfig.c. Once windsorPatchConfig.c generated this can be removed */
#pragma warning(disable:4132)

/*scsSecMgmt.c */
/* warning C4328: 'VFR_RESULT VFRGetMatchScore(VFR_HANDLE,VFR_MATCH_SCORE *)' : indirection alignment of 
formal parameter 2 (4) is greater than the actual argument alignment (1) */
/*vcs_m3_dp.c*/
/* warning C4328: 'vcsResult_t _scsSecMgmtGetSensorCertificate(scsSensorHandle_t,vcsUint32_t *,vcsUint32_t *,vcsUint8_t *,vcsUint8_t *,vcsfw_ecc_certificate_t *)' : 
indirection alignment of formal parameter 2 (4) is greater than the actual argument alignment (1) */
#pragma warning(disable:4328)

/*vcsmRidgeMatcher.c*/
/* warning C4255: '_get_purecall_handler' : no function prototype given: converting '()' to '(void)' */
#pragma warning(disable:4255)
 /* warning C4996: 'fopen': This function or variable may be unsafe. Consider using fopen_s instead. */
#pragma warning(disable:4996)

/*palUsbProtocol.c*/
/* warning C4826: Conversion from 'void *' to 'vcsUint64_t' is sign-extended. */
/*palProdTrace.c*/
/* warning C4826: Conversion from 'char *' to 'ULONG64' is sign-extended.*/
/*vfmVendorOp.c*/
/* warning C4826: Conversion from 'vcsUint8_t *__w64 ' to 'vcsUint64_t' is sign-extended. */
#pragma warning(disable:4826)

/* palCrypto.c   (pal/shared/windows) */
/* warning C6309: Argument '3' is null: this does not adhere to function specification of 'CryptSignHashA'*/
#pragma warning(disable:6309)
/* warning C6387: 'argument 3' might be '0': this does not adhere to the specification for the function 'CryptSignHashA' */
#pragma warning(disable:6387)

/*palFileIo.c (pal/shared/windows) */
/* warning C4054: 'type cast' : from function pointer 'FARPROC' to data pointer 'vcsHandle_t' */
/*palDevice.c*/
/* warning C4054: 'type cast' : from function pointer 'palDeviceEventHandler_t' to data pointer 'void *' */
/*scs.c*/
/* warning C4054: 'type cast' : from function pointer 'void (__cdecl *)(vcsHandle_t,void *)' to data pointer 'vcsHandle_t' */
#pragma warning(disable:4054)

/*palSynchronizationInternal.c (pal/shared/windows) */
/* warning C6258: Using TerminateThread does not allow proper thread clean up*/
#pragma warning(disable:6258)

/* vcsSnsrTestCommonDoc.c */
/* warning C6385: Invalid data: accessing 'pVcsSnsrTestNumDocFuncList', the readable size is '8' bytes, but '12' bytes might be read */
#pragma warning(disable:6385)
/******TODO END******/

/* warning C4514: '....' : unreferenced inline function has been removed */
#pragma warning(disable:4514)

#pragma warning(push)

/* warning C4201: nonstandard extension used : nameless struct/union */
#pragma warning(disable:4201)

/* warning C4214: nonstandard extension used : bit field types other than int */
#pragma warning(disable:4214)

/* warning C4115: '....' : named type definition in parentheses */
#pragma warning(disable:4115)

/* warning C4820: '....' : 'bytes' bytes padding added after construct 'member_name' */
#pragma warning(disable:4820)

/* warning C4255: '....' : 'function': no function prototype given: converting '()' to '(void)' */
#pragma warning(disable:4255)

/* warning C4668: '....' : 'symbol' is not defined as a preprocessor macro, replacing with '0' for 'directives' */
#pragma warning(disable:4668)

#endif /* _MSC_VER */

