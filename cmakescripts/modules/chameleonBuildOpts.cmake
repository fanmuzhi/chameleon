#
# niseCore Build Options configurations
#
# This file contains a list of all the configurable options and 
# their default settings for niseCore build to exclude modules' builds.
# To change the default setting of an option, define it in the
# nisePlatform CMAKE config file.
#
##############

########################################
# Macro defintions
########################################

# Dump cmake build feature variables to BuildVariables.txt
SET (BUILD_VARS_TXT ${CMAKE_CURRENT_BINARY_DIR}/BuildVariables.txt)
FILE (WRITE ${BUILD_VARS_TXT}     "")

#
# Macro to define a BUILD_FEATURE
#
# Syntax: BUILD_FEATURE(featureName defaultValue)
#
MACRO(BUILD_FEATURE _FEATURE_NAME_ _DEFAULT_VALUE_)
    IF (NOT DEFINED ${_FEATURE_NAME_})
        SET(${_FEATURE_NAME_} ${_DEFAULT_VALUE_})
    ENDIF (NOT DEFINED ${_FEATURE_NAME_})
    IF (BUILD_VARS_TXT)
        FILE(APPEND ${BUILD_VARS_TXT}    " ${_FEATURE_NAME_} \"${${_FEATURE_NAME_}}\"\n")
    ENDIF (BUILD_VARS_TXT)
ENDMACRO(BUILD_FEATURE)

#
# Macro to define a BUILD_FEATURE_HARD
#
# Syntax: BUILD_FEATURE_HARD(featureName value)
#
MACRO(BUILD_FEATURE_HARD _FEATURE_NAME_ _VALUE_)
    SET(${_FEATURE_NAME_} ${_VALUE_})
    IF (BUILD_VARS_TXT)
        FILE(APPEND ${BUILD_VARS_TXT}    " ${_FEATURE_NAME_} \"${${_FEATURE_NAME_}}\"\n")
    ENDIF (BUILD_VARS_TXT)
ENDMACRO(BUILD_FEATURE_HARD)

MESSAGE ("Using chameleon build configuration options...")

#############
# Production build definitions to enable the Product version
IF (ENABLE_PRODUCTION_BUILD EQUAL 1)
    MESSAGE ("Production build")
    BUILD_FEATURE (NISE_BUILD_PRODUCTION 1)
ELSE (ENABLE_PRODUCTION_BUILD EQUAL 1)
    BUILD_FEATURE (NISE_BUILD_PRODUCTION 0)
ENDIF (ENABLE_PRODUCTION_BUILD EQUAL 1)

########################################
# Build option definitions
########################################

IF (ENABLE_PRODUCTION_BUILD EQUAL 1)
    # For the production build FCL build should be explicitly disabled
    BUILD_FEATURE_HARD (NISE_BUILD_FCL     0)
    BUILD_FEATURE_HARD (NISE_BUILD_AS_TEST 0)
ELSE (ENABLE_PRODUCTION_BUILD EQUAL 1)
    # In case of non-production build, set default value for FCL build feature,
    # and let target platform specific CMake files to decide feature value.
    BUILD_FEATURE (NISE_BUILD_FCL 1)
ENDIF (ENABLE_PRODUCTION_BUILD EQUAL 1)

