/*! @file vcsScs.h
*******************************************************************************
**  USDK SCS Subsystem Definitions
**
**  This file contains the USDK SCS Subsystem definitions.
**
**  Copyright 2006-2012 Validity Sensors, Inc. All Rights Reserved.
*/

#ifndef __vcsScs_h__
#define __vcsScs_h__

#include "vcsPal.h"
#include "vcsDefs.h"
//#include "vcsFlexIds.h"

#include "vcsfw_v4.h"
#include "vcsSystem.h"

#include "vcsEccCurveParam.h"

#define SCS_PARAM_VALIDATE(_param, _message)    \
if (VCS_ISNULL(_param))                         \
{                                               \
    DBG_WARNING(DBG_CTX, (_message));           \
    result = VCS_RESULT_GEN_BAD_PARAM;          \
    goto cleanup;                               \
}                                               \


#define SCS_SENSOR_VALIDATE(_sensor_)           \
result = scsSensorValidate(_sensor_);           \
if (VCS_FAILURE(result))                        \
{                                               \
    goto cleanup;                               \
}                                               \

#if VCS_FEATURE_SENSOR_CPID
/* This is to store the calibration data in the host data base */
//For now enable for SSTZ
#if defined(VCS_OS_SSTZ)
#define ENABLE_CPID_CAL_DATA_STORAGE
#endif //defined(VCS_OS_SSTZ)
#define CPID_CAL_DATA_STORAGE_VERSION 1
#endif

#define VCS_STORAGE_PART_HOST_ID        1
#define VCS_STORAGE_PART_FWEXT_ID       2
#define VCS_STORAGE_PART_EFI_ID         3
#define VCS_STORAGE_PART_DB_ID          4
#define VCS_STORAGE_PART_PRINTCFG_ID    5
#define VCS_STORAGE_PART_BASELINECFG_ID 6

#define DBG_SCS_SYS                             0x00000010L
#define DBG_SCS_SERVICE_SYS                     0x00000020L
#define DBG_SCS_STARTUP_SHUTDOWN_SYS            0x00000040L
#define DBG_SCS_SENSOR_SYS                      0x00000080L
#define DBG_SCS_SENSOR_DEVICE_SYS               0x00000100L
#define DBG_SCS_DISCOVERY_SYS                   0x00000200L
#define DBG_SCS_API_SYS                         0x00000400L
#define DBG_SCS_SENSOR_INIT_SYS                 0x00000800L

/* For cfgBuilder application the lists and binary data declared in the
   configuration structures must be non-const.
   Otherwise (for USDK build)those declarations must be const.
*/
#ifdef  VCS_CONFIG_BUILDER
#define DECLARE_LIST             palListEntry_t
#define DECLARE_BYTE_POINTER     vcsUint8_t *
#else /* VCS_CONFIG_BUILDER */
#define DECLARE_LIST             palListEntryConst_t
#define DECLARE_BYTE_POINTER     const vcsUint8_t *
#endif /* VCS_CONFIG_BUILDER */

/* These definitions are made specifically so that the V3 (the lowest
   supported sensor command protocol version) value is 0. This allows
   us to use these definitions in indexing arrays (e.g. for indexed
   access to command protocol specific functions. */
#define VCS_COMMAND_PROTOCOL_UNKNOWN      0xff
#define VCS_COMMAND_PROTOCOL_VERSION_4    0x01

/* GetFingerprint parameter definitions */
#define VCS_GETPRINT_PARAM_DEFAULT                  0x00000000
#define VCS_GETPRINT_PARAM_ENCRYPT                  0x00000004  /* Indicates that the sensor data should be encrypted before sending it to the host. */
#define VCS_GETPRINT_PARAM_SIGNATURE                0x00000008  /* Indicates that the fingerprint data should be signed before sending it to the host. */
#define VCS_GETPRINT_PARAM_CONFIGREPLY              0x00000010
#define VCS_GETPRINT_PARAM_BW_REDUCTION_OFF         0x00000020  /* To not add BW reduction parameter*/
#define VCS_GETPRINT_PARAM_HWCULL                   0x00000040  /* Enable HW culling (for Windsor only) */
#define VCS_GETPRINT_PARAM_MULTIPLE_FP_BUFFERS      0x00000080  /* To indicate that more than a single FP buffer is used for the fingerprint.
                                                                   This flag is currently actual for WinCE platform, only. */
#define VCS_GETPRINT_PARAM_REQUEST_BASELINEDATA     0x00000400  /* Indicates that returned lines will be used for baseline calculation. For Eagle only. */
#define VCS_GETPRINT_PARAM_OPMODE_FULL_IR           0x00000800  /* Set operation mode to full IR (for Windsor only) */
#define VCS_GETPRINT_PARAM_OPMODE_IR_ASSIST         0x00001000  /* Set operation mode to IR assist (for Windsor/Cosworth) */
#define VCS_GETPRINT_PARAM_OPMODE_NAV_ASSIST        0x00002000  /* Set operation mode to NAV assist (for Windsor/Cosworth) */
#define VCS_GETPRINT_PARAM_CACHE                    0x00004000  /* Indicates that current command should be cached. */
#define VCS_GETPRINT_PARAM_APPEND_BASELINEDATA      0x00008000  /* Indicates that baseline data should be appended to the current command. */
#define VCS_GETPRINT_PARAM_FW_LINEAVG               0x00010000  /* Indicates that firmware will perform line averaging */
#define VCS_GETPRINT_PARAM_FD_OFF                   0x00020000  /* To not add finger detection parameters */
#define VCS_GETPRINT_PARAM_USE_WOF                  0x00040000  /* Add WOF parameter to support WOF finger detection */
#define VCS_GETPRINT_PARAM_FW_BASELINE              0x00080000  /* FW based baseline collectioni */

#define VCS_GETPRINT_PARAM_CAPTURE_WAIT_FOR_FU      0x00100000  /* Wait for finger up if finger is on the sensor at start */
#define VCS_GETPRINT_PARAM_CAPTURE_CAPTURE_TILL_FU  0x00200000  /* Keep capturing until finger up is detected */

#define VCS_GETPRINT_PARAM_AUTO_RESTART             0x00400000  /* Add AutoRestart parameter */

/* SCM WOF param is to be sent during get print only 
 * if VCS_GETPRINT_PARAM_USE_WOF is sent */
#define VCS_GETPRINT_PARAM_USE_SCM_WOF              0x00800000  /* Add SCM WOF parameter to support WOF finger detection during NAV */
#define VCS_GETPRINT_PARAM_TWO_FRAME_AVERAGE        0x01000000  /* Two Frame Average */
#define VCS_GETPRINT_PARAM_MIDQUARTILE_BASED_NORMALIZE_ENABLE  0x02000000  /* new Pre and line Middle Quartile Normalization */

#define VCS_DEFAULT_RESET_DELAY_TIME        2  /* Default delay time for internal calls to reset function. */

#define VCS_NAV_FW_ACCUM_TIME               6  /* fw nav status polling time */

#define VCS_DEFAULT_WOE_THRESHOLD   16      /* default WOE threshold, if calibration failed */
#define VCS_DEFAULT_WOE_GAIN        128     /* default WOE gain, if calibration failed */
#define VCS_MAX_WOE_THRESHOLD       63      /* maximum WOE threshold, use when sensor is in idle */
#define VCS_MAX_WOE_HIGHCOUNT       15      /* maximum WOE highcount, use when sensor is in idle */

/* The WOENF defaults below should be overridden with calibration values or defaults from
   falconCalibrationLimits.h during normal operation. The defaults below would only be used
   for catastrophic failures. These are based on Rich Nelson's calibration data and limits. CWB */
#define VCS_DEFAULT_WOENF_THRESHOLD         41      /* catastrophic default WOE-NF threshold */
#define VCS_DEFAULT_WOENF_GAIN              160     /* catastrophic default WOE-NF gain */
#define VCS_DEFAULT_WOENF_MINTHRESHOLD      3       /* catastrophic default WOE-NF minimum ratcheting threshold */
#define VCS_DEFAULT_WOENF_MAXGAIN           230     /* catastrophic default WOE-NF maximum ratcheting gain */

#define VCS_LEAST_SENSITIVE_WOENF_THRESHOLD 4       /* Least-sensitive WOE-NF threshold, for idle sensor */
#define VCS_LEAST_SENSITIVE_WOENF_GAIN      255     /* Least-sensitive WOE-NF gain, for idle sensor */

#define VCS_DEFAULT_WOVAR_THRESHOLD         0x200   /* default WOVAR threshold.*/
#define VCS_DEFAULT_WOVAR_THRESHOLD_STEP    0x010   /* Step for WOVAR ratcheting*/
#define VCS_DEFAULT_WOE_THRESHOLD_STEP      0x01    /* Step for WOE ratcheting*/

/* Maximum length of fingerprint scan line */
#define VCS_FP_MAX_LINE_SIZE                320

/* Default line length for falcon/raptor */
#define VCS_FALCON_FP_DEFAULT_LINE_SIZE     288
/* Default line length for eagle */
#define VCS_EAGLE_FP_DEFAULT_LINE_SIZE      140

/* maximum number of lines to be captured during regular swipe.*/
#define MAX_LINES_DEFAULT                   6000

/* Max size in dwords of the config reply data for Eagle/Metallica */
#define VCS_V4_MAX_CONFIG_SIZE              1024

/* Calibration type to perform */
#define VCS_CAL_NONE                0
#define VCS_CAL_ONETIME             1
#define VCS_CAL_POWERUP             2
#define VCS_CAL_PERIODIC            3
#define VCS_CAL_PRESWIPE            4
#define VCS_CAL_POSTSWIPE           5
#define VCS_CAL_PREWOE              6
#define VCS_CAL_POSTWOE             7

/* Falcon patch phase bit definitions */
#define FALCON_CONFIG_PATCH_PHASE_BOOT          0x00000001
#define FALCON_CONFIG_PATCH_PHASE_CAL           0x00000002
#define FALCON_CONFIG_PATCH_PHASE_SECMGMT       0x00000004
#define FALCON_CONFIG_PATCH_PHASE_FLASH         0x00000008
#define FALCON_CONFIG_PATCH_PHASE_MODULETEST    0x00000010
#define FALCON_CONFIG_PATCH_PHASE_LED           0x00000020
#define FALCON_CONFIG_PATCH_PHASE_PROVISION     0x00000040
#define FALCON_CONFIG_PATCH_PHASE_GETPRINT      0x00000080
#define FALCON_CONFIG_PATCH_PHASE_NAV           0x00000100
#define FALCON_CONFIG_PATCH_PHASE_SECUREMATCH   0x00000200
#define FALCON_CONFIG_PATCH_PHASE_TAKEOWNSIMUL  0x00000400
#define FALCON_CONFIG_PATCH_PHASE_SETVCKIVENG   0x00000800
#define FALCON_CONFIG_PATCH_PHASE_SETVCKIVPRD   0x00001000
#define FALCON_CONFIG_PATCH_PHASE_SPANSUSP      0x00002000
#define FALCON_CONFIG_PATCH_PHASE_PREBOOT       0x00004000
#define FALCON_CONFIG_PATCH_PHASE_KEYEXCHANGE   0x00008000
#define FALCON_CONFIG_PATCH_PHASE_ENCDEC        0x00010000
#define FALCON_CONFIG_PATCH_PHASE_WOVAR         0x00020000
#define FALCON_CONFIG_PATCH_PHASE_PRESECMGMT    0x00040000
#define FALCON_CONFIG_PATCH_PHASE_AUTH          0x00080000
#define FALCON_CONFIG_PATCH_PHASE_VCK           0x00100000
#define FALCON_CONFIG_PATCH_PHASE_SENSOR_TEST   0x00200000
#define FALCON_CONFIG_PATCH_PHASE_IMGRECON      0x00400000
#define FALCON_CONFIG_PATCH_PHASE_WOF2		0x00800000
#define FALCON_CONFIG_PATCH_PHASE_UNKNOWN       0xFFFFFFFF

#define VCS_MAX_AUTH_TOKEN_SIZE     4096

typedef struct scsSubsystem_s *     scsSubsystemHandle_t;
typedef struct scsSensor_s *        scsSensorHandle_t;
typedef vcsHandle_t                 scsSensorConfigHandle_t;

#define VCS_WOF_FINGER_STATE_UNKNOWN   0
#define VCS_WOF_FINGER_STATE_FD        1
#define VCS_WOF_FINGER_STATE_FU        2

#define VCS_WOF_FINGER_EVENT_FD        0x01
#define VCS_WOF_FINGER_EVENT_FU        0x02


/* Metallica storage/database definitions */
#define DB_USER_ID_MAX_SIZE             128
#define DB_USER_CONTAINER_ID_MAX_SIZE   128
#define DB_INFO_MAX_SIZE                (20 * sizeof(vcsfw_db_user_container_entry_t))
#define DB_OBJECT_INFO_MAX_SIZE         (50 * sizeof(vcsfw_db_object_entry_t))
#define DB_OBJECT_DATA_MAX_SIZE         (16 * 1024)
#define DB_USER_INFO_TEMPLATE_MAX_SIZE  (20 * sizeof(vcsfw_db_template_entry_t))
#define DB_USER_INFO_OBJECT_MAX_SIZE    (20 * sizeof(vcsfw_reply_db_user_info_get_t))
#define DB_SYSTEM_PARAM_MAX_SIZE        512
#define DB_CONTAINER_INFO_MAX_SIZE      (20 * sizeof(vcsfw_db_user_entry_t))
#define STORAGE_INFO_MAX_SIZE           (10 * sizeof(vcsfw_storage_part_entry_t))
#define STORAGE_FW_PART_INFO_MAX_SIZE   (20 * sizeof(vcsfw_storage_fwpart_module_t))
#define DB_TUID_MAX_SIZE                32

/* Default name for User Container object */
#define USER_CONTAINER_ID               VCS_STRING("MetallicaStorage")

/**
* Authentication session status polling period timeout.
*/
#define AUTH_SESSION_STATUS_POLLING_TIMEOUT 50

/* Autonomous mode session states. */
/* FIXME. The definitions should be shared between vfm and scs, as there are
duplicate definitions in vfm also. */
#define AUTONOMOUS_MODE_STATE_UNDEFINED         0
#define AUTONOMOUS_MODE_STATE_NOT_AUTHENTICATED 1
#define AUTONOMOUS_MODE_STATE_AUTHENTICATING    2
#define AUTONOMOUS_MODE_STATE_AUTHENTICATED     3
#define AUTONOMOUS_MODE_STATE_ERROR             4

/**********************************************
* SCS event listening Functions and types     *
**********************************************/
typedef void (* scsEventListenCB_t)(vcsUint32_t eventType, vcsUint32_t status, vcsBlobData_t * pEventData, void * pUserData);

#define SCS_CALLBACK_EVENT_CAPTURE_STATUS           0x01
#define SCS_CALLBACK_EVENT_MOC_STATUS               0x02
#define SCS_CALLBACK_EVENT_EOC_STATUS               0x03

/* event data of SCS_CALLBACK_EVENT_CAPTURE_STATUS event is vcsUint8_t type data
   which indicates finger status described below */
#define SCS_FP_STATUS_NONE      0
#define SCS_FP_STATUS_ABSENT    1
#define SCS_FP_STATUS_STILL     2
#define SCS_FP_STATUS_MOVING    3

/* Note, that in case if SCS_CALLBACK_EVENT_CAPTURE_STATUS event data value is
SCS_FP_STATUS_MOVING then it will be followed by vcsUint16_t type
data which indicates image reconstruction status described below. */
#define SCS_EVENT_CAPTURE_STATUS_NONE                       ((vcsUint16_t) 0)
#define SCS_EVENT_CAPTURE_STATUS_FINGER_DETECTED            ((vcsUint16_t) 1)
#define SCS_EVENT_CAPTURE_STATUS_IN_PROGRESS                ((vcsUint16_t) 2)
#define SCS_EVENT_CAPTURE_STATUS_FINGER_SETTLED             ((vcsUint16_t) 3)
#define SCS_EVENT_CAPTURE_STATUS_IMAGE_READY                ((vcsUint16_t) 4)
#define SCS_EVENT_CAPTURE_STATUS_FINGER_ON_SENSOR           ((vcsUint16_t) 5)
#define SCS_EVENT_CAPTURE_STATUS_FINGER_REMOVED             ((vcsUint16_t) 6)
#define SCS_EVENT_CAPTURE_STATUS_IMAGE_ACQUISITION_NO_IMAGE ((vcsUint16_t) 7)
#define SCS_EVENT_CAPTURE_STATUS_CAPTURE_ERROR              ((vcsUint16_t) 8)
#define SCS_EVENT_CAPTURE_STATUS_FRAME_READY                ((vcsUint16_t) 9)

