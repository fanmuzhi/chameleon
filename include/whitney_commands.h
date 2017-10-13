/* -*- mode: c; tab-width: 4 -*- */ /* $Header$ */

/*
 * INTERNAL USE ONLY
 *
 * Copyright (c) 2016-2017 Synaptics Incorporated.  All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated ("Synaptics"). The holder of this file shall treat all
 * information contained herein as confidential, shall use the
 * information only for its intended purpose, and shall not duplicate,
 * disclose, or disseminate any of this information in any manner unless
 * Synaptics has otherwise provided express, written permission.
 */

/*
 * whitney_commands.h -- This file defines the externally accessible command
 *  mechanism.  It is inherited from the Nassau Mission Firmware code base,
 *  ported to the Chimera environment and the Billion Unit Sensor (BUS) project.
 *
 * The file is based on the Validity Command Set (VCS) supported for the Nassau
 * products, the opcodes are similar, but there is no guarantee of backward
 * compatabilty.  This is a new command set.
 *
 */

#ifndef __WHITNEY_COMMANDS_H
#define __WHITNEY_COMMANDS_H

/****************************************************************************
 * Supported Commands:
 * These COMMANDS are used to communicate with the sensor device fw.
 *
 * NOTE: The original allocation of opcodes is based on the command opcodes
 * originally used by the Validity Command Set on the Nassau products.  For
 * those commands that were inherited from Nassau, the same command opcode is
 * used.
****************************************************************************/
#define WFW_CMD_NOOP                          0x00
#define WFW_CMD_GET_VERSION                   0x01
#define WFW_CMD_RESET                         0x05
#define WFW_CMD_PATCH                         0x06
#define WFW_CMD_PEEK                          0x07
#define WFW_CMD_POKE                          0x08
#define WFW_CMD_TIDLE_SET                     0x57
#define WFW_CMD_ECHO                          0x7c
#define WFW_CMD_IOTA_WRITE                    0x83
#define WFW_CMD_EVENT_READ                    0x87
#define WFW_CMD_IOTA_FIND                     0x8e
#define WFW_CMD_IOTA_INSINUATE                0x90
#define WFW_CMD_PUBK_GET                      0x91

// The Whitney Specific Commands start at opcode 146.  There is no guarantee
// that there will not be future collision with the Validity Command Set as
// defined in the Nassau code base starting at address 146.
#define WFW_CMD_STREAM_RAW_DATA               0x92
#define WFW_CMD_GET_FRAME                     0x93
#define WFW_CMD_EVENT_NOTIFY                  0x94
#define WFW_CMD_EVENT_READ_CONFIG             0x95
#define WFW_CMD_GET_TIMESTAMP                 0x96
#define WFW_CMD_GET_SYS_INFO                  0x97

/*
 * These following commands are for ATE, characterization and firmware test
 * patches the also get used in the USDK5 for device specific calibration.
 */
#define WFW_CMD_ATE_BULK_READ                 0xee
#define WFW_CMD_TEST_PARAMETER_SET            0xfa
#define WFW_CMD_TEST_CONTINUE                 0xfb
#define WFW_CMD_TEST_LENGTH_READ              0xfc
#define WFW_CMD_TEST_READ                     0xfd
#define WFW_CMD_TEST_RUN                      0xfe

#define WFW_CMD_INVALID                       0xff


/****************************************************************************
 *  Returned Status values.
 ****************************************************************************/
#define WFW_STATUS_OKAY                     0
#define WFW_STATUS_OK                       WFW_STATUS_OKAY

#define WFW_STATUS_ERR_FLAG                 0x0400

//Each error code below starts at 0x400 and counts up... Each code from 0x0401
//through 0x0618 is inherited from Nassau and is being used as per Nassau's
//definition.
#define WFW_STATUS_ERR_INVALID_CMD                  0x0401
#define WFW_STATUS_ERR_INVALID_PARAM                0x0403
#define WFW_STATUS_ERR_DENIED                       0x0404
#define WFW_STATUS_ERR_CMDTOOSHORT                  0x0405
#define WFW_STATUS_ERR_CMDTOOLONG                   0x0406
#define WFW_STATUS_ERR_TIDLE_SET_OUTOFRANGE         0x050c
#define WFW_STATUS_ERR_IOTA_WRITE_CHAINLOCKED       0x05d0
#define WFW_STATUS_ERR_IOTA_WRITE_OUTOFSPACE        0x05d1
#define WFW_STATUS_ERR_IOTA_WRITE_BADLENGTH         0x05d2
#define WFW_STATUS_ERR_IOTA_WRITE_FLASHUNLOCK       0x05d3
#define WFW_STATUS_ERR_IOTA_WRITE_FLASHWRITE        0x05d4
#define WFW_STATUS_ERR_IOTA_WRITE_TOOSHORT          0x05d5
#define WFW_STATUS_ERR_IOTA_CRC_MISMATCH            0x063c


//For other errors 0x0402 through 0x0618, see the Nassau code
//base for the list.

