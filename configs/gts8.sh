#!/bin/bash

# Build an Android kernel that is actually UEFI disguised as the Kernel
cat ./BootShim/BootShim.bin "./Build/gts8Pkg/${_TARGET_BUILD_MODE}_CLANG38/FV/GTS8_UEFI.fd" > "./Build/gts8Pkg/${_TARGET_BUILD_MODE}_CLANG38/FV/GTS8_UEFI.fd-bootshim"||exit 1
gzip -c < "./Build/gts8Pkg/${_TARGET_BUILD_MODE}_CLANG38/FV/GTS8_UEFI.fd-bootshim" > "./Build/gts8Pkg/${_TARGET_BUILD_MODE}_CLANG38/FV/GTS8_UEFI.fd-bootshim.gz"||exit 1
cat "./Build/gts8Pkg/${_TARGET_BUILD_MODE}_CLANG38/FV/GTS8_UEFI.fd-bootshim.gz" ./ImageResources/DTBs/gts8.dtb > ./ImageResources/bootpayload.bin||exit 1

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
  -o boot.img \
  ||_error "\nFailed to create Android Boot Image!\n"

# Compress Boot Image in a tar File for Odin/heimdall Flash
tar -c boot.img -f Mu-gts8.tar||exit 1
mv boot.img Mu-gts8.img||exit 1