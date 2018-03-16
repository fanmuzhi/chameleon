#
# VCS Feature configurations
#
# This file contains a list of all the configurable features and their default
# settings. To change the default setting of a feature, define it in the
# platform CMAKE config file.
#
# For example, if a platform should not include the Cogent matcher, add the
# following line to the platform CMAKE config file.
#
#
##############

# Use vcsFeatures.h file in the build. vcsFeatures.h will be included by vcsBase.h
MESSAGE(STATUS "**************************************************************************")
MESSAGE(STATUS "CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}")
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR})
MESSAGE(STATUS "**************************************************************************")
#ADD_DEFINITIONS(-DVCS_FEATURES_DEFINED=1)

# vcsFeatures.h is automatically generated to define the settings of all the VCS_FEATURES
# for each platform. The file is generated in the root of the build directory.
IF (${CMAKE_CFG_DIR}/modules/vcsFeatures.cmake IS_NEWER_THAN ${CMAKE_BINARY_DIR}/vcsFeatures.h)
    SET (VCS_FEATURES_H ${CMAKE_BINARY_DIR}/vcsFeatures.h)
ENDIF (${CMAKE_CFG_DIR}/modules/vcsFeatures.cmake IS_NEWER_THAN ${CMAKE_BINARY_DIR}/vcsFeatures.h)

# Insert NDA text and generate the file header
#IF (VCS_FEATURES_H)
#    # Get the NDA header to be added to the automatically generated 'vcsFeatures.h' file.
#    # Note that current header is adjusted to be used in Android.mk and shell script files,
#    # so the comment character need to be removed from it first.
#    FILE(READ ${CMAKE_CFG_DIR}/sourceRelease/NDAHeader NDA_HEADER_DIRTY)
#    string(REPLACE "#" "" NDA_HEADER_CCODE ${NDA_HEADER_DIRTY})
#    FILE(WRITE ${VCS_FEATURES_H}	"${NDA_HEADER_CCODE}\n\n")
#    FILE(APPEND ${VCS_FEATURES_H}	"/*\n  Automatically generated. DO NOT EDIT!!!\n*/\n")
#    FILE(APPEND ${VCS_FEATURES_H}	"/* vcsFeatures definitions */\n")
#ENDIF (VCS_FEATURES_H)


########################################
# Macro defintions
########################################

#
# Macro to define a VCS_FEATURE
#
# Syntax: VCS_FEATURE(featureName defaultValue)
#
MACRO(VCS_FEATURE _FEATURE_NAME_ _DEFAULT_VALUE_)
    IF (NOT DEFINED ${_FEATURE_NAME_})
        SET(${_FEATURE_NAME_} ${_DEFAULT_VALUE_})
    ENDIF (NOT DEFINED ${_FEATURE_NAME_})
    IF (VCS_FEATURES_H)
        FILE(APPEND ${VCS_FEATURES_H}	"#define ${_FEATURE_NAME_} ${${_FEATURE_NAME_}}\n")
    ENDIF (VCS_FEATURES_H)
ENDMACRO(VCS_FEATURE)

#
# Macro to define a VCS_FEATURE_IF
#
# Syntax: VCS_FEATURE_IF(featureName condition)
#
MACRO(VCS_FEATURE_IF _FEATURE_NAME_ _CONDITION_)
    IF (NOT DEFINED ${_FEATURE_NAME_})
        IF (${_CONDITION_})
            SET(${_FEATURE_NAME_} 1)
        ELSE (${_CONDITION_})
            SET(${_FEATURE_NAME_} 0)
        ENDIF (${_CONDITION_})
    ENDIF(NOT DEFINED ${_FEATURE_NAME_})
    IF (VCS_FEATURES_H)
        FILE(APPEND ${VCS_FEATURES_H}	"#define ${_FEATURE_NAME_} ${${_FEATURE_NAME_}}\n")
    ENDIF (VCS_FEATURES_H)
ENDMACRO(VCS_FEATURE_IF)