//These are new codes used for Whitney.  At the time these codes were selected,
//there was no equivalent in the Validity Command Set as implemented in the Nassau code
//base.  No guarantee is being made that these will never be used in the
//Validity Command Set
#define WFW_STATUS_ERR_EP1IN_UNDERRUN             0x0719
#define WFW_STATUS_ERR_EP1IN_NOT_READY            0x071a
#define WFW_STATUS_ERR_NO_COMMAND_BYTE            0x071b
#define WFW_STATUS_ERR_EP1OUT_NOT_READY           0x071c
#define WFW_STATUS_ERR_BAD_COMM_STATE             0x071d
#define WFW_STATUS_ERR_NO_CONTROL_BYTE            0x071e
#define WFW_STATUS_ERR_INVALID_CTL_BYTE           0x071f
#define WFW_STATUS_ERR_UNKNOWN_INTERRUPT          0x0720
#define WFW_STATUS_ERR_EP1IN_OVERRUN              0x0721
#define WFW_STATUS_ERR_XFER_ERROR                 0x0722
#define WFW_STATUS_ERR_UNINITIALIZED_CMD          0x0723
#define WFW_STATUS_ERR_BAD_ALIGN                  0x0724
#define WFW_STATUS_ERR_BOOTLDR_PATCH_XFER_ERROR   0x0725
#define WFW_STATUS_ERR_BOOTLDR_PATCH_TOO_LONG     0x0726
#define WFW_STATUS_ERR_BOOTLDR_PATCH_BAD_ALIGN    0x0727
#define WFW_STATUS_ERR_BOOTLDR_PATCH_NO_PAYLOAD   0x0728
#define WFW_STATUS_ERR_PATCH_SHORTHDR             0x0730
#define WFW_STATUS_ERR_PATCH_NO_PAYLOAD           0x0731
#define WFW_STATUS_ERR_PATCH_DUPLICATE_TAG        0x0732
#define WFW_STATUS_ERR_PATCH_INVALID_TAG          0x0733
#define WFW_STATUS_ERR_PATCH_DATA_TOO_LONG        0x0734
#define WFW_STATUS_ERR_PATCH_DATA_BAD_ALIGN       0x0735
#define WFW_STATUS_ERR_PATCH_SECLEVEL_LENGTH      0x0736
#define WFW_STATUS_ERR_PATCH_SIGNATURE_LENGTH     0x0737
#define WFW_STATUS_ERR_PATCH_IV_LENGTH            0x0738
#define WFW_STATUS_ERR_PATCH_INVALID_SECLEVEL     0x0739
#define WFW_STATUS_ERR_PATCH_UNEXPECTED_SIGNATURE 0x073a
#define WFW_STATUS_ERR_PATCH_UNEXPECTED_IV        0x073b
#define WFW_STATUS_ERR_PATCH_NOT_ENCRYPTED        0x073c
#define WFW_STATUS_ERR_PATCH_NOT_SIGNED           0x073d
#define WFW_STATUS_ERR_PATCH_NOT_PROD             0x073e
#define WFW_STATUS_ERR_PATCH_NO_IV                0x073f
#define WFW_STATUS_ERR_PATCH_VERIFICATION_FAILED  0x0740
#define WFW_STATUS_ERR_IOTA_FIND_INVALID_ITYPE    0x0742
#define WFW_STATUS_ERR_IOTA_FIND_BAD_NBYTES       0x0743
#define WFW_STATUS_ERR_IOTA_FIND_TOO_LONG         0x0744
#define WFW_STATUS_ERR_IOTA_FIND_BAD_OFFSET       0x0745
#define WFW_STATUS_ERR_IOTA_FIND_OTP_READ_FAILED  0x0746
#define WFW_STATUS_ERR_INVALID_EVENT              0x0747
#define WFW_STATUS_ERR_CONFLICTING_EVENTS         0x0748
#define WFW_STATUS_ERR_INVALID_FRAME_TYPE         0x0749
#define WFW_STATUS_ERR_FRAME_READ_NOTYET          0x074a
#define WFW_STATUS_ERR_FRAME_READ_NOTRUNNING      0x074b
#define WFW_STATUS_ERR_IOTA_UNRECOGNIZED_TYPE     0x074c
#define WFW_STATUS_ERR_IOTA_MALFORMATTED          0x074e
#define WFW_STATUS_ERR_IOTA_DM_TOO_LONG           0x074f
#define WFW_STATUS_ERR_IOTA_DM_BAD_LENGTH         0x0750
#define WFW_STATUS_ERR_IOTA_REGBLOCK_INVALID_ADDR 0x0751
#define WFW_STATUS_ERR_IOTA_REGBLOCK_MALFORMATTED 0x0752
#define WFW_STATUS_ERR_IOTA_FRAME_TOO_BIG         0x0753
#define WFW_STATUS_ERR_WOF_NOTCONFIGURED          0x0754

//Whitney-specific definitions

/* values for wfw_reply_get_version_t::target */
#define WFW_TARGET_ROM              1
#define WFW_TARGET_PATCH_RAM        6

/* values for wfw_reply_get_version_t::product */
#define WFW_PRODUCT_WHITNEY         62

/* values for wfw_reply_get_version_t::interface */
#define WFW_IFACE_SPI               3

/* The following bits describe security options in
** wfw_reply_get_version_t::security[1] bit-field */
#define WFW_SECURITY1_PROVISIONED      (1 << 0)
#define WFW_SECURITY1_FA               (1 << 2)
#define WFW_SECURITY1_PROD_SENSOR      (1 << 5)
#define WFW_SECURITY1_SECCOM_ENFORCED  (1 << 7)

/****************************************************************************
 *  Structures used to manage command data.  These are largely ported directly
 *  from the Nassau code base, but there are numerous changes. The specification
 *  for each of these is in Whitney_Commands.docx.
 ****************************************************************************/

/****************************************************************************
 * WFW_CMD_GET_VERSION
 ****************************************************************************/
typedef struct WFW_PACKED wfw_reply_get_version_s
{
  wfwUint32_t   buildtime;        // Unix-style build time, in seconds
                                  //  from 1/1/1970 12:00 AM GMT
  wfwUint32_t   buildnum;         // build number
  wfwUint8_t    vmajor;           // major version
  wfwUint8_t    vminor;           // minor version
  wfwUint8_t    target;           // target, e.g. WFW_TARGET_ROM
  wfwUint8_t    product;          // product, e.g.  WFW_PRODUCT_WHITNEY
  wfwUint8_t    siliconrev;       // silicon revision
  wfwUint8_t    formalrel;        // boolean: non-zero -> formal release
  wfwUint8_t    platform;         // Platform (PCB) revision
  wfwUint8_t    patch;            // patch level
  wfwUint8_t    serial_number[6]; // 48-bit Serial Number
  wfwUint8_t    security[2];      // bytes 0 and 1 of OTP
  wfwUint32_t   patchsig;         // opaque patch signature
  wfwUint8_t    iface;            // interface type, see below
  wfwUint8_t    otpsig[3];        // OTP Patch Signature
  wfwUint16_t   otpspare1;        // spare space
  wfwUint8_t    reserved;         // reserved byte
  wfwUint8_t    device_type;      // device type
} wfw_reply_get_version_t;

