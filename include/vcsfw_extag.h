/* -*- mode: c; tab-width: 4 -*- */
/* $Header$ */

/*
 *******************************************************************************
 **
 **                           NDA AND NEED-TO-KNOW REQUIRED
 **
 *****************************************************************************
 **
 **  Copyright (c) 2015-2017 Synaptics Incorporated. All rights reserved.
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

/*
 * Firmware OTPROM extended tag data definitions.
 */

#if !defined(__VCSFW_EXTAG_H)
#define __VCSFW_EXTAG_H

/* For USDK, 1-byte alignment */
#include "vcsPushPack1.h"


/* caldata header flag definitions */
#define VCSFW_CALDATA_HEADER_FLAGS_ARRAYWIDTH               0x07
#define VCSFW_CALDATA_HEADER_FLAGS_ARRAYWIDTH_B             0
#define VCSFW_CALDATA_HEADER_FLAGS_ARRAYWIDTH_N             3
#define VCSFW_CALDATA_HEADER_FLAGS_ARRAYSIGNED              0x08

#define VCSFW_CALDATA_HEADER_FLAGS_TRANSFORMID              0xF0
#define VCSFW_CALDATA_HEADER_FLAGS_TRANSFORMID_B            4
#define VCSFW_CALDATA_HEADER_FLAGS_TRANSFORMID_N            4

/* header of all the VCSFW_OTPROM_EXTAGTYPE_WINDSOR_CALDATA_* tags */
typedef struct VCS_PACKED vcsfw_otprom_tag_caldata_header_s
{
    vcsUint8_t     flags;       /* see flags definitions for VCSFW_CALDATA_HEADER_FLAGS_* */
    vcsUint8_t     adjvalue;    /* adjust value to apply by the transform function */ 
    vcsUint16_t    reserved;
} vcsfw_otprom_tag_caldata_header_t;

/* VCSFW_OTPROM_EXTAGTYPE_WINDSOR_CALDATA_REV1_COMMDET_CF1 */
/* caldata header followed by CF1 data array (1-byte per receiver) */

/* VCSFW_OTPROM_EXTAGTYPE_WINDSOR_CALDATA_REV1_PGAGAIN */
/* caldata header followed by PGA GAIN data array (1-byte per receiver) */

/* VCSFW_OTPROM_EXTAGTYPE_WINDSOR_CALDATA_REV1_LNA_OFFSET */
/* caldata header followed by LNA OFFSET data array (1-byte per receiver) */

/* VCSFW_OTPROM_EXTAGTYPE_WINDSOR_CALDATA_REV1_LNA_GAIN */
/* caldata header followed by LNA GAIN data array (1-byte per receiver) */

/* VCSFW_OTPROM_EXTAGTYPE_WINDSOR_CALDATA_REV1_PGA_OFFSET */
/* caldata header followed by PGA OFFSET data array (1-byte per receiver) */

/* VCSFW_OTPROM_EXTAGTYPE_WINDSOR_CALDATA_REV1_HPFBASELINE */
/* caldata header followed by 5-lines of HPFBASELINE data array (1-byte per pixel) */

/* data structure to store WOF calibratioin result */
typedef struct VCS_PACKED vcsfw_otprom_tag_caldata_wof_s
{
    vcsUint8_t     thresh_nofinger; /* threshold with no finger */
    vcsUint8_t     thresh_finger;   /* threshold with finger */
    vcsUint8_t     start_gain;      /* gain setting */
    vcsUint8_t     reserved;
} vcsfw_otprom_tag_caldata_wof_t;