/* event data of SCS_CALLBACK_EVENT_MOC_STATUS event is vcsUint16_t type data
   which indicates MOC status described below */
#define SCS_EVENT_MOC_STATUS_NONE           ((vcsUint16_t) 0)
#define SCS_EVENT_MOC_STATUS_IN_PROGRESS    ((vcsUint16_t) 1)
#define SCS_EVENT_MOC_STATUS_COMPLETE       ((vcsUint16_t) 2)

/* event data of SCS_CALLBACK_EVENT_EOC_STATUS event is vcsUint16_t type data
   which indicates EOC status described below */
#define SCS_EVENT_EOC_STATUS_NONE           ((vcsUint16_t) 0)
#define SCS_EVENT_EOC_STATUS_IN_PROGRESS    ((vcsUint16_t) 1)
#define SCS_EVENT_EOC_STATUS_VIEW_READY     ((vcsUint16_t) 2)
#define SCS_EVENT_EOC_STATUS_READY_NEXT     ((vcsUint16_t) 3)
#define SCS_EVENT_EOC_STATUS_FAILURE        ((vcsUint16_t) 4)
#define SCS_EVENT_EOC_STATUS_COMPLETE       ((vcsUint16_t) 5)

/* Secure wrap maximum data length */
#define SECURE_WRAP_MAX_DATA_LEN        1024

/* Secure wrap maximum data padding size */
#define SECURE_WRAP_MAX_DATA_PADDING    64

/* Partition programming maximum chunk size. */
#define SCS_PART_WRITE_CHUNK_SIZE     (4 * 1024)

/* Application request maximum data length. */
#define SCS_APP_REQUEST_MAX_DATA_LEN  (3 * 1024)

/* scs sensor close operation control flags. */
#define SCS_DEVICE_CLOSE_SKIP_TLS_CLOSURE   0x00000001

/* [TG] FIXME: Remove FW definitions from header file. */
/* Enroll session status debug info data size. */
#define SCS_ENROLL_SESSION_STATUS_DBG_INFO_DATA_SIZE VCSFW_ENROLL_SESSION_STATUS_DBG_INFO_DATA_SIZE

/* As we cant use the macros from vfm, redeclared those for scs functions */
#define SCS_POWER_UP_BASELINE       0
#define SCS_PRE_PRINT_BASELINE      1
#define SCS_PROCESSED_BASELINE      2


/* SCS Async API common definitions. */
#define SCS_ASYNC_OPERATION_BUSY        ((vcsUint32_t) 0)
#define SCS_ASYNC_OPERATION_COMPLETE    ((vcsUint32_t) 1)

typedef struct scsAsyncOperationCtx_s
{
    vcsUint32_t status;
    vcsUint32_t suggestedPollTime;
    vcsUint32_t progress;
    vcsUint32_t reserved;
} scsAsyncOperationCtx_t;

typedef enum scsBaselineStatus_e
{
    scsBaselineStatusNotVerified = 0,
    scsBaselineStatusValid,
    scsBaselineStatusInvalid
} scsBaselineStatus_t;

/******************************************************************************
 * This structure is used to hold information about specific baseline data.
 *****************************************************************************/
typedef struct scsBaselineData_s
{
    scsBaselineStatus_t status;
    vcsUint32_t         lineLength;
    vcsUint32_t         lineNumber;
    vcsUint8_t *        pBaseLineData;
    /* used for HW baseline during finger detect for touch, only 1 line of data */
    vcsUint8_t *        pHwBaselineData;
} scsBaselineData_t;

/******************************************************************************
 * This structure is used to hold entire baseline data information (both
   power-up and pre-print) for the sensor.
 *****************************************************************************/
typedef struct scsSensorBaselineInfo_s
{
    scsBaselineData_t prePrintBaseline;  /* Received during sensor initialization. */
    scsBaselineData_t powerUpBaseline;   /* Received during GetPrint operation
                                            before unlimited GetPrint */

    scsBaselineData_t processedBaseline; /* Used by IR during reconstruction.
                                            Calculated based on power-up or
                                            pre-print data (if power-up is invalid).*/
} scsSensorBaselineInfo_t;
typedef scsSensorBaselineInfo_t* scsSensorBaselineInfoHandle_t;

typedef struct scsBitReductionData_s
{
    vcsUint32_t bBitReductionEnabled;
    vcsUint32_t bitReductionLinePeriod;
} scsBitReductionData_t;

/* Calibration results
 * This structure is OBSOLETE. Use scsCalibrationResult_t instead */
#define NUM_CAL 7
typedef struct CalibrationResult_s
{
    vcsBool_t   isDefault;              /* is default calibration data used? */
    vcsUint32_t GetPrintPhaseStatus;    /* is GetPrint phase failed? */
    vcsUint32_t rc[NUM_CAL];            /* calibration return codes */
} CalibrationResult_t;

/* Swipe direction mode.
 * Indicates which line any portion of finger is
   go over first, primary or secondary */
typedef enum vcsSwipeDirection_e
{
    SwipeDirPriFirst = 0,
    SwipeDirSecFirst = 1,
    SwipeDirAuto = 2,
    SwipeDirUpDownLeftRight = 3
} vcsSwipeDirection_t;

#include "vcsPushPack1.h"

/*
** This structure holds information on the sensor firmware version.
*/
typedef VCS_PACKED struct scsSensorFwVersion_s
{
    vcsUint32_t   buildtime;
    vcsUint32_t   buildnum;         /* build number                        */
    vcsUint8_t    vmajor;
    vcsUint8_t    vminor;
    vcsUint8_t    target;           /* target, e.g. VCSFW_TARGET_ASIC      */
    vcsUint8_t    product;          /* product, e.g.  VCSFW_PRODUCT_FALCON */
    vcsUint8_t    siliconrev;       /* silicon revision                    */
    vcsUint8_t    formalrel;        /* boolean: non-zero -> formal release */
    vcsUint8_t    platform;         /* Platform (PCB) revision             */
    vcsUint8_t    patch;
    vcsUint8_t    serial_number[6]; /* 48-bit Serial Number. Present for Falcon only.*/
    vcsUint8_t    security[2];
    vcsUint8_t    device_type;      /* device type: cmd protocol and security state  */
    vcsUint16_t   otpspare1;        /* spare space                         */
} scsSensorFwVersion_t;

/*
** This structure holds information on the sensor FW extension version.
*/
typedef VCS_PACKED struct scsSensorFwExtVersion_s
{
    vcsUint16_t vMajor;     /* Major version of the FW extension */
    vcsUint16_t vMinor;     /* Minor version of the FW extension */
    vcsUint16_t type;       /* Type of the FW extension */
    vcsUint16_t buildNum;   /* Build number of the FW extension */
    vcsUint32_t buildTime;  /* Build time of the FW extension in seconds
                               from 1/1/1970 12:00 AM GMT */
} scsSensorFwExtVersion_t;

#include "vcsPopPack.h"

typedef struct scsSensorDriverVersion_s
{
    vcsUint32_t major;
    vcsUint32_t minor;
    vcsUint32_t subminor;
    vcsUint32_t build;
    vcsUint16_t idProduct;
    vcsUint16_t idVendor;
    vcsUint8_t  communication;
} scsSensorDriverVersion_t;

#define MAX_TIME_SLOT 352

/*Structure to cache GetPrint command to be used during reconstruction*/
typedef struct scsSensorGetPrintCmd_s
{
    vcsUint32_t   lineLength;
    vcsUint32_t   swipeSpeed;           /* swipe speed in cm/s */
    vcsUint32_t   linePeriod;           /* line period in microsecond per line */
    vcsUint32_t   SOClock;
    vcsUint32_t   bitDepth;
    vcsUint8_t  * pGetPrintBlob;        /* a copy of the GetPrint command (including line length, number of lines and parameter block blob)sent to the sensor*/
    vcsUint32_t   GetPrintBlobSize;
} scsSensorGetPrintCmd_t;

/* dimension of frame acquired */
typedef struct scsSensorFrameDimension_s
{
    vcsUint16_t bitdepth;         /* bit depth of a pixel - 8 or 16 */
    vcsUint16_t frameHdrLen;      /* number of bytes of frame header */
    vcsUint16_t frameLineTotal;   /* total number of lines per frame */
    vcsUint16_t frameLineSkip;    /* lines to skip at beginning of frame */
    vcsUint16_t frameLineNum;     /* usable lines in a frame */
    vcsUint16_t lineHdrLen;       /* number of bytes of line header */
    vcsUint16_t linePixTotal;     /* total number of pixels in a line */
    vcsUint16_t linePixSkip;      /* pixels to skip at beginning of line */
    vcsUint16_t linePixNum;       /* usable pixels in a line */
} scsSensorFrameDimension_t;

#define NASSAU_LINE_LENGTH(dim) \
    (dim.lineHdrLen + (((dim.linePixTotal * dim.bitdepth + 31) / 8) & (~3)))

#define NASSAU_LINE_LENGTH_NO_ALIGN(dim) \
    (dim.lineHdrLen + (((dim.linePixTotal * dim.bitdepth + 7) / 8)))

#define NASSAU_FRAME_SIZE(dim) \
    (dim.frameHdrLen + (dim.frameLineTotal * NASSAU_LINE_LENGTH(dim)))

#define NASSAU_LINE_CROP_LENGTH(dim) \
    (dim.lineHdrLen + (((dim.linePixNum * dim.bitdepth + 31) / 8) & (~3)))

#define NASSAU_FRAME_CROP_SIZE(dim) \
    (dim.frameHdrLen + (dim.frameLineNum * NASSAU_LINE_CROP_LENGTH(dim)))


/* max size of iota data returned */
#define MAX_IOTA_READ_SIZE      (32 * 1024)

/* Temp Correction Factor*/
typedef struct scsSensorTempCorrectionFactor_s
{
    vcsUint16_t     tempcorrect60;      /* temperature calibration @ 60 degC */
    vcsUint16_t     tempcorrect25;      /* temperature calibration @ 25 degC */
} scsSensorTempCorrectionFactor_t;


typedef struct VCS_PACKED scsSensorframeTagExtfpsSundryregsRegset_s {
    vcsUint16_t     shutterOfLiveCapturedImage;
    vcsUint16_t     vblank;
    vcsUint16_t     gain;
    vcsUint16_t     firmwareSunSenseDecision; /* 1 is sunlight. 0 is normal light*/
    vcsUint16_t     shutterOfCalibrationImage;    
} scsSensorframeTagExtfpsSundryregsRegset_t;

/******************************************************************************
 * This structure contains information about calibration process. It is common
 * for all sensors. This structure should be used only internally in USDK
 * (between USDK layers), and should not be used by other Tools, Applications.
 *****************************************************************************/
typedef struct scsCalibrationResult_s
{
    vcsBool_t       isDefault;              /* is default calibration data used? */
    vcsUint32_t     getPrintPhaseStatus;    /* is GetPrint phase failed? */
    vcsUint32_t *   rc;                     /* Calibration Return Codes */
    vcsUint32_t     rcNumber;               /* Number of Return Codes */
} scsCalibrationResult_t;

/******************************************************************************
 * This structure contains all information about GetPrint operation, including
 * information about sensor on which GetPrint is acquired, configuration data
 * (Time Slot and Registry) used for GetPrint, etc.
 * This structure is common for all sensors.
 * This structure should be used only internally in USDK
 * (between USDK layers), and should not be used by other Tools, Applications.
 *****************************************************************************/
typedef struct scsSensorPrintConfigurationData_s
{
    vcsUint32_t                 time;                   /* Unix time_t format, unsigned, UTC of Print */
    vcsUint32_t                 flexId;                 /* Sensor flex id */
    vcsUint32_t                 interfaceType;          /* Sensor Interface type, USB/SPI/Parallel, etc. */
    vcsUint32_t                 vid;                    /* Sensor VID */
    vcsUint32_t                 pid;                    /* Sensor PID */
    scsSensorFwVersion_t *      pFwVersion;             /* Sensor Firmware Version */
    vcsBlobData_t               timeSlotTable;          /* Time Slot Table */
    vcsBlobData_t               configReply;            /* GetPrint ConfigReply Blob */
    scsSensorGetPrintCmd_t      getPrintCmdBlob;        /* Cached GetPrint Command Sent to the Sensor */
    scsSensorBaselineInfo_t     baselineInfo;           /* Information About Sensor Baseline */
    void *                      pCalResults;            /* Calibration Results */
    scsBitReductionData_t       bitReductionData;       /* Information About Bit Reduction */
    vcsSwipeDirection_t         swipeDirection;         /* Swipe Direction */
    vcsUint32_t                 liveBaselineVariance;   /* baselineVariance used in live print capture */
    scsSensorFrameDimension_t   frameDim;               /* dimension of acquired frame */
} scsSensorPrintConfigurationData_t;

/* Structure to store SensorCommand reply */
typedef vcsBlobData_t vcsSensorCommandReplyData_t;

/* FIXME bjf - It would be nice if the following types came out of the firmware
               header file. */
typedef vcsInt32_t  scsSensorStatus_t;
typedef vcsUint16_t scsSensorParameterId_t;

#define VCS_VERSION_SIZE                80
typedef vcsChar_t   scsSensorVersion_t[VCS_VERSION_SIZE];

/* Types for Sensor Product Families */
typedef enum vcsProductFamily_e
{
    vcsProductUnknown = 0,  /* Device not supported yet by USDK */
    vcsProductWindsor,      /* Windsor FW-based sensor */
    vcsProductNassau,       /* Nassau FW-based sensor */
    vcsProductMax
} vcsProductFamily_t;

typedef vcsUint32_t scsPid_t;

/* Used to bound sensor PID with sensor Product Family */
typedef struct scsPidFamilyPairEntry_s
{
    scsPid_t            pid;
    vcsProductFamily_t  family;
} scsPidFamilyPairEntry_t;

/* Used to bind sensor product type returned by GetVersion command
   with sensor Product Family */
typedef struct scsProductTypeFamilyPairEntry_s
{
    vcsUint8_t          type;
    vcsProductFamily_t  family;
} scsProductTypeFamilyPairEntry_t;

/* Sensor configuration types */
typedef enum scsConfigType_e
{
    scsPatchConfig = 0,
    scsPrintConfig,
    scsCalConfig,
#if VCS_FEATURE_NAVIGATION_USDK5
    scsNavConfig,
#endif /* VCS_FEATURE_NAVIGATION_USDK5 */
    scsWoeConfig,
    scsMaxConfig
} scsConfigType_t;

typedef vcsResult_t (VCS_API * scsGenericCommandsCB_t)(vcsSensorCommandReplyData_t * pV, vcsResult_t result, void * pUserData);
typedef vcsResult_t (VCS_API * scsSensorGenericCB_t)(void * pV, vcsResult_t result, void * pUserData);
typedef vcsResult_t (VCS_API * scsSensorGenericFpDataCB_t)(void * pUserData, vcsUint32_t status, vcsUint8_t * pData, vcsUint32_t dataSize);

/* FIXME [KY] - this staff need to be moved to somewhere else...*/
/* Flash device info */
#define VCS_PART_NUMBER_MAX_LEN 20

typedef struct FlashMemoryInfo_s
{
    vcsChar_t   partNumber[VCS_PART_NUMBER_MAX_LEN];
    vcsUint32_t capacity;
    vcsUint8_t  signature;
    vcsUint8_t  mfgID;
    vcsUint8_t  memoryType;
    vcsUint8_t  memoryCapacity;
    vcsInt32_t  sectorSize;
    vcsUint32_t erasableUnitSize;
    vcsUint8_t  unitEraseCommand;
    vcsUint16_t unitEraseCommandSize;
} FlashMemoryInfo_t;