/****************************************************************************
 * WFW_CMD_RESET
 ****************************************************************************/
typedef struct WFW_PACKED wfw_cmd_reset_s
{
  wfwUint16_t delaytime;  // time in msec to delay after reply
} wfw_cmd_reset_t;

typedef struct WFW_PACKED wfw_reply_reset_s
{
  wfwUint16_t delaytime;  // time in msec that will be delayed prior to reset
} wfw_reply_reset_t;


/****************************************************************************
 * WFW_CMD_PATCH
 ****************************************************************************/
/* COMMAND                                                                  */
/*
 * A patch consists of a set of segments in the form
 *  {tag, length, data}.  See possible tags listed below.
 */
#define WFW_CMD_PATCH_HEADER_SIZE_BYTES 4
typedef struct WFW_PACKED wfw_cmd_patch_s
{
    wfwUint16_t   tag;
    wfwUint16_t   length;
} wfw_cmd_patch_t;

/*
 * A list of supported values for the TAG field
 */
#define WFW_TAG_PATCH_DATA             0x0000
/* Format: WFW_TAG_PATCH_DATA + 2-bytes-length + data... */
#define WFW_TAG_PATCH_SECLEVEL         0x0001
/* Format: WFW_TAG_PATCH_SECLEVEL + 2-bytes-length (always = 2) + 2-byte-level... */
#define WFW_TAG_PATCH_SIGNATURE        0x0002
/* Format: WFW_TAG_PATCH_DATA + 2-bytes-length (always = 16) + 16-byte signature... */
#define WFW_TAG_PATCH_IV               0x0003
/* Format: WFW_TAG_PATCH_IV + 2-bytes-length (always = 12) + 12-byte IV */

/*
 * A list of supported values for the SECLEVEL tag.
 */
#define WFW_PATCH_SECLEVEL_OPEN  0  /* default */
#define WFW_PATCH_SECLEVEL_ENG   1
#define WFW_PATCH_SECLEVEL_PROD  2


/****************************************************************************
 * WFW_CMD_PEEK
 ****************************************************************************/
typedef struct WFW_PACKED wfw_cmd_peek_s
{
  wfwUint16_t address;
  wfwUint16_t reserved;
  wfwUint8_t  opsize;
} wfw_cmd_peek_t;

typedef struct WFW_PACKED wfw_reply_peek_s
{
  wfwUint32_t value;
} wfw_reply_peek_t;

/****************************************************************************
 * WFW_CMD_POKE
 ****************************************************************************/
typedef struct WFW_PACKED wfw_cmd_poke_s
{
  wfwUint16_t address;
  wfwUint16_t reserved;
  wfwUint32_t value;
  wfwUint8_t  opsize;
} wfw_cmd_poke_t;

/****************************************************************************
 * WFW_CMD_TIDLE_SET
 ****************************************************************************/
typedef struct WFW_PACKED wfw_cmd_tidle_set_s
{
  wfwUint32_t timer_val;  //value in units of 80usec
} wfw_cmd_tidle_set_t;


/****************************************************************************/
/* WFW_CMD_IOTA_WRITE                                                     */
/****************************************************************************/

#define WFW_CMD_IOTA_WRITE_HEADER_SIZE_BYTES 4
typedef struct WFW_PACKED wfw_cmd_iota_write_s {
     wfwUint16_t     itype;
     wfwUint8_t      unused[2];
} wfw_cmd_iota_write_t;

/****************************************************************************/
/* WFW_CMD_IOTA_FIND                                                      */
/****************************************************************************/

/*
 * The IOTA_FIND command.  Iotas can exceed the size of available
 *  RAM in the part.  In order to allow the host to read them
 *  the IOTA_FIND command supports transferring iotas with
 *  multiple commands.
 */
typedef struct WFW_PACKED wfw_cmd_iota_find_s
{
    wfwUint16_t     itype;      /* type of iotas to find */
    wfwUint16_t     flags;      /* flags, see below */
    wfwUint8_t      maxniotas;  /* maximum number of iotas to return.
                                 *  0 = unlimited */
    wfwUint8_t      firstidx;   /* first index of iotas to return */
    wfwUint8_t      dummy[2];
    wfwUint32_t     offset;     /* byte offset of data to return */
    wfwUint32_t     nbytes;     /* maximum number of bytes to return */
} wfw_cmd_iota_find_t;

#define WFW_CMD_IOTA_FIND_FLAGS_ALLIOTAS  0x0001    /* itype ignored*/
#define WFW_CMD_IOTA_FIND_FLAGS_READMAX   0x0002    /* nbytes ignored */

typedef struct WFW_PACKED wfw_reply_iota_find_s
{
    wfwUint32_t      fullsize;  /* size of all data */
} wfw_reply_iota_find_t;

/* This is followed by a chain of iotas with headers */


/****************************************************************************/
/* WFW_CMD_IOTA_INSINUATE                                                 */
/****************************************************************************/

typedef struct WFW_PACKED wfw_cmd_iota_insinuate_s
{
    wfwUint16_t     itype;      /* type of iota being insinuated */
    wfwUint8_t      unused[2];
} wfw_cmd_iota_insinuate_t;

/* IOTA_INSINUATE returns only the generic reply */


/****************************************************************************
 * WFW_CMD_ATE_BULK_READ
 ****************************************************************************/
typedef struct WFW_PACKED wfw_cmd_bulk_read_s
{
  wfwUint32_t address;
  wfwUint32_t size;
} wfw_cmd_bulk_read_t;


/****************************************************************************/
/* WFW_CMD_EVENT_READ_CONFIG                                                     */
/****************************************************************************/

