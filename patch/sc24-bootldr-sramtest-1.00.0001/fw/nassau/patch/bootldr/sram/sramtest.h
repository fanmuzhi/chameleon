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
 * Implement the VCSFW_CMD_SRAM_TEST command
 *
 */

#define VCSFW_CMD_SRAM_TEST   211

/*
 * error code: 2btyes
 *
 */
#define RAM_TEST_SUCCESS         0
#define FW_BL_RAM_TEST_ERROR     0x055
#define LNA_BL_RAM_TEST_ERROR    0x155
#define ADC_RAM1_TEST_ERROR      0x355
#define ADC_RAM2_TEST_ERROR      0x455
#define DM_RAM_TEST_ERROR        0x655
#define CDM_RAM_TEST_ERROR       0x755
#define IMAGE_RAM_TEST_ERROR     0x955
#define SYSTEM_RAM_TEST_ERROR    0x0A55
#define PUF_RAM_TEST_ERROR		 0x0B55
#define RAM_TEST_FAIL			 0x1


// return data structure:
// 2byte return data length and 
// 2byte error code followed by 4bytes error addr for each SRAM with the order same as above definition, total 9 SRAMs. 
// So the number of return data is 2 + 9 * ( 2 + 4 ) = 56bytes
// if error code is RAM_TEST_SUCCESS, the 4bytes error addr = 0;

