#/* !@file CMakeLists.txt
# *******************************************************************************
# **
# **                           NDA AND NEED-TO-KNOW REQUIRED
# **
# *****************************************************************************
# **
# **  Copyright (C) 2016-2018 Synaptics Incorporated. All rights reserved.
# **
# **
# ** This file contains information that is proprietary to Synaptics
# ** Incorporated ("Synaptics"). The holder of this file shall treat all
# ** information contained herein as confidential, shall use the
# ** information only for its intended purpose, and shall not duplicate,
# ** disclose, or disseminate any of this information in any manner unless
# ** Synaptics has otherwise provided express, written permission.
# ** Use of the materials may require a license of intellectual property
# ** from a third party or from Synaptics. Receipt or possession of this
# ** file conveys no express or implied licenses to any intellectual
# ** property rights belonging to Synaptics.
# **
# **
# ** INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS," AND
# ** SYNAPTICS EXPRESSLY DISCLAIMS ALL EXPRESS AND IMPLIED WARRANTIES,
# ** INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
# ** PARTICULAR PURPOSE, AND ANY WARRANTIES OF NON-INFRINGEMENT OF ANY
# ** INTELLECTUAL PROPERTY RIGHTS. IN NO EVENT SHALL SYNAPTICS BE LIABLE
# ** FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE, OR
# ** CONSEQUENTIAL DAMAGES ARISING OUT OF OR IN CONNECTION WITH THE USE OF
# ** THE INFORMATION CONTAINED IN THIS DOCUMENT, HOWEVER CAUSED AND BASED
# ** ON ANY THEORY OF LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# ** NEGLIGENCE OR OTHER TORTIOUS ACTION, AND EVEN IF SYNAPTICS WAS ADVISED
# ** OF THE POSSIBILITY OF SUCH DAMAGE. IF A TRIBUNAL OF COMPETENT
# ** JURISDICTION DOES NOT PERMIT THE DISCLAIMER OF DIRECT DAMAGES OR ANY
# ** OTHER DAMAGES, SYNAPTICS' TOTAL CUMULATIVE LIABILITY TO ANY PARTY
# ** SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.
#*/


# Setup the minimum version of CMake required
CMAKE_MINIMUM_REQUIRED(VERSION 3.4.3)

# Name of the cmake project we are building
# This name will be used to reference the project specific options definitions
SET(CMAKE_PROJECT   "chameleon")

# Setup the chameleon root directory and determaine the tools path
file(TO_CMAKE_PATH "$ENV{CHAMELEON_DIR}" CHAMELEON_DIR)
file(TO_CMAKE_PATH "$ENV{CMAKE_CFG_DIR}" CMAKE_CFG_DIR)
file(TO_CMAKE_PATH "$ENV{TOOLS}" TOOLS)

# Select the value of TARG_OS/TARG_CPU/TARG_PLAT/BUILDTYPE
SET(TARG_OS   "win" CACHE STRING "Select target OS")
SET(TARG_CPU  "x86" CACHE STRING "Select target CPU")
SET(TARG_PLAT "generic" CACHE STRING "Select target PLATFORM")
SET(BUILDTYPE "Release" CACHE STRING "Select build type")

# Make sure important variables are defined
IF(DEFINED CHAMELEON_DIR)
    MESSAGE(STATUS "CHAMELEON_DIR set to ${CHAMELEON_DIR}")
ELSE(DEFINED CHAMELEON_DIR)
    MESSAGE(FATAL_ERROR "CHAMELEON_DIR not defined!")
ENDIF(DEFINED CHAMELEON_DIR)

IF(DEFINED CMAKE_CFG_DIR)
    MESSAGE(STATUS "CMAKE_CFG_DIR set to ${CMAKE_CFG_DIR}")
ELSE(DEFINED CMAKE_CFG_DIR)
    MESSAGE(FATAL_ERROR "CMAKE_CFG_DIR not defined!")
ENDIF(DEFINED CMAKE_CFG_DIR)

IF(DEFINED TARG_OS)
    MESSAGE(STATUS "TARG_OS set to ${TARG_OS}")
ELSE(DEFINED TARG_OS)
    MESSAGE(FATAL_ERROR "TARG_OS not defined!")
ENDIF(DEFINED TARG_OS)

IF(DEFINED TARG_CPU)
    MESSAGE(STATUS "TARG_CPU set to ${TARG_CPU}")
ELSE(DEFINED TARG_CPU)
    MESSAGE(FATAL_ERROR "TARG_CPU not defined!")
ENDIF(DEFINED TARG_CPU)

IF(DEFINED TARG_PLAT)
    MESSAGE(STATUS "TARG_PLAT set to ${TARG_PLAT}")
ELSE(DEFINED TARG_PLAT)
    MESSAGE(FATAL_ERROR "TARG_PLAT not defined!")
ENDIF(DEFINED TARG_PLAT)

