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
 * Implement the VCSFW_CMD_WOF_BAELINE VCSFW_CMD_WOF_SIGNAL command
 *
 */



/* ************************************************************************* */
/* Patch Version Information                                                 */
/* ************************************************************************* */
#define PATCH_BUILD_MONTH       (3)
#define PATCH_BUILD_DAY         (30)
#define PATCH_BUILD_YEAR        (17)
#define PATCH_BUILD_VERSION     (3)


/* ************************************************************************* */
/* *** Patch Information Definitions                                         */
/* ************************************************************************* */
#define TEST_PATCH_TYPE                       (((OPEN_SHORT_TEST_PATCH & 0xffff) << 16) | (PACKAGE_DENALI_95B & 0xffff))  		
#define TEST_PATCH_ID_INFO                    (((PATCH_BUILD_MONTH & 0xff) << 24) | ((PATCH_BUILD_DAY & 0xff) << 16) | ((PATCH_BUILD_YEAR & 0xff)<<8) | PATCH_BUILD_VERSION )




#include "basetypes.h"
#include "bootldr.h"
#include "hwdef.h"
#include "hwreg.h"
#include "reg.h"
#include "fwcmd.h"
#include "cmdmgr.h"
#include "open_short.h"
#include "Test_codes_def.h"

extern uint32_t crc32(const uint32_t *ptr, unsigned int nbytes);

/* Set this to 1 to enable peek/poke */
#define UNIT_TEST 0

#if NASSAU_PRODUCT==NASSAU_PRODUCT_DENALI
#   define PATCH_PBL_PRODUCT VCSFW_PRODUCT_DENALIPBL
#elif NASSAU_PRODUCT==NASSAU_PRODUCT_HAYES
#   define PATCHOPT_FLASH_SOFTPART
#   define PATCH_PBL_PRODUCT VCSFW_PRODUCT_HAYESPBL
#elif NASSAU_PRODUCT==NASSAU_PRODUCT_SHASTA
#   define PATCHOPT_FLASH_SOFTPART
#   define PATCH_PBL_PRODUCT VCSFW_PRODUCT_SHASTAPBL
//#define OUTPUT_WITH_TXRX_NUMBER
//#define ALL_TX_SHORT_OUTPUT
#endif


#define wr_word(a, d)          (*((volatile unsigned int*)(a))) = (unsigned int)(d)
#define rd_word(a, d)          d = (*((volatile unsigned int*)(a)))

/*
* Handy logical testing macros.
*/
#define REG_BITISON(val, mask)                                              \
    (((val) & (mask)) == (mask))
#define REG_BITISOFF(val, mask)                                             \
    (((val) & (mask)) == 0)
#define REG_BITISON_B(val, bitnum)                                          \
    ((((val) >> (bitnum)) & 0x1) != 0)
#define REG_BITISOFF_B(val, bitnum)                                         \
    ((((val) >> (bitnum)) & 0x1) == 0)
#define REG_ALLBITSAREON(val, mask)                                         \
    (((val) & (mask)) == (mask))
#define REG_ANYBITSAREON(val, mask)                                         \
    (((val) & (mask)) != 0)
#define REG_ALLBITSAREOFF(val, mask)                                        \
    (((val) & (mask)) == 0)
#define REG_ANYBITSAREOFF(val, mask)                                        \
    (((val) & (mask)) != (mask))


/* -------------------------------------------------------------------------- */
/* Static prototypes */
static int patch_cmd_handler_test_Tx_shorts( void *ctxp, const uint8_t *cmdbufp, unsigned int cmdlen, uint8_t *replyp );
static int patch_cmd_handler_test_Rx_shorts( void *ctxp, const uint8_t *cmdbufp, unsigned int cmdlen, uint8_t *replyp );
static int patch_cmd_handler_test_TxRx_open( void *ctxp, const uint8_t *cmdbufp, unsigned int cmdlen, uint8_t *replyp );
static int patch_cmd_handler_test_open_shorts( void *ctxp, const uint8_t *cmdbufp, unsigned int cmdlen, uint8_t *replyp );
static int test( void *ctxp, const uint8_t *cmdbufp, unsigned int cmdlen, uint8_t *replyp );


static int test_Tx_shorts( uint8_t *replyp );


#if UNIT_TEST
static int patch_cmd_handler_peek(void *ctxp, const uint8_t *cmdbufp, 
                                           unsigned int cmdlen, uint8_t *replyp);
static int patch_cmd_handler_poke(void *ctxp, const uint8_t *cmdbufp, 
                                            unsigned int cmdlen, uint8_t *replyp);
#endif

/* -------------------------------------------------------------------------- */
/* Static initialized data  */

static cmdmgr_cmdentry_t patch_cmdtable[] = {
    { VCSFW_CMD_SHORT_TX,   &patch_cmd_handler_test_Tx_shorts   },
    { VCSFW_CMD_SHORT_RX,   &patch_cmd_handler_test_Rx_shorts   },
//    { VCSFW_CMD_OPEN_TXRX,  &patch_cmd_handler_test_TxRx_open   },
	{ VCSFW_CMD_OPEN_TXRX,  &test   },
    { VCSFW_CMD_OPEN_SHORT, &patch_cmd_handler_test_open_shorts },    
#if UNIT_TEST
    { VCSFW_CMD_PEEK, &patch_cmd_handler_peek },
    { VCSFW_CMD_POKE, &patch_cmd_handler_poke }
#endif
};


/* -------------------------------------------------------------------------- */
/* Global functions */

int
patch_start(void *ctxp, uint8_t *replyp)
{
    if (((const bootldr_ver_roinfo_t *)
         BOOTLDR_MEMMAP_VER_ROINFO_BASE)->product != PATCH_PBL_PRODUCT) {
        return VCSFW_STATUS_ERR_BOOTLDR_PATCH_PRODUCT;
    }

    return cmdmgr(ctxp, &(patch_cmdtable[0]), NELEM(patch_cmdtable));
}

static int patch_cmd_handler_test_open_shorts( void *ctxp, const uint8_t *cmdbufp, unsigned int cmdlen, uint8_t *replyp )
{
	test_Tx_shorts( ( uint8_t *) replyp );

	if( TEST_PXLS_PER_RX == 112 ) // Rx = 112, rectangle sensor
	{
	#ifdef ALL_TX_SHORT_OUTPUT
		cmdmgr_reply( ctxp, (uint8_t *)(((uint16_t *) replyp)+1),
	                 sizeof(uint16_t) + 9 * TEST_PXLS_PER_RX  * sizeof(uint16_t));
	#else
	#ifdef OUTPUT_WITH_TXRX_NUMBER
		// debug output
		cmdmgr_reply( ctxp, (uint8_t *)(((uint16_t *) replyp)+1),
	                 sizeof(uint16_t) + 2 * TEST_PXLS_PER_RX  * sizeof(uint16_t));
	#else  
		// release output
		cmdmgr_reply( ctxp, (uint8_t *)(((uint16_t *) replyp)+1),
	                 sizeof(uint16_t) + 1 * TEST_PXLS_PER_RX  * sizeof(uint16_t));
	#endif
	#endif              
	}
	else	// Rx = 88, square sensor
	{
	#ifdef ALL_TX_SHORT_OUTPUT
		cmdmgr_reply( ctxp, (uint8_t *)(((uint16_t *) replyp) + 1 ), sizeof(uint16_t) + 9 * 112  * sizeof(uint16_t));	// output all 112 Tx
	#else
	#ifdef OUTPUT_WITH_TXRX_NUMBER
		// debug output
		*(((uint16_t *) replyp) + 1 + 12 * 2 ) = *(((uint16_t *) replyp) + 1 );      // copy VCSFW_STATUS to Tx11
		cmdmgr_reply( ctxp, (uint8_t *)(((uint16_t *) replyp) + 1 + 12 * 2 ), sizeof(uint16_t) + 2 * TEST_PXLS_PER_RX  * sizeof(uint16_t)); // output 88 Tx
	#else  
		// release output
		*(((uint16_t *) replyp) + 1 + 12 ) = *(((uint16_t *) replyp) + 1 );          // copy VCSFW_STATUS to Tx11
		cmdmgr_reply( ctxp, (uint8_t *)(((uint16_t *) replyp) + 1 + 12 ), sizeof(uint16_t) + 1 * TEST_PXLS_PER_RX  * sizeof(uint16_t)); // output 88 Tx
	#endif
	#endif  
	}
 
	return -1;
}

#if 1
#ifdef BUILD_FOR_ATE  // TODO : need to check for ATE build

//==================================================================================================
//                               PATID Variables for ATE
//``````````````````````````````````````````````````````````````````````````````````````````````````
// __Generate_patid__starts__ 
uint32_t   use_pad_ids        = 0;         /* Set to a non-zero value to actually use the padid's.  Otherwise the built-in values in the test_setup_limits file are used */

uint32_t   lna_bl_skip_frames = 2;
uint32_t   fw_bl_skip_frames = 0;

int32_t    tx_open_params_min_limit =  50;
int32_t    tx_open_params_max_limit = 200;

int32_t    rx_open_max_delta_frm_avg_min_limit =   0;
int32_t    rx_open_max_delta_frm_avg_max_limit = 10000; //essentially disable this test

int32_t    rx_open_max_range_min_limit = 0;
int32_t    rx_open_max_range_max_limit = 1000;

int32_t    rx_open_max_tx0_min_limit =  0;
int32_t    rx_open_max_tx0_max_limit = 1000;

int32_t    rx_lna_tolerance = 200;

uint32_t   n_rx_open_skip = 2;
uint32_t   rx_open_skip[MAX_RX_OPEN_SKIP] = {0, 79, 0, 0, 0, 0, 0, 0, 0};

uint32_t   ate_ctrl_code = (CTRL_CODE_CLEAR_MEM_BUFFERS        |
                            CTRL_CODE_INC_CHECK_MASK_ID        |
                            CTRL_CODE_INC_BIST_TX_SHORT        |
                            CTRL_CODE_INC_BIST_RX_SHORT        |
                            CTRL_CODE_INC_RX_OPEN              |
                            CTRL_CODE_INC_TX_OPEN              |
                            CTRL_CODE_INC_VERIFY_FIB_VALS      |
	                        0);

uint32_t   limit_set     = 0;  


/* Results Structure */
results_struct_t  results;



/* Include the test setup / limit information */
#include "test_setup_limits.c"

void run_test(results_struct_t  *results);

static int test( void *ctxp, const uint8_t *cmdbufp, unsigned int cmdlen, uint8_t *replyp )
{
    uint32_t   i;

#if 1
    /* Code to setup the part on the ATE, not needed in firmware patch  */
    wr_word(WOE_HV_DPOR, WOE_HV_DPOR_KEY_VALUE);
    wr_word(WOE_STARTUP_DELAY,0x00020010);   // cpu fast clock
#endif

    /* Code to map the ATE padid's to the appropriate setup variables */
    if (use_pad_ids)
    {
        test_const_params.lna_bl_skip_frames = lna_bl_skip_frames & 0xff;
        test_const_params.fw_bl_skip_frames = fw_bl_skip_frames & 0xff;

        test_params[limit_set].tx_open.min = tx_open_params_min_limit;
        test_params[limit_set].tx_open.max = tx_open_params_max_limit;

        test_params[limit_set].rx_open_max_delta_frm_avg.min = rx_open_max_delta_frm_avg_min_limit;
        test_params[limit_set].rx_open_max_delta_frm_avg.max = rx_open_max_delta_frm_avg_max_limit;

        test_params[limit_set].rx_open_max_range.min = rx_open_max_range_min_limit;
        test_params[limit_set].rx_open_max_range.max = rx_open_max_range_max_limit;

        test_params[limit_set].rx_open_max_tx0.min = rx_open_max_tx0_min_limit;
        test_params[limit_set].rx_open_max_tx0.max = rx_open_max_tx0_max_limit;

        test_const_params.rx_lna_tolerance = rx_lna_tolerance;
        test_const_params.n_rx_open_skip = n_rx_open_skip & 0xff;
        for(i=0; i<n_rx_open_skip; i++)
        {
            test_const_params.rx_open_skip[i] = rx_open_skip[i] & 0xff;
        }

        test_params[limit_set].ctrl_code     = ate_ctrl_code;
    }

    /* Execute the test function */
    run_test(&results);


   	*( ((uint16_t *) replyp ) + 1 ) = VCSFW_STATUS_OKAY;
	uint16_t *rp = ( uint16_t * ) replyp + 1;

	rp++;
	*rp++ = *( ( volatile unsigned short * )SCM_SCMSTAT );
	*rp++ = *( ( volatile unsigned short * )( SCM_SCMSTAT + 2) );

	*rp++ = *( ( volatile unsigned short * )SCM_TXCFG0 );
	*rp++ = *( ( volatile unsigned short * )( SCM_TXCFG0 + 2) );
	*rp++ = *( ( volatile unsigned short * )SCM_TXCFG1 );
	*rp++ = *( ( volatile unsigned short * )( SCM_TXCFG1 + 2) );
	*rp++ = *( ( volatile unsigned short * )SCM_TXCFG2 );
	*rp++ = *( ( volatile unsigned short * )( SCM_TXCFG2 + 2) );
	*rp++ = *( ( volatile unsigned short * )SCM_TXCFG3 );
	*rp++ = *( ( volatile unsigned short * )( SCM_TXCFG3 + 2) );
	
	*rp++ = *( ( volatile unsigned short * )CDM_CDMCFG );
	*rp++ = *( ( volatile unsigned short * )( CDM_CDMCFG + 2) );
	
	*rp++ = *( ( volatile unsigned short * )CDM_CDMSTAT );
	*rp++ = *( ( volatile unsigned short * )( CDM_CDMSTAT + 2) );
	
	*rp++ = *( ( volatile unsigned short * )WOE_HS_OSC_TRIM );
	*rp++ = 0xffff;

	*rp++ = *( ( volatile unsigned short * )WOE_DVREG );
	*rp++ = 0xffff;

	*rp++ = *( ( volatile unsigned short * )BC_CB_STRT_ADDR );
	*rp++ = *( ( volatile unsigned short * )( BC_CB_STRT_ADDR + 2) );

	*rp++ = *( ( volatile unsigned short * )BC_CB_END_ADDR );
	*rp++ = *( ( volatile unsigned short * )( BC_CB_END_ADDR + 2) );

	*rp++ = 0xffff;
	*rp++ = 0xffff;

	//results.status[0] = 0x55555555;
	
#if 0
	*rp++ = ( ( volatile unsigned short * )dummybuf );
	*rp++ = ( ( volatile unsigned short * )( dummybuf + 2) );

	*rp++ = ( ( volatile unsigned short * )dummyptr );
	*rp++ = ( ( volatile unsigned short * )( dummyptr + 2) );
#endif




	

#if 0	
		for( i = 0; i < HWDEF_MEMMAP_ADCRAM1_SIZE / 4 - 50; i++ )
		{
			*rp++ = *((volatile unsigned short*)( HWDEF_MEMMAP_ADCRAM1_BASE + i * 4 ) );
			*rp++ = *((volatile unsigned short*)( HWDEF_MEMMAP_ADCRAM1_BASE + i * 4 + 2 ) );
		}
		
		for( i = 0; i < HWDEF_MEMMAP_FW_BL_RAM_SIZE / 4 - 50; i++ )
		{
			*rp++ = *((volatile unsigned short*)( HWDEF_MEMMAP_FW_BL_RAM_BASE + i * 4 ) );
			*rp++ = *((volatile unsigned short*)( HWDEF_MEMMAP_FW_BL_RAM_BASE + i * 4 + 2 ) );
		}
	
		// can not access when frame collection
		for( i = 0; i < HWDEF_MEMMAP_LNA_BL_RAM_SIZE / 4 - 50; i++ )
		{
			*rp++ = *((volatile unsigned short*)( HWDEF_MEMMAP_LNA_BL_RAM_BASE + i * 4 ) );
			*rp++ = *((volatile unsigned short*)( HWDEF_MEMMAP_LNA_BL_RAM_BASE + i * 4 + 2 ) );
		}
		
		for( i = 0; i < HWDEF_MEMMAP_CDMRAM_SIZE / 4 - 50; i++ )
		{
			*rp++ = *((volatile unsigned short*)( HWDEF_MEMMAP_CDMRAM_BASE + i * 4 ) );
			*rp++ = *((volatile unsigned short*)( HWDEF_MEMMAP_CDMRAM_BASE + i * 4 + 2 ) );
		}
	
		// can not access when frame collection
		for( i = 0; i < HWDEF_MEMMAP_DMRAM_SIZE / 4 - 50; i++ )
		{
			*rp++ = *((volatile unsigned short*)( HWDEF_MEMMAP_DMRAM_BASE + i * 4 ) );
			*rp++ = *((volatile unsigned short*)( HWDEF_MEMMAP_DMRAM_BASE + i * 4 + 2 ) );
		}
		
		for( i = 0; i < HWDEF_MEMMAP_IMGRAM_SIZE / 4 - 50; i++ )
		{
			*rp++ = *((volatile unsigned short*)( HWDEF_MEMMAP_IMGRAM_BASE + i * 4 ) );
			*rp++ = *((volatile unsigned short*)( HWDEF_MEMMAP_IMGRAM_BASE + i * 4 + 2 ) );
		}
			
			// just for debug
			for( i = 0; i < 100; i++ )
		{
			*++rp = i + 5;
		}
#endif

//	cmdmgr_reply( ctxp, ( uint8_t* )((( uint16_t* ) replyp ) + 1 ), sizeof( uint16_t ) + 2 * 4096  * sizeof( uint16_t ) );
	uint16_t *presults = ( uint16_t * )( &results );
	for( i = 0; i < sizeof( results ) / 2; i++ )
	{
		*rp++ = *presults++;
	}
	cmdmgr_reply( ctxp, ( uint8_t* )((( uint16_t* ) replyp ) + 1 ), sizeof( results ) *  sizeof( uint16_t ) );


	return -1;
}

#else /* !BUILD_FOR_ATE */

/*	Which limit set should be used
 *	0 --> ATE Test Limits
 *	1 --> MT BGA Test Limits
 *	2 --> MT Test Limits
 *	3 --> MT Final Test Limits
 *	4 --> In-System Test Limits 
 *	5 --> SW Test Limits
 */
#ifdef BUILD_FOR_ATE
static uint32_t limit_set                             = 0; 
#elif BUILD_FOR_IST
static uint32_t limit_set                             = 4; 
#else /* BUILD_FOR_MT */
static uint32_t limit_set                             = 2; 
#endif


/* Include the test setup / limit information */
#include "test_setup_limits.c"



#endif /* !BUILD_FOR_ATE */


/*
 * Read a 32-bit value from the fib.  Note: ptr must
 *  be a 32-bit aligned pointer.
 */
#define NVM_FIB_READ32(ptr)     (*((const uint32_t *) ((const void *) (ptr))))

/* Determine if the given FIB value is valid (high bits are 2'b01 or 2'b10) */
#define HWINIT_FIB_ISVALID(x)   (((x) >> 30) == 1 || ((x) >> 30) == 2)

/* Strip off the "validity" bits at the top */
#define HWINIT_FIB_VAL(x)       ((x) & 0x3fffffff)


#define SET_STATUS_BIT(bit_num)   (results->status[(bit_num)/32] |= 1 << ((bit_num)%32))
#define CLEAR_STATUS_BIT(bit_num) (results->status[(bit_num)/32] &= (~(1 << ((bit_num)%32))))

//==================================================================================================
//                               Miscellaneous Variables
//``````````````````````````````````````````````````````````````````````````````````````````````````
static uint32_t ctrl_code                             = 0;
static uint8_t limit_or_mask;  



//==================================================================================================
//                              Support Test Function Defintion
//``````````````````````````````````````````````````````````````````````````````````````````````````

#include "get_bin_fail_list.h"