//Each of these event type opcodes is inherited from the Nassau code base.
#define WFW_EVENT_TYPE_NOOP               0x00
#define WFW_EVENT_TYPE_FINGERDOWN         0x01
#define WFW_EVENT_TYPE_FINGERUP           0x02
#define WFW_EVENT_TYPE_TDL_TAP            0x03
#define WFW_EVENT_TYPE_TDL_DTAP           0x04
#define WFW_EVENT_TYPE_TDL_LONGPRESS      0x05
#define WFW_EVENT_TYPE_NAV_SWIPE_UP       0x06
#define WFW_EVENT_TYPE_NAV_SWIPE_DOWN     0x07
#define WFW_EVENT_TYPE_NAV_SWIPE_LEFT     0x08
#define WFW_EVENT_TYPE_NAV_SWIPE_RIGHT    0x09
#define WFW_EVENT_TYPE_OVERFLOW           0x0A
#define WFW_EVENT_TYPE_FINGER_SETTLED     0x0B
#define WFW_EVENT_TYPE_CAL_COMPLETED      0x0C
#define WFW_EVENT_TYPE_FINGER_RESYNC      0x0D
// NOTE: event opcode 0x0E is not used, it is the IOTA_INSINUATED event on Nassau
// products.
#define WFW_EVENT_TYPE_FINGER_REJECTED    0x0F
#define WFW_EVENT_TYPE_NAV_SWIPE_ABORTED  0x10

//These are all new event types to Whitney.  At the time of these event types'
//creation, these event type opcodes were not used in the Validity Command Set
//definition, but no guarantee is being made that collisions will not appear in
//the future.
#define WFW_EVENT_TYPE_FRAME_READY        0x11
#define WFW_EVENT_TYPE_SOFT_RESET         0x12
#define WFW_EVENT_TYPE_SB_0_PRESS         0x13
#define WFW_EVENT_TYPE_SB_0_RELEASE       0x14
#define WFW_EVENT_TYPE_SB_1_PRESS         0x15
#define WFW_EVENT_TYPE_SB_1_RELEASE       0x16
#define WFW_EVENT_TYPE_FORCE_PRESS        0x17
#define WFW_EVENT_TYPE_FORCE_RELEASE      0x18
#define WFW_EVENT_TYPE_FORCE_FRAME_READY  0x19
#define WFW_EVENT_TYPE_LAST_EVENT         WFW_EVENT_TYPE_FORCE_FRAME_READY
#define EVENT_NEVENTTYPES                 (WFW_EVENT_TYPE_LAST_EVENT+1)

/*
 * NOTE: When a new event is added here, the EVENT_SUPPORTED_MASK in eventMgr.h
 * needs to be updated.
 */

/*
 * Macro for getting the soft button event from the soft button number
 */
#define WFW_SB_EVENT_PRESS(x) \
  ((WFW_EVENT_TYPE_SB_0_PRESS) + (x << 1))
#define WFW_SB_EVENT_RELEASE(x) \
  ((WFW_EVENT_TYPE_SB_0_RELEASE) + (x << 1))

/* The command: We don't support an event_config command structure on Whitney.*/

/* The reply: */
typedef wfwUint32_t wfw_event_mask_t[4];
typedef struct WFW_PACKED wfw_reply_event_read_config_s
{
  wfw_event_mask_t  active;     /* bitmask of events to generate */
  wfw_event_mask_t  supported;  /* bitmask of events supported */
  wfwUint32_t       timescale;  /* timescale of event time, in Hz */
  wfwUint32_t       nmax;       /* max number of events buffered */
  wfwUint16_t       npending;   /* number of events in queue now */
  wfwUint16_t       nextnum;    /* next generate event number */
} wfw_reply_event_read_config_t;
#define WFW_RESP_EVENT_READ_CONFIG_SIZE_BYTES BYTE_SIZEOF(wfw_reply_event_read_config_t)


/****************************************************************************
 * WFW_CMD_EVENT_READ
 ****************************************************************************/

/*
 * Event details for specific event occurrences.
 */
#define WFW_CAL_COMPLETED_TYPE_IMAGE  1
#define WFW_CAL_COMPLETED_TYPE_NAV    2
#define WFW_CAL_COMPLETED_TYPE_WOF    3

#define WFW_CAL_COMPLETED_OP_CBC      0x0001
#define WFW_CAL_COMPLETED_OP_BASELINE 0x0002

/*
 * Reasons for FINGER_REJECTED event. One of these values is stored in the details
 * field for this event.
 */
#define WFW_FINGER_REJECTED_COVERAGE_FAIL 0x01
#define WFW_FINGER_REJECTED_IMAGE_READY   0x02


//This union requires some commentary, we have the ability to report the
//number of events that the EventManager has missed.  That'll be in the
//"overflow" structure's nmissed element.  Otherwise, if no events have been
//missed, the "dummy" field can contain any piece of metadata associated with
//the event.
typedef union WFW_PACKED wfw_event_details_s
{
  wfwUint32_t       value;

  //Event details to WFW_EVENT_TYPE_OVERFLOW
  struct
  {
    wfwUint16_t     nmissed;    /* number of missed events */
  } overflow;

  //Event details to WFW_EVENT_TYPE_FINGERUP/DOWN, SB_0/1_PRESS/RELEASE
  struct
  {
    wfwSint16_t     threshold;  //Threshold that was tripped to cause event to
                                //be recorded
    wfwSint16_t     adcVal;     //ADC value that caused threshold to be tripped.
  } finger_state_change_info;

  //Event details to WFW_EVENT_TYPE_CAL_COMPLETED
  struct
  {
    wfwUint16_t     cal_type;   //what imaging subsystem just finished calibration
    wfwUint16_t     cal_operation; //whether CBC, baseline, or both just finished
  } cal_completion_info;

  //Event details to WFW_EVENT_TYPE_FINGER_REJECTED
  struct
  {
    wfwUint16_t     rej_reason;   //why the finger was rejected.
    wfwUint16_t     rej_coverage; //coverage percentage
  } finger_rejected_info;

  //Event details to WFW_EVENT_TYPE_SOFT_RESET
  struct
  {
    wfwUint16_t     reset_condition;
    wfwUint16_t     sanity_code;
  } soft_reset;
} wfw_event_details_t;

