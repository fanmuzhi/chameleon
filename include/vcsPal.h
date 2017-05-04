/*! @file vcsPal.h
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

#ifndef __vcsPal_h__
#define __vcsPal_h__

#include "vcsPalEx.h"
#include "vcsSecMgmt.h"
#include "vcsPalList.h"
#include "vcsPalProfiler.h"

#define DBG_PAL_SYS                             0x00000010L
#define DBG_PAL_SERVICE_SYS                     0x00000020L
#define DBG_PAL_SENSOR_SYS                      0x00000040L
#define DBG_PAL_MEMORY_SYS                      0x00000080L
#define DBG_PAL_MUTEX_SYS                       0x00000100L
#define DBG_PAL_EVENT_SYS                       0x00000200L
#define DBG_PAL_TIMER_SYS                       0x00000400L
#define DBG_PAL_WAIT_SYS                        0x00000800L
#define DBG_PAL_THREAD_SYS                      0x00001000L
#define DBG_PAL_PNP_SYS                         0x00002000L
#define DBG_PAL_DRIVERX_SYS                     0x00004000L
#define DBG_PAL_WORK_ITEM_SYS                   0x00008000L

#define DBG_PAL_MALLOC_TRACE_ON                 0x00010000L
#define DBG_PAL_SHARED_MEM_SYS                  0x00020000L
#define DBG_PAL_SEMAPHORE_SYS                   0x00040000L
#define DBG_PAL_CRITICAL_SEC_SYS                0x00080000L
#define DBG_PAL_SOCKET_SYS                      0x00100000L

#define VCS_USDK_ON_DEMAND

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* We need to undef palMalloc and palFree macros since they already defined in vcsPalEx.h.
   But here we don't need that definition */
#ifdef palMalloc
#undef palMalloc
#endif

#ifdef palFree
#undef palFree
#endif

/* Memory allocation functions */


#if VCS_FEATURE_PRF_MEMORY
#if !(VCS_FEATURE_ENABLE_MINIMAL_LOG)
    #define palMalloc(_size_)           palPrfMalloc(_size_, __FILE__, __LINE__, __FUNC__)
    #define palFree(_ptr_)              palPrfFree(_ptr_, __FILE__, __LINE__, __FUNC__)
#else /* !(VCS_FEATURE_ENABLE_MINIMAL_LOG) */
    #define palMalloc(_size_)           palPrfMalloc(_size_, __FILE__, __LINE__, 0)
    #define palFree(_ptr_)              palPrfFree(_ptr_, __FILE__, __LINE__, 0)
#endif /* !(VCS_FEATURE_ENABLE_MINIMAL_LOG) */
#else /* VCS_FEATURE_PRF_MEMORY */
#define palMalloc                       palRealMalloc
#define palFree                         palRealFree
#endif /* VCS_FEATURE_PRF_MEMORY */

/* Fingerprint buffer specific allocation/deallocation macros */
#if VCS_FEATURE_SOC
/* FP buffer specific mem allocation function : one large block mem repeated used only.
 * Used for both BCM target and Win simulation, imp in pal\bcmush\palMemory.c for both  */
void * VCS_API palMallocFP(vcsUint32_t size);
void   VCS_API palFreeFP(void * pMem);
#define PAL_FP_BUF_MALLOC(_size_)  (vcsUint8_t*)palMallocFP(_size_)
#define PAL_FP_BUF_FREE(_ptr_)     palFreeFP(_ptr_)
#else /* VCS_FEATURE_SOC */
#define PAL_FP_BUF_MALLOC(_size_)  (vcsUint8_t*)palMalloc(_size_)
#define PAL_FP_BUF_FREE(_ptr_)     palFree(_ptr_)
#endif /* VCS_FEATURE_SOC */

#define PAL_FP_BUF_CLEANUP(_ptr_)  \
    if (!VCS_ISNULL(_ptr_))        \
    {                              \
        PAL_FP_BUF_FREE(_ptr_);    \
        _ptr_ = VCS_NULL;          \
    }

#define cleanupSemaphore(_sem_)    \
    if (!VCS_ISNULL(_sem_))        \
    {                              \
        palSemaphoreFree(_sem_);   \
        _sem_ = VCS_NULL;          \
    }

#define cleanupMutex(_mutex_)      \
    if (!VCS_ISNULL(_mutex_))      \
    {                              \
        palMutexFree(_mutex_);     \
        _mutex_ = VCS_NULL;        \
    }

/* byte order conversion functions (Sensor data is all little endian) */
#define             palHostToSensor8(x)             (x)
vcsUint16_t VCS_API palHostToSensor16(vcsUint16_t _u16);
vcsUint32_t VCS_API palHostToSensor32(vcsUint32_t _u32);
#define             palSensorToHost8(x)             (x)
vcsUint16_t VCS_API palSensorToHost16(vcsUint16_t _u16);
vcsUint32_t VCS_API palSensorToHost32(vcsUint32_t _u32);

typedef struct palSensor_s *        palSensorHandle_t;
typedef struct palSubsystem_s *     palSubsystemHandle_t;
typedef vcsHandle_t                 palMutexHandle_t;
typedef vcsUint32_t                 vcsTimerType_t;

typedef vcsHandle_t                 palSharedMemoryHandle_t;
typedef vcsHandle_t                 palSemaphoreHandle_t;

#if defined(linux) || defined(VCS_OS_LINUX) || defined(VCS_OS_MAC)
/*
 * When a pipe is opened for READ or WRITE, it will block the thread.
 * Therefore in palPipeCreate(), a pipe will be created but not opened.
 * The pipe will be opened in palPipeRead() and palPipeWrite().
 */
typedef struct  vcsPosixPipe_s
{
    vcsUint32_t     nMode;          /* O_RDONLY or O_WRONLY */
    vcsString_t     pstrName;       /* name of the pipe     */
    vcsInt32_t      nFD;            /* file descriptor of the opened pipe */
} vcsPosixPipe_t;

typedef vcsPosixPipe_t *            palPipeHandle_t;
#else /* linux || VCS_OS_LINUX || VCS_OS_MAC */
typedef vcsHandle_t                 palPipeHandle_t;
#endif /* linux || VCS_OS_LINUX || VCS_OS_MAC */


typedef void (VCS_API * vcsTimerCallback_t)(unsigned int timerId,
                                            unsigned int msg,
                                            vcsUint32_t  pContext,
                                            vcsUint32_t  arg1,
                                            vcsUint32_t  arg2);

#define VCS_SERVICE_EVENT_TIMEOUT           1
#define VCS_MUTEX_TIMEOUT                   5000 /* more than 5 seconds?  something is broken */
#define VCS_THREAD_SHUTDOWN_TIMEOUT         (5 * VCS_MUTEX_TIMEOUT)
#define VCS_SERVICE_QUANTUM                 500
#define VCS_SYNC_CALLBACK_EVENT_TIMEOUT     2

#include "vcsPalList.h"

vcsResult_t VCS_API palInit(void);
vcsResult_t VCS_API palUnInit(void);
void VCS_API palDbgInit(void);

vcsResult_t VCS_API palSensorOpen(vcsString_t          devName,
                                  palSensorHandle_t *  phDevice);
vcsResult_t VCS_API palSensorClose(palSensorHandle_t    hDevice);
vcsResult_t VCS_API palSensorStartup(palSensorHandle_t  pSensor);
vcsResult_t VCS_API palSensorShutdown(palSensorHandle_t  pSensor);
vcsResult_t VCS_API palSensorRemount(palSensorHandle_t  pSensor);

vcsResult_t VCS_API palSensorGetNextCmdSequenceNumber(palSensorHandle_t hDevice,
                                                      vcsUint16_t *     cmdSequenceNumber);

// Send a VCSDRV_IOCTL and datablob to the device over the device's comm channel and retrieve the replyblob
vcsResult_t VCS_API palSensorSend(palSensorHandle_t hSensor,
                                  vcsUint32_t       ioControlCode,
                                  void *            pInBuffer,
                                  vcsUint32_t       inBufferSize,
                                  void *            pOutBuffer,
                                  vcsUint32_t       outBufferSize,
                                  vcsUint32_t *     pBytesReturned,
                                  void *            pOverlapped);

// Send a CmdData? structured command blob to the device and retieve the response blob
vcsResult_t VCS_API palSensorSendCmdData(palSensorHandle_t hSensor,
                                         vcsBlobData_t *   pCmdData,
                                         vcsBlobData_t *   pReplyData);

// Capture and return a chunk of finger image data from the sensor.
vcsResult_t VCS_API palSensorReadFpData(palSensorHandle_t   hSensor,
                                        vcsUint8_t *        pFpBuffer,
                                        vcsUint32_t         fpBufferSize,
                                        vcsUint32_t         readChunkSize,
                                        vcsUint32_t *       pReadDataSize,
                                        vcsBool_t *         pbReadComplete);
// Clear image data from bufferss
vcsResult_t VCS_API palSensorFlushFpData(palSensorHandle_t hSensor);
// Reset the sensor
vcsResult_t VCS_API palSensorReset(palSensorHandle_t hPalSensor);
// Put the sensor into the ready state
vcsResult_t VCS_API palSensorEnable(palSensorHandle_t hPalSensor, vcsBool_t bEnable, vcsBool_t bOffOn);
// Put the sensor into the low power state
vcsResult_t VCS_API palSensorSuspend(palSensorHandle_t hPalSensor, vcsBool_t bOffOn);
// Resume active operations 
vcsResult_t VCS_API palSensorResume(palSensorHandle_t hPalSensor);

#define PAL_SENSOR_PIPE_REQUEST         0
#define PAL_SENSOR_PIPE_REPLY           1
#define PAL_SENSOR_PIPE_FINGERPRINT     2
#define PAL_SENSOR_PIPE_INTERRUPT       3
#define PAL_SENSOR_PIPE_NAV_INTERRUPT   4

vcsResult_t VCS_API palSensorGetPipeTimeouts(palSensorHandle_t hDevice,
                                             vcsUint32_t *     pReplyTimeout,
                                             vcsUint32_t *     pRequestTimeout);

