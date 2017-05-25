

/* ********************************************************************** */
/* ********** Bin Codes                                                   */
/* ********************************************************************** */
#define BIN_CODE_PASS                            1
#define BIN_CODE_FAIL_LAYER_ID_CHECK             10        /* A check of the ASIC type failed */
#define BIN_CODE_FAIL_RX_OPEN_CHECK              15        /* A check for Rx open failed */
#define BIN_CODE_FAIL_RX_SHORT_CHECK             16        /* A check for Rx short failed */
#define BIN_CODE_FAIL_TX_OPEN_CHECK              17        /* A check for Tx open failed */
#define BIN_CODE_FAIL_TX_SHORT_CHECK             18        /* A check for Tx short failed */

#define BIN_CODE_FAIL_OTP_CHECK                  20        /* A check of the OTP contents failed */
#define BIN_CODE_FAIL_OTP_BOOT_ROW_CHCK          21        /* A check related to the OTP Boot Row Failed */
#define BIN_CODE_FAIL_OTP_TAG_CHCK               22        /* A check related to checking main OTP memory failed */
#define BIN_CODE_FAIL_OTP_REDUNDANCY_CHCK        23        /* A check related to checking redundancy in the OTP failed (duplicate tags or duplicate boot row words */
#define BIN_CODE_FAIL_OTP_STRESS_TEST            24        /* An OTP stress test failed */
#define BIN_CODE_FAIL_LNA_OFFSET_CALIBRATION     25        /* LNA offset calibration failed */
#define BIN_CODE_FAIL_FDETECT_DURING_CAL         30        /* Finger detection during calibration failed (i.e. score test) */
#define BIN_CODE_FAIL_SW_BASELINE_VALIDATION     35        /* Software baseline validation failed */
#define BIN_CODE_FAIL_PIXEL_CHECK                40        /* Failed a pixel check.  Check the quantity and relationship of bad pixels */
#define BIN_CODE_FAIL_SNR                        45        /* Failed SNR */
#define BIN_CODE_FAIL_SNR_SIGNAL                 46        /* Failed SNR - Signal */
#define BIN_CODE_FAIL_SNR_NOISE                  47        /* Failed SNR - Noise */
#define BIN_CODE_FAIL_AFE_MATCH_CHECK            50        /* Failed AFE Match Check */
#define BIN_CODE_FAIL_WOVAR_CHECK                51        /* Failed WOVAR Check */
#define BIN_CODE_FAIL_HP_CORR_CHECK              52        /* Failed Check of HP Correction Circuit */
#define BIN_CODE_FAIL_RX_NOISE_CHECK             53        /* Failed Rx Noise Check */
#define BIN_CODE_FAIL_RX_STDEV_CHECK             55        /* Failed Rx Std. Deviation Check */
#define BIN_CODE_FAIL_WOVAR_STIM_CHECK           56        /* Failed Wovar with stimulus check */
#define BIN_CODE_FAIL_WOF_CAL                    57        /* Failed WOF / WOF2 Calibration */
#define BIN_CODE_FAIL_WOF_VAL                    58        /* Failed WOf / WOF2 Validation Check (Use Calibration Values) */
#define BIN_CODE_FAIL_SCM_WOF_CAL                59        /* Failed SCM WOF Calibration */
#define BIN_CODE_FAIL_SCM_WOF_VAL                60        /* Failed SCM WOf Validation Check (Use Calibration Values) */
#define BIN_CODE_FAIL_FIB_CHECK                  61        /* A check of the flash info block failed */
#define BIN_CODE_FAIL_REG_CHECK                  62        /* Failed a register value check */
#define BIN_CODE_FAIL_DATA_COLL                  63        /* Failed a data collection */
#define BIN_CODE_FAIL_IMPERFECTIONS              64        /* Failed Imperfections Test */
#define BIN_CODE_FAIL_IOTA_CHECK                 65        /* A check of the IOTAs failed */

#define BIN_CODE_FAIL_CAL_TEST_PATCH_RUN         80        /* Calibration/Test patch failure */
#define BIN_CODE_FAIL_PATCH_INTERNAL_ERROR       81        /* Internal error occurred during execution of the patch */
#define BIN_CODE_FAIL_OTP_TEST_PATCH_RUN         82        /* OTP Test Patch Fail */
#define BIN_CODE_FAIL_WOF_TEST_PATCH_RUN         83        /* WOF Test Patch Fail */
#define BIN_CODE_FAIL_OPSH_PATCH_RUN             84        /* Open/Short Patch Fail */
#define BIN_CODE_FAIL_CONS_PATCH_RUN             85        /* Consolidated Patch Fail */

#define BIN_CODE_FAIL_DATA_NOT_AVAIL             249       /* Req Data is not available */
#define BIN_CODE_FAIL_FORCED_FAIL                250       /* Forced failure */



/* Bin / Label Defintions    */
#define BIN_LABEL_LIST_QNTY              (41)
#define BIN_LABEL_LIST_LEN               (BIN_LABEL_LIST_QNTY * 16)
#define BIN_LABEL_LIST_DBG_DECODE_LEN    (BIN_LABEL_LIST_LEN + 1)          /* Qnty Labels (1 byte) + Label List */
                                                                   /* 1   2   3   4   5   6   7   8   9  10  11  12  13  14  15 */
