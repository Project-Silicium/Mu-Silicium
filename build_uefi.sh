#!/bin/bash

# Function to display Help Message
function _help(){
	echo "Usage: ./build_uefi.sh -d <Codename> [-r <Build Mode>] [-m <Device Model>]"
	echo
	echo "Build Project Mu UEFI for various AARCH64/ARM Devices."
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

# Functions to display the Message Type (Error or Warning)
function _error(){ echo -e "\033[1;31m${@}\033[0m" >&2; exit 1; }
function _warn(){ echo -e "\033[0;33m${@}\033[0m" >&2; }

# Check if any Parameter were given
OPTS="$(getopt -o d:r:m:ch -l device:,release:,model:,clean,help -n 'build_uefi.sh' -- "$@")" || exit 1
eval set -- "${OPTS}"

# Parse Parameters
while true
do	case "${1}" in
		-d|--device) TARGET_DEVICE="${2}";shift 2;;
		-h|--help) _help 0;shift;;
		-r|--release) TARGET_BUILD_MODE="${2}";shift 2;;
		-m|--model) TARGET_DEVICE_MODEL="${2}";shift 2;;
		-c|--clean) DO_CLEAN_BUILD=1;shift;;
		--) shift;break;;
		*) _help 1;;
	esac
done

# If no Device arg is present, Display the Help Message
if [ -z ${TARGET_DEVICE} ]
then _help
fi

# Include Device Config if it exists
if [ -f "Resources/Configs/${TARGET_DEVICE}.conf" ]
then source "Resources/Configs/${TARGET_DEVICE}.conf"
else _error "\nDevice configuration not found from ${TARGET_DEVICE}!\nCheck if your .conf File is in the 'Recourses/Configs' Folder.\n"
fi

# Check if the Device has Multiple Models
if [ ${TARGET_MULTIPLE_MODELS} == 1 ]; then
	if [ -z ${TARGET_DEVICE_MODEL} ];
	then _error "\nThis Device has ${TARGET_NUMBER_OF_MODELS} different Models.\nUse -m or --model To define wich Model using a Number.\n"
	fi
fi

# Delete Output Files if present
rm -r Conf &> /dev/null
rm ./BootShim/${TARGET_ARCH}/BootShim.bin &> /dev/null
rm ./BootShim/${TARGET_ARCH}/BootShim.elf &> /dev/null
rm ./Resources/bootpayload.bin &> /dev/null
rm Mu-${TARGET_DEVICE}.* &> /dev/null
rm UEFILoader.efi &> /dev/null

# Compile BootShim
if [ ${TARGET_REQUIRES_BOOTSHIM} == 1 ]; then
	cd BootShim/${TARGET_ARCH} || exit 1
	make CREATE_FDT_POINTER=${TARGET_CREATE_POINTER} FDT_POINTER_ADDRESS=${TARGET_POINTER_ADDRESS} UEFI_BASE=${TARGET_FD_BASE} UEFI_SIZE=${TARGET_FD_SIZE} || _error "\nFailed to Compile BootShim!\n"
	cd ../..
fi

# Remove Mu Patches
cd Common/Mu || exit 1
git apply -R Math.patch &> /dev/null
rm Math.patch &> /dev/null
cd ../../Mu_Basecore || exit 1
git apply -R UsbBus.patch &> /dev/null
git apply -R BdsWait.patch &> /dev/null
git apply -R Tools-Conf.patch &> /dev/null
git apply -R PdbPointer.patch &> /dev/null
git apply -R PageDebug.patch &> /dev/null
rm UsbBus.patch &> /dev/null
rm BdsWait.patch &> /dev/null
rm Tools-Conf.patch &> /dev/null
rm PdbPointer.patch &> /dev/null
rm PageDebug.patch &> /dev/null
cd ../Silicon/Arm/Mu_Tiano || exit 1
git apply -R Timer.patch &> /dev/null
git apply -R GcdAttribute.patch &> /dev/null
rm Timer.patch &> /dev/null
rm GcdAttribute.patch &> /dev/null
cd ../../..

# Delete Build Files if present
if [[ ${DO_CLEAN_BUILD} == 1 ]];
then rm -r Build &> /dev/null
fi

# Set Release Type of UEFI
case "${TARGET_BUILD_MODE^^}" in
	DEBUG) _TARGET_BUILD_MODE=DEBUG;;
	*) _TARGET_BUILD_MODE=RELEASE;;
esac

# Setup & Update UEFI Enviroment
python3 "Platforms/${TARGET_DEVICE_VENDOR}/${TARGET_DEVICE}Pkg/PlatformBuild.py" --setup -t ${_TARGET_BUILD_MODE} || _error "\nFailed to Setup UEFI Env!\n"
python3 "Platforms/${TARGET_DEVICE_VENDOR}/${TARGET_DEVICE}Pkg/PlatformBuild.py" --update -t ${_TARGET_BUILD_MODE} || _error "\nFailed to Update UEFI Env!\n"

# Copy Mu Patches to the Right Location
cp ./Resources/MuPatches/UsbBus.patch ./Resources/MuPatches/BdsWait.patch ./Resources/MuPatches/Tools-Conf.patch ./Resources/MuPatches/PdbPointer.patch ./Mu_Basecore/ || exit 1
cp ./Resources/MuPatches/Timer.patch ./Silicon/Arm/Mu_Tiano/ || exit 1
cp ./Resources/MuPatches/Math.patch ./Common/Mu/ || exit 1

# Apply Mu Patches
cd Mu_Basecore || exit 1
git apply UsbBus.patch &> /dev/null
git apply BdsWait.patch &> /dev/null
git apply Tools-Conf.patch &> /dev/null
git apply PdbPointer.patch &> /dev/null
cd ../Silicon/Arm/Mu_Tiano || exit 1
git apply Timer.patch &> /dev/null
cd ../../..

if [ ${TARGET_ARCH} == "ARM" ]; then
	cd Common/Mu || exit 1
	git apply Math.patch &> /dev/null
	cd ../..
fi

# Start the Real Build of the UEFI
python3 "Platforms/${TARGET_DEVICE_VENDOR}/${TARGET_DEVICE}Pkg/PlatformBuild.py" "TARGET=${_TARGET_BUILD_MODE}" "FD_BASE=${TARGET_FD_BASE}" "FD_SIZE=${TARGET_FD_SIZE}" "FD_BLOCKS=${TARGET_FD_BLOCKS}" "DEVICE_MODEL=${TARGET_DEVICE_MODEL}" || _error "\nFailed to Compile UEFI!\n"

# Execute Device Specific Image Creation
if [ -f "Resources/Scripts/${TARGET_DEVICE}.sh" ]
then source Resources/Scripts/${TARGET_DEVICE}.sh
else _warn "\nImage Creation Script of ${TARGET_DEVICE} was not Found!\nNo Image Was Generated.\n"
fi

# Check for Updates
git fetch &> /dev/null
UPDATE_CHECK=$(git status)
if [[ ${UPDATE_CHECK} == *"git pull"* ]];
then _warn "\nYou are using an old Version of Mu-Silicium.\nPlease Update to use newest Fixes and Features.\nUse 'git pull' to Update your Local Repo.\n"
fi
