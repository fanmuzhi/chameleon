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
 * Implement the VCSFW_CMD_SHORT_TX VCSFW_CMD_SHORT_RX command
 *
 */

#define VCSFW_CMD_OPEN_SHORT   203

#define VCSFW_CMD_SHORT_TX     200
#define VCSFW_CMD_SHORT_RX     201
#define VCSFW_CMD_OPEN_TXRX    202



#define BUILD_FOR_ATE


/* Global define to handle building for different wrappers
  - Build for ATE 
  - Build for Bootload Patch
  - Set the default to build for ATE 
  - The makefile will define BUILD_FOR_ATE for the bootloader
    patches
  - Changed makefile 2017.01.17 
    -> PatchBuildCmd.bat will define as belows :
       BUILD_FOR_ATE for ATE patch build
       BUILD_FOR_IST for IST patch build
       BUILD_FOR_MT for MT patch build
*/

#define FEATURE_RX_OPEN_DELTA
//#define FEATURE_OPSH_GET_FRAME

/* ************************************************************************* */
/* *** Test Debug Data Structure Definitions                                 */
/* ************************************************************************* */

/* Status word bit definitions            
     STATUS_COUNT define indicates the number of 32-bit status words
     defines beginning with "TEST_STATUS_" will be annotated in the output report  */ 
#define TEST_STATUS_PASS                             0    /* Overall pass indication */
#define TEST_STATUS_FAIL                             1    /* Overall fail indication */
#define TEST_STATUS_FAIL_INTERNAL                    2    /* If set, a general run error occurred.  See Run Fail Code Parameter for location of errror. */
#define TEST_STATUS_FAIL_FIB_VAL_CHCK                4    /* Fail Check of the specified FIB Values */
#define TEST_STATUS_FAIL_MASK_ID                     5    /* Fail Check of the MASK ID Registers */
#define TEST_STATUS_FAIL_BIST_RX_SHORT               8    /* Failed BIST Rx Short Check */
#define TEST_STATUS_FAIL_BIST_TX_SHORT               9    /* Failed BIST Tx Short Check */
#define TEST_STATUS_FAIL_RX_OPEN                     12   /* Failed Rx Open Check */
#define TEST_STATUS_FAIL_RX_SHORT                    13   /* Failed Rx Short Check */
#define TEST_STATUS_FAIL_TX_OPEN                     14   /* Failed Tx Open/Short Check */
#define TEST_STATUS_FAIL_RX_OPEN_IMG                 15   /* Failed Rx Open Check using image. Ethan 01/04/17: SS request to separate Open failure */
#ifdef FEATURE_RX_OPEN_DELTA
#define TEST_STATUS_FAIL_RX_OPEN_IMG_DELTA           16   /* Failed Rx Open Delta Check Chris 01/19/17: SS report to escape issue. */
#endif /* FEATURE_RX_OPEN_DELTA */


#define STATUS_COUNT                                 1   /* Number of status words (not bits) */

/* Parameter word definitions
   PARAMETER_COUNT define indicates the number of 32-bit parameter words
   defines beginning with "TEST_PARAMETER_" will be annotated in the output report */

   /* Basic Patch Information */
#define TEST_PARAMETER_PATCH_TYPE                          0    /* What type of patch is this */
#define TEST_PARAMETER_PATCH_VERSION                       1    /* Patch Version */
#define TEST_PARAMETER_LIMIT_VERSION                       2    /* Limit Set Version */
#define TEST_PARAMETER_SER_NUM_A                           3    /* Leading 4 bytes of the serial number */
#define TEST_PARAMETER_SER_NUM_B                           4    /* Last 2 bytes of the serial number */
#define TEST_PARAMETER_LIMIT_SET                           5    /* Limit set used during test */
#define TEST_PARAMETER_CTRL_CODE                           6    /* Control code (bits) used to specify what is supposed to run */
/* ****************************** */
/* Test Control Bit Codes             */ 
#define CTRL_CODE_CLEAR_MEM_BUFFERS         0x00000001          /* Bit xx - Set to clear the memory buffers */
#define CTRL_CODE_INC_CHECK_MASK_ID         0x00000002          /* Bit xx - Set to include check of the Mask Id Registers */
#define CTRL_CODE_INC_BIST_TX_SHORT         0x00000004          /* Bit xx - Set to include BIST TX Short */
#define CTRL_CODE_INC_BIST_RX_SHORT         0x00000008          /* Bit xx - Set to include BIST TX Short */
#define CTRL_CODE_INC_RX_OPEN               0x00000010          /* Bit xx - Set to include test to look for rx open/short using imaging data  */
#define CTRL_CODE_INC_TX_OPEN               0x00000020          /* Bit xx - Set to include test to look for tx open/short using imaging data  */
#define CTRL_CODE_INC_VERIFY_FIB_VALS       0x00000040          /* Bit xx - Set to include the check to verify that the fib values have been programmed */