vcsResult_t VCS_API palSensorSetPipeTimeout(palSensorHandle_t hDevice,
                                            vcsUint32_t       pipeId,
                                            vcsUint32_t       nTimeout);
vcsResult_t VCS_API palSensorGetPipeTimeout(palSensorHandle_t hPalSensor,
                                            vcsUint32_t       pipeId,
                                            vcsUint32_t *     pTimeout);

vcsResult_t VCS_API palSensorEventListenStart(palSensorHandle_t hPalSensor,
                                              vcsUint32_t       eventSource,
                                              vcsHandle_t       callback,
                                              void *            pUserData);
vcsResult_t VCS_API palSensorEventListenCancel(palSensorHandle_t hPalSensor,
                                               vcsUint32_t       eventSource);

#if VCS_FEATURE_PRF_MEMORY
extern void * VCS_API
palPrfMalloc(
    vcsUint32_t  size,
    const char * fileName,
    vcsUint32_t  lineNumber,
    const char * function);

extern void VCS_API
palPrfFree(
    void *       ptr,
    const char * fileName,
    vcsUint32_t  lineNumber,
    const char * function);

#if VCS_FEATURE_PRF_MEMORY_GUARDING
extern void VCS_API 
palPrfMemoryValidate(void);
#endif /* VCS_FEATURE_PRF_MEMORY_GUARDING*/

#endif /*VCS_FEATURE_PRF_MEMORY */



void        VCS_API palDbgPrintSensor(palSensorHandle_t hSubsystem);


palMutexHandle_t VCS_API palMutexAlloc(vcsBool_t   initialOwner,
                                       vcsString_t pName);
vcsResult_t VCS_API palMutexFree(palMutexHandle_t hMutex);
vcsResult_t VCS_API palMutexRelease(palMutexHandle_t hMutex);
vcsResult_t VCS_API palMutexAcquire(palMutexHandle_t hMutex,
                                    vcsUint32_t      timeout);

#define SERVICE_MUTEX_ACQUISITION_TIMEOUT     60000  /* FIXME bjf - what's a reasonable timeout? */

#define acquireCriticalSectionOrCleanup(_cs_)                                   \
{                                                                               \
    vcsResult_t _result_;                                                       \
    _result_ = palCriticalSectionAcquire(_cs_);                                 \
    if (VCS_FAILURE(_result_))                                                  \
    {                                                                           \
        if(VCS_SUCCESS(result))                                                 \
        {                                                                       \
            result = _result_;                                                  \
        }                                                                       \
        DBG_WARNING(DBG_CTX, ("Failed to acquire the critical section.\n"));    \
        goto cleanup;                                                           \
    }                                                                           \
}

#define PAL_MALLOC_VALIDATE(_param_)                              \
if (VCS_ISNULL(_param_))                                          \
{                                                                 \
    if(VCS_SUCCESS(result))                                       \
    {                                                             \
        result = VCS_RESULT_SYS_OUT_OF_MEMORY;                    \
    }                                                             \
    DBG_ERROR(DBG_CTX, ("Memory allocation failed.\n"));          \
    goto cleanup;                                                 \
}

#define VCS_PAD(_num_, _base_)  ((_num_) + (_base_) - ((_num_) % (_base_)))

/* Math functions
*/
vcsInt32_t VCS_API palSqrt(vcsInt32_t x);
/* result = a * b / c */
vcsInt32_t VCS_API palMulDiv(vcsInt32_t number,
                             vcsInt32_t numerator,
                             vcsInt32_t denominator);
vcsInt32_t VCS_API RoundNearest(double d);

typedef vcsInt32_t (* palComparator_t)(
    const void * obj1,
    const void * obj2);

extern void palSort(
    void *          base,
    vcsUint32_t     num,
    vcsUint32_t     size,
    palComparator_t compar);

/* Date/Time
*/
vcsInt32_t  VCS_API palGetDateTime(void);
vcsResult_t VCS_API palGetDateTimeString(vcsString_t buf,
                                         vcsUint32_t size,
                                         vcsBool_t   bTimeOnly);

/* Return time as number of seconds elapsed since Jan 1, 1970 00:00:00 UTC */
vcsUint32_t VCS_API palGetSecondsSinceEpoch(void);

vcsBool_t VCS_API palIsBadWritePtr(vcsHandle_t pHandle,
                                   vcsUint32_t size);
vcsBool_t VCS_API palIsBadReadPtr(vcsHandle_t pHandle,
                                  vcsUint32_t size);

#if VCS_FEATURE_NAVIGATION_USDK5

#include "vcsNavClient.h"

typedef vcsInt32_t (*navProcessEvent_t) ( vcsNavEvent_t *pEvent);   /* Plugin - nav event handling function */

/*
 *
 */
typedef struct vcsNavInfo_s
{
    vcsUint32_t nSize;
    vcsUint32_t nMask;      /* bit-0 : toggle Y axis.   */
#if  defined(VCS_OS_LINUX) || defined(VCS_OS_ANDROID)
    vcsUint32_t nAppID;     /* client process ID that request nav events - only 1 client for now */
    vcsInt32_t  fdUInput;   /* uinput file descriptor */
    void        *pLib;      /* nav plugin module (libvalNavDev.so)    */
    navProcessEvent_t   ProcessEvent;
#endif
} vcsNavInfo_t;

/* send keyboard/mouse event to OS
 */

vcsResult_t VCS_API palSendInput( struct vcsNavReport_s *pNavReport, vcsNavInfo_t *pNavInfo);
#endif /* VCS_FEATURE_NAVIGATION_USDK5 */

#if defined(VCS_OS_LINUX) || defined(VCS_OS_ANDROID)


#ifndef VCS_KEY_EVENT_FINGER_DETECTED
#define VCS_KEY_EVENT_FINGER_DETECTED 0x230
#endif /* VCS_KEY_EVENT_FINGER_DETECTED */

#ifndef VCS_KEY_EVENT_FINGER_REMOVED
#define VCS_KEY_EVENT_FINGER_REMOVED  0x231
#endif /* VCS_KEY_EVENT_FINGER_REMOVED */

#ifndef VCS_KEY_EVENT_FINGER_RESTED
#define VCS_KEY_EVENT_FINGER_RESTED   0x232
#endif /* VCS_KEY_EVENT_FINGER_RESTED */

#ifndef VCS_KEY_EVENT_FINGER_LONGPRESS
#define VCS_KEY_EVENT_FINGER_LONGPRESS 0x233
#endif /* VCS_KEY_EVENT_FINGER_LONGPRESS  */

#ifndef VCS_KEY_EVENT_DPAD_RIGHT
#define VCS_KEY_EVENT_DPAD_RIGHT      0x234
#endif /* VCS_KEY_EVENT_DPAD_RIGHT */

#ifndef VCS_KEY_EVENT_DPAD_LEFT
#define VCS_KEY_EVENT_DPAD_LEFT       0x235
#endif /* VCS_KEY_EVENT_DPAD_LEFT */

#ifndef VCS_KEY_EVENT_DPAD_DOWN
#define VCS_KEY_EVENT_DPAD_DOWN       0x236
#endif /* VCS_KEY_EVENT_DPAD_DOWN  */

#ifndef VCS_KEY_EVENT_DPAD_UP
#define VCS_KEY_EVENT_DPAD_UP         0x237
#endif /* VCS_KEY_EVENT_DPAD_UP */

#ifndef VCS_KEY_EVENT_CLICK
#define VCS_KEY_EVENT_CLICK           0x238
#endif /* VCS_KEY_EVENT_CLICK */

#ifndef VCS_KEY_EVENT_FINGER_DTAP
#define VCS_KEY_EVENT_FINGER_DTAP     0x239
#endif /* VCS_KEY_EVENT_FINGER_DTAP */

vcsResult_t VCS_API palNavCreate( vcsInt32_t *pnFD);
vcsResult_t VCS_API palNavDestroy( vcsInt32_t uinp_fd);
vcsInt32_t VCS_API navDoClick( vcsInt32_t fdUInput, vcsUint32_t nKey );
#endif /* VCS_OS_LINUX */



/* Crypto operations API */

#define AES_KEY_SIZE_256    32
#define AES_KEY_SIZE_192    24
#define AES_KEY_SIZE_128    16
#define AES_BLOCK_SIZE      16
#define AES_INIT_VECTOR_SIZE    AES_BLOCK_SIZE
#define AES_KEY_CTX_SIZE    sizeof(vcsUint32_t) * 61

#define DES_KEY_SIZE    8
#define DES_BLOCK_SIZE  8

#define MD5_HASH_SIZE   16

#define SHA1_HASH_SIZE  20
#define SHA1_MD5_HASH_BLOCK_SIZE 64

#define SHA256_HASH_SIZE 32
#define SHA256_BLOCK_SIZE 64

#define PRESENT80_BLOCK_SIZE 8

/* Supported RSA key length in bits */
#define VCS_RSA_KEY_LEN_2048        2048
#define VCS_RSA_KEY_LEN_1024        1024

/* Key formats. */
typedef enum palKeyFormat_e
{
    palKeyFormat_raw,           /* Raw sequence of bytes.  */
    palKeyFormat_der,           /* DER format - ASN.1 DER  */
    palKeyFormat_oct,           /* Octet string.           */
} palKeyFormat_t;

/* Supported PRFs */
typedef enum palPRF_e
{
    palPRF_TLS12SHA256,         /* TLS1.2 w/ SHA256 */
    palPRF_Unknown,             /* other protocol/alg not implemented */
} palPRF_t;

/**< Handle to the RSA key object (Public or Private).
     This handle can be obtained from a call to
     palCryptoRsaGenerateKeypair() function. This handle
     should be destroyed via palCryptoObjectDestroyHandle().*/
typedef struct palCryptoObject_s * palCryptoObjectHandle_t;

/** Initialize crypto engine.
*/
vcsResult_t VCS_API palCryptoInitialize(void);

/** Uninitialize crypto engine.
*/
vcsResult_t VCS_API palCryptoUninitialize(void);

/**
    Get the system seed.

    @param pInSeed          Contain seed material which will be mixed to system seed.
    @param pSeed            Pointer to the buffer to be filled with seed data.

    @return VCS return code,    VCS_RESULT_OK if no error.
*/
vcsResult_t VCS_API
palSystemSeedGet(
    vcsBlobData_t * pInSeed,
    vcsBlobData_t * pSysSeed);