#define BIN_LABEL_LIST        BIN_CODE_PASS,                         'P','a','s','s', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_LAYER_ID_CHECK,          'L','y','r','I','d', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_RX_OPEN_CHECK,           'R','x',' ','O','p','e','n', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_RX_SHORT_CHECK,          'R','x',' ','S','h','o','r','t', 0 , 0 , 0 , 0 , 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_TX_OPEN_CHECK,           'T','x',' ','O','p','e','n', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_TX_SHORT_CHECK,          'T','x',' ','S','h','o','r','t', 0 , 0 , 0 , 0 , 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_OTP_CHECK,               'O','t','p','C','h','c','k', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_OTP_BOOT_ROW_CHCK,       'O','t','p','B','o','o','t','R','o','w','C','h','c','k', 0 ,\
                              BIN_CODE_FAIL_OTP_TAG_CHCK,            'O','t','p','T','a','g','C','h','c','k', 0 , 0 , 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_OTP_REDUNDANCY_CHCK,     'O','t','p','D','u','p','l','C','h','c','k', 0 , 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_OTP_STRESS_TEST,         'O','t','p','S','t','r','e','s','s','C','h','c','k', 0 , 0 ,\
                              BIN_CODE_FAIL_LNA_OFFSET_CALIBRATION,  'L','n','a','C','a','l', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_FDETECT_DURING_CAL,      'F','d','t','c','t','C','a','l', 0 , 0 , 0 , 0 , 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_SW_BASELINE_VALIDATION,  'S','w','B','a','s','e','V','a','l', 0 , 0 , 0 , 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_PIXEL_CHECK,             'P','x','l','C','h','c','k', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_SNR,                     'S','N','R', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_SNR_SIGNAL,              'S','i','g','n','a','l', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_SNR_NOISE,               'N','o','i','s','e', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_AFE_MATCH_CHECK,         'A','f','e','M','t','c','h', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_WOVAR_CHECK ,            'W','o','v','a','r', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_HP_CORR_CHECK,           'H','p','C','o','r','r', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_RX_NOISE_CHECK,          'R','x','N','s','e', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_RX_STDEV_CHECK,          'R','x','S','t','d','e','v', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_WOVAR_STIM_CHECK,        'W','o','v','a','r','S','t','i','m', 0 , 0 , 0 , 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_WOF_CAL,                 'W','O','F','C','a','l', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_WOF_VAL,                 'W','O','F','V','a','l', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_SCM_WOF_CAL,             'S','C','M','W','O','F','C','a','l', 0 , 0 , 0 , 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_SCM_WOF_VAL,             'S','C','M','W','O','F','V','a','l', 0 , 0 , 0 , 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_FIB_CHECK,               'F','I','B',' ','D','a','t','a', 0 , 0 , 0 , 0 , 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_REG_CHECK,               'R','e','g',' ','V','a','l',' ','C','h','c','k', 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_DATA_COLL,               'D','a','t','a',' ','C','o','l','l', 0 , 0 , 0 , 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_IMPERFECTIONS,           'I','m','p','e','r','f','e','c','t','i','o','n','s', 0 , 0 ,\
                              BIN_CODE_FAIL_IOTA_CHECK,              'I','O','T','A',' ','D','a','t','a', 0 , 0 , 0 , 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_CAL_TEST_PATCH_RUN,      'P','t','c','h','F','a','i','l', 0 , 0 , 0 , 0 , 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_PATCH_INTERNAL_ERROR,    'P','t','c','h','I','n','t', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_OTP_TEST_PATCH_RUN,      'P','t','c','h','O','t','p','F','a','i','l', 0 , 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_WOF_TEST_PATCH_RUN,      'P','t','c','h','W','O','F','F','a','i','l', 0 , 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_OPSH_PATCH_RUN,          'P','t','c','h','O','p','S','h','F','a','i','l', 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_CONS_PATCH_RUN,          'P','t','c','h','C','o','n','s','F','a','i','l', 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_DATA_NOT_AVAIL,          'R','e','q',' ','D','a','t','a',' ','N','A', 0 , 0 , 0 , 0 ,\
                              BIN_CODE_FAIL_FORCED_FAIL,             'F','o','r','c','e','d','F','a','i','l', 0 , 0 , 0 , 0 , 0 ,





/* ********************************************************************** */
/* Common Test Number Defintions                                          */
/*   - Use with debug data decode                                         */
/* ********************************************************************** */
#define DEFAULT_TEST_NUMBER_INCREMENT        (5)

#define OPSH_TEST_NUMBER_START               (1000)
#define PIXEL_TESTS_NUMBER_START             (2000)
#define WOF_TEST_NUMBER_START                (3000)