#define DATA_DECODE_CTRL_CODE_N_LIST           8
                                           /* First entry is the list label string */
                                           /* Bit       Label - Must end with 0, string termination, must be the right length */
#define DATA_DECODE_CTRL_CODE_BIT_LBL_LIST     0,       'C','t','r','l',' ','C','o','d','e', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,\
                                               0,       'C','l','e','a','r',' ','M','e','m',' ','B','u','f','f','e','r','s', 0 , 0 ,\
                                               1,       'I','n','c',' ','C','h','c','k',' ','M','a','s','k',' ','I','d', 0 , 0 , 0 ,\
                                               2,       'I','n','c',' ','B','i','s','t',' ','T','x',' ','S','h','r','t', 0 , 0 , 0 ,\
                                               3,       'I','n','c',' ','B','i','s','t',' ','R','x',' ','S','h','r','t', 0 , 0 , 0 ,\
                                               4,       'I','n','c',' ','R','x',' ','O','p','S','h',' ','I','m', 0 , 0 , 0 , 0 , 0 ,\
                                               5,       'I','n','c',' ','T','x',' ','O','p','S','h',' ','I','m', 0 , 0 , 0 , 0 , 0 ,\
                                               6,       'I','n','c',' ','C','h','c','k',' ','F','I','B',' ','P','r','o','g', 0 , 0 ,\


#define TEST_PARAMETER_RUN_CODE                            7    /* Run code (bits) used to indicate what was run */
/* ****************************** */
/* Test Control Bit Codes             */ 
#define RUN_CODE_CLEAR_MEM_BUFFERS         0x00000001          /* Bit xx - Set to clear the memory buffers */
#define RUN_CODE_INC_CHECK_MASK_ID         0x00000002          /* Bit xx - Set to include check of the Mask Id Registers */
#define RUN_CODE_INC_BIST_TX_SHORT         0x00000004          /* Bit xx - Set to include BIST TX Short */
#define RUN_CODE_INC_BIST_RX_SHORT         0x00000008          /* Bit xx - Set to include BIST TX Short */
#define RUN_CODE_INC_RX_OPEN               0x00000010          /* Bit xx - Set to include test to look for rx open/short using imaging data  */
#define RUN_CODE_INC_TX_OPEN               0x00000020          /* Bit xx - Set to include test to look for tx open/short using imaging data  */
#define RUN_CODE_INC_VERIFY_FIB_VALS       0x00000040          /* Bit xx - Set to include the check to verify that the fib values have been programmed */

#define DATA_DECODE_RUN_CODE_N_LIST           8
                                           /* First entry is the list label string */
                                           /* Bit       Label - Must end with 0, string termination, must be the right length */
#define DATA_DECODE_RUN_CODE_BIT_LBL_LIST      0,       'R','u','n',' ','C','o','d','e', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,\
                                               0,       'C','l','e','a','r',' ','M','e','m',' ','B','u','f','f','e','r','s', 0 , 0 ,\
                                               1,       'C','h','c','k',' ','M','a','s','k',' ','I','d', 0 , 0 , 0 , 0 , 0 , 0 , 0 ,\
                                               2,       'B','i','s','t',' ','T','x',' ','S','h','r','t', 0 , 0 , 0 , 0 , 0 , 0 , 0 ,\
                                               3,       'B','i','s','t',' ','R','x',' ','S','h','r','t', 0 , 0 , 0 , 0 , 0 , 0 , 0 ,\
                                               4,       'R','x',' ','O','p','S','h',' ','I','m', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,\
                                               5,       'T','x',' ','O','p','S','h',' ','I','m', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,\
                                               6,       'C','h','c','k',' ','F','I','B',' ','P','r','o','g', 0 , 0 , 0 , 0 , 0 , 0 ,\


