#!/bin/bash

function _help(){
	echo "Usage: build_uefi.sh --device DEV"
	echo
	echo "Build MU UEFI for Qualcomm Snapdragon platforms."
	echo
	echo "Options:"
	echo "	--device DEV, -d DEV:    build for DEV."
	echo "	--release MODE, -r MODE: Release mode for building, default is 'RELEASE', 'DEBUG' alternatively."
	echo "	--help, -h:              Shows this Help."
	echo "	--memory -m:             Define how much Memory your Device has."
	echo
	echo "MainPage: https://github.com/Robotix22/MU-Qcom"
	exit 1
}

function _error(){ echo "${@}" >&2;exit 1; }

TARGET_DEVICE=""
TARGET_RAM_SIZE=""
MULTIPLE_RAM_SIZE="FALSE"
TARGET_BUILD_MODE="RELEASE"
OPTS="$(getopt -o d:hfabcACDO:r:m: -l device:,help,release:,memory: -n 'build_uefi.sh' -- "$@")"||exit 1
eval set -- "${OPTS}"
while true
do	case "${1}" in
		-d|--device) TARGET_DEVICE="${2}";shift 2;;
		-h|--help) _help 0;shift;;
		-r|--release) TARGET_BUILD_MODE="${2}";shift 2;;
		-m|--memory) TARGET_RAM_SIZE="${2}";shift 2;;
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

if [ -z ${TARGET_RAM_SIZE} ]; then
if [ ${MULTIPLE_RAM_SIZE} = TRUE ]; then
    echo ""
    echo "=================[ WARNING ]================="
    echo "The Device you chose has more than one RAM Size!"
    echo "Use -m or --memory to define how much RAM your Device has."
    echo "============================================="
    echo ""
    exit 1
fi
fi

rm ./BootShim/BootShim.bin
rm ./BootShim/BootShim.elf
rm ./ImageResources/bootpayload.bin
rm MU-${TARGET_DEVICE}.img

cd BootShim

make UEFI_BASE=${FD_BASE} UEFI_SIZE=${FD_SIZE}||exit 1

cd ..

for i in "${SIMPLE_INIT}" GPLDrivers/Library/SimpleInit
do
	if [ -n "${i}" ]&&[ -f "${i}/SimpleInit.inc" ]
	then
		_SIMPLE_INIT="$(realpath "${i}")"
		break
	fi
done
export CROSS_COMPILE="${CROSS_COMPILE:-aarch64-linux-gnu-}"
export GCC5_AARCH64_PREFIX="${CROSS_COMPILE}"
export CLANG38_AARCH64_PREFIX="${CROSS_COMPILE}"
mkdir -p "${_SIMPLE_INIT}/build" "${_SIMPLE_INIT}/root/usr/share/locale"
for i in "${_SIMPLE_INIT}/po/"*.po
do
	[ -f "${i}" ]||continue
	_name="$(basename "$i" .po)"
	_path="${_SIMPLE_INIT}/root/usr/share/locale/${_name}/LC_MESSAGES"
	mkdir -p "${_path}"
	msgfmt -o "${_path}/simple-init.mo" "${i}"
done

bash "${_SIMPLE_INIT}/scripts/gen-rootfs-source.sh" \
	"${_SIMPLE_INIT}" \
	"${_SIMPLE_INIT}/build"

stuart_setup -c "Platforms/${SOC_PLATFORM}Pkg/PlatformBuild.py" TOOL_CHAIN_TAG=CLANG38||exit 1
stuart_update -c "Platforms/${SOC_PLATFORM}Pkg/PlatformBuild.py" TOOL_CHAIN_TAG=CLANG38||exit 1
stuart_build -c "Platforms/${SOC_PLATFORM}Pkg/PlatformBuild.py" TOOL_CHAIN_TAG=CLANG38 "TARGET=${_TARGET_BUILD_MODE}" "TARGET_DEVICE=${TARGET_DEVICE}" "FD_BASE=${FD_BASE}" "FD_SIZE=${FD_SIZE}" "RAM_SIZE=${TARGET_RAM_SIZE}"||exit 1

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
