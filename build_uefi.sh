#!/bin/bash

# Help Message Function
function _help(){
	echo "Usage: ./build_uefi.sh -d <Codename> [-r <Build Mode>] [-m <Device Model>]"
	echo
	echo "Build Project Silicium UEFI for defined ARM-Based Devices."
	echo
	echo "Options:"
	echo "	--device <Codename>, -d <Codename>:         Builds defined Device."
	echo "	--release <Build Mode>, -r <Build Mode>:    Defines the Release Type of the Build."
	echo "	--model <Device Model>, -m <Device Model>:  Defines the Model Type of the Selected Target Device."
	echo "  --clean, -c:                                Removes Old Build Files and Starts a Clean Build."
	echo "	--help, -h:                                 Shows this Help."
	echo
	echo "MainPage: https://github.com/Project-Silicium/Mu-Silicium"
	exit 1
}

# Message Functions (Error & Warning)
function _error(){ echo -e "\033[1;31m${@}\033[0m" >&2; exit 1; }
function _warn(){ echo -e "\033[0;33m${@}\033[0m" >&2; }

# Check for Parameters
OPTS="$(getopt -o d:r:m:ch -l device:,release:,model:,clean,help -n 'build_uefi.sh' -- "$@")" || exit 1
eval set -- "${OPTS}"

# Parse Parameters
while true
do case "${1}" in
		-d|--device) TARGET_DEVICE="${2}";shift 2;;
		-r|--release) TARGET_BUILD_MODE="${2}";shift 2;;
		-m|--model) TARGET_DEVICE_MODEL="${2}";shift 2;;
		-c|--clean) DO_CLEAN_BUILD=1;shift;;
		-h|--help) _help 0;shift;;
		--) shift;break;;
		*) _help 1;;
	esac
done

# Check Device Parameter
if [ -z ${TARGET_DEVICE} ]
then _help
fi

# Parse Device Config File
if [ -f "Resources/Configs/${TARGET_DEVICE}.conf" ]
then source "Resources/Configs/${TARGET_DEVICE}.conf"
else _error "\nDevice configuration not found for ${TARGET_DEVICE}!\nCheck if your .conf File is in the 'Resources/Configs' Folder.\n"
fi

# Check Device Models
if [ ${TARGET_MULTIPLE_MODELS} == 1 ]; then
	if [ -z ${TARGET_DEVICE_MODEL} ];
	then _error "\nThis Device has ${TARGET_NUMBER_OF_MODELS} different Models.\nUse -m or --model To define wich Model using a Number.\n"
	fi
fi

# Delete left over Output Files
rm -r Conf &> /dev/null
rm ./BootShim/BootShim.bin &> /dev/null
rm ./BootShim/BootShim.elf &> /dev/null
rm ./Resources/bootpayload.bin &> /dev/null
rm Mu-${TARGET_DEVICE}.* &> /dev/null

# Delete Old Build Files
if [[ ${DO_CLEAN_BUILD} == 1 ]];
then rm -r Build &> /dev/null
fi

# Compile BootShim
if [ ${TARGET_REQUIRES_BOOTSHIM} == 1 ]; then
	pushd BootShim  &> /dev/null || exit 1
	make CREATE_FDT_POINTER=${TARGET_CREATE_POINTER} FDT_POINTER_ADDRESS=${TARGET_POINTER_ADDRESS} UEFI_BASE=${TARGET_FD_BASE} UEFI_SIZE=${TARGET_FD_SIZE} || _error "\nFailed to Compile BootShim!\n"
	popd &> /dev/null
fi

# Remove Mu_Basecore Patches
pushd Mu_Basecore  &> /dev/null || exit 1
git apply -R BdsWait.patch &> /dev/null
rm BdsWait.patch &> /dev/null
popd &> /dev/null

# Remove Mu_Tiano Patches
pushd Silicon/Arm/Mu_Tiano  &> /dev/null || exit 1
git apply -R Timer.patch &> /dev/null
git apply -R UsbBus.patch &> /dev/null
rm Timer.patch &> /dev/null
rm UsbBus.patch &> /dev/null
popd &> /dev/null

# Set Release Type of UEFI
while true
do case "${TARGET_BUILD_MODE^^}" in
		DEBUG) TARGET_BUILD_MODE=DEBUG;break;;
		*) TARGET_BUILD_MODE=RELEASE;break;;
	esac
done

# Setup & Update UEFI Enviroment
python3 "Platforms/${TARGET_DEVICE_VENDOR}/${TARGET_DEVICE}Pkg/DeviceBuild.py" --setup -t ${TARGET_BUILD_MODE} || _error "\nFailed to Setup UEFI Env!\n"
python3 "Platforms/${TARGET_DEVICE_VENDOR}/${TARGET_DEVICE}Pkg/DeviceBuild.py" --update -t ${TARGET_BUILD_MODE} || _error "\nFailed to Update UEFI Env!\n"

# Copy Mu Patches to the Right Location
cp Resources/MuPatches/BdsWait.patch Resources/MuPatches/UsbBus.patch Mu_Basecore/ || exit 1
cp Resources/MuPatches/Timer.patch Silicon/Arm/Mu_Tiano/ || exit 1

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
python3 "Platforms/${TARGET_DEVICE_VENDOR}/${TARGET_DEVICE}Pkg/DeviceBuild.py" "TARGET=${TARGET_BUILD_MODE}" "FD_BASE=${TARGET_FD_BASE}" "FD_SIZE=${TARGET_FD_SIZE}" "FD_BLOCKS=${TARGET_FD_BLOCKS}" "DEVICE_MODEL=${TARGET_DEVICE_MODEL}" || _error "\nFailed to Compile UEFI!\n"

# Run Device Specific Image Creation
if [ -f "Resources/Scripts/${TARGET_DEVICE}.sh" ]
then source Resources/Scripts/${TARGET_DEVICE}.sh
else _warn "\nImage Creation Script of ${TARGET_DEVICE} was not Found!\nNo Output Image Was Generated.\n"
fi

# Check for Updates
git fetch &> /dev/null
UPDATE_CHECK=$(git status)
if [[ ${UPDATE_CHECK} == *"git pull"* ]];
then _warn "\nYou are using an old Version of Mu-Silicium.\nPlease Update to use newest Fixes and Features.\nUse 'git pull' to Update your Local Repo.\n"
fi