#define CONS_PATCH_TEST_NUMBER_START                (500)
#define CONS_PATCH_TEST_NUM_PASS_STATUS_BIT         (CONS_PATCH_TEST_NUMBER_START + 0*DEFAULT_TEST_NUMBER_INCREMENT)
#define CONS_PATCH_TEST_NUM_FAIL_STATUS_BIT         (CONS_PATCH_TEST_NUMBER_START + 1*DEFAULT_TEST_NUMBER_INCREMENT)
#define CONS_PATCH_TEST_NUM_STATUS_WORD             (CONS_PATCH_TEST_NUMBER_START + 2*DEFAULT_TEST_NUMBER_INCREMENT)
#define CONS_PATCH_TEST_NUM_FIRST_BIN_CODE          (CONS_PATCH_TEST_NUMBER_START + 3*DEFAULT_TEST_NUMBER_INCREMENT)
#define CONS_PATCH_TEST_NUM_LIMIT_SET               (CONS_PATCH_TEST_NUMBER_START + 4*DEFAULT_TEST_NUMBER_INCREMENT)
#define CONS_PATCH_TEST_NUM_PATCH_TYPE_CODE         (CONS_PATCH_TEST_NUMBER_START + 5*DEFAULT_TEST_NUMBER_INCREMENT)
#define CONS_PATCH_TEST_NUM_CODE_DATE_CODE          (CONS_PATCH_TEST_NUMBER_START + 6*DEFAULT_TEST_NUMBER_INCREMENT)
#define CONS_PATCH_TEST_NUM_LIMIT_DATE_CODE         (CONS_PATCH_TEST_NUMBER_START + 7*DEFAULT_TEST_NUMBER_INCREMENT)

#define LAYER_ID_TEST_NUMBER_START                  (600)
#define LAYER_ID_TEST_NUM_LAYER_ID_STATUS_BIT       (LAYER_ID_TEST_NUMBER_START)

#define BIST_TX_SHORT_TEST_NUMBER_START             (650)
#define BIST_TX_SHORT_TEST_NUM_STATUS_BIT           (BIST_TX_SHORT_TEST_NUMBER_START)
#define BIST_TX_SHORT_TEST_NUM_QNTY_TX_SHORT        (BIST_TX_SHORT_TEST_NUMBER_START + DEFAULT_TEST_NUMBER_INCREMENT)

#define BIST_RX_SHORT_TEST_NUMBER_START             (700)
#define BIST_RX_SHORT_TEST_NUM_STATUS_BIT           (BIST_RX_SHORT_TEST_NUMBER_START)
#define BIST_RX_SHORT_TEST_NUM_QNTY_RX_SHORT        (BIST_RX_SHORT_TEST_NUMBER_START + DEFAULT_TEST_NUMBER_INCREMENT)

#define RX_TX_OPEN_TEST_NUMBER_START                (750)
#define RX_TX_OPEN_TEST_NUM_TX_OPEN_STATUS_BIT      (RX_TX_OPEN_TEST_NUMBER_START)
#define RX_TX_OPEN_TEST_NUM_TX_SHORT_STATUS_BIT     (RX_TX_OPEN_TEST_NUMBER_START + 1*DEFAULT_TEST_NUMBER_INCREMENT)
#define RX_TX_OPEN_TEST_NUM_RX_OPEN_STATUS_BIT      (RX_TX_OPEN_TEST_NUMBER_START + 2*DEFAULT_TEST_NUMBER_INCREMENT)
#define RX_TX_OPEN_TEST_NUM_RX_SHORT_STATUS_BIT     (RX_TX_OPEN_TEST_NUMBER_START + 3*DEFAULT_TEST_NUMBER_INCREMENT)
#define RX_TX_OPEN_TEST_NUM_TX_OPSH_FAILS           (RX_TX_OPEN_TEST_NUMBER_START + 4*DEFAULT_TEST_NUMBER_INCREMENT)
#define RX_TX_OPEN_TEST_NUM_MAX_TX_DELTA_FRM_AVG    (RX_TX_OPEN_TEST_NUMBER_START + 5*DEFAULT_TEST_NUMBER_INCREMENT)
#define RX_TX_OPEN_TEST_NUM_RX_OPSH_MAX_DELTA_FAILS (RX_TX_OPEN_TEST_NUMBER_START + 6*DEFAULT_TEST_NUMBER_INCREMENT)
#define RX_TX_OPEN_TEST_NUM_RX_OPSH_RANGE_FAILS     (RX_TX_OPEN_TEST_NUMBER_START + 7*DEFAULT_TEST_NUMBER_INCREMENT)
#define RX_TX_OPEN_TEST_NUM_RX_OPSH_MIN_TX0_FAILS   (RX_TX_OPEN_TEST_NUMBER_START + 8*DEFAULT_TEST_NUMBER_INCREMENT)
#define RX_TX_OPEN_TEST_NUM_MAX_RX_DELTA_FRM_AVG    (RX_TX_OPEN_TEST_NUMBER_START + 9*DEFAULT_TEST_NUMBER_INCREMENT)
#define RX_TX_OPEN_TEST_NUM_MAX_RX_RNG              (RX_TX_OPEN_TEST_NUMBER_START + 10*DEFAULT_TEST_NUMBER_INCREMENT)
#define RX_TX_OPEN_TEST_NUM_MIN_RX_TX0              (RX_TX_OPEN_TEST_NUMBER_START + 11*DEFAULT_TEST_NUMBER_INCREMENT)

