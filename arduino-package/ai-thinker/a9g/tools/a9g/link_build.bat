@echo off
setlocal

set "BUILD_PATH=%~1"
set "PROJECT_NAME=%~2"
set "PLATFORM_PATH=%~3"
set "SDK_PATH=%~4"
set "CSDTK_PATH=%~5"
set "STAGE_ROOT=C:\A9G\arduino-build\%PROJECT_NAME%"
set "WORKSPACE_SDK=%STAGE_ROOT%\GPRS_C_SDK"
set "GENERATED_APP_SRC=%STAGE_ROOT%\generated\app\src"
set "GENERATED_APP_INC=%STAGE_ROOT%\generated\app\include"

if "%SDK_PATH%"=="" set SDK_PATH=C:\A9G\GPRS_C_SDK
if "%CSDTK_PATH%"=="" set CSDTK_PATH=C:\A9G\CSDTK4.2

if not exist "%SDK_PATH%\build.bat" (
    echo A9G SDK not found at %SDK_PATH%
    exit /b 1
)

if not exist "%CSDTK_PATH%\CSDTKvars.bat" (
    echo CSDTK not found at %CSDTK_PATH%
    exit /b 1
)

if not exist "%GENERATED_APP_SRC%\sketch.cpp" (
    echo No staged sketch found for %PROJECT_NAME%
    exit /b 1
)

if not exist "%STAGE_ROOT%" mkdir "%STAGE_ROOT%"
robocopy "%SDK_PATH%" "%WORKSPACE_SDK%" /E /XO /XD .git build hex >nul
if errorlevel 8 exit /b 1

robocopy "%GENERATED_APP_SRC%" "%WORKSPACE_SDK%\app\src" /E /NJH /NJS /NFL /NDL >nul
if errorlevel 8 exit /b 1
robocopy "%GENERATED_APP_INC%" "%WORKSPACE_SDK%\app\include" /E /NJH /NJS /NFL /NDL >nul
if errorlevel 8 exit /b 1
robocopy "%GENERATED_APP_INC%" "%WORKSPACE_SDK%\app\src" /E /NJH /NJS /NFL /NDL >nul
if errorlevel 8 exit /b 1
for /r "%GENERATED_APP_INC%" %%F in (*.h *.hpp *.hh *.inc) do copy /Y "%%F" "%WORKSPACE_SDK%\app\include\%%~nxF" >nul

set "GPRS_CSDTK42_PATH=%CSDTK_PATH%"
pushd "%WORKSPACE_SDK%"
call build.bat app
set BUILD_RESULT=%ERRORLEVEL%
popd

if not "%BUILD_RESULT%"=="0" exit /b %BUILD_RESULT%

if not exist "%BUILD_PATH%" mkdir "%BUILD_PATH%"
if exist "%WORKSPACE_SDK%\hex\app\app_flash.lod" copy /Y "%WORKSPACE_SDK%\hex\app\app_flash.lod" "%BUILD_PATH%\%PROJECT_NAME%.lod" >nul
if exist "%WORKSPACE_SDK%\build\app\app.elf" copy /Y "%WORKSPACE_SDK%\build\app\app.elf" "%BUILD_PATH%\%PROJECT_NAME%.elf" >nul
if exist "%WORKSPACE_SDK%\build\app\app.map" copy /Y "%WORKSPACE_SDK%\build\app\app.map" "%BUILD_PATH%\%PROJECT_NAME%.map" >nul

exit /b 0
