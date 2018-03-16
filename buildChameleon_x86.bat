REM set BUILD_TARGET=win_mpc04_x86_dbg
REM set BUILD_TARGET=win_m5_x86_dbg
set BUILD_TARGET=win_m5_x86_dbg
set TARGET_PLATFORM=steller
set BUILD_ENABLE_UNIT_TEST=1

call cmakescripts\common\buildscripts\buildvars.bat C:\tools

echo.
echo PATH = %PATH%
echo HOME = %HOME%
echo.

set CMD=make -f cmakescripts\common\buildscripts\Makefile %BUILD_TARGET% TARG_PLAT=%TARGET_PLATFORM% BUILD_ENABLE_UNIT_TEST=%BUILD_ENABLE_UNIT_TEST%
echo "%CMD%"
%CMD%

pause