#define TEST_PARAMTER_RUN_FAIL_CODE_LOC                    8    /* Value to indicate location of internal error, if one occurred.  Value should be 0 */

#define INTERNAL_ERROR_CODE_FPSCAN_TAG_FAIL                1    /* Failure during fpscan tag execution */


#define DATA_DECODE_INT_ERR_N_LIST             2
#define DATA_DECODE_INT_ERR_LBL_LIST           0,       'I','n','t',' ','E','r','r','o','r',' ','C','o','d','e', 0 , 0 , 0 , 0 , 0 ,\
                                               1,       'F','p','S','c','a','n',' ','T','a','g',' ','P','r','o','c', 0 , 0 , 0 , 0 ,\


#define TEST_PARAMETER_10USEC_CYLCES                       9    /* Number of 10usec clock cycles that test execution took */

#define TEST_PARAMETER_FIB_CHECKS                          10    /* FIB checks that were performed */
#define FIB_CHECK_FAST_CLOCK_OR_MASK                       0x00000001
#define FIB_CHECK_VTRIM_OR_MASK                            0x00000002
#define FIB_CHECK_BGTRIM_OR_MASK                           0x00000004
#define FIB_CHECK_SLOWCLK_OR_MASK                          0x00000008
#define FIB_CHECK_SER_NUM_OR_MASK                          0x00000010

#define DATA_DECODE_FIB_CHCK_N_LIST             6
#define DATA_DECODE_FIB_CHCK_LBL_LIST           0,       'F','I','B',' ','C','h','e','c','k',' ','L','i','s','t', 0 , 0 , 0 , 0 , 0 ,\
                                                0,       'F','a','s','t',' ','C','l','o','c','k', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,\
                                                1,       'V','T','r','i','m', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,\
                                                2,       'B','G',' ','T','r','i','m', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,\
                                                3,       'S','l','o','w',' ','C','l','o','c','k', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,\
                                                4,       'S','e','r','i','a','l',' ','N','u','m','b','e','r', 0 , 0 , 0 , 0 , 0 , 0 ,\



#define TEST_PARAMETER_FIB_CHECKS_OK                       11   /* FIB checks that were OK */

#define TEST_PARAMETER_MASK_ID_LO                          12    /* Contents of the MASK ID Low register */
#define TEST_PARAMETER_MASK_ID_HI                          13    /* Contents of the MASK ID High register */
#define TEST_PARAMETER_QNTY_BIST_TX_SHORT_CHCK             14    /* Quantity of Tx that were checked for short */
#define TEST_PARAMETER_QNTY_BIST_TX_FAIL_SHORT             15    /* Quantity of Tx that fail the short Tx */
#define TEST_PARAMETER_QNTY_BIST_RX_SHORT_CHCK             16    /* Quantity of Rx that were checked for short */
#define TEST_PARAMETER_QNTY_BIST_RX_FAIL_SHORT             17    /* Quantity of Rx that fail the short Rx */
#define TEST_PARAMETER_TAG_WRITE_TAGS_USED                 18    /* Debug --> Qnty tags in built-in tag data that were used */
#define TEST_PARAMETER_TAG_WRITE_TAGS_SKIP                 19    /* Debug --> Qnty tags in built-in tag data that were skipped */
#define TEST_PARAMETER_QNTY_RX_OPEN_MAX_DELTA_FAILS        20   /* Quantity failing Rx for max delta from average */
#define TEST_PARAMETER_QNTY_RX_OPEN_MAX_RANGE_FAILS        21   /* Quantity of failing Rx for minimum range */
//#define TEST_PARAMETER_QNTY_RX_OPEN_MAX_TX0_FAILS          22
#define TEST_PARAMETER_MAX_RX_DELTA_FRM_AVG                23   /* Minimum Tx Reading in opens test mode */
#define TEST_PARAMETER_MAX_RX_DELTA_FRM_AVG_IDX            24   /* Minimum Tx Reading in opens test mode */
#define TEST_PARAMETER_MAX_RX_RNG                          25   /* Minimum Tx Reading in opens test mode */
#define TEST_PARAMETER_MAX_RX_RNG_IDX                      26   /* Index of maximum percent variance (x10) from average TX */
//#define TEST_PARAMETER_MAX_RX_TX0                          27   /* Minimum limit */
//#define TEST_PARAMETER_MAX_RX_TX0_IDX                      28   /* Minimum limit */
#define TEST_PARAMETER_QNTY_TX_OPEN_FAILS                  29   /* Quantity of open Tx found */
#define TEST_PARAMETER_MAX_TX_RNG_DELTA                    30   /* Tx to Tx range delta max, better for detecting defects in the array area */
#define TEST_PARAMETER_MAX_TX_RNG_DELTA_IDX                31   /* Tx to Tx range delta max, better for detecting defects in the array area */
#define TEST_PARAMETER_MAX_TX_DELTA_FRM_AVG                32   /* Tx delta from avg max*/
#define TEST_PARAMETER_MAX_TX_DELTA_FRM_AVG_IDX            33   /* Index of Tx delta from avg max */
#define TEST_PARAMETER_MAX_TX_DELTA_ENDS_FRM_AVG           34   /* Tx delta from avg max for outside 4 Tx's*/
#define TEST_PARAMETER_MAX_TX_DELTA_ENDS_FRM_AVG_IDX       35   /* Index of Tx delta from avg max for outside 4 Tx's*/

