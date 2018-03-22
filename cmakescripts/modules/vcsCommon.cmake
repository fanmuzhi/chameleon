# Setup the necessary cmake policy
if(COMMAND cmake_policy)
    # Libraries linked via full path no longer produce linker search paths.
    cmake_policy(SET CMP0003 NEW)
	# Included scripts do automatic cmake_policy PUSH and POP.
    cmake_policy(SET CMP0011 OLD)
	# Duplicate binary directories are not allowed.
    cmake_policy(SET CMP0013 OLD)
	# Policy CMP0012 is not set: if() recognizes numbers and boolean constants
    cmake_policy(SET CMP0012 NEW)
endif(COMMAND cmake_policy)

# Determine the tools path
IF($ENV{TOOLS} MATCHES ".+")
    file(TO_CMAKE_PATH "$ENV{TOOLS}" TOOLS)
ELSE($ENV{TOOLS} MATCHES ".+")
  IF(${TARG_OS} MATCHES "linux*" )
    MESSAGE ("${TARG_OS} needs no TOOLS")
  ELSE(${TARG_OS} MATCHES "linux*" )
    MESSAGE(FATAL_ERROR "Please setup the environment variable 'TOOLS' to the tools directory for OS:${TARG_OS}")
  ENDIF(${TARG_OS} MATCHES "linux*" )
ENDIF($ENV{TOOLS} MATCHES ".+")

# Select the target OS and CPU
SET(TARG_OS   "win" CACHE STRING "Select target OS")
SET(TARG_CPU  "x86" CACHE STRING "Select target CPU")
SET(TARG_PLAT "generic" CACHE STRING "Select target PLATFORM")

# Select the build type
SET(BUILDTYPE "Release" CACHE STRING "Select build type")

# Select IDE project file
SET(USE_IDE "Off" CACHE STRING "Select project file for IDE")

# Make sure CHAMELEON_DIR is defined
IF(DEFINED CHAMELEON_DIR)
    MESSAGE(STATUS "CHAMELEON_DIR set to ${CHAMELEON_DIR}")
ELSE(DEFINED CHAMELEON_DIR)
    IF(DEFINED USDK_ROOT_DIR)
        SET(CHAMELEON_DIR ${USDK_ROOT_DIR})
        MESSAGE(STATUS "CHAMELEON_DIR set to ${CHAMELEON_DIR}")
    ELSE(DEFINED USDK_ROOT_DIR)
        MESSAGE(FATAL_ERROR "CHAMELEON_DIR not defined!")
    ENDIF(DEFINED USDK_ROOT_DIR)
ENDIF(DEFINED CHAMELEON_DIR)

#IF(NOT DEFINED SDK_PLATFORM)
#    SET(SDK_PLATFORM    "usdk5")
#ENDIF(NOT DEFINED SDK_PLATFORM)
#MESSAGE(STATUS "SDK_PLATFORM set to ${SDK_PLATFORM}")

# Make sure TARG_OS and TARG_CPU are defined
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


# Include path to our custom CMake modules
LIST(APPEND CMAKE_MODULE_PATH   ${PROJECT_MODULE_DIR})
LIST(APPEND CMAKE_MODULE_PATH   ${CMAKE_CFG_DIR}/modules)

#IF (SDK_PLATFORM STREQUAL "nise")
#    SET(CMAKE_PLATFORMS_PATH    ${CMAKE_CFG_DIR}/nisePlatforms)
#ELSE (SDK_PLATFORM STREQUAL "nise")
#    SET(CMAKE_PLATFORMS_PATH    ${CMAKE_CFG_DIR}/platforms)
#ENDIF (SDK_PLATFORM STREQUAL "nise")

SET(CMAKE_PLATFORMS_PATH    ${CMAKE_CFG_DIR}/chameleonPlatforms)

# Directory of all the external 3rd party components
#SET(EXT_DIR ${CHAMELEON_DIR}/../external)

# Include all the target specific configurations
INCLUDE(${CMAKE_PLATFORMS_PATH}/${TARG_OS}/CMakeDefs.txt)

SET(BIN_DIR ${CHAMELEON_DIR}/bin/${TARG_OS}/${TARG_CPU}/${TARG_PLAT})
SET(LIB_DIR ${CHAMELEON_DIR}/lib/${TARG_OS}/${TARG_CPU}/${TARG_PLAT})

# Place all output to the Collate directory
SET(EXECUTABLE_OUTPUT_PATH  ${BIN_DIR})
SET(LIBRARY_OUTPUT_PATH     ${LIB_DIR})
SET(CMAKE_BUILD_TYPE        ${BUILDTYPE})
IF (TARG_CPU STREQUAL "x86")
    SET(CMAKE_PLATFORM   "Win32")
ELSE (TARG_CPU STREQUAL "x86")
    SET(CMAKE_PLATFORM "x64")
ENDIF (TARG_CPU STREQUAL "x86")


MESSAGE(STATUS "**************************************************************************************")
MESSAGE(STATUS "PROJECT_MODULE_DIR=${PROJECT_MODULE_DIR}")
MESSAGE(STATUS "CMAKE_MODULE_PATH=${CMAKE_MODULE_PATH}")
MESSAGE(STATUS "CMAKE_PLATFORMS_PATH=${CMAKE_PLATFORMS_PATH}")
MESSAGE(STATUS "BIN_DIR=${BIN_DIR}")
MESSAGE(STATUS "LIB_DIR=${LIB_DIR}")
MESSAGE(STATUS "EXECUTABLE_OUTPUT_PATH=${EXECUTABLE_OUTPUT_PATH}")
MESSAGE(STATUS "LIBRARY_OUTPUT_PATH=${LIBRARY_OUTPUT_PATH}")
MESSAGE(STATUS "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
MESSAGE(STATUS "CMAKE_PLATFORM=${CMAKE_PLATFORM}")
MESSAGE(STATUS "**************************************************************************************")

# Automatically add CMAKE_CURRENT_SOURCE_DIR and CMAKE_CURRENT_BINARY_DIR to
# the include directories in every processed CMakeLists.txt.
SET(CMAKE_INCLUDE_CURRENT_DIR                   TRUE)

# Order the include directories so that directories which are in the source or
# build tree always come before directories outside the project.
SET(CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE    TRUE)

# Add the collate directory to the library search path
LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})

# Include the feature configuration file. Any features not explicitly specified
# in the platform configuration file will be initialize with the default value.
INCLUDE(${CMAKE_CFG_DIR}/modules/vcsFeatures.cmake)
