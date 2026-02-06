#!/bin/bash

# Help Message Function
function _help(){
	echo "Usage: ./build_uefi.sh -d <Codename> [-r <Build Mode>] [-m <Device Model>]"
	echo
	echo "Build Project Silicium UEFI for defined ARM64-Based Devices."
	echo
	echo "Options:"
	echo "	--device <Codename>, -d <Codename>:         Builds defined Device."
	echo "	--release <Build Mode>, -r <Build Mode>:    Defines the Release Type of the Build."
	echo "	--model <Device Model>, -m <Device Model>:  Defines the Model Type of the Selected Target Device."
	echo "	--disable-secureboot, -i:                   Disables Secure Boot."
	echo "	--clean, -c:                                Removes Old Build Files and Starts a Clean Build."
	echo "	--help, -h:                                 Shows this Help."
	echo
	echo "MainPage: https://github.com/Project-Silicium/Mu-Silicium"
	exit 1
}

# Message Functions (Error & Warning)
function _error(){ echo -e "\033[1;31m${@}\033[0m" >&2; exit 1; }
function _warn(){ echo -e "\033[0;33m${@}\033[0m" >&2; }

# Check for Parameters
OPTS="$(getopt -o d:r:m:ich -l device:,release:,model:,disable-secureboot,clean,help -n 'build_uefi.sh' -- "$@")" || exit 1
eval set -- "${OPTS}"

# Set Default Security Argument
TARGET_DISABLE_SECUREBOOT=0

# Parse Parameters
while true
do case "${1}" in
		-d|--device) TARGET="${2}";shift 2;;
		-r|--release) TARGET_BUILD_MODE="${2}";shift 2;;
		-m|--model) TARGET_MODEL="${2}";shift 2;;
		-i|--disable-secureboot) TARGET_DISABLE_SECUREBOOT=1;shift;;
		-c|--clean) DO_CLEAN_BUILD=1;shift;;
		-h|--help) _help 0;shift;;
		--) shift;break;;
		*) _help 1;;
	esac
done

# Check Device Parameter
if [ -z "$TARGET" ]
then _help
fi

# Check Device Model Parameter
if [ -z "$TARGET_MODEL" ]
then TARGET_MODEL=0
fi

# Find Device Folder
TARGET_PATH=$(find ./Platforms -type d -name "${TARGET}Pkg" -print -quit)

# Verify Device Path
if [ -z "$TARGET_PATH" ]
then _error "\nThere are no Build Files for $TARGET!\n"
fi

# Parse Device Config File
if [ -f "Resources/Configs/$TARGET.conf" ]
then source "Resources/Configs/$TARGET.conf"
else _error "\nDevice Configuration not found for $TARGET!\nCheck if your .conf File is in the \"Resources/Configs\" Folder.\n"
fi

# Delete Old Build Files
if [[ ${DO_CLEAN_BUILD} == 1 ]];
then rm -r Build &> /dev/null
fi

# Delete left over Output Files
rm -r Conf &> /dev/null
rm ./BootShim/BootShim.bin &> /dev/null
rm ./BootShim/BootShim.elf &> /dev/null
rm ./Resources/bootpayload.bin &> /dev/null
rm Mu-$TARGET.* &> /dev/null

# Remove Mu_Tiano_Plus Patches
pushd Common/Mu_Tiano_Plus  &> /dev/null || exit 1
git apply -R Auth-Service.patch &> /dev/null
rm Auth-Service.patch &> /dev/null
popd &> /dev/null

# Remove Mu_Basecore Patches
pushd Mu_Basecore  &> /dev/null || exit 1
git apply -R BdsWait.patch &> /dev/null
git apply -R UsbBus.patch &> /dev/null
rm BdsWait.patch &> /dev/null
rm UsbBus.patch &> /dev/null
popd &> /dev/null

# Remove Mu_Tiano Patches
pushd Silicon/Arm/Mu_Tiano  &> /dev/null || exit 1
git apply -R Timer.patch &> /dev/null
rm Timer.patch &> /dev/null
popd &> /dev/null

# Set Release Type of UEFI
while true
do case "${TARGET_BUILD_MODE^^}" in
		DEBUG) TARGET_BUILD_MODE=DEBUG;break;;
		*) TARGET_BUILD_MODE=RELEASE;break;;
	esac
done

# Calculate FD Block Size
TARGET_FD_BLOCKS="$(printf "0x%x" $(($TARGET_FD_SIZE / 0x1000)))"

# Compile BootShim
pushd BootShim  &> /dev/null || exit 1
make REQUIRES_KERNEL_HEADER=$TARGET_REQUIRES_KERNEL_HEADER FD_BASE=$TARGET_FD_BASE FD_SIZE=$TARGET_FD_SIZE || _error "\nFailed to Compile BootShim!\n"
popd &> /dev/null

# Setup & Update UEFI Environment
python3 "$TARGET_PATH/DeviceBuild.py" --setup -t $TARGET_BUILD_MODE || _error "\nFailed to Setup UEFI Env!\n"
python3 "$TARGET_PATH/DeviceBuild.py" --update -t $TARGET_BUILD_MODE || _error "\nFailed to Update UEFI Env!\n"

# Copy Mu Patches to the Right Location
cp Resources/MuPatches/Auth-Service.patch Common/Mu_Tiano_Plus/ || exit 1
cp Resources/MuPatches/BdsWait.patch Resources/MuPatches/UsbBus.patch Mu_Basecore/ || exit 1
cp Resources/MuPatches/Timer.patch Silicon/Arm/Mu_Tiano/ || exit 1

# Apply Mu_Tiano_Plus Patches
pushd Common/Mu_Tiano_Plus  &> /dev/null || exit 1
git apply Auth-Service.patch &> /dev/null
popd &> /dev/null

# Apply Mu_Basecore Patches
pushd Mu_Basecore  &> /dev/null || exit 1
git apply BdsWait.patch &> /dev/null
git apply UsbBus.patch &> /dev/null
popd &> /dev/null

# Apply Mu_Tiano Patches
pushd Silicon/Arm/Mu_Tiano  &> /dev/null || exit 1
git apply Timer.patch &> /dev/null
popd &> /dev/null

# Start the Real UEFI Build
python3 "$TARGET_PATH/DeviceBuild.py" "TARGET=$TARGET_BUILD_MODE" "DISABLE_SECUREBOOT=$TARGET_DISABLE_SECUREBOOT" "FD_BASE=$TARGET_FD_BASE" "FD_SIZE=$TARGET_FD_SIZE" "FD_BLOCKS=$TARGET_FD_BLOCKS" "DEVICE_MODEL=$TARGET_MODEL" || _error "\nFailed to Compile UEFI!\n"

# Run Device Specific Image Creation
if [ -f "Resources/Scripts/$TARGET.sh" ]
then source Resources/Scripts/$TARGET.sh
else _warn "\nImage Creation Script of $TARGET was not Found!\nNo Output Image Was Generated.\n"
fi

# Check for Updates
git fetch &> /dev/null
UPDATE_CHECK=$(git status)
if [[ ${UPDATE_CHECK} == *"git pull"* ]];
then _warn "\nYou are using an old Version of Mu-Silicium.\nPlease Update to use newest Fixes and Features.\nUse \"git pull\" to Update your Local Repo.\n"
fi
