/* -*- mode: c; tab-width: 4 -*- */
/*
 * INTERNAL USE ONLY
 *
 * Copyright (c) 2015-2016 Synaptics Incorporated.  All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated ("Synaptics"). The holder of this file shall treat all
 * information contained herein as confidential, shall use the
 * information only for its intended purpose, and shall not duplicate,
 * disclose, or disseminate any of this information in any manner unless
 * Synaptics has otherwise provided express, written permission.
 */

/*
 * Implement the VCSFW_CMD_FLASH_TEST command
 *
 */

#define VCSFW_CMD_FLASH_TEST   210



/*
 * error code: 2btyes
 *
 */

#define FLASH_WRITE_55_ERROR   0x55
#define FLASH_CHECK_55_ERROR   0x155
#define FLASH_WRITE_AA_ERROR   0xAA
#define FLASH_CHECK_AA_ERROR   0x1AA
// the other used error code defined in Vcsfw_v4.h as follow
//#define VCSFW_STATUS_ERR_FLASH_ERASE_FAILURE (VCSFW_STATUS_ERR_FLAG | 473)

// and this one defined in Nvm_prog.h
//#define NVM_SUCCESS          0



//The return format is 2bytes error code followed by 4byte error address.