#define AFE_MATCH_TEST_NUMBER_START                 (850)
#define AFE_MATCH_TEST_NUM_CLIP_STATUS_BIT          (AFE_MATCH_TEST_NUMBER_START )
#define AFE_MATCH_TEST_NUM_RATIO_STATUS_BIT         (AFE_MATCH_TEST_NUMBER_START + 1*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_CHAN_RATIO_STATUS_BIT    (AFE_MATCH_TEST_NUMBER_START + 2*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_CLIP_PXL_CNT             (AFE_MATCH_TEST_NUMBER_START + 3*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_RATIO_LEFT_RIGHT         (AFE_MATCH_TEST_NUMBER_START + 4*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_RATIO_R0                 (AFE_MATCH_TEST_NUMBER_START + 5*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_RATIO_R1                 (AFE_MATCH_TEST_NUMBER_START + 6*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_RATIO_R2                 (AFE_MATCH_TEST_NUMBER_START + 7*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_RATIO_R3                 (AFE_MATCH_TEST_NUMBER_START + 8*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_RATIO_R4                 (AFE_MATCH_TEST_NUMBER_START + 9*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_RATIO_R5                 (AFE_MATCH_TEST_NUMBER_START + 10*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_RATIO_R6                 (AFE_MATCH_TEST_NUMBER_START + 11*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_RATIO_L0                 (AFE_MATCH_TEST_NUMBER_START + 12*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_RATIO_L1                 (AFE_MATCH_TEST_NUMBER_START + 13*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_RATIO_L2                 (AFE_MATCH_TEST_NUMBER_START + 14*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_RATIO_L3                 (AFE_MATCH_TEST_NUMBER_START + 15*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_RATIO_L4                 (AFE_MATCH_TEST_NUMBER_START + 16*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_RATIO_L5                 (AFE_MATCH_TEST_NUMBER_START + 17*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_RATIO_L6                 (AFE_MATCH_TEST_NUMBER_START + 18*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_RATIO_R0_V2              (AFE_MATCH_TEST_NUMBER_START + 5*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_RATIO_R1_V2              (AFE_MATCH_TEST_NUMBER_START + 6*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_RATIO_R2_V2              (AFE_MATCH_TEST_NUMBER_START + 7*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_RATIO_R3_V2              (AFE_MATCH_TEST_NUMBER_START + 8*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_RATIO_R4_V2              (AFE_MATCH_TEST_NUMBER_START + 9*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_RATIO_R5_V2              (AFE_MATCH_TEST_NUMBER_START + 10*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_RATIO_R6_V2              (AFE_MATCH_TEST_NUMBER_START + 11*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_RATIO_R7_V2              (AFE_MATCH_TEST_NUMBER_START + 12*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_RATIO_R8_V2              (AFE_MATCH_TEST_NUMBER_START + 13*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_RATIO_R9_V2              (AFE_MATCH_TEST_NUMBER_START + 14*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_RATIO_L0_V2              (AFE_MATCH_TEST_NUMBER_START + 15*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_RATIO_L1_V2              (AFE_MATCH_TEST_NUMBER_START + 16*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_RATIO_L2_V2              (AFE_MATCH_TEST_NUMBER_START + 17*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_RATIO_L3_V2              (AFE_MATCH_TEST_NUMBER_START + 18*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_RATIO_L4_V2              (AFE_MATCH_TEST_NUMBER_START + 19*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_RATIO_L5_V2              (AFE_MATCH_TEST_NUMBER_START + 20*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_RATIO_L6_V2              (AFE_MATCH_TEST_NUMBER_START + 21*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_RATIO_L7_V2              (AFE_MATCH_TEST_NUMBER_START + 22*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_RATIO_L8_V2              (AFE_MATCH_TEST_NUMBER_START + 23*DEFAULT_TEST_NUMBER_INCREMENT)
#define AFE_MATCH_TEST_NUM_RATIO_L9_V2              (AFE_MATCH_TEST_NUMBER_START + 24*DEFAULT_TEST_NUMBER_INCREMENT)

#define DEN_HAYS_BASE_COLL_TEST_NUMBER_START                       (950)
#define DEN_HAYS_BASE_COLL_TEST_NUM_LNA_CFG_CHCK_STATUS_BIT        (DEN_HAYS_BASE_COLL_TEST_NUMBER_START)
#define DEN_HAYS_BASE_COLL_TEST_NUM_SAT_DET_STATUS_BIT             (DEN_HAYS_BASE_COLL_TEST_NUMBER_START + 1*DEFAULT_TEST_NUMBER_INCREMENT)

#define DEN_HAYS_BASE_COLL_TEST_NUMBER_START_V2                    (980)
#define DEN_HAYS_BASE_COLL_TEST_NUM_LNA_CFG_CHCK_STATUS_BIT_V2     (DEN_HAYS_BASE_COLL_TEST_NUMBER_START_V2)
#define DEN_HAYS_BASE_COLL_TEST_NUM_SAT_DET_STATUS_BIT_V2          (DEN_HAYS_BASE_COLL_TEST_NUMBER_START_V2 + 1*DEFAULT_TEST_NUMBER_INCREMENT)