/**
    Generate pseudo random numbers.

    @param output           Pointer to the output buffer to be filled with random bytes.

    @param bytes            Number of random bytes.

    @return VCS return code,    VCS_RESULT_OK if no error.
*/
vcsResult_t VCS_API palCryptoRng(vcsUint8_t * output,
                                 vcsUint32_t  bytes);

/*!
*******************************************************************************
** Symmetric key generation function using PRF.
**
** @param[in]       pSecret
**      Secret key for PRF.
** @param[in]       label
**      Unique label for each purpose of PRF.
** @param[in]       pSeed
**      Seed value to bind into key.
** @param[in]       keyLength
**      Number of bytes of key to generate.
** @param[out]      pOutputKey
**      Generated pseudo-random key.
** @return          VCS return code, VCS_RESULT_OK if no error.
**      Error codes are:
**          VCS_RESULT_GEN_NULL_POINTER
**          VCS_RESULT_GEN_NOT_IMPLEMENTED
**          VCS_RESULT_GEN_BAD_PARAM
**          VCS_RESULT_GEN_ERROR
*/
vcsResult_t VCS_API
palSymKeyGen(
             vcsBlobData_t * pSecret,
             vcsString_t     label,
             vcsBlobData_t * pSeed,
             vcsUint32_t     keyLength,
             vcsUint8_t *    pOutputKey
             );


/*!
*******************************************************************************
** PRF function.
**
** @param[in]       pSecret
**      Secret key for PRF.
** @param[in]       label
**      Unique label for each purpose of PRF.
** @param[in]       labelLength
**      Size of label.
** @param[in]       pSeed
**      Seed value to bind into key.
** @param[in]       keyLength
**      Number of bytes of key to generate.
** @param[out]      pOutputKey
**      Generated pseudo-random key.
** @return          VCS return code, VCS_RESULT_OK if no error.
**      Error codes are:
**          VCS_RESULT_GEN_NULL_POINTER
**          VCS_RESULT_GEN_NOT_IMPLEMENTED
**          VCS_RESULT_GEN_BAD_PARAM
**          VCS_RESULT_GEN_ERROR
*/
vcsResult_t VCS_API
palPRF(
       vcsBlobData_t * pSecret,
       vcsUint8_t *    label,
       vcsUint32_t     labelLength,
       vcsBlobData_t * pSeed,
       vcsUint32_t     keyLength,
       vcsUint8_t *    pOutputKey,
       palPRF_t        ePRF
       );

/**
    Generate AES symmetric key.

    @param key           Pointer to the buffer to be filled with generated key value.

    @param bytes         AES key len. Should be on of AES_KEY_SIZE_256, AES_KEY_SIZE_192 and
                         AES_KEY_SIZE_128.

    @return VCS return code,    VCS_RESULT_OK if no error.
*/
vcsResult_t VCS_API palCryptoAesKeyGen(vcsUint8_t * key,
                                       vcsUint32_t  len);

/*
  Derive AES key in the base of pBaseData, which can
  be a password, password hash or any data.
  pKey is a derived key, which is an out parameter
*/
vcsResult_t VCS_API palCryptoAesKeyDerive(vcsUint8_t * pBaseData,
                                          vcsUint32_t  len,
                                          vcsUint8_t * pKey);

/**
    Generate Triple DES symmetric key.

    @param key           Pointer to the buffer to be filled with generated key value.
                         Key buffer should be of 3*DES_KEY_SIZE size.

    @return VCS return code,    VCS_RESULT_OK if no error.
*/
vcsResult_t VCS_API palCryptoTdesKeyGen(vcsUint8_t * des_key);

/**
    Encrypt input buffer via provided symmetric key.

    @param in_data           Pointer to the input data buffer to be encrypted.

    @param in_data_len       Input data buffer length.

    @param out_data          Pointer to the output buffer to be filled with encrypted data.

    @param out_data_len      Encrypted data length.

    @param key_value         Pointer to the symmetric key buffer. Type of the key determined
                             by last (mech) parameter.

    @param key_len           Key buffer length.

    @param init_v            Pointer to the initialization vector buffer. Buffer size should be 16.
                             This parameter is used only when last(mech) parameter is VCS_TDES_CBC
                             or VCS_AES_CBC.

    @param mech              Mechanism to use for encryption. Possible values are VCS_AES_CBC,
                             VCS_AES_ECB, VCS_TDES_CBC and VCS_TDES_ECB.

    @return VCS return code,    VCS_RESULT_OK if no error.
*/
vcsResult_t VCS_API palCryptoEncrypt(
                              vcsUint8_t    * in_data,
                              vcsUint32_t     in_data_len,
                              vcsUint8_t    * out_data,
                              vcsUint32_t   * out_data_len,
                              vcsUint8_t    * key_value,
                              vcsUint32_t     key_len,
                              vcsUint8_t    * init_v,
                              vcsMechanism_t  mech);

/**
    Decrypt input buffer via provided symmetric key.

    @param in_data           Pointer to the input data buffer to be decrypted.

    @param in_data_len       Input data buffer length.

    @param out_data          Pointer to the output buffer to be filled with decrypted data.

    @param out_data_len      Decrypted data length.

    @param key_value         Pointer to the symmetric key buffer. Type of the key determined
                             by last (mech) parameter.

    @param key_len           Key buffer length.

    @param init_v            Pointer to the initialization vector buffer. Buffer size should be 16.
                             This parameter is used only when last(mech) parameter is VCS_TDES_CBC
                             or VCS_AES_CBC.

    @param mech              Mechanism to use for decryption. Possible values are VCS_AES_CBC,
                             VCS_AES_ECB, VCS_TDES_CBC and VCS_TDES_ECB.

    @return VCS return code,    VCS_RESULT_OK if no error.
*/
vcsResult_t VCS_API palCryptoDecrypt(
                              vcsUint8_t    * in_data,
                              vcsUint32_t     in_data_len,
                              vcsUint8_t    * out_data,
                              vcsUint32_t   * out_data_len,
                              vcsUint8_t    * key_value,
                              vcsUint32_t     key_len,
                              vcsUint8_t    * init_v,
                              vcsMechanism_t  mech);

/* Memory protection modes used by palCryptoProtectMemory() function */
typedef enum palMemProtectMode_e
{
    palMemProtect_sameProcess,  /* Encrypt/decrypt memory in the same process */
    palMemProtect_crossProcess, /* Encrypt/decrypt memory in different processes of app. */
    palMemProtect_sameLogon,    /* Encrypt/decrypt memory in any process
                                 * executed by current logged in user */
    palMemProtect_localMachine  /* Any user on local machine can decrypt the data */
} palMemProtectMode_t;

/*
    Protect specified memory

    @param pMemory           A pointer to the memory to encrypt/decrypt.

    @param length            Number of bytes to which pMemory points.
                             The number of bytes must be a multiple of the
                             CRYPTPROTECTMEMORY_BLOCK_SIZE = 16 constant defined in Wincrypt.h.

    @param palMemProtectType_t
                             ProtectionMode.

    @param bProtect          must be VCS_TRUE if we want to protect (encrypt) data
                             or VCS_FALSE if we want to un-protect (decrypt)

    @return VCS return code, VCS_RESULT_OK if no error.
*/

vcsResult_t VCS_API palCryptoProtectMemory(
                       vcsUint8_t         * pMemBuf,
                       vcsUint32_t          length,
                       palMemProtectMode_t  protectMode,
                       vcsBool_t            bProtect);
/**
    Destroy RSA key object handle.

    @param pObj                 Pointer to the handle of the RSA key object.

    @return VCS return code,    VCS_RESULT_OK if no error.

*/
vcsResult_t VCS_API palCryptoObjectDestroyHandle(palCryptoObjectHandle_t * pObj);

/**
    Create RSA public key handle from external data

    @param modBits              RSA key length in bits to be generated.
                                Should be in the range [1024,2048]

    @param pPubExponent         Pointer to the buffer containing
                                the public exponent of RSA key.

    @param pModulus             Pointer to the buffer containing
                                the modulus.

    @param phPubKey             Pointer to the RSA public key
                                handle to be generated.

    @return VCS return code,    VCS_RESULT_OK if no error.
*/
vcsResult_t VCS_API palCryptoRsaCreatePublicKeyHandle(
                                            vcsUint32_t              modBits,
                                            vcsUint8_t *             pPubExponent,
                                            vcsUint8_t *             pModulus,
                                            palCryptoObjectHandle_t* phPubKey);

/**
    Create RSA private key handle from external data

    @param modBits              RSA key length in bits to be generated.
                                Should be in the range [1024,2048]

    @param pPrivKeyBlob         Pointer to the buffer containing
                                the blob data of RSA private key.
                                The RSA private key blob data is generated as
                                result of palCryptoRsaExportPrivateKey()
                                function call.

    @param phPrivKey            Pointer to the RSA private key
                                handle to be generated.

    @return VCS return code,    VCS_RESULT_OK if no error.
*/
vcsResult_t VCS_API palCryptoRsaCreatePrivateKeyHandle(
                                            vcsUint32_t               modBits,
                                            vcsUint8_t *              pPrivKeyBlob,
                                            palCryptoObjectHandle_t * phPrivKey);

/**
    Export the RSA public key

    @param pModBits             Pointer to the variable for exporting
                                RSA key length (in bits).

    @param pPubExponent         Pointer to the output buffer for the
                                public exponent of RSA key.

    @param pModulus             Pointer to the output buffer for
                                the modulus.

    @param hPubKey              Handle of the RSA public key.

    @return VCS return code,    VCS_RESULT_OK if no error.
*/

vcsResult_t VCS_API palCryptoRsaExportPublicKey(
                                            vcsUint32_t *           pModBits,
                                            vcsUint32_t *           pPubExponent,
                                            vcsUint8_t *            pModulus,
                                            palCryptoObjectHandle_t hPubKey);