void build_bin_fail_list(results_struct_t *results)
{
#if 1
    sts_bin_list_elem_t bit_bin_list[13];

	bit_bin_list[0].sts_bit = TEST_STATUS_PASS;
	bit_bin_list[0].bin = BIN_CODE_PASS;
	bit_bin_list[1].sts_bit = TEST_STATUS_FAIL_INTERNAL;
	bit_bin_list[1].bin = BIN_CODE_FAIL_PATCH_INTERNAL_ERROR;
	bit_bin_list[2].sts_bit = TEST_STATUS_FAIL_FIB_VAL_CHCK;
	bit_bin_list[2].bin = BIN_CODE_FAIL_FIB_CHECK;
	bit_bin_list[3].sts_bit = TEST_STATUS_FAIL_MASK_ID;
	bit_bin_list[3].bin = BIN_CODE_FAIL_LAYER_ID_CHECK;
	bit_bin_list[4].sts_bit = TEST_STATUS_FAIL_BIST_RX_SHORT;
	bit_bin_list[4].bin = BIN_CODE_FAIL_RX_SHORT_CHECK;
	bit_bin_list[5].sts_bit = TEST_STATUS_FAIL_BIST_TX_SHORT;
	bit_bin_list[5].bin = BIN_CODE_FAIL_TX_SHORT_CHECK;
	bit_bin_list[6].sts_bit = TEST_STATUS_FAIL_RX_OPEN;
	bit_bin_list[6].bin = BIN_CODE_FAIL_RX_OPEN_CHECK;
	bit_bin_list[7].sts_bit = TEST_STATUS_FAIL_RX_SHORT;
	bit_bin_list[7].bin = BIN_CODE_FAIL_RX_SHORT_CHECK;
	bit_bin_list[8].sts_bit = TEST_STATUS_FAIL_TX_OPEN;
	bit_bin_list[8].bin = BIN_CODE_FAIL_TX_OPEN_CHECK;
	bit_bin_list[9].sts_bit = TEST_STATUS_FAIL_RX_OPEN_IMG;
	bit_bin_list[9].bin = BIN_CODE_FAIL_RX_OPEN_CHECK;
	bit_bin_list[10].sts_bit = TEST_STATUS_FAIL_RX_OPEN_IMG_DELTA;
	bit_bin_list[10].bin = BIN_CODE_FAIL_RX_OPEN_CHECK;
	bit_bin_list[11].sts_bit = TEST_STATUS_FAIL;
	bit_bin_list[11].bin = BIN_CODE_FAIL_OPSH_PATCH_RUN;
	bit_bin_list[12].sts_bit = STS_BIT_LIST_END;
	bit_bin_list[12].bin = 255;
#else
const sts_bin_list_elem_t bit_bin_list[] = {
	{TEST_STATUS_PASS,					 BIN_CODE_PASS},
	{TEST_STATUS_FAIL_INTERNAL, 		 BIN_CODE_FAIL_PATCH_INTERNAL_ERROR},
	{TEST_STATUS_FAIL_FIB_VAL_CHCK, 	 BIN_CODE_FAIL_FIB_CHECK},
	{TEST_STATUS_FAIL_MASK_ID,			 BIN_CODE_FAIL_LAYER_ID_CHECK},
	{TEST_STATUS_FAIL_BIST_RX_SHORT,	 BIN_CODE_FAIL_RX_SHORT_CHECK},
	{TEST_STATUS_FAIL_BIST_TX_SHORT,	 BIN_CODE_FAIL_TX_SHORT_CHECK},
	{TEST_STATUS_FAIL_RX_OPEN,			 BIN_CODE_FAIL_RX_OPEN_CHECK},
	{TEST_STATUS_FAIL_RX_SHORT, 		 BIN_CODE_FAIL_RX_SHORT_CHECK},
	{TEST_STATUS_FAIL_TX_OPEN,			 BIN_CODE_FAIL_TX_OPEN_CHECK},
	{TEST_STATUS_FAIL_RX_OPEN_IMG,		 BIN_CODE_FAIL_RX_OPEN_CHECK},
	{TEST_STATUS_FAIL_RX_OPEN_IMG_DELTA, BIN_CODE_FAIL_RX_OPEN_CHECK},			
	{TEST_STATUS_FAIL,					 BIN_CODE_FAIL_OPSH_PATCH_RUN},
	{STS_BIT_LIST_END,					 255},
};

#endif

    results->n_bin_codes = BIN_CODE_LIST_MAX_QNTY;
    get_bin_fail_list(&results->status[0], &bit_bin_list[0], 
        &results->bin_codes[0], &results->n_bin_codes);
}

void check_value_add_fail_count(int32_t value,  uint8_t fail_bit,  limits_t *limits,  results_struct_t *results, uint32_t count_param)
{
    if ((value < limits->min) || (value > limits->max))
	{
		SET_STATUS_BIT(fail_bit);
        results->parameter[count_param]++;
	}
}


/* ************************************************************************* */
/* LNA BL Calibration Functions                                              */
/* ************************************************************************* */

void collect_frame(void)
{
    uint32_t   rd_data;
    uint32_t   i;
    uint32_t   j;
    uint32_t   addr;
    uint32_t   val;

    addr = SCM_LNA_CFG0;
    val = 0x00002020;
    for(i=0; i<80; i++, addr +=4)
    {
     REG_VARWRITE32(addr, val);
    }

    REG_WRITE32(SCM_SCMIE, 0);
    REG_WRITE32(SCM_SCMCMD, SCM_SCMCMD_SCMSRST);
    for (j = 0; j<200; j++) asm("nop");
    REG_WRITE32(SCM_SCMCMD, 0);

    /* Start Frame Collection */
    REG_WRITE32(SCM_SCMCMD, REG_STUFFINT32(SCM_SCMCMD_RUNCMD_RUN_SCAN,
                                           SCM_SCMCMD_RUNCMD_B,
                                           SCM_SCMCMD_RUNCMD));
    // Wait for SCM scan end
    do {
        rd_data = REG_READ32(SCM_SCMSTAT);
    } while (REG_BITISOFF_B(rd_data, SCM_SCMSTAT_SE_B));

    REG_WRITE32(SCM_SCMSTAT, rd_data);
    // Wait till CDM busy deasserted
    do {
        rd_data = REG_READ32(CDM_CDMSTAT);
    } while (REG_BITISON_B(rd_data, CDM_CDMSTAT_CDMBUSY_B));
}


static unsigned int
cal_bl_getj(unsigned int i)
{
    unsigned int j;
    if(i<2016)
           j = (i%14)*144 + i/14;
    else if(i<4032)
           j = 2016 + ((i-2016)%14)*144 + (i-2016)/14;
    else if(i<6048)
           j = 4032 + ((i-4032)%14)*144 + (i-4032)/14;
    else
           j = 6048 + ((i-6048)%14)*144 + (i-6048)/14;

    return j;
}
static int
cal_bl_getcenter(unsigned int i,int32_t *target)
{
     int center;
     if(i<2016)  // sub_frame 0
       {
            switch(i%14){
                case 0: center = target[0]; break;
                case 1: center = target[1]; break;
                case 2: center = target[8]; break;
                case 3: center = target[9]; break;
                case 4: center = target[16]; break;
                case 5: center = target[17]; break;
                case 6: center = target[24]; break;
                case 7: center = target[25]; break;
                case 8: center = target[32]; break;
                case 9: center = target[33]; break;
                case 10: center = target[40]; break;
                case 11: center = target[41]; break;
                case 12: center = target[48]; break;
                case 13: center = target[49]; break;
            }
       }
       else if(i<4032)  // sub_frame 1
       {
            switch(i%14){
                case 0: center = target[2]; break;
                case 1: center = target[3]; break;
                case 2: center = target[10]; break;
                case 3: center = target[11]; break;
                case 4: center = target[18]; break;
                case 5: center = target[19]; break;
                case 6: center = target[26]; break;
                case 7: center = target[27]; break;
                case 8: center = target[34]; break;
                case 9: center = target[35]; break;
                case 10: center = target[42]; break;
                case 11: center = target[43]; break;
                case 12: center = target[50]; break;
                case 13: center = target[51]; break;
            }
       }
       else if(i<6048)   // sub_frame 2
       {
            switch(i%14){
                case 0: center = target[4]; break;
                case 1: center = target[5]; break;
                case 2: center = target[12]; break;
                case 3: center = target[13]; break;
                case 4: center = target[20]; break;
                case 5: center = target[21]; break;
                case 6: center = target[28]; break;
                case 7: center = target[29]; break;
                case 8: center = target[36]; break;
                case 9: center = target[37]; break;
                case 10: center = target[44]; break;
                case 11: center = target[45]; break;
                case 12: center = target[52]; break;
                case 13: center = target[53]; break;
            }
       }
       else   // sub_frame 3
       {
            switch(i%14){
                case 0: center = target[6]; break;
                case 1: center = target[7]; break;
                case 2: center = target[14]; break;
                case 3: center = target[15]; break;
                case 4: center = target[22]; break;
                case 5: center = target[23]; break;
                case 6: center = target[30]; break;
                case 7: center = target[31]; break;
                case 8: center = target[38]; break;
                case 9: center = target[39]; break;
                case 10: center = target[46]; break;
                case 11: center = target[47]; break;
                case 12: center = target[54]; break;
                case 13: center = target[55]; break;
            }
       }
       return center;
}


static bool_t
cal_bl_binarysearch(int itr, int32_t *target, int tolerance)
{
    //*****************************************//
    // Need to replace the hardcoded constant in following statements:
    //        tmp >= center+tolerance*4/5 and
    //        pFW_BL_RAM16[j] = itr>4? 1:0
    //*****************************************//

    unsigned int i,j;
    short tmp;
    short temp;
    short FW_tmp;
    int center;
    bool_t flag;
    volatile uint32_t *pIMGRAM;
    volatile uint16_t *pIMGRAM16;
    volatile uint32_t *pLNA_BL_RAM;
    volatile uint16_t *pLNA_BL_RAM16;
    volatile uint16_t *pFW_BL_RAM16;

    flag = FALSE;
    pIMGRAM = (volatile uint32_t *) HWDEF_MEMMAP_IMGRAM_BASE;
    pIMGRAM16 = (volatile uint16_t *) HWDEF_MEMMAP_IMGRAM_BASE;
    pLNA_BL_RAM = (volatile uint32_t *) HWDEF_MEMMAP_LNA_BL_RAM_BASE;
    pLNA_BL_RAM16 = (volatile uint16_t *) HWDEF_MEMMAP_LNA_BL_RAM_BASE;
    pFW_BL_RAM16 = (volatile uint16_t *) HWDEF_MEMMAP_FW_BL_RAM_BASE;

    for(i=0;i<TEST_TOTAL_PXLS;i++)    
    {
           // j = cal_bl_getj(i);
           j=i;
           tmp = pIMGRAM16[i];
           temp = pLNA_BL_RAM16[j];
           FW_tmp = pFW_BL_RAM16[j];
//           center = cal_bl_getcenter(i,target);
           center = 0;
           if(temp<0)
           {
                  temp = -1*(temp & 0x0fff);
           }
           if(tmp <= center-tolerance)      // Imgram data is below the target range
           {
                  temp += 2048>>(itr);
                  flag = 1;
           }
           else if(tmp >= center+tolerance)    // Imgram data is above the target range
           {
                  temp -= 2048>>(itr);
                  flag = 1;
           }
           else if(tmp >= center+tolerance*4/5)   // Imgram data is inside target range but close to lower boundary (Intended to handle noise drifting)
           {
                   temp -= 1;
                   flag = 1;
           }
           else if(tmp <= center-tolerance*4/5)   // Imgram data is inside target range but close to upper boundary (Intended to handle noise drifting)
           {
                   temp += 1;
                   flag = 1;
           }
           else                                   // Imgram data is inside target range and solid
           {
              pFW_BL_RAM16[j] = itr>4? 1:0;
           }
           //write back to LNA_BL_RAM
           if(!FW_tmp)
           {
           pLNA_BL_RAM16[j] = temp >= 0? temp : ((-1)*temp)|0xf000;
           }
    }
    return flag;
}

                          /* Rx    0,     1,     2,     3,     4,     5,     6,     7,     8,     9,    10,    11,    12,    13, */
int32_t    rx_lna_center[56] = {   0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,  

                          /* Rx   14,    15,    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,    26,    27, */
                                   0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0, 

                          /* Rx   28,    29,    30,    31,    32,    33,    34,    35,    36,    37,    38,    39,    40,    41, */
                                  0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0, 

                          /* Rx   42,    43,    44,    45,    46,    47,    48,    49,    50,    51,    52,    53,    54,    55, */
                                   0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0}; 

uint8_t            *dummyptr;
uint32_t           dummybuf[(2*HWDEF_DCACHE_LINESIZE)/sizeof(uint32_t) - 1];

void cal_bl(results_struct_t *results)
{
    uint32_t           itr; 
    uint32_t           idx;
    uint32_t           bsflag;
    uint32_t           orig_rcm_adc_clk;
    uint32_t           orig_scm_scmie;
    uint32_t           orig_cdm_cdmcfg, cdm_cdmcfg;
    volatile uint16_t  *pIMGRAM16;
    volatile uint16_t  *pFW_BL_RAM16;
    volatile uint16_t  *pLNA_BL_RAM16;

    /*
     * Preserve a bunch of registers so we can restore them later.
     */
    orig_rcm_adc_clk = REG_READ32(RCM_ADC_CLK);
    REG_WRITE32(SCM_MASKID_HI, 0xffffffff); //precharge
    orig_scm_scmie = REG_READ32(SCM_SCMIE);
    orig_cdm_cdmcfg = REG_READ32(CDM_CDMCFG);

    /* Clear out the LNA Baseline RAM */
    register_bulk_set(HWDEF_MEMMAP_LNA_BL_RAM_BASE, 0x00000000, (HWDEF_MEMMAP_LNA_BL_RAM_SIZE/4));

    /* Clear out the FW Baseline RAM */
    register_bulk_set(HWDEF_MEMMAP_FW_BL_RAM_BASE, 0x00000000, (HWDEF_MEMMAP_FW_BL_RAM_SIZE/4));

    /*
     * Override some registers for calibration mode.
     */

    REG_WRITE32(RCM_ADC_CLK, (RCM_ADC_CLK_CTRL
                              | REG_STUFFINT32(RCM_ADC_CLK_FREQ_SEL_DIV4,
                                               RCM_ADC_CLK_FREQ_SEL_B,
                                               RCM_ADC_CLK_FREQ_SEL)));

    /* Per Gordon Shen, 2017/2/9 */
    cdm_cdmcfg = (CDM_CDMCFG_BYPASS
                  | REG_STUFFINT32(
                       CDM_CDMCFG_PSO_PIXEL_ARRIVAL_ORDER,
                       CDM_CDMCFG_PSO_B,
                       CDM_CDMCFG_PSO)
                  | REG_STUFFINT32(
                       CDM_CDMCFG_FW_BLSO_PIXEL_ARRIVAL_ORDER,
                       CDM_CDMCFG_FW_BLSO_B,
                       CDM_CDMCFG_FW_BLSO)
                  |CDM_CDMCFG_DISBL);
    REG_WRITE32(CDM_CDMCFG, cdm_cdmcfg);

    REG_WRITE32(SCM_SCMIE, 0);

    /* Collect and discard some initial frames */
    for(itr=0; itr< test_const_params.lna_bl_skip_frames; itr++)
    {
        collect_frame();
    }


    /* LNA BL Calibration */
    for(itr=0; itr<12; itr++)
    {
        collect_frame();

        // Binary Search again
        bsflag = cal_bl_binarysearch(itr, &rx_lna_center[0], test_const_params.rx_lna_tolerance);
        if(!bsflag) 
        {
            break;
        }
    }



    /* Code to check the results of the LNA offset calibration */
    {
        pFW_BL_RAM16 = (volatile uint16_t *) HWDEF_MEMMAP_FW_BL_RAM_BASE;
        pLNA_BL_RAM16 = (volatile uint16_t *) HWDEF_MEMMAP_LNA_BL_RAM_BASE;
    }

    /* Per Gordon Shen, 2017/2/9 */  // 0x234
    cdm_cdmcfg = (REG_STUFFINT32(
                    CDM_CDMCFG_PSO_PIXEL_ARRIVAL_ORDER,
                    CDM_CDMCFG_PSO_B,
                    CDM_CDMCFG_PSO)
                  | REG_STUFFINT32(
                    CDM_CDMCFG_FW_BLSO_PIXEL_ARRIVAL_ORDER,
                    CDM_CDMCFG_FW_BLSO_B,
                    CDM_CDMCFG_FW_BLSO)
                  | REG_STUFFINT32(
                    2,
                    CDM_CDMCFG_SAMT_CDM_B,
                    CDM_CDMCFG_SAMT_CDM)
                  | CDM_CDMCFG_DISBL);
    REG_WRITE32(CDM_CDMCFG, cdm_cdmcfg);

    /* Collect a baseline frame and copy to FW_BL and evaluate*/
    {
        for(itr=0; itr<test_const_params.fw_bl_skip_frames; itr++)
        {
            collect_frame();
        }

        collect_frame();

        pIMGRAM16 = (volatile uint16_t *) HWDEF_MEMMAP_IMGRAM_BASE;
        pFW_BL_RAM16 = (volatile uint16_t *) HWDEF_MEMMAP_FW_BL_RAM_BASE;

        for (idx = 0; idx < TEST_TOTAL_PXLS; idx++) 
        {
            pFW_BL_RAM16[idx] = (-1)*pIMGRAM16[idx];
        }
    }

    /* Restore the original registers */
    REG_WRITE32(CDM_CDMCFG, orig_cdm_cdmcfg);
    REG_WRITE32(RCM_ADC_CLK, orig_rcm_adc_clk);
    REG_WRITE32(SCM_SCMIE, orig_scm_scmie);

}

#include "pkg_151A_setup_tags.h"