#define PARAMETER_COUNT                                    36  /* Number of parameters */


/* Debug Data Structure:  Format:
   - Status Words
   - Parameter Words
   - Additional Data Definitions */

#include "basetypes.h"

#define TEST_QNTY_RX                             80          /* Quantity of receivers */
#define TEST_PXLS_PER_RX                         88          /* Quantity of pixels / receiver */
#define TEST_TOTAL_PXLS                          (TEST_QNTY_RX * TEST_PXLS_PER_RX)
#define TEST_PXL_MEM_HALF_WORDS                  (TEST_TOTAL_PXLS * 2)


/* Min/Max Limits */
typedef struct {
    int32_t       min;
    int32_t       max;
} limits_t; 

//ERIC 09/01/17: to get limit value by results for MT
typedef struct opsh_limit_type_s 
{
    limits_t           tx_open;
    limits_t           tx_open_ends;
    limits_t           tx_open_rng;
    limits_t           rx_open_max_delta_frm_avg;
    limits_t           rx_open_max_range;
    limits_t           rx_open_max_tx0;  
} opsh_limit_type_t;

typedef struct results_struct_s {
    uint32_t status[STATUS_COUNT];
#define TEST_RESULT_STATUS_DATA_TYPE                 0x00000001
#define TEST_RESULT_STATUS_START_BYTE                4  /* Offset for fw reply and pad */
#define TEST_RESULT_STATUS_BYTE_LEN                  (STATUS_COUNT*sizeof(uint32_t))

    uint32_t parameter[PARAMETER_COUNT];
#define TEST_RESULT_PARAMETER_DATA_TYPE              0x00000002
#define TEST_RESULT_PARAMETER_START_BYTE             (TEST_RESULT_STATUS_START_BYTE + TEST_RESULT_STATUS_BYTE_LEN)
#define TEST_RESULT_PARAMETER_BYTE_LEN               (PARAMETER_COUNT*sizeof(uint32_t))

  /* Data Storage Memory */
#define BIN_CODE_LIST_MAX_QNTY      7        /* Total bytes = 16 = 4 words for bin information */
  uint8_t          n_bin_codes;
  uint8_t          bin_codes[BIN_CODE_LIST_MAX_QNTY];
#define TEST_RESULT_BIN_CODE_LIST_DATA_TYPE          0x0000003
#define TEST_RESULT_BIN_CODE_LIST_START_BYTE         (TEST_RESULT_PARAMETER_START_BYTE + TEST_RESULT_PARAMETER_BYTE_LEN)
#define TEST_RESULT_BIN_CODE_LIST_BYTE_LEN           ((BIN_CODE_LIST_MAX_QNTY+1)*sizeof(uint8_t))

/* Quantity of shorts found for each transimtter */
  uint8_t  tx_short_count[TEST_PXLS_PER_RX];
#define TEST_RESULT_TX_SHORT_DATA_TYPE               0x00000004
#define TEST_RESULT_TX_SHORT_DATA_START_BYTE         (TEST_RESULT_BIN_CODE_LIST_START_BYTE + TEST_RESULT_BIN_CODE_LIST_BYTE_LEN)
#define TEST_RESULT_TX_SHORT_DATA_BYTE_LEN           (TEST_PXLS_PER_RX*sizeof(uint8_t))

/* Transmitter Short Debug Data */
#define MAX_QNTY_TX_SHORT_DEBUG_DATA          4
  uint32_t tx_short_dbg_cnt; 
  uint32_t tx_short_dbg_data[6*MAX_QNTY_TX_SHORT_DEBUG_DATA];     /* For each entry: Word 0 = Tx, Word 1-4 = Register data */
#define TEST_RESULT_TX_SHORT_DBG_DATA_TYPE           0x00000005
#define TEST_RESULT_TX_SHORT_DBG_DATA_START_BYTE     (TEST_RESULT_TX_SHORT_DATA_START_BYTE + TEST_RESULT_TX_SHORT_DATA_BYTE_LEN)
#define TEST_RESULT_TX_SHORT_DBG_DATA_BYTE_LEN       ((MAX_QNTY_TX_SHORT_DEBUG_DATA*6 + 1)*sizeof(uint32_t))

/* Quantity of shorts found for each transimtter */
  uint8_t  rx_short_data[TEST_QNTY_RX];
#define TEST_RESULT_RX_SHORT_DATA_TYPE               0x00000006
#define TEST_RESULT_RX_SHORT_DATA_START_BYTE         (TEST_RESULT_TX_SHORT_DBG_DATA_START_BYTE + TEST_RESULT_TX_SHORT_DBG_DATA_BYTE_LEN)
#define TEST_RESULT_RX_SHORT_DATA_BYTE_LEN           (TEST_QNTY_RX*sizeof(uint8_t))

  int16_t  rx_im_rng[TEST_QNTY_RX];
#define TEST_RESULT_RX_IM_RNG_DATA_TYPE              0x00000007
#define TEST_RESULT_RX_IM_RNG_DATA_START_BYTE        (TEST_RESULT_RX_SHORT_DATA_START_BYTE + TEST_RESULT_RX_SHORT_DATA_BYTE_LEN)
#define TEST_RESULT_RX_IM_RNG_DATA_BYTE_LEN          (TEST_QNTY_RX*sizeof(int16_t))

  int16_t  rx_im_tx0[TEST_QNTY_RX];
#define TEST_RESULT_RX_IM_TX0_DATA_TYPE              0x00000008
#define TEST_RESULT_RX_IM_TX0_DATA_START_BYTE        (TEST_RESULT_RX_IM_RNG_DATA_START_BYTE + TEST_RESULT_RX_IM_RNG_DATA_BYTE_LEN)
#define TEST_RESULT_RX_IM_TX0_DATA_BYTE_LEN          (TEST_QNTY_RX*sizeof(int16_t))

  int16_t  rx_im_delta[TEST_QNTY_RX];
#define TEST_RESULT_RX_IM_DELTA_DATA_TYPE            0x00000009
#define TEST_RESULT_RX_IM_DELTA_DATA_START_BYTE      (TEST_RESULT_RX_IM_TX0_DATA_START_BYTE + TEST_RESULT_RX_IM_TX0_DATA_BYTE_LEN)
#define TEST_RESULT_RX_IM_DELTA_DATA_BYTE_LEN        (TEST_QNTY_RX*sizeof(int16_t))

  int32_t  tx_im_gnd_sum[TEST_PXLS_PER_RX];
#define TEST_RESULT_TX_IM_GND_SUM_DATA_TYPE          0x0000000a
#define TEST_RESULT_TX_IM_GND_SUM_DATA_START_BYTE    (TEST_RESULT_RX_IM_DELTA_DATA_START_BYTE + TEST_RESULT_RX_IM_DELTA_DATA_BYTE_LEN)
#define TEST_RESULT_TX_IM_GND_SUM_DATA_BYTE_LEN      (TEST_PXLS_PER_RX*sizeof(int32_t))

  int32_t  tx_im_gnd_rng[TEST_PXLS_PER_RX];
#define TEST_RESULT_TX_IM_GND_RNG_DATA_TYPE          0x0000000b
#define TEST_RESULT_TX_IM_GND_RNG_DATA_START_BYTE    (TEST_RESULT_TX_IM_GND_SUM_DATA_START_BYTE + TEST_RESULT_TX_IM_GND_SUM_DATA_BYTE_LEN)
#define TEST_RESULT_TX_IM_GND_RNG_DATA_BYTE_LEN      (TEST_PXLS_PER_RX*sizeof(int32_t))

  //ERIC 09/01/17: to get limit value by results for MT
  opsh_limit_type_t limits;
#define TEST_RESULT_LIMIT_DATA_TYPE		             0x0000000c
#define TEST_RESULT_LIMIT_START_BYTE    	         (TEST_RESULT_TX_IM_GND_SUM_DATA_START_BYTE + TEST_RESULT_TX_IM_GND_SUM_DATA_BYTE_LEN)
#define TEST_RESULT_LIMIT_BYTE_LEN          	     (sizeof(opsh_limit_type_t))
} results_struct_t;

