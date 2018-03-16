#/* !@file CMakeLists.txt
# *******************************************************************************
# **
# **                           NDA AND NEED-TO-KNOW REQUIRED
# **
# *****************************************************************************
# **
# **  Copyright (C) 2016-2017 Synaptics Incorporated. All rights reserved.
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
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)


# Name of the cmake project we are building
# This name will be used to reference the project specific options definitions
SET(CMAKE_PROJECT   "chameleon")

# Specify the SDK platform name: usdk5 or chameleon
#SET(SDK_PLATFORM    "chameleon")

# Setup the chameleon root directory
file(TO_CMAKE_PATH "$ENV{CHAMELEON_DIR}" CHAMELEON_DIR)
#file(TO_CMAKE_PATH "$ENV{NISECORE_DIR}" NISECORE_DIR)
file(TO_CMAKE_PATH "$ENV{CMAKE_CFG_DIR}" CMAKE_CFG_DIR)
file(TO_CMAKE_PATH "$ENV{CMAKE_MODULE_DIR}" CMAKE_MODULE_DIR)

# Include the VCS common definitions
#export CMAKE_MODULE_DIR := $(CMAKE_CFG_DIR)/modules
INCLUDE(${CMAKE_CFG_DIR}/modules/vcsCommon.cmake)

# Specify the name of the project
PROJECT("chameleon")


#*************************************************************
# Add external library as prebuilt libraries
#*************************************************************
# Add ftd2xx.dll as a prebuilt library 
FILE (GLOB ftd2xx_libs "${CHAMELEON_DIR}/package/ftdi/Win32/*")
FILE (COPY ${ftd2xx_libs} DESTINATION ${LIB_DIR})
ADD_LIBRARY(ftd2xx SHARED IMPORTED)
SET_PROPERTY(TARGET ftd2xx PROPERTY IMPORTED_LOCATION ${LIB_DIR}/ftd2xx.dll)
SET_PROPERTY(TARGET ftd2xx PROPERTY PROPERTY IMPORTED_IMPLIB ${LIB_DIR}/ftd2xx.lib)

# Add gtestd.dll as a prebuilt library 			   
FILE (GLOB gtesd_libs "${CHAMELEON_DIR}/package/gtest/lib/Win32/${BUILDTYPE}/*")
FILE (COPY ${gtesd_libs} DESTINATION ${LIB_DIR})
ADD_LIBRARY(gtestd STATIC IMPORTED)
SET_PROPERTY(TARGET gtestd PROPERTY IMPORTED_LOCATION ${LIB_DIR}/gtestd.lib)

# Add libglog.dll and libglog_static.lib as a prebuilt library 
FILE (GLOB libglog_libs "${CHAMELEON_DIR}/package/glog/lib/Win32/Debug/*")
FILE (COPY ${libglog_libs} DESTINATION ${LIB_DIR})
ADD_LIBRARY(libglog SHARED IMPORTED)
SET_PROPERTY(TARGET libglog PROPERTY IMPORTED_LOCATION ${LIB_DIR}/libglog.dll)
SET_PROPERTY(TARGET libglog PROPERTY PROPERTY IMPORTED_IMPLIB ${LIB_DIR}/libglog.lib)
ADD_LIBRARY(libglog_static STATIC IMPORTED)
SET_PROPERTY(TARGET libglog_static PROPERTY IMPORTED_LOCATION ${LIB_DIR}/libglog_static.lib)

# Add stellerAlgoShared.dll as a prebuilt library 
FILE (GLOB stellerAlgoShared_libs "${CHAMELEON_DIR}/cmlFpTestEngine/FpImageProcessing/lib/Win32/${BUILDTYPE}/*")
FILE (COPY ${stellerAlgoShared_libs} DESTINATION ${LIB_DIR})
ADD_LIBRARY(stellerAlgoShared SHARED IMPORTED)
SET_PROPERTY(TARGET stellerAlgoShared PROPERTY IMPORTED_LOCATION ${LIB_DIR}/stellerAlgoShared.dll)
SET_PROPERTY(TARGET stellerAlgoShared PROPERTY PROPERTY IMPORTED_IMPLIB ${LIB_DIR}/stellerAlgoShared.lib)

#*************************************************************
# Add submodule directory
#*************************************************************
# cmlFpSensorEngine
IF (${CML_BUILD_FP_SENSOR_ENGINE})
    ADD_SUBDIRECTORY(${CHAMELEON_DIR}/cmlFpSensorEngine cmlFpSensorEngine)
ENDIF (${CML_BUILD_FP_SENSOR_ENGINE})


# cmlFpTestEngine
IF (${CML_BUILD_FP_TEST_ENGINE})
    ADD_SUBDIRECTORY(${CHAMELEON_DIR}/cmlFpTestEngine cmlFpTestEngine)
ENDIF (${CML_BUILD_FP_TEST_ENGINE})

# cmlFpTestApp
IF (${CML_BUILD_FP_TEST_APP})
    ADD_SUBDIRECTORY(${CHAMELEON_DIR}/cmlFpTestApp cmlFpTestApp)
ENDIF (${CML_BUILD_FP_TEST_APP})
