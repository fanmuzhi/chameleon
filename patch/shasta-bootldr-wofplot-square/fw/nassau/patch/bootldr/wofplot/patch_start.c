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


#include "basetypes.h"
#include "bootldr.h"
#include "hwdef.h"
#include "hwreg.h"
#include "reg.h"
#include "fwcmd.h"
#include "cmdmgr.h"
#include "wofplot.h"


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
/* PIF Definition*/

#define  PIF_MON_VGO              0x80003FF0
#define  PIF_MON_VEGO_REG_ADDR    0x80003FF0
#define  PIF_MON_VEGO_SYNC_ADDR   0x80003FF8

/* -------------------------------------------------------------------------- */
/* WOF Gain Settings*/

//typedef struct wof_amp_gain_s
//{
//    uint8_t    amp2;  
//    uint8_t    amp3;
//    uint8_t    amp4;
//    uint8_t    reserved;    
//    uint16_t   value;
//}wof_amp_gain_t;
//
//
//static wof_amp_gain_t   wof_gain_setting[WOF_GAIN_MAX_IDX] = 
//{
//    {.amp2=0,   .amp3=2,    .amp4=0,  .value=0x810},    /* Normal Gain */
//    {.amp2=0,   .amp3=3,    .amp4=0,  .value=0x818},    /* Lower Gain  */
//    {.amp2=0,   .amp3=4,    .amp4=0,  .value=0x820},    /* Low Gain    */
//    {.amp2=0,   .amp3=5,    .amp4=0,  .value=0x828},    /* High Gain   */
//    {.amp2=0,   .amp3=6,    .amp4=0,  .value=0x830},    /* Higer Gain  */
//};


/* -------------------------------------------------------------------------- */
/* Static prototypes */

static int patch_cmd_handler_wof_baseline(void *ctxp, const uint8_t *cmdbufp, 
                                                       unsigned int cmdlen, uint8_t *replyp);
static int patch_cmd_handler_wof_signal(void *ctxp, const uint8_t *cmdbufp, 
                                                    unsigned int cmdlen, uint8_t *replyp);

static void program_wof_setting();
static void get_dca_offset(uint32_t* offset);
static void get_signal_level_direct(uint32_t* signal);
//static void program_gain(uint32_t j, uint32_t k, uint32_t m);
static void sleep(uint32_t times);


#if UNIT_TEST
static int patch_cmd_handler_peek(void *ctxp, const uint8_t *cmdbufp, 
                                           unsigned int cmdlen, uint8_t *replyp);
static int patch_cmd_handler_poke(void *ctxp, const uint8_t *cmdbufp, 
                                            unsigned int cmdlen, uint8_t *replyp);
#endif

/* -------------------------------------------------------------------------- */
/* Static initialized data  */

