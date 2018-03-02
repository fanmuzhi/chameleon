/* -*- mode: c; tab-width: 4 -*- */
/* $Header$ */

/*
 *******************************************************************************
 **
 **                           NDA AND NEED-TO-KNOW REQUIRED
 **
 *****************************************************************************
 **
 **  Copyright (c) 2012-2018 Synaptics Incorporated. All rights reserved.
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
 * vcsfw_v4fido.h - This file defines the command and reply structures of 
 * VCSFW_CMD_FIDOOP command and all data structures of FIDO operations.
 *
 * NOTE:
 * The interfaces defined here is expected to be implemented only
 * in a patch.
 * 
 * Smbat Tonoyan, 30. August, 2012.
*/

/*
 * Note that as of September 1, 2015, this file is controlled
 *  authoritatively in the git repository
 *    ssh://gitms@git.synaptics.com/git/biometrics/include.git.
 * Updates will continue to be copied into the CVSNT repository
 *  in /test/engineering/private/impl/micro/falcon/shared/vcsfw_v4fido.h.
 * The last authoritative version of this file in CVSNT was
 *   /test/engineering/private/impl/micro/falcon/shared/vcsfw_v4fido.h,v 1.8 2012/09/07 11:39:49 kyazichyan
 * DO NOT EDIT THIS FILE IN THE CVSNT REPOSITORY.  Your changes will
 *  be overwritten.
 */

#ifndef __VCSFW_V4FIDO_H
#define __VCSFW_V4FIDO_H

#include "vcsPushPack1.h"

/****************************************************************************/
/* VCSFW_CMD_FIDOOP                                                         */
/****************************************************************************/

/* Operation IDs */
#define VCSFW_CMD_FIDOOP_ID_CREATE_AUTH_DATA     0
#define VCSFW_CMD_FIDOOP_ID_PROVISION_OTP_SEED   1
#define VCSFW_CMD_FIDOOP_ID_CREATE_HOTP_TOKEN    2

/* Other definitions. */
#define VCSFW_FIDOOP_MIN_RANDOM_LEN        16
#define VCSFW_FIDOOP_AES_BLOCK_SIZE        16
#define VCSFW_FIDOOP_SHA256_SIZE           32
#define VCSFW_FIDOOP_SHA1_SIZE             20
#define VCSFW_FIDOOP_AES128_KEYSIZE        16
#define VCSFW_FIDOOP_ENC_BLOCK_MAXSIZE     6*VCSFW_FIDOOP_AES_BLOCK_SIZE 
                                       /* IV + 32 byte key + 16 byte padding. */
#define VCSFW_FIDOOP_MAX_KEY_LEN           VCSFW_FIDOOP_SHA256_SIZE
#define VCSFW_FIDOOP_MAX_URL_LEN           256

#define VCSFW_FIDOOP_MAX_CHALLENGE_LEN     256

/* Command/reply structures for FIDO operations.                            */

/* VCSFW_CMD_FIDOOP_ID_CREATE_AUTH_DATA */
typedef struct VCS_PACKED vcsfw_cmd_fidoop_create_auth_data_s
{
    vcsUint16_t auth_code_pwd_len;
    vcsUint16_t client_random_len;
    vcsUint32_t clid_url_len;
    vcsUint8_t  auth_code_pwd[VCSFW_FIDOOP_SHA256_SIZE * 2]; 
        /*!!! Passwords longer than 64 bytes MUST BE SHA1 hashed on the host. */
    vcsUint8_t  client_random[VCSFW_FIDOOP_MIN_RANDOM_LEN];
    vcsUint8_t  clid_url[VCSFW_FIDOOP_MAX_URL_LEN]; /* Server ID and URL */
} vcsfw_cmd_fidoop_create_auth_data_t;

typedef struct VCS_PACKED vcsfw_reply_fidoop_create_auth_data_t
{
    vcsUint8_t  auth_data[VCSFW_FIDOOP_MIN_RANDOM_LEN];
} vcsfw_reply_fidoop_create_auth_data_t;

/* VCSFW_CMD_FIDOOP_ID_PROVISION_OTP_SEED */
typedef struct VCS_PACKED vcsfw_cmd_fidoop_provision_otp_seed_s
{
    vcsUint8_t   emac_key_len;
    vcsUint8_t   eprvk_key_len;
    vcsUint16_t  server_id_len;
    vcsUint8_t   emac_key[VCSFW_FIDOOP_ENC_BLOCK_MAXSIZE];
    vcsUint8_t   eprv_key[VCSFW_FIDOOP_ENC_BLOCK_MAXSIZE];
    vcsUint8_t   mac_prvk[VCSFW_FIDOOP_SHA1_SIZE];
    vcsUint8_t   mac_hello[VCSFW_FIDOOP_SHA256_SIZE];
    vcsUint8_t   dgst_hello[VCSFW_FIDOOP_SHA256_SIZE];
    vcsUint8_t   server_id[VCSFW_FIDOOP_MAX_URL_LEN];
} vcsfw_cmd_fidoop_provision_otp_seed_t;

typedef struct VCS_PACKED vcsfw_reply_fidoop_provision_otp_seed_t
{
    vcsUint8_t eseed[VCSFW_FIDOOP_AES_BLOCK_SIZE + VCSFW_FIDOOP_SHA256_SIZE];
} vcsfw_reply_fidoop_provision_otp_seed_t;

/* VCSFW_CMD_FIDOOP_ID_CREATE_HOTP_TOKEN */
typedef struct VCS_PACKED vcsfw_cmd_fidoop_create_hotp_token_s
{
    vcsUint32_t challenge_len;
    vcsUint8_t  eseed[VCSFW_FIDOOP_AES_BLOCK_SIZE + VCSFW_FIDOOP_SHA256_SIZE];
    vcsUint8_t  challenge[VCSFW_FIDOOP_MAX_CHALLENGE_LEN];
} vcsfw_cmd_fidoop_create_hotp_token_t;

typedef struct VCS_PACKED vcsfw_reply_fidoop_create_hotp_token_t
{
    vcsUint8_t token[VCSFW_FIDOOP_SHA256_SIZE];
} vcsfw_reply_fidoop_create_hotp_token_t;

/* COMMAND                                                                  */
typedef struct VCS_PACKED vcsfw_cmd_fidoop_s
{
    vcsUint16_t  opid;   /* The operation ID. */
    vcsUint16_t  unused; /* To make 'operation' 4-byte aligned for Metallica. */
    union
    {
        vcsfw_cmd_fidoop_create_auth_data_t    create_authdata;
        vcsfw_cmd_fidoop_provision_otp_seed_t  provision_otpseed;
        vcsfw_cmd_fidoop_create_hotp_token_t   create_hotptoken;
    } operation;
} vcsfw_cmd_fidoop_t;

/* REPLY                                                                  */
typedef union VCS_PACKED vcsfw_reply_fidoop_u
{
    vcsfw_reply_fidoop_create_auth_data_t    create_authdata;
    vcsfw_reply_fidoop_provision_otp_seed_t  provision_otpseed;
    vcsfw_reply_fidoop_create_hotp_token_t   create_hotptoken;
} vcsfw_reply_fidoop_t;

#include "vcsPopPack.h"

#endif      /* __VCSFW_V4FIDO_H */