#define FAIL_PXL_CHCK_TEST_NUMBER_START                            (1000)
#define FAIL_PXL_CHCK_TEST_NUM_TOT_PXL_STATUS_BIT                  (FAIL_PXL_CHCK_TEST_NUMBER_START  + 0*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_RX_PXL_STATUS_BIT                   (FAIL_PXL_CHCK_TEST_NUMBER_START  + 1*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_TX_PXL_STATUS_BIT                   (FAIL_PXL_CHCK_TEST_NUMBER_START  + 2*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_AREA_PXL_STATUS_BIT                 (FAIL_PXL_CHCK_TEST_NUMBER_START  + 3*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_PROX_PXL_STATUS_BIT                 (FAIL_PXL_CHCK_TEST_NUMBER_START  + 4*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_N_PXL_FAIL                          (FAIL_PXL_CHCK_TEST_NUMBER_START  + 5*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_RX_MAX_PXL_FAIL_CNT                 (FAIL_PXL_CHCK_TEST_NUMBER_START  + 6*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_TX_MAX_PXL_FAIL_CNT                 (FAIL_PXL_CHCK_TEST_NUMBER_START  + 7*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_AREA_MAX_PXL_FAIL_CNT               (FAIL_PXL_CHCK_TEST_NUMBER_START  + 8*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_PXL_FAIL_PROX_FAIL_CNT              (FAIL_PXL_CHCK_TEST_NUMBER_START  + 9*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_PXL_FAIL_PROX_FAIL_MAX_FAILS        (FAIL_PXL_CHCK_TEST_NUMBER_START  + 10*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_N_PXL_FAIL_LNA_BL_VAL               (FAIL_PXL_CHCK_TEST_NUMBER_START  + 11*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_N_PXL_FAIL_LNA_BL_OK                (FAIL_PXL_CHCK_TEST_NUMBER_START  + 12*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_N_PXL_FAIL_FW_BL_VAL                (FAIL_PXL_CHCK_TEST_NUMBER_START  + 13*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_N_PXL_FAIL_IMG_VAL                  (FAIL_PXL_CHCK_TEST_NUMBER_START  + 14*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_N_PXL_FAIL_IMG_2D_DIFF              (FAIL_PXL_CHCK_TEST_NUMBER_START  + 15*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_N_PXL_FAIL_NOISE                    (FAIL_PXL_CHCK_TEST_NUMBER_START  + 16*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_N_PXL_FAIL_DYN_RANGE                (FAIL_PXL_CHCK_TEST_NUMBER_START  + 17*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_N_PXL_FAIL_DEAD                     (FAIL_PXL_CHCK_TEST_NUMBER_START  + 18*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_MIN_LNA_BL_EDGE                     (FAIL_PXL_CHCK_TEST_NUMBER_START  + 19*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_MAX_LNA_BL_EDGE                     (FAIL_PXL_CHCK_TEST_NUMBER_START  + 20*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_MIN_LNA_BL_TRAN                     (FAIL_PXL_CHCK_TEST_NUMBER_START  + 21*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_MAX_LNA_BL_TRAN                     (FAIL_PXL_CHCK_TEST_NUMBER_START  + 22*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_MIN_LNA_BL_CNTR                     (FAIL_PXL_CHCK_TEST_NUMBER_START  + 23*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_MAX_LNA_BL_CNTR                     (FAIL_PXL_CHCK_TEST_NUMBER_START  + 24*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_MIN_FW_BL_EDGE                      (FAIL_PXL_CHCK_TEST_NUMBER_START  + 25*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_MAX_FW_BL_EDGE                      (FAIL_PXL_CHCK_TEST_NUMBER_START  + 26*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_MIN_FW_BL_TRAN                      (FAIL_PXL_CHCK_TEST_NUMBER_START  + 27*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_MAX_FW_BL_TRAN                      (FAIL_PXL_CHCK_TEST_NUMBER_START  + 28*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_MIN_FW_BL_CNTR                      (FAIL_PXL_CHCK_TEST_NUMBER_START  + 29*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_MAX_FW_BL_CNTR                      (FAIL_PXL_CHCK_TEST_NUMBER_START  + 30*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_MIN_IM_EDGE                         (FAIL_PXL_CHCK_TEST_NUMBER_START  + 31*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_MAX_IM_EDGE                         (FAIL_PXL_CHCK_TEST_NUMBER_START  + 32*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_MIN_IM_TRAN                         (FAIL_PXL_CHCK_TEST_NUMBER_START  + 33*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_MAX_IM_TRAN                         (FAIL_PXL_CHCK_TEST_NUMBER_START  + 34*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_MIN_IM_CNTR                         (FAIL_PXL_CHCK_TEST_NUMBER_START  + 35*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_MAX_IM_CNTR                         (FAIL_PXL_CHCK_TEST_NUMBER_START  + 36*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_MIN_IM_2D_DIFF_EDGE                 (FAIL_PXL_CHCK_TEST_NUMBER_START  + 37*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_MAX_IM_2D_DIFF_EDGE                 (FAIL_PXL_CHCK_TEST_NUMBER_START  + 38*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_MIN_IM_2D_DIFF_TRAN                 (FAIL_PXL_CHCK_TEST_NUMBER_START  + 39*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_MAX_IM_2D_DIFF_TRAN                 (FAIL_PXL_CHCK_TEST_NUMBER_START  + 40*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_MIN_IM_2D_DIFF_CNTR                 (FAIL_PXL_CHCK_TEST_NUMBER_START  + 41*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_MAX_IM_2D_DIFF_CNTR                 (FAIL_PXL_CHCK_TEST_NUMBER_START  + 42*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_MIN_NOISE_EDGE                      (FAIL_PXL_CHCK_TEST_NUMBER_START  + 43*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_MAX_NOISE_EDGE                      (FAIL_PXL_CHCK_TEST_NUMBER_START  + 44*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_MIN_NOISE_TRAN                      (FAIL_PXL_CHCK_TEST_NUMBER_START  + 45*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_MAX_NOISE_TRAN                      (FAIL_PXL_CHCK_TEST_NUMBER_START  + 46*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_MIN_NOISE_CNTR                      (FAIL_PXL_CHCK_TEST_NUMBER_START  + 47*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_MAX_NOISE_CNTR                      (FAIL_PXL_CHCK_TEST_NUMBER_START  + 48*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_MIN_DYN_RANGE_EDGE                  (FAIL_PXL_CHCK_TEST_NUMBER_START  + 49*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_MAX_DYN_RANGE_EDGE                  (FAIL_PXL_CHCK_TEST_NUMBER_START  + 50*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_MIN_DYN_RANGE_TRAN                  (FAIL_PXL_CHCK_TEST_NUMBER_START  + 51*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_MAX_DYN_RANGE_TRAN                  (FAIL_PXL_CHCK_TEST_NUMBER_START  + 52*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_MIN_DYN_RANGE_CNTR                  (FAIL_PXL_CHCK_TEST_NUMBER_START  + 53*DEFAULT_TEST_NUMBER_INCREMENT)
#define FAIL_PXL_CHCK_TEST_NUM_MAX_DYN_RANGE_CNTR                  (FAIL_PXL_CHCK_TEST_NUMBER_START  + 54*DEFAULT_TEST_NUMBER_INCREMENT)


