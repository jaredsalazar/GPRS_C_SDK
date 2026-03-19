@echo off
setlocal

set ARCHIVE=%~1
for %%I in ("%ARCHIVE%") do (
    if not exist "%%~dpI" mkdir "%%~dpI"
)

type nul > "%ARCHIVE%"
exit /b 0