#ifdef FEATURE_OPSH_GET_FRAME
void test_rx_tx_open_use_image(results_struct_t *results, half_word_frame_data_t *frame1, half_word_frame_data_t *frame2)
#else
void test_rx_tx_open_use_image(results_struct_t *results)
#endif
{

    uint32_t           i;
    volatile int16_t   *pIMGRAM16;
    volatile uint16_t  *pFWBL16;
    volatile uint32_t  *pDMRAM;
    int16_t            val1;
    int16_t            min, max;
    int32_t            ix;
    int32_t            r;
    int32_t            c;
    int32_t            sum, rng_sum, avg, avg2, max32;
    uint32_t           orig_cdm_cdmcfg;
    uint32_t           orig_scm_itglen;
    uint32_t           lessTx=0;

#ifdef FEATURE_OPSH_GET_FRAME
    frame1->frame_cols = TEST_QNTY_RX;
    frame1->frame_rows = TEST_PXLS_PER_RX;
    frame2->frame_cols = TEST_QNTY_RX;
    frame2->frame_rows = TEST_PXLS_PER_RX;
#endif

    /* Clear out the memories */
    {
        /* Clear out the Image RAM */
        register_bulk_set(HWDEF_MEMMAP_IMGRAM_BASE, 0x00000000, (HWDEF_MEMMAP_IMGRAM_SIZE/4));
       
        /* Clear out Drive Matrix RAM */
        register_bulk_set(HWDEF_MEMMAP_DMRAM_BASE, 0x00000000, (HWDEF_MEMMAP_DMRAM_SIZE/4));

        /* Clear out dode division multiplexing RAM */
        register_bulk_set(HWDEF_MEMMAP_CDMRAM_BASE, 0x00000000, (HWDEF_MEMMAP_CDMRAM_SIZE/4));

        /* Clear out the LNA Baseline RAM */
        register_bulk_set(HWDEF_MEMMAP_LNA_BL_RAM_BASE, 0x00000000, (HWDEF_MEMMAP_LNA_BL_RAM_SIZE/4));

        /* Clear out the FW Baseline RAM */
        register_bulk_set(HWDEF_MEMMAP_FW_BL_RAM_BASE, 0x00000000, (HWDEF_MEMMAP_FW_BL_RAM_SIZE/4));
    }

    /* Load the setups using tag memory */
    i = fpscan_tagsparse(&fpscan_tagdata[0], sizeof(fpscan_tagdata), FALSE,
        &results->parameter[TEST_PARAMETER_TAG_WRITE_TAGS_USED],
        &results->parameter[TEST_PARAMETER_TAG_WRITE_TAGS_SKIP]);

    if (0 != i)
    {
        results->parameter[TEST_PARAMTER_RUN_FAIL_CODE_LOC] = INTERNAL_ERROR_CODE_FPSCAN_TAG_FAIL;
        SET_STATUS_BIT(TEST_STATUS_FAIL_INTERNAL); 
    }

    /*
     * Point the buffer controller to point to our cache-aligned
     *  location on the stack.  I guess it doesn't really need
     *  to be cache-aligned, but it's good form.
     */
    dummyptr = (uint8_t *) HWDEF_DCACHE_ALIGN(&(dummybuf[0]));
    REG_WRITE32(BC_CB_STRT_ADDR,
                REG_STUFFINT32(dummyptr,
                               BC_CB_STRT_ADDR_LOC_B,
                               BC_CB_STRT_ADDR_LOC));
    REG_WRITE32(BC_CB_END_ADDR,
                REG_STUFFINT32(dummyptr + HWDEF_DCACHE_LINESIZE,
                               BC_CB_END_ADDR_LOC_B,
                               BC_CB_END_ADDR_LOC));

    /* write 0x1 to release afe from reset. If this is not done,
      analog front end will be held in reset. */
    REG_WRITE32(WOE_SPARE, 0x00000001);

	// Set up the clocks for fingerprint scanning.
	// Assumes fast clock and slow clock are already set.
	// Initialize SCM registers to allow a default fingerprint
	// to be generated without calibrated values.
    REG_WRITE32(RCM_AFE_CLOCK,
                (RCM_AFE_CLOCK_CTRL
                 | REG_STUFFINT32(RCM_AFE_CLOCK_FREQ_SEL_DIV24,
                                  RCM_AFE_CLOCK_FREQ_SEL_B,
                                  RCM_AFE_CLOCK_FREQ_SEL)));
    REG_WRITE32(RCM_ADC_CLK,
                (RCM_ADC_CLK_CTRL
                 | REG_STUFFINT32(RCM_ADC_CLK_FREQ_SEL_DIV4,
                                  RCM_ADC_CLK_FREQ_SEL_B,
                                  RCM_ADC_CLK_FREQ_SEL)));
    REG_WRITE32(RCM_CDM_CLOCK,
                (RCM_CDM_CLOCK_CTRL
                 | REG_STUFFINT32(RCM_CDM_CLOCK_FREQ_SEL_DIV1,
                                  RCM_CDM_CLOCK_FREQ_SEL_B,
                                  RCM_CDM_CLOCK_FREQ_SEL)));

    REG_WRITE32(RCM_OTHER_CLK, RCM_OTHER_CLK_GPIO_CTRL);

    orig_scm_itglen = REG_READ32(SCM_ITGLEN); //store for later use
    orig_cdm_cdmcfg = REG_READ32(CDM_CDMCFG); //store for later use

    pIMGRAM16 = (volatile int16_t *) HWDEF_MEMMAP_IMGRAM_BASE;
    pFWBL16 = (volatile uint16_t *) HWDEF_MEMMAP_FW_BL_RAM_BASE;
    pDMRAM = (volatile uint32_t *) HWDEF_MEMMAP_DMRAM_BASE;

    /* Write the Drive Matrix Memory to 0 */

   pDMRAM[0] = 0x55000000; //enable first and last 8 TX's
    for(i=1; i<784; i++)
    {
    	
        if (!(i%7))
        		pDMRAM[i] = 0x55000000; //was 0's  //enable Tx11
        else if (!((i+6)%7))
        		pDMRAM[i] = 0x00000055; //was 0's  //enable Tx11
        else if (!((i+2)%7))
        		pDMRAM[i] = 0x55000000; //was 0's  //enable Tx11
        else if (!((i+1)%7))
        		pDMRAM[i] = 0x00000055; //was 0's  //enable Tx11
        else
            pDMRAM[i] = 0x00000000; //was 0's

    }

    //Calibration 
    cal_bl(results);  //TROY: cal was enabled for new Rx opens/shorts test



    REG_WRITE32(CDM_CDMCFG, 0x0000002c);  // FW_BLSO set, disable BL subtraction, and disable CDM decode for the Rx opens/shorts test...

    /* Perform Rx Open Test, defect/open in the pixel area */
    if (0 < (ctrl_code & CTRL_CODE_INC_RX_OPEN))
    {

        /* Perform Rx Open Test, defect/open in the pixel area */
        if (0 < (ctrl_code & CTRL_CODE_INC_RX_OPEN))
        {
            results->parameter[TEST_PARAMETER_RUN_CODE] |= RUN_CODE_INC_RX_OPEN;
        }

        /* Collect Frame data for Rx testing */
        collect_frame();

        for(i=0; i<TEST_TOTAL_PXLS; i++)
        {
            pIMGRAM16[i] += 2048;

            pFWBL16[i] = pIMGRAM16[i];
#ifdef FEATURE_OPSH_GET_FRAME
            frame1->data[i] = pIMGRAM16[i];
#endif           
        }




#if 0

// Calculate the parameters for each rx 
for(c=0; c<TEST_QNTY_RX; c++)
{
	sum = 0;
	min=10000;
	max=0;
	for(r=0; r<TEST_PXLS_PER_RX-lessTx; r++) //adjusted for 112C1
	{
		if (pIMGRAM16[r*TEST_QNTY_RX+c] > max) 
			max = pIMGRAM16[r*TEST_QNTY_RX+c];
		if (pIMGRAM16[r*TEST_QNTY_RX+c] < min) 
			min = pIMGRAM16[r*TEST_QNTY_RX+c];
	
		sum += pIMGRAM16[r*TEST_QNTY_RX+c];
	}
				
	results->rx_im_delta[c] = sum/TEST_PXLS_PER_RX;
	results->rx_im_rng[c] = max - min; 
}  

#else

        // Calculate the parameters for each rx 
        for(c=0; c<TEST_QNTY_RX; c++)
        {
            sum = 0;
            min=10000;
            max=0;
            for(r=0; r<TEST_PXLS_PER_RX-lessTx; r++) //adjusted for 112C1
            {
                if (pIMGRAM16[r*TEST_QNTY_RX+c] > max) 
                    max = pIMGRAM16[r*TEST_QNTY_RX+c];
                if (pIMGRAM16[r*TEST_QNTY_RX+c] < min) 
                    min = pIMGRAM16[r*TEST_QNTY_RX+c];
            
                sum += pIMGRAM16[r*TEST_QNTY_RX+c];
            }
						
            results->rx_im_delta[c] = sum/TEST_PXLS_PER_RX;
            results->rx_im_rng[c] = max - min; 
        }   
        
        
        sum = 0;
        rng_sum = 0;
        for(c=0; c<TEST_QNTY_RX; c++)
        {
            sum += results->rx_im_delta[c];
            rng_sum += results->rx_im_rng[c];
        }
        avg = sum/TEST_QNTY_RX;
        avg2 = rng_sum/TEST_QNTY_RX;
        
        for(c=0; c<TEST_QNTY_RX; c++)
        {
            if (avg > results->rx_im_delta[c])
                results->rx_im_delta[c] = avg - results->rx_im_delta[c];
            else
                results->rx_im_delta[c] = results->rx_im_delta[c] - avg;
        }
        for(c=0; c<TEST_QNTY_RX; c++)
        {
            if (avg2 > results->rx_im_rng[c])
                results->rx_im_rng[c] = avg2 - results->rx_im_rng[c];
            else
                results->rx_im_rng[c] = results->rx_im_rng[c] - avg2;
        }
        
//return(0);




        results->parameter[TEST_PARAMETER_MAX_RX_DELTA_FRM_AVG]=0;
        for(c=0; c<TEST_QNTY_RX; c++)
        {
            sum = 1;
            for(i=0; i<test_const_params.n_rx_open_skip; i++) // skip 2 edges
            {
                if (c == test_const_params.rx_open_skip[i])
                {
                    sum = 0;
                    break;
                }
            }

            if (sum)
            {
                check_value_add_fail_count(results->rx_im_delta[c],  TEST_STATUS_FAIL_RX_OPEN_IMG,  
                    &test_params[limit_set].rx_open_max_delta_frm_avg,  results, TEST_PARAMETER_QNTY_RX_OPEN_MAX_DELTA_FAILS);
#if 0
				if( c == 0x48 )
				{
					results->parameter[TEST_PARAMETER_QNTY_RX_OPEN_MAX_DELTA_FAILS ] = test_params[limit_set].rx_open_max_delta_frm_avg.max;
				}
#endif
                if ((int16_t)results->parameter[TEST_PARAMETER_MAX_RX_DELTA_FRM_AVG] < results->rx_im_delta[c])
                {
                    results->parameter[TEST_PARAMETER_MAX_RX_DELTA_FRM_AVG] = results->rx_im_delta[c];
                    results->parameter[TEST_PARAMETER_MAX_RX_DELTA_FRM_AVG_IDX] = c;
                }
            }
        }
        
        results->parameter[TEST_PARAMETER_MAX_RX_RNG]=0;
        for(c=0; c<TEST_QNTY_RX; c++)
        {
            sum = 1;
            for(i=0; i<test_const_params.n_rx_open_skip; i++)
            {
                if (c == test_const_params.rx_open_skip[i])
                {
                    sum = 0;
                    break;
                }
            }

            if (sum)
            {
                check_value_add_fail_count(results->rx_im_rng[c],  TEST_STATUS_FAIL_RX_OPEN_IMG,  
                    &test_params[limit_set].rx_open_max_range,  results, TEST_PARAMETER_QNTY_RX_OPEN_MAX_RANGE_FAILS);

                if ((int16_t)results->parameter[TEST_PARAMETER_MAX_RX_RNG] < results->rx_im_rng[c])
                {
                    results->parameter[TEST_PARAMETER_MAX_RX_RNG] = results->rx_im_rng[c];
                    results->parameter[TEST_PARAMETER_MAX_RX_RNG_IDX] = c;
                }
            }
        }
#endif

    }

    // Tx testing: 
    //   The basis of the Tx open testing is to use the LNA's in a single ended mode to generate signals (or lack of).  To reduce the effect of offset differences, the delta of 
    //   Tx's clocking and no clocking is used as the frame data.  The average is calculated for each Tx seperately, then the absolute delta from average.  The max value is found 
    //   and judged against the max limit.  
    if (0 < (ctrl_code & CTRL_CODE_INC_TX_OPEN))
    {    

    	        /* Clear out the LNA Baseline RAM */
        register_bulk_set(HWDEF_MEMMAP_LNA_BL_RAM_BASE, 0x00000000, (HWDEF_MEMMAP_LNA_BL_RAM_SIZE/4));
    	
        /* Load the setups using tag memory */
        i = fpscan_tagsparse(&fpscan_tagdata[0], sizeof(fpscan_tagdata), FALSE,
            &results->parameter[TEST_PARAMETER_TAG_WRITE_TAGS_USED],
            &results->parameter[TEST_PARAMETER_TAG_WRITE_TAGS_SKIP]);

        results->parameter[TEST_PARAMETER_RUN_CODE] |= RUN_CODE_INC_TX_OPEN;

        REG_WRITE32(SCM_ITGLEN, orig_scm_itglen);
        REG_WRITE32(CDM_CDMCFG, orig_cdm_cdmcfg); 

        collect_frame();

        for(i=0; i<TEST_TOTAL_PXLS; i++)
        {
            val1 = pIMGRAM16[i];
            val1 /= 8;
            val1 = val1 + 4096;
            pIMGRAM16[i] = val1;
            pFWBL16[i] = val1;     //save in the FW_BL RAM to perform the Tx clocking/not_clocking delta
        }

        for(i=0; i<784; i++)
        {
            pDMRAM[i]   = 0x00000000; //debug only: should change this to a register_bulk_set
        }

        // Clear out the Image RAM 
        register_bulk_set(HWDEF_MEMMAP_IMGRAM_BASE, 0x00000000, (HWDEF_MEMMAP_IMGRAM_SIZE/4));

        collect_frame();

        for(i=0; i<TEST_TOTAL_PXLS; i++)
        {
            val1 = pIMGRAM16[i];
            val1 /= 8;
            val1 = val1 + 4096;
            pIMGRAM16[i] = val1;
            if (pIMGRAM16[i] > pFWBL16[i])
                pIMGRAM16[i] = pIMGRAM16[i] - pFWBL16[i];
            else
                pIMGRAM16[i] = pFWBL16[i] - pIMGRAM16[i];

#ifdef FEATURE_OPSH_GET_FRAME
            frame2->data[i] = pIMGRAM16[i];
#endif
        }

        sum = 0;
        i = 0;
        for(r=0; r<TEST_PXLS_PER_RX-lessTx; r++) //adjusted for 112C1
        {
            min = 10000;
            max = 0;
            results->tx_im_gnd_sum[r] = 0;
            for(c=0; c<TEST_QNTY_RX; c++)
            {
                results->tx_im_gnd_sum[r] += pIMGRAM16[i];
                if (max < pIMGRAM16[i])
                {
                    max = pIMGRAM16[i];
                }
                if (min > pIMGRAM16[i])
                {
                    min = pIMGRAM16[i];
                }

                i++; //next pixel
            }
            results->tx_im_gnd_rng[r] = max - min;
            results->tx_im_gnd_sum[r] = results->tx_im_gnd_sum[r]/TEST_QNTY_RX;
            sum += results->tx_im_gnd_sum[r];
        }
        avg = sum/(TEST_PXLS_PER_RX-lessTx); //adjusted for 150A

        //calculate overall delta from avg data, and the tx to tx delta of range data
        for(r=0; r<TEST_PXLS_PER_RX; r++) //adjusted for 150A
        {
            if (avg > results->tx_im_gnd_sum[r])
                results->tx_im_gnd_sum[r] = avg - results->tx_im_gnd_sum[r];
            else
                results->tx_im_gnd_sum[r] = results->tx_im_gnd_sum[r] - avg;
        }

        //find the max and count the # of fails for the inside 108 Tx's
        max = 0;
        results->parameter[TEST_PARAMETER_QNTY_TX_OPEN_FAILS] = 0;
        for(r=2; r<TEST_PXLS_PER_RX-2; r++) //adjusted for 150A
        {
            if (max < results->tx_im_gnd_sum[r])
            {
                max = results->tx_im_gnd_sum[r];
                ix = r;
            }

            if (results->tx_im_gnd_sum[r] > test_params[limit_set].tx_open.max)
                results->parameter[TEST_PARAMETER_QNTY_TX_OPEN_FAILS]++;
        }
        results->parameter[TEST_PARAMETER_MAX_TX_DELTA_FRM_AVG] = max;
        results->parameter[TEST_PARAMETER_MAX_TX_DELTA_FRM_AVG_IDX] = ix;

        if (max > test_params[limit_set].tx_open.max)
            SET_STATUS_BIT(TEST_STATUS_FAIL_TX_OPEN);

        //need seperate limit for outside 4 Tx's (2 on each end)
        //  find the max and count of the # of fails for the outside 4 Tx's (2 on each end)
        max = 0;
        for(r=0; r<2; r++) //adjusted for 150A
        {
            if (max < results->tx_im_gnd_sum[r])
            {
                max = results->tx_im_gnd_sum[r];
                ix = r;
            }

            if (results->tx_im_gnd_sum[r] > test_params[limit_set].tx_open_ends.max)
                results->parameter[TEST_PARAMETER_QNTY_TX_OPEN_FAILS]++;
        }
        for(r=TEST_PXLS_PER_RX-2; r<TEST_PXLS_PER_RX; r++) //adjusted for 150A
        {
            if (max < results->tx_im_gnd_sum[r])
            {
                max = results->tx_im_gnd_sum[r];
                ix = r;
            }

            if (results->tx_im_gnd_sum[r] > test_params[limit_set].tx_open_ends.max)
                results->parameter[TEST_PARAMETER_QNTY_TX_OPEN_FAILS]++;
        }
        results->parameter[TEST_PARAMETER_MAX_TX_DELTA_ENDS_FRM_AVG] = max;
        results->parameter[TEST_PARAMETER_MAX_TX_DELTA_ENDS_FRM_AVG_IDX] = ix;

        if (max > test_params[limit_set].tx_open_ends.max)
            SET_STATUS_BIT(TEST_STATUS_FAIL_TX_OPEN);


        //calculate the tx to tx delta using the range data, 
        // this is the best screen for defects in the array area 
        max = 0;
        sum = 0;
        for(r=0; r<TEST_PXLS_PER_RX-1; r++) //adjusted for 150A
        {
            if (results->tx_im_gnd_rng[r] > results->tx_im_gnd_rng[r+1])
                results->tx_im_gnd_rng[r] = results->tx_im_gnd_rng[r] - results->tx_im_gnd_rng[r+1];
            else
                results->tx_im_gnd_rng[r] = results->tx_im_gnd_rng[r+1] - results->tx_im_gnd_rng[r];

            if (max < results->tx_im_gnd_rng[r])
            {
                max = results->tx_im_gnd_rng[r];
                ix = r;
            }

            if (results->tx_im_gnd_rng[r] > test_params[limit_set].tx_open_rng.max)
                    sum++;
        }

        results->parameter[TEST_PARAMETER_MAX_TX_RNG_DELTA] = max;
        results->parameter[TEST_PARAMETER_MAX_TX_RNG_DELTA_IDX] = ix;
        
        if (max > test_params[limit_set].tx_open_rng.max)
            SET_STATUS_BIT(TEST_STATUS_FAIL_TX_OPEN);
            
        //when doing the tx to tx delta, you get two fail values for every one Tx failure, if the open Tx's are not consecutive
        if (sum > 1)
            results->parameter[TEST_PARAMETER_QNTY_TX_OPEN_FAILS] += sum/2;
        else
            results->parameter[TEST_PARAMETER_QNTY_TX_OPEN_FAILS] += sum;


    } //end of Tx testing


}