/**
    Export the blob data of the RSA private key

    @param pBlobLen          Pointer to the variable for blob data length.

    @param pPrivKeyBlob      Pointer to the output buffer for the blob data.

    @param hPrivKey          Handle of the RSA private key.

    @return VCS return code, VCS_RESULT_OK if no error.
*/
vcsResult_t VCS_API palCryptoRsaExportPrivateKey(
                                            vcsUint32_t *           pBlobLen,
                                            vcsUint8_t *            pPrivKeyBlob,
                                            palCryptoObjectHandle_t hPrivKey);

/**
    Generate RSA keypair (public/private).

    @param modulus_bits         RSA key length in bits to be generated.
                                Should be in the range [512, 2048]

    @param publ_obj             Pointer to the RSA public key handle to be generated.

    @param priv_obj             Pointer to the RSA public key handle to be generated.

    @param bSigningKeypair      Indicates if the RSA keypair will be generated for
                                sign/verify operations. This information is required by
                                MS CAPI. In non-Windows platforms this parameter may be unused.

    @return VCS return code,    VCS_RESULT_OK if no error.
*/
vcsResult_t VCS_API palCryptoRsaGenerateKeypair(vcsUint32_t                modulus_bits,
                                                palCryptoObjectHandle_t  * publ_obj,
                                                palCryptoObjectHandle_t  * priv_obj,
                                                vcsBool_t                  bSigningKeypair);

/**
    Do RSA private decryption

    @param in_data              Pointer to the encrypted input data to be decrypted.

    @param in_data_len          Input data length in bytes. Should be equal to the RSA private key
                                length.

    @param out_data             Pointer to the output data buffer to be filled with ciphertext.
                                This buffer size should be greater or equal to the (RSA key length - 11).

    @param out_data_len         Pointer to the output data buffer length. As an input the size of out_data
                                buffer should be specified. It should be greater or equal to
                                the (RSA key length - 11). After successful operation it will contain
                                the plaintext data length. It will be less or equal to (RSA key length - 11).

    @param priv_key_obj         Handle to the RSA private key object.

    @return VCS return code,    VCS_RESULT_OK if no error.
*/

vcsResult_t VCS_API palCryptoRsaDecrypt(
                                  vcsUint8_t             * in_data,
                                  vcsUint32_t              in_data_len,
                                  vcsUint8_t             * out_data,
                                  vcsUint32_t            * out_data_len,
                                  palCryptoObjectHandle_t  priv_key_obj );

/**
    Do RSA public encryption

    @param in_data              Pointer to the input data to be encrypted.

    @param in_data_len          Input data length in bytes. Should be less or equal to the (RSA key
                                length - 11).

    @param out_data             Pointer to the output data buffer to be filled with encrypted data.
                                This buffer size should be greater or equal to the RSA key length.

    @param out_data_len         Pointer to the output data buffer len.
                                This value will be equal to the RSA key length.

    @param publ_key_obj         Handle to the RSA public key object.

    @return VCS return code,    VCS_RESULT_OK if no error.
*/
vcsResult_t VCS_API palCryptoRsaEncrypt(
                                  vcsUint8_t             * in_data,
                                  vcsUint32_t              in_data_len,
                                  vcsUint8_t             * out_data,
                                  vcsUint32_t            * out_data_len,
                                  palCryptoObjectHandle_t  publ_key_obj );

/**
    RSA sign the specified data hash

    @param in_hash              Pointer to the input hash to be signed.

    @param in_hash_len          The length of input hash.

    @param in_hash_type         Hash type.

    @param out_data             Pointer to the output data buffer to be filled with signature.
                                This buffer size should be greater or equal to the RSA key length.

    @param out_data_len         Pointer to the signature buffer len.
                                This value will be equal to the RSA key length.

    @param priv_key_obj         Handle to the RSA private key object.

    @return VCS return code,    VCS_RESULT_OK if no error.
**/

vcsResult_t VCS_API palCryptoRsaSign(
                               vcsUint8_t             * in_hash,
                               vcsUint32_t              in_hash_len,
                               vcsMechanism_t           in_hash_type,
                               vcsUint8_t             * out_data,
                               vcsUint32_t            * out_data_len,
                               palCryptoObjectHandle_t  priv_key_obj );

/**
    Verify the RSA signature of specified hash

    @param in_signature         Pointer to the RSA signature buffer.

    @param in_signature_len     Signature length in bytes. Should be equal to the
                                RSA key length.

    @param in_hash              Pointer to the input hash the signature of which
                                should be verified.

    @param in_data_len          Input hash length.

    @param in_hash_type         Hash type.

    @param publ_key_obj         Handle to the RSA public key object.

    @return VCS return code,    VCS_RESULT_OK if verification succeeded,
                                VCS_RESULT_GEN_SIGNATURE_VERIFICATION_FAILED if verification failed
**/

vcsResult_t VCS_API palCryptoRsaVerify(
                                 vcsUint8_t             * in_signature,
                                 vcsUint32_t              in_signature_len,
                                 vcsUint8_t             * in_hash,
                                 vcsUint32_t              in_hash_len,
                                 vcsMechanism_t           in_hash_type,
                                 palCryptoObjectHandle_t  publ_key_obj );

/* Calculate the length of the smallest
** buffer multiple to 4-bytes which can fit
** "strength"-bit number. */
#define PAL_CRYPT_ECC_STRENGTH_TO_PARAMLEN(strength)                           \
    (((strength) & 0x1F) == 0) ? ((strength) >> 3) :                           \
    ((((strength) + (0x20 - ((strength) & 0x1F)))) >> 3)

/* Ecc domain parameters */
typedef struct palEccDomainParams_s
{
    vcsUint32_t iana_id;    /* Curve IANA ID                                */
    vcsUint32_t strength;   /* Public key length in bits: 256, 348 or 521   */
    vcsUint32_t param_len;  /* Length of the curve parameters in bytes.
                               If the curve is N-bit, then this is the
                               length of the smallest buffer multiple to
                               4-bytes which can fit N-bit number.
                               For example, it is 68 bytes for 512-bit curve.*/
} palEccDomainParams_t;

/**
  Specify the Elliptic Curve and build domain parameters.

  @param pCurveParams         Pointer to Elliptic curve parameters structure.

  @param phDomainParameters   Pointer to the handle of ECC Domain Parameters object.

  @return VCS return code     VCS_RESULT_OK if no error.
*/

vcsResult_t VCS_API 
palCryptoEccEllipticCurveSet(
                             vcsEccCurveParams_t    * pCurveParams,
                             palCryptoObjectHandle_t * phDomainParameters
                             );

/**
    Get ECC domain parameters

    @param hDomainParameters    Handle of ECC Domain Parameters object obtained
                                by calling palCryptoEccEllipticCurveSet() function.

    @return                     Pointer to a palEccDomainParams_t struct. 
                                Returns VCS_NULL on failure.
*/
palEccDomainParams_t * VCS_API 
palCryptoEccDomainParamsGet(
                            palCryptoObjectHandle_t hDomainParameters
                            );

/**
    Create ECC public key handle from external data

    @param hDomainParameters    Handle of ECC Domain Parameters object obtained
                                by calling palCryptoEccEllipticCurveSet() function.

    @param keyByteLength        ECC key length in bytes to be generated.

    @param pPubKeyX             Pointer to the buffer containing
                                the X coordinate of the public key.

    @param pPubKeyY             Pointer to the buffer containing
                                the Y coordinate of the public key.

    @param phPubKey             Pointer to the ECC public key
                                handle to be generated.

    @param phKey                Pointer to resulting key handle.
    
    @return VCS return code,    VCS_RESULT_OK if no error.
*/
vcsResult_t VCS_API 
palCryptoEccCreatePublicKeyHandle(
                                  palCryptoObjectHandle_t   hDomainParameters,
                                  vcsUint32_t               keyByteLen,
                                  vcsUint8_t *              pPubKeyX,
                                  vcsUint8_t *              pPubKeyY,
                                  palCryptoObjectHandle_t * phKey
                                  );

/**
    Create ECC private key handle from external data

    @param hDomainParameters    Handle of ECC Domain Parameters object obtained
                                by calling palCryptoEccEllipticCurveSet() function.

    @param keyByteLength        ECC key length in bytes to be generated.

    @param pPrivKeyBlob         Pointer to the buffer containing
                                the ECC private key blob.

    @param phKey                Pointer to resulting key handle.
    
    @return VCS return code,    VCS_RESULT_OK if no error.
*/
vcsResult_t VCS_API 
palCryptoEccCreatePrivateKeyHandle(
                                   palCryptoObjectHandle_t   hDomainParameters,
                                   vcsUint32_t               blobLen,
                                   vcsUint8_t *              pPrivKeyBlob,
                                   palCryptoObjectHandle_t * phKey
                                   );

/**
    Export a public ECC key.

    @param hDomainParameters    Handle of ECC Domain Parameters object obtained
                                by calling palCryptoEccEllipticCurveSet() function.

    @param pKeyByteLen          Pointer to the variable for exporting 
                                ECC key length.

    @param pPubKeyX             Pointer to the output buffer for
                                the X coordinate of the public key.

    @param pPubKeyY             Pointer to the output buffer for
                                the Y coordinate of the public key.

    @param hKey                 The input ECC public key handle.

    @return VCS return code     VCS_RESULT_OK if no error.
  */
vcsResult_t VCS_API 
palCryptoEccExportPublicKey(
                            palCryptoObjectHandle_t   hDomainParameters,
                            vcsUint32_t *             pKeyByteLen,
                            vcsUint8_t *              pPubKeyX,
                            vcsUint8_t *              pPubKeyY,
                            palCryptoObjectHandle_t   hKey
                            );

/**
    Export a public ECC key.

    @param hDomainParameters    Handle of ECC Domain Parameters object obtained
                                by calling palCryptoEccEllipticCurveSet() function.

    @param pBlobLen             Pointer to the variable for exporting 
                                blob data length.

    @param pPrivKeyBlob         Pointer to the output buffer for the blob data.

    @param hKey                 The input ECC private key handle.

    @return VCS return code     VCS_RESULT_OK if no error.
  */
vcsResult_t VCS_API 
palCryptoEccExportPrivateKey(
                             palCryptoObjectHandle_t   hDomainParameters,
                             vcsUint32_t *             pBlobLen,
                             vcsUint8_t *              pPrivKeyBlob,
                             palCryptoObjectHandle_t   hKey
                             );


