@echo off
setlocal

set BUILD_PATH=%~1
set PROJECT_NAME=%~2
set SDK_PATH=%~3

if exist "%BUILD_PATH%\%PROJECT_NAME%.lod" exit /b 0

if exist "%SDK_PATH%\hex\app\app_flash.lod" (
    copy /Y "%SDK_PATH%\hex\app\app_flash.lod" "%BUILD_PATH%\%PROJECT_NAME%.lod" >nul
    exit /b 0
)

exit /b 1
