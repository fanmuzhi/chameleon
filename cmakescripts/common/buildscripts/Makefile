#/* !@file Makefile
# Makefile to build chameleon libraries
# *******************************************************************************
# **
# **                           NDA AND NEED-TO-KNOW REQUIRED
# **
# *****************************************************************************
# **
# **  Copyright (C) 2008-2018 Synaptics Incorporated. All rights reserved.
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


#-----------------------------------------------------------------------
#
# Used to specify sensor communication interface supported in the build.
# Possible values are:
build_host 		?= $(shell uname -s)

#-----------------------------------------------------------------------
#
# Directory where the CMake configuration files are located
#
export CHAMELEON_DIR 	?= $(shell pwd)
export CMAKE_CFG_DIR    := $(wildcard $(CHAMELEON_DIR)/cmakescripts)
#export CMAKE_MODULE_DIR := $(CMAKE_CFG_DIR)/modules

makefile_dir	:= $(CHAMELEON_DIR)/cmakescripts/common/cmake/

# Include host & platform specific makefile definitions
include $(CMAKE_CFG_DIR)/hosts/$(build_host)/Makefile.def

#-----------------------------------------------------------------------
#
# Determine the OS-specific console to redirect the output to
#
ifeq ($(build_host), WindowsNT) 
    export CURRENT_CONSOLE := "/con:"
else
    ifeq ($(build_host), Linux)
        export CURRENT_CONSOLE := "/dev/tty"
    endif
endif

#-----------------------------------------------------------------------
#
# Default Build Target
#
.PHONY: all
all:
	@$(ECHO) Default build target not supported. Use \"make help\" for supported targets. > $(CURRENT_CONSOLE)

#-----------------------------------------------------------------------
#
# Pre-build step to generate debug logs ID and <ID, log_msg> pair to the mapping file.
#
log_ids_gen: 

#-----------------------------------------------------------------------
#
# Windows targets
#

# M5 support is available for x86/x64
win_m5: win_m5_x86 win_m5_x64
win_m5_rel: win_m5_x86_rel win_m5_x64_rel 
win_m5_dbg: win_m5_x86_dbg win_m5_x64_dbg
win_m5_x86: win_m5_x86_dbg win_m5_x86_rel
win_m5_x64: win_m5_x64_dbg win_m5_x64_rel

win_m5_x86_dbg: log_ids_gen
	$(eval TAG_OS := win)
	@$(ECHO) Building win_x86 Debug... > $(CURRENT_CONSOLE)
	$(MAKE) -f $(makefile_dir)chameleon.mk TARG_OS=$(TAG_OS) TARG_CPU=x86 BUILDTYPE=Debug ENABLE_UNIT_TEST=$(ENABLE_UNIT_TEST)

win_m5_x86_rel: log_ids_gen
	$(eval TAG_OS := win)
	@$(ECHO) Building win_x86 Release... > $(CURRENT_CONSOLE)
	$(MAKE) -f $(makefile_dir)chameleon.mk TARG_OS=$(TAG_OS) TARG_CPU=x86 BUILDTYPE=Release ENABLE_UNIT_TEST=$(ENABLE_UNIT_TEST)

win_m5_x64_dbg: log_ids_gen
	$(eval TAG_OS := win)
	@$(ECHO) Building win_x64 Debug... > $(CURRENT_CONSOLE)
	$(MAKE) -f $(makefile_dir)chameleon.mk TARG_OS=$(TAG_OS) TARG_CPU=x64 BUILDTYPE=Debug ENABLE_UNIT_TEST=$(ENABLE_UNIT_TEST)

win_m5_x64_rel: log_ids_gen
	$(eval TAG_OS := win)
	@$(ECHO) Building win_x64 Release... > $(CURRENT_CONSOLE)
	$(MAKE) -f $(makefile_dir)chameleon.mk TARG_OS=$(TAG_OS) TARG_CPU=x64 BUILDTYPE=Release ENABLE_UNIT_TEST=$(ENABLE_UNIT_TEST)	


#-----------------------------------------------------------------------
#
# Clean target
#
clean:
	-@$(RM) -rf build 2> $(NUL)
	-@$(RM) -rf bin 2> $(NUL)
	-@$(RM) -rf lib 2> $(NUL)

#-----------------------------------------------------------------------
#
# Help target
#
help:
	@$(ECHO) "\nThe following build targets are supported..."
	@$(ECHO) "... clean ................. Remove all build genererated files"
	@$(ECHO) "... win ................... Build all Windows platforms"
	@$(ECHO) "... win_x86 ............... Windows X86 platforms"
	@$(ECHO) "... win_x64 ............... Windows X64 platforms"
	@$(ECHO) "\nThe following macros are supported to select the specific platform"
	@$(ECHO) "... USE_IDE ............... [*0 1] - Generate and build using MSVS project files"
	@$(ECHO) "... USE_VS2015 ............ [*0 1] - Use Visual Studio 2015. Applicable only for win"
	@$(ECHO) "\nThe following build arguments are supported..."
	@$(ECHO) "... CONFGEN_ONLY .......... [=true] - Generate target-specific configuration files only"
	@$(ECHO) "... LICENSED_SOURCE_RELEASE [=true] - Generate NISE Licensed Source release package"
	@$(ECHO) "... SOURCE_TREE_DEST ...... [=/path/to/dest/dir] - directory to copy NISE Licensed Source release package to\n"