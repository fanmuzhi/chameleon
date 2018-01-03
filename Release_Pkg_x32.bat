@ECHO OFF
SET QT_DIR_X32=C:\Qt\Qt5.5.1\5.5\msvc2013\bin
SET BUILD_DIR=Win32\Release
SET BUILD_DIR_INTERNAL=Win32\Release_Internal
SET BUILD_DEMO_APP_DIR=cmlFpTestApp\DemoConsoleApp

SET REL_PKG_DIR=Steller_MT_Win32_vX.X.X
SET SDC_EXE_DIR=SDC_Exe
SET SMDSP_APP_DIR=SMDSP_APP\DemoConsoleApp
SET SMDSP_EXE_DIR=SMDSP_Exe

SET REL_PKG_INTERNAL_DIR=Steller_MT_Win32_Internal_vX.X.X

@echo.
@echo.
@echo. ======== Customer Release Package Win32 ========
@echo.
@echo.
set path=%QT_DIR_X32%;%path%
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

%QT_DIR_X32%\windeployqt.exe %BUILD_DIR%\ChameleonQt.exe

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

@echo.
@echo.
@echo. ======== Internal Release Package x32 ========
@echo.
@echo.
%QT_DIR_X32%\windeployqt.exe %BUILD_DIR_INTERNAL%\ChameleonQt_InternalOnly.exe
 
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