/* Used to store sensor configuration parameters like flex ID,
SO clock, swipe speed, etc. */
typedef struct scsSensorConfigInfo_s
{
    vcsUint32_t flexId;         /* The Flex ID for the sensor device. */
    vcsUint32_t SOClock;        /* SOClock to use */
    vcsUint32_t swipeSpeed;     /* swipe speed to use */
    vcsUint32_t bitDepth;       /* bitDepth to use */
    vcsUint32_t projId;         /* ProjectId of the sensor */    
} scsSensorConfigInfo_t;

typedef struct scsTakeownFlashInfo_s
{
    vcsUint32_t flashSize;
    vcsUint32_t erasableUnitSize;
    vcsUint16_t dummy;
    vcsUint8_t  eraseCmdId;
    vcsUint8_t  readMode;
} scsTakeownFlashInfo_t;

/************************************************************************/
/*                                                                        */
/*                          SCS COMMAND STRUCTURES                           */
/*                                                                        */
/************************************************************************/

/* SensorReset */
typedef struct scsCmdReset_s
{
    vcsUint16_t delayTime;      /* Delay in ms after which sensor will be reset. */
} scsCmdReset_t;


#define GETPRINT_PARAM_ENTRY_ALLOCATED  0x0001  /* Parameter data is allocated and need to be freed */

/* GetFingerprint parameter entry */
typedef struct scsGetFingerprintParamEntry_s
{
    palListEntry_t          list;
    vcsUint16_t             flags;
    vcsUint16_t             paramSize;          /* Size of the GetFingerprint parameter data */
    vcsUint8_t *            pParam;             /* GetFingerprint parameter data to send */
} scsGetFingerprintParamEntry_t;

/* GetFingerprint */
typedef struct scsCmdGetFingerprint_s
{
    vcsUint16_t             numScanLines;       /* Maximum number of scan lines to obtain. Acceptable values are from 0-65535. */
    vcsUint32_t             captureMode;        /* Indicates whether the sensor should wait until finger detected of should immediately start sending scan data. */
    vcsUint32_t             flags;              /* GetFingerprint parameters bitfield. */
    vcsUint16_t             lineLength;         /* Length of each scan line */
    vcsUint32_t             swipeSpeed;         /* swipe speed in cm/s */
    vcsUint32_t             linePeriod;         /* line period in microsecond per line */
    vcsUint32_t             SOClock;            /* SO clock frequency */
    vcsUint32_t             bitDepth;           /* pixel bit-depth */
    vcsUint32_t             baselineLen;        /* length of baseline data */
    palListEntry_t          getprintParamlist;  /* list of getprint parameter entries to send */
} scsCmdGetFingerprint_t;

/* DownloadBootLoaderPatch */
typedef struct scsCmdDownloadBootLoaderPatch_s
{
    vcsUint32_t         length;     /* Length of the path data in bytes. */
    const vcsUint8_t *  pData;      /* Patch data buffer to be sent to the sensor. */
} scsCmdDownloadBootLoaderPatch_t;

/* DownloadPatch */
typedef struct scsCmdDownloadPatch_s
{
    vcsUint16_t         address;    /* Address where the patch data is to be written. */
    vcsUint32_t         length;     /* Length of the path data in bytes. */
    const vcsUint8_t *  pData;      /* Patch data buffer to be sent to the sensor. */
} scsCmdDownloadPatch_t;

/* GPIO */
typedef struct GPIORegMask_s
{
    union
    {
        vcsUint32_t newval;     /* ??? */
        vcsUint8_t  setmask;    /* ??? */
    }set_u; /* unions must be named to avoid compile errors */
    union
    {
        vcsUint32_t mask;       /* ??? */
        vcsUint8_t  clrmask;    /* ??? */
    }clr_u; /* unions must be named to avoid compile errors */
} GPIORegMask_t;

typedef struct scsCmdSGPIO_s
{
    GPIORegMask_t   alt;        /* ??? */
    GPIORegMask_t   dir;        /* ??? */
    GPIORegMask_t   reserved;   /* ??? */
    GPIORegMask_t   dout;       /* ??? */
    GPIORegMask_t   wpu;        /* ??? */
    GPIORegMask_t   hdrv;       /* ??? */
    GPIORegMask_t   alts;       /* ??? */
    GPIORegMask_t   ien;        /* ??? */
    GPIORegMask_t   pdr;        /* ??? */
} scsCmdGPIO_t;

/* ResetOwnership */
typedef struct scsCmdResetOwnership_s
{
    vcsUint16_t     passwordSize;   /* Password length */
    vcsUint8_t *    pPassword;      /* Password buffer */
} scsCmdResetOwnership_t;

/* Peek */
typedef struct scsCmdPeek_s
{
    vcsUint32_t address;    /* Address where the value is to be obtained. */
    vcsUint8_t  numBytes;   /* Length of the value that is to be obtained, in bytes. Valid values are 1, 2 or 4. */
} scsCmdPeek_t;

/* Poke */
typedef struct scsCmdPoke_s
{
    vcsUint32_t address;    /* Address where the value is to be written. */
    vcsUint8_t  numBytes;   /* Length of the value that is to be written, in bytes. Valid values are 1, 2 or 4. */
    vcsUint32_t value;      /* Value to be written. */
} scsCmdPoke_t;

/* AnyCmd */
/* TODO: Review this in the future */
typedef struct scsCmdAny_s
{
    void *      pCommand;   /* Blob data buffer to send to the sensor. */
    vcsUint32_t cmdSize;    /* Buffer length */
} scsCmdAny_t;
/* Reply structure not needed */

typedef struct scsCmdTakeOwnershipWithKeysEx_s
{/*FIXME: [KY] - add customer injected staff*/
    vcsUint8_t *    pStorageInfo;       /* Storage params */
    vcsUint8_t *    pPartTable;         /* Partition table image */
    vcsUint8_t *    pHAPubKey;          /* Host authentication public key */
    vcsUint8_t *    pHAPubKeySign;      /* Host authentication public key signature */
    vcsUint32_t     nHAPubKeySignLen;   /* Host authentication public key signature length. */
    vcsUint8_t *    pEccCurve;
    vcsBool_t       bIsECC;
} scsCmdTakeOwnershipWithKeysEx_t;

/* GetParam */
typedef struct scsCmdGetParam_s
{
    scsSensorParameterId_t paramId; /* The parameter identifier for the parameter the value is to be obtained. */
} scsCmdGetParam_t;

/* SetParam */
typedef struct scsCmdSetParam_s
{
    scsSensorParameterId_t  paramId;    /* The parameter identifier for the parameter the value is to be obtained. */
    vcsUint16_t             value;      /* The value of the parameter. */
} scsCmdSetParam_t;

/* IspRequest */
typedef struct scsCmdIspRequest_s
{
    vcsUint8_t request; /* The ISP request to go down to the sensor. */
} scsCmdIspRequest_t;

/* CallFunction */
typedef struct scsCmdCallFunction_s
{
    vcsUint32_t address; /* Address where to jump to. */
} scsCmdCallFunction_t;

/* SetLEDStateEx2 */
typedef struct ledEx2Led_s
{
    vcsUint8_t  startbrightness;    /* Starting Brightness of the state (PWM_LED)   */
    vcsUint8_t  maxbrightness;      /* 0-255 (WOE_LED)                              */
    vcsUint16_t reserved1;          /* Reserved for future use                      */
    vcsUint16_t reserved2;          /* Reserved for future use                      */
} ledEx2Led_t;

typedef struct ledEx2State_s
{
    vcsUint16_t dwelltime;                  /* Specifies how long to run this state, in ms   */
    vcsUint16_t reserved;                   /* Reserved for future use                       */
    vcsUint8_t  active;                     /* When is this state in use?                    */
    vcsUint8_t  startbrightness;            /* Starting Brightness of the state (PWM_BRIGHT) */
    vcsInt8_t   step;                       /* Signed incremental step for startbrightness   */
    vcsUint8_t  steptime;                   /* How often (in ms) to step startbrightness     */
    ledEx2Led_t led[VCSFW_LED_EX2_LEDS];    /* Settings for the three LEDs in this state     */
} ledEx2State_t;

typedef struct scsCmdSetLEDStateEx2_s
{
    vcsUint32_t dwelltime;                      /* How long to run this command (in ms) */
    ledEx2State_t state[VCSFW_LED_EX2_STATES];  /* Description of each active state     */
} scsCmdSetLEDStateEx2_t;

typedef vcsUint32_t scsCommandId_t;

/* StoragePartFormat */
typedef struct scsCmdStoragePartFormat_s
{
   vcsUint8_t partid;            /* Partition ID of the partition to format */
} scsCmdStoragePartFormat_t;

/* StoragePartRead */
typedef struct scsCmdStoragePartRead_s
{
    vcsUint8_t  partid;         /* Partition ID of the partition to format */
    vcsUint8_t  mode;           /* Read mode: 0 - Default, 1 - Raw */
    vcsUint16_t reserved;       /* Unused */
    vcsUint32_t offset;         /* Offset from the beginning of the partition */
    vcsUint32_t size;           /* Number of bytes to read */
} scsCmdStoragePartRead_t;

/* StoragePartWrite */
typedef struct scsCmdStoragePartWrite_s
{
    vcsUint8_t      partid;         /* Partition ID of the partition to format */
    vcsUint8_t      mode;           /* Read mode: 0 - Default, 1 - Raw */
    vcsUint16_t     reserved;       /* Unused */
    vcsUint32_t     offset;         /* Offset from the beginning of the partition */
    vcsUint32_t     size;           /* Number of bytes to write */
    vcsUint8_t *    pWriteData;
} scsCmdStoragePartWrite_t;

/* StorageFWPartValidate */
typedef struct scsCmdStorageFWPartValidate_s
{
    vcsUint8_t      partid;         /* Partition ID of the FW Extension partition */
    vcsUint8_t      signtype;       /* Type of signature */
    vcsUint16_t     signsize;       /* Size of the signature */
    vcsUint8_t *    pSignatureData; /* Signature Data */
} scsCmdStorageFWPartValidate_t;

/* StorageFWPartInfoGet */
typedef struct scsCmdStorageFWPartInfoGet_s
{
    vcsUint8_t partid;       /* Partition ID of the FW Extension partition */
} scsCmdStorageFWPartInfoGet_t;

/* DbObjectInfoGet */
typedef struct scsCmdDbObjectInfoGet_s
{
   vcsUint32_t objectid;     /* Object id of the object to retrieve */
} scsCmdDbObjectInfoGet_t;

/* DbObjectCreate */
typedef struct scsCmdDbObjectCreateUserObject_s
{
    vcsUint8_t *    pUUID;          /* uuid data */
} scsCmdDbObjectCreateUserObject_t;

typedef struct scsCmdDbObjectCreateTemplateObject_s
{
    vcsUint16_t     templateId;     /* template ID */
    vcsUint16_t     attrib;         /* attribute of the template */
    vcsUint16_t     templateSize;   /* size of the template data */
    vcsUint16_t     pekSize;        /* size of the payload encryption key (PEK) */
    vcsUint8_t *    pTemplateData;  /* template data*/
    vcsUint8_t *    pPekData;       /* pek data */
} scsCmdDbObjectCreateTemplateObject_t;

typedef struct scsCmdDbObjectCreateAuthObject_s
{
    vcsUint16_t     datatype;       /* type of authentication data */
    vcsUint16_t     datasize;       /* size of the authentication data blob */
    vcsUint8_t *    pAuthData;      /* authentication data */
} scsCmdDbObjectCreateAuthObject_t;

typedef struct scsCmdDbObjectCreateGenericObject_s
{
    vcsUint16_t     datatype;       /* type of generic data */
    vcsUint16_t     datasize;       /* size of the generic data blob */
    vcsUint8_t *    pGenericData;   /* generic data */
} scsCmdDbObjectCreateGenericObject_t;

typedef struct scsCmdDbObjectCreate_s
{
    vcsUint32_t     parentid;       /* Object id of the parent object */
    vcsUint16_t     type;           /* object type */
    vcsUint16_t     attrib;         /* attribute of the object */
    vcsUint32_t     size;           /* size of the object data in bytes */
    vcsUint8_t *    pObjectData;
} scsCmdDbObjectCreate_t;

/* DbObjectDelete */
typedef struct scsCmdDbObjectDelete_s
{
   vcsUint32_t objectid;  /* Object id of the object */
} scsCmdDbObjectDelete_t;

/* DbObjectDataGet */
typedef struct scsCmdDbObjectDataGet_s
{
  vcsUint32_t objectid;   /* Object id of the object */
} scsCmdDbObjectDataGet_t;

/* DbUserInfoGet */
typedef struct scsCmdDbUserInfoGet_s
{
    vcsUint32_t     objectid;   /* Object id of the user object */
    vcsUint32_t     usercontid; /* Object id of the user container object */
    vcsUint32_t     uuidsize;   /* size of the UUID data follow */
    vcsUint8_t *    pUUIDData;
} scsCmdDbUserInfoGet_t;

/* DbObjectRestore */
typedef struct scsCmdDbObjectRestore_s
{
    vcsUint32_t         size;       /* size of data to restore */
    vcsUint16_t         parentid;   /* Object id of the parent object */
    vcsUint16_t         reserved;   /* reserved */
    vcsUint8_t *        pRestoreData;
} scsCmdDbObjectRestore_t;

/* DbSystemParamGet */
typedef struct scsCmdDbSystemParamGet_s
{
   vcsString_t sName;
   vcsUint16_t namesize;   /* size of the parameter name in bytes */
} scsCmdDbSystemParamGet_t;

/* DbSystemParamSet */
typedef struct scsCmdDbSystemParamSet_s
{
   vcsUint16_t namesize;   /* size of the parameter name in bytes */
   vcsUint16_t valuesize;  /* size of the parameter value in bytes */
   vcsString_t sParamName;
   vcsString_t sParamValue;
} scsCmdDbSystemParamSet_t;

/* DbContainerInfoGet */
typedef struct scsCmdDbContainerInfoGet_s
{
    vcsUint32_t     objectid;   /* Object id of the user container object */
    vcsUint32_t     ucuidsize;  /* size of the UCUID data follow */
    vcsUint8_t *    pUCUID;
} scsCmdDbContainerInfoGet_t;

/* ReadFpData */
typedef struct scsCmdReadFpData_s
{
    vcsUint32_t dataSize;
} scsCmdReadFpData_t;

/* AuthSessionStart */
typedef struct scsCmdAuthSessionStart_s
{
    vcsUint8_t  authType;     /* Authentication type: SM, MOC, POA, etc.   */
    vcsUint32_t timeout;      /* Session timeout in seconds.               */
    vcsUint32_t ucObjId;      /* User object container ID.                 */
    vcsUint32_t usrObjId;     /* User object ID.                           */
    vcsUint16_t authDataType; /* Aka Authentication algorithm ID - AlgID.  */
    vcsUint32_t addDataLen;   /* Additional data length. This may be required
                                * for POA mode. */
    vcsUint8_t * pAddData;    /* Additional data. */
} scsCmdAuthSessionStart_t;

/* AuthSessionFinish */
typedef struct scsCmdAuthSessionFinish_s
{
    vcsUint32_t sessionId;     /* Session ID. */
} scsCmdAuthSessionFinish_t;

/* AuthSessionStatusGet */
typedef struct scsCmdAuthSessionStatusGet_s
{
    vcsUint32_t sessionId;     /* Session ID. */
    vcsBool_t   bOpTypePoa;    /* Operation type is POA. */
} scsCmdAuthSessionStatusGet_t;

/* AuthSessionResultGet */
typedef struct scsCmdAuthSessionResultGet_s
{
    vcsUint32_t sessionId;     /* Session ID. */
} scsCmdAuthSessionResultGet_t;

/* AuthSessionDataGet */
typedef struct scsCmdAuthSessionDataGet_s
{
    vcsUint32_t sessionId;      /* Session ID. */
    vcsUint16_t authDataType;   /* aka Authentication algorithm ID - AlgID.  */
    vcsUint16_t paramDataSize;  /* This can be a "Challenge" data in case of
                                 * Challenge-Response authentication. */
    vcsUint8_t * pParamData;    /* Param data is appended to the command. */
} scsCmdAuthSessionDataGet_t;

/* EnrollSessionStart */
typedef struct scsCmdEnrollSessionStart_s
{
    vcsUint32_t sessionId;  /* Session Id. */
    vcsUint32_t flags;      /* Enrollment control flags. */
} scsCmdEnrollSessionStart_t;