#define SNR_TEST_NUMBER_START                       (1400)
#define SNR_TEST_NUM_NOISE_STATUS_BIT               (SNR_TEST_NUMBER_START + 0*DEFAULT_TEST_NUMBER_INCREMENT)
#define SNR_TEST_NUM_SIGNAL_STATUS_BIT              (SNR_TEST_NUMBER_START + 1*DEFAULT_TEST_NUMBER_INCREMENT)
#define SNR_TEST_NUM_SNR_STATUS_BIT                 (SNR_TEST_NUMBER_START + 2*DEFAULT_TEST_NUMBER_INCREMENT)
#define SNR_TEST_NUM_Z0_NOISE                       (SNR_TEST_NUMBER_START + 5*DEFAULT_TEST_NUMBER_INCREMENT)
#define SNR_TEST_NUM_Z0_SIGNAL                      (SNR_TEST_NUMBER_START + 6*DEFAULT_TEST_NUMBER_INCREMENT)
#define SNR_TEST_NUM_Z0_SNR                         (SNR_TEST_NUMBER_START + 7*DEFAULT_TEST_NUMBER_INCREMENT)
#define SNR_TEST_NUM_Z1_NOISE                       (SNR_TEST_NUMBER_START + 10*DEFAULT_TEST_NUMBER_INCREMENT)
#define SNR_TEST_NUM_Z1_SIGNAL                      (SNR_TEST_NUMBER_START + 11*DEFAULT_TEST_NUMBER_INCREMENT)
#define SNR_TEST_NUM_Z1_SNR                         (SNR_TEST_NUMBER_START + 12*DEFAULT_TEST_NUMBER_INCREMENT)
#define SNR_TEST_NUM_Z2_NOISE                       (SNR_TEST_NUMBER_START + 15*DEFAULT_TEST_NUMBER_INCREMENT)
#define SNR_TEST_NUM_Z2_SIGNAL                      (SNR_TEST_NUMBER_START + 16*DEFAULT_TEST_NUMBER_INCREMENT)
#define SNR_TEST_NUM_Z2_SNR                         (SNR_TEST_NUMBER_START + 17*DEFAULT_TEST_NUMBER_INCREMENT)
#define SNR_TEST_NUM_Z3_NOISE                       (SNR_TEST_NUMBER_START + 20*DEFAULT_TEST_NUMBER_INCREMENT)
#define SNR_TEST_NUM_Z3_SIGNAL                      (SNR_TEST_NUMBER_START + 21*DEFAULT_TEST_NUMBER_INCREMENT)
#define SNR_TEST_NUM_Z3_SNR                         (SNR_TEST_NUMBER_START + 22*DEFAULT_TEST_NUMBER_INCREMENT)
#define SNR_TEST_NUM_Z4_NOISE                       (SNR_TEST_NUMBER_START + 25*DEFAULT_TEST_NUMBER_INCREMENT)
#define SNR_TEST_NUM_Z4_SIGNAL                      (SNR_TEST_NUMBER_START + 26*DEFAULT_TEST_NUMBER_INCREMENT)
#define SNR_TEST_NUM_Z4_SNR                         (SNR_TEST_NUMBER_START + 27*DEFAULT_TEST_NUMBER_INCREMENT)
#define SNR_TEST_NUM_Z5_NOISE                       (SNR_TEST_NUMBER_START + 30*DEFAULT_TEST_NUMBER_INCREMENT)
#define SNR_TEST_NUM_Z5_SIGNAL                      (SNR_TEST_NUMBER_START + 31*DEFAULT_TEST_NUMBER_INCREMENT)
#define SNR_TEST_NUM_Z5_SNR                         (SNR_TEST_NUMBER_START + 32*DEFAULT_TEST_NUMBER_INCREMENT)
#define SNR_TEST_NUM_Z6_NOISE                       (SNR_TEST_NUMBER_START + 35*DEFAULT_TEST_NUMBER_INCREMENT)
#define SNR_TEST_NUM_Z6_SIGNAL                      (SNR_TEST_NUMBER_START + 36*DEFAULT_TEST_NUMBER_INCREMENT)
#define SNR_TEST_NUM_Z6_SNR                         (SNR_TEST_NUMBER_START + 37*DEFAULT_TEST_NUMBER_INCREMENT)


