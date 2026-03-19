@echo off
setlocal EnableDelayedExpansion

set "SRC=%~1"
set "OBJ=%~2"
set "BUILD_PATH=%~3"
set "PLATFORM_PATH=%~4"
set "SDK_PATH=%~5"
set "PROJECT_NAME=%~6"
set "STAGE_ROOT=C:\A9G\arduino-build\%PROJECT_NAME%"
set "GENERATED_APP_SRC=%STAGE_ROOT%\generated\app\src"
set "GENERATED_APP_INC=%STAGE_ROOT%\generated\app\include"

for %%I in ("%OBJ%") do (
    if not exist "%%~dpI" mkdir "%%~dpI"
)

if not exist "%GENERATED_APP_SRC%" mkdir "%GENERATED_APP_SRC%"
if not exist "%GENERATED_APP_INC%" mkdir "%GENERATED_APP_INC%"

set "SRC_NAME=%~nx1"
set "SRC_EXT=%~x1"
set "SRC_DIR=%~dp1"

if /I "%SRC_EXT%"==".cpp" goto stage_source
if /I "%SRC_EXT%"==".c" goto stage_source
if /I "%SRC_EXT%"==".S" goto stage_source
goto finalize

:stage_source
if /I "%SRC_NAME:~-8%"==".ino.cpp" (
    copy /Y "%SRC%" "%GENERATED_APP_SRC%\sketch.cpp" >nul
) else (
    set "A9G_COMPILE_SRC=%SRC%"
    for /f "usebackq tokens=1,2,3 delims=|" %%A in (`powershell -NoProfile -Command "$src=[IO.Path]::GetFullPath($env:A9G_COMPILE_SRC); $dir=Split-Path $src -Parent; $name=[IO.Path]::GetFileName($src); if($src -like '*\src\*'){ $parts=$src -split '\\src\\',2; $root=$parts[0] + '\src'; $rel=$parts[1]; $bucket=Split-Path (Split-Path $root -Parent) -Leaf; } else { $root=$dir; $rel=$name; $bucket='sketch'; } Write-Output ($root + '|' + $rel + '|' + $bucket)"`) do (
        set "ROOT_DIR=%%A"
        set "REL_FILE=%%B"
        set "BUCKET=%%C"
    )
    set "A9G_COMPILE_SRC="

    set "DEST_SRC=%GENERATED_APP_SRC%\libraries\!BUCKET!\!REL_FILE!"
    for %%I in ("!DEST_SRC!") do (
        if not exist "%%~dpI" mkdir "%%~dpI"
        set "DEST_SRC_DIR=%%~dpI"
    )
    copy /Y "%SRC%" "!DEST_SRC!" >nul

    set "DEST_INC_DIR=%GENERATED_APP_INC%\libraries\!BUCKET!"
    if not exist "!DEST_INC_DIR!" mkdir "!DEST_INC_DIR!"
    robocopy "!ROOT_DIR!" "!DEST_SRC_DIR!" *.h *.hpp *.hh *.inc /S /NJH /NJS /NFL /NDL >nul
    robocopy "!ROOT_DIR!" "!DEST_INC_DIR!" *.h *.hpp *.hh *.inc /S /NJH /NJS /NFL /NDL >nul
)

:finalize
type nul > "%OBJ%"
exit /b 0