typedef struct WFW_PACKED wfw_event_s
{
  wfwUint8_t          type;   /* one of WFW_EVENT_TYPE_* */
  wfwUint8_t          unused1;
  wfwUint16_t         num;    /* sequence number of this event's occurrence */
  wfwUint32_t         time;   /* time, in units of
                               *  wfw_reply_event_read_config_t::timescale
                               *  per second */
  wfw_event_details_t details;
} wfw_event_t;

/* The command: */
typedef struct WFW_PACKED wfw_cmd_event_read_s
{
  //The 16-bit unused field takes up the space of the 'firstnum' field from
  //Nassau.  Event re-transmission is not supported for Whitney; the field
  //remains in order that the overall size of the structure remains unchanged.
  wfwUint16_t         unused;
  wfwUint16_t         nevents;    // number of events requested
} wfw_cmd_event_read_t;

/* The reply: */
typedef struct WFW_PACKED wfw_reply_event_read_s
{
  wfwUint16_t         nevents;    /* number of events to follow */
  wfwUint16_t         npending;   /* number of events in the queue */
  /* This header is followed by an array of nevents of wfw_event_t */
} wfw_reply_event_read_t;

/****************************************************************************
 * WFW_CMD_STREAM_RAW_DATA
 ****************************************************************************/

/* The command: */
typedef struct WFW_PACKED wfw_cmd_stream_raw_data_s {
    wfwUint16_t  frameType;   /* The desired frame type to stream */
} wfw_cmd_stream_raw_data_t;

/* Frame types that are available to stream */
#define FRAME_TYPE_NONE                  0x0000  /* Special case to stop streaming */
#define FRAME_TYPE_FULL_IMAGE            0x0001  /* Full fingerprint image */
#define FRAME_TYPE_NAV                   0x0002  /* Nav image with ganged electrodes */
#define FRAME_TYPE_PARTIAL_SCAN          0x0003  /* Partial scan image for settling detect */
#define FRAME_TYPE_WOF                   0x0004  /* Raw ADC values for WOF */
#define FRAME_TYPE_SOFT_BUTTONS          0x0005  /* Raw ADC values for soft buttons */

#define FRAME_TYPE_CAL_SINGLE_BURST      0x0100  /* Single burst, lsb used to select burst num */

#define FRAME_TYPE_CAL_FULL_IMAGE        0x0200  /* Calibration image (CDM & FW BL bypassed) */
#define FRAME_TYPE_FWBL_FULL_IMAGE       0x0201  /* same as CAL_FULL_IMAGE but keep IMG_AVG_CTRL from IOTA */

/* STREAM_RAW_DATA returns only the generic reply */

/****************************************************************************
 * WFW_CMD_GET_FRAME
 ****************************************************************************/

/* The command is nothing more than the WFW_CMD_GET_FRAME opcode */

/* The reply format is as follows. */
typedef struct WFW_PACKED wfw_reply_get_frame_data_s {
  wfwUint16_t         nrows;     /* number of rows of pixels per frame */
  wfwUint16_t         ncols;     /* number of columns of pixels per frame */
  wfwUint16_t         bitdepth;  /* number of bits per pixel (16, 12, etc) */
} wfw_reply_get_frame_data_t;


/****************************************************************************
 * WFW_CMD_TEST_RUN
 ****************************************************************************/

/* The command is followed by a generic test opcode byte and possibly a payload.
 * Opcodes are defined as follows: */

// Opcodes 0x00 through 0x0F reserved for MFG patches
#define WFW_TEST_OPCODE_RECALIBRATE        0x10
#define WFW_TEST_OPCODE_GET_FRAME_PROPS    0x11
#define WFW_TEST_OPCODE_RECALIBRATE_WOF    0x12

//Second opcode to RECALIBRATE_WOF indicates if we're doing a complete
//recalibration or just baselining.
#define WFW_TEST_PARAM_RECAL_WOF_RECAL     0x00
#define WFW_TEST_PARAM_RECAL_WOF_BASELINE  0x01

/* Reply to WFW_TEST_OPCODE_GET_FRAME_PROPS command */
typedef struct WFW_PACKED wfw_test_reply_get_frame_props_s {
  wfw_reply_get_frame_data_t dims;
} wfw_test_reply_get_frame_props_t;

/* ------------------------------------------------------------------------- */
/* End of command definitions                                                */
/* ------------------------------------------------------------------------- */

/****************************************************************************
 *  Iota Type Definitions
 ****************************************************************************/
/* Iotas are blocks of configuration data that can either be stored in OTPROM
 * or pushed down via an IOTA_INSINUATE command and are consumed by various
 * firmware components */

/* (internal) The ANY iota type is used as a wildcard for iota find */
#define WFW_IOTA_ITYPE_ANY                0x0000
/* The FRAME_BASE iota contains register settings and a drive matrix that are
 * used for full fingerprint images.  */
#define WFW_IOTA_ITYPE_FRAME_BASE         0x0001  // TODO: deprecate
/* The CONFIG_VERSION iota contains information about the iotas programmed
 * to the sensor and the identifier of the sensor (i.e.: FM number) */
#define WFW_IOTA_ITYPE_CONFIG_VERSION     0x0002
/* The BASE_REG iota contains registers that are applied for ANY imaging
 * type shift.  For example, when entering WOF mode, BASE_REG, WOF_REG,
 * and WOF_DM are applied; when entering imaging mode, BASE_REG, IMAGE_REG
 * and IMAGE_DM are applied. */
#define WFW_IOTA_ITYPE_BASE_REG           0x0003
/* WOF_REG and WOF_DM contain the registers and drive matrix for wake-on
 * finger scanning.  Similarly, IMAGE_REG and IMAGE_DM are the registers
 * and drive-matrix for fingerprint (full-image) scanning, and NAV_REG and
 * NAV_DM are the registers and drive matrix for swipe (coarse image)
 * detection. */
