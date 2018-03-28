REM set BUILD_TARGET=win_m5_x86_dbg
REM set BUILD_TARGET=win_m5_x86_rel
set BUILD_TARGET=win_m5_x86
set TARGET_PLATFORM=steller
set ENABLE_UNIT_TEST=1

call cmakescripts\common\buildscripts\buildvars.bat C:\tools

echo.
echo PATH = %PATH%
echo HOME = %HOME%
echo.

set CMD=make -f cmakescripts\common\buildscripts\Makefile %BUILD_TARGET% TARG_PLAT=%TARGET_PLATFORM% ENABLE_UNIT_TEST=%ENABLE_UNIT_TEST%
echo "%CMD%"
%CMD%

pause
