#!/bin/bash

# Build an Android kernel that is actually UEFI disguised as the Kernel
cat ./BootShim/BootShim.bin "./Build/gtaxlltePkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/GTAXLLTE_UEFI.fd" > ./Resources/bootpayload.bin||exit 1

# Create bootable Android boot.img
python3 ./Resources/Scripts/mkbootimg.py \
  --kernel ./Resources/bootpayload.bin \
  --ramdisk ./Resources/ramdisk \
  --kernel_offset 0x00008000 \
  --ramdisk_offset 0x00008000 \
  --tags_offset 0x00008000 \
  --os_version 13.0.0 \
  --os_patch_level "$(date '+%Y-%m')" \
  --header_version 1 \
  -o boot.img \
  ||_error "\nFailed to create Android Boot Image!\n"

# Compress Boot Image in a tar File for Odin/heimdall Flash
tar -c boot.img -f Mu-gtaxllte.tar||exit 1
mv boot.img Mu-gtaxllte.img||exit 1