/* EnrollSessionFinish */
typedef struct scsCmdEnrollSessionFinish_s
{
    vcsBool_t   bEraseSessionCtx;  /* Flag to erase the session context. */
} scsCmdEnrollSessionFinish_t;

/* TemplateUpdate */
typedef struct scsCmdTemplateUpdate_s
{
    vcsUint16_t  templDataSize; /* Size of template data. */
    vcsUint8_t * pTemplData;    /* Template data. */
} scsCmdTemplateUpdate_t;

typedef struct scsCmdSetIdleTimer_s
{
    vcsUint8_t Timeout[2]; /* Idle timer timeout  */
} scsCmdSetIdleTimer_t;

/* SecureWrap */
typedef struct scsCmdSecureWrap_s
{
    vcsUint16_t     flags;
    vcsUint16_t     dataLength;
    vcsUint8_t *    pData;
} scsCmdSecureWrap_t;

#define VCS_MAX_OTPROM_SIZE (4096)

typedef struct scsCmdOtpRomTagFind_s
{
    vcsUint8_t      flags;          /* flags, see below */
    vcsUint8_t      tag;            /* (OTPROM_TAGTYPE_REG_INIT_INITIAL >> 12), etc.  Special case: 0 = match all tags */
    vcsUint8_t      maxntags;       /* maximum number of tags to return. 0 = unlimited */
    vcsUint8_t      firstidx;       /* first index of tags to return */
    vcsUint32_t     extended_tag;   /* extended tag value.  Used if tag == (OTPROM_TAGTYPE_EXTENDED >> 12) */
}scsCmdOtpRomTagFind_t;

typedef struct scsOtpRomTagElement_s
{
    vcsUint32_t     offsetInOtpRom;
    vcsUint8_t      tag;
    vcsUint32_t     extendedTag;
    vcsBlobData_t   pTagDataBlob;
} scsOtpRomTagElement_t;

/* Application request */
typedef struct scsCmdAppRequest_s
{
    vcsUint32_t  appId;         /* Application ID. */
    vcsUint32_t  requestLength; /* Length of request data. */
    vcsUint8_t * pRequestData;  /* Request data. */
} scsCmdAppRequest_t;

/* WOF configure command params */
typedef struct scsCmdWofConfigure_s
{
    vcsUint32_t  fdMode;        /* Finger detection mode. */
    vcsUint32_t  wofDataLength; /* Length of WOF parameter. */
    vcsUint8_t * pWofData;      /* WOF parameter data. */
} scsCmdWofConfigure_t;

/* WOF2 Rx Select command params */
typedef struct scsCmdWofRxSelect_s
{
    vcsUint32_t  wofRxSelParamLength; /* Length of WOF Rx select parameter. */
    vcsUint8_t * pWofRxSelParam;  /* WOF2 Rx select parameter data. */
} scsCmdWofRxSelect_t;

/* ReadLineAvgRawFpData (Read raw data, firmware returns line averaged data on EP1) */
typedef struct scsCmdReadLineAvgRawFpData_s
{
    vcsUint32_t dataSize;
} scsCmdReadLineAvgRawFpData_t;

typedef struct scsCmdCalWof2_s
{
    vcsUint16_t     max_replysize;  /* Maximum bytes in reply, based on params */
    vcsUint8_t      max_gain;       /* Highest gain allowed */
    vcsUint8_t      min_gain;       /* Lowest gain allowed */
    vcsUint8_t      start_gain;     /* Starting gain */
    vcsUint8_t      step_gain;      /* Step size for gain */
    vcsUint8_t      max_thresh;     /* Highest threshold allowed */
    vcsUint8_t      min_thresh;     /* Lowest threshold allowed (> 0) */
    vcsUint8_t      polltime;       /* Wake-on-Timer definition (ms) */
    vcsUint8_t      debug;          /* Verbose reply debug data (1 zone only) */
    vcsUint8_t      wof2_flags;     /* indicates zone */
    vcsUint16_t     wof2_ctrl1;     /* WOF2_CTRL1 value for calibration */
    vcsUint32_t     wof2_ctrl2;     /* WOF2_CTRL3 value for calibration */
    vcsUint32_t     wof2_thresh;    /* WOF2_THRESH value for calibration */
    vcsUint32_t     wof2_threshz1;  /* WOF2_THRESHZ1 value for calibration */
    vcsBlobData_t   wofParamBlob;   /* wof2 param blob */
} scsCmdCalWof2_t;

typedef struct scsCmdFrameRead_s
{
    vcsUint32_t frameSize;
    vcsUint16_t frameNum;
} scsCmdFrameRead_t;

typedef struct scsCmdFrameAcquire_s
{
    vcsUint32_t flags;
    vcsUint32_t mode;
    vcsUint16_t numFrames;
    vcsBlobData_t   tagDataBlob;
} scsCmdFrameAcquire_t;

typedef struct scsCmdEventConfig_s
{
    const vcsUint8_t *  pEvents;
    vcsUint32_t         nEventsNum;
} scsCmdEventConfig_t;

typedef struct scsCmdEventRead_s
{
    vcsUint16_t     firstNum;
    vcsUint16_t     nEvents;
} scsCmdEventRead_t;

typedef struct scsCmdFrameStream_s
{
    vcsUint16_t     nBytes;
} scsCmdFrameStream_t;

typedef struct scsCmdIotaFind_s
{
    vcsUint16_t     itype;      /* type of iotas to find */
    vcsUint16_t     flags;      /* flags, see below */
    vcsUint8_t      maxniotas;  /* maximum number of iotas to return.
                                 *  0 = unlimited */
    vcsUint8_t      firstidx;   /* first index of iotas to return */
    vcsUint8_t      dummy[2];
    vcsUint32_t     offset;     /* byte offset of data to return */
    vcsUint32_t     nbytes;     /* maximum number of bytes to return */
} scsCmdIotaFind_t;

typedef enum scsWitId_e
{
    /* Sensor commands: General */
    scsWitIdCmdNoop = 0,                    /* ALL */
    scsWitIdCmdGetVersion,                  /* ALL */
    scsWitIdCmdReset,                       /* ALL */
    scsWitIdCmdGetStartInfo,                /* ALL but vfs201 */
    scsWitIdCmdPeek,                        /* ALL */
    scsWitIdCmdPoke,                        /* ALL */
    scsWitIdCmdGpio,                        /* ALL */
    /* Sensor commands: Patch */
    scsWitIdCmdPatch,                       /* ALL */
    scsWitIdCmdUnloadPatch,                 /* ALL but vfs201 */
    /* Sensor commands: GetPrint */
    scsWitIdCmdGetPrint,                    /* ALL */
    scsWitIdCmdStopPrint,                   /* ALL */
    scsWitIdCmdGetFingerStatus,             /* ALL but Windsor */
    scsWitIdCmdReadFpData,                  /* Windsor */
    scsWitIdCmdGetNavState,                 /* Windsor */
    scsWitSetIdleTimer,                     /* Windsor */
    scsWitIdCmdIfsStatsGet,                 /* Windsor */
#if !defined(VCS_OS_BCMUSH)
    /* Sensor commands: Security */
    scsWitIdCmdTakeOwnershipEx2,            /* Windsor */
    scsWitIdCmdResetOwnership,              /* Falcon-vfs451/471/491, Windsor */
    scsWitIdCmdGetCertificateEx,            /* Windsor */
    /* Sensor commands: Secure wrap/unwrap */
    scsWitIdCmdSecureWrap,                  /* Windsor */
    /* Sensor commands: Storage */
    scsWitIdCmdStorageInfoGet,              /* Windsor */
    scsWitIdCmdStoragePartFormat,           /* Windsor */
    scsWitIdCmdStoragePartRead,             /* Windsor */
    scsWitIdCmdStoragePartWrite,            /* Windsor */
    scsWitIdCmdStorageFwPartValidate,       /* Windsor */
    scsWitIdCmdStorageFwPartInfoGet,        /* Windsor */
    /* Sensor commands: DB */
    scsWitIdCmdDbInfoGet,                   /* Windsor */
    scsWitIdCmdDbObjectCreate,              /* Windsor */
    scsWitIdCmdDbObjectDelete,              /* Windsor */
    scsWitIdCmdDbObjectInfoGet,             /* Windsor */
    scsWitIdCmdDbObjectDateGet,             /* Windsor */
    scsWitIdCmdDbObjectRestore,             /* Windsor */
    scsWitIdCmdDbUserInfoGet,               /* Windsor */
    scsWitIdCmdDbSystemParamGet,            /* Windsor */
    scsWitIdCmdDbSystemParamSet,            /* Windsor */
    scsWitIdCmdDbContainerInfoGet,          /* Windsor */
    /* Sensor commands: Matching */
    scsWitIdCmdAuthSessionStart,            /* Windsor */
    scsWitIdCmdAuthSessionFinish,           /* Windsor */
    scsWitIdCmdAuthSessionStatusGet,        /* Windsor */
    scsWitIdCmdAuthSessionResultGet,        /* Windsor */
    scsWitIdCmdAuthSessionDataGet,          /* Windsor */
    /* Sensor commands: Enrollment */
    scsWitIdCmdEnrollSessionStart,          /* Windsor */
    scsWitIdCmdEnrollSessionFinish,         /* Windsor */
    scsWitIdCmdEnrollSessionStatusGet,      /* Windsor */
    scsWitIdCmdTemplateUpdate,              /* Windsor */
    /* Sensor Commands: OTP */
    scsWitIdOtpRomTagFind,                  /* Windsor */
    /* Sensor commands: Application request */
    scsWitIdCmdAppRequest,                  /* Windsor */
#endif /* !defined(VCS_OS_BCMUSH) */
    scsWitWofConfigure,                     /* Windsor */
    scsWitWofStatusGet,                     /* Windsor */
    scsWitIdCmdReadLineAvgRawFpData,        /* Windsor */
    scsWitIdCmdCalWof2V3,
#if VCS_FEATURE_LED
    scsWitIdCmdLEDEx2,
#endif /*VCS_FEATURE_LED*/
    scsWitWofRxSelect,                     /* Windsor */
    scsWitAutorestartStatusGet,            /* Windsor */
    scsWitIdCmdCustidGet,                  /* Windsor */
    scsWitFlushFPData,                     /* Windsor */
    scsWitIdCmdGetNavStateV2,              /* Windsor */
#if VCS_FEATURE_SENSOR_NASSAU
    scsWitIdCmdFrameRead,                  /* Nassau */
    scsWitIdCmdFrameAcquire,               /* Nassau */
    scsWitIdCmdFrameFinish,                /* Nassau */
    scsWitIdCmdFrameDimensionGet,          /* Nassau */
    scsWitIdCmdEventConfig,                /* Nassau */
    scsWitIdCmdEventRead,                  /* Nassau */
    scsWitIdCmdLnaBlGet,                   /* Nassau */
    scsWitIdCmdBootLoaderPatch,            /* Nassau */
    scsWitIdCmdFrameStream,                /* Nassau */
    scsWitIdCmdIotaFind,                   /* Nassau */
    scsWitIdCmdXsreg8blkGet,               /* Nassau */
    scsWitIdTempCorrectionFactorGet,       /* Nassau */
#endif /* VCS_FEATURE_SENSOR_NASSAU */
    scsWit_maxCount /* to be able to validate the ID */
} scsWitId_t;

typedef struct scsCommand_s
{
    vcsUint32_t             witId;          /* WorkItem ID */
    scsSensorGenericCB_t    callback;       /* A pointer to the client create callback function. This function is called
                                             * once when the client creation has completed. If the creation completes
                                             * successfully the created scsClientHandle_t is passed to the callback and
                                             * can be used in subsequent SCS API calls.*/
    void *                  pUserData;      /* A pointer to a caller allocated context block. This pointer is passed to
                                             * the callback function as a parameter.
                                             * This parameter can be VCS_NULL if there is no application context to be passed
                                             * back to the caller. */
    vcsUint32_t *           pRequestId;     /* Each command which is eventually submitted to the hardware is assigned a
                                             * unique id. That unique id is written to the location pointed to by this
                                             * parameter */
    union
    {
        scsCmdReset_t                       reset;
        scsCmdGetFingerprint_t              getPrint;
        scsCmdDownloadPatch_t               downloadPatch;
        scsCmdResetOwnership_t              resetOwnership;
        scsCmdPeek_t                        peek;
        scsCmdPoke_t                        poke;
        scsCmdTakeOwnershipWithKeysEx_t     takeOwnershipWithKeysEx;
        scsCmdGPIO_t                        gpio;
        scsCmdGetParam_t                    getParam;
        scsCmdSetParam_t                    setParam;
        scsCmdIspRequest_t                  ispRequest;
        scsCmdCallFunction_t                callFunction;
        scsCmdSetLEDStateEx2_t              setLEDStateEx2;
        scsCmdStoragePartFormat_t           storagePartFormat;
        scsCmdStoragePartRead_t             storagePartRead;
        scsCmdStoragePartWrite_t            storagePartWrite;
        scsCmdStorageFWPartValidate_t       storageFWPartValidate;
        scsCmdStorageFWPartInfoGet_t        storageFWPartInfoGet;
        scsCmdDbObjectInfoGet_t             dbObjectInfoGet;
        scsCmdDbObjectCreate_t              dbObjectCreate;
        scsCmdDbObjectDelete_t              dbObjectDelete;
        scsCmdDbObjectDataGet_t             dbObjectDataGet;
        scsCmdDbUserInfoGet_t               dbUserInfoGet;
        scsCmdDbObjectRestore_t             dbObjectRestore;
        scsCmdDbSystemParamGet_t            dbSystemParamGet;
        scsCmdDbSystemParamSet_t            dbSystemParamSet;
        scsCmdDbContainerInfoGet_t          dbContainerInfoGet;
        scsCmdReadFpData_t                  readFpData;
        scsCmdAuthSessionStart_t            authSessionStart;
        scsCmdAuthSessionFinish_t           authSessionFinish;
        scsCmdAuthSessionStatusGet_t        authSessionStatusGet;
        scsCmdAuthSessionResultGet_t        authSessionResultGet;
        scsCmdAuthSessionDataGet_t          authSessionDataGet;
        scsCmdEnrollSessionStart_t          enrollSessionStart;
        scsCmdEnrollSessionFinish_t         enrollSessionFinish;
        scsCmdTemplateUpdate_t              templateUpdate;
        scsCmdSecureWrap_t                  secureWrapUnwrap;
        scsCmdSetIdleTimer_t                idleTimerTimeout;
        scsCmdOtpRomTagFind_t               otpRomTagFindParams;
        scsCmdAppRequest_t                  appRequest;
        scsCmdWofConfigure_t                wofConfigure;
        scsCmdReadLineAvgRawFpData_t        readLineAvgRawFpDataParams;
        scsCmdCalWof2_t                     calWof2Params;
        scsCmdWofRxSelect_t                 wofRxSelect;
        scsCmdFrameRead_t                   frameRead;
        scsCmdFrameAcquire_t                frameAcq;
        scsCmdEventConfig_t                 evnConfig;
        scsCmdEventRead_t                   evnRead;
        scsCmdDownloadBootLoaderPatch_t     downloadBootLoaderPatch;
        scsCmdFrameStream_t                 frameStream;
        scsCmdIotaFind_t                    iotaFind;
        scsCmdAny_t                         any;
    } params;
} scsCommand_t;

#define SET_CMD_COMMON_DATA(_cmd_, _wit_, _callbac_, _uData_, _reqId_)  \
{                                                                       \
    (_cmd_)->witId      = (_wit_);                                      \
    (_cmd_)->callback   = (scsSensorGenericCB_t)(_callbac_);            \
    (_cmd_)->pUserData  = (_uData_);                                    \
    (_cmd_)->pRequestId = (_reqId_);                                    \
}                                                                       \

     /************************************************************************/
   /*                                                                        */
/*                          SCS COMMAND STRUCTURES                           */
   /*                                                                        */
     /************************************************************************/

vcsResult_t VCS_API scsGetStorageMemoryInfo(scsSensorHandle_t hScsSensor, FlashMemoryInfo_t * pFlashMemoryInfo);