#ifdef FEATURE_OPSH_GET_FRAME
void run_test(results_struct_t *results, half_word_frame_data_t *frame1, half_word_frame_data_t *frame2)
#else
void run_test(results_struct_t *results)
#endif
{
    uint32_t           clck_start;
    uint32_t           i;
    uint8_t            *x;
    uint32_t           rd_data;
    uint32_t           orig_bg_cfg, orig_sosc_cfg;

    /* Code to setup the part on the ATE, not needed in firmware patch  */
    //wr_word(WOE_HV_DPOR, WOE_HV_DPOR_KEY_VALUE);
    //wr_word(WOE_STARTUP_DELAY,0x00020010);   // cpu fast clock

    //for (i = 0; i<10000; i++) asm("nop");

    //REG_WRITE32(WOE_DVREG, 0x000000f);


    //REG_WRITE32(SCM_LNA_AVREG0, 0x0000000f);
    //REG_WRITE32(SCM_LNA_AVREG1, 0x0000000f);

    //REG_WRITE32(SCM_BG_BIAS_TRIM, (
    //                    REG_STUFFINT32(0x10,
    //                                   SCM_BG_BIAS_TRIM_IREF_TRIM_B,
    //                                   SCM_BG_BIAS_TRIM_IREF_TRIM)));

    clck_start = REG_READ32(WOE_TIMER_COUNT);    //Read Clock Register

    /* Clear the results struture */
    x = (uint8_t *)&results->status[0];
    for(i=0; i<sizeof(results_struct_t); i++)
    {
        x[i] = 0;
    }

    /* Initialize some of the parameters */
    {
        results->parameter[TEST_PARAMETER_PATCH_TYPE] = TEST_PATCH_TYPE;
        results->parameter[TEST_PARAMETER_PATCH_VERSION] = TEST_PATCH_ID_INFO;
        results->parameter[TEST_PARAMETER_LIMIT_VERSION] = TEST_PATCH_LIMIT_INFO;

        rd_data = REG_READ32(HWDEF_FIB_SERIALNO);
        results->parameter[TEST_PARAMETER_SER_NUM_A] = rd_data;

        rd_data = REG_READ32(HWDEF_FIB_SERIALNO + sizeof(uint32_t));
        results->parameter[TEST_PARAMETER_SER_NUM_B] = rd_data & 0xffff;

        results->parameter[TEST_PARAMETER_LIMIT_SET] = limit_set;
        limit_or_mask = (1 << limit_set);

        ctrl_code = test_params[limit_set].ctrl_code;
        results->parameter[TEST_PARAMETER_CTRL_CODE ] = ctrl_code;
    }

    /* Check the Mask ID Registers */
    if (0 < (ctrl_code & CTRL_CODE_INC_CHECK_MASK_ID))
    {
        results->parameter[TEST_PARAMETER_RUN_CODE] |= RUN_CODE_INC_CHECK_MASK_ID;

    REG_WRITE32(SCM_MASKID_HI, 0xffffffff); //PRE-CHARGE the data bus
        rd_data = REG_READ32(SCM_MASKID_LO);
        results->parameter[TEST_PARAMETER_MASK_ID_LO] = rd_data;
		//results->parameter[TEST_PARAMETER_MASK_ID_LO] = 0x55555555;
        if (rd_data != test_const_params.maskid[0])
        {
            SET_STATUS_BIT(TEST_STATUS_FAIL_MASK_ID);
        }

    REG_WRITE32(SCM_MASKID_HI, 0xffffffff); //PRE-CHARGE the data bus
        rd_data = REG_READ32(SCM_MASKID_HI);
        results->parameter[TEST_PARAMETER_MASK_ID_HI] = rd_data;
        if (rd_data != test_const_params.maskid[1])
        {
            SET_STATUS_BIT(TEST_STATUS_FAIL_MASK_ID);
        }
    }

    /* Initialize some hardware registers */
    {
        /* Check the fast clock FIB */
        results->parameter[TEST_PARAMETER_FIB_CHECKS] |= FIB_CHECK_FAST_CLOCK_OR_MASK;
        rd_data = NVM_FIB_READ32(HWDEF_FIB_FASTCLK);
        if (!HWINIT_FIB_ISVALID(rd_data)) 
        { /* The value is not valid -- mark as error 
              and program default value */

            /* REG_WRITE32(WOE_HS_OSC_TRIM, (
                        REG_STUFFINT32(2,
                                       WOE_HS_OSC_TRIM_FAST_MODE_B,
                                       WOE_HS_OSC_TRIM_FAST_MODE)
                        | REG_STUFFINT32(96,
                                         WOE_HS_OSC_TRIM_INC_VREF_B,
                                         WOE_HS_OSC_TRIM_INC_VREF))); */

            REG_WRITE32(WOE_HS_OSC_TRIM,0x000013ab);
        }
        else
        {
            /* If the value in the FIB is valid then assume that
                the bootloader has already programmed these. 
                This may need to be modified for ATE */
            results->parameter[TEST_PARAMETER_FIB_CHECKS_OK] |= FIB_CHECK_FAST_CLOCK_OR_MASK;
        }

        /* Check HWDEF_FIB_VTRIM */
        results->parameter[TEST_PARAMETER_FIB_CHECKS] |= FIB_CHECK_VTRIM_OR_MASK;
        rd_data = NVM_FIB_READ32(HWDEF_FIB_VTRIM);
        if (HWINIT_FIB_ISVALID(rd_data)) 
        {
            REG_WRITE32(WOE_DVREG, HWINIT_FIB_VAL(rd_data));
            results->parameter[TEST_PARAMETER_FIB_CHECKS_OK] |= FIB_CHECK_VTRIM_OR_MASK;
        }
        else 
        {
            /*
             * WOE_DVREG set to middle range,
             * per Gordon Shen 2015sep23 3:12PM PDT
             */
            REG_WRITE32(WOE_DVREG, 0x000000f);
        }

        /* Check HWDEF_FIB_BGTRIM */
        results->parameter[TEST_PARAMETER_FIB_CHECKS] |= FIB_CHECK_BGTRIM_OR_MASK;
        rd_data = NVM_FIB_READ32(HWDEF_FIB_BGTRIM);
        if (HWINIT_FIB_ISVALID(rd_data)) 
        {
            REG_WRITE32(SCM_BG_BIAS_TRIM, HWINIT_FIB_VAL(rd_data));
            results->parameter[TEST_PARAMETER_FIB_CHECKS_OK] |= FIB_CHECK_BGTRIM_OR_MASK;
            orig_bg_cfg = HWINIT_FIB_VAL(rd_data);
        }
        else 
        {
            /* = 0x00002000 */
            REG_WRITE32(SCM_BG_BIAS_TRIM, (
                        REG_STUFFINT32(0x10,
                                       SCM_BG_BIAS_TRIM_IREF_TRIM_B,
                                       SCM_BG_BIAS_TRIM_IREF_TRIM)));
            orig_bg_cfg = REG_STUFFINT32(0x10,
                                       SCM_BG_BIAS_TRIM_IREF_TRIM_B,
                                       SCM_BG_BIAS_TRIM_IREF_TRIM);
        }

        /* Check HWDEF_FIB_SLOWCLK */
        rd_data = NVM_FIB_READ32(HWDEF_FIB_SLOWCLK);
        results->parameter[TEST_PARAMETER_FIB_CHECKS] |= FIB_CHECK_SLOWCLK_OR_MASK;
        if (HWINIT_FIB_ISVALID(rd_data)) 
        {
            REG_WRITE32(SCM_SOSC_BIAS_TRIM, HWINIT_FIB_VAL(rd_data));
            orig_sosc_cfg = HWINIT_FIB_VAL(rd_data);
            results->parameter[TEST_PARAMETER_FIB_CHECKS_OK] |= FIB_CHECK_SLOWCLK_OR_MASK;
        }
        else
        {
            /* Is there a default value? */
            orig_sosc_cfg = 0x00000030;
        }

        /* Set some default values --> may be set later */
        REG_WRITE32(SCM_LNA_AVREG0, 0x0000000f);
        REG_WRITE32(SCM_LNA_AVREG1, 0x0000000f);

        if (0 < (ctrl_code & CTRL_CODE_INC_VERIFY_FIB_VALS))
        {
            results->parameter[TEST_PARAMETER_RUN_CODE] |= RUN_CODE_INC_VERIFY_FIB_VALS;
            if (results->parameter[TEST_PARAMETER_FIB_CHECKS] != results->parameter[TEST_PARAMETER_FIB_CHECKS_OK])
            {
                SET_STATUS_BIT(TEST_STATUS_FAIL_FIB_VAL_CHCK);
            }
        }
    }

    /* write 0x1 to release afe from reset. If this is not done,
      analog front end will be held in reset. */
    REG_WRITE32(WOE_SPARE, 0x00000001);

    /* Clear out the memories */
    if (0 < (ctrl_code & CTRL_CODE_CLEAR_MEM_BUFFERS))
    {
        results->parameter[TEST_PARAMETER_RUN_CODE] |= RUN_CODE_CLEAR_MEM_BUFFERS;

        /* Clear out the Image RAM */
        register_bulk_set(HWDEF_MEMMAP_IMGRAM_BASE, 0x00000000, (HWDEF_MEMMAP_IMGRAM_SIZE/4));

        /* Clear out Drive Matrix RAM */
        register_bulk_set(HWDEF_MEMMAP_DMRAM_BASE, 0x00000000, (HWDEF_MEMMAP_DMRAM_SIZE/4));

        /* Clear out dode division multiplexing RAM */
        register_bulk_set(HWDEF_MEMMAP_CDMRAM_BASE, 0x00000000, (HWDEF_MEMMAP_CDMRAM_SIZE/4));

        /* Clear out the LNA Baseline RAM */
        register_bulk_set(HWDEF_MEMMAP_LNA_BL_RAM_BASE, 0x00000000, (HWDEF_MEMMAP_LNA_BL_RAM_SIZE/4));

        /* Clear out the FW Baseline RAM */
        register_bulk_set(HWDEF_MEMMAP_FW_BL_RAM_BASE, 0x00000000, (HWDEF_MEMMAP_FW_BL_RAM_SIZE/4));
    }

    //ERIC 09/01/17: Set Test limit values
    results->limits.tx_open = test_params[limit_set].tx_open;
    results->limits.tx_open_ends = test_params[limit_set].tx_open_ends;
    results->limits.tx_open_rng = test_params[limit_set].tx_open_rng;
    results->limits.rx_open_max_delta_frm_avg = test_params[limit_set].rx_open_max_delta_frm_avg;
    results->limits.rx_open_max_range = test_params[limit_set].rx_open_max_range;
    results->limits.rx_open_max_tx0 = test_params[limit_set].rx_open_max_tx0;

    // return these HW register settings to their original settings
    REG_WRITE32(SCM_BG_BIAS_TRIM, orig_bg_cfg);
    REG_WRITE32(SCM_SOSC_BIAS_TRIM, orig_sosc_cfg);

#if 0
    /* BIST TX Shorts */
    if (0 < (ctrl_code & CTRL_CODE_INC_BIST_TX_SHORT))
    {
        results->parameter[TEST_PARAMETER_RUN_CODE] |= RUN_CODE_INC_BIST_TX_SHORT;
        test_tx_shorts(results);

    }

    /* BIST RX Shorts */
    if (0 < (ctrl_code & CTRL_CODE_INC_BIST_RX_SHORT))
    {
        results->parameter[TEST_PARAMETER_RUN_CODE] |= RUN_CODE_INC_BIST_RX_SHORT;
        test_rx_shorts(results);

    }
#endif

    /* Open / Short test using SCM Image */
#ifdef FEATURE_OPSH_GET_FRAME
    test_rx_tx_open_use_image(results, frame1, frame2);
#else
    test_rx_tx_open_use_image(results);
#endif



exit:

    /* Set the main fail bit */ 
	{
	    uint8_t status_num;
	    bool_t status_fail = 0;
		
		/* Set overall pass or fail bit - fail if any other status bit is already set */
		status_fail = 0;
		for (status_num=0; status_num<STATUS_COUNT; status_num++)
		{
			if (results->status[status_num]) status_fail = 1;
		}
		if (status_fail) SET_STATUS_BIT(TEST_STATUS_FAIL);
		else SET_STATUS_BIT(TEST_STATUS_PASS);

        build_bin_fail_list(results);
	}

	i = REG_READ32(WOE_TIMER_COUNT);    //Read Clock Register
	if (i < clck_start)
	{
		rd_data = (0xffffffff - clck_start) + i;
	}
	else
	{
		rd_data = i - clck_start;
	}
    results->parameter[TEST_PARAMETER_10USEC_CYLCES] = rd_data;

    

}

#else
/* -------------------------------------------------------------------------- */
/* Static functions */

/* ************************************************************************* */
/* LNA BL Calibration Functions                                              */
/* ************************************************************************* */

void collect_frame(void)
{
    uint32_t   rd_data;
    uint32_t   i;
    uint32_t   j;
    uint32_t   addr;
    uint32_t   val;
 
    addr = SCM_LNA_CFG0;
    //val = 0x00002020;
    val = 0x00004020;
    for(i=0; i<80; i++, addr +=4)
    {
     REG_VARWRITE32(addr, val);	// force_NI_VCM
    }

    REG_WRITE32(SCM_SCMIE, 0);
    REG_WRITE32(SCM_SCMCMD, SCM_SCMCMD_SCMSRST);
    for (j = 0; j<200; j++) asm("nop");
    REG_WRITE32(SCM_SCMCMD, 0);

#if 1
    /* Start Frame Collection */
    REG_WRITE32(SCM_SCMCMD, REG_STUFFINT32(SCM_SCMCMD_RUNCMD_RUN_SCAN,
                                           SCM_SCMCMD_RUNCMD_B,
                                           SCM_SCMCMD_RUNCMD));
#endif


#if 1
for (j = 0; j<200000000; j++) asm("nop");
for (j = 0; j<20000000; j++) asm("nop");
#else
    // Wait for SCM scan end
    do {
        rd_data = REG_READ32(SCM_SCMSTAT);
    } while (REG_BITISOFF_B(rd_data, SCM_SCMSTAT_SE_B));
   
#if 0

    REG_WRITE32(SCM_SCMSTAT, rd_data);
    // Wait till CDM busy deasserted
    do {
        rd_data = REG_READ32(CDM_CDMSTAT);
    } while (REG_BITISON_B(rd_data, CDM_CDMSTAT_CDMBUSY_B));
#endif 
#endif 

}

#if 0
static unsigned int
cal_bl_getj(unsigned int i)
{
    unsigned int j;
    if(i<2016)
           j = (i%14)*144 + i/14;
    else if(i<4032)
           j = 2016 + ((i-2016)%14)*144 + (i-2016)/14;
    else if(i<6048)
           j = 4032 + ((i-4032)%14)*144 + (i-4032)/14;
    else
           j = 6048 + ((i-6048)%14)*144 + (i-6048)/14;

    return j;
}
static int
cal_bl_getcenter(unsigned int i,int32_t *target)
{
     int center;
     if(i<2016)  // sub_frame 0
       {
            switch(i%14){
                case 0: center = target[0]; break;
                case 1: center = target[1]; break;
                case 2: center = target[8]; break;
                case 3: center = target[9]; break;
                case 4: center = target[16]; break;
                case 5: center = target[17]; break;
                case 6: center = target[24]; break;
                case 7: center = target[25]; break;
                case 8: center = target[32]; break;
                case 9: center = target[33]; break;
                case 10: center = target[40]; break;
                case 11: center = target[41]; break;
                case 12: center = target[48]; break;
                case 13: center = target[49]; break;
            }
       }
       else if(i<4032)  // sub_frame 1
       {
            switch(i%14){
                case 0: center = target[2]; break;
                case 1: center = target[3]; break;
                case 2: center = target[10]; break;
                case 3: center = target[11]; break;
                case 4: center = target[18]; break;
                case 5: center = target[19]; break;
                case 6: center = target[26]; break;
                case 7: center = target[27]; break;
                case 8: center = target[34]; break;
                case 9: center = target[35]; break;
                case 10: center = target[42]; break;
                case 11: center = target[43]; break;
                case 12: center = target[50]; break;
                case 13: center = target[51]; break;
            }
       }
       else if(i<6048)   // sub_frame 2
       {
            switch(i%14){
                case 0: center = target[4]; break;
                case 1: center = target[5]; break;
                case 2: center = target[12]; break;
                case 3: center = target[13]; break;
                case 4: center = target[20]; break;
                case 5: center = target[21]; break;
                case 6: center = target[28]; break;
                case 7: center = target[29]; break;
                case 8: center = target[36]; break;
                case 9: center = target[37]; break;
                case 10: center = target[44]; break;
                case 11: center = target[45]; break;
                case 12: center = target[52]; break;
                case 13: center = target[53]; break;
            }
       }
       else   // sub_frame 3
       {
            switch(i%14){
                case 0: center = target[6]; break;
                case 1: center = target[7]; break;
                case 2: center = target[14]; break;
                case 3: center = target[15]; break;
                case 4: center = target[22]; break;
                case 5: center = target[23]; break;
                case 6: center = target[30]; break;
                case 7: center = target[31]; break;
                case 8: center = target[38]; break;
                case 9: center = target[39]; break;
                case 10: center = target[46]; break;
                case 11: center = target[47]; break;
                case 12: center = target[54]; break;
                case 13: center = target[55]; break;
            }
       }
       return center;
}


static bool_t
cal_bl_binarysearch(int itr, int32_t *target, int tolerance)
{
    //*****************************************//
    // Need to replace the hardcoded constant in following statements:
    //        tmp >= center+tolerance*4/5 and
    //        pFW_BL_RAM16[j] = itr>4? 1:0
    //*****************************************//

    unsigned int i,j;
    short tmp;
    short temp;
    short FW_tmp;
    int center;
    bool_t flag;
    volatile uint32_t *pIMGRAM;
    volatile uint16_t *pIMGRAM16;
    volatile uint32_t *pLNA_BL_RAM;
    volatile uint16_t *pLNA_BL_RAM16;
    volatile uint16_t *pFW_BL_RAM16;

    flag = FALSE;
    pIMGRAM = (volatile uint32_t *) HWDEF_MEMMAP_IMGRAM_BASE;
    pIMGRAM16 = (volatile uint16_t *) HWDEF_MEMMAP_IMGRAM_BASE;
    pLNA_BL_RAM = (volatile uint32_t *) HWDEF_MEMMAP_LNA_BL_RAM_BASE;
    pLNA_BL_RAM16 = (volatile uint16_t *) HWDEF_MEMMAP_LNA_BL_RAM_BASE;
    pFW_BL_RAM16 = (volatile uint16_t *) HWDEF_MEMMAP_FW_BL_RAM_BASE;

    for(i=0;i<8064;i++)
    {
           j = cal_bl_getj(i);
           tmp = pIMGRAM16[i];
           temp = pLNA_BL_RAM16[j];
           FW_tmp = pFW_BL_RAM16[j];
           center = cal_bl_getcenter(i,target);
           if(temp<0)
           {
                  temp = -1*(temp & 0x0fff);
           }
           if(tmp <= center-tolerance)      // Imgram data is below the target range
           {
                  temp += 2048>>(itr);
                  flag = 1;
           }
           else if(tmp >= center+tolerance)    // Imgram data is above the target range
           {
                  temp -= 2048>>(itr);
                  flag = 1;
           }
           else if(tmp >= center+tolerance*4/5)   // Imgram data is inside target range but close to lower boundary (Intended to handle noise drifting)
           {
                   temp -= 1;
                   flag = 1;
           }
           else if(tmp <= center-tolerance*4/5)   // Imgram data is inside target range but close to upper boundary (Intended to handle noise drifting)
           {
                   temp += 1;
                   flag = 1;
           }
           else                                   // Imgram data is inside target range and solid
           {
              pFW_BL_RAM16[j] = itr>4? 1:0;
           }
           //write back to LNA_BL_RAM
           if(!FW_tmp)
           {
           pLNA_BL_RAM16[j] = temp >= 0? temp : ((-1)*temp)|0xf000;
           }
    }
    return flag;
}

#endif

                          /* Rx    0,     1,     2,     3,     4,     5,     6,     7,     8,     9,    10,    11,    12,    13, */
int32_t    rx_lna_center[56] = {   0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,  

                          /* Rx   14,    15,    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,    26,    27, */
                                   0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0, 

                          /* Rx   28,    29,    30,    31,    32,    33,    34,    35,    36,    37,    38,    39,    40,    41, */
                                  0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0, 

                          /* Rx   42,    43,    44,    45,    46,    47,    48,    49,    50,    51,    52,    53,    54,    55, */
                                   0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0}; 