# Enables/disables palCrypto lib build
#BUILD_FEATURE (NISE_BUILD_PAL_CRYPTO    1)
#
## Enables/disables niseEngTest build
#BUILD_FEATURE (NISE_BUILD_ENG_TEST      0)
#
## Enables/disables Matcher module build
#BUILD_FEATURE (NISE_BUILD_MATCHER       0)
#
## Enables/disables Sensor module build
#BUILD_FEATURE (NISE_BUILD_SENSOR        0)
#
## Enables/disables Storage module build
#BUILD_FEATURE (NISE_BUILD_STORAGE       0)
#
## Enables/disables VFM build
#BUILD_FEATURE (NISE_BUILD_VFM           0)
#
## Enables/disables VFM utility build
#BUILD_FEATURE (NISE_BUILD_VFM_UTIL      0)
#
## Enables/disables Authenticator build
#BUILD_FEATURE (NISE_BUILD_AUTHENTICATOR 0)
#
## Enables/disables IST script engine lib build
#BUILD_FEATURE (NISE_BUILD_IST_SE        0)
#
## Enables/disables IST execution engine lib build
#BUILD_FEATURE (NISE_BUILD_IST_EE        0)
#
## Enables/disables AS tool build
#BUILD_FEATURE (NISE_BUILD_AS_TEST       0)
#
## Enables/disables vfmEngTest build
#BUILD_FEATURE (NISE_BUILD_VFM_ENG_TEST  0)
#
## Enables/disables NITE RTE lib build
#BUILD_FEATURE (NISE_BUILD_NITE_RTE       0)
#
## Enables/disables NITE TSE lib build
#BUILD_FEATURE (NISE_BUILD_NITE_TSE       0)
#
##################################
## Unit Test Framework definitions
#
#IF (ENABLE_PRODUCTION_BUILD EQUAL 1)
#    # For the production build Unit test builds should be explicitly disabled
#    BUILD_FEATURE_HARD (NISE_BUILD_UNIT_TESTS    0)
#    BUILD_FEATURE_HARD (NISE_BUILD_GTEST         0)
#    BUILD_FEATURE_HARD (NISE_BUILD_CPPUTEST      0)
#    BUILD_FEATURE_HARD (NISE_BUILD_UNITY         0)
#ELSE (ENABLE_PRODUCTION_BUILD EQUAL 1)
#    # In case of non-production build, set default values for Unit test build features,
#    # and let target platform specific CMake files to decide feature values.
#    # Enables/disables Unit Tests build
#    BUILD_FEATURE (NISE_BUILD_UNIT_TESTS    0)
#    # Enables/disables Google Test build
#    BUILD_FEATURE (NISE_BUILD_GTEST         0)
#    # Enables/disables CppUTest build
#    BUILD_FEATURE (NISE_BUILD_CPPUTEST      0)
#    # Enables/disables Unity build
#    BUILD_FEATURE (NISE_BUILD_UNITY         0)
#ENDIF (ENABLE_PRODUCTION_BUILD EQUAL 1)
#
## Appending sensor family/type, matcher and storage types to BuildVariables.txt
#IF (BUILD_VARS_TXT)
#    FILE(APPEND ${BUILD_VARS_TXT}    " NISE_SENSOR_FAMILY \"${NISE_SENSOR_FAMILY}\"\n")
#    FILE(APPEND ${BUILD_VARS_TXT}    " SENSOR_TYPE \"${SENSOR_TYPE}\"\n")
#    FILE(APPEND ${BUILD_VARS_TXT}    " IPL_VER \"${IPL_VER}\"\n")
#    FILE(APPEND ${BUILD_VARS_TXT}    " NISE_MATCHER_TYPE \"${NISE_MATCHER_TYPE}\"\n")
#    FILE(APPEND ${BUILD_VARS_TXT}    " QM_VER \"${QM_VER}\"\n")
#    FILE(APPEND ${BUILD_VARS_TXT}    " QM_FLAVOR \"${QM_FLAVOR}\"\n")
#    FILE(APPEND ${BUILD_VARS_TXT}    " ASP_VER \"${ASP_VER}\"\n")
#    FILE(APPEND ${BUILD_VARS_TXT}    " ASP_FLAVOR \"${ASP_FLAVOR}\"\n")
#    FILE(APPEND ${BUILD_VARS_TXT}    " NISE_STORAGE_TYPE \"${NISE_STORAGE_TYPE}\"\n")
#    FILE(APPEND ${BUILD_VARS_TXT}    " TARG_ANDROID_VERSION \"${TARG_ANDROID_VERSION}\"\n")
#ENDIF (BUILD_VARS_TXT)
