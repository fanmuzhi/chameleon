#
# chameleon Options configurations
#
# This file contains a list of all the configurable options and
# their default settings for chameleon.
# To change the default setting of an option, define it in the
# nisePlatform CMAKE config file.
#
##############

#MESSAGE ("Using chameleon configuration options...")

########################################
# Build option definitions
########################################


##############
# Include nise build options

INCLUDE(chameleonBuildOpts)

########################################
# Sensor communication interface selection
########################################

##############
# To enable USB driver, set NISE_FEATURE_USB_DRIVER to 0
#VCS_FEATURE (NISE_FEATURE_USB_DRIVER         0)
#
###############
## To enable SPI support for Linux/Android, set NISE_FEATURE_SPI_DRIVER to 1
#VCS_FEATURE (NISE_FEATURE_SPI_DRIVER         0)
#
###############
## To enable MPC04 support set NISE_FEATURE_MPC04_DRIVER to 1
#VCS_FEATURE (NISE_FEATURE_MPC04_DRIVER        0)
#
###############
## To enable M5 support set NISE_FEATURE_M5_DRIVER to 1
#VCS_FEATURE (NISE_FEATURE_M5_DRIVER         0)
#
###############
## To enable M5 extended module API support (calibrate, voltage current measure etc) set NISE_FEATURE_M5_EXT_DRIVER to 1
#VCS_FEATURE (NISE_FEATURE_M5_EXT_DRIVER     0)
#
###############
## To enable soft button support set NISE_FEATURE_SOFT_BUTTON to 1
#VCS_FEATURE (NISE_FEATURE_SOFT_BUTTON       0)
#
###############
## To enable sensor Lock support set NISE_FEATURE_SENSOR_LOCK to 1
#VCS_FEATURE (NISE_FEATURE_SENSOR_LOCK       0)
#
##############
## Enable registration of VTP callback support.
#VCS_FEATURE (NISE_FEATURE_VTP_CALLBACK                  0)
#
##############
## Enable VFM function packing mechanism/module for NW side
#VCS_FEATURE (NISE_FEATURE_FUNC_PACK                     0)
#
## Enable VFM function unpacking mechanism/module for TZ side
#VCS_FEATURE (NISE_FEATURE_FUNC_UNPACK                   0)
#
## To indicate that storage functionality control present in specific build, set NISE_FEATURE_STG_FUNC to 1
#VCS_FEATURE (NISE_FEATURE_STG_FUNC                      0)
#
## To indicate that HAL Auth Id generation functionality present in specific build, set NISE_FEATURE_HAL_AUTH_ID_FUNC to 1
#VCS_FEATURE (NISE_FEATURE_HAL_AUTH_ID_FUNC              0)
#
##############
## To enable memory profiling functionality, set NISE_FEATURE_PRF_MEMORY to 1
#VCS_FEATURE_IFNOT (NISE_FEATURE_PRF_MEMORY  NISE_BUILD_PRODUCTION)
#
##############
## To enable diagnostic thread functionality, set NISE_FEATURE_DIAGNOSTIC_THREAD to 1
#VCS_FEATURE (NISE_FEATURE_DIAGNOSTIC_THREAD  0)
# 	
##############
## Used to enable/disable fingerprint image providing from VFM layer. This feature should be
## disabled for production build. For engineering build it is enabled by default
## and can be disabled for specific builds.
#VCS_FEATURE_IFNOT (NISE_FEATURE_PROVIDE_FP_IMAGE  NISE_BUILD_PRODUCTION)
#
##############
## Used to enable/disable fingerprint image injection to VFM layer. This feature
## should be disabled for production build. For engineering build it is enabled
## by default and can be disabled for specific builds.
#VCS_FEATURE_IFNOT (NISE_FEATURE_INJECT_FP_IMAGE  NISE_BUILD_PRODUCTION)
#
##############
## Used to enable/disable raw fingerprint data providing from VFM layer. This feature
## should be disabled for production build. For engineering build it is enabled
##  by default and can be disabled for specific builds.
#VCS_FEATURE_IFNOT (NISE_FEATURE_PROVIDE_FP_RAW  NISE_BUILD_PRODUCTION)
#
##############
## To indicate that specific platform do not have timer support, set NISE_FEATURE_NO_TIMER_SUPPORT to 1
#VCS_FEATURE (NISE_FEATURE_NO_TIMER_SUPPORT	0)
#
##############
## Enable to optimize for embedded platform with low memory resource
#VCS_FEATURE (NISE_FEATURE_EMBEDDED          0)
#
##############
## Enable to check IOTA compatibility in production build
## The NISE_FEATURE_IOTA_COMPAT disabled by default
#VCS_FEATURE_IF(NISE_FEATURE_IOTA_COMPAT NISE_BUILD_PRODUCTION)
#
#########################################
## Nassau specific definitions starts here
#
##############
## To enable Nassau SPI data integrity, set NISE_FEATURE_NASSAU_SPI_INTEGRITY to 1
#VCS_FEATURE (NISE_FEATURE_NASSAU_SPI_INTEGRITY      1)
#
## Enable configuration through PersistentData
#VCS_FEATURE_IFNOT(NISE_FEATURE_PERSISTENT  NISE_BUILD_PRODUCTION)
#
## enables/disables overall debug logging support
#VCS_FEATURE(NISE_FEATURE_DBG_LOG_ENABLE 1)
#
## enables/disables tracing support
#VCS_FEATURE(NISE_FEATURE_DBG_LOG_TRACE 1)
#
## enables/disables logging messages, this is separated from tracing to have better control
## in cases when we want to only see trace or only messages without trace
#VCS_FEATURE(NISE_FEATURE_DBG_LOG_MSG 1)
#
## enables/disables concise mode, if 0 then full mode is enabled
#VCS_FEATURE_IF(NISE_FEATURE_DBG_LOG_CONCISE  NISE_BUILD_PRODUCTION)
#
## Enables/disables storing of authenticator Id in secure storage
## (should be enabled if target platform has a secure storage support).
## In case of disabled auth Id will be stored in Android filesystem.
#VCS_FEATURE(NISE_FEATURE_HAL_SEC_AUTH_ID                 0)
#
## enables/disables template protection in NISE VfmCore
#VCS_FEATURE(NISE_FEATURE_ENABLE_TEMPLATE_PROTECTION  1)
#
##############
## Used to enable/disable playback data processing functionality.
## This feature should be disabled for production build
#VCS_FEATURE_IFNOT(NISE_FEATURE_PLAYBACK_SUPPORT  NISE_BUILD_PRODUCTION)
#
################
## ASP specific...
## set NISE_FEATURE_ENABLE_ASP to 1, to enable specific env.
#VCS_FEATURE(NISE_FEATURE_ENABLE_ASP  0)
#
################
## ASP specific...
## set NISE_FEATURE_USE_RAW_LBP to 1, to enable raw lbp calculation for Anti-spoof.
#VCS_FEATURE(NISE_FEATURE_USE_RAW_LBP  0)
#
################
## Enable Android HAL build
#VCS_FEATURE(NISE_FEATURE_ADR_HAL     0)
#
##Enable this macro for BVS saving
## This feature should be disabled for production build
#VCS_FEATURE_IFNOT(NISE_FEATURE_SAVE_BVS    NISE_BUILD_PRODUCTION)
#
##Enable this macro for BMP saving
## This feature should be disabled for production build
#VCS_FEATURE_IFNOT(NISE_FEATURE_SAVE_IMG     NISE_BUILD_PRODUCTION)
#
## A temporary solution to enable Android HAL support for non-TZ
#VCS_FEATURE(NISE_FEATURE_ADR_HAL_NON_TZ        0)
#
## Enable IPC based HMAC key checking
#VCS_FEATURE(NISE_FEATURE_AUTH_TOKEN_IPC_GET    0)
#
## Enable secure session support (TLS)
#VCS_FEATURE(NISE_FEATURE_SECURE_SESSION    0)
#
##Indicates that in case of MFW update or DoPair/Unpair
## operation driver process will unload.
#VCS_FEATURE(NISE_FEATURE_UNLOAD_ON_RESET    0)
#
##To enable retrieving OEM seed from the system,
## otherwise test key material will be used.
#VCS_FEATURE(NISE_FEATURE_OEM_SEED           0)
#
##Indicates Pairing Data should be stored in Secure Storage
#VCS_FEATURE(NISE_FEATURE_SEC_PD_STORING     0)
#
##Indicates that Pairing Data should be encrypted with key derived from key material
## This can be used in multi-domain integrations.
#VCS_FEATURE(NISE_FEATURE_ENC_PD_SYNAKM     0)
#
## Enable IST when NISE_BUILD_IST_SE or NISE_BUILD_IST_EE is enabled
#VCS_FEATURE_IF_OR (NISE_FEATURE_IST  NISE_BUILD_IST_SE NISE_BUILD_IST_EE)
#
## Enable NITE model when NISE_BUILD_NITE_TSE or NISE_BUILD_NITE_RTE is enabled
#VCS_FEATURE_IF_OR (NISE_FEATURE_NITE  NISE_BUILD_NITE_TSE NISE_BUILD_NITE_RTE)
#
## Enable latency profiling support
#VCS_FEATURE_IFNOT(NISE_FEATURE_PRF_LATENCY   NISE_BUILD_PRODUCTION)
#
## TODO: temporary feature to distinguish rectangle button.
## This feature is to be removed when host SW has ability
## to handle IPL parameters and sensor configuration in
## a generic way
#VCS_FEATURE(NISE_FEATURE_HWI_SENSOR_RECTANGLE 0)
#
## TODO: temporary feature to distinguish square button.
## This feature is to be removed when host SW has ability
## to handle IPL parameters and sensor configuration in
## a generic way
#VCS_FEATURE(NISE_FEATURE_HWI_SENSOR_SQUARE 0)
#
## To enable combo driver support set NISE_FEATURE_MULTI_TRANSPORT to 1
#VCS_FEATURE (NISE_FEATURE_MULTI_TRANSPORT          0)
#
## To enable rejecting all captures with warnings/errors set NISE_FEATURE_ONLY_0X0_FOR_ENROLL 1
#VCS_FEATURE(NISE_FEATURE_ONLY_0X0_FOR_ENROLL 0)
#
## Enable file io operations in fcl by default when NISE_BUILD_IST_EE is enabled
#VCS_FEATURE_IF (NISE_FEATURE_FCL_FILE_IO NISE_BUILD_FCL)
#
## To enable sending masked image to QM, set NISE_FEATURE_ENABLE_MASK_IMAGE to 1
#VCS_FEATURE(NISE_FEATURE_ENABLE_MASK_IMAGE 0)
#
## To enable using LCM value, set NISE_FEATURE_ENABLE_LCM to 1
#VCS_FEATURE(NISE_FEATURE_ENABLE_LCM 0)
#
## To Enable multi frame feature
#VCS_FEATURE(NISE_FEATURE_MULTI_FRAME_CAPTURE 0) 
#
## To enable using TS_IN pin  based fast tap NISE_FEATURE_FW_BASED_FINGERUP to 1
#VCS_FEATURE(NISE_FEATURE_FW_BASED_FINGERUP 0)
#
################
## MiS specific...
## set NISE_FEATURE_MATCHER_MIS to 1, to enable match in sensor.
#VCS_FEATURE (NISE_FEATURE_MATCHER_MIS       0)
#
## Enable "storage on sensor" capability
#VCS_FEATURE (NISE_FEATURE_STG_ON_SENSOR 0)
#
###############
## Include matcher options
#
##IF (NISE_BUILD_MATCHER EQUAL 1)
##    INCLUDE(niseMatcherOpts)
##ENDIF (NISE_BUILD_MATCHER EQUAL 1)
#
##Disable Temperature correction for Optical Sensors, until the feature is ready
#VCS_FEATURE(NISE_FEATURE_OPTICAL_SENSOR_TEMPERATURE_CORRECTION 0)
#
##Disable SunDry register saving to BVS file for Optical Sensors. It will be enabled in Steller platforms vcsfeatures.txt
#VCS_FEATURE(NISE_FEATURE_OPTICAL_SENSOR_SUNDRY_REGS 0)
#
################
## RFG related flags
## disabled by default - enable for specific sensors
#VCS_FEATURE(NISE_FEATURE_ROLLING_FG 0)
#
##Save RFG as an image - this is applicable only if RFG is enabled and file write is possible
#VCS_FEATURE(NISE_FEATURE_SAVE_RFG 0)
#
## Feature to enable/disable auxilary template match,
## This is a special template feature for Steller
#VCS_FEATURE(NISE_FEATURE_ENABLE_AUX_MATCH 0)
#
## Enable inject enrollment functionality
## This feature should be disabled for production build
#VCS_FEATURE_IFNOT(NISE_FEATURE_INJECT_ENROLLMENT     NISE_BUILD_PRODUCTION)