uint8_t            *dummyptr;
uint32_t           dummybuf[(2*HWDEF_DCACHE_LINESIZE)/sizeof(uint32_t) - 1];
#if 1
//void cal_bl(results_struct_t *results)
void cal_bl( uint8_t *replyp )
{
		uint32_t           i; 
    uint32_t           itr; 
    uint32_t           idx;
    uint32_t           bsflag;
    uint32_t           orig_rcm_adc_clk;
    uint32_t           orig_scm_scmie;
    uint32_t           orig_cdm_cdmcfg;
    volatile uint16_t  *pIMGRAM16;
    volatile uint16_t  *pFW_BL_RAM16;
    volatile uint16_t  *pLNA_BL_RAM16;

    /*
     * Preserve a bunch of registers so we can restore them later.
     */
    orig_rcm_adc_clk = REG_READ32(RCM_ADC_CLK);
    orig_scm_scmie = REG_READ32(SCM_SCMIE);
    orig_cdm_cdmcfg = REG_READ32(CDM_CDMCFG);

    /* Clear out the LNA Baseline RAM */
    //register_bulk_set(HWDEF_MEMMAP_LNA_BL_RAM_BASE, 0x00000000, (HWDEF_MEMMAP_LNA_BL_RAM_SIZE/4));
		for( i = 0; i < HWDEF_MEMMAP_LNA_BL_RAM_SIZE / 4; i++ )
    {
    	*( uint32_t * )( HWDEF_MEMMAP_LNA_BL_RAM_BASE + i * 4 ) = 0x0;
    }
        
    /* Clear out the FW Baseline RAM */
    //register_bulk_set(HWDEF_MEMMAP_FW_BL_RAM_BASE, 0x00000000, (HWDEF_MEMMAP_FW_BL_RAM_SIZE/4));
		for( i = 0; i < HWDEF_MEMMAP_FW_BL_RAM_SIZE / 4; i++ )
    {
    	*( uint32_t * )( HWDEF_MEMMAP_FW_BL_RAM_BASE + i * 4 ) = 0x0;
    }

    /*
     * Override some registers for calibration mode.
     */

#if 1
	wr_word( RCM_ADC_CLK,	  0 );			 // ADC clk = 24MHz, and enable
#else
    REG_WRITE32(RCM_ADC_CLK, (RCM_ADC_CLK_CTRL
                              | REG_STUFFINT32(RCM_ADC_CLK_FREQ_SEL_DIV4,
                                               RCM_ADC_CLK_FREQ_SEL_B,
                                               RCM_ADC_CLK_FREQ_SEL)));			// ADC clk = 12MHz
#endif
/* = 0x00000003 */
/*
    REG_WRITE32(CDM_CDMCFG, (CDM_CDMCFG_BYPASS
                             | REG_STUFFINT32(
                                 CDM_CDMCFG_PSO_PIXEL_ARRIVAL_ORDER,
                                 CDM_CDMCFG_PSO_B,
                                 CDM_CDMCFG_PSO)));
*/
    /* = 0x00000028 */
    //REG_WRITE32(CDM_CDMCFG, 0x0000002C);  // Hardcode it to be 2C. Denali
    REG_WRITE32(CDM_CDMCFG, 0x0000001C);  // Shasta 1c

//	REG_WRITE32( SCM_ITGLEN, 0x0151); // 0xA0 = 160
	REG_WRITE32( SCM_SSDC, 0x00ff);	// must be >= 0x0028
	REG_WRITE32( SCM_ISDC, 0x00ff);	// 

    REG_WRITE32(SCM_SCMIE, 0);

	REG_WRITE32(SCM_TXCAL, 0);

    /* Collect and discard some initial frames */
    for( itr = 0; itr < 1; itr++ )
    {
        collect_frame( );
    }

   	*( ((uint16_t *) replyp ) + 1 ) = VCSFW_STATUS_OKAY;
		uint16_t *rp = ( uint16_t * ) replyp + 1;

	rp++;
	*rp++ = *( ( volatile unsigned short * )SCM_SCMSTAT );
	*rp++ = *( ( volatile unsigned short * )( SCM_SCMSTAT + 2) );

	*rp++ = *( ( volatile unsigned short * )SCM_TXCFG0 );
	*rp++ = *( ( volatile unsigned short * )( SCM_TXCFG0 + 2) );
	*rp++ = *( ( volatile unsigned short * )SCM_TXCFG1 );
	*rp++ = *( ( volatile unsigned short * )( SCM_TXCFG1 + 2) );
	*rp++ = *( ( volatile unsigned short * )SCM_TXCFG2 );
	*rp++ = *( ( volatile unsigned short * )( SCM_TXCFG2 + 2) );
	*rp++ = *( ( volatile unsigned short * )SCM_TXCFG3 );
	*rp++ = *( ( volatile unsigned short * )( SCM_TXCFG3 + 2) );
	
	*rp++ = *( ( volatile unsigned short * )CDM_CDMCFG );
	*rp++ = *( ( volatile unsigned short * )( CDM_CDMCFG + 2) );
	
	*rp++ = *( ( volatile unsigned short * )CDM_CDMSTAT );
	*rp++ = *( ( volatile unsigned short * )( CDM_CDMSTAT + 2) );
	
	*rp++ = *( ( volatile unsigned short * )WOE_HS_OSC_TRIM );
	*rp++ = 0xffff;

	*rp++ = *( ( volatile unsigned short * )WOE_DVREG );
	*rp++ = 0xffff;

	*rp++ = *( ( volatile unsigned short * )BC_CB_STRT_ADDR );
	*rp++ = *( ( volatile unsigned short * )( BC_CB_STRT_ADDR + 2) );

	*rp++ = *( ( volatile unsigned short * )BC_CB_END_ADDR );
	*rp++ = *( ( volatile unsigned short * )( BC_CB_END_ADDR + 2) );

	*rp++ = ( ( volatile unsigned short * )dummybuf );
	*rp++ = ( ( volatile unsigned short * )( dummybuf + 2) );

	*rp++ = ( ( volatile unsigned short * )dummyptr );
	*rp++ = ( ( volatile unsigned short * )( dummyptr + 2) );




for( i = 0; i < HWDEF_MEMMAP_IMGRAM_SIZE / 4 - 50; i++ )
{
	*rp++ = *((volatile unsigned short*)( HWDEF_MEMMAP_IMGRAM_BASE + i * 4 ) );
	*rp++ = *((volatile unsigned short*)( HWDEF_MEMMAP_IMGRAM_BASE + i * 4 + 2 ) );
}

#if 0	
	for( i = 0; i < HWDEF_MEMMAP_ADCRAM1_SIZE / 4 - 50; i++ )
    {
		*rp++ = *((volatile unsigned short*)( HWDEF_MEMMAP_ADCRAM1_BASE + i * 4 ) );
		*rp++ = *((volatile unsigned short*)( HWDEF_MEMMAP_ADCRAM1_BASE + i * 4 + 2 ) );
    }
	
	for( i = 0; i < HWDEF_MEMMAP_FW_BL_RAM_SIZE / 4 - 50; i++ )
	{
		*rp++ = *((volatile unsigned short*)( HWDEF_MEMMAP_FW_BL_RAM_BASE + i * 4 ) );
		*rp++ = *((volatile unsigned short*)( HWDEF_MEMMAP_FW_BL_RAM_BASE + i * 4 + 2 ) );
	}

	// can not access when frame collection
	for( i = 0; i < HWDEF_MEMMAP_LNA_BL_RAM_SIZE / 4 - 50; i++ )
	{
		*rp++ = *((volatile unsigned short*)( HWDEF_MEMMAP_LNA_BL_RAM_BASE + i * 4 ) );
		*rp++ = *((volatile unsigned short*)( HWDEF_MEMMAP_LNA_BL_RAM_BASE + i * 4 + 2 ) );
	}
	
	for( i = 0; i < HWDEF_MEMMAP_CDMRAM_SIZE / 4 - 50; i++ )
	{
		*rp++ = *((volatile unsigned short*)( HWDEF_MEMMAP_CDMRAM_BASE + i * 4 ) );
		*rp++ = *((volatile unsigned short*)( HWDEF_MEMMAP_CDMRAM_BASE + i * 4 + 2 ) );
	}

	// can not access when frame collection
	for( i = 0; i < HWDEF_MEMMAP_DMRAM_SIZE / 4 - 50; i++ )
	{
		*rp++ = *((volatile unsigned short*)( HWDEF_MEMMAP_DMRAM_BASE + i * 4 ) );
		*rp++ = *((volatile unsigned short*)( HWDEF_MEMMAP_DMRAM_BASE + i * 4 + 2 ) );
	}
	
	for( i = 0; i < HWDEF_MEMMAP_IMGRAM_SIZE / 4 - 50; i++ )
	{
		*rp++ = *((volatile unsigned short*)( HWDEF_MEMMAP_IMGRAM_BASE + i * 4 ) );
		*rp++ = *((volatile unsigned short*)( HWDEF_MEMMAP_IMGRAM_BASE + i * 4 + 2 ) );
	}
		
		// just for debug
		for( i = 0; i < 100; i++ )
    {
    	*++rp = i + 5;
    }
#endif
		
#if 0	
		for( i = 0; i < HWDEF_MEMMAP_IMGRAM_SIZE / 2; i++ )
    {
    	*rp++ = *( uint16_t * )( HWDEF_MEMMAP_IMGRAM_BASE + i * 2 );
    }
    

    /* LNA BL Calibration */
    for(itr=0; itr<12; itr++)
    {
        collect_frame();

        // Binary Search again
        bsflag = cal_bl_binarysearch(itr, &rx_lna_center[0], test_const_params.rx_lna_tolerance);
        if(!bsflag) 
        {
            break;
        }
    }

    /* Code to check the results of the LNA offset calibration */
    {
        pFW_BL_RAM16 = (volatile uint16_t *) HWDEF_MEMMAP_FW_BL_RAM_BASE;
        pLNA_BL_RAM16 = (volatile uint16_t *) HWDEF_MEMMAP_LNA_BL_RAM_BASE;
    }

    /* Restore the original registers */
    REG_WRITE32(CDM_CDMCFG, orig_cdm_cdmcfg);
    REG_WRITE32(RCM_ADC_CLK, orig_rcm_adc_clk);
    REG_WRITE32(SCM_SCMIE, orig_scm_scmie);


    /* Collect a baseline frame and copy to FW_BL and evaluate*/
    {
        for(itr=0; itr<test_const_params.fw_bl_skip_frames; itr++)
        {
            collect_frame();
        }

        collect_frame();

        pIMGRAM16 = (volatile uint16_t *) HWDEF_MEMMAP_IMGRAM_BASE;
        pFW_BL_RAM16 = (volatile uint16_t *) HWDEF_MEMMAP_FW_BL_RAM_BASE;

        for (idx = 0; idx < TEST_TOTAL_PXLS; idx++) 
        {
            pFW_BL_RAM16[idx] = pIMGRAM16[idx];
        }
    }
#endif

}
#endif

// BIST Tx Rx open Test
//#include "pkg_95b_b0_setup_tags.h"