#
# Macro to define a VCS_FEATURE_IFNOT
#
# Syntax: VCS_FEATURE_IFNOT(featureName condition)
#
MACRO(VCS_FEATURE_IFNOT _FEATURE_NAME_ _CONDITION_)
    IF (NOT DEFINED ${_FEATURE_NAME_})
        IF (${_CONDITION_})
            SET(${_FEATURE_NAME_} 0)
        ELSE (${_CONDITION_})
            SET(${_FEATURE_NAME_} 1)
        ENDIF (${_CONDITION_})
    ENDIF(NOT DEFINED ${_FEATURE_NAME_})
    IF (VCS_FEATURES_H)
        FILE(APPEND ${VCS_FEATURES_H}	"#define ${_FEATURE_NAME_} ${${_FEATURE_NAME_}}\n")
    ENDIF (VCS_FEATURES_H)
ENDMACRO(VCS_FEATURE_IFNOT)

#
# Macro to define a VCS_FEATURE_IF_OR
#
# Syntax: VCS_FEATURE_IF_OR(featureName condition, condition1)
#
MACRO(VCS_FEATURE_IF_OR _FEATURE_NAME_ _CONDITION_ _CONDITION1_)
    IF (NOT DEFINED ${_FEATURE_NAME_})
        IF (${_CONDITION_} OR ${_CONDITION1_})
            SET(${_FEATURE_NAME_} 1)
        ELSE (${_CONDITION_} OR ${_CONDITION1_})
            SET(${_FEATURE_NAME_} 0)
        ENDIF (${_CONDITION_} OR ${_CONDITION1_})
    ENDIF(NOT DEFINED ${_FEATURE_NAME_})
    IF (VCS_FEATURES_H)
        FILE(APPEND ${VCS_FEATURES_H}	"#define ${_FEATURE_NAME_} ${${_FEATURE_NAME_}}\n")
    ENDIF (VCS_FEATURES_H)
ENDMACRO(VCS_FEATURE_IF_OR)

#
# Macro to define a VCS_FEATURE_SENSOR_FID
#
# Syntax: VCS_FEATURE_SENSOR_FID(flexid defaultValue)
#
MACRO(VCS_FEATURE_SENSOR_FID _FLEXID_ _DEFAULT_VALUE_)
    IF (NOT DEFINED VCS_FEATURE_SENSOR_FID${_FLEXID_})
        SET(VCS_FEATURE_SENSOR_FID${_FLEXID_} ${_DEFAULT_VALUE_})
    ENDIF (NOT DEFINED VCS_FEATURE_SENSOR_FID${_FLEXID_})
    IF (VCS_FEATURES_H)
        FILE(APPEND ${VCS_FEATURES_H}	"#define VCS_FEATURE_SENSOR_FID${_FLEXID_} ${VCS_FEATURE_SENSOR_FID${_FLEXID_}}\n")
    ENDIF (VCS_FEATURES_H)
ENDMACRO(VCS_FEATURE_SENSOR_FID)

##############
# Macro to check definition existence, set default value if not defined.
#
# Syntax: VCS_DEFINITION(definitionName defaultValue)
#
MACRO(VCS_DEFINITION _DEF_NAME_ _DEF_VAL_)
    GET_DIRECTORY_PROPERTY(_DIR_DEFS_ DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} COMPILE_DEFINITIONS)
    SET(MACRO_FOUND 0)
    FOREACH( d ${_DIR_DEFS_} )
        IF(${d} MATCHES "${_DEF_NAME_}*")
            SET(MACRO_FOUND 1)
            BREAK()
        ENDIF(${d} MATCHES "${_DEF_NAME_}*")
    ENDFOREACH()
    IF(MACRO_FOUND EQUAL 0)
        ADD_DEFINITIONS(-D${_DEF_NAME_}=${_DEF_VAL_})
    ENDIF(MACRO_FOUND EQUAL 0)
ENDMACRO(VCS_DEFINITION)

########################################
# Include the project specific feature definitions
INCLUDE(${CMAKE_PROJECT}Opts)
