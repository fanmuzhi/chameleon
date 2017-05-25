//**************************************************************************************************
// Copyright (c) 2016 Synaptics, Inc.  All rights reserved.
// Confidential and Proprietary Information of Synaptics, Inc.
//**************************************************************************************************

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!! PLEASE CHANGE THE FOLLOWING 4 VALUES WHEN MAKING CHANGES TO THIS FILE!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define PATCH_LIMIT_BUILD_MONTH      (3)          /* The month that the setup/limit was changed (01 to 12) */
#define PATCH_LIMIT_BUILD_DAY        (9)         /* The day that the setup/limit was changed (01 to 31) */
#define PATCH_LIMIT_BUILD_YEAR       (17)         /* The last two digits of the year that the setup/limit was changed (16 - 99) */
#define PATCH_LIMIT_BUILD_VERSION    (1)          /* The version of the setup/information for the day (1 - 255)
                                                   When the month, day, or year changes please set the version back to 1.
                                                   The version should be used in case there are multiple versions of
                                                     the patch built and released on the same day */ 

//==================================================================================================
//File History:
//  7/1/16_01 - MER - 



//==================================================================================================
// This file contains setup and limit information.  There are four sets of information.  Two sets
// are used for the calibration portion of the test.  Two sets are used for stimulus and no
// no stimulus tests.
//
// The code supports up to 6 different test steps.  This is managed inside the code by using a 
// variable called <limit_set>.  The <limit_set> variable can have six different values.
//   <limit_set> = 0 --> ==> ATE Limits
//   <limit_set> = 1 --> ==> MT BGA Limits (or LGA) 
//   <limit_set> = 2 --> ==> MT Limits
//   <limit_set> = 3 --> ==> MT Final Limits (or OQC / IQC)
//   <limit_set> = 4 --> ==> In-System Limits
//   <limit_set> = 5 --> ==> SW Limits (if needed)
//
// Calibration Information
// --------------------------
// cal_const_params --> calibration setup information that is the same for all <limit_set>.
// cal_limits_params[6] --> calibration setup/limit information that may be different for <limit_set>.
//




//==================================================================================================
// SECTION : Constant Test Setup Information --> same for all limit sets
//``````````````````````````````````````````````````````````````````````````````````````````````````
static test_const_params_t   test_const_params = {

    .maskid = {0x00001111, 0x00000111},
    .rx_lna_tolerance = 200,
    .lna_bl_skip_frames = 2,
    .fw_bl_skip_frames = 0,
    .n_rx_open_skip = 2,
    .rx_open_skip = {0, 79, 0, 0, 0, 0, 0, 0, 0}, 
};


//``````````````````````````````````````````````````````````````````````````````````````````````````