//void test_rx_tx_open_use_image(results_struct_t *results)
static int patch_cmd_handler_test_TxRx_open( void *ctxp, const uint8_t *cmdbufp, unsigned int cmdlen, uint8_t *replyp )
{
  uint32_t          i;
  volatile int16_t  *pIMGRAM16;
  volatile uint16_t *pFWBL16;
  volatile uint32_t *pDMRAM;
  int16_t           val1;
  int16_t           min, max, tx_range_max;
  int32_t           ix, ix_tx_rng;
  int32_t           r;
  int32_t           c;
  int32_t           sum, avg, max32, sum_tx_rng, avg_tx_rng, max32_tx_rng;
  uint32_t          orig_cdm_cdmcfg;
  uint32_t          lessTx;
	int32_t           frame_avg;
	int16_t           line_avg[TEST_QNTY_RX] = {0};
	int16_t           pxl_delta[TEST_PXLS_PER_RX] = {0};
	int16_t           imgData;


    // return these HW register settings to their original settings
    //REG_WRITE32(SCM_BG_BIAS_TRIM, orig_bg_cfg);
    wr_word(SCM_ADC_RAMP_GEN0, 0x00000000);
    wr_word(SCM_ADC_RAMP_GEN1, 0x00000000);
    wr_word(SCM_AFE_SATDET0, 0x00000000);
    wr_word(SCM_AFE_SATDET1, 0x00000000);
    REG_WRITE32(SCM_LNA_AVREG0, 0x0000000f);
    REG_WRITE32(SCM_LNA_AVREG1, 0x0000000f);
	

	//POR release
	wr_word(WOE_HV_DPOR, 0xA57E5A69);

	wr_word(WOE_STARTUP_DELAY, 0x00020010);   // cpu fast clock

	REG_WRITE32(WOE_HS_OSC_TRIM,0x00003115);


	REG_WRITE32(WOE_DVREG, 0x000000f);

	
    wr_word(RCM_AFE_CLOCK, 0x00000007);
    wr_word(RCM_ADC_CLK, 0x00000001);
    wr_word(RCM_CDM_CLOCK, 0x00000001);      // CDM CLOCK 144MHZ 
    wr_word(RCM_OTHER_CLK, 0x00000002);

	        /* Set some default values --> may be set later */
        wr_word(SCM_LNA_AVREG0, 0x0000000f);
        wr_word(SCM_LNA_AVREG1, 0x0000000f);

		
  wr_word( SCM_SCMCFG ,   0x02 | SCM_SCMCFG_MXRWIN | SCM_SCMCFG_FORCE_AFE_ON );                  // sensor type: square sensor
	
 // wr_word(WOE_SPARE, 0x00000001);
  
  
  
    /* Clear out the memories */
    {
        /* Clear out the Image RAM */
        //register_bulk_set(HWDEF_MEMMAP_IMGRAM_BASE, 0x00000000, (HWDEF_MEMMAP_IMGRAM_SIZE/4));
				for( i = 0; i < HWDEF_MEMMAP_IMGRAM_SIZE / 4; i++ )
        {
        	*( uint32_t * )( HWDEF_MEMMAP_IMGRAM_BASE + i * 4 ) = 0x0;
        }
       
        /* Clear out Drive Matrix RAM */
        //register_bulk_set(HWDEF_MEMMAP_DMRAM_BASE, 0x00000000, (HWDEF_MEMMAP_DMRAM_SIZE/4));
				for( i = 0; i < HWDEF_MEMMAP_DMRAM_SIZE / 4; i++ )
        {
        	*( uint32_t * )( HWDEF_MEMMAP_DMRAM_BASE + i * 4 ) = 0x0;
        }
        
        /* Clear out dode division multiplexing RAM */
        //register_bulk_set(HWDEF_MEMMAP_CDMRAM_BASE, 0x00000000, (HWDEF_MEMMAP_CDMRAM_SIZE/4));
				for( i = 0; i < HWDEF_MEMMAP_CDMRAM_SIZE / 4; i++ )
        {
        	*( uint32_t * )( HWDEF_MEMMAP_CDMRAM_BASE + i * 4 ) = 0x0;
        }
        
        /* Clear out the LNA Baseline RAM */
        //register_bulk_set(HWDEF_MEMMAP_LNA_BL_RAM_BASE, 0x00000000, (HWDEF_MEMMAP_LNA_BL_RAM_SIZE/4));
				for( i = 0; i < HWDEF_MEMMAP_LNA_BL_RAM_SIZE / 4; i++ )
        {
        	*( uint32_t * )( HWDEF_MEMMAP_LNA_BL_RAM_BASE + i * 4 ) = 0x0;
        }
        
        /* Clear out the FW Baseline RAM */
        //register_bulk_set(HWDEF_MEMMAP_FW_BL_RAM_BASE, 0x00000000, (HWDEF_MEMMAP_FW_BL_RAM_SIZE/4));
				for( i = 0; i < HWDEF_MEMMAP_FW_BL_RAM_SIZE / 4; i++ )
        {
        	*( uint32_t * )( HWDEF_MEMMAP_FW_BL_RAM_BASE + i * 4 ) = 0x0;
        }

				/* Clear out the ADC RAM0 */
				for( i = 0; i < HWDEF_MEMMAP_ADCRAM0_SIZE / 4; i++ )
        {
        	*( uint32_t * )( HWDEF_MEMMAP_ADCRAM0_BASE + i * 4 ) = 0x0 + i;
        }

				/* Clear out the ADC RAM1 */
				for( i = 0; i < HWDEF_MEMMAP_ADCRAM1_SIZE / 4; i++ )
        {
        	*( uint32_t * )( HWDEF_MEMMAP_ADCRAM1_BASE + i * 4 ) = 0x0 + i + 100;
        }

				/* Clear out the ADC RAM2 */
				for( i = 0; i < HWDEF_MEMMAP_ADCRAM2_SIZE / 4; i++ )
        {
        	*( uint32_t * )( HWDEF_MEMMAP_ADCRAM2_BASE + i * 4 ) = 0x0 + i + 200;
        }

				/* Clear out the ADC RAM3 */
				for( i = 0; i < HWDEF_MEMMAP_ADCRAM3_SIZE / 4; i++ )
        {
        	*( uint32_t * )( HWDEF_MEMMAP_ADCRAM3_BASE + i * 4 ) = 0x0 + i + 300;
        }
    }
#if 0
    /* Load the setups using tag memory */
    i = fpscan_tagsparse( &fpscan_tagdata[0], sizeof(fpscan_tagdata), FALSE,
        									&results->parameter[TEST_PARAMETER_TAG_WRITE_TAGS_USED],
        									&results->parameter[TEST_PARAMETER_TAG_WRITE_TAGS_SKIP] );

    if( 0 != i )
    {
        results->parameter[TEST_PARAMTER_RUN_FAIL_CODE_LOC] = INTERNAL_ERROR_CODE_FPSCAN_TAG_FAIL;
        SET_STATUS_BIT(TEST_STATUS_FAIL_INTERNAL); 
    }
#endif

    /*
     * Point the buffer controller to point to our cache-aligned
     *  location on the stack.  I guess it doesn't really need
     *  to be cache-aligned, but it's good form.
     */
    dummyptr = ( uint8_t * )HWDEF_DCACHE_ALIGN( &( dummybuf[0] ) );
    REG_WRITE32( BC_CB_STRT_ADDR, REG_STUFFINT32( dummyptr, BC_CB_STRT_ADDR_LOC_B, BC_CB_STRT_ADDR_LOC) );
    REG_WRITE32( BC_CB_END_ADDR, REG_STUFFINT32( dummyptr + HWDEF_DCACHE_LINESIZE, BC_CB_END_ADDR_LOC_B, BC_CB_END_ADDR_LOC) );

    /* write 0x1 to release afe from reset. If this is not done,
      analog front end will be held in reset. */
    REG_WRITE32( WOE_SPARE, 0x00000001 );

		//wr_word( SCM_BG_BIAS_TRIM,0x0000A000 ); 				 // Current trim
		wr_word( SCM_BG_BIAS_TRIM,0x0000A000 );                  // Current trim
		

#if 0		// rectangle sensor
		// configure Max image sise 112x56
		// Enable random TXCFGs 
		wr_word( SCM_TXCFG0, 0xFFFFFFFF );
		wr_word( SCM_TXCFG1, 0xFFFFFFFF );
		wr_word( SCM_TXCFG2, 0xFFFFFFFF );
		wr_word( SCM_TXCFG3, 0x0000FFFF );
                                    
		wr_word( SCM_RXCFG0, 0xFFFFFFFF );
		wr_word( SCM_RXCFG1, 0x00FFFFFF );
		wr_word( SCM_RXCFG2, 0x00000000 );
#else   // square sensor
  	// Enable 12 ~ 99 Tx driver 
	*( ( volatile unsigned int *)SCM_TXCFG0 ) = 0x0FFFFF000;
	*( ( volatile unsigned int *)SCM_TXCFG1 ) = 0x0FFFFFFFF;
	*( ( volatile unsigned int *)SCM_TXCFG2 ) = 0x0FFFFFFFF;
	*( ( volatile unsigned int *)SCM_TXCFG3 ) = 0x00000000F;

	// Enable 0 ~ 79 Rx receiver 
	*( ( volatile unsigned int *)SCM_RXCFG0 ) = 0x0FFFFFFFF;
	*( ( volatile unsigned int *)SCM_RXCFG1 ) = 0x0FFFFFFFF;
	*( ( volatile unsigned int *)SCM_RXCFG2 ) = 0x00000FFFF;
#endif

#if 0	
	// Set up the clocks for fingerprint scanning.
	// Assumes fast clock and slow clock are already set.
	// Initialize SCM registers to allow a default fingerprint
	// to be generated without calibrated values.
    REG_WRITE32( RCM_AFE_CLOCK, ( RCM_AFE_CLOCK_CTRL | REG_STUFFINT32( RCM_AFE_CLOCK_FREQ_SEL_DIV24,
                                  																		 RCM_AFE_CLOCK_FREQ_SEL_B, RCM_AFE_CLOCK_FREQ_SEL)));		// enable AFE clk, freq = 6MHz
    REG_WRITE32( RCM_ADC_CLK, ( RCM_ADC_CLK_CTRL | REG_STUFFINT32(	RCM_ADC_CLK_FREQ_SEL_DIV2,
                                  																	RCM_ADC_CLK_FREQ_SEL_B,
                                  																	RCM_ADC_CLK_FREQ_SEL)));					// enable ADC clk, freq = 24MHz
    REG_WRITE32( RCM_CDM_CLOCK, ( RCM_CDM_CLOCK_CTRL | REG_STUFFINT32(	RCM_CDM_CLOCK_FREQ_SEL_DIV1,
                                  																			RCM_CDM_CLOCK_FREQ_SEL_B,
                                  																			RCM_CDM_CLOCK_FREQ_SEL)));		// enable CDM clk, freq = 144MHz

    REG_WRITE32( RCM_OTHER_CLK, RCM_OTHER_CLK_GPIO_CTRL );		// enable GPIO clk
#else
   	wr_word( RCM_AFE_CLOCK,   RCM_AFE_CLOCK_CTRL | 3 << RCM_AFE_CLOCK_FREQ_SEL_B );  // AFE clk = 6MHz, and enable, phase delay = 0.0 CPU clk
		wr_word( RCM_ADC_CLK,     RCM_ADC_CLK_CTRL );            // ADC clk = 24MHz, and enable
		wr_word( RCM_CDM_CLOCK,   RCM_CDM_CLOCK_CTRL      );     // CDM clk = 144MHZ, and enable 
		wr_word( RCM_OTHER_CLK,   RCM_OTHER_CLK_GPIO_CTRL );     // GPIO clk enable
#endif
    //Calibration 
    //cal_bl(results);  //TROY: cal was enabled for new Rx opens/shorts test
    cal_bl( replyp );
#if 0
    orig_cdm_cdmcfg = REG_READ32(CDM_CDMCFG); //store for later use

    REG_WRITE32(CDM_CDMCFG, 0x0000000C);  // disable BL subtraction and disable CDM decode for the Rx opens/shorts test...

    pIMGRAM16 = (volatile int16_t *) HWDEF_MEMMAP_IMGRAM_BASE;
    pFWBL16 = (volatile uint16_t *) HWDEF_MEMMAP_FW_BL_RAM_BASE;
    pDMRAM = (volatile uint32_t *) HWDEF_MEMMAP_DMRAM_BASE;


    /* Perform Rx Open Test, defect/open in the pixel area */
    if( 0 < ( ctrl_code & CTRL_CODE_INC_RX_OPEN ) )
    {
        results->parameter[TEST_PARAMETER_RUN_CODE] |= RUN_CODE_INC_RX_OPEN;

        /* Write the Drive Matrix Memory to 0 */
        pDMRAM[0] = 0x00000001; //was 0's  //enable Tx0
        for( i = 1; i < 1296; i++ )
        {
					if ( !( i % 9 ) )
						pDMRAM[i] = 0x00000001; //was 0's  //enable Tx0
					else if( !(( i + 1 ) % 9 ) )
						pDMRAM[i] = 0x40000000; //was 0's  //enable Tx143
					else
						pDMRAM[i] = 0x00000000;
        }

        /* Collect Frame data for Rx testing */
        collect_frame();

        for( i = 0; i < TEST_TOTAL_PXLS; i++ )
        {
					pIMGRAM16[i] += 2048;
        }

        lessTx = 0;
        // Calculate the parameters for each rx 
        for( c = 0; c < TEST_QNTY_RX; c++ )
        {
					sum = 0;
					min = 10000;
					max = 0;
					for( r = 0; r < TEST_PXLS_PER_RX - lessTx; r++ ) //adjusted for 112C1
					{
						if( pIMGRAM16[ r * TEST_QNTY_RX + c ] > max ) 
							max = pIMGRAM16[ r * TEST_QNTY_RX + c ];
						if( pIMGRAM16[ r * TEST_QNTY_RX + c] < min) 
							min = pIMGRAM16[ r * TEST_QNTY_RX + c];
					}
					results->rx_im_rng[c] = max - min; 
        }

        results->parameter[TEST_PARAMETER_MIN_RX_RNG]=10000;
        for( c = 0; c < TEST_QNTY_RX; c++ )
        {
            check_value_add_fail_count(results->rx_im_rng[c],  TEST_STATUS_FAIL_RX_OPEN,  
                &test_params[limit_set].rx_open_min_range,  results, TEST_PARAMETER_QNTY_RX_OPEN_MIN_RANGE_FAILS);

            sum = 1;
            for( i = 0; i < test_const_params.n_rx_open_skip; i++ )
            {
                if( c == test_const_params.rx_open_skip[i] )
                {
                    sum = 0;
                    break;
                }
            }

            //find the min and max along with the idx
            if ((int16_t)results->parameter[TEST_PARAMETER_MIN_RX_RNG] > results->rx_im_rng[c])
            {
                results->parameter[TEST_PARAMETER_MIN_RX_RNG] = results->rx_im_rng[c];
                results->parameter[TEST_PARAMETER_MIN_RX_RNG_IDX] = c;
            }

        }


    /* Perform Rx Open Test, defect/open in the via area, entire trace floating */
        /* Clear out the LNA Baseline RAM */
        //register_bulk_set(HWDEF_MEMMAP_LNA_BL_RAM_BASE, 0x00000000, (HWDEF_MEMMAP_LNA_BL_RAM_SIZE/4));
        for( i = 0; i < HWDEF_MEMMAP_LNA_BL_RAM_SIZE / 4; i++ )
        {
        	*( uint32_t * )( HWDEF_MEMMAP_LNA_BL_RAM_BASE + i * 4 ) = 0x0;
        }
        
        /* Collect Frame data for Rx testing */
        collect_frame( );

        for( i = 0; i < TEST_TOTAL_PXLS; i++ )
        {
					pIMGRAM16[i] += 2048;
        }
        
        // Get each Rx line avg - Chris
        for( c = 0; c < TEST_QNTY_RX; c++ )
        {
					sum = 0;
					// Calculate average rx data except edge 2 pixels.
					for( r = 2; r < TEST_PXLS_PER_RX-lessTx - 2; r++ ) //adjusted for 112C1
					{
						sum += pIMGRAM16[ r * TEST_QNTY_RX + c ];
					}
					line_avg[c] = ( sum / ( TEST_PXLS_PER_RX - lessTx - 2 - 2 ) ); //adjusted for 112C1
					frame_avg += line_avg[c];
        }
				frame_avg = frame_avg / TEST_QNTY_RX;

        // Calculate the parameters for each rx 
        for( c = 0; c < TEST_QNTY_RX; c++ )
        {
					sum = 0;
					for( r = 0; r < TEST_PXLS_PER_RX - lessTx; r++ ) //adjusted for 112C1
					{
						imgData = pIMGRAM16[ r * TEST_QNTY_RX + c ];

						//pxl_delta[r] = imgData >= frame_avg ? imgData - frame_avg : frame_avg - imgData;
						if( imgData >= frame_avg )
							pxl_delta[r] = imgData - frame_avg;
						if( imgData < frame_avg )
							pxl_delta[r] = frame_avg - imgData;

						sum += pxl_delta[r];
					}
					results->rx_im_tx0[c] = pIMGRAM16[c];
					results->rx_im_delta[c] = ( sum / ( TEST_PXLS_PER_RX - lessTx ) ); //adjusted for 112C1
        }
        
        results->parameter[TEST_PARAMETER_MIN_RX_TX0]=10000;
        results->parameter[TEST_PARAMETER_MAX_RX_DELTA_FRM_AVG]=0;
        for(c=0; c<TEST_QNTY_RX; c++)
        {
            sum = 1;
            for(i=0; i<test_const_params.n_rx_open_skip; i++)
            {
                if (c == test_const_params.rx_open_skip[i])
                {
                    sum = 0;
                    break;
                }
            }
            if (sum)
            {
				check_value_add_fail_count(results->rx_im_delta[c],  TEST_STATUS_FAIL_RX_OPEN_IMG_DELTA,	
					&test_params[limit_set].rx_open_max_delta_frm_avg,	results, TEST_PARAMETER_QNTY_RX_OPEN_MAX_DELTA_FAILS); //this test has been disabled via the limits
                check_value_add_fail_count(results->rx_im_tx0[c],  TEST_STATUS_FAIL_RX_OPEN_IMG,  
                    &test_params[limit_set].rx_open_min_tx0,  results, TEST_PARAMETER_QNTY_RX_OPEN_MIN_TX0_FAILS);

				if ((int16_t)results->parameter[TEST_PARAMETER_MAX_RX_DELTA_FRM_AVG] < results->rx_im_delta[c])
				{
					results->parameter[TEST_PARAMETER_MAX_RX_DELTA_FRM_AVG] = results->rx_im_delta[c];
					results->parameter[TEST_PARAMETER_MAX_RX_DELTA_FRM_AVG_IDX] = c;
				}
                if ((int16_t)results->parameter[TEST_PARAMETER_MIN_RX_TX0] > results->rx_im_tx0[c])
                {
                    results->parameter[TEST_PARAMETER_MIN_RX_TX0] = results->rx_im_tx0[c];
                    results->parameter[TEST_PARAMETER_MIN_RX_TX0_IDX] = c;
                }
            }
        }

    }

    /* collecte frame data for Tx testing*/
    if (0 < (ctrl_code & CTRL_CODE_INC_TX_OPEN))
    {
        results->parameter[TEST_PARAMETER_RUN_CODE] |= RUN_CODE_INC_TX_OPEN;

        /* Load the setups using tag memory */
        //i = fpscan_tagsparse(&fpscan_tagdata[0], sizeof(fpscan_tagdata), results, FALSE);
        i = fpscan_tagsparse(&fpscan_tagdata[0], sizeof(fpscan_tagdata), FALSE,
            &results->parameter[TEST_PARAMETER_TAG_WRITE_TAGS_USED],
            &results->parameter[TEST_PARAMETER_TAG_WRITE_TAGS_SKIP]);
        if (0 != i)
        {
            results->parameter[TEST_PARAMTER_RUN_FAIL_CODE_LOC] = INTERNAL_ERROR_CODE_FPSCAN_TAG_FAIL;
            SET_STATUS_BIT(TEST_STATUS_FAIL_INTERNAL); 
        }


        REG_WRITE32(CDM_CDMCFG, orig_cdm_cdmcfg); 
        /* Clear out the Image RAM */
        register_bulk_set(HWDEF_MEMMAP_IMGRAM_BASE, 0x00000000, (HWDEF_MEMMAP_IMGRAM_SIZE/4));

        /* Clear out the LNA Baseline RAM */
        register_bulk_set(HWDEF_MEMMAP_LNA_BL_RAM_BASE, 0x00000000, (HWDEF_MEMMAP_LNA_BL_RAM_SIZE/4));

        /* Clear out the FW Baseline RAM */
        register_bulk_set(HWDEF_MEMMAP_FW_BL_RAM_BASE, 0x00000000, (HWDEF_MEMMAP_FW_BL_RAM_SIZE/4));

        collect_frame();

        for(i=0; i<TEST_TOTAL_PXLS; i++)
        {
            val1 = pIMGRAM16[i];
            val1 /= 8;
            val1 = val1 + 4096;
            pIMGRAM16[i] = val1;
        }

        sum = 0;
        sum_tx_rng = 0;
        i = 0;
        tx_range_max = 0;
        for(r=0; r<TEST_PXLS_PER_RX-lessTx; r++) //adjusted for 112C1
        {
            results->tx_im_gnd_sum[r] = 0;
            min = 10000;
            max = 0;
            for(c=0; c<TEST_QNTY_RX; c++)
            {
                results->tx_im_gnd_sum[r] += pIMGRAM16[i];
                if (pIMGRAM16[r*TEST_QNTY_RX+c] > max) 
                    max = pIMGRAM16[r*TEST_QNTY_RX+c];
                if (pIMGRAM16[r*TEST_QNTY_RX+c] < min) 
                    min = pIMGRAM16[r*TEST_QNTY_RX+c];
                i++;
            }
            results->tx_im_gnd_sum[r] = results->tx_im_gnd_sum[r]/TEST_QNTY_RX;
            sum += results->tx_im_gnd_sum[r];
            results->tx_rng[r] = max - min;
            sum_tx_rng += results->tx_rng[r];
        }
        avg = sum/(TEST_PXLS_PER_RX-lessTx-4); //adjusted for 112C1
        avg_tx_rng = sum_tx_rng/(TEST_PXLS_PER_RX-lessTx-4);

        max32 = 0;
        max32_tx_rng = 0;
        results->parameter[TEST_PARAMETER_QNTY_TX_OPEN_FAILS] = 0;

        for(r=2; r<TEST_PXLS_PER_RX-2-lessTx; r++) //skip outside 2  //adjusted for 112C1
        {
            if (avg > results->tx_im_gnd_sum[r])
                results->tx_im_gnd_sum[r] = avg - results->tx_im_gnd_sum[r]; 
            else
                results->tx_im_gnd_sum[r] = results->tx_im_gnd_sum[r] - avg;
                 
            if (max32 < results->tx_im_gnd_sum[r])
            {
                max32 = results->tx_im_gnd_sum[r];
                ix = r;
            }
            check_value_add_fail_count(results->tx_im_gnd_sum[r],  TEST_STATUS_FAIL_TX_OPEN,  
                    &test_params[limit_set].tx_open,  results, TEST_PARAMETER_QNTY_TX_OPEN_FAILS);


            if (avg_tx_rng > results->tx_rng[r])
                results->tx_rng[r] = avg_tx_rng - results->tx_rng[r]; 
            else
                results->tx_rng[r] = results->tx_rng[r] - avg_tx_rng;
                 
            if (max32_tx_rng < results->tx_rng[r])
            {
                max32_tx_rng = results->tx_rng[r];
                ix_tx_rng = r;
            }
            check_value_add_fail_count(results->tx_rng[r],  TEST_STATUS_FAIL_TX_OPEN,  
                    &test_params[limit_set].tx_range_open,  results, TEST_PARAMETER_QNTY_TX_OPEN_FAILS);
        }

        results->parameter[TEST_PARAMETER_MAX_TX_DELTA_FRM_AVG] = max32;
        results->parameter[TEST_PARAMETER_MAX_TX_DELTA_FRM_AVG_IDX] = ix;

        results->parameter[TEST_PARAMETER_MAX_TX_RNG] = max32_tx_rng;
        results->parameter[TEST_PARAMETER_MAX_TX_RNG_IDX] = ix_tx_rng;

    }
#endif
    
    cmdmgr_reply( ctxp, ( uint8_t* )((( uint16_t* ) replyp ) + 1 ), sizeof( uint16_t ) + 2 * 4096  * sizeof( uint16_t ) );
    
    return -1; 
}
#endif