/* Debug data location information 
    - Used with the fw patch interface to provide a way to 
      find the starting point in the reply for different types of data */
#define TEST_DEBUG_QNTY_DATA_TYPES           12

#define TEST_DEBUG_DATA_LOCATIONS           .info[0] =  {.type = TEST_RESULT_STATUS_DATA_TYPE,            .offset = TEST_RESULT_STATUS_START_BYTE,              .bytes = TEST_RESULT_STATUS_BYTE_LEN},\
                                            .info[1] =  {.type = TEST_RESULT_PARAMETER_DATA_TYPE,         .offset = TEST_RESULT_PARAMETER_START_BYTE,           .bytes = TEST_RESULT_PARAMETER_BYTE_LEN},\
                                            .info[2] =  {.type = TEST_RESULT_BIN_CODE_LIST_DATA_TYPE,     .offset = TEST_RESULT_BIN_CODE_LIST_START_BYTE,       .bytes = TEST_RESULT_BIN_CODE_LIST_BYTE_LEN},\
                                            .info[3] =  {.type = TEST_RESULT_TX_SHORT_DATA_TYPE,          .offset = TEST_RESULT_TX_SHORT_DATA_START_BYTE,       .bytes = TEST_RESULT_TX_SHORT_DATA_BYTE_LEN},\
                                            .info[4] =  {.type = TEST_RESULT_TX_SHORT_DBG_DATA_TYPE,      .offset = TEST_RESULT_TX_SHORT_DBG_DATA_START_BYTE,   .bytes = TEST_RESULT_TX_SHORT_DBG_DATA_BYTE_LEN},\
                                            .info[5] =  {.type = TEST_RESULT_RX_SHORT_DATA_TYPE,          .offset = TEST_RESULT_RX_SHORT_DATA_START_BYTE,       .bytes = TEST_RESULT_RX_SHORT_DATA_BYTE_LEN},\
                                            .info[6] =  {.type = TEST_RESULT_RX_IM_RNG_DATA_TYPE,         .offset = TEST_RESULT_RX_IM_RNG_DATA_START_BYTE,      .bytes = TEST_RESULT_RX_IM_RNG_DATA_BYTE_LEN},\
                                            .info[7] =  {.type = TEST_RESULT_RX_IM_TX0_DATA_TYPE,         .offset = TEST_RESULT_RX_IM_TX0_DATA_START_BYTE,      .bytes = TEST_RESULT_RX_IM_TX0_DATA_BYTE_LEN},\
                                            .info[8] =  {.type = TEST_RESULT_RX_IM_DELTA_DATA_TYPE,       .offset = TEST_RESULT_RX_IM_DELTA_DATA_START_BYTE,    .bytes = TEST_RESULT_RX_IM_DELTA_DATA_BYTE_LEN},\
                                            .info[9] =	{.type = TEST_RESULT_TX_IM_GND_SUM_DATA_TYPE,     .offset = TEST_RESULT_TX_IM_GND_SUM_DATA_START_BYTE,  .bytes = TEST_RESULT_TX_IM_GND_SUM_DATA_BYTE_LEN},\
                                            .info[10] =	{.type = TEST_RESULT_TX_IM_GND_RNG_DATA_TYPE,     .offset = TEST_RESULT_TX_IM_GND_RNG_DATA_START_BYTE,  .bytes = TEST_RESULT_TX_IM_GND_RNG_DATA_BYTE_LEN},\
                                            .info[11] = {.type = TEST_RESULT_LIMIT_DATA_TYPE,             .offset = TEST_RESULT_LIMIT_START_BYTE,               .bytes = TEST_RESULT_LIMIT_BYTE_LEN},

