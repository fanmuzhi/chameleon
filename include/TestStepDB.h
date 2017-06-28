#pragma once

#include <stdint.h>
#include <string.h>
#include <vector>

using namespace std;

#define TEST_STEP_ID_INITIALIZATIONSTEP                1001  
#define TEST_STEP_ID_RUNATEPATCH                       1002
#define TEST_STEP_ID_UPDATEFIB                         1003
#define TEST_STEP_ID_VERIFYOSCTRIM                     1004
#define TEST_STEP_ID_VERIFYSENSORCOMM                  1005

#define TEST_STEP_ID_RAMTEST                           1006
#define TEST_STEP_ID_ROMTEST                           1007
#define TEST_STEP_ID_OPENSSHORTSTEST                   1008
#define TEST_STEP_ID_WOF_BASELINE                      1009
#define TEST_STEP_ID_CALIBRATE                         1010
#define TEST_STEP_ID_BUTTONTESTWITHOUTSTIMULUS         1011
#define TEST_STEP_ID_ACQIMGNOFINGER                    1012
#define TEST_STEP_ID_CPACQIMGDARK                      1013
#define TEST_STEP_ID_CPACQIMGDARKLONG                  1014
#define TEST_STEP_ID_CPACQIMGREFLOW                    1015
#define TEST_STEP_ID_DRDYTEST                          1016
#define TEST_STEP_ID_SYNCHRONIZE                       1017
#define TEST_STEP_ID_PROGRAMMINGMISSIONFIRMWARE        1018
#define TEST_STEP_ID_PROGRAMMINGIOTA_BIN               1019

#define TEST_STEP_ID_WOF_SIGNAL                        2001
#define TEST_STEP_ID_BUTTONTESTWITHSTIMULUS            2002
#define TEST_STEP_ID_ACQIMGFINGER                      2003
#define TEST_STEP_ID_CPACQIMGSTIMULUS                  2004

#define TEST_STEP_ID_CPACQIMGREFHIGH                   3001
#define TEST_STEP_ID_CPACQIMGROI                       3002

#define TEST_STEP_ID_CURRENTTEST                       4001
#define TEST_STEP_ID_SLEEPCURRENTTEST                  4002
#define TEST_STEP_ID_WOFCURRENTTEST                    4003

#define TEST_STEP_ID_HUAWEIIMAGEQUALITYTEST            5001
#define TEST_STEP_ID_CPDARKBADPIXELTEST                5002
#define TEST_STEP_ID_CPBADPIXELTEST                    5003
#define TEST_STEP_ID_SHARPNESSTEST                     5004
#define TEST_STEP_ID_BUBBLETEST                        5005
#define TEST_STEP_ID_PIXELUNIFORMITYTEST               5006
#define TEST_STEP_ID_RXTXSTANDARDDEV                   5007 
#define TEST_STEP_ID_IMPERFECTION                      5008 
#define TEST_STEP_ID_SNRTEST                           5009 
#define TEST_STEP_ID_CPIMAGEQUALITY                    5010 

#define TEST_STEP_ID_IOTACHECK                         6001 
#define TEST_STEP_ID_MISSIONFIRMWARECHECK              6002 
#define TEST_STEP_ID_PROGRAMMINGIOTA_DATA              6003                                 
#define TEST_STEP_ID_FINALIZATIONSTEP                  9001 

typedef struct test_step_db_s                   
{                                               
    uint32_t   id;                              
    string     name;                            
    string     arg_names;                       
    string     args;                            
    string     args_desc;                       
    string     description;
    uint8_t    reserved;    
}test_step_db_t;