/*******************************************************************************
** Signature encoding function. (from R,S to DER ASN.1)
*/
vcsResult_t VCS_API 
palCryptoEccEncodeSignature(
                            palCryptoObjectHandle_t   hDomainParameters,
                            vcsUint8_t *    pInR,
                            vcsUint8_t *    pInS,
                            vcsUint32_t     inByteLength,
                            vcsUint8_t *    pOutData,
                            vcsUint32_t *   pOutDataLen
                            );

/*******************************************************************************
** Key format encoding function.
*/
vcsResult_t VCS_API 
palCryptoEccEncodePublicKey(
                            palCryptoObjectHandle_t   hDomainParameters,
                            vcsUint8_t *    pInPubKeyX,
                            vcsUint8_t *    pInPubKeyY,
                            vcsUint32_t     inKeyByteLength,
                            palKeyFormat_t  outDataKeyFormat,
                            vcsUint8_t *    pOutData,
                            vcsUint32_t *   pOutDataLen
                            );

/*******************************************************************************
** Key format decoding function.
*/
vcsResult_t VCS_API 
palCryptoEccDecodePublicKey(
                            palCryptoObjectHandle_t   hDomainParameters,
                            palKeyFormat_t  inDataKeyFormat,
                            vcsUint8_t *    pInData,
                            vcsUint32_t     inDataLen,
                            vcsUint8_t *    pOutPubKeyX,
                            vcsUint8_t *    pOutPubKeyY,
                            vcsUint32_t *   outKeyByteLength
                            );

/**
  Generate ECC keypair on specified Elliptic Curve.

  @param hDomainParameters    Handle of ECC Domain Parameters object obtained
                              by calling palCryptoEccEllipticCurveSet() function.

  @param pSeed                Pointer to seed from which to derive keypair (optional).

  @param phPubK               Pointer to the ECC public key handle to be generated.

  @param phPrivK              Pointer to the ECC private key handle to be generated.

  @return VCS return code     VCS_RESULT_OK if no error.
*/
vcsResult_t VCS_API 
palCryptoEccKeypairGenerate(
                            palCryptoObjectHandle_t   hDomainParameters,
                            vcsBlobData_t *           pSeed,
                            palCryptoObjectHandle_t * phPubK,
                            palCryptoObjectHandle_t * phPrivK);

/**
   ECDH: Calculate the shared key.

   @param hDomainParameters    Handle of ECC Domain Parameters object obtained
                               by calling palCryptoEccEllipticCurveSet() function.

   @param hPrivK               ECC private key handle.

   @param pPeerPubKBuf         Buffer, containing peer's public key in octet
                               string format.

   @param peerPubKBufLen       Peer's public key buffer length.

   @param pSharedKey           Buffer for storing the output shared secret.
                               Can be NULL for retrieving the shared key length.
                               It is equal to the byte-length of the curve "order"
                               padded with leading zeros.

   @param pSharedKeyLen        Length of final shared key.

   @return VCS return code     VCS_RESULT_OK if no error.
*/
vcsResult_t VCS_API palCryptoECDH(palCryptoObjectHandle_t   hDomainParameters,
                                  palCryptoObjectHandle_t   hPrivK,
                                  const vcsUint8_t        * pPeerPubKBuf,
                                  vcsUint32_t               peerPubKBufLen,
                                  vcsUint8_t              * pSharedKey,
                                  vcsUint32_t             * pSharedKeyLen);

/**
  ECDSA Sign

  @param hDomainParameters    Handle of ECC Domain Parameters object obtained
                              by calling palCryptoEccEllipticCurveSet() function.

  @param hPrivK               ECC private key handle.

  @param pInData              Buffer, containing input data to be signed.
                              It cannot exceed the curve "order" bit-length.

  @param inDataLen            Input data length.

  @param pSign                Signature buffer. Data is in DER, ASN.1 encoded
                              format.
                              Can be NULL for retriving maximal possible signature
                              length.

  @param pSignLen             Output signature length.

  @return VCS return code     VCS_RESULT_OK if no error.
*/
vcsResult_t VCS_API palCryptoECDSASign(palCryptoObjectHandle_t   hDomainParameters,
                                       palCryptoObjectHandle_t   hPrivK,
                                       const vcsUint8_t        * pInData,
                                       vcsUint32_t               inDataLen,
                                       vcsUint8_t              * pSign,
                                       vcsUint32_t             * pSignLen);


/**
  ECDSA Verify

  @param hDomainParameters    Handle of ECC Domain Parameters object obtained
                              by calling palCryptoEccEllipticCurveSet() function.

  @param hPubK                ECC public key handle.

  @param pSign                Buffer, containing the ECDSA signature.
                              Data is in DER, ASN.1 encoded format.

  @param signLen              Signature length.

  @param pHash                Pointer to hash buffer which will be compared with
                              the signature.

  @param hashLen              The hash length. It cannot be longer that the
                              curve "order".

  @return VCS return code     VCS_RESULT_OK if no error.
*/
vcsResult_t VCS_API palCryptoECDSAVerify(palCryptoObjectHandle_t   hDomainParameters,
                                         palCryptoObjectHandle_t   hPubK,
                                         const vcsUint8_t        * pSign,
                                         vcsUint32_t               signLen,
                                         vcsUint8_t              * pHash,
                                         vcsUint32_t               hashLen);


typedef vcsHandle_t palDigestCtxHandle_t;

vcsResult_t VCS_API
palCryptoDigestCtxDuplicate(
    palDigestCtxHandle_t *     pTargetCtx,
    const palDigestCtxHandle_t hSourceCtx);

vcsResult_t VCS_API
palCryptoDigestInit(
    palDigestCtxHandle_t * ctx,
    vcsMechanism_t         mech);

vcsResult_t VCS_API
palCryptoDigestUpdate(
    palDigestCtxHandle_t    ctx,
    const vcsUint8_t *      pData,
    vcsUint32_t             dataSize);

/* Calculates digest on data collected in context.
   @param[in]     ctx          - digest calculation context
   @param[out]    pHash        - buffer for digest
   @param[in-out] pHashBufSize - buffer size on input, calculated digest size on output
*/
vcsResult_t VCS_API
palCryptoDigestFinal(
    palDigestCtxHandle_t    ctx,
    vcsUint8_t *            pHash,
    vcsUint32_t *           pHashBufSize);

vcsResult_t VCS_API
palCryptoDigestCtxDuplicate1(
    palDigestCtxHandle_t *     pTargetCtx,
    const palDigestCtxHandle_t hSourceCtx);

/* Calculate the digest of the provided buffer.
   The 'pHash' argument should point to the buffer with correct hash size.
*/
vcsResult_t VCS_API palCryptoDigest(const vcsUint8_t *   pData,
                                    vcsUint32_t          dataSize,
                                    vcsUint8_t *         pHash,
                                    vcsMechanism_t       mech);

/* Calculate the HMAC of the provided buffer with the given key.
   The 'pMac' argument should point to the buffer with correct HMAC size.
*/
vcsResult_t VCS_API palCryptoMAC(const vcsUint8_t *   pData,
                                 vcsUint32_t          dataSize,
                                 vcsUint8_t *         pKey,
                                 vcsUint32_t          keySize,
                                 vcsUint8_t *         pMac,
                                 vcsUint32_t          macSize,
                                 vcsMechanism_t       mech);

/**
    Calculate checksum on message.

    @param InitialValue         Initial value for checksum.

    @param pChecksum            Checksum output.

    @param pMsg                 Pointer to the message buffer.

    @param length               Message length.

    @param mech                 Checksum algorithm.

    @return VCS return code,    VCS_RESULT_OK if no error.
                                Error code are VCS_RESULT_GEN_BAD_PARAM,
                                VCS_RESULT_GEN_INVALID.
*/
vcsResult_t VCS_API palCryptoChecksum(
                                vcsUint32_t    initialValue,
                                vcsUint32_t *  pChecksum,
                                vcsUint8_t  *  pMsg,
                                vcsUint32_t    length,
                                vcsMechanism_t mech);

palSharedMemoryHandle_t VCS_API palSharedMemoryAlloc(
                                    vcsString_t    pShMFileName,
                                    vcsUint32_t    ShmSize);

palSharedMemoryHandle_t VCS_API palSharedMemoryOpen(
                                    vcsString_t pShMFileName,
                                    vcsUint32_t ShmSize);
vcsResult_t VCS_API palSharedMemoryFree(palSharedMemoryHandle_t hSharedMemory);

vcsUint8_t* VCS_API palSharedMemoryPointerGet(
                                    palSharedMemoryHandle_t hSharedMemory,
                                    vcsUint32_t             ShmSize);

vcsResult_t VCS_API palSharedMemoryPointerRelease(vcsUint8_t* pBuf);

palSemaphoreHandle_t VCS_API palSemaphoreAlloc(
                                    vcsString_t pSemFileName,
                                    vcsUint32_t SemInitCount);
palSemaphoreHandle_t VCS_API palSemaphoreOpen(
                                    vcsString_t pSemFileName,
                                    vcsUint32_t SemInitCount
                                    );

vcsResult_t VCS_API palSemaphoreFree(palSemaphoreHandle_t   hSemaphore);

vcsResult_t VCS_API palSemaphoreAcquire(
                                    palSemaphoreHandle_t    hSemaphore,
                                    vcsUint32_t             timeout);

vcsResult_t VCS_API palSemaphoreRelease(palSemaphoreHandle_t    hSemaphore);

vcsResult_t VCS_API palThreadKill(palThreadHandle_t hThread,
                                  vcsUint32_t       uExitNum);
vcsUint32_t VCS_API palCurrentProcessId(void);
vcsUint32_t VCS_API palCurrentThreadId(void);
vcsBool_t   VCS_API palProcessExists(vcsUint32_t pID);
vcsResult_t VCS_API palGetApplicationName(vcsUint32_t   pID,
                                          vcsString_t   appName,
                                          vcsUint32_t * appNameLen);