extern vcsResult_t VCS_API
scsStoragePartitionsFormat(
    scsSensorHandle_t   hScsSensor,
    vcsUint8_t          partId);

extern vcsResult_t VCS_API
scsStoragePartitionWrite(
    scsSensorHandle_t   hSensor,
    vcsUint8_t          partId,
    vcsUint8_t          mode,
    vcsUint32_t         offset,
    vcsUint32_t         size,
    vcsUint8_t *        pWriteData,
    vcsUint32_t *       dataSize);

vcsResult_t VCS_API scsSensorSendCommand(scsSensorHandle_t hSensor, const scsCommand_t * pCommand);

vcsResult_t VCS_API
scsSensorEventListenStart(scsSensorHandle_t hSensor, vcsUint32_t eventType, scsEventListenCB_t callback, void * pUserData);

vcsResult_t VCS_API
scsSensorEventListenCancel(scsSensorHandle_t hSensor, vcsUint32_t eventType);

/***************************************
* Windsor Specific Functions and Types *
***************************************/

typedef struct scsSensorStoragePartInfo_s
{
    vcsUint8_t  id;         /* Partition ID */
    vcsUint8_t  type;       /* Partition Type */
    vcsUint16_t attrb;      /* Partition attributes */
    vcsUint32_t offset;     /* Beginning offset of the partition in bytes */
    vcsUint32_t size;       /* Size of the partition in bytes */
} scsSensorStoragePartInfo_t;

typedef struct scsSensorStorageInfo_s
{
    vcsUint16_t mfgid;          /* Manufacture ID of the Flash */
    vcsUint16_t deviceid;       /* Device ID of the Flash */
    vcsUint16_t blocksize;      /* Block size in bytes */
    vcsUint16_t sectorsize;     /* Erase sector size in blocks */
    vcsUint16_t capacity;       /* Capacity of the flash in blocks */
    vcsUint16_t reservedsize;   /* Reserved space in blocks, including the
                                 sensor security context and partition table */
    vcsUint32_t partitions;     /* Number of partitions */
    scsSensorStoragePartInfo_t * pPartInfo;  /* partition info data */
} scsSensorStorageInfo_t;

typedef struct scsSensorFwPartModule_s
{
    vcsUint16_t type;           /* Type of module */
    vcsUint16_t subtype;        /* Subtype of module
                                For static and dynamic FW patch - patch phase
                                For FW extension - type of extension*/
    vcsUint16_t majorver;       /* Major version of the FW module */
    vcsUint16_t minorver;       /* Minor version of the FW module */
    vcsUint32_t size;           /* Size of the module */
} scsSensorFwPartModule_t;

typedef struct scsSensorFwPartInfo_s
{
    vcsUint16_t vmajor;     /* Major version of the FW extension */
    vcsUint16_t vminor;     /* Minor version of the FW extension */
    vcsUint32_t buildtime;  /* Build time of the FW extension in seconds
                               from 1/1/1970 12:00 AM GMT */
    vcsUint32_t modules;    /* Number of FW modules in the module list */
    vcsBlobData_t   fwPartModule;   /* FW partition module info */
} scsSensorFwPartInfo_t;

typedef struct scsSensorDbInfo_s
{
    vcsUint16_t majorversion; /* Major version of the database */
    vcsUint16_t minorversion; /* Minor version of the database */
    vcsUint32_t attrib;       /* Attribute of the database */
    vcsUint32_t capacity;     /* Capacity of the database in bytes */
    vcsUint32_t usedspace;    /* Space consumed by valid data in bytes */
    vcsUint32_t freespace;    /* Available free space in bytes */
    vcsUint16_t revision;     /* Revision of the database */
                              /* Incremented on every database update */
    vcsUint16_t usercontainers;/* Number of user containers in the database */
    vcsBlobData_t   userContainerList; /* user container id list */
} scsSensorDbInfo_t;

typedef struct scsSensorDbObjectEntry_s
{
    vcsUint16_t id;     /* object id */
    vcsUint16_t type;   /* object type */
} scsSensorDbObjectEntry_t;

typedef struct scsSensorDbObjectInfo_s
{
    vcsUint16_t id;     /* object id */
    vcsUint16_t type;   /* object type */
    vcsUint16_t attrib; /* object attributes */
    vcsUint16_t size;   /* size of the object data in bytes */
    vcsUint32_t nchild; /* number of child object */
    vcsBlobData_t   childList;  /* object child list */
} scsSensorDbObjectInfo_t;

typedef struct scsSensorDbObject_s
{
    vcsUint16_t objectid;   /* object id */
    vcsUint16_t type;       /* object type */
    vcsUint16_t attrib;     /* object attributes */
    vcsUint16_t size;       /* size of the object in bytes */
    vcsBlobData_t   objectData; /* object data */
} scsSensorDbObject_t;

/* structure of template entry */
typedef struct scsSensorDbTemplateEntry_s
{
    vcsUint16_t objectid;   /* object id */
    vcsUint16_t templateId; /* template ID */
    vcsUint16_t attrib;     /* attribute */
    vcsUint16_t size;       /* size of the template data */
} scsSensorDbTemplateEntry_t;

/* structure of user data entry */
typedef struct scsSensorDbUserDataEntry_s
{
    vcsUint16_t objectid;    /* object id */
    vcsUint16_t datatype;    /* type of user data */
    vcsUint16_t size;        /* size of the data blob */
    vcsUint16_t reserved;    /* reserved */
} scsSensorDbUserDataEntry_t;

typedef struct scsSensorDbUserInfo_s
{
    vcsUint16_t objectid;   /* object id */
    vcsUint16_t templates;  /* number of templates enrolled by the user */
    vcsUint16_t objects;    /* number of data objects owned by the user */
    vcsUint16_t uuidsize;   /* size of UUID in bytes */
    vcsBlobData_t   templateData;
    vcsBlobData_t   objectData;
    vcsBlobData_t   uuidData;
} scsSensorDbUserInfo_t;

/* structure of user entry */
typedef struct scsSensorDbUserEntry_s
{
    vcsUint16_t objectid;    /* object id */
    vcsUint16_t size;        /* size of the user data */
} scsSensorDbUserEntry_t;

typedef struct scsSensorDbUserContainerInfo_s
{
    vcsUint16_t objectid;   /* object id */
    vcsUint16_t users;      /* number of users */
    vcsUint32_t ucuidsize;  /* size of UCUID in bytes */
    vcsBlobData_t   userEntry;
    vcsBlobData_t   ucuidData;
} scsSensorDbUserContainerInfo_t;

/* structure for storing the calibration data of differant params 
   that will be send in the line update transform */
typedef struct scsCalibrationData_s
{
	 /* CF1 Array params */
	vcsUint8_t *                pCalCF1Data;        
    vcsUint32_t                 calCF1DataSize;
    vcsUint16_t                 calCF1romOffset;
    vcsUint8_t                  calCF1romArrayFlags;
    vcsUint8_t                  calCF1romAdjAdd;
    vcsUint8_t                  calCF1bAdjArraySign;
    vcsUint8_t                  calCF1AdjAdd;
    vcsUint8_t                  calCF1adjArrayWidth;
    vcsUint8_t                  calCF1bAdjSigned;

	/* CF2 Data array params  */
    vcsUint8_t *                pCalCF2Data;
    vcsUint32_t                 calCF2DataSize;
    vcsUint8_t                  calCF2bAdjArraySign;
    vcsUint8_t                  calCF2AdjAdd;
    vcsUint8_t                  calCF2adjArrayWidth;
    vcsUint8_t                  calCF2bAdjSigned;


	vcsUint8_t *                pRxMapData;         	/* Rx Map data array  */
    vcsUint32_t                 rxmapDataSize;      	/* number of entries in the RxMap data array */
    vcsUint8_t                  bRxMapExistsInOtprom;  /* Check whether Rx Map exists in otprom or not */

	/* LNA Gain  params */
	vcsUint8_t *                pCalLNAGainData;         
    vcsUint32_t                 calLNAGainDataSize;
    vcsUint16_t                 calLNAGainromOffset;
    vcsUint8_t                  calLNAGainromArrayFlags;
    vcsUint8_t                  calLNAGainromAdjAdd;
    vcsUint8_t                  calLNAGainbAdjArraySign;
    vcsUint8_t                  calLNAGainAdjAdd;
    vcsUint8_t                  calLNAGainadjArrayWidth;
    vcsUint8_t                  calLNAGainbAdjSigned;

    /* PGA Gain  params */
    vcsUint8_t *                pCalPGAGainData;
    vcsUint32_t                 calPGAGainDataSize;
    vcsUint16_t                 calPGAGainromOffset;
    vcsUint8_t                  calPGAGainromArrayFlags;
    vcsUint8_t                  calPGAGainromAdjAdd;
    vcsUint8_t                  calPGAGainbAdjArraySign;
    vcsUint8_t                  calPGAGainAdjAdd;
    vcsUint8_t                  calPGAGainadjArrayWidth;
    vcsUint8_t                  calPGAGainbAdjSigned;

    /* LNA Offset parameters */
    vcsUint8_t *                pLnaOffsetData;         /* LNA offset data array to compensate the LNA gain of each receiver */
    vcsUint32_t                 lnaOffsetDataSize;      /* number of entries in the LNA offset data array */
    vcsUint16_t                 calLNAOffsetromOffset;
    vcsUint16_t                 calLNAOffsetAdjAdd;
    vcsUint8_t                  calLNAOffsetromArrayFlags;
    vcsUint8_t                  calLNAOffsetadjArrayWidth;
    vcsUint8_t                  calLNAOffsetromAdjAdd;
    vcsUint8_t                  calLnaOffsetExistInOTPROM;
    vcsUint8_t                  calLNAOffsetbAdjArraySign;
    vcsUint8_t                  calLNAOffsetbAdjSigned;

    /* HP Adder parameters */
    vcsUint8_t *                pHpAdderData;           /* HP_ADDER data array to compensate the HPF offset of each receiver */
    vcsUint32_t                 hpAdderDataSize;        /* number of entries in the HP_ADDER data array */
    vcsUint16_t                 calHPFromOffset;
    vcsUint8_t                  calHPFromArrayFlags;
    vcsUint8_t                  calHPFromAdjAdd;
    vcsUint8_t                  calHPFbAdjArraySign;
    vcsUint8_t                  calHPFAdjAdd;
    vcsUint8_t                  calHPFadjArrayWidth;
    vcsUint8_t                  calHPFbAdjSigned;

    /* PGA Offset  params */
    vcsUint8_t *                pCalPGAOffsetData;
    vcsUint32_t                 calPGAOffsetDataSize;
    vcsUint16_t                 calPGAOffsetromOffset;
    vcsUint8_t                  calPGAOffsetromArrayFlags;
    vcsUint8_t                  calPGAOffsetromAdjAdd;
    vcsUint8_t                  calPGAOffsetbAdjArraySign;
    vcsUint8_t                  calPGAOffsetAdjAdd;
    vcsUint8_t                  calPGAOffsetadjArrayWidth;
    vcsUint8_t                  calPGAOffsetbAdjSigned;

    /* otprom parameters */
    scsOtpRomTagElement_t *     pOtpRomTagElements;
    vcsUint32_t                 nOtpRomTagElements;

    /* OTP based baseline variance */
    vcsUint32_t                 lineVariance;
} scsCalibrationData_t;

/* Version of the cal data structure. Increment each time when structure change. */
#define VCS_CAL_DATA_VERSION    2

typedef struct scsCalibrationDataSet_s
{
    /* data structure version of the dataset */
    vcsUint32_t                 dataSetVersion;
    vcsUint32_t                 flexId;
    scsSensorFwVersion_t        fwVersion;
} scsCalibrationDataInfo_t;

typedef struct scsSensorMocStatInfo_s
{
    vcsUint16_t trulyTriggered; /* Truly triggered.              */
    vcsUint16_t falseTriggered; /* False triggered.              */
    vcsUint16_t badImages;      /* Bad image and other problems. */
    vcsUint16_t minCaptureTime; /* Min capture time (ms)         */
    vcsUint16_t maxCaptureTime; /* Max capture time (ms)         */
    vcsUint16_t minAuthTime;    /* Min auth time (ms)            */
    vcsUint16_t maxAuthTime;    /* Max auth time (ms)            */
} scsSensorMocStatInfo_t;

typedef struct scsSensorEnrollSessionStatusDbgInfo_s
{
    vcsUint32_t enrollDbgWrapperCode;
    vcsUint32_t enrollDbgMatcherCode;
    vcsUint16_t enrollDbgDataSize;
    vcsUint8_t  enrollDbgData[SCS_ENROLL_SESSION_STATUS_DBG_INFO_DATA_SIZE];
} scsSensorEnrollSessionStatusDbgInfo_t;

//scsGetDeviceState() request types
#define VCS_DEV_STATE_TYPE_UNKNOWN      ((vcsUint8_t) 0)	//error
#define VCS_DEV_STATE_TYPE_GENERIC      ((vcsUint8_t) 1)	//Return Acq# and Evt# only
#define VCS_DEV_STATE_TYPE_CAPTURE      ((vcsUint8_t) 2)	//Return scsDevStateCapture_t info
#define VCS_DEV_STATE_TYPE_MOC          ((vcsUint8_t) 3)	//Return scsDevStateMoc_t info
#define VCS_DEV_STATE_TYPE_EOC          ((vcsUint8_t) 4)	//Return scsDevStateEoc_t info

typedef struct scsDevStateCapture_s
{
    vcsUint8_t  fpStatus;
    vcsUint8_t  reserved;
    vcsUint16_t irStatus;
} scsDevStateCapture_t;

typedef struct scsDevStateMoc_s
{
    vcsUint16_t  sessionStatus;
    vcsUint16_t  sessionParam;
} scsDevStateMoc_t;

typedef struct scsDevStateEoc_s
{
    vcsUint16_t  sessionStatus;
    vcsUint16_t  sessionParam;
} scsDevStateEoc_t;

typedef struct scsDeviceState_s
{
    union
    {
        scsDevStateCapture_t    captureState;
        scsDevStateMoc_t        mocState;
        scsDevStateEoc_t        eocState;
    } info;
    vcsUint8_t acqNum;  /* Newest acquired frame acquisition number */
    vcsUint8_t evtNum;  /* Newest event number */
} scsDeviceState_t;

vcsResult_t VCS_API scsSensorStorageInfoGet(
    scsSensorHandle_t hSensor,
    scsSensorStorageInfo_t * pStorageInfo);

vcsResult_t VCS_API scsSensorStoragePartFormat(
    scsSensorHandle_t hSensor,
    vcsUint8_t partId);

vcsResult_t VCS_API scsSensorStoragePartRead(
    scsSensorHandle_t hSensor,
    vcsUint8_t  partId,
    vcsUint8_t  mode,
    vcsUint32_t offset,
    vcsUint32_t size,
    vcsUint32_t * dataSize,
    vcsUint8_t * pFlashData);

vcsResult_t VCS_API scsSensorStoragePartWrite(
    scsSensorHandle_t hSensor,
    vcsUint8_t  partId,
    vcsUint8_t  mode,
    vcsUint32_t offset,
    vcsUint32_t size,
    vcsUint8_t * pWriteData,
    vcsUint32_t * dataSize);

vcsResult_t VCS_API scsSensorStorageFWPartValidate(
    scsSensorHandle_t hSensor,
    vcsUint8_t * pSignatureData,
    vcsUint8_t partId,
    vcsUint8_t signType,
    vcsUint16_t signSize);

vcsResult_t VCS_API scsSensorStorageFWPartInfoGet(
    scsSensorHandle_t hSensor,
    vcsUint8_t  partId,
    scsSensorFwPartInfo_t * pFwPartInfo);

vcsResult_t VCS_API scsSensorDbInfoGet(
    scsSensorHandle_t hSensor,
    scsSensorDbInfo_t * pDbInfo);

vcsResult_t VCS_API
scsSensorDbObjectInfoGet(
    scsSensorHandle_t           hSensor,
    vcsUint32_t                 objectId,
    scsSensorDbObjectInfo_t *   pDbObjectInfo);

vcsResult_t VCS_API scsSensorDbObjectCreate(
    scsSensorHandle_t   hSensor,
    vcsUint32_t         parentId,
    vcsUint16_t         type,
    vcsUint16_t         attrib,
    vcsUint32_t         size,
    vcsUint8_t *        pObjectData,
    vcsUint32_t *       objectId);

