#/* !@file chameleon.mk
# Makefile to build chameleon libraries
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


#export build_host ?= $(shell uname -s)

# Default target platform configurations 
TARG_OS	      := win
TARG_CPU      := x86
TARG_PLAT     := generic
BUILDTYPE     := Release
CODE_ANALYZE  := OFF 

export TARG_OS
export TARG_CPU
export TARG_PLAT
export BUILDTYPE
#export CODE_ANALYZE

export TARG_BUILD_PATH  := $(TARG_OS)_$(TARG_CPU)_$(TARG_PLAT)
export TARG_OUT_PATH    := $(TARG_OS)/$(TARG_CPU)/$(TARG_PLAT)

# Include host & platform specific makefile definitions
include $(CMAKE_CFG_DIR)/chameleonPlatforms/$(TARG_OS)/Makefile.def

export src_dir 	 := $(CHAMELEON_DIR)/cmakescripts/common/cmake
export build_dir := build/$(TARG_BUILD_PATH)/$(BUILDTYPE)


cmake_flags += -D TARG_OS=$(TARG_OS)
cmake_flags += -D TARG_CPU=$(TARG_CPU)
cmake_flags += -D TARG_PLAT=$(TARG_PLAT) 
cmake_flags += -D BUILDTYPE=$(BUILDTYPE)
#cmake_flags += -D T_ARCH_ABI=$(T_ARCH_ABI) 
#cmake_flags += -D COMIFACE=$(COMIFACE)
#cmake_flags += -D ENABLE_MPC04=$(ENABLE_MPC04)
#cmake_flags += -D ENABLE_M5=$(ENABLE_M5)
#cmake_flags += -D CODE_ANALYZE=$(CODE_ANALYZE)

$(warning  ***********************************************************************************************************)
$(warning  TARG_OS=$(TARG_OS))
$(warning  TARG_CPU=$(TARG_CPU))
$(warning  TARG_PLAT=$(TARG_PLAT))
$(warning  BUILDTYPE=$(BUILDTYPE))
$(warning  CHAMELEON_DIR=$(CHAMELEON_DIR))
$(warning  CMAKE_CFG_DIR=$(CMAKE_CFG_DIR))
#$(warning  CMAKE_MODULE_DIR=$(CMAKE_MODULE_DIR))
$(warning  src_dir=$(src_dir))
$(warning  build_dir=$(build_dir))
$(warning  cmake_flags=$(cmake_flags))
#$(warning  CODE_ANALYZE=$(CODE_ANALYZE))
$(warning  PATH=$(PATH))
$(warning  ***********************************************************************************************************)

#-----------------------------------------------------------------------
#
# Default Build Target
#
.PHONY: all
   
all:	chameleon

.PHONY: chameleon
chameleon:	$(build_dir)/CMakeCache.txt
	cd $(build_dir) && $(build_cmd)	
	$(CP) $(CHAMELEON_DIR)/lib/win/$(TARG_CPU)/$(TARG_PLAT)/*.lib	$(CHAMELEON_DIR)/bin/win/$(TARG_CPU)/$(TARG_PLAT)/$(BUILDTYPE)
	$(CP) $(CHAMELEON_DIR)/lib/win/$(TARG_CPU)/$(TARG_PLAT)/*.dll	$(CHAMELEON_DIR)/bin/win/$(TARG_CPU)/$(TARG_PLAT)/$(BUILDTYPE)
	$(CP) $(CHAMELEON_DIR)/lib/win/$(TARG_CPU)/$(TARG_PLAT)/$(BUILDTYPE)/*.lib	$(CHAMELEON_DIR)/bin/win/$(TARG_CPU)/$(TARG_PLAT)/$(BUILDTYPE)
	$(CP) $(CHAMELEON_DIR)/lib/win/$(TARG_CPU)/$(TARG_PLAT)/$(BUILDTYPE)/*.dll	$(CHAMELEON_DIR)/bin/win/$(TARG_CPU)/$(TARG_PLAT)/$(BUILDTYPE)
	$(WINDEPLOYQT) $(CHAMELEON_DIR)/bin/win/$(TARG_CPU)/$(TARG_PLAT)/$(BUILDTYPE)
$(build_dir):
	@$(MKDIR) $(build_dir)

$(build_dir)/CMakeCache.txt: confgen-only

#-----------------------------------------------------------------------
#
# CMake configuration generation-only target
#
confgen-only: $(build_dir)
	@$(ECHO) "\nStarting the configuration generation..." > $(CURRENT_CONSOLE)
	cd $(build_dir) && $(CMAKE) -G $(cmake_generator) $(cmake_flags) $(src_dir)
	@$(ECHO) "Done!\n" > $(CURRENT_CONSOLE)



#-----------------------------------------------------------------------
#
# Licensed Source Release Package Generation Target
#
licensedSourceRelease: chameleon
	@$(ECHO) "\nPreparing to copy the NISE SLA-source tree..." > $(CURRENT_CONSOLE)
	@$(CMAKE_CFG_DIR)/sourceRelease/copySourceTree.sh $(SOURCE_TREE_DEST)
