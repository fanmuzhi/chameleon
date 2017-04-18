/*! @file vcsPalEx.h
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
**  USDK PAL Subsystem Definitions
**
**  This file contains the USDK PAL Subsystem definitions.
**
** 
*/

#ifndef __vcsPalEx_h__
#define __vcsPalEx_h__

#include "vcsPushWarning.h"
#include <stdio.h>
#include "vcsPopWarning.h"

#include "vcsTypes.h"
#include "vcsResults.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define VCS_INFINITE_TIMEOUT            0xFFFFFFFF
#define VCS_DEFAULT_TIMEOUT             250

#define palMax(__a,__b)                 (((__a) > (__b)) ? (__a) : (__b))
#define palMin(__a,__b)                 (((__a) < (__b)) ? (__a) : (__b))
#define palAbs(__a)                     (((__a) < 0) ? -(__a) : (__a))

/* Memory allocation functions */
#define palMalloc                       palRealMalloc
#define palFree                         palRealFree


typedef vcsHandle_t                     palEventHandle_t;
typedef vcsHandle_t                     palThreadHandle_t;
typedef vcsHandle_t                     palCriticalSectionHandle_t;

typedef vcsUint32_t                     (VCS_API * vcsThreadProc_t)(void * pCtx);

void *      VCS_API palRealMalloc(vcsUint32_t size);
void        VCS_API palRealFree(void * ptr);

void *      VCS_API palMemset(void *s, vcsInt32_t c, vcsUint32_t n);
void *      VCS_API palMemchr(const void *s, vcsInt32_t c, vcsUint32_t n);
void *      VCS_API palMemcpy(void *dest, const void *src, vcsUint32_t n);
vcsInt32_t  VCS_API palMemcmp(const void *s1, const void *s2, vcsUint32_t n);
vcsInt32_t  VCS_API palMemcmpNta(const void *s1, const void *s2, vcsUint32_t n);
vcsInt32_t  VCS_API palMemmove( void *pDest, vcsUint32_t nDestSize, const void *pSrc, vcsUint32_t n);

/* String handling functions */
vcsChar_t * VCS_API palStrchr(vcsConstString_t str, vcsChar_t c);
vcsChar_t * VCS_API palStrrchr(vcsConstString_t str, vcsChar_t c);
vcsChar_t * VCS_API palStrtok(vcsString_t str, const vcsChar_t *delim, vcsString_t *saveptr);

vcsUint32_t VCS_API palStrlen(vcsConstString_t str);
vcsString_t VCS_API palStrncpy(vcsString_t str1, vcsUint32_t size1, vcsConstString_t str2, vcsUint32_t n);
vcsString_t VCS_API palStrcpy(vcsString_t str1, vcsUint32_t size1, vcsConstString_t str2);
vcsString_t VCS_API palStrcat(vcsString_t str1, vcsUint32_t size1, vcsConstString_t str2);
vcsBool_t   VCS_API palToLower(vcsString_t string);
vcsBool32_t VCS_API palIsSpace(vcsChar_t symbol);

vcsString_t VCS_API palStrstr(vcsConstString_t str, vcsConstString_t search);
vcsBool_t   VCS_API palStringsMatch(vcsConstString_t str1, vcsConstString_t str2);
vcsBool_t   VCS_API palStringsIMatch(vcsConstString_t str1, vcsConstString_t str2);
vcsInt32_t  VCS_API palStrncmp(vcsConstString_t str1, vcsConstString_t str2, vcsUint32_t nCount);
vcsInt32_t  VCS_API palStrnicmp(vcsConstString_t str1, vcsConstString_t str2, vcsUint32_t nCount);
vcsInt32_t  VCS_API palStrcmp(vcsConstString_t str1, vcsConstString_t str2);
vcsInt32_t  VCS_API palStricmp(vcsConstString_t str1, vcsConstString_t str2);

vcsInt32_t  VCS_API palStrtol(vcsConstString_t nptr, vcsString_t *endptr, vcsInt32_t base);
vcsUint32_t VCS_API palStrtoul(vcsConstString_t nptr, vcsString_t *endptr, vcsInt32_t base);
void        VCS_API palStringToStringsArray(vcsString_t source, vcsString_t argv[], vcsUint32_t * argc);

vcsInt32_t  VCS_API palSprintf(vcsString_t str, vcsUint32_t size1, vcsConstString_t format, ...);
/* FIXME - sscanf needs to be implemented in the PAL.
** Since there is no vector version like vsprintf, this is just #defined for now.
*/
#if defined(VCS_OS_WINDOWS) || defined(VCS_OS_RTOS32)
#ifdef _CRT_DEPRECATE_TEXT
#define palSscanf _stscanf_s
#define palFscanf _ftscanf_s
#define palAsciiSscanf sscanf_s
#define palAsciiFscanf fscanf_s
#else /*_CRT_DEPRECATE_TEXT*/
#define palSscanf _stscanf
#define palFscanf _ftscanf
#define palAsciiSscanf sscanf
#define palAsciiFscanf fscanf
#endif /*_CRT_DEPRECATE_TEXT*/
#else /*VCS_OS_WINDOWS*/
#define palSscanf sscanf
#define palFscanf fscanf
#define palAsciiSscanf sscanf
#define palAsciiFscanf fscanf
#endif  /*VCS_OS_WINDOWS*/