static vector<test_step_db_t> TestStepDB = 
{
    /* initalize */
    // id                                           name                                arg_names,                                                                                                  args,                  args_desc       description
    {  TEST_STEP_ID_INITIALIZATIONSTEP,             "InitializationStep",               "",                                                                                                         "",                    "",             "initialize sensor comm"            },
    {  TEST_STEP_ID_RUNATEPATCH,                    "RunATEPatch",                      "",                                                                                                         "",                    "",             "ATE Patch for OSCTrim workaround"  },
    {  TEST_STEP_ID_UPDATEFIB,                      "UpdateFIB",                        "",                                                                                                         "",                    "",             "resize the flash"                  },
    {  TEST_STEP_ID_VERIFYOSCTRIM,                  "VerifyOSCTrim",                    "",                                                                                                         "",                    "",             "verify OSC trim value exists"      },
    {  TEST_STEP_ID_VERIFYSENSORCOMM,               "VerifySensorComm",                 "",                                                                                                         "",                    "",             "verify comm to extension sensor"   },

    /* no stimulus test steps */
    // id                                           name                                arg_names,                                                                                                  args,                  args_desc       description
    {  TEST_STEP_ID_RAMTEST,                        "RAMTest",                          "Delay",                                                                                                    "500",                 "mSec",         "RAM test with patch"               },
    {  TEST_STEP_ID_ROMTEST,                        "ROMTest",                          "Delay",                                                                                                    "500",                 "mSec",         "ROM test with patch"               },
    {  TEST_STEP_ID_OPENSSHORTSTEST,                "OpensShortsTest",                  "ReponseSize;Delay",                                                                                        "2056;500",            ";mSec",        "Open Short test with patch"        },
    {  TEST_STEP_ID_WOF_BASELINE,                   "WOF_Baseline",                     "",                                                                                                         "",                    "",             "WOF baseline calibration"          },
    {  TEST_STEP_ID_CALIBRATE,                      "Calibrate",                        "",                                                                                                         "",                    "",             "Image baseline cabliration"        },
    {  TEST_STEP_ID_BUTTONTESTWITHOUTSTIMULUS,      "ButtonTestWithoutStimulus",        "",                                                                                                         "",                    "",             "Check GPIO of button click"        },
    {  TEST_STEP_ID_ACQIMGNOFINGER,                 "AcqImgNoFinger",                   "frames",                                                                                                   "30",                  "",             "acquire images from sensor"        },
    {  TEST_STEP_ID_CPACQIMGDARK,                   "CpAcqImgDark",                     "frames",                                                                                                   "30",                  "",             "acquire images from sensor"        },
    {  TEST_STEP_ID_CPACQIMGDARKLONG,               "CpAcqImgDarkLong",                 "frames",                                                                                                   "30",                  "",             "acquire images from sensor"        },
    {  TEST_STEP_ID_CPACQIMGREFLOW,                 "CpAcqImgRefLow",                   "frames",                                                                                                   "30",                  "",             "acquire images from sensor"        },
    {  TEST_STEP_ID_DRDYTEST,                       "DRdyTest",                         "",                                                                                                         "",                    "",             "GPIO DRDy Test"                    },
    {  TEST_STEP_ID_SYNCHRONIZE,                    "Synchronize",                      "",                                                                                                         "",                    "",             "Wait for stimulus"                 },
    {  TEST_STEP_ID_PROGRAMMINGMISSIONFIRMWARE,     "ProgrammingMissionFirmware",       "",                                                                                                         "",                    "",             ""                                  },
    {  TEST_STEP_ID_PROGRAMMINGIOTA_BIN,            "ProgrammingIOTA_BIN",              "DIMS;PSELECT;CONFIG_VERSION;FRAME_NAV;CONFIG_NAV_SWIPE;CONFIG_NAV_TAP;FRAME_BUTTON;CONFIG_SOFTBUTTON",     "1",                   "",             ""                                  },

    /*  stimulus "fake finger" test steps */
    // id                                           name                                arg_names,                                                                                                  args,                  args_desc       description
    {  TEST_STEP_ID_WOF_SIGNAL,                     "WOF_Signal",                       "SignalLow;SignalHigh;OffsetLow;OffsetHigh",                                                                "40;70;30;74",         "",             "WOF baseline calibration"          },
    {  TEST_STEP_ID_BUTTONTESTWITHSTIMULUS,         "ButtonTestWithStimulus",           "",                                                                                                         "",                    "",             "Check GPIO of button click"        },
    {  TEST_STEP_ID_ACQIMGFINGER,                   "AcqImgFinger",                     "frames",                                                                                                   "30",                  "",             "acquire images from sensor"        },
    {  TEST_STEP_ID_CPACQIMGSTIMULUS,               "CpAcqImgStimulus",                 "frames",                                                                                                   "30",                  "",             "acquire images from sensor"        },

    /*  stimulus "Steller Ref High" test steps */
    // id                                           name                                arg_names,                                                                                                  args,                  args_desc       description
    {  TEST_STEP_ID_CPACQIMGREFHIGH,                "CpAcqImgRefHigh",                  "frames",                                                                                                   "30",                  "",             "acquire images from sensor"        },
    {  TEST_STEP_ID_CPACQIMGROI,                    "CpAcqImgROI",                      "frames",                                                                                                   "30",                  "",             "acquire images from sensor"        },

    /* current test */
    // id                                           name                                arg_names,                                                                                                  args,                  args_desc       description
    {  TEST_STEP_ID_CURRENTTEST,                    "CurrentTest",                      "VCC High Limit(uA);SPIVCC High Limit(uA);VCC Low Limit(uA);SPIVCC Low Limit(uA)",                          "50000;5000;1;0.01",   "",             "current when acquire images"       },
    {  TEST_STEP_ID_SLEEPCURRENTTEST,               "SleepCurrentTest",                 "VCC Limit(uA);SPIVCC Limit(uA);Delay(mS)",                                                                 "500;5;500",           "",             "sleep current"                     },
    {  TEST_STEP_ID_WOFCURRENTTEST,                 "WOFCurrentTest",                   "VCC Limit(uA);SPIVCC Limit(uA);Delay(mS)",                                                                 "500;5;500",           "",             "wof sleep current"                 },

    /* image quality test */
    // id                                           name                                arg_names,                                                                                                  args,                  args_desc       description
    {  TEST_STEP_ID_HUAWEIIMAGEQUALITYTEST,         "HuaweiImageQualityTest",           "SNR Low Limit;Signal Low Limit",                                                                           "30;80",               "",             ""                                  },
    {  TEST_STEP_ID_CPDARKBADPIXELTEST,             "CpDarkBadPixelTest",               "",                                                                                                         "2;1500;1500",         "",             ""                                  },
    {  TEST_STEP_ID_CPBADPIXELTEST,                 "CpBadPixelTest",                   "",                                                                                                         "10;5;5;4;8",          "",             ""                                  },
    {  TEST_STEP_ID_SHARPNESSTEST,                  "SharpnessTest",                    "Percent Variant High Limit",                                                                               "20",                  "",             ""                                  },
    {  TEST_STEP_ID_BUBBLETEST,                     "BubbleTest",                       "High Threshold;Low Threshold",                                                                             "150;100",             "",             ""                                  },
    {  TEST_STEP_ID_PIXELUNIFORMITYTEST,            "PixelUniformityTest",              "Floored Threshold;Pegged Threshold;Row Failure Counts;Col Failure Counts;Area Failure Counts",             "300;300;8;8;8",       "",             ""                                  },
    {  TEST_STEP_ID_RXTXSTANDARDDEV,                "RxTxStandardDev",                  "Limit",                                                                                                    "30",                  "",             ""                                  },
    {  TEST_STEP_ID_IMPERFECTION,                   "Imperfection",                     "High Threshold;Low Threshold;Consectutive Limit",                                                          "150;110;6",           "",             ""                                  },
    {  TEST_STEP_ID_SNRTEST,                        "SNR",                              "SNR High Limit;SNR Low Limit;Signal High Limit;Signal Low Limit;Noise High Limit",                         "5000;800;10000;80;10","",             ""                                  },
    {  TEST_STEP_ID_CPIMAGEQUALITY,                 "CpImageQuality",                   "SNR High Limit;SNR Low Limit;Signal High Limit;Signal Low Limit;Noise High Limit",                         "5000;800;10000;80;10","",             ""                                  },

    /*  Finalize */
    // id                                           name                                arg_names,                                                                                                  args,                  args_desc       description
    {  TEST_STEP_ID_IOTACHECK,                      "IOTACheck",                        "DIMS;FW_BL;LNA_BL;WOF;PSELECT;CONFIG_VERSION;FRAME_NAV;CONFIG_NAV_SWIPE;CONFIG_NAV_TAP;FRAME_BUTTON;CONFIG_SOFTBUTTON","",        "",             "check IOTA exists"                 },
    {  TEST_STEP_ID_MISSIONFIRMWARECHECK,           "MissionFirmwareCheck",             "",                                                                                                         "",                    "",             "check MFW  exists"                 },
    {  TEST_STEP_ID_PROGRAMMINGIOTA_DATA,           "ProgrammingIOTA_DATA",             "LNA_BL;FW_BL;WOF",																							"",                    "",             "program MT IOTA data"              },
    {  TEST_STEP_ID_FINALIZATIONSTEP,               "FinalizationStep",                 "",                                                                                                         "",                    "",             "finalize,"                         },
};