/* Persistent storage key paths string definitions */
#define REG_VAL_KEY_ROOT            "SOFTWARE\\Validity"
#define VCS_VAL_KEY_NAV             "SOFTWARE\\Validity\\Nav"
#define VCS_VAL_KEY_IR              "SOFTWARE\\Validity\\IR"
#define VCS_VAL_KEY_VIP             "SOFTWARE\\Validity\\VIP"
#define VCS_VAL_KEY_FIDO            "SOFTWARE\\Validity\\FIDO"
#define VCS_VAL_KEY_ACM_CEM         "SOFTWARE\\Validity\\AcmCem"
#define VCS_VAL_KEY_MATCHER         "SOFTWARE\\Validity\\Matcher"
#define VCS_VAL_KEY_CAL             "SOFTWARE\\Validity\\Cal"
#define VCS_VAL_KEY_CAL_WOF2        "SOFTWARE\\Validity\\Cal\\Wof2"
#define VCS_VAL_KEY_DBG             "SOFTWARE\\Validity\\Dbg"
#define REG_VAL_KEY_SENSOR          "SOFTWARE\\Validity\\Sensor"
#define VCS_VAL_KEY_CAPTURE         "SOFTWARE\\Validity\\Capture"
#define VCS_VAL_KEY_POWER_POLICY    "SOFTWARE\\Validity\\PowerPolicy"
#define VCS_VAL_KEY_SECURITY        "SOFTWARE\\Validity\\Security"
#define VCS_VAL_KEY_FDETECT         "SOFTWARE\\Validity\\FDetect"
#define VCS_VAL_KEY_ENROLL          "SOFTWARE\\Validity\\Enrollment"
#define VCS_VAL_KEY_LINEUPD_PARAMS  "SOFTWARE\\Validity\\LineUpdParams"

/* Persistent storage key paths with UNICODE/ASCII conversion per build/platform */
#define REGISTRY_KEY_ROOT            VCS_STRING(REG_VAL_KEY_ROOT)
#define VALIDITY_KEY_NAV             VCS_STRING(VCS_VAL_KEY_NAV)
#define VALIDITY_KEY_IR              VCS_STRING(VCS_VAL_KEY_IR)
#define VALIDITY_KEY_VIP             VCS_STRING(VCS_VAL_KEY_VIP)
#define VALIDITY_KEY_FIDO            VCS_STRING(VCS_VAL_KEY_FIDO)
#define VALIDITY_KEY_ACM_CEM         VCS_STRING(VCS_VAL_KEY_ACM_CEM)
#define VALIDITY_KEY_MATCHER         VCS_STRING(VCS_VAL_KEY_MATCHER)
#define VALIDITY_KEY_CAL             VCS_STRING(VCS_VAL_KEY_CAL)
#define VALIDITY_KEY_CAL_WOF2        VCS_STRING(VCS_VAL_KEY_CAL_WOF2)
#define VALIDITY_KEY_DBG             VCS_STRING(VCS_VAL_KEY_DBG)
#define REGISTRY_KEY_SENSOR          VCS_STRING(REG_VAL_KEY_SENSOR)
#define VALIDITY_KEY_CAPTURE         VCS_STRING(VCS_VAL_KEY_CAPTURE)
#define VALIDITY_KEY_POWER_POLICY    VCS_STRING(VCS_VAL_KEY_POWER_POLICY)
#define VALIDITY_KEY_SECURITY        VCS_STRING(VCS_VAL_KEY_SECURITY)
#define VALIDITY_KEY_FDETECT         VCS_STRING(VCS_VAL_KEY_FDETECT)
#define VALIDITY_KEY_ENROLL          VCS_STRING(VCS_VAL_KEY_ENROLL)
#define VALIDITY_KEY_LINEUPD_PARAMS  VCS_STRING(VCS_VAL_KEY_LINEUPD_PARAMS)
//compatibility transition names VCS->REG, VALIDITY->REGISTRY rodzzz
#define VALIDITY_KEY_ROOT            REGISTRY_KEY_ROOT
#define VALIDITY_KEY_SENSOR          REGISTRY_KEY_SENSOR
#define VCS_VAL_KEY_ROOT             REG_VAL_KEY_ROOT
#define VCS_VAL_KEY_SENSOR           REG_VAL_KEY_SENSOR

#if VCS_FEATURE_PERSISTENT_DATA_PATH
#define SYNC_PERSISTENT_DATA_PATH_ENV	"SyncPersistentDataPath"
#endif /* VCS_FEATURE_PERSISTENT_DATA_PATH */

/*!
*******************************************************************************
** Get long value from registry. If value can't be received it returns
** default value.
**
** @param[in]       keyName
**      null-terminated string containing the name of the sub key to open.
** @param[in]       valueName
**      null-terminated string containing the name of the value to query.
** @param[in]       defValue
**      Default value.
** @param[in]       longValue
**      Pointer where value will be stored
** @return          VCS return code, VCS_RESULT_OK if no error.
**      Error codes are:
**          VCS_RESULT_GEN_NULL_POINTER
**          VCS_RESULT_SYS_FILE_OPEN_FAILED
**          VCS_RESULT_SYS_FILE_READ_FAILED
*/
vcsResult_t VCS_API
palGetPersistentDataLongValue(
vcsConstString_t     keyName,
vcsConstString_t     valueName,
    vcsUint32_t     defValue,
    vcsUint32_t *   longValue);

/*!
*******************************************************************************
** Set long value in registry.
**
** @param[in]       keyName
**      null-terminated string containing the name of the sub key to open.
** @param[in]       valueName
**      null-terminated string containing the name of the value to query.
** @param[in]       longValue
**      Value to store.
** @return          VCS return code, VCS_RESULT_OK if no error.
**      Error codes are:
**          VCS_RESULT_GEN_NULL_POINTER
**          VCS_RESULT_SYS_FILE_OPEN_FAILED
**          VCS_RESULT_SYS_FILE_WRITE_FAILED
*/
vcsResult_t VCS_API
palSetPersistentDataLongValue(
    vcsConstString_t keyName,
    vcsConstString_t valueName,
    vcsUint32_t longValue);

/*!
*******************************************************************************
** Get string value from registry. If value can't be received it returns
** default value.
**
** @param[in]       keyName
**      null-terminated string containing the name of the sub key to open.
** @param[in]       valueName
**      null-terminated string containing the name of the value to query.
** @param[in]       defValue
**      Default value.
** @param[in]       defValueSize
**      Default value length.
** @param[in]       stringValue
**      Pointer where string will be stored
** @param[in]       stringValueSize
**      Pointer where size of string will be stored
** @return          VCS return code, VCS_RESULT_OK if no error.
**      Error codes are:
**          VCS_RESULT_GEN_NULL_POINTER
**          VCS_RESULT_SYS_FILE_OPEN_FAILED
**          VCS_RESULT_SYS_FILE_READ_FAILED
*/
vcsResult_t VCS_API
palGetPersistentDataStringValue(
    vcsConstString_t     keyName,
    vcsConstString_t     valueName,
    vcsConstString_t     defValue,
    vcsUint32_t     defValueSize,
    vcsString_t     stringValue,
    vcsUint32_t *   stringValueSize);

/*!
*******************************************************************************
** Set string value in registry.
**
** @param[in]       keyName
**      null-terminated string containing the name of the sub key to open.
** @param[in]       valueName
**      null-terminated string containing the name of the value to query.
** @param[in]       stringValue
**      Value to store.
** @param[in]       stringValueSize
**      Size of string value to store.
** @return          VCS return code, VCS_RESULT_OK if no error.
**      Error codes are:
**          VCS_RESULT_GEN_NULL_POINTER
**          VCS_RESULT_SYS_FILE_OPEN_FAILED
**          VCS_RESULT_SYS_FILE_WRITE_FAILED
*/
vcsResult_t VCS_API
palSetPersistentDataStringValue(
    vcsConstString_t keyName,
    vcsConstString_t valueName,
    vcsConstString_t stringValue,
    vcsUint32_t stringValueSize);

/*!
*******************************************************************************
** Get binary value from registry. If value can't be received it returns
** default value.
**
** @param[in]       keyName
**      null-terminated string containing the name of the sub key to open.
** @param[in]       valueName
**      null-terminated string containing the name of the value to query.
** @param[in]       defValue
**      Default value.
** @param[in]       defValueSize
**      Default value length.
** @param[in]       binaryValue
**      Pointer where binary data will be stored
** @param[in]       binaryValueSize
**      Pointer where size of binary data will be stored
** @return          VCS return code, VCS_RESULT_OK if no error.
**      Error codes are:
**          VCS_RESULT_GEN_NULL_POINTER
**          VCS_RESULT_SYS_FILE_OPEN_FAILED
**          VCS_RESULT_SYS_FILE_READ_FAILED
*/
vcsResult_t VCS_API
palGetPersistentDataBinaryValue(
    vcsConstString_t   keyName,
    vcsConstString_t   valueName,
    vcsUint8_t  * defValue,
    vcsUint32_t   defValueSize,
    vcsUint8_t  * binaryValue,
    vcsUint32_t * binaryValueSize);

/*!
*******************************************************************************
** Set binary value in registry.
**
** @param[in]       keyName
**      null-terminated string containing the name of the sub key to open.
** @param[in]       valueName
**      null-terminated string containing the name of the value to query.
** @param[in]       binaryValue
**      Value to store.
** @param[in]       binaryValueSize
**      Size of string value to store.
** @return          VCS return code, VCS_RESULT_OK if no error.
**      Error codes are:
**          VCS_RESULT_GEN_NULL_POINTER
**          VCS_RESULT_SYS_FILE_OPEN_FAILED
**          VCS_RESULT_SYS_FILE_WRITE_FAILED
*/
vcsResult_t VCS_API
palSetPersistentDataBinaryValue(
    vcsConstString_t keyName,
    vcsConstString_t valueName,
    vcsUint8_t * binaryValue,
    vcsUint32_t binaryValueSize);

/*!
*******************************************************************************
** Remove value from registry.
**
** @param[in]       keyName
**      null-terminated string containing the name of the sub key to open.
** @param[in]       valueName
**      null-terminated string containing the name of the value to query.
** @return          VCS return code, VCS_RESULT_OK if no error.
**      Error codes are:
**          VCS_RESULT_GEN_NULL_POINTER
**          VCS_RESULT_SYS_FILE_OPEN_FAILED
*/
vcsResult_t VCS_API
palRemovePersistentDataValue(
    vcsConstString_t keyName,
    vcsConstString_t valueName);