#define WOF_VERIFY_TEST_NUMBER_START                (1600)
#define WOF_VERIFY_TEST_NUM_STATUS_BIT              (WOF_VERIFY_TEST_NUMBER_START + 0*DEFAULT_TEST_NUMBER_INCREMENT)
#define WOF_VERIFY_TEST_NUM_WOF_STRUCT_1_CHECK      (WOF_VERIFY_TEST_NUMBER_START + 1*DEFAULT_TEST_NUMBER_INCREMENT)
#define WOF_VERIFY_TEST_NUM_WOF_STRUCT_2_CHECK      (WOF_VERIFY_TEST_NUMBER_START + 2*DEFAULT_TEST_NUMBER_INCREMENT)
#define WOF_VERIFY_TEST_NUM_WOF_STRUCT_3_CHECK      (WOF_VERIFY_TEST_NUMBER_START + 3*DEFAULT_TEST_NUMBER_INCREMENT)
#define WOF_VERIFY_TEST_NUM_WOF_STRUCT_4_CHECK      (WOF_VERIFY_TEST_NUMBER_START + 4*DEFAULT_TEST_NUMBER_INCREMENT)
#define WOF_VERIFY_TEST_NUM_WOF_STRUCT_5_CHECK      (WOF_VERIFY_TEST_NUMBER_START + 5*DEFAULT_TEST_NUMBER_INCREMENT)
#define WOF_VERIFY_TEST_NUM_WOF_STRUCT_6_CHECK      (WOF_VERIFY_TEST_NUMBER_START + 6*DEFAULT_TEST_NUMBER_INCREMENT)
#define WOF_VERIFY_TEST_NUM_WOF_STRUCT_7_CHECK      (WOF_VERIFY_TEST_NUMBER_START + 7*DEFAULT_TEST_NUMBER_INCREMENT)
#define WOF_VERIFY_TEST_NUM_WOF_STRUCT_8_CHECK      (WOF_VERIFY_TEST_NUMBER_START + 8*DEFAULT_TEST_NUMBER_INCREMENT)
#define WOF_VERIFY_TEST_NUM_WOF_STRUCT_9_CHECK      (WOF_VERIFY_TEST_NUMBER_START + 9*DEFAULT_TEST_NUMBER_INCREMENT)
#define WOF_VERIFY_TEST_NUM_WOF_STRUCT_10_CHECK     (WOF_VERIFY_TEST_NUMBER_START + 10*DEFAULT_TEST_NUMBER_INCREMENT)

#define WOF_CAL_TEST_NUMBER_START                   (1700)
#define WOF_CAL_TEST_NUM_DCA_OFFSET_STATUS_BIT      (WOF_CAL_TEST_NUMBER_START + 0*DEFAULT_TEST_NUMBER_INCREMENT)
#define WOF_CAL_TEST_NUM_SIGNAL_STATUS_BIT          (WOF_CAL_TEST_NUMBER_START + 1*DEFAULT_TEST_NUMBER_INCREMENT)
#define WOF_CAL_TEST_NUM_GAIN                       (WOF_CAL_TEST_NUMBER_START + 2*DEFAULT_TEST_NUMBER_INCREMENT)
#define WOF_CAL_TEST_NUM_DCA_OFFSET                 (WOF_CAL_TEST_NUMBER_START + 3*DEFAULT_TEST_NUMBER_INCREMENT)
#define WOF_CAL_TEST_NUM_SIGNAL                     (WOF_CAL_TEST_NUMBER_START + 4*DEFAULT_TEST_NUMBER_INCREMENT)