/* ************************************************************************* */
/* *** Test Information Structure Definitions                                */
/* ************************************************************************* */

/* Test parameters that are constant for all test steps */ 
typedef struct test_const_params_s
{
    uint32_t             maskid[2];                     /* Value that the mask id register should equal */
    int32_t              rx_lna_tolerance;
    uint8_t              lna_bl_skip_frames;
    uint8_t              fw_bl_skip_frames;
#define MAX_RX_OPEN_SKIP        9
    uint8_t              n_rx_open_skip;
    uint8_t              rx_open_skip[MAX_RX_OPEN_SKIP]; 

} test_const_params_t;

/* Test parameters that may change with test steps */ 
typedef struct test_params_s
{
    uint32_t           ctrl_code;    /* Control Code */

    limits_t           tx_open;
    limits_t           tx_open_ends;
    limits_t           tx_open_rng;
    limits_t           rx_open_max_delta_frm_avg;
    limits_t           rx_open_max_range;
    limits_t           rx_open_max_tx0;  
} test_params_t;

#ifndef BUILD_FOR_ATE // TODO : need to check for ATE build
/* ************************************************************************* */
/* *** Interface Command Definitions - Used with fw patch interface          */
/* ************************************************************************* */
/* FW Command to execute a test command */
#define TEST_FW_TEST_CMD                               254


