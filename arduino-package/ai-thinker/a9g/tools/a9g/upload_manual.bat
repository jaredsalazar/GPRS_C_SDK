@echo off
setlocal EnableExtensions

set BUILD_PATH=%~1
set PROJECT_NAME=%~2
set SDK_PATH=%~3
set CSDTK_PATH=%~4
set SERIAL_PORT=%~5

if "%SDK_PATH%"=="" set SDK_PATH=C:\A9G\GPRS_C_SDK
if "%CSDTK_PATH%"=="" set CSDTK_PATH=C:\A9G\CSDTK4.2

set LOD_FILE=%BUILD_PATH%\%PROJECT_NAME%.lod
if exist "%BUILD_PATH%\%PROJECT_NAME%_flash.lod" set LOD_FILE=%BUILD_PATH%\%PROJECT_NAME%_flash.lod
if exist "%BUILD_PATH%\%PROJECT_NAME%_flash_debug.lod" set LOD_FILE=%BUILD_PATH%\%PROJECT_NAME%_flash_debug.lod
if not exist "%LOD_FILE%" (
    for %%F in ("%BUILD_PATH%\*_flash*.lod") do (
        if exist "%%~fF" set LOD_FILE=%%~fF
    )
)
if not exist "%LOD_FILE%" if exist "%SDK_PATH%\hex\app\app_flash.lod" set LOD_FILE=%SDK_PATH%\hex\app\app_flash.lod
if not exist "%LOD_FILE%" if exist "%SDK_PATH%\hex\app\app_flash_debug.lod" set LOD_FILE=%SDK_PATH%\hex\app\app_flash_debug.lod
if not exist "%LOD_FILE%" (
    for %%F in ("%SDK_PATH%\hex\app\*_flash*.lod") do (
        if exist "%%~fF" set LOD_FILE=%%~fF
    )
)
set FLASH_PROG=%CSDTK_PATH%\cooltools\chipgen\Modem2G\toolpool\plugins\fastpf\flash_programmers\host_8955_flsh_spi32m_ramrun.lod
set COOLTOOLS_DIR=%CSDTK_PATH%\cooltools
set PROFILES_DIR=%COOLTOOLS_DIR%\profiles
set PROFILE_NAME=a9g_arduino
set PROFILE_FILE=%PROFILES_DIR%\%PROFILE_NAME%.cwp
set CONFIG_FILE=%COOLTOOLS_DIR%\config.cfg
set PROFILE_SCRIPT=%COOLTOOLS_DIR%\rbbase\%PROFILE_NAME%.rb
set LOCAL_PROFILE_SCRIPT=%~dp0%PROFILE_NAME%.rb

set SERIAL_PORT_NUM=%SERIAL_PORT%
if /I "%SERIAL_PORT_NUM:~0,3%"=="COM" set SERIAL_PORT_NUM=%SERIAL_PORT_NUM:~3%
if "%SERIAL_PORT_NUM%"=="usb" set SERIAL_PORT_NUM=

if not exist "%LOD_FILE%" (
    echo A9G upload failed: LOD file not found.
    echo Expected: %LOD_FILE%
    exit /b 1
)

if not exist "%FLASH_PROG%" (
    echo A9G upload failed: 8955 flash programmer not found.
    echo Expected: %FLASH_PROG%
    exit /b 1
)

if not exist "%PROFILES_DIR%" mkdir "%PROFILES_DIR%"

(
echo ^<?xml version='1.0' encoding='UTF-8' ?^>
echo ^<profile^>
echo ^<name^>%PROFILE_NAME%^</name^>
echo ^<softDir^>chipgen/Modem2G^</softDir^>
echo ^<project^>8809 for [CT8851]^</project^>
echo ^<mainScript^>CoolWatcher/cwuser.rb^</mainScript^>
echo ^<hardXmd^>chipgen/Modem2G/toolpool/map/8809/8955_hard.xml^</hardXmd^>
echo ^<softXmd^>chipgen/Modem2G/toolpool/map/8809/8955_soft_pkg.xmd^</softXmd^>
echo ^<chipDie^>8955^</chipDie^>
echo ^<defaultTfg^>rbbase/common/plugins/tracesniffer3G/default.tfg^</defaultTfg^>
echo ^<elfdumpSetting^>chipgen/Modem2G/toolpool/map/elfdump/8955.xml^</elfdumpSetting^>
echo ^<gdbExe^>mips-gdb/bin/mips-elf-insight^</gdbExe^>
echo ^<lastCoolGDBConfiguration^>rbbase/common/plugins/CoolGDB/profiles/8809_xcpu.cgdb^</lastCoolGDBConfiguration^>
echo ^<lastFlashProgForFastpf^>%FLASH_PROG:\=/%^</lastFlashProgForFastpf^>
echo ^<lastLodForDownload^>%LOD_FILE:\=/%^</lastLodForDownload^>
echo ^<lastLodForRomburn^>^</lastLodForRomburn^>
echo ^<lastcomport^>%SERIAL_PORT_NUM%^</lastcomport^>
echo ^<pathToGDB^>C:/CSDTK/cygwin/coolgdb/bin^</pathToGDB^>
echo ^<pathToTraceDB^>chipgen/Modem2G/toolpool/map/2gTraceDB^</pathToTraceDB^>
echo ^<tracerSetting^>rbbase/common/plugins/tracer/default2g.ini^</tracerSetting^>
echo ^</profile^>
) > "%PROFILE_FILE%"

copy /Y "%LOCAL_PROFILE_SCRIPT%" "%PROFILE_SCRIPT%" >nul

(
echo ^<?xml version='1.0' encoding='UTF-8' ?^>
echo ^<config^>
echo ^<lastOpenedProfile name='%PROFILE_NAME%'/^>
echo ^</config^>
) > "%CONFIG_FILE%"

set A9G_ARDUINO_LOD=%LOD_FILE%
set A9G_ARDUINO_FLASHPROG=%FLASH_PROG%
set A9G_ARDUINO_COMPORT=%SERIAL_PORT_NUM%
set A9G_ARDUINO_AUTORUN=1

echo A9G upload is prepared for CoolWatcher.
echo Profile: %PROFILE_NAME%
echo LOD file: %LOD_FILE%
echo Flash programmer: %FLASH_PROG%
if not "%SERIAL_PORT_NUM%"=="" (
    echo Selected HST serial port: COM%SERIAL_PORT_NUM%
) else (
    echo No HST serial port was supplied by Arduino IDE.
)
echo CoolWatcher will open with the generated upload profile.

if exist "%CSDTK_PATH%\cooltools\coolwatcher.exe" (
    if /I "%A9G_ARDUINO_NO_LAUNCH%"=="1" (
        echo A9G_ARDUINO_NO_LAUNCH is set. Skipping CoolWatcher launch.
    ) else (
        start "" "%CSDTK_PATH%\cooltools\coolwatcher.exe"
    )
)

exit /b 0