vcsResult_t VCS_API
scsSensorDbObjectDelete(
    scsSensorHandle_t   pSensor,
    vcsUint32_t         objectId);

vcsResult_t VCS_API
scsSensorDbObjectDataGet(
    scsSensorHandle_t       hSensor,
    vcsUint32_t             objectId,
    scsSensorDbObject_t *   pDbObject);

vcsResult_t VCS_API
scsSensorDbUserInfoGet(
    scsSensorHandle_t       hSensor,
    vcsUint32_t             objectId,
    vcsUint32_t             userContId,
    vcsUint32_t             uuidSize,
    vcsUint8_t *            pUUIDData,
    scsSensorDbUserInfo_t * pDbUserInfo);

vcsResult_t VCS_API scsSensorDbObjectRestore(
    scsSensorHandle_t hSensor,
    vcsUint32_t size,
    vcsUint16_t parentId,
    vcsUint8_t * pRestoreData,
    vcsUint16_t * objectId);

vcsResult_t VCS_API scsSensorDbSystemParamGet(
    scsSensorHandle_t hSensor,
    vcsString_t sName,
    vcsUint16_t nameSize,
    vcsUint16_t * dataSize,
    vcsString_t sParameterValue);

vcsResult_t VCS_API scsSensorDbSystemParamSet(
    scsSensorHandle_t hSensor,
    vcsString_t sParamName,
    vcsString_t sParamValue,
    vcsUint16_t nameSize,
    vcsUint16_t valueSize);

vcsResult_t VCS_API
scsSensorDbContainerInfoGet(
    scsSensorHandle_t                   hSensor,
    vcsUint32_t                         objectId,
    vcsUint32_t                         ucuidSize,
    vcsUint8_t *                        pUCUID,
    scsSensorDbUserContainerInfo_t *    pDbUserContainerInfo);

vcsResult_t VCS_API
scsSensorGetCertificateEx(
    scsSensorHandle_t           hSensor,
    vcsUint32_t *               replyLength,
    vcsfw_rsa_certificate_t *   pRsaCert, 
    vcsfw_ecc_certificate_t *   pEccCert);

vcsResult_t VCS_API
scsSensorTakeOwnershipWithKeysEx(
    scsSensorHandle_t   hSensor,
    vcsUint8_t *        pStorageInfo,
    vcsUint8_t *        pPartTable,
    vcsUint8_t *        pHAPubKey,
    vcsUint8_t *        pHAPubKeySign,
    vcsUint32_t         nHAPubKeySignLen,
    vcsUint8_t *        pEccCurve,
    vcsBool_t           bIsECC,
    vcsBlobData_t       encHAPubKey);

vcsResult_t VCS_API
scsSensorSecureWrap(
    scsSensorHandle_t   hSensor,
    vcsUint16_t         flags,
    vcsUint8_t *        pInData,
    vcsUint16_t         inDataLength,
    vcsUint8_t *        pOutData,
    vcsUint16_t *       pOutDataLength);

vcsResult_t VCS_API
scsSensorAuthSessionStart(
    scsSensorHandle_t   hSensor,
    vcsUint8_t          authType,
    vcsUint32_t         timeout,
    vcsUint32_t         ucObjId,
    vcsUint32_t         usrObjId,
    vcsUint16_t         authDataType,
    vcsUint32_t         addDataLen,
    vcsUint8_t *        pAddData,
    vcsUint32_t *       pSessionId);

vcsResult_t VCS_API
scsSensorAuthSessionFinish(
    scsSensorHandle_t   hSensor,
    vcsUint32_t         sessionId);

vcsResult_t VCS_API
scsSensorAuthSessionStatusGet(
    scsSensorHandle_t       hSensor,
    vcsBool_t               bOpTypePoa,
    vcsUint32_t             sessionId,
    vcsUint16_t *           pSessionStatus,
    vcsUint16_t *           pParam,
    scsSensorMocStatInfo_t * pMocStat);

vcsResult_t VCS_API
scsSensorAuthSessionResultGet(
    scsSensorHandle_t   hSensor,
    vcsUint32_t         sessionId,
    vcsUint32_t *       pUsrObjId,
    vcsUint32_t *       pScore,
    vcsUint32_t *       pMatcherRetCode,
    vcsUint32_t *       pPekSize,
    vcsUint16_t *       pTemplateIndex,
    vcsUint16_t *       pTemplateId,
    vcsUint8_t *        pPek,
    vcsUint32_t *       pTuidSize,
    vcsUint8_t *        pTuid,
    vcsUint16_t *       pTemplateObjId);

vcsResult_t VCS_API
scsSensorAuthSessionDataGet(
    scsSensorHandle_t   hSensor,
    vcsUint32_t         sessionId,
    vcsUint16_t         authDataType,
    vcsUint16_t         paramDataSize,
    vcsUint8_t *        pParamData,
    vcsUint16_t *       pAuthDataDize,
    vcsUint8_t *        pAuthData);

vcsResult_t VCS_API
scsSensorEnrollSessionStart(
    scsSensorHandle_t   hSensor,
    vcsUint32_t         flags,
    vcsUint32_t *       pSessionId);

vcsResult_t VCS_API
scsSensorEnrollSessionFinish(
    scsSensorHandle_t   hSensor,
    vcsBool_t           bEraseSessionCtx);

vcsResult_t VCS_API
scsSensorEnrollSessionStatusGet(
    scsSensorHandle_t   hSensor,
    vcsUint16_t *       pSessionStatus,
    vcsUint16_t *       pParam,
    scsSensorEnrollSessionStatusDbgInfo_t * pStatusDbgInfo);

vcsResult_t VCS_API
scsSensorTemplateUpdate(
    scsSensorHandle_t   hSensor,
    vcsUint16_t         inTemplDataSize,
    vcsUint8_t *        pInTemplData,
    vcsUint16_t *       pOutTemplDataSize,
    vcsUint8_t *        pOutTemplData,
    vcsUint16_t *       pEnrollmentStatusSize,
    vcsUint8_t *        pEnrollmentStatus,
    vcsUint32_t *       pTuidSize,
    vcsUint8_t *        pTuid);

vcsResult_t VCS_API
scsSensorReadFpData(
    scsSensorHandle_t   hSensor,
    vcsBlobData_t *     pFpData);

vcsResult_t VCS_API
scsSensorReadLineAvgRawFpData(
    scsSensorHandle_t   hSensor,
    vcsBlobData_t *     pFpData);

/**********************************************
* End of Windsor Specific Functions and Types *
**********************************************/

/* Reset */
extern vcsResult_t VCS_API scsSensorReset(scsSensorHandle_t hSensor, vcsUint16_t delayTime);

vcsResult_t VCS_API scsInit(scsSubsystemHandle_t * phSubsystem, vcsUint32_t mode);
vcsResult_t VCS_API scsUnInit(scsSubsystemHandle_t hSubsystem);

/* Sensor Open and Close
*/
vcsResult_t VCS_API scsSensorOpen(scsSubsystemHandle_t hSubsystem, vcsString_t sensorId, scsSensorHandle_t * phSensor);
vcsResult_t VCS_API scsSensorClose(scsSensorHandle_t hSensor, vcsUint32_t flags);
vcsResult_t VCS_API scsSensorStartup(scsSensorHandle_t pSensor);
vcsResult_t VCS_API scsSensorShutdown(scsSensorHandle_t pSensor);

vcsResult_t VCS_API scsSensorRemount(scsSensorHandle_t pSensor);

/* Hardware Reset
*/
vcsResult_t VCS_API scsSensorHardReset(scsSensorHandle_t pSensor);

/* GetVersion */
vcsResult_t VCS_API scsSensorGetVersion(scsSensorHandle_t hSensor, scsSensorVersion_t * version);

/* Download Patch to Sensor */
extern vcsResult_t VCS_API
scsSensorDownloadBootLoaderPatch(
    scsSensorHandle_t       hSensor,
    const vcsBlobData_t *   pPatchBlob);

/* DownloadPatch */
extern vcsResult_t VCS_API
scsSensorDownloadPatch(
    scsSensorHandle_t       hSensor,
    vcsUint16_t             address,
    const vcsBlobData_t *   pPatchBlob);

/* UnloadPatch from Sensor */
extern vcsResult_t VCS_API
scsSensorUnloadPatch(
    scsSensorHandle_t hSensor);

/* UnloadPatch */
/* ClearPatchInfoCache */
extern vcsResult_t VCS_API
scsSensorClearPatchInfoCache(
    scsSensorHandle_t hSensor);

/* Set the sensor Calibration type */
vcsResult_t VCS_API scsSensorCalibrate(scsSensorHandle_t hSensor, vcsUint32_t type);

/* Save the sensor calibration results */
vcsResult_t VCS_API
scsSensorCalResultsBackup(
    scsSensorHandle_t   pSensor,
    vcsBlobData_t * pCalData);

/* Restore saved calibration results */
vcsResult_t VCS_API
scsSensorCalResultsRestore(
    scsSensorHandle_t     pSensor,
    vcsBlobData_t *  pCalData);

vcsResult_t VCS_API scsSensorSetRecalibrationStatus(scsSensorHandle_t hSensor);

typedef struct calResultsContainer_s *  calResultsContainerHandle_t;

vcsResult_t VCS_API scsSensorGetCalibrationResults(scsSensorHandle_t hSensor, calResultsContainerHandle_t *phCalResults);
vcsResult_t VCS_API scsSensorSetCalibrationResults(scsSensorHandle_t hSensor, calResultsContainerHandle_t hCalResults);

vcsResult_t VCS_API scsSensorGetFingerStatus(scsSensorHandle_t hSensor, vcsUint8_t * pFingerStatus);

/*Get Navigation State */
vcsResult_t VCS_API scsSensorGetNavState(scsSensorHandle_t hSensor,  vcsfw_reply_get_nav_status_t *pFwNavStatus);
vcsResult_t VCS_API scsSensorGetNavStatus(scsSensorHandle_t hSensor, vcsfw_reply_get_nav_status_t *pFwNavStatus);
vcsResult_t VCS_API scsSensorGetNavStateV2(scsSensorHandle_t hSensor, vcsBlobData_t  *pNavBlobData);
vcsResult_t VCS_API scsSensorGetNavStatusV2(scsSensorHandle_t hSensor, vcsBlobData_t *      pNavBlobData);

/* Reset Sensor Ownership */
vcsResult_t VCS_API scsSensorResetOwnership(scsSensorHandle_t hSensor, vcsUint16_t passwordSize, vcsUint8_t * pPassword);

/* Abort Fingerprint Capture process */
vcsResult_t VCS_API scsSensorAbortFingerprint(scsSensorHandle_t hSensor);

/* FlushFpData */
vcsResult_t VCS_API scsSensorFlushFpData(scsSensorHandle_t hSensor);

/* Peek */
vcsResult_t VCS_API scsSensorPeek(scsSensorHandle_t hSensor, vcsUint32_t address, vcsUint8_t numBytes, vcsUint32_t * pValue);

/* Poke a register value */
vcsResult_t VCS_API scsSensorPoke(scsSensorHandle_t hSensor, vcsUint32_t address, vcsUint32_t value, vcsUint8_t numBytes);

/* GetStartInfo */
vcsResult_t VCS_API scsSensorGetStartInfo(scsSensorHandle_t hSensor, vcsUint8_t * startType, vcsUint8_t * resetType, vcsUint32_t * startStatus, vcsUint32_t * sanityPanic, vcsUint32_t * sanityCode, vcsUint32_t * reset_nvinfo);

/* Get the Custid and Projid of the sensor */
vcsResult_t VCS_API scsSensorCustidGet(scsSensorHandle_t hSensor, vcsUint32_t * pCustid, vcsUint32_t * pProjid);

extern vcsResult_t VCS_API
scsSensorSendCommandBlob(
    scsSensorHandle_t   hSensor,
    vcsBlobData_t *     pCmdBlob,
    vcsBlobData_t *     pReply);

/* Get Fingerprint Image from sensor */
vcsResult_t VCS_API
scsSensorGetFingerprint(
    scsSensorHandle_t       hSensor,
    vcsUint16_t             numLines,
    vcsUint32_t             captureMode,
    vcsUint32_t             calibrationType,
    vcsUint32_t             flags,
    vcsUint32_t             capturePurpose,
    scsSensorConfigHandle_t pGetPrintCfg,
    vcsBlobData_t *         pCfgReply);

/* PostFingerprintBuffer */
typedef vcsResult_t (VCS_API * scsSensorFingerprintBufferCB_t)(void * pUserData, vcsUint32_t status,
                                                               vcsUint8_t * pData, vcsUint32_t dataSize);
extern vcsResult_t VCS_API
scsSensorPostFingerprintBuffer(
    scsSensorHandle_t   hSensor,
    vcsUint8_t *        pBuffer,
    vcsUint32_t         bufferSize);


/* GetOwnershipInfo */
vcsResult_t VCS_API scsSensorGetOwnershipInfo(scsSensorHandle_t hSensor, vcsUint16_t * pTotalCycles, vcsUint16_t * pAvailableCycles);

/* Cancel All Fingerprint Buffers */

vcsResult_t VCS_API scsSensorCancelAllFingerprintBuffers(scsSensorHandle_t hSensor);

/* Read sensor FlexId */
vcsResult_t VCS_API scsSensorGetFlexId(scsSensorHandle_t hSensor, vcsUint32_t * flexId);
vcsResult_t VCS_API scsSensorGetProjId(scsSensorHandle_t hSensor, vcsUint32_t * pProjId);

/* Read the sensorsCommunications interface*/
extern vcsResult_t VCS_API
scsSensorGetCommIface(
    scsSensorHandle_t   hSensor,
    vcsCommIface_t *    pIface);

/* Read the fw Version of the sensor */
vcsResult_t VCS_API scsSensorGetFwVersion(scsSensorHandle_t hSensor, scsSensorFwVersion_t * pFwVersion);
vcsResult_t VCS_API scsSensorGetFwExtVersion(scsSensorHandle_t hSensor, scsSensorFwExtVersion_t * pFwExtVersion);
vcsResult_t VCS_API scsSensorGetDriverVersion(scsSensorHandle_t hSensor, scsSensorDriverVersion_t * pDriverVersion);
vcsResult_t VCS_API scsSensorGetCmdProtocolVersion(scsSensorHandle_t hSensor,vcsUint8_t * pCmdVersion);
void *     VCS_API scsGetStorageInfo(scsSensorHandle_t pSensor);
vcsResult_t VCS_API scsSetStorageInfo(scsSensorHandle_t hSensor, void * pStorageInfo);

vcsResult_t VCS_API scsSensorGetTimeSlotTable(scsSensorHandle_t hSensor, const vcsUint8_t ** ppTimeSlotTbl, vcsUint32_t * pTimeSlotTblSize);

vcsResult_t VCS_API scsSensorGetCfgReplyData(scsSensorHandle_t hSensor, const vcsUint8_t ** ppCfgReplyData, vcsUint32_t * pCfgReplyDataSize);

vcsResult_t VCS_API
scsSensorSetConfigReply(
    scsSensorHandle_t   hSensor,
    vcsBlobData_t *     pCfgReply);

vcsResult_t VCS_API scsSensorGetPrintConfigurationData(scsSensorHandle_t hSensor, scsSensorPrintConfigurationData_t * pSensorPrintConfigData);
vcsResult_t VCS_API scsSensorGetPrintConfigurationDataEx(scsSensorHandle_t hSensor, vcsUint32_t flexId, vcsUint32_t SOClock, vcsUint32_t swipeSpeed, vcsUint32_t bitDepth, scsSensorConfigHandle_t * pPrintConfig);
vcsResult_t VCS_API scsSensorFreePrintConfigurationData(scsSensorPrintConfigurationData_t * pSensorPrintConfigData);

/* Get current GetPrintCmd config */
vcsResult_t VCS_API scsSensorGetPrintCmd(scsSensorHandle_t hSensor, scsSensorGetPrintCmd_t * pGetPrintCmdCfg);

vcsResult_t VCS_API scsSensorGetBitReductionData(scsSensorHandle_t hSensor, scsBitReductionData_t * pBitReductionData);