// BIST Tx Short Test
#if 1
/*******************************************************************************************
Tx short test step:
1. Enable AFE clk with 6MHz
2. SCM config: Disable interrupt, Tx short test enable, testmode enable, sensor type set,
							 sensor operation voltage = 3V, LNA test enable
3. Pull down all Rxs
4. Drive all Txs to GND but only 1 to VCC one by one and 
		read the short status register to check whether short.
5. Send out resault
6. Quit Tx short mode
*******************************************************************************************/
static int patch_cmd_handler_test_Tx_shorts( void *ctxp, const uint8_t *cmdbufp, unsigned int cmdlen, uint8_t *replyp )
{
	uint32_t POLREG_ADDR = SCM_POLREG0;		                   //shasta: 0x800040a0;	denali: 0x800020a0
	uint32_t arr_tx_short_count[4];
	uint32_t polreg_data, rd_data, idx, nNumBitsSet;
	uint32_t i, j, k, l, nop_count;
	uint32_t uiTx = 0;
	uint16_t *rp = ( uint16_t * ) replyp + 1;
	
	*(((uint16_t *) replyp) + 1 ) = VCSFW_STATUS_OKAY;
	
	l = 0;                                                   // Tx of Shasta    square: 0...87 rectangle: 0...111

#if 0	                                                         
	wr_word( WOE_SPARE,       0x00000001 );                  
	wr_word( SCM_BG_BIAS_TRIM,0x0000A000 );                  // Current trim
#endif
	
#if 0
	wr_word( RCM_AFE_CLOCK,   0x00000307 );                  // AFE clk = 6MHz, and enable, phase delay = 1.5 CPU clk
	wr_word( RCM_ADC_CLK,     0x00000003 );                  // ADC clk = 12MHz, and enable
	wr_word( RCM_CDM_CLOCK,   0x00000001 );                  // CDM clk = 144MHZ, and enable
	wr_word( RCM_OTHER_CLK,   0x00000002 );                  // GPIO clk enable
#else
#if 0
	wr_word( RCM_AFE_CLOCK,   RCM_AFE_CLOCK_CTRL | 3 << RCM_AFE_CLOCK_FREQ_SEL_B );  // AFE clk = 6MHz, and enable, phase delay = 0.0 CPU clk
	wr_word( RCM_ADC_CLK,     RCM_ADC_CLK_CTRL   | 1 << RCM_ADC_CLK_FREQ_SEL_B   );  // ADC clk = 12MHz, and enable
	wr_word( RCM_CDM_CLOCK,   RCM_CDM_CLOCK_CTRL      );     // CDM clk = 144MHZ, and enable 
	wr_word( RCM_OTHER_CLK,   RCM_OTHER_CLK_GPIO_CTRL );     // GPIO clk enable
#else
	wr_word( RCM_AFE_CLOCK,   RCM_AFE_CLOCK_CTRL | 3 << RCM_AFE_CLOCK_FREQ_SEL_B );  // AFE clk = 6MHz, and enable, phase delay = 0.0 CPU clk 0x00000007
	wr_word( RCM_ADC_CLK,     0x00000000 );                  // Disable ADC clk
	wr_word( RCM_CDM_CLOCK,   0x00000000 );                  // Disable CDM clk
	wr_word( RCM_OTHER_CLK,   0x00000000 );                  // Diaable GPIO clk
#endif
#endif

	wr_word( SCM_SCMIE,       0x00000000 );                  // disable Interrupt
	wr_word( SCM_TESTCFG,     SCM_TESTCFG_TX_SHORTS_TEST | SCM_TESTCFG_TXTM );  // enable Tx short test(actually pull-up all Tx pad), testmode, // shasta: 0x03, denali: 0x07 powerok override 
	if( TEST_PXLS_PER_RX == 112 ) // rectangle sensor
	{
		wr_word( SCM_SCMCFG ,   0x0       );                  // 3V operation
	}
	else                          // square sensor
	{
		wr_word( SCM_SCMCFG ,   0x02);                  // 3V operation, sensor type: square sensor	
	}
	
	wr_word( SCM_LNA_AVREG0,  SCM_LNA_AVREG0_LNA_TEST_EN );  // Enable lna_test_en bit, shasta: 0x00000100 denali: 0x00001000
	wr_word( SCM_LNA_AVREG1,  SCM_LNA_AVREG0_LNA_TEST_EN );  // Enable lna_test_en bit, shasta: 0x00000100 denali: 0x00001000

	// Short all RXs to check TX-TX and TX-RX shorts.
	for( j = 0; j < 80; j++ )                                // Pulldown all RXs
	{                                                        
		wr_word( SCM_LNA_CFG0 + j * 4, SCM_LNA_CFG0_WOF_RX_GND );   //  shasta: 0x00000010 denali: 0x00000200
	} 

	// Loop to cycle through TXs                             
	for( i = 0; i < 7; i++ )                                 // Shasta No. of Tx: 112 == 16 * 7		88 == 16 * 6 - 8
	{
		for( j = 0; j < 8; j++ )                               // 32bit / 2 / k == 8
		{
			// 00 - GND, 01- High
			wr_word( SCM_POLREG0, 0x0 );
			wr_word( SCM_POLREG1, 0x0 );
			wr_word( SCM_POLREG2, 0x0 );
			wr_word( SCM_POLREG3, 0x0 );
			wr_word( SCM_POLREG4, 0x0 );
			wr_word( SCM_POLREG5, 0x0 );
			wr_word( SCM_POLREG6, 0x0 );
	
#if 0	// unnecessary delay, because of non-capacitive load
			// delay how much time? same as denali?
			for( nop_count = 0; nop_count < 1000; nop_count++ ) 
			{
				asm( "nop" );
			}
#endif
	
			for( k = 0; k < 2; k++, l++ )                        // lower 2bits and higher 2bits
			{
				polreg_data = 1 << ( j * 4 + k * 2 );

#if 0		// unnecessary delay, because of non-capacitive load
				// why delay? because we don't have hardware to debug so we must know all aim of unreasonable action that we think.
				for( nop_count = 0; nop_count < 1000; nop_count++ ) 
				{
					asm( "nop" );
				}
#endif
	
				wr_word( POLREG_ADDR + 4 * i, polreg_data );

#if 0		// unnecessary delay, because of non-capacitive load
				// delay how much time? same as denali?
				for( nop_count = 0; nop_count < 1000; nop_count++ )
				{
					asm( "nop" );
				}
#endif

				// what is the functionality of SCM_MASKID? there is no usable infomation in "Shasta Design Specification".
				// the test result is same without the PRE-CHARGE
				// for square sensor, Tx: 12...99
				REG_WRITE32( SCM_MASKID_HI,   0xffffffff            ); //PRE-CHARGE the data bus
				rd_word    ( SCM_TX_SHORTS_0, arr_tx_short_count[0] ); // Tx 0...31
				REG_WRITE32( SCM_MASKID_HI,   0xffffffff            ); //PRE-CHARGE the data bus
				rd_word    ( SCM_TX_SHORTS_1, arr_tx_short_count[1] ); // Tx 32...63
				REG_WRITE32( SCM_MASKID_HI,   0xffffffff            ); //PRE-CHARGE the data bus
				rd_word    ( SCM_TX_SHORTS_2, arr_tx_short_count[2] ); // Tx 64...95
				REG_WRITE32( SCM_MASKID_HI,   0xffffffff            ); //PRE-CHARGE the data bus
				rd_word    ( SCM_TX_SHORTS_3, arr_tx_short_count[3] ); // Tx 96...111

#if 0		// unnecessary delay, because of non-capacitive load
				// why delay?
			  for( nop_count = 0; nop_count < 1000; nop_count++ )
				{
					asm( "nop" );
				}
#endif

				if( TEST_PXLS_PER_RX == 112 ) // Rx = 112, rectangle sensor, Tx: 0...111		
				{
					arr_tx_short_count[3] &= 0x0000ffff;
				}
				else                          // Rx = 88, square sensor, Tx: 12...99
				{
					arr_tx_short_count[0] &= 0xfffff000;
					arr_tx_short_count[3] &= 0x0000000f;
				}
	 
			  // Calculate number of '1'
	      nNumBitsSet = 0;
			  for( idx = 0; idx < 4; idx++ )
			  {
				  rd_data = arr_tx_short_count[ idx ];
				  
				  while( rd_data != 0 )
				  {
				  	rd_data &= ( rd_data - 1 );
				  	nNumBitsSet++;
				  }
			  }
			  
#if 1
#ifdef OUTPUT_WITH_TXRX_NUMBER
				*++rp = uiTx++;
#endif
#ifdef ALL_TX_SHORT_OUTPUT
				*++rp = arr_tx_short_count[0]; 
				*++rp = arr_tx_short_count[0] >> 16;
				*++rp = arr_tx_short_count[1];
				*++rp = arr_tx_short_count[1] >> 16;
				*++rp = arr_tx_short_count[2];
				*++rp = arr_tx_short_count[2] >> 16;
				*++rp = arr_tx_short_count[3];
				*++rp = arr_tx_short_count[3] >> 16;
#else		
				*++rp = nNumBitsSet == 1 ? 0 : nNumBitsSet + 200;
#endif				 
#else
	      //Assign number to results
			  results->tx_short_count[ l ] = nNumBitsSet;
	      results->parameter[ TEST_PARAMETER_QNTY_BIST_TX_SHORT_CHCK ]++;
	
	      // Fail the part if more than a single bit is set for all TX_SHORTS registers
			  if( nNumBitsSet != 1 )
			  {
	      	results->parameter[TEST_PARAMETER_QNTY_BIST_TX_FAIL_SHORT]++;
					SET_STATUS_BIT( TEST_STATUS_FAIL_BIST_TX_SHORT );
	      	
        	if( MAX_QNTY_TX_SHORT_DEBUG_DATA > results->tx_short_dbg_cnt )
        	{
	      		idx = results->tx_short_dbg_cnt * 5;
	      		results->tx_short_dbg_data[idx] = l;
	      		idx++;
	      		for( n = 0; n < 4; n++, idx++ )
	      		{
	      			results->tx_short_dbg_data[idx] = arr_tx_short_count[n];
	      		}
	      		results->tx_short_dbg_cnt++;
        	}
			  }
#endif

			} // end loop k
		} // end loop j
	} // end loop i

	asm("nop");

if( TEST_PXLS_PER_RX == 112 ) // Rx = 112, rectangle sensor
{
#ifdef ALL_TX_SHORT_OUTPUT
	cmdmgr_reply( ctxp, (uint8_t *)(((uint16_t *) replyp)+1),
                 sizeof(uint16_t) + 9 * TEST_PXLS_PER_RX  * sizeof(uint16_t));
#else
#ifdef OUTPUT_WITH_TXRX_NUMBER
	// debug output
	cmdmgr_reply( ctxp, (uint8_t *)(((uint16_t *) replyp)+1),
                 sizeof(uint16_t) + 2 * TEST_PXLS_PER_RX  * sizeof(uint16_t));
#else  
	// release output
	cmdmgr_reply( ctxp, (uint8_t *)(((uint16_t *) replyp)+1),
                 sizeof(uint16_t) + 1 * TEST_PXLS_PER_RX  * sizeof(uint16_t));
#endif
#endif              
}
else	// Rx = 88, square sensor
{
#ifdef ALL_TX_SHORT_OUTPUT
	cmdmgr_reply( ctxp, (uint8_t *)(((uint16_t *) replyp) + 1 ), sizeof(uint16_t) + 9 * 112  * sizeof(uint16_t));	// output all 112 Tx
#else
#ifdef OUTPUT_WITH_TXRX_NUMBER
	// debug output
	*(((uint16_t *) replyp) + 1 + 12 * 2 ) = *(((uint16_t *) replyp) + 1 );      // copy VCSFW_STATUS to Tx11
	cmdmgr_reply( ctxp, (uint8_t *)(((uint16_t *) replyp) + 1 + 12 * 2 ), sizeof(uint16_t) + 2 * TEST_PXLS_PER_RX  * sizeof(uint16_t)); // output 88 Tx
#else  
	// release output
	*(((uint16_t *) replyp) + 1 + 12 ) = *(((uint16_t *) replyp) + 1 );          // copy VCSFW_STATUS to Tx11
	cmdmgr_reply( ctxp, (uint8_t *)(((uint16_t *) replyp) + 1 + 12 ), sizeof(uint16_t) + 1 * TEST_PXLS_PER_RX  * sizeof(uint16_t)); // output 88 Tx
#endif
#endif  
}

  // quit Tx short mode
  wr_word( RCM_AFE_CLOCK,   0x00000000 );                  // disable AFE clk
	wr_word( SCM_TESTCFG,     0x00000000 );                  // disable short test, disable testmode (*testmode must be disabled before Rx short test )
	wr_word( SCM_LNA_AVREG0,  0x00000000 );                  // Disable lna_test_en bit, shasta: 0x00000100 
	wr_word( SCM_LNA_AVREG1,  0x00000000 );                  // Disable lna_test_en bit, shasta: 0x00000100 

	return -1;                 
}
#else    // Dane code test
static int patch_cmd_handler_test_Tx_shorts( void *ctxp, const uint8_t *cmdbufp, unsigned int cmdlen, uint8_t *replyp )
{
	uint32_t i;
	uint32_t arr_tx_short_count[4];

	uint32_t POLREG_ADDR = SCM_POLREG0;		//shasta: 0x800040a0;			                 //denali: 0x800020a0
	uint32_t POLREG_DATA = 0xaaaaaaaa;
	
	uint32_t j, k, l, nop_count, polreg_data, rd_data, idx, nNumBitsSet;
	uint32_t txshort0, txshort1, txshort2, txshort3;
	
	l = 0;                                                   // Tx of Shasta: 0...111

#if 0	                                                         
	wr_word( WOE_SPARE,       0x00000001 );                  
	wr_word( SCM_BG_BIAS_TRIM,0x0000A000 );                  // Current trim
#endif
	
#if 0
	wr_word( RCM_AFE_CLOCK,   0x00000307 );                  // AFE clk = 6MHz, and enable, phase delay = 1.5 CPU clk
	wr_word( RCM_ADC_CLK,     0x00000003 );                  // ADC clk = 12MHz, and enable
	wr_word( RCM_CDM_CLOCK,   0x00000001 );                  // CDM clk = 144MHZ, and enable
	wr_word( RCM_OTHER_CLK,   0x00000002 );                  // GPIO clk enable
#else
#if 0
	wr_word( RCM_AFE_CLOCK,   RCM_AFE_CLOCK_CTRL | 3 << RCM_AFE_CLOCK_FREQ_SEL_B );  // AFE clk = 6MHz, and enable, phase delay = 0.0 CPU clk
	wr_word( RCM_ADC_CLK,     RCM_ADC_CLK_CTRL   | 1 << RCM_ADC_CLK_FREQ_SEL_B   );  // ADC clk = 12MHz, and enable
	wr_word( RCM_CDM_CLOCK,   RCM_CDM_CLOCK_CTRL      );     // CDM clk = 144MHZ, and enable 
	wr_word( RCM_OTHER_CLK,   RCM_OTHER_CLK_GPIO_CTRL );     // GPIO clk enable
#else
	wr_word( RCM_AFE_CLOCK,   0x00000007 );                  // AFE clk = 6MHz, and enable, phase delay = 0.0 CPU clk
	wr_word( RCM_ADC_CLK,     0x00000000 );                  // Disable ADC clk
	wr_word( RCM_CDM_CLOCK,   0x00000000 );                  // Disable CDM clk
	wr_word( RCM_OTHER_CLK,   0x00000000 );                  // Diaable GPIO clk
#endif
#endif

	wr_word( SCM_SCMIE,       0x00000000 );                  // disable Interrupt
	//wr_word( SCM_TESTCFG,     0x3        );                  // short test, testmode, powerok override  // denali: 0x07
	wr_word( SCM_TESTCFG,     SCM_TESTCFG_TX_SHORTS_TEST );  // short test, testmode, powerok override  // denali: 0x07
	//wr_word( SCM_SCMCFG ,     0x30       );                  // 3V operation
	
	// Short all RXs to check TX-TX and TX-RX shorts.
	
	wr_word( SCM_LNA_AVREG0,  SCM_LNA_AVREG0_LNA_TEST_EN );  // Enable lna_test_en bit, shasta: 0x00000100 denali: 0x00001000
	wr_word( SCM_LNA_AVREG1,  SCM_LNA_AVREG0_LNA_TEST_EN );  // Enable lna_test_en bit, shasta: 0x00000100 denali: 0x00001000

	for( j = 0; j < TEST_QNTY_RX; j++ )                      // Pulldown all RXs
	{                                                        
		wr_word( SCM_LNA_CFG0 + j * 4, SCM_LNA_CFG0_RX_PU  );   //  shasta: 0x00000010 denali: 0x00000200  SCM_LNA_CFG0_WOF_RX_GND
	} 

	*(((uint16_t *) replyp) + 1 ) = VCSFW_STATUS_OKAY;
	uint16_t *rp = ( uint16_t * ) replyp + 1;
	unsigned int uiTx = 0;

	// Loop to cycle through TXs                             
	for( i = 0; i < 7; i++ )                                 // Shasta No. of Tx: 112 == 16 * 7		
	{
		for( j = 0; j < 8; j++ )                               // 32bit / 2 / k == 8
		{
#if 0			
			// 00 - GND, 01- High
			wr_word( SCM_POLREG0, 0x0 );
			wr_word( SCM_POLREG1, 0x0 );
			wr_word( SCM_POLREG2, 0x0 );
			wr_word( SCM_POLREG3, 0x0 );
			wr_word( SCM_POLREG4, 0x0 );
			wr_word( SCM_POLREG5, 0x0 );
			wr_word( SCM_POLREG6, 0x0 );
#else			
			            // 10 - GND, 01- High
            wr_word(SCM_POLREG0,0xaaffffff); // first 12 are Rx's in this iota
            wr_word(SCM_POLREG1,0xaaaaaaaa);
            wr_word(SCM_POLREG2,0xaaaaaaaa);
            wr_word(SCM_POLREG3,0xaaaaaaaa);
            wr_word(SCM_POLREG4,0xaaaaaaaa);
            wr_word(SCM_POLREG5,0xaaaaaaaa);
            wr_word(SCM_POLREG6,0xffffffaa); // last 12 are Rx's in this iota
#endif
	
#if 1		
			// delay how much time? same as denali?
			for( nop_count = 0; nop_count < 1000; nop_count++ ) 
			{
				asm( "nop" );
			}
#endif
	
			for( k = 0; k < 2; k++, l++ )                        // lower 2bits and higher 2bits
			{
#if 0				
				polreg_data = 1 << ( j * 4 + k * 2 );
#else
                if (k == 0)
                {
                    polreg_data = POLREG_DATA - (1<<j*4) ; // grounding one Tx at a time
                }
                else
                {
                    polreg_data = POLREG_DATA - (4<<j*4); // grounding one Tx at a time
                }
#endif
                
#if 1				
				// why delay? because we don't have hardware to debug so we must know all aim of unreasonable action that we think.
				for( nop_count = 0; nop_count < 1000; nop_count++ ) 
				{
					asm( "nop" );
				}
#endif
	
				wr_word( POLREG_ADDR + 4 * i, polreg_data );

#if 1	
				// delay how much time? same as denali?
				for( nop_count = 0; nop_count < 1000; nop_count++ )
				{
					asm( "nop" );
				}
#endif

#if 0
				// what is the functionality of SCM_MASKID? there is no usable infomation in "Shasta Design Specification".
				// the result is same without the PRE-CHARGE
				REG_WRITE32( SCM_MASKID_HI,   0xffffffff            ); //PRE-CHARGE the data bus
				rd_word    ( SCM_TX_SHORTS_0, arr_tx_short_count[0] ); 
				REG_WRITE32( SCM_MASKID_HI,   0xffffffff            ); //PRE-CHARGE the data bus
				rd_word    ( SCM_TX_SHORTS_1, arr_tx_short_count[1] );
				REG_WRITE32( SCM_MASKID_HI,   0xffffffff            ); //PRE-CHARGE the data bus
				rd_word    ( SCM_TX_SHORTS_2, arr_tx_short_count[2] );
				REG_WRITE32( SCM_MASKID_HI,   0xffffffff            ); //PRE-CHARGE the data bus
				rd_word    ( SCM_TX_SHORTS_3, arr_tx_short_count[3] );

#if 0	
				// why delay?
			  for( nop_count = 0; nop_count < 1000; nop_count++ )
				{
					asm( "nop" );
				}
#endif
	 			
			  arr_tx_short_count[3] &= 0x0000ffff;
#else
               REG_WRITE32(SCM_MASKID_HI, 0xffffffff); //precharge
               rd_word(SCM_TX_SHORTS_0, txshort0); 
               REG_WRITE32(SCM_MASKID_HI, 0xffffffff); //precharge
               rd_word(SCM_TX_SHORTS_1, txshort1);
               REG_WRITE32(SCM_MASKID_HI, 0xffffffff); //precharge
               rd_word(SCM_TX_SHORTS_2, txshort2);
               REG_WRITE32(SCM_MASKID_HI, 0xffffffff); //precharge
               rd_word(SCM_TX_SHORTS_3, txshort3);

               for (nop_count = 0; nop_count<1000; nop_count++)
                    asm("nop");

#if 1
               // only 88 Tx's in this iota (square: 80 Rx's x 88 Tx's)
               txshort0 = ~txshort0 & 0xffffff000;
               txshort1 = ~txshort1;
               txshort2 = ~txshort2;
               txshort3 = ~txshort3 & 0x0000000f;
#endif

               arr_tx_short_count[0]= txshort0;
               arr_tx_short_count[1]= txshort1;
               arr_tx_short_count[2]= txshort2;
               arr_tx_short_count[3]= txshort3;
#endif	 
			  // Calculate number of '1'
	      nNumBitsSet = 0;
			  for( idx = 0; idx < 4; idx++ )
			  {
				  rd_data = arr_tx_short_count[ idx ];
				  
				  while( rd_data != 0 )
				  {
				  	rd_data &= ( rd_data - 1 );
				  	nNumBitsSet++;
				  }
			  }
			  
#if 1
				 *++rp = uiTx++;
#ifndef ALL_TX_SHORT_OUTPUT
				 *++rp = nNumBitsSet == 1 ? 0 : nNumBitsSet + 200;
#else		 
				 *++rp = arr_tx_short_count[0] >> 16;
				 *++rp = arr_tx_short_count[0];
				 *++rp = arr_tx_short_count[1] >> 16;
				 *++rp = arr_tx_short_count[1];
				 *++rp = arr_tx_short_count[2] >> 16;
				 *++rp = arr_tx_short_count[2];
				 *++rp = arr_tx_short_count[3] >> 16;
				 *++rp = arr_tx_short_count[3];

				 //*++rp = 0x1234;
				 //*++rp = 0x5678;
				 //*++rp = 0x9abc;
				 //*++rp = 0xdef0;
				 //*++rp = 0x1234;
				 //*++rp = 0x5678;
				 //*++rp = 0x9abc;
				 //*++rp = 0xdef0;
#endif				 
#else
	      //Assign number to results
			  results->tx_short_count[ l ] = nNumBitsSet;
	      results->parameter[ TEST_PARAMETER_QNTY_BIST_TX_SHORT_CHCK ]++;
	
	      // Fail the part if more than a single bit is set for all TX_SHORTS registers
			  if( nNumBitsSet != 1 )
			  {
	      	results->parameter[TEST_PARAMETER_QNTY_BIST_TX_FAIL_SHORT]++;
					SET_STATUS_BIT( TEST_STATUS_FAIL_BIST_TX_SHORT );
	      	
        	if( MAX_QNTY_TX_SHORT_DEBUG_DATA > results->tx_short_dbg_cnt )
        	{
	      		idx = results->tx_short_dbg_cnt * 5;
	      		results->tx_short_dbg_data[idx] = l;
	      		idx++;
	      		for( n = 0; n < 4; n++, idx++ )
	      		{
	      			results->tx_short_dbg_data[idx] = arr_tx_short_count[n];
	      		}
	      		results->tx_short_dbg_cnt++;
        	}
			  }
#endif

			} // end loop k
		} // end loop j
	} // end loop i
	asm("nop");

#ifdef ALL_TX_SHORT_OUTPUT
	cmdmgr_reply( ctxp, (uint8_t *)(((uint16_t *) replyp)+1),
                 sizeof(uint16_t) + 9 * TEST_PXLS_PER_RX  * sizeof(uint16_t));
#else
	cmdmgr_reply( ctxp, (uint8_t *)(((uint16_t *) replyp)+1),
                 sizeof(uint16_t) + 2 * TEST_PXLS_PER_RX  * sizeof(uint16_t));
#endif              
   
	return -1;                 
}
#endif