/*!
*******************************************************************************
** Query key info from registry.
**
** @param[in]       keyName
**      null-terminated string containing the name of the sub key to open.
** @param[out]       cSubKeys
**      number of sub keys.
** @param[out]       cMaxSubKey
**      length of the key's subkey with the longest name.
** @param[out]       cValues
**      number of values for key.
** @param[out]       cMaxValue
**      length of longest value name.
** @param[out]       cMaxValueData
**      length of the longest data component.
** @return          VCS return code, VCS_RESULT_OK if no error.
**      Error codes are:
**          VCS_RESULT_GEN_NULL_POINTER
**          VCS_RESULT_SYS_FILE_OPEN_FAILED
*/

vcsResult_t VCS_API
palGetPersistentQueryInfo(vcsConstString_t     keyName,
                          vcsUint32_t *   cSubKeys,
                          vcsUint32_t *   cMaxSubKey,
                          vcsUint32_t *   cValues,
                          vcsUint32_t *   cMaxValue,
                          vcsUint32_t *   cMaxValueData);

/*!
*******************************************************************************
** Get value name for key.
**
** @param[in]       keyName
**      null-terminated string containing the name of the sub key to open.
** @param[in]       index
**      Index of key enumeration.
** @param[out]       stringValue
**      Value to store.
** @param[out]       stringValueSize
**      Size of string value to store.
** @return          VCS return code, VCS_RESULT_OK if no error.
**      Error codes are:
**          VCS_RESULT_GEN_NULL_POINTER
**          VCS_RESULT_SYS_FILE_OPEN_FAILED
*/

vcsResult_t VCS_API
palGetPersistentEnumValue(vcsConstString_t     keyName,
                          vcsUint32_t     index,
                          vcsString_t     stringValue,
                          vcsUint32_t *   stringValueSize);

/*!
*******************************************************************************
** Get sub key name.
**
** @param[in]       keyName
**      null-terminated string containing the name of the sub key to open.
** @param[in]       index
**      Index of key enumeration.
** @param[out]       stringValue
**      Value to store.
** @param[out]       stringValueSize
**      Size of string value to store.
** @return          VCS return code, VCS_RESULT_OK if no error.
**      Error codes are:
**          VCS_RESULT_GEN_NULL_POINTER
**          VCS_RESULT_SYS_FILE_OPEN_FAILED
*/
vcsResult_t VCS_API
palGetPersistentEnumKey(vcsConstString_t     keyName,
                        vcsUint32_t     index,
                        vcsString_t     stringValue,
                        vcsUint32_t *   stringValueSize);

/*!
*******************************************************************************
** Create key with provided name.
**
** @param[in]       keyName
**      null-terminated string containing the name of the key to create.
** @param[in]       bAdminRigths
**      Indicate if created key should have only administrative access rights or not.
** @return          VCS return code, VCS_RESULT_OK if no error.
**      Error codes are:
**          VCS_RESULT_GEN_NULL_POINTER
**          VCS_RESULT_SYS_FILE_OPEN_FAILED
*/
vcsResult_t VCS_API
palCreatePersistentKey(vcsConstString_t keyName,
                       vcsBool_t   bAdminRigths);

/*!
*******************************************************************************
** Delete key with provided name.
**
** @param[in]       keyName
**      null-terminated string containing the name of the key to delete.
** @return          VCS return code, VCS_RESULT_OK if no error.
**      Error codes are:
**          VCS_RESULT_GEN_NULL_POINTER
*/
vcsResult_t VCS_API
palDeletePersistentKey(vcsConstString_t keyName);

vcsResult_t VCS_API palVerifyPersistentKeyExistence(vcsConstString_t keyName,
                                                    vcsBool_t * bExist);

/* palLoadLibrary explicitly link to a DLL.
   If successful, the function maps the specified DLL into the address
   space of the calling process and returns a handle to the DLL that can
   be used with other functions in explicit linking such as
   palGetProcAddress and palFreeLibrary.
   filePath should not contain extension.
*/
vcsHandle_t VCS_API palLoadLibrary(vcsString_t filePath);
vcsHandle_t VCS_API palGetProcAddress(vcsHandle_t hModule,
                                      vcsString_t ProcName);
vcsBool32_t VCS_API palFreeLibrary(vcsHandle_t hModule);

/* Signature verification API */
vcsResult_t palVerifyFileSignature( vcsString_t fileName);
vcsResult_t palVerifyModuleSignature(vcsString_t moduleName);
vcsResult_t palVerifyServiceSignature(void);
vcsResult_t palVerifyProcessSignature(vcsUint32_t procID);

typedef vcsResult_t (VCS_API * fnCallerVerification_t) (void);
vcsResult_t palSetCallerVerificationFunction(fnCallerVerification_t pFunction);
vcsResult_t palVerifyCallerApplication(void);

typedef struct palMACCtx_s *   palMACCtxHandle_t;

vcsResult_t VCS_API palCryptoMACInit(palMACCtxHandle_t * phCtx,
    vcsUint8_t *        pKey,
    vcsUint32_t         keyLen,
    vcsMechanism_t      mech);
vcsResult_t VCS_API palCryptoMACUpdate(palMACCtxHandle_t   hCtx,
    const vcsUint8_t *  data,
    vcsUint32_t         dataLen);
vcsResult_t VCS_API palCryptoMACFinal(palMACCtxHandle_t hCtx,
    vcsUint8_t *      mac,
    vcsUint32_t       macLen);

/* Resource IDs for the Production and Engineering versions of EFI images
   for different sensor products inbuilt into USDK binaries.
   These are the resource IDs that should be used to integrate corresponding
   binary resources into USDK in usdk/Makefile using RcMan tool (for
   Windows, at least).
   Note that for every sensor product the corresponding EFI image resource IDs
   for Prod and Eng version MUST differ by one, so that the ID of the Eng
   is greater by one of that of the Prod version.  So when adding new EFI
   image IDs, make sure they are added by pairs, even and odd numbers (even
   if one of the Eng or Prod versions is not going to be used or exist).
   Also note that Engineering versions of the images may appear in
   non-production builds of USDK *only*!
*/
#define VCS_EFI_IMAGE_RES_ID_451_PROD       100 
#define VCS_EFI_IMAGE_RES_ID_451_ENG        101

#define VCS_EFI_IMAGE_RES_ID_471_PROD       102
#define VCS_EFI_IMAGE_RES_ID_471_ENG        103

#define VCS_EFI_IMAGE_RES_ID_491_PROD       104
#define VCS_EFI_IMAGE_RES_ID_491_ENG        105

#define VCS_EFI_IMAGE_RES_ID_301_PROD       106
#define VCS_EFI_IMAGE_RES_ID_301_ENG        107

#define VCS_EFI_IMAGE_RES_ID_WINDSOR_PROD   108
#define VCS_EFI_IMAGE_RES_ID_WINDSOR_ENG    109

#define VCS_EFI_IMAGE_RES_ID_495_PROD  		110
#define VCS_EFI_IMAGE_RES_ID_495_ENG    	111

/* Get EFI image */
vcsResult_t VCS_API palGetEFIImage(vcsUint8_t *  pEFI,
                                   vcsUint32_t * pImageSize,
                                   vcsUint32_t   resId);

/* Power source definitions */
#define VCS_POWER_SOURCE_AC         0
#define VCS_POWER_SOURCE_DC         1
#define VCS_POWER_SOURCE_UNKNOWN    0xFF

vcsBool_t VCS_API palIsCalledFromService(void);

#if VCS_FEATURE_SOC
void VCS_API palOutputDebugString(vcsConstString_t str);
#endif

/******************************************************************************/
/*        Interfaces to store any type of data as <Tag, Value> pairs.         */
/******************************************************************************/
typedef vcsHandle_t     palTagValContainerHandle_t;
typedef vcsUint16_t     palTag_t;

typedef enum palValItemType_e
{
    palValTypeVoid = 0,
    palValTypeUint8 = 1,
    palValTypeUint16 = 2,
    palValTypeUint32 = 4,
} palValueType_t;

/* Initialize container. */
vcsResult_t VCS_API
palTagValContainerInit(
    palTagValContainerHandle_t * phContainer);

/* Uninitialize container and deallocate memory used by items. */
void VCS_API
palTagValContainerUnInit(
    palTagValContainerHandle_t hContainer);

/* Add DWORD type Tag/Value pair. Data will be stored in little endian format */
vcsResult_t VCS_API
palTagValSetDwordProperty(
    palTagValContainerHandle_t  hContainer,
    palTag_t                    tag,
    vcsUint32_t                 value);

/* Add WORD type Tag/Value pair. Data will be stored in little endian format. */
vcsResult_t VCS_API
palTagValSetWordProperty(
    palTagValContainerHandle_t  hContainer,
    palTag_t                    tag,
    vcsUint16_t                 value);

/* Add BYTE type Tag/Value pair. */
vcsResult_t VCS_API
palTagValSetByteProperty(
    palTagValContainerHandle_t  hContainer,
    palTag_t                    tag,
    vcsUint8_t                  value);

/* Add blob data to the container. Data will be stored in little endian format,
   assuming that all elements in provided data buffer have the same type
   (all DWORDs or all WORDs) if palValTypeUint16 or palValTypeUint32 is passed
   in with 'type' parameter.
   New memory is allocated for the provided data, and data is copied. */
vcsResult_t VCS_API
palTagValSetBlobDataProperty(
    palTagValContainerHandle_t  hContainer,
    palTag_t                    tag,
    palValueType_t              type,
    vcsUint32_t                 length,
    void *                      pValue);

/* Add BYTE data to the container. No conversion is applied.
   Provided data is stored by reference, hence provided pointer should remain
   valid during container lifetime. */
vcsResult_t VCS_API
palTagValSetByteDataPropertyByRef(
    palTagValContainerHandle_t  hContainer,
    palTag_t                    tag,
    vcsUint16_t                 length,
    vcsUint8_t *                pValue);

