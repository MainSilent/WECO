@echo off
setlocal

rem Get the directory of the batch script
set "script_dir=%~dp0"

rem Define the name of the service
set "service_name=eco_service"

rem Construct the absolute path to eco_service.exe
set "exe_path=%script_dir%\eco_service.exe"

rem Check if the service is running and stop it if it is
sc query %service_name% | find "STATE" | find "RUNNING" >nul
if %errorlevel% equ 0 (
    echo Stopping running service...
    sc stop %service_name%
    timeout /t 5 /nobreak >nul
)

rem Check if the service exists and delete it if it does
sc query %service_name% >nul 2>nul
if %errorlevel% equ 0 (
    echo Deleting existing service...
    sc delete %service_name%
    timeout /t 5 /nobreak >nul
)

rem Create the service
sc create %service_name% binPath= "%exe_path%"

rem Start the service
sc start %service_name%

endlocal