static cmdmgr_cmdentry_t patch_cmdtable[] = {
    { VCSFW_CMD_WOF_BASELINE, &patch_cmd_handler_wof_baseline },
    { VCSFW_CMD_WOF_SIGNAL, &patch_cmd_handler_wof_signal },
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

/* -------------------------------------------------------------------------- */
/* Static functions */

    
/*
 * Handler for VCSFW_CMD_WOF_BAELINE.
 */
static int
patch_cmd_handler_wof_baseline(void *ctxp, const uint8_t *cmdbufp,
                             unsigned int cmdlen, uint8_t *replyp)
{
    int status = VCSFW_STATUS_OKAY;
    uint32_t iter_gain;
    uint32_t offset = 0x55AA55AA;

    const vcsfw_cmd_wofbaseline_t   *ecpp;
    ecpp = (const vcsfw_cmd_wofbaseline_t*) cmdbufp;

    program_wof_setting();

    //GPIO 0 Control
    //wr_word(GPIO_DIR_CTRL, 0x00000000);
    //wr_word(GPIO_PULLUP_CTRL, 0x00000000);
    //wr_word(GPIO_INTR_CTRL, 0x00000000);

    *(((uint16_t *) replyp)+1) = VCSFW_STATUS_OKAY;

    uint16_t *rp = (uint16_t *) replyp + 1;

    //for(iter_gain=0; iter_gain< WOF_GAIN_MAX_IDX; iter_gain++)
    //{
    //    //program gain
    //    wr_word(SCM_WOF_GAIN, wof_gain_setting[iter_gain].value);

    //    //get offset
    //    get_dca_offset(&offset);

    //    *++rp = wof_gain_setting[iter_gain].value;
    //    *++rp = (uint16_t)offset;
    //}
   
    //program gain
    wr_word(SCM_WOF_GAIN, ecpp->gain_value);

    //get offset
    get_dca_offset(&offset);

    //reply data
    *++rp = ecpp->gain_value;
    *++rp = (uint16_t)offset;

    //    *++rp = wof_gain_setting[iter_gain].value;

    cmdmgr_reply(ctxp, (uint8_t *)(((uint16_t *) replyp)+1),
                 sizeof(uint16_t) + sizeof(vcsfw_reply_wofbaseline_t));
    
    return -1;
}

static int
patch_cmd_handler_wof_signal(void *ctxp, const uint8_t *cmdbufp,
                             unsigned int cmdlen, uint8_t *replyp)
{
    int status = VCSFW_STATUS_OKAY;

    uint32_t iter_gain;
    uint32_t signal = 0x0;

    const vcsfw_cmd_wofsignal_t   *ecpp;
    ecpp = (const vcsfw_cmd_wofsignal_t*) cmdbufp;

    program_wof_setting();

    *(((uint16_t *) replyp)+1) = VCSFW_STATUS_OKAY;
    uint16_t *rp = (uint16_t *) replyp + 1;

    //for(iter_gain=0; iter_gain< WOF_GAIN_MAX_IDX; iter_gain++)
    //{
    //    //program gain
    //    wr_word(SCM_WOF_GAIN, ecpp->gains[iter_gain]);
    //    
    //    //program DCA offset
    //    wr_word(WOE_WOF_DCA_OFFSET, ecpp->offsets[iter_gain] << WOE_WOF_DCA_OFFSET_RXP_B | 
    //                                ecpp->offsets[iter_gain] << WOE_WOF_DCA_OFFSET_RXM_B);       

    //    //get signal with gain and DCA offset
    //    get_signal_level_direct(&signal);

    //    *++rp = (uint16_t)signal;

    //}
    //
    //program gain
    wr_word(SCM_WOF_GAIN, ecpp->gain_value);

    //program DCA offset
    wr_word(WOE_WOF_DCA_OFFSET, ecpp->offsets << WOE_WOF_DCA_OFFSET_RXP_B | 
                                ecpp->offsets << WOE_WOF_DCA_OFFSET_RXM_B);       

    get_signal_level_direct(&signal);

    *++rp = ecpp->gain_value;
    *++rp = ecpp->offsets;
    *++rp = (uint16_t)signal;

    cmdmgr_reply(ctxp, (uint8_t *)(((uint16_t *) replyp)+1),
                 sizeof(uint16_t) + sizeof(vcsfw_reply_wofsignal_t));

    return -1;
}


/* -------------------------------------------------------------------------- */
/* Internal functions */

static void
program_wof_setting()
{   
    uint32_t rd_data;

    //POR release
    wr_word(WOE_HV_DPOR, 0xA57E5A69);

    wr_word(WOE_STARTUP_DELAY, 0x00020010);   // cpu fast clock
    wr_word(RCM_AFE_CLOCK, 0x00000007);
    wr_word(RCM_ADC_CLK, 0x00000001);
    wr_word(RCM_CDM_CLOCK, 0x00000001);      // CDM CLOCK 144MHZ 
    wr_word(RCM_OTHER_CLK, 0x00000002);
    wr_word(WOE_SPARE, 0x00000001);

	/*
    * Read trim code from Flash
    */
    //Fast clock
	rd_word(HWDEF_FIB_FASTCLK, rd_data);
	wr_word(WOE_HS_OSC_TRIM, rd_data);
	//Bandgap trim code
	rd_word(HWDEF_FIB_BGTRIM, rd_data);
	wr_word(SCM_BG_BIAS_TRIM, rd_data);
	//Voltage trim
	rd_word(HWDEF_FIB_VTRIM, rd_data);
	wr_word(SCM_LNA_AVREG0, rd_data);
	wr_word(SCM_LNA_AVREG1, rd_data);
	//Slow clock
	rd_word(HWDEF_FIB_SLOWCLK, rd_data);
	wr_word(SCM_SOSC_BIAS_TRIM, 0x2D);		//workaround for primax

	//rd_word(HWDEF_FIB_SLOWCLK, rd_data);
    //wr_word(SCM_SOSC_BIAS_TRIM, rd_data);


    wr_word(PIF_MON_VEGO_REG_ADDR, 0x000000001);

	/*
     *program TX RX for WOF
     */
    wr_word(SCM_LNA_CFG16, SCM_LNA_CFG16_WOF_TX_SEL);   // TX+ = 16
    wr_word(SCM_LNA_CFG19, SCM_LNA_CFG19_WOF_TX_SEL);   // TX+ = 19
    wr_word(SCM_LNA_CFG25, SCM_LNA_CFG25_WOF_TX_SEL);   // TX+ = 25
    wr_word(SCM_LNA_CFG28, SCM_LNA_CFG28_WOF_TX_SEL);   // TX+ = 28

    wr_word(SCM_LNA_CFG22, SCM_LNA_CFG22_WOF_RX_P_SEL 
            | (SCM_LNA_CFG22_LNA_GAIN & (SCM_LNA_CFG_LNA_GAIN_GAIN_17 << SCM_LNA_CFG22_LNA_GAIN_B))); // Rx+ = 22
    wr_word(SCM_LNA_CFG58, SCM_LNA_CFG58_WOF_RX_N_SEL 
            | (SCM_LNA_CFG58_LNA_GAIN & (SCM_LNA_CFG_LNA_GAIN_GAIN_17 << SCM_LNA_CFG58_LNA_GAIN_B))); // Rx- = 58 

    wr_word(SCM_LNA_CFG52, SCM_LNA_CFG52_WOF_TX_SEL | SCM_LNA_CFG52_WOF_TX_POL);   // TX- = 52
    wr_word(SCM_LNA_CFG55, SCM_LNA_CFG55_WOF_TX_SEL | SCM_LNA_CFG55_WOF_TX_POL);   // TX- = 55
    wr_word(SCM_LNA_CFG61, SCM_LNA_CFG61_WOF_TX_SEL | SCM_LNA_CFG61_WOF_TX_POL);   // TX- = 61
    wr_word(SCM_LNA_CFG64, SCM_LNA_CFG64_WOF_TX_SEL | SCM_LNA_CFG64_WOF_TX_POL);   // TX- = 64

    //Enable IO
    wr_word(WOE_CTRL, WOE_CTRL_IO_ENABLE);

	
    wr_word(WOE_WOF_CTRL1, 20 << WOE_WOF_CTRL1_SAMPLE_INTERVAL_B |   // Time between consecutive WoFs, 10us LSB, Min value 7 
        0 << WOE_WOF_CTRL1_FD_HIGH_POLARITY_B |   // Changes polarity. Set to 0.
        0 << WOE_WOF_CTRL1_MODE_B   // Modes to trigger interrupt. 00- FD, FU or FH, 01 - FD, 10 - FU
    );

    wr_word(WOE_WOF_CTRL2, 0 << WOE_WOF_CTRL2_FD_ACCUM_B |      // Accumulation mode for noise.
        0 << WOE_WOF_CTRL2_FU_ACCUM_B |         // Accumulation mode for noise
        1 << WOE_WOF_CTRL2_FD_SAMPLES_B |       // No of FD samples
        1 << WOE_WOF_CTRL2_FU_SAMPLES_B |       // No of FU samples
        1 << WOE_WOF_CTRL2_FH_SAMPLES_B);       // No of FHH samples

    wr_word(WOE_WOF_THRESH, 8 << WOE_WOF_THRESH_LOW_COUNT_B |   // 1- 15, number of comparator edges to detect FD
        8 << WOE_WOF_THRESH_HIGH_COUNT_B |      // 1- 15, number of comparator edges to detect FH
        8 << WOE_WOF_THRESH_HIGH_HIGH_COUNT_B   // 1- 15, number of comparator edges to detect FHH
    );

    wr_word(WOE_WOF_DCA_CTRL1, 1 << WOE_WOF_DCA_CTRL1_MAIN_EN_B |       // Enable calibration
        0 << WOE_WOF_DCA_CTRL1_RXP_DEC_EN_B |       // RXP cal decrease enable
        0 << WOE_WOF_DCA_CTRL1_RXP_INC_EN_B |       // RXP cal increase enable
        0 << WOE_WOF_DCA_CTRL1_RXM_DEC_EN_B |       // RXM cal decrease enable
        0 << WOE_WOF_DCA_CTRL1_RXM_INC_EN_B |       // RXM cal increase enable
        0 << WOE_WOF_DCA_CTRL1_RXP_FAST_DEC_EN_B |  // RXP cal fast decrease enable
        0 << WOE_WOF_DCA_CTRL1_RXP_FAST_INC_EN_B |  // RXP cal fast increase enable
        0 << WOE_WOF_DCA_CTRL1_RXM_FAST_DEC_EN_B |  // RXM cal fast decrease enable
        0 << WOE_WOF_DCA_CTRL1_RXM_FAST_INC_EN_B    // RXM cal fast increase enable
    );

    wr_word(WOE_WOF_DCA_CTRL2, 1 << WOE_WOF_DCA_CTRL2_COUNT_B |     // Number of wof cycles to update cal value
        0 << WOE_WOF_DCA_CTRL2_FD_UPDATE_EN_B); // Enable or disable cal update during FD.

                                                //wr_word(WOE_WOF_DCA_OFFSET,  450<<WOE_WOF_DCA_OFFSET_RXP_B |     // Initial cdac cal value
                                                //                             450<<WOE_WOF_DCA_OFFSET_RXM_B);

    wr_word(WOE_WOF_DCA_THRESH, 8 << WOE_WOF_DCA_THRESH_LO_CNT_B |
        8 << WOE_WOF_DCA_THRESH_HI_CNT_B |
        4 << WOE_WOF_DCA_THRESH_FAST_LO_CNT_B |
        4 << WOE_WOF_DCA_THRESH_FAST_HI_CNT_B
    );

    wr_word(WOE_WOF_DCA_STEP, 1 << WOE_WOF_DCA_STEP_RXP_FAST_DEC_B |
        1 << WOE_WOF_DCA_STEP_RXP_FAST_INC_B |
        1 << WOE_WOF_DCA_STEP_RXP_SLOW_DEC_B |
        1 << WOE_WOF_DCA_STEP_RXP_SLOW_INC_B |
        1 << WOE_WOF_DCA_STEP_RXM_FAST_DEC_B |
        1 << WOE_WOF_DCA_STEP_RXM_FAST_INC_B |
        1 << WOE_WOF_DCA_STEP_RXM_SLOW_DEC_B |
        1 << WOE_WOF_DCA_STEP_RXM_SLOW_INC_B);

    //Program RETENTION reg, enable 1.2V regulators.
    wr_word(WOE_RETENTION, WOE_RETENTION_LNA_BL_RAM | WOE_RETENTION_FW_BL_RAM);

    //wof and full power mode
    wr_word(WOE_MODE, WOE_MODE_WOF | WOE_MODE_POWER_FULL);


	wr_word(SCM_WOF_TESTMUX, 0x00000004);  // wof amp4 output
	wr_word(SCM_BG_BIAS_TRIM, 0x00008000); //Force enable bandgap
	wr_word(SCM_TEST_MUX_RIGHT, 0x00000005); //Enable test mux, bypass test buffer, id=88/0x58
	wr_word(WOE_TEST_E_N_CTRL, 0x0000003B); //Setup test_e_n pad for analog out
	wr_word(SCM_ADC_RAMP_GEN0, 0x00000200);


	
}


static void 
get_dca_offset(uint32_t* offset) {
    uint32_t LOW = 1;
    uint32_t HIGH = 0;
    uint32_t current = 0;
    uint32_t previous = 0;
    uint32_t sp = 1;
    uint32_t rd_data;
    uint32_t iter_counts =0;
    uint32_t error_counts = 0;
    
    const uint32_t iteration_limit   = 2048;

    wr_word(SCM_WOF_THRESH1, WOF_BASELINE<<SCM_WOF_THRESH1_BASE_THD_B | 125<<SCM_WOF_THRESH1_FINGER_THD_H_B ); 
    wr_word(SCM_WOF_THRESH0, 40<<SCM_WOF_THRESH0_FINGER_THD_L_B | 15<<SCM_WOF_THRESH0_FINGER_THD_L_HYST_B );  

    wr_word(WOE_WOF_DCA_OFFSET,  sp<<WOE_WOF_DCA_OFFSET_RXP_B |     // Initial cdac cal value
                                 sp<<WOE_WOF_DCA_OFFSET_RXM_B);

    //clear all events
    wr_word(WOE_EVENT, (WOE_EVENT_INTR
                      | WOE_EVENT_EV_TIMER
                      | WOE_EVENT_EV_SPI
                      | WOE_EVENT_EV_WOF
                      | WOE_EVENT_EV_ABORT));

    while((current == previous) && (iter_counts < iteration_limit) && (sp!=0)) {

        //enable WOE notification
        wr_word(WOE_CTRL, WOE_CTRL_START | WOE_CTRL_IO_ENABLE);
        sleep(256);

        //wait for WOF event
        rd_word(WOE_EVENT, rd_data);
        while(REG_BITISOFF_B(rd_data, WOE_EVENT_EV_WOF_B))
        {
            rd_word(WOE_EVENT, rd_data);
        }
 
        sleep(10000);

        //write 1 to event to clear the event.
        wr_word(WOE_EVENT,rd_data); 

        //check DCA status
        rd_word(WOE_WOF_DCA_STATUS, rd_data);
        if(REG_BITISON_B(rd_data, WOE_WOF_DCA_STATUS_LO_DET_B))
        {
            previous = current;
            current = LOW;
            if(iter_counts==0) previous = current;
            sp--;
            wr_word(WOE_WOF_DCA_OFFSET,  sp<<WOE_WOF_DCA_OFFSET_RXP_B |     // Initial cdac cal value
                                         sp<<WOE_WOF_DCA_OFFSET_RXM_B);
        }    
        else if(REG_BITISON_B(rd_data, WOE_WOF_DCA_STATUS_HI_DET_B))
        {
            previous = current;
            current = HIGH;
            if(iter_counts==0) previous = current;
            sp++;
            wr_word(WOE_WOF_DCA_OFFSET,  sp<<WOE_WOF_DCA_OFFSET_RXP_B |     // Initial cdac cal value
                                         sp<<WOE_WOF_DCA_OFFSET_RXM_B);
        }
        else
        {
            //error
            if(error_counts++ >iteration_limit) break;
        }
        iter_counts++;

        sleep(256);
    }    
    *offset = sp;

	//stop wof notification
	wr_word(WOE_CTRL, 0);	

}


static void
get_signal_level_direct(uint32_t* signal){
    uint32_t th;
    uint32_t rd_data;
    
    //clear all event
    wr_word(WOE_EVENT, (WOE_EVENT_INTR
                      | WOE_EVENT_EV_TIMER
                      | WOE_EVENT_EV_SPI
                      | WOE_EVENT_EV_WOF
                      | WOE_EVENT_EV_ABORT));
     
    for(th=0; th<128; th++)
    {
        wr_word(SCM_WOF_THRESH1, th<<SCM_WOF_THRESH1_BASE_THD_B | 120<<SCM_WOF_THRESH1_FINGER_THD_H_B );
        wr_word(SCM_WOF_THRESH0, 10<<SCM_WOF_THRESH0_FINGER_THD_L_B | 5<<SCM_WOF_THRESH0_FINGER_THD_L_HYST_B );

	   	//start wof notification
        wr_word(WOE_CTRL, WOE_CTRL_START | WOE_CTRL_IO_ENABLE);
        sleep(256);

        rd_word(WOE_EVENT,rd_data);
        while(REG_BITISOFF_B(rd_data,  WOE_EVENT_EV_WOF_B))
        {
            //if no WOF event, keep polling
            rd_word(WOE_EVENT,rd_data);
        }

        sleep(10000);
        
        //write 1 to event to clear the event.
        wr_word(WOE_EVENT,rd_data);

        //check DCA status
        rd_word(WOE_WOF_DCA_STATUS,rd_data);
        if( REG_BITISON_B(rd_data, WOE_WOF_DCA_STATUS_LO_DET_B ) )
        {
            //if detect low WOF event
            *signal = th;
            break;
        }    

        sleep(256);
    } 


	//stop wof notification
	wr_word(WOE_CTRL, 0);

	
}

//static void 
//program_gain(uint32_t j, uint32_t k, uint32_t m) {
//    wr_word(SCM_WOF_GAIN, j << SCM_WOF_GAIN_WOF_AMP_2_GAIN_B |
//        k << SCM_WOF_GAIN_WOF_AMP_3_GAIN_B |
//        m << SCM_WOF_GAIN_WOF_AMP_4_GAIN_B |
//        1 << SCM_WOF_GAIN_WOF_CAL_POLARITY_B      // polarity bit should be 1 when doing subtraction
//    );
//}


static void 
sleep(uint32_t times){
    uint32_t i=0;
    for(i=0; i<times; i++){
        asm("nop");
    }
}


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