#define WFW_IOTA_ITYPE_WOF_REG            0x0004
#define WFW_IOTA_ITYPE_WOF_DM             0x0005
#define WFW_IOTA_ITYPE_IMAGE_REG          0x0006
#define WFW_IOTA_ITYPE_IMAGE_DM           0x0007
#define WFW_IOTA_ITYPE_NAV_REG            0x0008
#define WFW_IOTA_ITYPE_NAV_DM             0x0009
/* The CONFIG_XXX iotas are for their associated firmware functions.
 * More details will be included with their iota definitions. */
#define WFW_IOTA_ITYPE_CONFIG_FRAME_AVG   0x000C
#define WFW_IOTA_ITYPE_CONFIG_FRAME_SELECT    0x000D
#define WFW_IOTA_ITYPE_CONFIG_FPPRESENT   0x000E  // Should we rename this to coverage detect?
#define WFW_IOTA_ITYPE_CONFIG_SWIPE       0x0010
#define WFW_IOTA_ITYPE_CONFIG_BASELINE_MGMT   0x0011
#define WFW_IOTA_ITYPE_CONFIG_WOF         0x0012
#define WFW_IOTA_ITYPE_CONFIG_TAP_DETECTOR    0x0013
#define WFW_IOTA_ITYPE_FORCE_REG          0x0014
#define WFW_IOTA_ITYPE_CONFIG_FORCE       0x0015

/* This one actually isn't used by the firmware, but is used exclusively by the
 * host.  We keep this here as a comment, and as a reminder to not allocate this
 * for a new Iota.
 */
#define WFW_IOTA_ITYPE_DIMS               0x0016

/* (internal) The CHAINEND iota type signals the end of iotas in OTPROM */
#define WFW_IOTA_ITYPE_CHAINEND           0xFFFF

/****************************************************************************
 *  Iota Structure definitions
 ****************************************************************************/
/* All iotas begin with this header */
typedef struct WFW_PACKED wfw_iota_header_s {
  wfwUint16_t        length;     // the length of the iota, in bytes
  wfwUint16_t        type;       // The type of iota
  wfwUint32_t        crc;        // CRC
} wfw_iota_header_t;

/* WFW_IOTA_ITYPE_<*>_REG */
/*
 * These iotas consist of one or more register blocks that each consist of the
 * following header followed by nregs 16-bit words of register settings
 */
typedef struct WFW_PACKED wfw_iota_reg_s {
    wfwUint16_t  regbase;          /* base address in block of registers */
    wfwUint16_t  nregs;            /* number of registers */
} wfw_iota_reg_t;

/* WFW_IOTA_ITYPE_<*>_DM */
/* These iotas consist of NUM_POLREG * NUM_ROWS 16-bit words representing the
 * drive matrix */

/* WFW_IOTA_ITYPE_CONFIG_VERSION */
/*
 * This iota contains the configuration id and version.
 */
typedef struct WFW_PACKED wfw_iota_config_version_s{
    wfwUint32_t      config_id1;    /* YYMMDD */
    wfwUint32_t      config_id2;    /* HHMMSS */
    wfwUint16_t      version;
    wfwUint16_t      unused;
} wfw_iota_config_version_t;


/* WFW_IOTA_ITYPE_CONFIG_TAP_DETECTOR */
/*
 * This iota contains configuration settings for the tap detector
 * algorithm
 */
typedef struct WFW_PACKED wfw_iota_config_tap_detector_s {
    wfwUint16_t max_dtap_gap_time_ms;
    wfwUint16_t min_longpress_time_ms;
} wfw_iota_config_tap_detector_t;

/* WFW_IOTA_ITYPE_CONFIG_FRAME_AVG */
/*
 * This iota contains configuration settings for frame accumulation or averaging
 * which is performed on a fingerprint image before it is sent to the host.
 */
typedef struct WFW_PACKED wfw_config_frame_avg_s {
    wfwUint8_t  num_frames;        /* number of frames to average (0 or 1 disables averaging) */
    wfwUint8_t  unused1;
    wfwUint16_t unused2;
} wfw_iota_config_frame_avg_t;

/* WFW_IOTA_ITYPE_CONFIG_FRAME_SELECT */
/*
 * This iota contains configuration settings for the settling detection
 * algorithm
 */
typedef struct WFW_PACKED wfw_config_frame_select_s {
    wfwUint8_t  diff_thresh;
    wfwUint8_t  cnt_thresh;
    wfwUint8_t  delay_interval_msec;
    wfwUint8_t  unused1;
    wfwUint8_t  pscan_col_size;
    wfwUint8_t  pscan_row_size;
    wfwUint8_t  pscan_start_row;
    wfwUint8_t  unused2;
} wfw_iota_config_frame_select_t;

/* WFW_IOTA_ITYPE_CONFIG_FPPRESENT */
/*
 * This iota contains configuration settings for the coverage detection
 * algorithm
 */
typedef struct WFW_PACKED wfw_iota_config_fppresent_s {
    wfwUint32_t variance_thresh;
    wfwUint16_t coverage_thresh;
    wfwUint16_t unused;
    wfwUint16_t blocksizex;
    wfwUint16_t blocksizey;
    wfwUint16_t marginx;
    wfwUint16_t marginy;
} wfw_iota_config_fppresent_t;

/* WFW_IOTA_ITYPE_CONFIG_WOF */
/*
 * This iota contains configuration settings for WOF thresholds.
 * Legend:
 *  - fp: represents an item used for configuration of WOF with the fingerprint
 *        sensor.
 *  - sb: represents an item used for configuration of WOF with the soft
 *        buttons.
 *  - Abs: indicates an absolute threshold value, used with the various wof
 *        THRESH threshold registers
 *  - Deriv: indicates a derivative threshold value, used with the various wof
 *        DERIV threshold reigsters.
 *
 * This iota also contains configuration settings for WOF calibration.
 *  fp_globalCbcCalTolerance: percentage tolerance for calculating the global
 *  CBC adc count step size.
 *  sbforce_calCbcCalTolerance: percentage tolerance for calculating the CAL
 *  CBC adc count step size.  The same value is used for both soft buttons and
 *  Force.
 */
