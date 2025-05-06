#!/bin/bash

cat ./BootShim/AARCH64/BootShim.bin "./Build/starltePkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/STARLTE_UEFI.fd" > ./Resources/bootpayload.bin||exit 1

python3 ./Resources/Scripts/mkbootimg.py \
	--kernel ./Resources/bootpayload.bin \
	--ramdisk ./Resources/ramdisk \
	--dtb "./Resources/DTBs/starlte.dtb" \
	--kernel_offset 0x00000000 \
	--ramdisk_offset 0x00000000 \
	--tags_offset 0x00000000 \
	--dtb_offset 0x00000000 \
	--os_version 13.0.0 \
	--os_patch_level "$(date '+%Y-%m')" \
	--header_version 1 \
	-o boot.img \
	||_error "\nFailed to create Android Boot Image!\n"

# Compress Boot Image in a tar File for Odin/heimdall Flash
tar -c boot.img -f Mu-starlte.tar||exit 1
mv boot.img Mu-starlte.img||exit 1