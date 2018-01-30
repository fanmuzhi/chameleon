@ECHO OFF
SET QT_DIR_X64=C:\Qt\Qt5.5.1_x64\5.5\msvc2013_64\bin
SET BUILD_DIR=x64\Release
SET BUILD_DIR_INTERNAL=x64\Release_Internal
SET BUILD_DEMO_APP_DIR=cmlFpTestApp\DemoConsoleApp

SET REL_PKG_DIR=Steller_MT_x64_vX.X.X
SET SDC_EXE_DIR=SDC_Exe
SET SMDSP_APP_DIR=DEMO_APP\DemoConsoleApp
SET SMDSP_EXE_DIR=DEMO_Exe

SET REL_PKG_INTERNAL_DIR=Steller_MT_x64_Internal_vX.X.X

@echo.
@echo.
@echo. ======== Customer Release Package x64 ========
@echo.
@echo.
set path=%QT_DIR_X64%;%path%
path
@echo. 
@echo. 

IF EXIST %BUILD_DIR%\iconengines\  (
@echo. ============================================== 
@echo. ## Delete previous QT library ##
@echo. 
rmdir /s /q %BUILD_DIR%\iconengines
rmdir /s /q %BUILD_DIR%\imageformats
rmdir /s /q %BUILD_DIR%\platforms
rmdir /s /q %BUILD_DIR%\translations

del %BUILD_DIR%\libEGL.dll
del %BUILD_DIR%\libGLESV2.dll
del %BUILD_DIR%\opengl32sw.dll
del %BUILD_DIR%\Qt5Core.dll
del %BUILD_DIR%\Qt5Gui.dll
del %BUILD_DIR%\Qt5Svg.dll
del %BUILD_DIR%\Qt5Widgets.dll
)

%QT_DIR_X64%\windeployqt.exe %BUILD_DIR%\ChameleonQt.exe

IF EXIST %REL_PKG_DIR%\  (
rmdir /s /q %REL_PKG_DIR%
)
@echo. ============================================== 
@echo. ## Build SDC Execute Package ##
@echo.
@mkdir %REL_PKG_DIR%\%SDC_EXE_DIR%
@xcopy  /S /Q %BUILD_DIR% %REL_PKG_DIR%\%SDC_EXE_DIR%
@rm %REL_PKG_DIR%\%SDC_EXE_DIR%\BravoDebugTool.exe
@rm %REL_PKG_DIR%\%SDC_EXE_DIR%\DemoConSoleApp.exe
@rm %REL_PKG_DIR%\%SDC_EXE_DIR%\MTConfigGenerator.exe
@CALL :sub_rmGeneratedFile %REL_PKG_DIR%\%SDC_EXE_DIR%


@echo. ============================================== 
@echo. ## Build SMDSP Application Package ##
@echo.
@mkdir %REL_PKG_DIR%\%SMDSP_APP_DIR%
@xcopy /S /Q %BUILD_DEMO_APP_DIR% %REL_PKG_DIR%\%SMDSP_APP_DIR% 
@rmdir /S /Q %REL_PKG_DIR%\%SMDSP_APP_DIR%\Win32
@rmdir /S /Q %REL_PKG_DIR%\%SMDSP_APP_DIR%\x64\Debug
@rmdir /S /Q %REL_PKG_DIR%\%SMDSP_APP_DIR%\x64\Release_Internal
@rmdir /S /Q %REL_PKG_DIR%\%SMDSP_APP_DIR%\x64\Release\DemoConSoleApp.tlog
@del /S /Q %REL_PKG_DIR%\%SMDSP_APP_DIR%\x64\Release\*.*
@del %REL_PKG_DIR%\%SMDSP_APP_DIR%\*.csv
@del %REL_PKG_DIR%\%SMDSP_APP_DIR%\*.bmp
@xcopy %BUILD_DIR%\cmlLogging.dll %REL_PKG_DIR%\%SMDSP_APP_DIR%\x64\Release\
@xcopy %BUILD_DIR%\FpBridge.dll %REL_PKG_DIR%\%SMDSP_APP_DIR%\x64\Release\
@xcopy %BUILD_DIR%\FpImageProcessing.dll %REL_PKG_DIR%\%SMDSP_APP_DIR%\x64\Release\
@xcopy %BUILD_DIR%\FpModule.dll %REL_PKG_DIR%\%SMDSP_APP_DIR%\x64\Release\
@xcopy %BUILD_DIR%\ftd2xx64.dll %REL_PKG_DIR%\%SMDSP_APP_DIR%\x64\Release\
@xcopy %BUILD_DIR%\libglog.dll %REL_PKG_DIR%\%SMDSP_APP_DIR%\x64\Release\
@xcopy %BUILD_DIR%\NativeTestEngine.dll %REL_PKG_DIR%\%SMDSP_APP_DIR%\x64\Release\
@xcopy %BUILD_DIR%\NativeTestEngine.lib %REL_PKG_DIR%\%SMDSP_APP_DIR%\x64\Release\
@xcopy %BUILD_DIR%\stellerAlgoShared.dll %REL_PKG_DIR%\%SMDSP_APP_DIR%\x64\Release\