#define DEN_HAYS_STIM_COLL_TEST_NUMBER_START                       (1800)
#define DEN_HAYS_STIM_COLL_TEST_NUM_LNA_CFG_CHCK_STATUS_BIT        (DEN_HAYS_STIM_COLL_TEST_NUMBER_START)
#define DEN_HAYS_STIM_COLL_TEST_NUM_SAT_DET_STATUS_BIT             (DEN_HAYS_STIM_COLL_TEST_NUMBER_START + 1*DEFAULT_TEST_NUMBER_INCREMENT)

#define RX_STDEV_TEST_NUMBER_START                                 (1900)
#define RX_STDEV_TEST_NUM_CHCK_STATUS_BIT                          (RX_STDEV_TEST_NUMBER_START + 0*DEFAULT_TEST_NUMBER_INCREMENT)
#define RX_STDEV_TEST_NUM_MIN_RX_STDEV_PER_VAR_FROM_AV_x10         (RX_STDEV_TEST_NUMBER_START + 1*DEFAULT_TEST_NUMBER_INCREMENT)

#define IMPERFECTIONS_TEST_NUMBER_START                            (2000)
#define IMPERFECTIONS_TEST_NUM_CHCK_STATUS_BIT                     (IMPERFECTIONS_TEST_NUMBER_START + 0*DEFAULT_TEST_NUMBER_INCREMENT)
#define IMPERFECTIONS_TEST_NUM_Z0_MEASURE                          (IMPERFECTIONS_TEST_NUMBER_START + 1*DEFAULT_TEST_NUMBER_INCREMENT)
#define IMPERFECTIONS_TEST_NUM_Z1_MEASURE                          (IMPERFECTIONS_TEST_NUMBER_START + 2*DEFAULT_TEST_NUMBER_INCREMENT)
#define IMPERFECTIONS_TEST_NUM_Z2_MEASURE                          (IMPERFECTIONS_TEST_NUMBER_START + 3*DEFAULT_TEST_NUMBER_INCREMENT)
#define IMPERFECTIONS_TEST_NUM_Z3_MEASURE                          (IMPERFECTIONS_TEST_NUMBER_START + 4*DEFAULT_TEST_NUMBER_INCREMENT)
#define IMPERFECTIONS_TEST_NUM_Z4_MEASURE                          (IMPERFECTIONS_TEST_NUMBER_START + 5*DEFAULT_TEST_NUMBER_INCREMENT)
#define IMPERFECTIONS_TEST_NUM_Z5_MEASURE                          (IMPERFECTIONS_TEST_NUMBER_START + 6*DEFAULT_TEST_NUMBER_INCREMENT)
#define IMPERFECTIONS_TEST_NUM_Z6_MEASURE                          (IMPERFECTIONS_TEST_NUMBER_START + 7*DEFAULT_TEST_NUMBER_INCREMENT)


#define FIB_CHCKS_TEST_NUMBER_START                                 (2200)
#define FIB_CHCKS_TEST_NUM_CHCK_STATUS_BIT                          (FIB_CHCKS_TEST_NUMBER_START + 0*DEFAULT_TEST_NUMBER_INCREMENT)
#define FIB_CHCKS_TEST_NUM_CHCK_OK_RESULTS                          (FIB_CHCKS_TEST_NUMBER_START + 1*DEFAULT_TEST_NUMBER_INCREMENT)
#define FIB_CHCKS_TEST_NUM_CHCKS_FOR_PASS_RESULTS                   (FIB_CHCKS_TEST_NUMBER_START + 2*DEFAULT_TEST_NUMBER_INCREMENT)

#define IOTA_CHCKS_TEST_NUMBER_START                                (2250)
#define IOTA_CHCKS_TEST_NUM_CHCK_STATUS_BIT                         (IOTA_CHCKS_TEST_NUMBER_START + 0*DEFAULT_TEST_NUMBER_INCREMENT)
#define IOTA_CHCKS_TEST_NUM_CHCK_OK_RESULTS                         (IOTA_CHCKS_TEST_NUMBER_START + 1*DEFAULT_TEST_NUMBER_INCREMENT)
#define IOTA_CHCKS_TEST_NUM_CHCKS_FOR_PASS_RESULTS                  (IOTA_CHCKS_TEST_NUMBER_START + 2*DEFAULT_TEST_NUMBER_INCREMENT)





/* ********************************************************************** */
/* Common Patch Type Definitions                                          */
/*   - Use an entry in debug data in patch reply to indicate the type     */
/*     of test(s) that the patch executes                                 */
/* ********************************************************************** */
#define OPEN_SHORT_TEST_PATCH                (10)      /* Open Short Patch */
#define CONSOLIDATED_TEST_PATCH              (20)      /* Consolidated Test Patch */



/* ********************************************************************** */
/* Common Package Type Definitions                                        */
/*   - Use an entry in debug data in patch reply to indicate the package  */
/*     type(s) that the patch is used for                                 */
/* ********************************************************************** */

#define PACKAGE_DENALI_95B                  (0x095b)     /* Package = 95b */ 
#define PACKAGE_DENALI_93A                  (0x093a)     /* Package = 93a */ 


