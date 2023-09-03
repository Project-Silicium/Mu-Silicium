#!/bin/bash

# Function to display Help Message
function _help(){
	echo "Usage: ./build_uefi.sh -d <Codename> [-t <Tool Chain> -r <Build Mode> -m <RAM Size>]"
	echo
	echo "Build Project Mu UEFI for Qualcomm Snapdragon Platforms."
	echo
	echo "Options:"
	echo "	--device <Codename>, -d <Codename>:         Build a Device."
	echo "	--release <Build Mode>, -r <Build Mode>:    Release mode for building, 'RELEASE' is the default or use 'DEBUG' alternatively."
	echo "	--tool-chain <Tool Chain>, -t <Tool Chain>: Define wich Tool Chain to use, 'CLANG38' is default."
	echo "	--help, -h:                                 Shows this Help."
	echo "	--memory <RAM Size>, -m <RAM Size>:         Define how much Memory your Device has."
	echo
	echo "MainPage: https://github.com/Robotix22/Mu-Qcom"
	exit 1
}

# Functions to display the Message Type (Error or Warning)
function _error(){ echo -e "\033[1;31m${@}\033[0m" >&2;exit 1; }
function _warn(){ echo -e "\033[0;33m${@}\033[0m" >&2;exit 0; }

# Set Default Defines
TARGET_BUILD_MODE="RELEASE"
MULTIPLE_RAM_SIZE="FALSE"
TOOL_CHAIN_TAG="CLANG38"

# Check if any args were given
OPTS="$(getopt -o d:hfabcACDO:r:t:m: -l device:,help,release:,tool-chain:,memory: -n 'build_uefi.sh' -- "$@")"||exit 1
eval set -- "${OPTS}"
while true
do	case "${1}" in
		-d|--device) TARGET_DEVICE="${2}";shift 2;;
		-h|--help) _help 0;shift;;
		-r|--release) TARGET_BUILD_MODE="${2}";shift 2;;
		-t|--tool-chain) TOOL_CHAIN_TAG="${2}";shift 2;;
		-m|--memory) TARGET_RAM_SIZE="${2}";shift 2;;
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
	RELEASE) _TARGET_BUILD_MODE=RELEASE;;
	*) _TARGET_BUILD_MODE=DEBUG;;
esac

# Include Device Config if it exists
if [ -f "configs/${TARGET_DEVICE}.conf" ]
then source "configs/${TARGET_DEVICE}.conf"
else _error "\nDevice configuration not found!\nCheck if your .conf File is in the 'configs' Folder\n"
fi

# Check if Device has more that one Static RAM Size
if [ -z ${TARGET_RAM_SIZE} ]; then
	if [ ${MULTIPLE_RAM_SIZE} == TRUE ]
	then _warn "\nThe Device you chose has more than one RAM Size!\nUse -m or --memory to define how much RAM your Device has.\n" && exit 0
	fi
fi

# Delete Output Files if present
rm ./BootShim/BootShim.bin &> /dev/null
rm ./BootShim/BootShim.elf &> /dev/null
rm ./ImageResources/bootpayload.bin &> /dev/null
rm Mu-${TARGET_DEVICE}.img &> /dev/null

# Compile BootShim
cd BootShim
make UEFI_BASE=${TARGET_FD_BASE} UEFI_SIZE=${TARGET_FD_SIZE}||_error "\nFailed to Compile BootShim!\n"
cd ..

# Start the Real Build of the UEFI
stuart_setup -c "Platforms/${TARGET_DEVICE_VENDOR}/${TARGET_DEVICE}Pkg/PlatformBuild.py" "TOOL_CHAIN_TAG=${TOOL_CHAIN_TAG}"||_error "\nFailed to Setup UEFI Env!\n"
stuart_update -c "Platforms/${TARGET_DEVICE_VENDOR}/${TARGET_DEVICE}Pkg/PlatformBuild.py" "TOOL_CHAIN_TAG=${TOOL_CHAIN_TAG}"||_error "\nFailed to Update UEFI Env!\n"
stuart_build -c "Platforms/${TARGET_DEVICE_VENDOR}/${TARGET_DEVICE}Pkg/PlatformBuild.py" "TOOL_CHAIN_TAG=${TOOL_CHAIN_TAG}" "TARGET=${_TARGET_BUILD_MODE}" "RAM_SIZE=${TARGET_RAM_SIZE}" "FD_BASE=${TARGET_FD_BASE}" "FD_SIZE=${TARGET_FD_SIZE}" "FD_BLOCKS=${TARGET_FD_BLOCKS}"||_error "\nFailed to Compile UEFI!\n"

# Build an Android kernel that is actually UEFI disguised as the Kernel
cat ./BootShim/BootShim.bin "./Build/${TARGET_DEVICE}Pkg/${_TARGET_BUILD_MODE}_CLANG38/FV/${TARGET_DEVICE^^}_UEFI.fd" > "./Build/${TARGET_DEVICE}Pkg/${_TARGET_BUILD_MODE}_CLANG38/FV/${TARGET_DEVICE^^}_UEFI.fd-bootshim"||exit 1
gzip -c < "./Build/${TARGET_DEVICE}Pkg/${_TARGET_BUILD_MODE}_CLANG38/FV/${TARGET_DEVICE^^}_UEFI.fd-bootshim" > "./Build/${TARGET_DEVICE}Pkg/${_TARGET_BUILD_MODE}_CLANG38/FV/${TARGET_DEVICE^^}_UEFI.fd-bootshim.gz"||exit 1
cat "./Build/${TARGET_DEVICE}Pkg/${_TARGET_BUILD_MODE}_CLANG38/FV/${TARGET_DEVICE^^}_UEFI.fd-bootshim.gz" "./ImageResources/DTBs/${TARGET_DEVICE}.dtb" > ./ImageResources/bootpayload.bin||exit 1

# Create bootable Android boot.img
python3 ./ImageResources/mkbootimg.py \
  --kernel ./ImageResources/bootpayload.bin \
  --ramdisk ./ImageResources/ramdisk \
  --kernel_offset 0x00000000 \
  --ramdisk_offset 0x00000000 \
  --tags_offset 0x00000000 \
  --os_version 13.0.0 \
  --os_patch_level "$(date '+%Y-%m')" \
  --header_version 1 \
  -o "Mu-${TARGET_DEVICE}.img" \
  ||_error "\nFailed to create Android Boot Image!\n"

if [[ ${STATUS} != "STABLE" ]]; then
	if [[ ${STATUS} == "UNSTABLE" ]];
	then _warn "\n${TARGET_DEVICE} is marked as Unstable.\nThings are expected to be broken.\n"
	elif [[ ${STATUS} == "LIMITED" ]];
	then _warn "\n${TARGET_DEVICE} is marked as Limited.\nIt's stable but does not have much functionality\n"
	fi
fi

git fetch &> /dev/null
UPDATE_CHECK=$(git status)
if [[ ${UPDATE_CHECK} == *"git pull"* ]];
then _warn "\nYou are using an old Version of Mu-Qcom.\nThis Image may be unstable.\n"
fi