/* Commands 0 through 7 should be used for get info or setup types of commands */ 
#define TEST_FW_RUN_CMD_READ_CMD_LENGTHS              0x00000002         /* Read the lengths (and commands) of the commands supported */
#define TEST_FW_RUN_CMD_MAX_QNTY_CMDS                 64                 /* Maximum number of commands supported 
                                                                              --> needed to define the size of the read_reply_lengths command */
#define TEST_FW_RUN_CMD_SET_PARAMS                    0x00000003         /* Set parameter command(s) - Not implmented */

/* Commands 8 through 15 should be used for execution types of commands */
#define TEST_FW_RUN_CMD_RUN_TEST                      0x00000008         /* Execute initial test */


/* Commands 16 - 64 should be used to get information from the patch */
  /*  Commands that would usually be supported in all versions, available after calibration */
#define TEST_FW_RUN_CMD_GET_FRAME1_DATA               0x00000010         /* Read Frame1 Data */
#define TEST_FW_RUN_CMD_READ_IMAGE_RAM                0x00000011         /* Read the image frame buffer (uses frame1 data) */
#define TEST_FW_RUN_CMD_COLL_READ_IMAGE_RAM           0x00000012         /* Collect and read the image frame buffer (uses frame1 data) */
#define TEST_FW_RUN_CMD_READ_DMRAM_DATA               0x00000014         /* Read the DM RAM (uses frame1 data */ 
#define TEST_FW_RUN_CMD_READ_CDMRAM_DATA              0x00000015         /* Read the CDM RAM (uses frame1 data */
#ifdef FEATURE_OPSH_GET_FRAME
#define TEST_FW_RUN_CMD_GET_FRAME2_DATA               0x00000018         /* Read Frame2 Data */
#else
//#define TEST_FW_RUN_CMD_GET_FRAME2_DATA               0x00000018         /* Read Frame2 Data */
#endif
#define TEST_FW_RUN_CMD_READ_FW_BL_RAM                0x00000019         /* Read the FW baseline buffer (uses frame1 data) */
#define TEST_FW_RUN_CMD_READ_LNA_BL_RAM               0x0000001A         /* Read the LNA baseline buffer (uses frame1 data) */
#define TEST_FW_RUN_CMD_COLL_READ_IM_SUB_FW_BL        0x0000001B         /* Read the image frame buffer (uses frame1 data) and subtract the contents of the firmware baseline frame */

#define TEST_FW_RUN_CMD_GET_STATUS                    0x00000020         /* Read the status bits */
#define TEST_FW_RUN_CMD_GET_DEBUG_DATA_LOCATIONS      0x00000021         /* Read the locations of different data in the debug data */
#define TEST_FW_RUN_CMD_GET_DEBUG_DATA                0x00000022         /* Read the results data (status, parameters, test data) */
#define TEST_FW_RUN_CMD_LOAD_DEBUG_DATA               0x00000023         /* Load the debug data into the patch */
#define TEST_FW_RUN_CMD_GET_SETUP_DATA                0x00000024         /* Read the setup information - Not implemented*/


