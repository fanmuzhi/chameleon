#pragma once

#include <stdint.h>
#include <string>
using namespace std;


//FPMODULE error code
#define ERROR_TIME_OUT                                      0x1500
#define ERROR_CRC_VERIFY                                    0x1501
#define ERROR_BL_MODE                                       0x1502
#define ERROR_PARAM_UNDEFINE                                0x1503
#define ERROR_TYPE                                          0x1504
#define ERROR_PARAMETER                                     0x1505
#define ERROR_RESULT_NOTFOUND                               0x1506
#define ERROR_REPLY_LENGTH_TOO_SHORT                        0x1507
#define ERROR_NOT_IMPLEMENTED                               0x1508
#define ERROR_IOTA_NOT_FOUND                                0x1509

//FT status:
#define SYN_M5_OK                                           0
#define SYN_M5_INVALID_HANDLE                               0x2001
#define SYN_M5_DEVICE_NOT_FOUND                             0x2002
#define SYN_M5_DEVICE_NOT_OPENED                            0x2003
#define SYN_M5_IO_ERROR                                     0x2004
#define SYN_M5_INSUFFICIENT_RESOURCES                       0x2005
#define SYN_M5_INVALID_PARAMETER                            0x2006
#define SYN_M5_INVALID_BAUD_RATE                            0x2007
#define SYN_M5_DEVICE_NOT_OPENED_FOR_ERASE                  0x2008
#define SYN_M5_DEVICE_NOT_OPENED_FOR_WRIT                   0x2009
#define SYN_M5_FAILED_TO_WRITE_DEVICE                       0x200A
#define SYN_M5_EEPROM_READ_FAILED                           0x200B
#define SYN_M5_EEPROM_WRITE_FAILED                          0x200C
#define SYN_M5_EEPROM_ERASE_FAILED                          0x200D
#define SYN_M5_EEPROM_NOT_PRESENT                           0x200E
#define SYN_M5_EEPROM_NOT_PROGRAMMED                        0x200F
#define SYN_M5_INVALID_ARGS                                 0x2010
#define SYN_M5_NOT_SUPPORTED                                0x2011
#define SYN_M5_OTHER_ERROR                                  0x2012
#define SYN_M5_DEVICE_LIST_NOT_READY                        0x2013
#define SYN_M5_OUTPUT_VOLTAGE_OUT_OF_RANGE                  0x2014
#define SYN_M5_SET_INA226_REGISTER_FAILED                   0x2015
#define SYN_M5_SET_GPIO_FAILED                              0x2016

//mpc04 status
#define SYN_MPC04_ERR_OK                                    0
#define SYN_MPC04_ERR_INITIALIZATION_FAILED                 0x3001
#define SYN_MPC04_ERR_DEINITIALIZATION_FAILED               0x3002
#define SYN_MPC04_ERR_GET_DEVICE_NUMBER_FAILED              0x3003
#define SYN_MPC04_ERR_GET_DEVICE_HANDLE_FAILED              0x3004
#define SYN_MPC04_ERR_COMMUNICATION_INITIALIZE_FAILED       0x3005
#define SYN_MPC04_ERR_COMMUNICATION_FAILED                  0x3006
#define SYN_MPC04_ERR_FILE_NAME_TOO_LONG                    0x3007
#define SYN_MPC04_ERR_OPEN_LOCAL_FILE_FAILED                0x3008
#define SYN_MPC04_ERR_INVALID_FILE_INFORMATION              0x3009
#define SYN_MPC04_ERR_SCRIPT_CONVERT_FAILED                 0x300a
#define SYN_MPC04_ERR_LOAD_PYTHON_MODULE_FAILED             0x300b
#define SYN_MPC04_ERR_PYTHON_VERSION_NOT_MATCH              0x300c
#define SYN_MPC04_ERR_INVALID_DEVICE_HANDLE                 0x300d
#define SYN_MPC04_ERR_DEVICE_NOT_CONNECTED                  0x300e
#define SYN_MPC04_ERR_INSUFICIENT_SCRIPT_DATA               0x300f
#define SYN_MPC04_ERR_FILE_NOT_FOUND                        0x3010
#define SYN_MPC04_ERR_NOT_INITIALIZED                       0x3011
#define SYN_MPC04_ERR_COMMAND_VALIDATION_FAILED             0x3012
#define SYN_MPC04_ERR_SET_ABORT_UPDATE_FIRMWARE_FAILED      0x3013
#define SYN_MPC04_ERR_UPDATE_FIRMWARE_ABORTED               0x3014
#define SYN_MPC04_ERR_INSUFFICIENT_SCRIPT_OUTPUT_BUFFER     0x3015
#define SYN_MPC04_ERR_ARG_LIST_TOO_LONG                     0x3016
#define SYN_MPC04_ERR_SCRIPT_EXE_FAILED                     0x3017
#define SYN_MPC04_ERR_HID_NULL_REPORT_DESC                  0x3018
#define SYN_MPC04_ERR_HID_LENGTH_FAILED                     0x3019
#define SYN_MPC04_ERR_HID_TABLE_FAILED                      0x301a
#define SYN_MPC04_ERR_HID_VALUE_RANGE_FAILED                0x301b
#define SYN_MPC04_ERR_HID_ENUM_FAILED                       0x301c
#define SYN_MPC04_ERR_HID_SWITCH_MODE_FAILED                0x301d
#define SYN_MPC04_ERR_HID_REPORT_UNAVAILABLE                0x301e
#define SYN_MPC04_ERR_FP_COMMAND_FAILED                     0x301f
#define SYN_MPC04_ERR_FP_INVALID_SENSOR_TYPE                0x3020
#define SYN_MPC04_ERR_FP_INVALID_PARAMS                     0x3021
#define SYN_MPC04_ERR_PARAM_VLIDATION_FAILED                0x3ffe
#define SYN_MPC04_ERR_NOT_IMPLEMENTED                       0x3fff

// 0x4000 represents for sensor command error

#define SYN_ERROR_DEVIVENUMBER_NULL                         0x5001
#define SYN_ERROR_DEVICE_UNKNOWN                            0x5002
#define SYN_ERROR_DEVICE_INIT_FAILED                        0x5003
#define SYN_ERROR_DEVICE_WRONG_HANDLE                       0x5004
#define SYN_ERROR_OTHER_ERROR                               0x5005
#define SYN_ERROR_DEVICE_ASSIGNED                           0x5006

// Test Engine Error
#define ERROR_TESTENGINE_STATE                              0x6001
#define ERROR_TESTENGINE_DATA                               0x6002
#define ERROR_TESTSTEP_UNDEFINE                             0x6003
#define ERROR_TSETSTEP_UNEXCUTED                            0x6004
#define ERROR_TSETSTEP_ARGSLENGTH                           0x6005
#define ERROR_CONFIG_DATA_LOST                              0x6006
#define ERROR_BOOTLOADERMODE_CHECK                          0x6007
#define ERROR_DUTUTILS_NULL                                 0x6010
#define ERROR_BRAVOMODULE_NULL                              0x6011
#define ERROR_ENGINELOG_FOLDER                              0x6101
#define ERROR_ENGINELOG_FILE                                0x6102
#define ERROR_LOAD_CPID_LIB                                 0x6103
#define ERROR_LED_STATUS                                    0x6104