/* VCSFW_OTPROM_EXTAGTYPE_WINDSOR_CALDATA_REV1_WOF_ZONE0 */
/* VCSFW_OTPROM_EXTAGTYPE_WINDSOR_CALDATA_REV1_WOF_ZONE1 */
/* VCSFW_OTPROM_EXTAGTYPE_WINDSOR_CALDATA_REV1_SCM_WOF_ZONE0 */
/* VCSFW_OTPROM_EXTAGTYPE_WINDSOR_CALDATA_REV1_SCM_WOF_ZONE1 */
/* VCSFW_OTPROM_EXTAGTYPE_WINDSOR_CALDATA_REV1_WOF_FU_ZONE0 */
/* VCSFW_OTPROM_EXTAGTYPE_WINDSOR_CALDATA_REV1_WOF_FU_ZONE1 */
/* 
    caldata header followed by WOF calibration result 
    (vcsfw_otprom_tag_caldata_wof_t) 
 */

/* data structure of the VCSFW_OTPROM_EXTAGTYPE_WINDSOR_SNR tag */
typedef struct VCS_PACKED vcsfw_otprom_tag_snr_s
{
    vcsUint8_t      snr;
    vcsUint8_t      signal;
    vcsUint8_t      noise;
    vcsUint8_t      reserved;
} vcsfw_otprom_tag_snr_t;

/* data structure of the VCSFW_OTPROM_EXTAGTYPE_WINDSOR_REV1_TEMP tag */
typedef struct VCS_PACKED vcsfw_otprom_tag_caltemp_s
{
    vcsUint16_t     temperature;    /* temperature reading at cal time */
} vcsfw_otprom_tag_caltemp_t;

/* data structure of the VCSFW_OTPROM_EXTAGTYPE_WINDSOR_CALCONFIG_INFO tag */
typedef struct VCS_PACKED vcsfw_otprom_tag_calconfig_info_s
{
    vcsUint16_t    flexid;     /* flexid of the config used to calibrate */
    vcsUint16_t    configrev;  /* revision number of the config */
    vcsUint16_t    caldatarev; /* revision number of the calibration data */
    vcsUint16_t    reserved;   /* reserved for future extension */
} vcsfw_otprom_tag_calconfig_info_t;

/* data structure of the VCSFW_OTPROM_EXTAGTYPE_WINDSOR_MT_INFO tag */
typedef struct VCS_PACKED vcsfw_otprom_tag_mtinfo_s
{
    vcsUint8_t      mt_partnum[8];  /* MT SW part number */
    vcsUint8_t      mt_config[8];   /* MT test config */
                                    /* date when MT performed */
    vcsUint8_t      mt_month;       /* 0-11 */
    vcsUint8_t      mt_day;         /* 0-30 */
    vcsUint16_t     mt_year;        /* 0-65535 (little-endian format) */
} vcsfw_otprom_tag_mtinfo_t;

/* data structure of the VCSFW_OTPROM_EXTAGTYPE_WINDSOR_HPFBASELINE_SCORE tag */
typedef struct VCS_PACKED vcsfw_otprom_tag_hpfbaseline_score_s
{
    vcsUint32_t     hpfbaseline_score;  /* calculated score against the 5-line HPFBASELINE data */
} vcsfw_otprom_tag_var_score_t;

/* data structure of the VCSFW_OTPROM_EXTAGTYPE_WINDSOR_CALDATA_REV2_HPFBASELINE header */
typedef struct VCS_PACKED vcsfw_otprom_tag_hpfbaseline_hdr_s
{
    vcsUint8_t      pixel_start;    /* starting index of the first pixel to include in the calculation */
    vcsUint8_t      pixel_step;     /* step between successive pixels */
    vcsUint8_t      pixel_count;    /* number of pixels included in the calculation */
    vcsUint8_t      rx_offset[5];   /* zero based offset of the Rx lines included */
    /* followed by the HPFBASELINE data */
} vcsfw_otprom_tag_hpfbaseline_hdr_t;

/* VCSFW_OTPROM_EXTAGTYPE_WINDSOR_CALDATA_REV2_HPFBASELINE */
/* caldata header + vcsfw_otprom_tag_hpfbaseline_hdr_t followed by 5-lines of HPFBASELINE data array (1-byte per pixel) */

#include "vcsPopPack.h"

#endif      // !defined(__VCSFW_EXTAG_H)