# Include all the target specific configurations
SET(CMAKE_PLATFORMS_PATH    ${CMAKE_CFG_DIR}/chameleonPlatforms)
INCLUDE(${CMAKE_PLATFORMS_PATH}/${TARG_OS}/CMakeDefs.txt)

SET(BIN_DIR ${CHAMELEON_DIR}/bin/${TARG_OS}_${TARG_CPU}_${TARG_PLAT})
#SET(LIB_DIR ${CHAMELEON_DIR}/lib/${TARG_OS}_${TARG_CPU}_${TARG_PLAT})
SET(LIB_DIR ${CHAMELEON_DIR}/bin/${TARG_OS}_${TARG_CPU}_${TARG_PLAT})

# Place all output to the Collate directory
SET(EXECUTABLE_OUTPUT_PATH  ${BIN_DIR})
SET(LIBRARY_OUTPUT_PATH     ${LIB_DIR})
SET(CMAKE_BUILD_TYPE        ${BUILDTYPE})
IF (TARG_CPU STREQUAL "x86")
    SET(CMAKE_PLATFORM   "Win32")
ELSE (TARG_CPU STREQUAL "x86")
    SET(CMAKE_PLATFORM "x64")
ENDIF (TARG_CPU STREQUAL "x86")

# Automatically add CMAKE_CURRENT_SOURCE_DIR and CMAKE_CURRENT_BINARY_DIR to the include directories in every processed CMakeLists.txt.
SET(CMAKE_INCLUDE_CURRENT_DIR                   TRUE)

# Order the include directories so that directories which are in the source or
# build tree always come before directories outside the project.
SET(CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE    TRUE)

# Add the collate directory to the library search path
LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})

# Specify the name of the project
PROJECT("chameleon")