vcsAsciiChar_t * VCS_API palAsciiStrchr(const vcsAsciiChar_t *str, vcsInt32_t c);
vcsUint32_t      VCS_API palAsciiStrlen(const vcsAsciiChar_t *str);
vcsAsciiChar_t * VCS_API palAsciiStrncpy(vcsAsciiChar_t *str1, vcsUint32_t size1, const vcsAsciiChar_t *str2, vcsUint32_t n);
vcsAsciiChar_t * VCS_API palAsciiStrcpy(vcsAsciiChar_t *str1, vcsUint32_t size1, const vcsAsciiChar_t *str2);
vcsAsciiChar_t * VCS_API palAsciiStrcat(vcsAsciiChar_t *str1, vcsUint32_t size1, const vcsAsciiChar_t *str2);

vcsAsciiChar_t * VCS_API palAsciiStrstr(const vcsAsciiChar_t *str, const vcsAsciiChar_t *search);
vcsBool_t        VCS_API palAsciiStringsMatch(const vcsAsciiChar_t *str1, const vcsAsciiChar_t *str2);
vcsBool_t        VCS_API palAsciiStringsIMatch(const vcsAsciiChar_t *str1, const vcsAsciiChar_t *str2);

vcsInt32_t       VCS_API palAsciiSprintf(vcsAsciiChar_t *str, vcsUint32_t size1, const vcsAsciiChar_t *format, ...);

vcsInt32_t       VCS_API palAsciiStrtol(const vcsAsciiChar_t * nptr, vcsAsciiChar_t **endptr, vcsInt32_t base);
vcsUint32_t      VCS_API palAsciiStrtoul(const vcsAsciiChar_t * nptr, vcsAsciiChar_t **endptr, vcsInt32_t base);
vcsAsciiChar_t * VCS_API palAsciiStrtok(vcsAsciiChar_t * str, const vcsAsciiChar_t *delim, vcsAsciiChar_t **saveptr);

vcsUint32_t VCS_API palAsciiToUnicodeString(vcsAsciiChar_t *pStr,vcsUnicodeChar_t *pOutBuffer,vcsUint32_t uSize);
vcsUint32_t VCS_API palUnicodeToAsciiString(vcsUnicodeChar_t *pStr,vcsAsciiChar_t *pOutBuffer,vcsUint32_t uSize);

/* File handling functions */
#define PAL_FILE                          FILE
#define PAL_STDOUT                        (PAL_FILE *) stdout
#define PAL_STDIN                         (PAL_FILE *) stdin
#define PAL_STDERR                        (PAL_FILE *) stderr

#if defined(VCS_OS_BCMUSH)
#undef PAL_FILE
#define PAL_FILE                          void
#endif

#if defined(VCS_OS_QSEE) || defined(VCS_OS_MOBICORE) || defined(VCS_OS_SGX)
#undef PAL_FILE
#define PAL_FILE                          int
#endif

#define VCS_PAL_FILE_SEEK_SET             0
#define VCS_PAL_FILE_SEEK_CUR             1
#define VCS_PAL_FILE_SEEK_END             2

#if defined(VCS_OS_LINUX) || defined(VCS_OS_MAC)
#undef palFscanf
vcsInt32_t palFscanf ( PAL_FILE *fp, vcsConstString_t format, ...);
#endif

vcsResult_t VCS_API palDirPathCreate(vcsConstString_t dirPath);

#if defined(VCS_OS_WINXP) 
void VCS_API InitializeACL(PSECURITY_DESCRIPTOR pSD, PACL pACL, vcsUint32_t autorityType);
#endif

PAL_FILE *  VCS_API palFileOpen(vcsConstString_t path, vcsConstString_t mode);
vcsInt32_t  VCS_API palFileClose(PAL_FILE *fp);
vcsUint32_t VCS_API palFileRead(void *ptr, vcsUint32_t size, vcsUint32_t nmemb, PAL_FILE *fp);
vcsUint32_t VCS_API palFileWrite(const void *ptr, vcsUint32_t size, vcsUint32_t nmemb, PAL_FILE *fp);
vcsUint32_t VCS_API palFileSeek(PAL_FILE *fp, vcsUint32_t offset, vcsInt32_t origin);
vcsChar_t * VCS_API palFileGets(vcsChar_t *str, vcsInt32_t size, PAL_FILE *fp);
vcsUint32_t VCS_API palFileGetc(PAL_FILE *fp);
vcsInt32_t  VCS_API palFileRemove(vcsConstString_t path);
vcsInt32_t  VCS_API palFileRename(vcsConstString_t oldName, vcsConstString_t newName);
vcsUint32_t VCS_API palFileTell(PAL_FILE *fp);
vcsInt32_t  VCS_API palFFlush(PAL_FILE *fp);
vcsInt32_t  VCS_API palFprintf(PAL_FILE *fp, vcsConstString_t format, ...);