@echo. ============================================== 
@echo. ## Build SMDSP Execute Package ##
@echo.
@mkdir %REL_PKG_DIR%\%SMDSP_EXE_DIR%
@xcopy %BUILD_DEMO_APP_DIR%\DemoConfig.xml %REL_PKG_DIR%\%SMDSP_EXE_DIR%\
@xcopy %BUILD_DIR%\DemoConSoleApp.exe %REL_PKG_DIR%\%SMDSP_EXE_DIR%\
@xcopy %BUILD_DIR%\cmlLogging.dll %REL_PKG_DIR%\%SMDSP_EXE_DIR%\
@xcopy %BUILD_DIR%\FpBridge.dll %REL_PKG_DIR%\%SMDSP_EXE_DIR%\
@xcopy %BUILD_DIR%\FpImageProcessing.dll %REL_PKG_DIR%\%SMDSP_EXE_DIR%\
@xcopy %BUILD_DIR%\FpModule.dll %REL_PKG_DIR%\%SMDSP_EXE_DIR%\
@xcopy %BUILD_DIR%\ftd2xx64.dll %REL_PKG_DIR%\%SMDSP_EXE_DIR%\
@xcopy %BUILD_DIR%\libglog.dll %REL_PKG_DIR%\%SMDSP_EXE_DIR%\
@xcopy %BUILD_DIR%\NativeTestEngine.dll %REL_PKG_DIR%\%SMDSP_EXE_DIR%\
@xcopy %BUILD_DIR%\NativeTestEngine.lib %REL_PKG_DIR%\%SMDSP_EXE_DIR%\
@xcopy %BUILD_DIR%\stellerAlgoShared.dll %REL_PKG_DIR%\%SMDSP_EXE_DIR%\

 
@echo.
@echo.
@echo. ======== Internal Release Package x64 ========
@echo.
@echo.

IF EXIST %BUILD_DIR_INTERNAL%\iconengines\  (
@echo. ============================================== 
@echo. ## Delete previous QT library ##
@echo. 
rmdir /s /q %BUILD_DIR_INTERNAL%\iconengines
rmdir /s /q %BUILD_DIR_INTERNAL%\imageformats
rmdir /s /q %BUILD_DIR_INTERNAL%\platforms
rmdir /s /q %BUILD_DIR_INTERNAL%\translations

del %BUILD_DIR_INTERNAL%\libEGL.dll
del %BUILD_DIR_INTERNAL%\libGLESV2.dll
del %BUILD_DIR_INTERNAL%\opengl32sw.dll
del %BUILD_DIR_INTERNAL%\Qt5Core.dll
del %BUILD_DIR_INTERNAL%\Qt5Gui.dll
del %BUILD_DIR_INTERNAL%\Qt5Svg.dll
del %BUILD_DIR_INTERNAL%\Qt5Widgets.dll
)

%QT_DIR_X64%\windeployqt.exe %BUILD_DIR_INTERNAL%\ChameleonQt_InternalOnly.exe
 
IF EXIST %REL_PKG_INTERNAL_DIR%\  (
@rmdir /S /Q %REL_PKG_INTERNAL_DIR%
)
@mkdir %REL_PKG_INTERNAL_DIR%
@xcopy /S /Q %BUILD_DIR_INTERNAL% %REL_PKG_INTERNAL_DIR%
@CALL :sub_rmGeneratedFile %REL_PKG_INTERNAL_DIR%

GOTO sub_Exit
 
:sub_rmGeneratedFile
del %1\*.pdb
del %1\*.exp
del %1\*.metagen
del %1\*.lib
del %1\*.xml
EXIT /B
 
:sub_Exit
pause