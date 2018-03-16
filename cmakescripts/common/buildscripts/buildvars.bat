@echo OFF

REM /* !@file buildvars.bat
REM *******************************************************************************
REM **
REM **                           NDA AND NEED-TO-KNOW REQUIRED
REM **
REM *****************************************************************************
REM **
REM **  Copyright (C) 2008-2017 Synaptics Incorporated. All rights reserved.
REM **
REM **
REM ** This file contains information that is proprietary to Synaptics
REM ** Incorporated ("Synaptics"). The holder of this file shall treat all
REM ** information contained herein as confidential, shall use the
REM ** information only for its intended purpose, and shall not duplicate,
REM ** disclose, or disseminate any of this information in any manner unless
REM ** Synaptics has otherwise provided express, written permission.
REM ** Use of the materials may require a license of intellectual property
REM ** from a third party or from Synaptics. Receipt or possession of this
REM ** file conveys no express or implied licenses to any intellectual
REM ** property rights belonging to Synaptics.
REM **
REM **
REM ** INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS," AND
REM ** SYNAPTICS EXPRESSLY DISCLAIMS ALL EXPRESS AND IMPLIED WARRANTIES,
REM ** INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
REM ** PARTICULAR PURPOSE, AND ANY WARRANTIES OF NON-INFRINGEMENT OF ANY
REM ** INTELLECTUAL PROPERTY RIGHTS. IN NO EVENT SHALL SYNAPTICS BE LIABLE
REM ** FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE, OR
REM ** CONSEQUENTIAL DAMAGES ARISING OUT OF OR IN CONNECTION WITH THE USE OF
REM ** THE INFORMATION CONTAINED IN THIS DOCUMENT, HOWEVER CAUSED AND BASED
REM ** ON ANY THEORY OF LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
REM ** NEGLIGENCE OR OTHER TORTIOUS ACTION, AND EVEN IF SYNAPTICS WAS ADVISED
REM ** OF THE POSSIBILITY OF SUCH DAMAGE. IF A TRIBUNAL OF COMPETENT
REM ** JURISDICTION DOES NOT PERMIT THE DISCLAIMER OF DIRECT DAMAGES OR ANY
REM ** OTHER DAMAGES, SYNAPTICS' TOTAL CUMULATIVE LIABILITY TO ANY PARTY
REM ** SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.
REM */


REM Setup TOOLS from the command line argument
if not _%1 == _    set TOOLS=%1

REM Setup TOOLS to default if not defined
if _%TOOLS% == _   set TOOLS=T:

set CMAKE=%TOOLS%\CMake-3.4.3
set UNIX_UTILS=%TOOLS%\UnixUtils
set PYTHON=%TOOLS%\Python2.6.1
set NINJA=%TOOLS%\ninja-1.6.0
set GIT=%TOOLS%\Git2.0.0

REM Check tools directory
if not exist %CMAKE%\bin\cmake.exe goto :notools
if not exist %UNIX_UTILS%\make.exe goto :notools
if not exist %GIT%\cmd\git.exe goto :notools

REM Tools found
echo buildvars.bat: Build tools were found under %TOOLS%
goto :setmore

REM Tools not found - report error with RED screen and WHITE text
:notools
color 4F
echo ***************************************************************************************
echo * buildvars.bat: Build tools were NOT FOUND in %TOOLS% - You must use proper tools!
echo * Check the presence of "%CMAKE%", "%UNIX_UTILS%", "%GIT%"
echo ***************************************************************************************

REM Setup paths for build tools
:setmore
REM Setup the PATH to make sure nothing unexpected is included
set PATH=%CMAKE%\BIN;%WINDIR%\SYSTEM32
set PATH=%GIT%\cmd;%PATH%
set PATH=%NINJA%\bin;%PATH%
set PATH=%UNIX_UTILS%;%PATH%
set PATH=%TOOLS%\BIN;%PATH%
set PATH=%PYTHON%;%PATH%

REM Clear any existing INCLUDE and LIB environment variables to avoid interference
set MSVCDIR=
set INCLUDE=
set LIB=

REM GIT is required this variable to locate SSH keys.
set HOME=%USERPROFILE%