#define MAX_GEN_MEM_REPLY_SIZE_BYTES                  256   /* Maximum size of the generic reply buffer.  Needs to be larger than the get setup data reply */


/* Parameter definitions for use with the TEST_FW_RUN_CMD_SET_PARAMS command */
#define TEST_SET_PARAM_ATE                            0      /* Limit Set = 0 --> ATE              Use: TEST_SET_PARAM_ATE(word) */
#define TEST_SET_PARAM_MT_BGA                         1      /* Limit Set = 1 --> MT BGA           Use: TEST_SET_PARAM_MT_BGA(word) */
#define TEST_SET_PARAM_MT                             2      /* Limit Set = 2 --> MT               Use: TEST_SET_PARAM_MT(word) */
#define TEST_SET_PARAM_MT_FINAL                       3      /* Limit Set = 3 --> MT Final         Use: TEST_SET_PARAM_MT_FINAL(word) */
#define TEST_SET_PARAM_SYS                            4      /* Limit Set = 4 --> In-System Test   Use: TEST_SET_PARAM_SYS(word) */
#define TEST_SET_PARAM_SW                             5      /* Limit Set = 5 --> Software         Use: TEST_SET_PARAM_SW(word) */

#define TEST_SET_PARAM_CTRL_OR                        11     /* OR mask to apply to the ctrl code  Use: TEST_SET_PARAM_CTRL_OR(word)  OR_MASK(word) */
#define TEST_SET_PARAM_CTRL_AND                       12     /* AND mask to apply to the ctrl code Use: TEST_SET_PARAM_CTRL_AND(word)  AND_MASK(word) */

#define TEST_SET_PARAM_TURN_ON_SCM                    32     /* Turn on the SCM, just send the parameter */
#define TEST_SET_PARAM_TURN_OFF_SCM                   33     /* Turn off the SCM, just send the parameter */

extern uint32_t test_set_params(const uint32_t *cmdp, uint32_t n_words);

/* Run Test Function */
#ifndef FEATURE_OPSH_GET_FRAME
extern void run_test(results_struct_t *results); // test function is defined in test.c
#endif

/* Define the test function as a define so that different input parameters can be 
   handled by the same patch start function */
#ifdef FEATURE_OPSH_GET_FRAME
#define RUN_TEST_FUNCTION      run_test(&static_memory.results, &static_memory.frame1, &static_memory.frame2);
#else
#define RUN_TEST_FUNCTION      run_test(&static_memory.results);
#endif

/* Get Setup Function */
/* Structure definition for setup information */
typedef struct testSetupInfo_s
{
    /* Generic Information */
    uint32_t                      patch_type;
    uint32_t                      patch_ver;
    uint32_t                      limit_ver;
	uint32_t                      limit_set;

    uint32_t                      const_param_size;
    test_const_params_t           const_params;

    uint32_t                      param_size;
    test_params_t                 params;

}testSetupInfo_t;
extern void get_test_setup(testSetupInfo_t *setup);

/* Acquire Frame Data Function */
typedef struct half_word_frame_data_s
{
    uint32_t   frame_cols;
	uint32_t   frame_rows;
    int16_t    data[TEST_QNTY_RX * TEST_PXLS_PER_RX];    /* This is the data that is output to support the get frame request.*/
} half_word_frame_data_t;
extern void get_frame_data(half_word_frame_data_t *frame_datap, uint32_t cmd);

#ifdef FEATURE_OPSH_GET_FRAME
extern void run_test(results_struct_t *results, half_word_frame_data_t *frame1, half_word_frame_data_t *frame2);
#endif

#else  /* !BUILD_FOR_ATE */
/* ************************************************************************* */
/* *** Interface Command Definitions - Used with fw patch for ate build      */
/* ************************************************************************* */

/* FW Command to execute a test command for validation purpose
  - Extremely interface
  - Load Patch, patch replies with result structure at end */
#define TEST_FW_TEST_CMD                               254

/* Test function */
//extern void test(void);


#endif /* !BUILD_FOR_ATE */