static int test_Tx_shorts( uint8_t *replyp )
{
	uint32_t POLREG_ADDR = SCM_POLREG0;		                   //shasta: 0x800040a0;	denali: 0x800020a0
	uint32_t arr_tx_short_count[4];
	uint32_t polreg_data, rd_data, idx, nNumBitsSet;
	uint32_t i, j, k, l, nop_count;
	uint32_t uiTx = 0;
	uint16_t *rp = ( uint16_t * ) replyp + 1;
	
	*(((uint16_t *) replyp) + 1 ) = VCSFW_STATUS_OKAY;
	
	l = 0;                                                   // Tx of Shasta    square: 0...87 rectangle: 0...111

#if 0	                                                         
	wr_word( WOE_SPARE,       0x00000001 );                  
	wr_word( SCM_BG_BIAS_TRIM,0x0000A000 );                  // Current trim
#endif
	
#if 0
	wr_word( RCM_AFE_CLOCK,   0x00000307 );                  // AFE clk = 6MHz, and enable, phase delay = 1.5 CPU clk
	wr_word( RCM_ADC_CLK,     0x00000003 );                  // ADC clk = 12MHz, and enable
	wr_word( RCM_CDM_CLOCK,   0x00000001 );                  // CDM clk = 144MHZ, and enable
	wr_word( RCM_OTHER_CLK,   0x00000002 );                  // GPIO clk enable
#else
#if 0
	wr_word( RCM_AFE_CLOCK,   RCM_AFE_CLOCK_CTRL | 3 << RCM_AFE_CLOCK_FREQ_SEL_B );  // AFE clk = 6MHz, and enable, phase delay = 0.0 CPU clk
	wr_word( RCM_ADC_CLK,     RCM_ADC_CLK_CTRL   | 1 << RCM_ADC_CLK_FREQ_SEL_B   );  // ADC clk = 12MHz, and enable
	wr_word( RCM_CDM_CLOCK,   RCM_CDM_CLOCK_CTRL      );     // CDM clk = 144MHZ, and enable 
	wr_word( RCM_OTHER_CLK,   RCM_OTHER_CLK_GPIO_CTRL );     // GPIO clk enable
#else
	wr_word( RCM_AFE_CLOCK,   RCM_AFE_CLOCK_CTRL | 3 << RCM_AFE_CLOCK_FREQ_SEL_B );  // AFE clk = 6MHz, and enable, phase delay = 0.0 CPU clk 0x00000007
	wr_word( RCM_ADC_CLK,     0x00000000 );                  // Disable ADC clk
	wr_word( RCM_CDM_CLOCK,   0x00000000 );                  // Disable CDM clk
	wr_word( RCM_OTHER_CLK,   0x00000000 );                  // Diaable GPIO clk
#endif
#endif

	wr_word( SCM_SCMIE,       0x00000000 );                  // disable Interrupt
	wr_word( SCM_TESTCFG,     SCM_TESTCFG_TX_SHORTS_TEST | SCM_TESTCFG_TXTM );  // enable Tx short test(actually pull-up all Tx pad), testmode, // shasta: 0x03, denali: 0x07 powerok override 
	if( TEST_PXLS_PER_RX == 112 ) // rectangle sensor
	{
		wr_word( SCM_SCMCFG ,   0x0       );                  // 
	}
	else                          // square sensor
	{
		wr_word( SCM_SCMCFG ,   0x02);                  //  sensor type: square sensor	
	}
	
	wr_word( SCM_LNA_AVREG0,  SCM_LNA_AVREG0_LNA_TEST_EN );  // Enable lna_test_en bit, shasta: 0x00000100 denali: 0x00001000
	wr_word( SCM_LNA_AVREG1,  SCM_LNA_AVREG0_LNA_TEST_EN );  // Enable lna_test_en bit, shasta: 0x00000100 denali: 0x00001000

	// Short all RXs to check TX-TX and TX-RX shorts.
	for( j = 0; j < 80; j++ )                                // Pulldown all RXs
	{                                                        
		wr_word( SCM_LNA_CFG0 + j * 4, SCM_LNA_CFG0_WOF_RX_GND );   //  shasta: 0x00000010 denali: 0x00000200
	} 

	// Loop to cycle through TXs                             
	for( i = 0; i < 7; i++ )                                 // Shasta No. of Tx: 112 == 16 * 7		88 == 16 * 6 - 8
	{
		for( j = 0; j < 8; j++ )                               // 32bit / 2 / k == 8
		{
			// 00 - GND, 01- High
			wr_word( SCM_POLREG0, 0x0 );
			wr_word( SCM_POLREG1, 0x0 );
			wr_word( SCM_POLREG2, 0x0 );
			wr_word( SCM_POLREG3, 0x0 );
			wr_word( SCM_POLREG4, 0x0 );
			wr_word( SCM_POLREG5, 0x0 );
			wr_word( SCM_POLREG6, 0x0 );
	
#if 0	// unnecessary delay, because of non-capacitive load
			// delay how much time? same as denali?
			for( nop_count = 0; nop_count < 1000; nop_count++ ) 
			{
				asm( "nop" );
			}
#endif
	
			for( k = 0; k < 2; k++, l++ )                        // lower 2bits and higher 2bits
			{
				polreg_data = 1 << ( j * 4 + k * 2 );

#if 0		// unnecessary delay, because of non-capacitive load
				// why delay? because we don't have hardware to debug so we must know all aim of unreasonable action that we think.
				for( nop_count = 0; nop_count < 1000; nop_count++ ) 
				{
					asm( "nop" );
				}
#endif
	
				wr_word( POLREG_ADDR + 4 * i, polreg_data );

#if 0		// unnecessary delay, because of non-capacitive load
				// delay how much time? same as denali?
				for( nop_count = 0; nop_count < 1000; nop_count++ )
				{
					asm( "nop" );
				}
#endif

				// what is the functionality of SCM_MASKID? there is no usable infomation in "Shasta Design Specification".
				// the test result is same without the PRE-CHARGE
				// for square sensor, Tx: 12...99
				REG_WRITE32( SCM_MASKID_HI,   0xffffffff            ); //PRE-CHARGE the data bus
				rd_word    ( SCM_TX_SHORTS_0, arr_tx_short_count[0] ); // Tx 0...31
				REG_WRITE32( SCM_MASKID_HI,   0xffffffff            ); //PRE-CHARGE the data bus
				rd_word    ( SCM_TX_SHORTS_1, arr_tx_short_count[1] ); // Tx 32...63
				REG_WRITE32( SCM_MASKID_HI,   0xffffffff            ); //PRE-CHARGE the data bus
				rd_word    ( SCM_TX_SHORTS_2, arr_tx_short_count[2] ); // Tx 64...95
				REG_WRITE32( SCM_MASKID_HI,   0xffffffff            ); //PRE-CHARGE the data bus
				rd_word    ( SCM_TX_SHORTS_3, arr_tx_short_count[3] ); // Tx 96...111

#if 0		// unnecessary delay, because of non-capacitive load
				// why delay?
			  for( nop_count = 0; nop_count < 1000; nop_count++ )
				{
					asm( "nop" );
				}
#endif

				if( TEST_PXLS_PER_RX == 112 ) // Rx = 112, rectangle sensor, Tx: 0...111		
				{
					arr_tx_short_count[3] &= 0x0000ffff;
				}
				else                          // Rx = 88, square sensor, Tx: 12...99
				{
					arr_tx_short_count[0] &= 0xfffff000;
					arr_tx_short_count[3] &= 0x0000000f;
				}
	 
			  // Calculate number of '1'
	      nNumBitsSet = 0;
			  for( idx = 0; idx < 4; idx++ )
			  {
				  rd_data = arr_tx_short_count[ idx ];
				  
				  while( rd_data != 0 )
				  {
				  	rd_data &= ( rd_data - 1 );
				  	nNumBitsSet++;
				  }
			  }
			  
#if 1
#ifdef OUTPUT_WITH_TXRX_NUMBER
				*++rp = uiTx++;
#endif
#ifdef ALL_TX_SHORT_OUTPUT
				*++rp = arr_tx_short_count[0]; 
				*++rp = arr_tx_short_count[0] >> 16;
				*++rp = arr_tx_short_count[1];
				*++rp = arr_tx_short_count[1] >> 16;
				*++rp = arr_tx_short_count[2];
				*++rp = arr_tx_short_count[2] >> 16;
				*++rp = arr_tx_short_count[3];
				*++rp = arr_tx_short_count[3] >> 16;
#else		
				*++rp = nNumBitsSet == 1 ? 0 : nNumBitsSet + 200;
#endif				 
#else
	      //Assign number to results
			  results->tx_short_count[ l ] = nNumBitsSet;
	      results->parameter[ TEST_PARAMETER_QNTY_BIST_TX_SHORT_CHCK ]++;
	
	      // Fail the part if more than a single bit is set for all TX_SHORTS registers
			  if( nNumBitsSet != 1 )
			  {
	      	results->parameter[TEST_PARAMETER_QNTY_BIST_TX_FAIL_SHORT]++;
					SET_STATUS_BIT( TEST_STATUS_FAIL_BIST_TX_SHORT );
	      	
        	if( MAX_QNTY_TX_SHORT_DEBUG_DATA > results->tx_short_dbg_cnt )
        	{
	      		idx = results->tx_short_dbg_cnt * 5;
	      		results->tx_short_dbg_data[idx] = l;
	      		idx++;
	      		for( n = 0; n < 4; n++, idx++ )
	      		{
	      			results->tx_short_dbg_data[idx] = arr_tx_short_count[n];
	      		}
	      		results->tx_short_dbg_cnt++;
        	}
			  }
#endif

			} // end loop k
		} // end loop j
	} // end loop i

	asm("nop");

  // quit Tx short mode
  wr_word( RCM_AFE_CLOCK,   0x00000000 );                  // disable AFE clk
	wr_word( SCM_TESTCFG,     0x00000000 );                  // disable short test, disable testmode (*testmode must be disabled before Rx short test )
	wr_word( SCM_LNA_AVREG0,  0x00000000 );                  // Disable lna_test_en bit, shasta: 0x00000100 
	wr_word( SCM_LNA_AVREG1,  0x00000000 );                  // Disable lna_test_en bit, shasta: 0x00000100 

	return -1;                 
}

// BIST Rx Short Test
/*******************************************************************************************
Rx short test step:
1. Enable AFE clk with 6MHz, enable Iref
2. SCM config: Disable interrupt, Rx short test enable, Charge Pump enable, sensor type set,
							 sensor operation voltage = 3V, LNA test enable, all Tx driver disable, Rx driver enable
3. Drive all Rxs to GND but only 1 to VCC one by one and 
		read the short status register to check whether short.
4. Send out resault
5. Quit Rx short mode
*******************************************************************************************/
static int patch_cmd_handler_test_Rx_shorts( void *ctxp, const uint8_t *cmdbufp, unsigned int cmdlen, uint8_t *replyp )
{
	uint32_t i, j, nop_count; 
  uint32_t rxshort;

	// configuration
#if 0 // Dane
	wr_word( WOE_SPARE,       0x00000001 );                  // *seems only Denali need enable this bit
	wr_word( RCM_AFE_CLOCK,   0x00000307 );                  // AFE clk = 6MHz, and enable, phase delay = 1.5 CPU clk
	wr_word( RCM_ADC_CLK,     0x00000003 );                  // ADC clk = 12MHz, and enable
	wr_word( RCM_CDM_CLOCK,   0x00000001 );                  // CDM CLOCK 144MHZ, and enable 
	wr_word( RCM_OTHER_CLK,   0x00000002 );                  // GPIO clk enable
	wr_word( SCM_BG_BIAS_TRIM, 1<<SCM_BG_BIAS_TRIM_IREF_FORCE_EN_B| 16<<SCM_BG_BIAS_TRIM_IREF_TRIM_B ); //Force enable bandgap
#else
	wr_word( RCM_AFE_CLOCK,   RCM_AFE_CLOCK_CTRL | 3 << RCM_AFE_CLOCK_FREQ_SEL_B );             // AFE clk = 6MHz, and enable, phase delay = 0.0 CPU clk
	wr_word( RCM_ADC_CLK,     0x00000000 );                  // Disable ADC clk
	wr_word( RCM_CDM_CLOCK,   0x00000000 );                  // Disable CDM clk
	wr_word( RCM_OTHER_CLK,   0x00000000 );                  // Diaable GPIO clk
	//wr_word( SCM_BG_BIAS_TRIM, 1<<SCM_BG_BIAS_TRIM_IREF_FORCE_EN_B| 16<<SCM_BG_BIAS_TRIM_IREF_TRIM_B ); //Force enable bandgap ?
	//REG_WRITE32( SCM_MASKID_HI, 0xffffffff );              //precharge ?
	*( (volatile uint32_t *)SCM_BG_BIAS_TRIM ) |= SCM_BG_BIAS_TRIM_IREF_FORCE_EN;               // enable Iref ?
#endif
	
	// Force enable CP to enable Imirror  <==== but why force enable frame start signal for AFE? after debug, the frame start bit can be disabled
	wr_word( SCM_ADC_RAMP_GEN0, SCM_ADC_RAMP_GEN0_BIAS_F_EN | SCM_ADC_RAMP_GEN0_FS_F_EN );
	wr_word( SCM_ADC_RAMP_GEN1, SCM_ADC_RAMP_GEN0_BIAS_F_EN | SCM_ADC_RAMP_GEN0_FS_F_EN );
	wr_word( SCM_AFE_SATDET0, SCM_AFE_SATDET0_RX_SHORT_EN ); // Enable rx_short_en bit, shasta: 0x00000100 denali: 0x00008000
	wr_word( SCM_AFE_SATDET1, SCM_AFE_SATDET0_RX_SHORT_EN ); // Enable rx_short_en bit, shasta: 0x00000100 denali: 0x00008000
	wr_word( SCM_LNA_AVREG0, SCM_LNA_AVREG0_LNA_VREG_FORCE_EN | SCM_LNA_AVREG0_LNA_TEST_EN );   // Enable lna_test_en bit, shasta: 0x00000100 denali: 0x00001000
	wr_word( SCM_LNA_AVREG1, SCM_LNA_AVREG0_LNA_VREG_FORCE_EN | SCM_LNA_AVREG0_LNA_TEST_EN );   // Enable lna_test_en bit, shasta: 0x00000100 denali: 0x00001000

	// Disable 0 ~ 87 Tx driver 
	*( ( volatile unsigned int *)SCM_TXCFG0 ) = 0x00000000;
	*( ( volatile unsigned int *)SCM_TXCFG1 ) = 0x00000000;
	*( ( volatile unsigned int *)SCM_TXCFG2 ) = 0x00000000;
	*( ( volatile unsigned int *)SCM_TXCFG3 ) = 0x00000000;

	// always treat as 80 Rxs
	// Enable 0 ~ 79 Rx receiver 
	*( ( volatile unsigned int *)SCM_RXCFG0 ) = 0x0FFFFFFFF;
	*( ( volatile unsigned int *)SCM_RXCFG1 ) = 0x0FFFFFFFF;
	*( ( volatile unsigned int *)SCM_RXCFG2 ) = 0x00000FFFF;
	wr_word( SCM_SCMCFG ,     0x02 );               // sensor type: square sensor
	
#if 0 // Dane
	results->parameter[ TEST_PARAMETER_QNTY_BIST_RX_SHORT_CHCK ] = TEST_QNTY_RX;
#else
	*(((uint16_t *) replyp) + 1 ) = VCSFW_STATUS_OKAY;
	uint16_t *rp = ( uint16_t * ) replyp + 1;
	unsigned int uiRx = 0;
#endif
		
	// Rx short check one by one
	for( i = 0; i < 80; i++ )
	{
		for( j = 0; j < 80; j++ )                              // Pulldown all RXs
		{                                                      
			wr_word( SCM_LNA_CFG0 + j * 4, SCM_LNA_CFG0_WOF_RX_GND | SCM_LNA_CFG0_FORCE_DIS_RX_VCM );      // shasta: 0x00000010 denali: 0x00000200
		}                                                      

		wr_word( SCM_LNA_CFG0 +   i       * 4, SCM_LNA_CFG0_RX_PU | SCM_LNA_CFG0_FORCE_DIS_RX_VCM );     //Pullup one Rx
		//wr_word( SCM_LNA_CFG0 + ( i + 1 ) * 4, SCM_LNA_CFG0_RX_PU );   //Pullup one Rx

#if 1
		for( nop_count = 0; nop_count < 100; nop_count++ )
		{
			asm("nop");
		}
#endif		
		//REG_WRITE32(SCM_MASKID_HI, 0xffffffff); //PRE-CHARGE the data bus (THIS CRASHED THE PATCH)
		rd_word( SCM_TX_SHORTS_3, rxshort );

    /* Mask off and shift the bits of interest to lower byte */ 
    // shasta has button_scan function and accordingly with TX_BUTTON_SHORT bit in this registor, so is it need to open this bit to test?
		rxshort = ( rxshort >> 16 ) & 0x3;                               // 0x7 if enable TX_BUTTON_SHORT
		
#if 0		// Dane
		results->rx_short_data[ i ] = ( uint8_t )rxshort;
			
		/* Fail if rx_short  */
		// seems 0: short, 1: non-short
		if( 0x03 != rxshort )
		{
			SET_STATUS_BIT( TEST_STATUS_FAIL_BIST_RX_SHORT );
			results->parameter[ TEST_PARAMETER_QNTY_BIST_RX_FAIL_SHORT ]++;
		}
#else
#ifdef OUTPUT_WITH_TXRX_NUMBER		     // debug output
		*++rp = uiRx++;
		//*++rp = rxshort;
		*++rp = ( ( rxshort == 1 ) || ( rxshort == 2 ) ) ? 0 : ( 200 + rxshort );
#else                                  // release output
		*++rp = ( ( rxshort == 1 ) || ( rxshort == 2 ) ) ? 0 : ( 200 + rxshort );
#endif			
#endif
	}

	// send resault to host
	if( TEST_QNTY_RX == 80 )             // square sensor, Rx: 0...79
	{
#ifdef OUTPUT_WITH_TXRX_NUMBER
	// debug output
	cmdmgr_reply( ctxp, ( uint8_t* )((( uint16_t* ) replyp ) + 1 ), sizeof( uint16_t ) + 2 * TEST_QNTY_RX  * sizeof( uint16_t ) );                 
#else
	// release output
	cmdmgr_reply( ctxp, ( uint8_t* )((( uint16_t* ) replyp ) + 1 ), sizeof( uint16_t ) + 1 * TEST_QNTY_RX  * sizeof( uint16_t ) );
#endif
	}
	else                                 // rectangle sensor, Rx: 12...67
	{
#ifdef OUTPUT_WITH_TXRX_NUMBER         // debug output
	*((( uint16_t* ) replyp ) + 1 + 12 * 2 ) = *((( uint16_t* ) replyp ) + 1 );  // copy VCSFW_STATUS to Rx11
	cmdmgr_reply( ctxp, ( uint8_t* )((( uint16_t* ) replyp ) + 1 + 12 * 2 ), sizeof( uint16_t ) + 2 * TEST_QNTY_RX  * sizeof( uint16_t ) );                 
#else                                  // release output
	*((( uint16_t* ) replyp ) + 1 + 12 ) = *((( uint16_t* ) replyp ) + 1 );      // copy VCSFW_STATUS to Rx11
	cmdmgr_reply( ctxp, ( uint8_t* )((( uint16_t* ) replyp ) + 1 + 12 ), sizeof( uint16_t ) + 1 * TEST_QNTY_RX  * sizeof( uint16_t ) );
#endif
	}
	// quit Rx short mode
  wr_word( RCM_AFE_CLOCK,     0x00000000 );                // disable AFE clk
	wr_word( SCM_LNA_AVREG0,    0x00000000 );                // Disable lna_test_en bit, shasta: 0x00000100 
	wr_word( SCM_LNA_AVREG1,    0x00000000 );                // Disable lna_test_en bit, shasta: 0x00000100 
	wr_word( SCM_ADC_RAMP_GEN0, 0x00000000 );                // Force disable CP to disable Imirror
	wr_word( SCM_ADC_RAMP_GEN1, 0x00000000 );                // Force disable CP to disable Imirror
	wr_word( SCM_AFE_SATDET0,   0x00000000 );                // Disable rx_short_en bit, shasta: 0x00000100 
	wr_word( SCM_AFE_SATDET1,   0x00000000 );                // Disable rx_short_en bit, shasta: 0x00000100 

	return -1;
}

/* -------------------------------------------------------------------------- */
/* Internal functions */




//static void 
//program_gain(uint32_t j, uint32_t k, uint32_t m) {
//    wr_word(SCM_WOF_GAIN, j << SCM_WOF_GAIN_WOF_AMP_2_GAIN_B |
//        k << SCM_WOF_GAIN_WOF_AMP_3_GAIN_B |
//        m << SCM_WOF_GAIN_WOF_AMP_4_GAIN_B |
//        1 << SCM_WOF_GAIN_WOF_CAL_POLARITY_B      // polarity bit should be 1 when doing subtraction
//    );
//}



/* -------------------------------------------------------------------------- */
/* UNIT_TEST functions */


#if UNIT_TEST
/*
 * Handler for VCSFW_CMD_PEEK.
 */
static int
patch_cmd_handler_peek(void *ctxp, const uint8_t *cmdbufp,
                       unsigned int cmdlen, uint8_t *replyp)
{
    uint32_t                         value;
    const fwcmd_cmd_peek_t       *ecpp;

    ecpp = (const fwcmd_cmd_peek_t *) cmdbufp;

    switch (ecpp->opsize) {
    case 1:
        value = (uint32_t) *((volatile uint8_t *) ecpp->address);
        break;

    case 2:
        value = (uint32_t) *((volatile uint16_t *) ecpp->address);
        break;

    default:
        value = (uint32_t) REG_READ32(ecpp->address);
        break;
    }

    *(((uint16_t *) replyp)+1) = VCSFW_STATUS_OKAY;
    *(((uint32_t *) replyp)+1) = value;

    cmdmgr_reply(ctxp, (uint8_t *)(((uint16_t *) replyp)+1),
                 sizeof(uint16_t) + sizeof(uint32_t));

    return -1;
}
    
/*
 * Handler for VCSFW_CMD_POKE.
 */
static int
patch_cmd_handler_poke(void *ctxp, const uint8_t *cmdbufp,
                       unsigned int cmdlen, uint8_t *replyp)
{
    const fwcmd_cmd_poke_t       *ecpp;

    ecpp = (const fwcmd_cmd_poke_t *) cmdbufp;

    switch (ecpp->opsize) {
    case 1:
        *((volatile uint8_t *) ecpp->address) = (uint8_t) ecpp->value;
        break;

    case 2:
        *((volatile uint16_t *) ecpp->address) = (uint16_t) ecpp->value;
        break;

    default:
        REG_VARWRITE32(ecpp->address, ecpp->value);
        break;
    }

    return VCSFW_STATUS_OKAY;
}
#endif