typedef struct WFW_PACKED wfw_iota_config_wof {
    wfwSint16_t fp_fuAbsThresh;
    wfwSint16_t fp_fdAbsThresh;
    wfwSint16_t fp_fuDerivThresh;
    wfwSint16_t fp_fdDerivThresh;
    wfwSint16_t sb_fuAbsThresh;
    wfwSint16_t sb_fdAbsThresh;
    wfwSint16_t sb_fuDerivThresh;
    wfwSint16_t sb_fdDerivThresh;
    wfwUint8_t  fp_globalCbcCalTolerance;
    wfwUint8_t  sbforce_calCbcCalTolerance;
    wfwUint16_t reserved[3];
} wfw_iota_config_wof_t;

/* WFW_IOTA_ITYPE_CONFIG_FORCE */
/*
 * This iota contains configuration settings for WOF thresholds specific to the
 * force detection functionality.
 */
typedef struct WFW_PACKED wfw_iota_config_force {
    wfwSint16_t force_fuThresh;
    wfwSint16_t force_fdThresh;
} wfw_iota_config_force_t;

/* WFW_IOTA_ITYPE_CONFIG_SWIPE */
/*
 * This iota contains configuration settings for Swipe/NAV detection
 * Legend:
 *  - mode: represents an item used for calibration mode setting
 *          modeConfig: 0 - all run-time checks disabled
 *                      1 - attempt recalibration only when start-up check fails
 *                      2 - 1 + recalibrate fine correction(s) if temp drift exceeds threshold
 *                      4 - 2 + recalibrate coarse and fine correction(s) if pixel variation check fails
 *  - temp: represents an item used for calibration relating to
 *          temperature sensor data
 *  - pixel: represents an item used for calibration relating to
 *           image pixel values
 */

/* structure used to store parameters for the swipe algorithm on a given axis */
typedef struct WFW_PACKED wfw_swipe_parameters_s {
    wfwUint8_t   req_dist_posdir;  /* total distance a finger must travel to be a swipe in the positive direction */
    wfwUint8_t   req_dist_negdir;  /* total distance a finger must travel to be a swipe in the negative direction */
    wfwUint8_t   min_exit_posdir;  /* the minimum final position a finger must reach to be a positive swipe */
    wfwUint8_t   max_exit_negdir;  /* the maximum final position a finger must reach to be a negative swipe */

    wfwUint8_t   min_movement;     /* the amount a finger needs to be moved to start tracking motion */
    wfwUint8_t   min_backward_movement;  /* the amount a finger needs to be moved in the opposite direction to reverse swipe direction */
    wfwUint8_t   flags;            /* see flag definitions below */
    wfwUint8_t   unused;

    wfwUint16_t  finger_threshold;
} wfw_swipe_parameters_t;

#define SWIPE_FLAGS_BLOCK_OTHER_AXIS  0x01  /* if a swipe is detected on this axis, block any on the other axis */
#define SWIPE_FLAGS_BLOCK_TDL         0x02  /* if a swipe is detected on this axis, block any TDL events */

typedef struct WFW_PACKED wfw_iota_config_swipe {
    wfwUint16_t             nav_first_col;   /* index of the first column that is valid in a NAV frame */
    wfwUint16_t             nav_col_stride;  /* number of columns to skip when reading NAV frames */
    wfwUint16_t             nav_frame_rate_ms;  /* maximum number of ms between NAV frames */
    wfwUint16_t             swipe_timeout_ms;   /* maximum number of milliseconds a swipe can take to be classified as a swipe */

    wfw_swipe_parameters_t  h_params;        /* horizontal swipe parameters */
    wfw_swipe_parameters_t  v_params;        /* vertical swipe parameters */
} wfw_iota_config_swipe_t;

/* WFW_IOTA_ITYPE_CONFIG_BASELINE_MGMT */
/*
 * This iota contains configuration settings for WOF, Image, NAV Baseline Management
 * Legend:
 *  - mode: represents an item used for calibration mode setting
 *  - temp: represents an item used for calibration relating to
 *          temperature sensor data
 *  - pixel: represents an item used for calibration relating to
 *           image pixel values
 *
 * Actual field descriptions:
 *  modeConfig: 0 - all run-time checks disabled (during testing, calibration may be
 *                  initiated by test command.
 *              1 - perform automatic startup calibration and attempt recalibration only if start-up
 *                  check fails or it was determined that the calibration was performed while finger down.
 *             >1 - 1 + recalibrate coarse and fine corrections if temp exceeds 'Recal' threshold,
 *                      recalibrate fine corrections if temp exceeds 'Drift' threshold.
 *  tempCheck_interval:  time interval at the end of which the temperature is checked for drift from
 *                       its value at last image calibration.  Units: milliseconds. Range 1 to (2**32)-1
 *  tempDrift_threshold: temperature drift from most recent calibration at which finest calibration
 *                       is to be initiated. Units: degrees C. Range 0-65535.
 *  tempRecal_threshold: temperature drift from most recent calibration at which a complete re-calibration
 *                       is to be initiated. Units: degrees C. Range 0-65535.
 *  pixelCalTolerance_ErrMargin: Image global/local additional margin added to calculated ADC code shift
 *                               tolerance for calibration convergence. This value is divided into the
 *                               calculated ADC coded shift tolerance to get a percentage of the tolerance
 *                               that is then added to the tolerance; e.g., a value of 5 will derive a 20%
 *                               additional tolerance. Units: as described. Range 1-65535.
 */
