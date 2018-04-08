REM cd chameeleon
call cmakescripts\common\buildscripts\buildvars.bat c:\tools
make -f cmakescripts\common\buildscripts\Makefile clean
REM cd ..

REM cd niteCore
REM call common\buildscripts\buildvars.bat c:\tools
REM make -f common\buildscripts\Makefile clean
REM cd ..

pause

REM echo .
REM echo Removing prebuilt folder ...
REM rmdir /s /q prebuilt
REM 
REM pause