/* Get DWORD value by specified tag. Data will be returned in host native format,
   i.e. for big endian platforms data will be returned in big endian format. */
vcsResult_t VCS_API
palTagValGetDwordProperty(
    palTagValContainerHandle_t  hContainer,
    palTag_t                    tag,
    vcsUint32_t *               pValue);

/* Get WORD value by specified tag. Data will be returned in host native format,
   i.e. for big endian platforms data will be returned in big endian. */
vcsResult_t VCS_API
palTagValGetWordProperty(
    palTagValContainerHandle_t  hContainer,
    palTag_t                    tag,
    vcsUint16_t *               pValue);

/* Get BYTE value by specified tag. */
vcsResult_t VCS_API
palTagValGetByteProperty(
    palTagValContainerHandle_t  hContainer,
    palTag_t                    tag,
    vcsUint8_t *                pValue);

/* Get blob data by specified tag. Data will be returned in host native format,
   assuming that all elements in provided data buffer have the same type
   (all DWORDs or all WORDs) if palValTypeUint16 or palValTypeUint32 is passed
   in with 'type' parameter - i.e. for big endian platforms data will be
   returned in big endian format.
   To get the resuling blob size prior to retrieving the data, the function
   should be called with 0 buffer size. */
vcsResult_t VCS_API
palTagValGetBlobDataProperty(
    palTagValContainerHandle_t  hContainer,
    palTag_t                    tag,
    palValueType_t              type,
    vcsUint32_t *               pLength,
    void *                      pValue);

/* Get blob data by specified tag. Data will be returned by reference, hence
   container should remain initialized whil pointer is in use by the caller. */
vcsResult_t VCS_API
palTagValGetByteDataPropertyByRef(
    palTagValContainerHandle_t  hContainer,
    palTag_t                    tag,
    vcsUint32_t *               pLength,
    vcsUint8_t **               ppValue);

/* Delete property by specified tag. */
vcsResult_t VCS_API
palTagValDeleteProperty(
    palTagValContainerHandle_t  hContainer,
    palTag_t                    tag);

/* Construct container based on the provided data. The data buffer should have
   been constructed with use of palGetContainerDataAsBlob() function earlier.
   Provided data buffer will be duplicated and stored inside the container. */
vcsResult_t VCS_API
palTagValSetBlobAsContainerData(
    palTagValContainerHandle_t  hContainer,
    vcsUint32_t                 length,
    void *                      pData);

/* Construct container based on the provided data. The data buffer should have
   been constructed with use of palGetContainerDataAsBlob() function earlier.
   Provided data buffer will be used by the container by reference, hence it
   should remain valid during container lifetime. */
vcsResult_t VCS_API
palTagValSetBlobAsContainerDataByRef(
    palTagValContainerHandle_t  hContainer,
    vcsUint32_t                 length,
    void *                      pData);

/* Return final data blob that can now be serialized. Data is stored in little
   endian format. This blob is opaque for caller, and can be parsed by passing
   it to palSetBlobAsContainerData() or palSetBlobAsContainerDataByRef()
   functions to be accessed by getter functions. */
vcsResult_t VCS_API
palTagValGetContainerDataAsBlob(
    palTagValContainerHandle_t  hContainer,
    vcsUint32_t *               pLength,
    void *                      pData);


/*!
*******************************************************************************
** Remove file from secure storage.
**
**
** @return
**      - VCS_RESULT_OK
**        Successfully deletes the file.
**      - VCS_RESULT_SYS_CALL_FAILED
**        System specific failure.
*/
vcsResult_t VCS_API palSecureStorageRemove(
                                           vcsString_t fileID
                                           );

/* An API - specific structure for setting the package consumer 
 * information. */
typedef struct palExtSecureWrapConsumerInfo_s
{
    vcsUint32_t     spId;            /* Service Provider ID.        */
    vcsUint8_t      *pConsumerID;    /* Consumer ID.                */
    vcsUint32_t     consumerIDLen;   /* Length of the buffer above. */

} palExtSecureWrapConsumerInfo_t;

/*!
*******************************************************************************
** Data wrapping function. In case of external wrapping (pConsumerInfo != null)
** both keys should be null.
**
** @param[in]       pInData
**      Data to be encrypted and signed.
** @param[out]      pOutData
**      Buffer containing encryped data and hash.
** @param[in]       pEncKey
**      Key used for encryption. If null in case of internal wrapping, the key
**      is retrieved from storage.
** @param[in]       pSignKey
**      Key used for signing. If null in case of internal wrapping, the key is
**      retrieved from storage.
** @param[in]       pConsumerInfo
**      Pointer to the consumer information structure. NULL if the API is called
**      for internal use only.
** @return          VCS return code, VCS_RESULT_OK if no error.
**      Error codes are:
**          VCS_RESULT_GEN_NULL_POINTER
**          VCS_RESULT_GEN_NOT_IMPLEMENTED
**          VCS_RESULT_SYS_CALL_FAILED
**          VCS_RESULT_GEN_BAD_PARAM
**          VCS_RESULT_GEN_ERROR
*/
vcsResult_t VCS_API
palSecureWrap(
    vcsBlobData_t                  * pInData,
    vcsBlobData_t                  * pOutData,
    vcsBlobData_t                  * pEncKey,
    vcsBlobData_t                  * pSignKey,
    palExtSecureWrapConsumerInfo_t * pConsumerInfo);

/*!
*******************************************************************************
** Data unwrapping function. In case of external unwrapping (pConsumerInfo !=
** null) both keys should be null.
**
** @param[in]       pInData
**      Message to be verified and decrypted.
** @param[out]      pOutData
**      Buffer containing decryped data.
** @param[in]       pEncKey
**      Key used for decryption. If null in case of internal unwrapping, the
**      key is retrieved from storage.
** @param[in]       pSignKey
**      Key used for verification. If null in case of interal unwrapping, the
**      key is retrieved from storage.
** @param[in]       pConsumerInfo
**      Pointer to the consumer information structure. NULL if the API is called
**      for internal use only.
** @return          VCS return code, VCS_RESULT_OK if no error.
**      Error codes are:
**          VCS_RESULT_GEN_NULL_POINTER
**          VCS_RESULT_GEN_NOT_IMPLEMENTED
**          VCS_RESULT_SYS_CALL_FAILED
**          VCS_RESULT_GEN_BAD_PARAM
**          VCS_RESULT_GEN_SIGNATURE_VERIFICATION_FAILED
*/
vcsResult_t VCS_API
palSecureUnwrap(
    vcsBlobData_t                  * pInData,
    vcsBlobData_t                  * pOutData,
    vcsBlobData_t                  * pEncKey,
    vcsBlobData_t                  * pSignKey,
    palExtSecureWrapConsumerInfo_t * pConsumerInfo);

/*!
*******************************************************************************
** Data wrapping function, extended version of palSecureWrap. The first
** plainTextLen bytes of input aren't encrypted. plainTextLen is saved
** in the result buffer in order to restore it in palSecureUnwrapEx.
** Signing is made on the whole buffer including plain and cipher texts
** and plainTextLen. In case of external wrapping(pConsumerInfo != null)
** both keys should be null.
**
** @param[in]       pInData
**      Data to be encrypted and signed.
** @param[out]      pOutData
**      Buffer containing encryped data and hash.
** @param[in]       pEncKey
**      Key used for encryption. If null in case of internal wrapping, only
**      signing is done, if pEncKey->pData is null, key from storage is
**      retrieved and used.
** @param[in]       pSignKey
**      Key used for signing. If null in case of internal wrapping, the key is
**      retrieved from storage.
** @param[in]       plainTextLen
**      Length of plain text.
** @param[in]       pConsumerInfo
**      Pointer to the consumer information structure. NULL if the API is called
**      for internal use only.
** @return          VCS return code, VCS_RESULT_OK if no error.
**      Error codes are:
**          VCS_RESULT_GEN_NULL_POINTER
**          VCS_RESULT_GEN_NOT_IMPLEMENTED
**          VCS_RESULT_SYS_CALL_FAILED
**          VCS_RESULT_GEN_BAD_PARAM
**          VCS_RESULT_GEN_ERROR
*/
vcsResult_t VCS_API
palSecureWrapEx(
    vcsBlobData_t                  * pInData,
    vcsBlobData_t                  * pOutData,
    vcsBlobData_t                  * pEncKey,
    vcsBlobData_t                  * pSignKey,
    vcsUint32_t                      plainTextLen,
    palExtSecureWrapConsumerInfo_t * pConsumerInfo);

/*!
*******************************************************************************
** Data unwrapping function, extended version of palSecureUnwrap. Verifies the
** input buffer, decrypts part beginning from [buffer + sizeof(iv) + plainTextLen]
** and returns [plain + decrypted] texts. In case of external unwrapping
** (pConsumerInfo != null) both keys should be null.
**
** @param[in]       pInData
**      Message to be verified and decrypted.
** @param[out]      pOutData
**      Buffer containing decryped data.
** @param[in]       pEncKey
**      Key used for decryption. If null in case of internal unwrapping, only
**      verification is done, if pEncKey->pData is null, key from storage is
**      retrieved and used.
** @param[in]       pSignKey
**      Key used for verification. If null in case of internal unwrapping, the
**      key is retrieved from storage.
** @param[in]       pConsumerInfo
**      Pointer to the consumer information structure. NULL if the API is called
**      for internal use only.
** @return          VCS return code, VCS_RESULT_OK if no error.
**      Error codes are:
**          VCS_RESULT_GEN_NULL_POINTER
**          VCS_RESULT_GEN_NOT_IMPLEMENTED
**          VCS_RESULT_SYS_CALL_FAILED
**          VCS_RESULT_GEN_BAD_PARAM
**          VCS_RESULT_GEN_SIGNATURE_VERIFICATION_FAILED
*/
vcsResult_t VCS_API
palSecureUnwrapEx(
    vcsBlobData_t                  * pInData,
    vcsBlobData_t                  * pOutData,
    vcsBlobData_t                  * pEncKey,
    vcsBlobData_t                  * pSignKey,
    palExtSecureWrapConsumerInfo_t * pConsumerInfo);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __vcsPal_h__ */
