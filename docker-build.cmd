@echo off
setlocal enabledelayedexpansion

:: Get the directory of the batch file
for %%i in ("%~dp0.") do set "REPO=%%~fi"

:: Change to the repository directory
cd /d "%REPO%"

:: Get the Git commit hash as version
for /f "delims=" %%a in ('git log -n 1 --pretty=format:"%%H" -- Dockerfile') do set "VERSION=%%a"
set "IMG_NAME=mu_builder:!VERSION!"

:: Build the Docker image
docker build -t "!IMG_NAME!" .

:: Run the Docker container
docker run --rm -it -v "%REPO%:/app" "!IMG_NAME!" /runasuser.sh --uid 1000 --command "./build_uefi.sh %*"

endlocal
