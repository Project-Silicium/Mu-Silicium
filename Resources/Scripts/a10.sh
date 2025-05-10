#!/bin/bash

# Build an Android kernel that is actually UEFI disguised as the Kernel
cat ./BootShim/BootShim.bin "./Build/a10Pkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/A10_UEFI.fd" > "./Resources/bootpayload.bin"||exit 1

# Create a Bootable Android Boot Image
python3 ./Resources/Scripts/mkbootimg.py \
  --kernel ./Resources/bootpayload.bin \
  --ramdisk ./Resources/ramdisk \
  --kernel_offset 0x00000000 \
  --ramdisk_offset 0x01000000 \
  --tags_offset 0x00000100 \
  --os_version 11.0.0 \
  --os_patch_level "$(date '+%Y-%m')" \
  --header_version 1 \
  -o "boot.img" \
  ||_error "\nFailed to create Android Boot Image!\n"

# Compress Boot Image in a tar File for Odin/heimdall Flash
tar -c boot.img -f Mu-a10.tar||exit 1
mv boot.img Mu-a10.img||exit 1