//==================================================================================================
// SECTION : Variable Test Setup Information --> may vary by limit set 
//``````````````````````````````````````````````````````````````````````````````````````````````````
/* Test parameters that can vary across limit sets */
static test_params_t   test_params[6] = {

    /* limit set 0 ==> ATE Limits */
    {
        .ctrl_code = (CTRL_CODE_CLEAR_MEM_BUFFERS        |
                      CTRL_CODE_INC_CHECK_MASK_ID        |
                      CTRL_CODE_INC_BIST_TX_SHORT        |
                      CTRL_CODE_INC_BIST_RX_SHORT        |
                      CTRL_CODE_INC_RX_OPEN              |
                      CTRL_CODE_INC_TX_OPEN              |
                      CTRL_CODE_INC_VERIFY_FIB_VALS      |
	                  0),

        .tx_open = {0, 50}, //middle 108 Tx's
        .tx_open_ends = {0, 80}, //outside 4 Tx's (2 on each end)
        .tx_open_rng = {0, 100}, //screen for defects in the array area 
//#ifdef FEATURE_RX_OPEN_DELTA
        .rx_open_max_delta_frm_avg = {0, 1000},	 // Enable new Rx Open Image test.
//#else
		//.rx_open_max_delta_frm_avg = {0, 10000}, //disable this test with the limit TH 11/23/2016
//#endif
        .rx_open_max_range = {0, 1000},
        .rx_open_max_tx0 = {0, 1000},

    },

    /* limit set 1 ==> MT BGA Limits */
    {
        .ctrl_code = (CTRL_CODE_CLEAR_MEM_BUFFERS        |
//                      CTRL_CODE_INC_CHECK_MASK_ID        |
                      CTRL_CODE_INC_BIST_TX_SHORT        |
                      CTRL_CODE_INC_BIST_RX_SHORT        |
                      CTRL_CODE_INC_RX_OPEN              |
                      CTRL_CODE_INC_TX_OPEN              |
                      CTRL_CODE_INC_VERIFY_FIB_VALS      |
	                  0),

        .tx_open = {0, 50}, //middle 108 Tx's
        .tx_open_ends = {0, 80}, //outside 4 Tx's (2 on each end)
        .tx_open_rng = {0, 100}, //screen for defects in the array area 
        .rx_open_max_delta_frm_avg = {0, 1000}, //disable this test with the limit TH 11/23/2016
        .rx_open_max_range = {0, 1000},
        .rx_open_max_tx0 = {0, 1000},
    },

    /* limit set 2 ==> MT Limits */
    {
        .ctrl_code = (CTRL_CODE_CLEAR_MEM_BUFFERS        |
//                      CTRL_CODE_INC_CHECK_MASK_ID        |
                      CTRL_CODE_INC_BIST_TX_SHORT        |
                      CTRL_CODE_INC_BIST_RX_SHORT        |
                      CTRL_CODE_INC_RX_OPEN              |
                      CTRL_CODE_INC_TX_OPEN              |
                      CTRL_CODE_INC_VERIFY_FIB_VALS      |
	                  0),
        .tx_open = {0, 50}, //middle 108 Tx's
        .tx_open_ends = {0, 80}, //outside 4 Tx's (2 on each end)
        .tx_open_rng = {0, 100}, //screen for defects in the array area 
//#ifdef FEATURE_RX_OPEN_DELTA
        .rx_open_max_delta_frm_avg = {0, 1000},	 // Enable new Rx Open Image test.
//#else
		//.rx_open_max_delta_frm_avg = {0, 10000}, //disable this test with the limit TH 11/23/2016
//#endif
        .rx_open_max_range = {0, 1000},
        .rx_open_max_tx0 = {0, 1000},
    },

    /* limit set 3 ==> MT Final Limits */
    {
        .ctrl_code = (CTRL_CODE_CLEAR_MEM_BUFFERS        |
                      CTRL_CODE_INC_CHECK_MASK_ID        |
                      CTRL_CODE_INC_BIST_TX_SHORT        |
                      CTRL_CODE_INC_BIST_RX_SHORT        |
                      CTRL_CODE_INC_RX_OPEN              |
                      CTRL_CODE_INC_TX_OPEN              |
                      CTRL_CODE_INC_VERIFY_FIB_VALS      |
	                  0),

        .tx_open = {0, 50}, //middle 108 Tx's
        .tx_open_ends = {0, 80}, //outside 4 Tx's (2 on each end)
        .tx_open_rng = {0, 100}, //screen for defects in the array area 
        .rx_open_max_delta_frm_avg = {0, 1000},
        .rx_open_max_range = {0, 1000},
        .rx_open_max_tx0 = {0, 1000},
    },

    /* limit set 4 ==> In-System Limits */
    {
        .ctrl_code = (CTRL_CODE_CLEAR_MEM_BUFFERS        |
//                      CTRL_CODE_INC_CHECK_MASK_ID        |
                      CTRL_CODE_INC_BIST_TX_SHORT        |
                      CTRL_CODE_INC_BIST_RX_SHORT        |
                      CTRL_CODE_INC_RX_OPEN              |
                      CTRL_CODE_INC_TX_OPEN              |
                      CTRL_CODE_INC_VERIFY_FIB_VALS      |
	                  0),

        .tx_open = {0, 50}, //middle 108 Tx's
        .tx_open_ends = {0, 80}, //outside 4 Tx's (2 on each end)
        .tx_open_rng = {0, 100}, //screen for defects in the array area 
//#ifdef FEATURE_RX_OPEN_DELTA
        .rx_open_max_delta_frm_avg = {0, 1000},	 // Enable new Rx Open Image test.
//#else
		//.rx_open_max_delta_frm_avg = {0, 10000}, //disable this test with the limit TH 11/23/2016
//#endif
        .rx_open_max_range = {0, 1000},
        .rx_open_max_tx0 = {0, 1000},
    },

    /* limit set 5 ==> SW Limits */
    {
        .ctrl_code = (CTRL_CODE_CLEAR_MEM_BUFFERS        |
                      CTRL_CODE_INC_CHECK_MASK_ID        |
                      CTRL_CODE_INC_BIST_TX_SHORT        |
                      CTRL_CODE_INC_BIST_RX_SHORT        |
                      CTRL_CODE_INC_RX_OPEN              |
                      CTRL_CODE_INC_TX_OPEN              |
                      CTRL_CODE_INC_VERIFY_FIB_VALS      |
	                  0),

        .tx_open = {0, 50}, //middle 108 Tx's
        .tx_open_ends = {0, 80}, //outside 4 Tx's (2 on each end)
        .tx_open_rng = {0, 100}, //screen for defects in the array area 
        .rx_open_max_delta_frm_avg = {0, 1000},
        .rx_open_max_range = {0, 1000},
        .rx_open_max_tx0 = {0, 1000},
    },
 
};


//``````````````````````````````````````````````````````````````````````````````````````````````````

#define TEST_PATCH_LIMIT_INFO          (((PATCH_LIMIT_BUILD_MONTH & 0xff) << 24) | ((PATCH_LIMIT_BUILD_DAY & 0xff) << 16) | ((PATCH_LIMIT_BUILD_YEAR & 0xff)<<8) | PATCH_LIMIT_BUILD_VERSION )


//==============================================================================