/* Read the portion of fingerprint data stream synchronously in the fingerprint buffer(s)*/
vcsResult_t VCS_API scsReadFingerprintData(scsSensorHandle_t hSensor, vcsUint32_t nDataSize, vcsUint32_t * pReadDataSize, vcsBool_t * pbReadComplete);

vcsResult_t VCS_API scsSensorGetFalconNavConfigData(scsSensorHandle_t hScsSensor, scsSensorConfigHandle_t * pNavConfig);

vcsResult_t VCS_API scsSensorGetWindsorNavConfigData(scsSensorHandle_t hScsSensor, scsSensorConfigHandle_t * pNavConfig);

/* Finalize FP signature and reset/release FP encryption and signing contexts */
vcsResult_t VCS_API scsSensorFpCryptContextFinalize(scsSensorHandle_t hSensor,
                                                    vcsBool_t         keepFpSign);

/*
* Patch management functions
*/

/* Download flash patch if available */
vcsResult_t VCS_API scsDownloadFlashPatch(scsSensorHandle_t hSensor);

/* Download secure match patch if available */
vcsResult_t VCS_API scsDownloadSecureMatchPatch(scsSensorHandle_t hSensor);

vcsResult_t VCS_API scsDownloadGetPrintPatch(scsSensorHandle_t hSensor);

vcsResult_t VCS_API scsDownloadPatchByPhase(
    scsSensorHandle_t hSensor,vcsUint32_t phaseSelected);

vcsResult_t VCS_API scsDownloadWof2Patch(scsSensorHandle_t hSensor);

vcsResult_t VCS_API
scsDownloadImgReconPatch(
    scsSensorHandle_t hSensor);

/* Download the pre-boot patch. */
vcsResult_t VCS_API scsDownloadPrebootPatch(scsSensorHandle_t hSensor);

/* Download the run patch (Boot patch) */
vcsResult_t VCS_API scsDownloadRunPatch(scsSensorHandle_t hSensor);

/* Download the authentication patch */
vcsResult_t VCS_API scsDownloadAuthPatch(scsSensorHandle_t hSensor, vcsUint32_t algID);

/* Get the current patch level */
vcsResult_t VCS_API scsSensorGetPatchLevel(scsSensorHandle_t hSensor, vcsUint32_t * pPatchLevel);

/*
* Load patch on specified sensor, based on phase.
* Patch is selected based on the phaseSelected and phaseExcluded arguments.
*
* phaseSelected     patch selected must support the specified phases
* phaseExcluded     patch selected must not support the specified phases
*/
vcsResult_t VCS_API scsSensorLoadPatch(scsSensorHandle_t pSensor,
                                       void **     ppPatchList,
                                       vcsUint32_t phaseSelected,
                                       vcsUint32_t phaseExcluded,
                                       vcsBool_t * bPatchFound);

vcsResult_t VCS_API scsSensorLoadPatchEx(scsSensorHandle_t pSensor,
                                       void **     ppPatchList,
                                       vcsUint32_t phaseSelected,
                                       vcsUint32_t subPhaseSelected,
                                       vcsUint32_t phaseExcluded,
                                       vcsBool_t * bPatchFound);

/*
* Unload current patch, and load run patch if it was present before loading current patch.
*/
vcsResult_t VCS_API scsSensorRestoreRunPatch(scsSensorHandle_t pSensor);

/* Find the FlexId for Eagle sensors based on the specified channel */
vcsUint32_t VCS_API scsSensorEagleFindFlexId(scsSensorHandle_t hSensor, vcsUint16_t channel, vcsUint16_t projid);

/* Enable/Disable sensor */
vcsResult_t VCS_API scsSensorEnable(scsSensorHandle_t hSensor, vcsBool_t bEnable, vcsBool_t bOffOn);
/*Suspend/Resume sensor*/
vcsResult_t VCS_API scsSensorSuspend(scsSensorHandle_t hSensor, vcsBool_t bPowerOff);
vcsResult_t VCS_API scsSensorResume(scsSensorHandle_t hSensor);

/* Validate scs sensor */
vcsResult_t VCS_API scsSensorValidate(scsSensorHandle_t hSensor);

/* Send 1 line get print command to determine FlexId for the given sensor */
vcsResult_t VCS_API scsSensorDetermineFlexId(scsSensorHandle_t hSensor);

vcsResult_t VCS_API scsSensorSetDefaultPrintConfiguration(scsSensorHandle_t hSensor);

/* Functions to check if the given sensor is of a particular type or family */
vcsBool_t VCS_API scsSensorIsWindsor(scsSensorHandle_t hSensor);
vcsBool_t VCS_API scsSensorIsTouchSensor(scsSensorHandle_t hSensor);
vcsBool_t VCS_API scsSensorIsProductionSensor(scsSensorHandle_t hSensor);

vcsBool_t VCS_API scsSensorIsNassau(scsSensorHandle_t hSensor);
vcsBool_t VCS_API scsSensorInBootLoaderMode(scsSensorHandle_t hSensor);

vcsBool_t VCS_API scsSensorIsDenali(scsSensorHandle_t hSensor);
vcsBool_t VCS_API scsSensorIsHayes(scsSensorHandle_t hSensor);
vcsBool_t VCS_API scsSensorIsCpid(scsSensorHandle_t hSensor);
vcsUint32_t VCS_API scsSensorGetPixId(scsSensorHandle_t pSensor);


/* Functions to check if the given sensor is supports specific capability */
vcsBool_t VCS_API scsSensorIsSecured(scsSensorHandle_t pSensor);
vcsBool_t VCS_API scsSensorIsProvisioned(scsSensorHandle_t pSensor);
vcsBool_t VCS_API scsSensorIsRSASecured(scsSensorHandle_t hSensor);
vcsBool_t VCS_API scsSensorIsECCSecured(scsSensorHandle_t hSensor);
vcsBool_t VCS_API scsSensorIsBasicSecured(scsSensorHandle_t hSensor);
vcsBool_t VCS_API scsSensorIsFlashless(scsSensorHandle_t pSensor);
vcsBool_t VCS_API scsSensorFWExtExists(scsSensorHandle_t pSensor);

vcsResult_t VCS_API scsGetDeviceState(scsSensorHandle_t hSensor, vcsUint8_t devStateType, scsDeviceState_t * devState);

vcsResult_t VCS_API scsSensorGetProductFamily(scsSensorHandle_t hSensor, vcsProductFamily_t * pFamily);
vcsProductFamily_t VCS_API scsSensorGetProductFamilyByProductType(vcsUint8_t productType);

/* Unique identifier */
typedef struct scsSensorUID_s
{
    vcsUint16_t VID;
    vcsUint16_t PID;
    vcsUint8_t  UID[VCSFW_RAPTOR_UID_SIZE];
} scsSensorUID_t;

/* Baseline management functions */
vcsResult_t VCS_API scsSensorGetBaseLineInfo(scsSensorHandle_t hSensor, scsSensorBaselineInfoHandle_t * phBaseLineInfo);
vcsResult_t VCS_API scsCloneBaseLineInfo(scsSensorBaselineInfoHandle_t hIn, scsSensorBaselineInfoHandle_t hOut);
vcsResult_t VCS_API scsFreeBaseLineInfo(scsSensorBaselineInfoHandle_t hBaseLineInfo);

vcsResult_t VCS_API
scsSensorGetBaselineData(
    scsSensorHandle_t   hSensor,
    scsBaselineData_t * pBaselineData,
    vcsBool_t           bRawBaseline);

vcsResult_t VCS_API
scsSensorValidateAndProcessBaseLineData(
    scsSensorHandle_t   hSensor,
    scsBaselineData_t * pBaselineToValidate,
    scsBaselineData_t * pBaselineProcessed,
    vcsBool_t           bValidate);

vcsResult_t VCS_API
scsSensorGetBaselineVariance(
    scsSensorHandle_t   hSensor,
    vcsUint32_t *       pVariance);

vcsResult_t VCS_API
scsSensorSetBaselineVariance(
    scsSensorHandle_t   hSensor,
    const vcsUint32_t   variance);

vcsResult_t VCS_API
scsSensorBaseLineInfoRestore(
    scsSensorHandle_t       hSensor,
    vcsBlobData_t *    pBaseLineInfo);

vcsResult_t VCS_API
scsSensorBaseLineInfoBackup(
    scsSensorHandle_t   hSensor,
    vcsBlobData_t *     pBaseLineInfo);

vcsResult_t VCS_API
scsSensorCpidCalBackup(
    scsSensorHandle_t   hSensor,
    vcsBlobData_t *     pCpidCal);

vcsResult_t VCS_API
scsSensorCpidCalRestore(
    scsSensorHandle_t   hSensor,
    vcsBlobData_t *     pCpidCal);

vcsResult_t VCS_API
scsSensorCpidCalCleanup(
    scsSensorHandle_t   hSensor);

/* Utilities to operate with sensor configuration */
vcsResult_t VCS_API scsSensorGetDefaultPrintConfiguration(scsSensorHandle_t hSensor, scsSensorConfigHandle_t * phPrintConfig);

vcsResult_t VCS_API scsSensorFreePrintConfiguration(scsSensorHandle_t hSensor, scsSensorConfigHandle_t * phPrintConfig);
vcsResult_t VCS_API scsSensorClonePrintConfiguration(scsSensorHandle_t hSensor, scsSensorConfigHandle_t hSrcConfig,
                                                     scsSensorConfigHandle_t * phDestConfig);

typedef enum scsPrintConfigDataId_e
{
    scsPrintConfigFwMajor,
    scsPrintConfigFwMinor,
    scsPrintConfigFwBuild,
    scsPrintConfigFwPatch,
    scsPrintConfigFlexID,
    scsPrintConfigSOClock,
    scsPrintConfigSwipeSpeed,
    scsPrintConfigLinePeriod,
    scsPrintConfigLineLength,
    scsPrintConfigSecurity,
    scsPrintConfigBitDepth,
    scsPrintConfigBitReduction,
    scsPrintConfigBitReductionLinePeriod
} scsPrintConfigDataId_t;

vcsResult_t VCS_API
scsSensorGetUint32DataFromPrintConfig(scsSensorHandle_t hSensor,
                                      const scsSensorConfigHandle_t hPrintCfg,
                                      scsPrintConfigDataId_t id,
                                      vcsUint32_t * pData);
vcsResult_t VCS_API
scsSensorSetUint32DataToPrintConfig(scsSensorHandle_t hSensor,
                                    const scsSensorConfigHandle_t hPrintCfg,
                                    scsPrintConfigDataId_t id,
                                    vcsUint32_t data);

vcsResult_t VCS_API
scsSensorGetPrintParamBlobFromPrintConfig(scsSensorHandle_t hSensor,
                                          const scsSensorConfigHandle_t pPrintCfg,
                                          const vcsUint8_t ** ppParamBlob,
                                          vcsUint32_t * pBlobSize);
vcsResult_t VCS_API
scsSensorSetPrintParamBlobToPrintConfig(scsSensorHandle_t hSensor,
                                        const scsSensorConfigHandle_t pPrintCfg,
                                        const vcsUint8_t * pParamBlob,
                                        vcsUint32_t paramBlobSize);

typedef vcsBool_t (VCS_API * scsSensorEnumPrintParamsCB_t)(const vcsUint8_t * pPrintParam,
                                                           void *             pUserData);

vcsResult_t VCS_API
scsSensorEnumPrintParams(const vcsUint8_t *pPrintParamBlob,
                         vcsUint32_t blobSize,
                         scsSensorEnumPrintParamsCB_t pEnumCallback,
                         void *pUserData);
vcsResult_t VCS_API
scsSensorFindPrintParam(const vcsUint8_t *pPrintParamBlob,
                        vcsUint32_t blobSize,
                        vcsUint16_t paramId,
                        const vcsUint8_t **ppParam);

vcsResult_t VCS_API
scsSensorGetCountedLinesSynch(
    scsSensorHandle_t       hSensor,
    vcsUint32_t             lineLength,
    vcsUint32_t             numScanLines,
    vcsUint32_t             calibrationType,
    vcsUint32_t             flags,
    scsSensorConfigHandle_t pGetPrintCfg,
    vcsBlobData_t *         pConfigReply,
    vcsBlobData_t *         pBuffer);

vcsResult_t VCS_API
scsSensorTLSResumeSession(
    scsSensorHandle_t   hSensor);

vcsResult_t VCS_API
scsSensorTLSSuspendSession(
    scsSensorHandle_t   hSensor,
    vcsBool_t           bSendCloseNotification);

vcsResult_t VCS_API
scsSensorTLSGetSessionStatus(
    scsSensorHandle_t  hSensor,
    vcsUint8_t *       pStatus);

#if VCS_FEATURE_SSL

vcsResult_t VCS_API
scsSensorTlsDataGet(
    scsSensorHandle_t   pSensor,
    vcsBlobData_t *     pTlsData);

vcsResult_t VCS_API
scsSensorTlsDataRestore(
    scsSensorHandle_t   pSensor,
    vcsBlobData_t *     pTlsData);
#endif /* VCS_FEATURE_SSL */

/*
* Metallica secure wrap/unwrap flags.
*/
typedef enum secureWrapUnwrapFlags_e
{
    secureWrapUnwrapFlagsWrap,
    secureWrapUnwrapFlagsUnwrap
} secureWrapUnwrapFlags_t;

vcsResult_t VCS_API
scsSensorSecureWrapUnwrap(
    scsSensorHandle_t       hSensor,
    secureWrapUnwrapFlags_t flags,
    vcsUint8_t *            pInData,
    vcsUint16_t             inDataLength,
    vcsUint8_t *            pOutData,
    vcsUint16_t *           pOutDataLength);

vcsResult_t VCS_API
scsSensorAutonomusModeStateGet(
    scsSensorHandle_t           hSensor,
    vcsUint32_t                 sessionId,
    vcsUint32_t *               pStatusCode,
    vcsUint32_t *               pErrorCode,
    scsSensorMocStatInfo_t *    pMocStatInfo);

vcsResult_t VCS_API
scsSensorAuthenticationStateGet(
    scsSensorHandle_t   hSensor,
    vcsUint32_t         sessionId,
    vcsUint16_t *       pSessionStatus,
    vcsResult_t *       pParam);

vcsResult_t VCS_API
scsSensorEOCStateGet(
    scsSensorHandle_t   hSensor,
    vcsUint16_t *       pStatusCode,
    vcsResult_t *       pErrorCode,
    scsSensorEnrollSessionStatusDbgInfo_t * pStatusDbgInfo);

/*vcsResult_t VCS_API
scsSensorGetNavFingerStatus(
    scsSensorHandle_t   hSensor,
    scsSensorGenericCB_t    *CBFuncPtr,
    void * cbd
   );*/

vcsResult_t VCS_API
scsSensorGetNavStateCB(
    vcsSensorCommandReplyData_t *   pReplyData,
    vcsResult_t                     status,
    void *                          pUserData);

vcsResult_t VCS_API
scsSensorGetNavFingerStateCB(
    vcsSensorCommandReplyData_t *   pReplyData,
    vcsResult_t                     status,
    void *                          pUserData);

vcsResult_t VCS_API
scsSensorGetSensorOrientation(scsSensorHandle_t   hSensor,vcsUint32_t* orientation);

vcsResult_t VCS_API
scsSensorEnableOrDisableNavigationMode(
    scsSensorHandle_t pSensor,
    vcsBool_t bState);

vcsResult_t VCS_API
scsSensorSetIdleTimer(
    scsSensorHandle_t   hSensor,
    vcsBool_t           status);

vcsResult_t VCS_API
scsSensorDeviceInitialize(
    scsSensorHandle_t   hSensor,
    vcsBool_t           bEocCapable,
    vcsBool_t           bFastInit);

extern vcsResult_t VCS_API
scsSensorSwitchToFpMode(
    scsSensorHandle_t   hSensor,
    vcsUint16_t         lineLength);

extern vcsResult_t VCS_API
scsSensorGetPrintCfgUpdate(
    scsSensorHandle_t   hSensor);

extern vcsResult_t VCS_API
scsSensorBaselineCfgUpdate(
    scsSensorHandle_t   hSensor);

extern vcsResult_t VCS_API
scsSensorCreateWofConfig(
    scsSensorHandle_t   hSensor,
    vcsBlobData_t     * pParam);

