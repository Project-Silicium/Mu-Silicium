#!/bin/bash

function _help(){
	echo "Usage: build_uefi.sh --device DEV"
	echo
	echo "Build MU UEFI for Qualcomm Snapdragon platforms."
	echo
	echo "Options:"
	echo "	--device DEV, -d DEV:    build for DEV."
	echo "	--release MODE, -r MODE: Release mode for building, default is 'RELEASE', 'DEBUG' alternatively."
	echo "        --help, -h:              Shows this Help."
	echo
	echo "MainPage: https://github.com/Robotix22/MU-msm"
	exit 1
}

function _error(){ echo "${@}" >&2;exit 1; }

TARGET_DEVICE=""
TARGET_BUILD_MODE="RELEASE"
OPTS="$(getopt -o d:hfabcACDO:r: -l device:,help:,release: -n 'build_uefi.sh' -- "$@")"||exit 1
eval set -- "${OPTS}"
while true
do	case "${1}" in
		-d|--device) TARGET_DEVICE="${2}";shift 2;;
		-h|--help) _help 0;shift;;
		-r|--release) TARGET_BUILD_MODE="${2}";shift 2;;
		--) shift;break;;
		*) _help 1;;
	esac
done

case "${TARGET_BUILD_MODE}" in
	RELEASE) _TARGET_BUILD_MODE=RELEASE;;
	*) _TARGET_BUILD_MODE=DEBUG;;
esac

if [ -z ${TARGET_DEVICE} ]; then
    _help
fi

if [ -f "configs/devices/${TARGET_DEVICE}.conf" ]
then source "configs/devices/${TARGET_DEVICE}.conf"
else _error "Device configuration not found"
fi
if [ -f "configs/${SOC_PLATFORM}.conf" ]
then source "configs/${SOC_PLATFORM}.conf"
else _error "SoC configuration not found"
fi

# for overriding config
source "configs/devices/${TARGET_DEVICE}.conf"

rm ./BootShim/BootShim.bin
rm ./BootShim/BootShim.elf
rm ./ImageResources/bootpayload.bin
rm MU-*.img

cd BootShim

make UEFI_BASE=${FD_BASE} UEFI_SIZE=${FD_SIZE}||exit 1

cd ..

stuart_setup -c "Platforms/${SOC_PLATFORM}Pkg/PlatformBuild.py" TOOL_CHAIN_TAG=CLANG38||exit 1
stuart_update -c "Platforms/${SOC_PLATFORM}Pkg/PlatformBuild.py" TOOL_CHAIN_TAG=CLANG38||exit 1
stuart_build -c "Platforms/${SOC_PLATFORM}Pkg/PlatformBuild.py" TOOL_CHAIN_TAG=CLANG38 "TARGET=${_TARGET_BUILD_MODE}" "TARGET_DEVICE=${TARGET_DEVICE}" "FD_BASE=${FD_BASE}" "FD_SIZE=${FD_SIZE}"||exit 1

cat ./BootShim/BootShim.bin "./Build/${SOC_PLATFORM}Pkg/${_TARGET_BUILD_MODE}_CLANG38/FV/${SOC_PLATFORM}PKG_UEFI.fd" > "./Build/${SOC_PLATFORM}Pkg/${_TARGET_BUILD_MODE}_CLANG38/FV/${SOC_PLATFORM}PKG_UEFI.fd-bootshim" ||exit 1
gzip -c < "./Build/${SOC_PLATFORM}Pkg/${_TARGET_BUILD_MODE}_CLANG38/FV/${SOC_PLATFORM}PKG_UEFI.fd-bootshim" > "./Build/${SOC_PLATFORM}Pkg/${_TARGET_BUILD_MODE}_CLANG38/FV/${SOC_PLATFORM}PKG_UEFI.fd-bootshim.gz" ||exit 1
cat "./Build/${SOC_PLATFORM}Pkg/${_TARGET_BUILD_MODE}_CLANG38/FV/${SOC_PLATFORM}PKG_UEFI.fd-bootshim.gz" "./ImageResources/DTBs/${TARGET_DEVICE}.dtb" > ./ImageResources/bootpayload.bin||exit 1

python3 ./ImageResources/mkbootimg.py \
  --kernel ./ImageResources/bootpayload.bin \
  --ramdisk ./ImageResources/ramdisk \
  --kernel_offset 0x00000000 \
  --ramdisk_offset 0x00000000 \
  --tags_offset 0x00000000 \
  --os_version 13.0.0 \
  --os_patch_level "$(date '+%Y-%m')" \
  --header_version 1 \
  -o "MU-${TARGET_DEVICE}.img" \
  ||exit 1