#*************************************************************
# Print important info for debug
#*************************************************************
MESSAGE(STATUS "**************************************************************************")
MESSAGE(STATUS "CHAMELEON_DIR=${CHAMELEON_DIR}")
MESSAGE(STATUS "CMAKE_CFG_DIR=${CMAKE_CFG_DIR}")
MESSAGE(STATUS "CMAKE_PLATFORMS_PATH=${CMAKE_PLATFORMS_PATH}")
MESSAGE(STATUS "TARG_PLAT=${TARG_PLAT}")
MESSAGE(STATUS "ENABLE_UNIT_TEST=${ENABLE_UNIT_TEST}")
MESSAGE(STATUS "**************************************************************************")
MESSAGE(STATUS "PATH=$ENV{PATH}")
MESSAGE(STATUS "INCLUDE=$ENV{INCLUDE}")
MESSAGE(STATUS "LIB=$ENV{LIB}")
MESSAGE(STATUS "CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}")
MESSAGE(STATUS "EXECUTABLE_OUTPUT_PATH=${EXECUTABLE_OUTPUT_PATH}")
MESSAGE(STATUS "LIBRARY_OUTPUT_PATH=${LIBRARY_OUTPUT_PATH}")
MESSAGE(STATUS "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
MESSAGE(STATUS "CMAKE_PLATFORM=${CMAKE_PLATFORM}")
MESSAGE(STATUS "**************************************************************************")
MESSAGE(STATUS "PROJECT_BINARY_DIR=${PROJECT_BINARY_DIR}")
MESSAGE(STATUS "PROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}")
MESSAGE(STATUS "chameleon_BINARY_DIR=${chameleon_BINARY_DIR}")
MESSAGE(STATUS "chameleon_SOURCE_DIR=${chameleon_SOURCE_DIR}")
MESSAGE(STATUS "**************************************************************************")
MESSAGE(STATUS "CMAKE_C_FLAGS=${CMAKE_C_FLAGS}")
MESSAGE(STATUS "CMAKE_C_FLAGS_DEBUG=${CMAKE_C_FLAGS_DEBUG}")
MESSAGE(STATUS "CMAKE_C_FLAGS_RELEASE=${CMAKE_C_FLAGS_RELEASE}")
MESSAGE(STATUS "CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}")
MESSAGE(STATUS "CMAKE_CXX_FLAGS_DEBUG=${CMAKE_CXX_FLAGS_DEBUG}")
MESSAGE(STATUS "CMAKE_CXX_FLAGS_RELEASE=${CMAKE_CXX_FLAGS_RELEASE}")
MESSAGE(STATUS "CMAKE_CONFIGURATION_TYPES=${CMAKE_CONFIGURATION_TYPES}")
MESSAGE(STATUS "**************************************************************************")

#*************************************************************
# Add external library as prebuilt libraries
#*************************************************************
# Add ftd2xx.dll as a prebuilt library 
FILE(GLOB ftd2xx_libs "${CHAMELEON_DIR}/package/ftdi/${CMAKE_PLATFORM}/*")
FILE(COPY ${ftd2xx_libs} DESTINATION ${LIB_DIR}/${BUILDTYPE})
ADD_LIBRARY(ftd2xx SHARED IMPORTED)
SET_PROPERTY(TARGET ftd2xx PROPERTY IMPORTED_LOCATION ${LIB_DIR}/${BUILDTYPE}/ftd2xx.dll)
SET_PROPERTY(TARGET ftd2xx PROPERTY PROPERTY IMPORTED_IMPLIB ${LIB_DIR}/${BUILDTYPE}/ftd2xx.lib)

# Add gtestd.dll as a prebuilt library 		   
FILE(GLOB gtesd_libs "${CHAMELEON_DIR}/package/gtest/lib/${CMAKE_PLATFORM}/${CMAKE_BUILD_TYPE}/*")
FILE(COPY ${gtesd_libs} DESTINATION ${LIB_DIR}/${BUILDTYPE})
IF(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	ADD_LIBRARY(gtestd STATIC IMPORTED)
	SET_PROPERTY(TARGET gtestd PROPERTY IMPORTED_LOCATION ${LIB_DIR}/${BUILDTYPE}/gtestd.lib)
ELSE(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	ADD_LIBRARY(gtest STATIC IMPORTED)
	SET_PROPERTY(TARGET gtest PROPERTY IMPORTED_LOCATION ${LIB_DIR}/${BUILDTYPE}/gtest.lib)
ENDIF(${CMAKE_BUILD_TYPE} STREQUAL "Debug")

# Add libglog.dll and libglog_static.lib as a prebuilt library 
FILE(GLOB libglog_libs "${CHAMELEON_DIR}/package/glog/lib/${CMAKE_PLATFORM}/${CMAKE_BUILD_TYPE}/*")
FILE(COPY ${libglog_libs} DESTINATION ${LIB_DIR}/${BUILDTYPE})
ADD_LIBRARY(libglog SHARED IMPORTED)
SET_PROPERTY(TARGET libglog PROPERTY IMPORTED_LOCATION ${LIB_DIR}/libglog.dll)
SET_PROPERTY(TARGET libglog PROPERTY PROPERTY IMPORTED_IMPLIB ${LIB_DIR}/${BUILDTYPE}/libglog.lib)
ADD_LIBRARY(libglog_static STATIC IMPORTED)
SET_PROPERTY(TARGET libglog_static PROPERTY IMPORTED_LOCATION ${LIB_DIR}/${BUILDTYPE}/libglog_static.lib)

# Add stellerAlgoShared.dll as a prebuilt library 
FILE (GLOB stellerAlgoShared_libs "${CHAMELEON_DIR}/cmlFpTestEngine/FpImageProcessing/lib/${CMAKE_PLATFORM}/${CMAKE_BUILD_TYPE}/*")
FILE (COPY ${stellerAlgoShared_libs} DESTINATION ${LIB_DIR}/${BUILDTYPE})
ADD_LIBRARY(stellerAlgoShared SHARED IMPORTED)
SET_PROPERTY(TARGET stellerAlgoShared PROPERTY IMPORTED_LOCATION ${LIB_DIR}/${BUILDTYPE}/stellerAlgoShared.dll)
SET_PROPERTY(TARGET stellerAlgoShared PROPERTY PROPERTY IMPORTED_IMPLIB ${LIB_DIR}/${BUILDTYPE}/stellerAlgoShared.lib)

#*************************************************************
# Add submodule directory
#*************************************************************
# cmlFpSensorEngine
IF(${CML_BUILD_FP_SENSOR_ENGINE})
    ADD_SUBDIRECTORY(${CHAMELEON_DIR}/cmlFpSensorEngine cmlFpSensorEngine)
ENDIF(${CML_BUILD_FP_SENSOR_ENGINE})


# cmlFpTestEngine
IF(${CML_BUILD_FP_TEST_ENGINE})
    ADD_SUBDIRECTORY(${CHAMELEON_DIR}/cmlFpTestEngine cmlFpTestEngine)
ENDIF(${CML_BUILD_FP_TEST_ENGINE})

# cmlFpTestApp
IF(${CML_BUILD_FP_TEST_APP})
	# Find the Qt5Core/Qt5Core/Qt5Widgets/Qt5Xml/ActiveQt library
	FIND_PACKAGE(Qt5Core REQUIRED)
	FIND_PACKAGE(Qt5Gui REQUIRED)
	FIND_PACKAGE(Qt5Widgets REQUIRED)
	FIND_PACKAGE(Qt5Xml REQUIRED)
	FIND_PACKAGE(Qt5AxBase REQUIRED)
	FIND_PACKAGE(Qt5AxContainer REQUIRED)
    ADD_SUBDIRECTORY(${CHAMELEON_DIR}/cmlFpTestApp cmlFpTestApp)
ENDIF(${CML_BUILD_FP_TEST_APP})