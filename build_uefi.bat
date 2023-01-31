@echo off

::Set Default Variables
set TARGET_DEVICE=
set TARGET_MEM_SIZE=
set MULTIPLE_MEM_SIZE=FALSE
set TARGET_BUILD_MODE=RELEASE
set UNTESTED_DEVICE=FALSE
set IGNORE_WARNING=FALSE
set MULTIPLE_DISPLAY_PANEL=FALSE
set DISPLAY_PANEL=
set SOC_PLATFORM=

::Get Parameters from cmd
set TARGET_DEVICE=%1
if "%1" == "" goto Help
set TARGET_BUILD_MODE=%2
set TARGET_MEM_SIZE=%3
set DISPLAY_PANEL=%4
set IGNORE_WARNING=%5

::Get Parameters from Device Config
if NOT exist %CD%\configs\devices\%TARGET_DEVICE%.cmd (echo Device configuration not found & exit /B) else (call %CD%\configs\devices\%TARGET_DEVICE%.cmd)
if NOT exist %CD%\configs\%SOC_PLATFORM%.cmd (echo SoC configuration not found) else (call %CD%\configs\%SOC_PLATFORM%.cmd)

::Check Parameters (NOTE: Needs more care)
if "%TARGET_MEM_SIZE%" == "" (
    if %MULTIPLE_MEM_SIZE% == TRUE (
        echo.
        echo ==================[ ERROR ]==================
        echo The Device you chose has more than one Mem Size!
        echo Use SIZE to define how much Mem your Device has.
        echo =============================================
        exit /B
    )
)
if "%DISPLAY_PANEL%" == "" (
    if %MULTIPLE_DISPLAY_PANEL% == TRUE (
        echo.
        echo ==================[ ERROR ]==================
        echo The Device you chose has more than one Display Panel!
        echo Use PANEL to define wich panel your Device uses.
        echo =============================================
        exit /B
    )
)
if "%IGNORE_WARNING%" == TRUE ( echo Warning skipped) else (
    if %UNTESTED_DEVICE% == TRUE (
        echo.
        echo =================[ WARNING ]=================
        echo The Device you chosse to build is not Tested!
        echo We are not responsible for bricked devices.
        echo Use IGNORE to build %TARGET_DEVICE%.
        echo =============================================
        exit /B
    )
)

::Check if Python is Installed (NOTE: Needs more care)
where python > %TEMP%\python.tmp
set /p python=<%TEMP%\python.tmp
del %TEMP%\python.tmp
if "%python%" == "" ( echo Python could not been found! & exit /B )

:: Start the actual UEFI Build
python "%python%\..\Scripts\stuart_setup.exe" -c "Platforms/%SOC_PLATFORM%Pkg/PlatformBuild.py" TOOL_CHAIN_TAG=CLANG38 || exit /B
python "%python%\..\Scripts\stuart_update.exe" -c "Platforms/%SOC_PLATFORM%Pkg/PlatformBuild.py" TOOL_CHAIN_TAG=CLANG38 || exit /B
python "%python%\..\Scripts\stuart_build.exe" -c "Platforms/%SOC_PLATFORM%Pkg/PlatformBuild.py" TOOL_CHAIN_TAG=CLANG38 "TARGET=%TARGET_BUILD_MODE%" "TARGET_DEVICE=%TARGET_DEVICE%" "FD_BASE=%FD_BASE%" "FD_SIZE=%FD_SIZE%" "MEM_SIZE=%TARGET_MEM_SIZE%" "DISPLAY_PANEL=%DISPLAY_PANEL%" || exit /B
exit /B

:Help
echo Usage: build_uefi.bat DEV [MODE] [SIZE] [PANEL] [IGNORE]
echo.
echo Build MU UEFI for Qualcomm Snapdragon platforms.
echo.
echo Options:
echo    DEV:            build for DEV.
echo    MODE:           Release mode for building, default is 'RELEASE', 'DEBUG' alternatively.
echo    SIZE:           Define how much Memory your Device has.
echo    PANEL:          Define what Display Panel your Device uses.
echo    IGNORE:         If used all Script Warnings will be disabled.
echo.
echo MainPage: https://github.com/Robotix22/MU-Qcom
exit /B