vcsResult_t VCS_API
scsSensorWofConfigure(
    scsSensorHandle_t   hSensor,
    vcsUint8_t          fdMode);

vcsResult_t VCS_API
scsSensorWofStatusGet(
    scsSensorHandle_t   hSensor,
    vcsUint8_t *        fstate,
    vcsUint8_t *        fevent);

vcsResult_t VCS_API
scsSensorWofRxSelect(
    scsSensorHandle_t   hSensor);

vcsResult_t VCS_API
scsSensorAutorestartStatusGet(
    scsSensorHandle_t   hSensor,
    vcsUint8_t *        pFFcount);

vcsResult_t VCS_API
scsSensorSetFdetectSensitivity(
    scsSensorHandle_t   hSensor,
    const vcsUint32_t   sensitivity);

vcsResult_t VCS_API
scsSensorCpuSpeedup(
   scsSensorHandle_t   hSensor,
   vcsBool_t           bSpeedup);

/* ***************************************************************************
 *          Security management specific functions and definitions           *
 ****************************************************************************/

/**< The RSA key maximal length */
#define SCS_SECMGMT_MAX_RSA_KEY_LENGTH   (2048 >> 3)

/**< ECC parameter maximal length */
#define SCS_SECMGMT_MAX_ECC_PARAM_LEN    68

/* The public key certificate type.*/
#define SCS_SECMGMT_PUBLIC_KEY_CERTIFICATE_TYPE_RSA ((vcsUint32_t) 0)
#define SCS_SECMGMT_PUBLIC_KEY_CERTIFICATE_TYPE_ECC ((vcsUint32_t) 1)

/**< Security state info */
typedef struct scsSecMgmtGetInfo_s
{
    vcsBool_t   bOwned;
    vcsBool_t   bProvisioned;
} scsSecMgmtGetInfo_t;

/**< The RSA certificate */
typedef struct scsRsaCertificate_s
{
    vcsUint32_t keyLen;                                     /* In bytes   */
    vcsUint32_t pubExp;
    vcsUint8_t  modulus[SCS_SECMGMT_MAX_RSA_KEY_LENGTH];    /* Public key */
    vcsUint8_t  sign[SCS_SECMGMT_MAX_RSA_KEY_LENGTH];       /* RSA sign   */
} scsRsaCertificate_t;

/**< The ECC certificate */
typedef struct scsEccCertificate_s
{
    vcsUint32_t keyLen;                                     /* In bytes.  */
    vcsUint32_t curveId;                                    /* Curve ID   */
    vcsUint8_t  x[SCS_SECMGMT_MAX_ECC_PARAM_LEN];           /* Public key */
    vcsUint8_t  y[SCS_SECMGMT_MAX_ECC_PARAM_LEN];
    vcsUint8_t  sign[SCS_SECMGMT_MAX_RSA_KEY_LENGTH];       /* RSA sign */
} scsEccCertificate_t;

typedef struct scsSensorCertificate_s
{
    vcsUint32_t type;
    union
    {
        scsRsaCertificate_t rsaCert;
        scsEccCertificate_t eccCert;
    } data;
} scsSensorCertificate_t;

vcsResult_t VCS_API
scsSecMgmtGetSensorCertificate(
    scsSensorHandle_t           hSensor,
    scsSensorCertificate_t *    pubKeyCert);

vcsResult_t VCS_API
scsSecMgmtGetInfo(
    scsSensorHandle_t       hSensor,
    scsSecMgmtGetInfo_t *   pSecurityInfoGet);


#define SCS_SECMGMT_DO_PAIR_STATE_UNKNOWN       0
#define SCS_SECMGMT_DO_PAIR_STATE_COMPLETE      1
#define SCS_SECMGMT_DO_PAIR_STATE_REPEAT        2
#define SCS_SECMGMT_DO_PAIR_STATE_NEED_RESET    3

extern vcsResult_t VCS_API
scsSecMgmtDoPair(
    scsSensorHandle_t   hSensor,
    vcsUint8_t *        pState,
    vcsBlobData_t *     pIntx,
    vcsBlobData_t *     pOutCtx);

vcsResult_t VCS_API
scsSecMgmtUnPair(
    scsSensorHandle_t   hSensor,
    vcsUint8_t *        pState);

/* ***************************************************************************
 *      End of Security management specific functions and definitions        *
 ****************************************************************************/

vcsResult_t VCS_API
scsSensorIfsStatsGet(scsSensorHandle_t       hSensor);
/*!
**************************************************************************************************
** Find OTPROM tag(s) of specified type in Sensor OTP ROM. This is a synchronous call and does not
** complete until the sensor responds with a status for performing the operation.
**
** @param[in]       hSensor
**      A handle to an open sensor. This handle is obtained from a successfully
**      scsSensorOpen call.
** @param[in]   flags
**      Flags indicating where to find tags in OTPROM
**          VCSFW_CMD_OTPROM_TAG_FIND_FLAGS_CHAINS_BOTH     0
**          VCSFW_CMD_OTPROM_TAG_FIND_FLAGS_CHAINS_ONLYLOW  1
**          VCSFW_CMD_OTPROM_TAG_FIND_FLAGS_CHAINS_ONLYHIGH 2
** @param[in]   tag
**      Tag id to find. Value 0 finds all tags
** @param[in]   nMaxTagsToFind
**      Maximum no.of found tags to be returned. Value 0 returns all tags found
** @param[in]   firstIdx
**      Starting index in found tags set to return results
** @param[in]   extendedTag
**      Extended tag value to find. Used if .tag = VCSFW_OTPROM_TAGTYPE_WINDSOR_EXTENDED
**
** @param[out]  pFreeSpace
**      Number of bytes between chains
** @param[out]  pnTagsFound
**      Number of tags found
** @param[out]  pOtpRomTagElements
**      Array of OtpRomTagElements
** caller should free allocated memory for pOtpRomTagElements 
**      and memory allocated for tag data (pointed by pData of blob) for each element
*/
vcsResult_t VCS_API
scsSensorOtpRomTagFind(
    scsSensorHandle_t       hSensor,
    vcsUint8_t              flags,
    vcsUint8_t              tag,
    vcsUint8_t              nMaxTagsToFind,
    vcsUint8_t              firstIdx,
    vcsUint32_t             extendedTag,
    vcsUint32_t             *pFreeSpace,
    vcsUint32_t              *pnTagsFound,
    scsOtpRomTagElement_t   **pOtpRomTagElements
    );

vcsResult_t VCS_API
scsSensorAppRequest(
    scsSensorHandle_t   hSensor,
    vcsUint32_t         appId,
    vcsUint32_t         requestDataLength,
    vcsUint8_t *        pRequestData,
    vcsUint32_t *       pResponseDataLength,
    vcsUint8_t *        pResponseData);

vcsResult_t VCS_API
scsSensorCalWof2(
    scsSensorHandle_t   hSensor,
    scsCmdCalWof2_t     *pCalWof2Param,
    vcsBlobData_t       *pCalWof2ReplyBlob);


vcsResult_t VCS_API
scsFreeotpROMData(
    scsOtpRomTagElement_t   *pOtpRomTagElements,
    vcsUint32_t    nElements);

vcsResult_t VCS_API
scsFreeCalParams(
    scsCalibrationData_t *  pCalParams);

vcsResult_t VCS_API
scsSensorFrameRead(
    scsSensorHandle_t   hSensor,
    vcsBlobData_t *     pFrame,
    vcsBool_t *         bLastFrame,
    vcsUint16_t *       pReadFrameIndex);

#define SCS_FRAME_ACQ_FLAGS_DEFAULT         ((vcsUint32_t) 0x00000000)
#define SCS_FRAME_ACQ_FLAGS_INFINITE        ((vcsUint32_t) 0x00000001)
#define SCS_FRAME_ACQ_FLAGS_CALIBRATE       ((vcsUint32_t) 0x00000002)

vcsResult_t VCS_API
scsSensorFrameAcquire(
    scsSensorHandle_t   hSensor,
    vcsUint32_t         flags,
    vcsUint32_t         mode,
    vcsUint16_t         numFrames,
    vcsBlobData_t       tagDataBlob);

vcsResult_t VCS_API
scsSensorFrameFinish(
    scsSensorHandle_t   hSensor);

vcsResult_t VCS_API
scsSensorFrameDimensionGet(
    scsSensorHandle_t           hSensor,
    scsSensorFrameDimension_t * pDim);

vcsResult_t VCS_API
scsSensorTempCorrectionFactorGet(
    scsSensorHandle_t           hSensor,
    scsSensorTempCorrectionFactor_t * pTempCorrFactor);

vcsResult_t VCS_API
scsSensorPixIdGet(
    scsSensorHandle_t           hSensor);

#define SCS_EVENT_TYPE_NOOP                 0x00
#define SCS_EVENT_TYPE_FINGERDOWN           0x01
#define SCS_EVENT_TYPE_FINGERUP             0x02
#define SCS_EVENT_TYPE_NAV_TAP              0x03
#define SCS_EVENT_TYPE_NAV_DTAP             0x04
#define SCS_EVENT_TYPE_NAV_LONGPRESS        0x05
#define SCS_EVENT_TYPE_NAV_SWIPE_UP         0x06
#define SCS_EVENT_TYPE_NAV_SWIPE_DOWN       0x07
#define SCS_EVENT_TYPE_NAV_SWIPE_LEFT       0x08
#define SCS_EVENT_TYPE_NAV_SWIPE_RIGHT      0x09
#define SCS_EVENT_TYPE_OVERFLOW             0x7f

#define SCS_EVENTS_MAX_NUM                  32

#define SCS_EVENT_CONFIG_FLAG_SIGNAL_FPSTATE_ABSENT     0x00000001
#define SCS_EVENT_CONFIG_FLAG_SIGNAL_FPSTATE_PRESENT    0x00000002

typedef struct scsSensorEventConfigData_s
{
    vcsUint32_t         flags;
    vcsUint32_t         timeScale;
    vcsUint32_t         nMax;
    vcsUint16_t         nPending;
    vcsUint16_t         nextNum;
}scsSensorEventConfigData_t;

vcsResult_t VCS_API
scsSensorEventConfig(
    scsSensorHandle_t               hSensor,
    const vcsUint8_t *              pEvents,
    vcsUint32_t                     nEventsNum,
    scsSensorEventConfigData_t *    pEvnConfigData);

typedef struct VCS_PACKED scsSensorEventData_s {
    vcsUint8_t          type; /* One of SCS_EVENT_TYPE... */
    vcsUint32_t         time;
} scsSensorEventData_t;

vcsResult_t VCS_API
scsSensorEventRead(
    scsSensorHandle_t       hSensor,
    scsSensorEventData_t *  pEvents,
    vcsUint32_t *           pEventNum, /*[ in/out] */
    vcsUint32_t *           pPendingEventNum);

vcsResult_t VCS_API
scsSensorLnaBlGet(
    scsSensorHandle_t   hSensor,
    vcsBlobData_t *     pLnaBl);

vcsResult_t VCS_API
scsSensorFrameStream(
    scsSensorHandle_t   hSensor,
    vcsBlobData_t *     pFrame,
    vcsBool_t *         bEndOfFrame,
    vcsBool_t *         bEndOfStream,
    vcsBool_t *         bIllum,
    vcsBool_t *         bExtFp,
    vcsBool_t *         bWasSunlightCfgUsed,
    vcsUint16_t *       pFrameNum,
    vcsUint32_t *       pDataLost);

vcsResult_t VCS_API
scsSensorIotaRead(
    scsSensorHandle_t   hSensor,
    vcsBlobData_t *     pIotaData,
    vcsUint16_t         iotaType,
    vcsUint16_t         flags);

vcsResult_t VCS_API
scsSensorRefImageGet_Calibration_type_0(
    scsSensorHandle_t   hSensor,
    vcsBlobData_t *     pRefImage);


vcsResult_t VCS_API
scsSensorRefImageGet(
    scsSensorHandle_t   hSensor,
    vcsBlobData_t *     pRefImage);

vcsResult_t VCS_API
scsSensorGetBackGroundImage(
    scsSensorHandle_t   hSensor,
    vcsBlobData_t *     pRefImage);

vcsResult_t VCS_API
scsSensorGetDarkCurrentImage(
    scsSensorHandle_t   hSensor,
    vcsBlobData_t *     pRefImage);

vcsResult_t VCS_API
scsSensorGetBadPixelMap(
    scsSensorHandle_t   hSensor,
    vcsBlobData_t *     pRefImage);

vcsResult_t VCS_API
scsSensorPointerToDarkRateMap(
    scsSensorHandle_t   hSensor,
    vcsBlobData_t *     pRefImage,
    vcsBlobData_t *     pDataCpidRefImageWithoutImageCalHeader);

vcsResult_t VCS_API
scsSensorDarkRateMapGet(
    scsSensorHandle_t   hSensor,
    vcsBlobData_t *     pRefImage);

vcsResult_t VCS_API
    scsSensorPointerToMeanAndBadPixelMap(
        scsSensorHandle_t   hSensor,
        vcsBlobData_t *     pRefImage,
        vcsBlobData_t *     pDataCpidRefImageWithoutImageCalHeader,
        vcsBlobData_t *     pDataCpidBadPixelMapWithoutMapHeader);
    
    vcsResult_t VCS_API
    scsSensorMeanAndBadPixelMapGet(
        scsSensorHandle_t   hSensor,
        vcsBlobData_t *     pRefImage);

vcsResult_t VCS_API
scsSensorPointerToDarkCurrentImage(
scsSensorHandle_t   hSensor,
vcsBlobData_t *     pRefImage,
vcsBlobData_t *     pDataCpidRefImageWithoutImageCalHeader,
vcsUint32_t			nFrameLength);

vcsResult_t VCS_API
scsSensorPointerToMeanImage(
scsSensorHandle_t   hSensor,
vcsBlobData_t *     pRefImage,
vcsBlobData_t *     pDataCpidMeanImageWithoutMapHeader,
vcsUint32_t			nFrameLength);


void VCS_API
scsSensorExtractFrame16(
    vcsUint8_t *                pRawFrame, 
    scsSensorFrameDimension_t * pFrameDim,
    vcsInt16_t *                pFrame, 
    vcsUint32_t                 frameWidth,
    vcsUint32_t                 frameHeight);

void VCS_API
scsSensorExtractFrame12(
    vcsUint8_t *                pRawFrame, 
    scsSensorFrameDimension_t * pFrameDim,
    vcsInt16_t *                pFrame, 
    vcsUint32_t                 frameWidth,
    vcsUint32_t                 frameHeight);

void VCS_API
scsSensorExtractFrame12NoWordAlign(
    vcsUint8_t *pRawFrame, 
    vcsInt16_t *pFrame,
    vcsUint32_t frameWidth,
    vcsUint32_t frameHeight );

vcsResult_t VCS_API
scsSensorFrameTagsGet(
    scsSensorHandle_t   hSensor,
    vcsBlobData_t *     pFrameTags);

extern vcsResult_t VCS_API
scsSensorEnterBootLoaderMode(
    scsSensorHandle_t hSensor);

extern vcsResult_t VCS_API
scsSensorExitBootLoaderMode(
    scsSensorHandle_t hSensor);

#define SCS_SENSOR_UPDATE_STATUS_NONE           0
#define SCS_SENSOR_UPDATE_STATUS_NEED_UPDATE    1

extern vcsResult_t VCS_API
scsSensorUpdateInit(
    scsSensorHandle_t           hSensor,
    const vcsConstBlobData_t *  pFw,
    const vcsConstBlobData_t *  pCfg,
    vcsUint32_t *               pStatus);

#define SCS_SENSOR_UPDATE_STATE_UNKNOWN     0
#define SCS_SENSOR_UPDATE_STATE_COMPLETE    1
#define SCS_SENSOR_UPDATE_STATE_REPEAT      2
#define SCS_SENSOR_UPDATE_STATE_NEED_RESET  3

extern vcsResult_t VCS_API
scsSensorUpdateProcess(
    scsSensorHandle_t   hSensor,
    vcsUint8_t *        pState,
    vcsBlobData_t *     pInCtx,
    vcsBlobData_t *     pOutCtx);

extern vcsResult_t VCS_API
scsSensorInitFrameAcq(
    scsSensorHandle_t hSensor);

#endif /* __vcsScs_h__ */