typedef struct WFW_PACKED wfw_iota_config_bl_mgmt {
    wfwUint16_t modeConfig;
    wfwUint32_t tempCheck_interval;
    wfwUint16_t tempDrift_threshold;
    wfwUint16_t tempRecal_threshold;
    wfwUint16_t pixelCalTolerance_ErrMargin; // Image global/local additional
} wfw_iota_config_bl_mgmt_t;


/* WFW_IOTA_ITYPE_DIMS */
/* As is commented above, this actually isn't used by the firmware, but is
 * documented here for host use.
 */
typedef struct WFW_PACKED wfw_iota_dims {
    wfwUint16_t bitdepth;          // bits per pixel (1, 2, 4, 8, 12?, 16)
 
    // frame (line counts) 
    wfwUint16_t frame_nlines;      // total number of lines per frame (Y-Dim)
    wfwUint16_t frame_header;      // number of bytes of per-frame header
    wfwUint16_t frame_lineskip;    // lines to skip at beginning of frame
    wfwUint16_t frame_linesusable; // lines to use for a frame
 
    // line (pixel counts)
    wfwUint16_t line_npix;         // total number of pixels across (X-Dim)
    wfwUint16_t line_header;       // number of bytes to skip to get to pix
    wfwUint16_t line_pixskip;      // pixels to skip at beginning of line (with sub-8 bpp could be a fractional byte)
    wfwUint16_t line_pixusable;    // pixels to use on line

    wfwUint16_t unused;
} wfw_iota_dims_t;

/****************************************************************************
 *  GET_TIMESTAMP
 ****************************************************************************/
typedef struct WFW_PACKED wfw_reply_get_timestamp_s {
  wfwUint32_t time;             //current value of the timer
  wfwUint32_t ticks_per_second;
} wfw_reply_get_timestamp_t;

/****************************************************************************
 *  GET_SYS_INFO
 ****************************************************************************/
typedef struct WFW_PACKED wfw_reply_get_sys_info_s {
  wfwUint16_t recalTemp;          //temp sense at most recent coarse&fine cal.
  wfwUint16_t driftTemp;          //temp sense at most recent fine only cal
  wfwUint16_t currentTemp;        //most recent temperature sensed
  wfwSint16_t wofFP_Baseline;     //most recent baseline calculated for WOF
  wfwUint16_t wofFP_Cbc;          //most recent CBC from WOF calibration
  wfwSint16_t wofFP_fuAbsThresh;  //fu threshold actually written to the hardware
  wfwSint16_t wofFP_fdAbsThresh;  //fd threshold actually written to the hardware
  wfwSint16_t wofSBGang_Baseline; //most recent baseline calculated for ganged soft buttons
  wfwUint16_t wofSBGang_CalCbc;   //most recent CalCBC calculated for ganged soft buttons
  wfwSint16_t wofSB0_Baseline;    //most recent baseline calculated for SB0 soft button
  wfwSint16_t wofSB0_CalCbc;      //most recent CalCBC calculated for SB0 soft button
  wfwSint16_t wofSB1_Baseline;    //most recent baseline calculated for SB1 soft button
  wfwSint16_t wofSB1_CalCbc;      //most recent CalCBC calculated for SB1 soft button
  wfwSint16_t Force_Baseline;     //most recent baseline calculated for Force button
  wfwSint16_t Force_CalCbc;       //most recent CalCBC calculated for Force button
  wfwSint16_t wofSB_fuAbsThresh;  //fu threshold used to detect finger up
  wfwSint16_t wofSB_fdAbsThresh;  //fd threshold used to detect finger down
  wfwUint16_t reset_condition;    //value of the reset condition register
  wfwUint32_t calStatus;          //bit pattern indicating status of all calibrations
  wfwUint16_t calCount;           //Count of calibration occurrences
  wfwSint16_t wofFP_PrevAdc;      //ADC reading from WOF fingerprint sensor
  wfwUint16_t reserved[2];
} wfw_reply_get_sys_info_t;



/****************************************************************************
 *  Frame Tag Definitions
 ****************************************************************************/
/* Iotas of type WFW_IOTA_ITYPE_FRAME_XXXXX consist of a list of frame tags
 * that contain data of specific formats. This section defines the valid frame
 * tag types and each of their structures.
 */

/* Frame Tag Header */
typedef struct WFW_PACKED wfw_frame_tag_s {
    wfwUint16_t   nwords;    /* number of 32-bit to follow */
    wfwUint8_t    flags;
    wfwUint8_t    tagid;
} wfw_frame_tag_t;

/* frame tag types */
#define WFW_FRAME_TAG_DM       3 /* Drive matrix */
#define WFW_FRAME_TAG_REG16BLK 6 /* A block of 16-bit register values */

/* WFW_FRAME_TAG_DM */
/* followed by a drive matrix that is numBursts * numTransmitters * 2 bits in
 * length */

/* WFW_FRAME_TAG_REG16BLK */
typedef struct WFW_PACKED wfw_frame_tag_reg16blk_s {
    wfwUint16_t  regbase;          /* base address in block of registers */
    wfwUint16_t  nregs;            /* number of registers */
} wfw_frame_tag_reg16blk_t;
/* followed by a list of nregs words specifying register settings */


/****************************************************************************
 *  EP0 Status definitions
 ****************************************************************************/

/*
 * EP0 status isn't accessible via a Whitney Command, but its definintion is
 * part of the command interface which we wish to share, so it is included here.
 */
//Definitions for the various EP0STATUS fields.  The ep0status is defined below.
#define EP0STATUS_FPSTATE_UNKNOWN   0
#define EP0STATUS_FPSTATE_ABSENT    1
#define EP0STATUS_FPSTATE_PRESENT   2

#define EP0STATUS_SSLSTATE_INSECURE 0
#define EP0STATUS_SSLSTATE_SECURE   1

//Alive is sort of an interesting one.  We use two bits to store this status, and
//set the two bits differently to indicate ALIVE, so that we can differentiate
//ourself from the state where the host interrogates EP0status when the system
//is asleep, and they see all 0s or all 1s.
#define EP0STATUS_ALIVE             2

#endif // !__WHITNEY_COMMANDS_H
