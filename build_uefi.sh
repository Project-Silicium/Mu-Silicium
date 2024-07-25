#!/bin/bash

# Function to display Help Message
function _help(){
	echo "Usage: ./build_uefi.sh -d <Codename> [-r <Build Mode>] [-m <Device Model>]"
	echo
	echo "Build Project Mu UEFI for various AARCH64/ARM Devices."
	echo
	echo "Options:"
	echo "	--device <Codename>, -d <Codename>:         Build a Device."
	echo "	--release <Build Mode>, -r <Build Mode>:    Release mode for building, 'RELEASE' is the default or use 'DEBUG' alternatively."
	echo "	--model <Device Model>, -m <Device Model>:  Defines the Model Type of the Selected Target Device."
	echo "	--help, -h:                                 Shows this Help."
	echo
	echo "MainPage: https://github.com/Project-Silicium/Mu-Silicium"
	exit 1
}

# Functions to display the Message Type (Error or Warning)
function _error(){ echo -e "\033[1;31m${@}\033[0m" >&2; exit 1; }
function _warn(){ echo -e "\033[0;33m${@}\033[0m" >&2; }

# Check if any args were given
OPTS="$(getopt -o d:hr:m: -l device:,help,release:,model: -n 'build_uefi.sh' -- "$@")" || exit 1
eval set -- "${OPTS}"
while true
do	case "${1}" in
		-d|--device) TARGET_DEVICE="${2}";shift 2;;
		-h|--help) _help 0;shift;;
		-r|--release) TARGET_BUILD_MODE="${2}";shift 2;;
		-m|--model) TARGET_DEVICE_MODEL="${2}";shift 2;;
		--) shift;break;;
		*) _help 1;;
	esac
done


# If no Device arg is present, Display the Help Message
if [ -z ${TARGET_DEVICE} ]
then _help
fi

# Set Release Type of UEFI
case "${TARGET_BUILD_MODE}" in
	DEBUG) _TARGET_BUILD_MODE=DEBUG;;
	*) _TARGET_BUILD_MODE=RELEASE;;
esac

# Include Device Config if it exists
if [ -f "Resources/Configs/${TARGET_DEVICE}.conf" ]
then source "Resources/Configs/${TARGET_DEVICE}.conf"
else _error "\nDevice configuration not found!\nCheck if your .conf File is in the 'configs' Folder\n"
fi

# Check if the Device has Multiple Models
if [ ${TARGET_MULTIPLE_MODELS} == "TRUE" ]; then
	if [ -z ${TARGET_DEVICE_MODEL} ];
	then _error "\nThis Device has multiple Models.\nUse -m or --model To define wich Model with Numbers.\n"
	fi
fi

# Delete Output Files if present
rm -r Conf &> /dev/null
rm ./BootShim/${TARGET_ARCH}/BootShim.bin &> /dev/null
rm ./BootShim/${TARGET_ARCH}/BootShim.elf &> /dev/null
rm ./Resources/bootpayload.bin &> /dev/null
rm Mu-${TARGET_DEVICE}.* &> /dev/null

DSC_FILE="Platforms/${TARGET_DEVICE_VENDOR}/${TARGET_DEVICE}Pkg/${TARGET_DEVICE}.dsc"
KEY="DEVICE_PRESERVES_FDT"

value=$(grep -E "^[[:space:]]*$KEY[[:space:]]*=" "$DSC_FILE" | sed -E "s/^[[:space:]]*$KEY[[:space:]]*=[[:space:]]*//")

if [ -z "$value" ]; then
  value=0
fi

# Compile BootShim
cd BootShim/${TARGET_ARCH} || exit 1
make DEVICE_PRESERVES_FDT=${value} UEFI_BASE=${TARGET_FD_BASE} UEFI_SIZE=${TARGET_FD_SIZE}||_error "\nFailed to Compile BootShim!\n"
cd ../..

# Remove Mu Patches
## Mu
cd Common/Mu || exit 1
git reset --hard
git clean --force
cd ../..

## Mu_Basecore
cd Mu_Basecore || exit 1
git reset --hard
git clean --force
cd ..

## Mu_Tiano
cd Silicon/Arm/Mu_Tiano || exit 1
git reset --hard
git clean --force
cd ../../..

# Setup & Update UEFI Enviroment
python3 "Platforms/${TARGET_DEVICE_VENDOR}/${TARGET_DEVICE}Pkg/PlatformBuild.py" --setup -t ${_TARGET_BUILD_MODE} || _error "\nFailed to Setup UEFI Env!\n"
python3 "Platforms/${TARGET_DEVICE_VENDOR}/${TARGET_DEVICE}Pkg/PlatformBuild.py" --update -t ${_TARGET_BUILD_MODE} || _error "\nFailed to Update UEFI Env!\n"

# Apply Mu Patches
## Mu
if [ ${TARGET_ARCH} == "ARM" ]; then
	cp ./Resources/MuPatches/Math.patch ./Common/Mu/
	cd Common/Mu || exit 1
	git apply Math.patch &> /dev/null
	cd ../..
fi

## Mu_Basecore
cp ./Resources/MuPatches/UsbBus.patch ./Resources/MuPatches/BdsWait.patch ./Resources/MuPatches/Tools-Conf.patch ./Resources/MuPatches/PdbPointer.patch ./Mu_Basecore/
cd Mu_Basecore || exit 1
git apply UsbBus.patch &> /dev/null
git apply BdsWait.patch &> /dev/null
git apply Tools-Conf.patch &> /dev/null
git apply PdbPointer.patch &> /dev/null
cd ..

## Mu_Tiano
cp ./Resources/MuPatches/Timer.patch ./Silicon/Arm/Mu_Tiano/
cd Silicon/Arm/Mu_Tiano || exit 1
git apply Timer.patch &> /dev/null
cd ../../..

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