vcsAsciiChar_t *    VCS_API palAsciiFileGets(vcsAsciiChar_t *str, vcsInt32_t size, PAL_FILE *fp);
vcsInt32_t          VCS_API palAsciiFprintf(PAL_FILE *fp, const vcsAsciiChar_t * format, ...);

vcsUint32_t VCS_API palGetch(void);
vcsUint32_t VCS_API palKbhit(void);

vcsUint32_t VCS_API palGetModuleFileName( vcsString_t filePath, vcsUint32_t size );
vcsUint32_t VCS_API palGetCurrentDirectory(vcsString_t path, vcsUint32_t size);

palEventHandle_t VCS_API palEventAlloc(vcsBool_t manualReset, vcsBool_t initialState, vcsString_t pName);
vcsResult_t VCS_API palEventFree(palEventHandle_t hEvent);
vcsResult_t VCS_API palEventSet(palEventHandle_t hEvent);
vcsResult_t VCS_API palEventReset(palEventHandle_t hEvent);
vcsResult_t VCS_API palEventWaitFor(palEventHandle_t hEvent, vcsUint32_t timeout);

vcsConstString_t VCS_API palEventName(palEventHandle_t hEvent);
palEventHandle_t VCS_API palEventAllocEx(vcsBool_t manualReset, vcsBool_t initialState, vcsString_t pName, vcsBool_t securAtributes);
palEventHandle_t VCS_API palEventOpen(vcsBool_t manualReset, vcsBool_t initialState, vcsString_t pName);

vcsResult_t VCS_API palWaitForSingleObject(vcsHandle_t hHandle, vcsUint32_t timeout);

vcsResult_t VCS_API palCriticalSectionInitialize(palCriticalSectionHandle_t * phCS);
vcsResult_t VCS_API palCriticalSectionFree(palCriticalSectionHandle_t   hCS);
vcsResult_t VCS_API palCriticalSectionAcquire(palCriticalSectionHandle_t    hCS);
vcsResult_t VCS_API palCriticalSectionRelease(palCriticalSectionHandle_t    hCS);
    
vcsUint32_t VCS_API palSupportedNumberOfLogicalThreads( void );

vcsResult_t VCS_API palThreadBegin(vcsUint32_t stackSize, vcsThreadProc_t threadProc, void * pContext, vcsUint32_t initialState, palThreadHandle_t * phThread, vcsUint32_t * pThreadId);
vcsResult_t VCS_API palThreadEnd(vcsUint32_t result);
vcsBool_t   VCS_API palIsCurrentThread(palThreadHandle_t threadHandle, vcsUint32_t threadId);
vcsResult_t VCS_API palThreadWaitFor(palThreadHandle_t hThread, vcsUint32_t timeout);
vcsResult_t VCS_API palThreadFree(palThreadHandle_t hThread);

#if VCS_FEATURE_PRF_MEMORY
void        VCS_API palPrfDumpMemoryStatistics(void);
#endif /* VCS_FEATURE_PRF_MEMORY */

#define cleanupMalloc(_ptr_)                                                  \
    if (!VCS_ISNULL(_ptr_))                                                   \
    {                                                                         \
        palFree(_ptr_);                                                       \
        _ptr_ = VCS_NULL;                                                     \
    }

#define cleanupEvent(_event_)                                                 \
    if (!VCS_ISNULL(_event_))                                                 \
    {                                                                         \
        palEventFree(_event_);                                                \
        _event_ = VCS_NULL;                                                   \
    }

#define cleanupCriticalSection(_cs_)                                          \
    if (!VCS_ISNULL(_cs_))                                                    \
    {                                                                         \
        palCriticalSectionFree(_cs_);                                         \
        _cs_ = VCS_NULL;                                                      \
    }

typedef struct
{
    vcsUint32_t     d_attr;
    vcsChar_t       d_name[VCS_MAX_PATH];
} PAL_DIR_ENTRY;

typedef struct DIR
{
    vcsHandle_t     fh;
    vcsHandle_t     fd;
    PAL_DIR_ENTRY   entry;
    vcsBool_t       has_next;
} PAL_DIR;

typedef void (VCS_API * PAL_DIR_CALLBACK)(void *pUserData, vcsUint32_t cnt, vcsConstString_t file_name);

PAL_DIR * VCS_API palDirOpen(vcsConstString_t name);
PAL_DIR_ENTRY * VCS_API palDirRead(PAL_DIR *dir);
vcsBool_t VCS_API  palDirClose(PAL_DIR *dir);
vcsUint32_t VCS_API palDirWalk(vcsConstString_t dir_name, vcsConstString_t file_ext, vcsUint32_t cnt, PAL_DIR_CALLBACK callback, void * pUserData);

/* system tick count function */
vcsUint32_t VCS_API palGetTickCount(void);
vcsUint32_t VCS_API palGetAccurateTickCount(void);
void        VCS_API palSleep(vcsUint32_t msTime);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __vcsPalEx_h__ */
