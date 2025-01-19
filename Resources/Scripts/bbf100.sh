#!/bin/bash

# Build an Android kernel that is actually UEFI disguised as the Kernel
cat ./BootShim/AARCH64/BootShim.bin "./Build/bbf100Pkg/${_TARGET_BUILD_MODE}_CLANGPDB/FV/BBF100_UEFI.fd" > "./Build/bbf100Pkg/${_TARGET_BUILD_MODE}_CLANGPDB/FV/bbf100_UEFI.fd-bootshim"||exit 1
gzip -c < "./Build/bbf100Pkg/${_TARGET_BUILD_MODE}_CLANGPDB/FV/bbf100_UEFI.fd-bootshim" > "./Build/bbf100Pkg/${_TARGET_BUILD_MODE}_CLANGPDB/FV/bbf100_UEFI.fd-bootshim.gz"||exit 1
cat "./Build/bbf100Pkg/${_TARGET_BUILD_MODE}_CLANGPDB/FV/bbf100_UEFI.fd-bootshim.gz" ./Resources/DTBs/bbf100.dtb > ./Resources/bootpayload.bin||exit 1

# Create bootable Android boot.img
python3 ./Resources/Scripts/mkbootimg.py \
  --kernel ./Resources/bootpayload.bin \
  --ramdisk ./Resources/ramdisk \
  --kernel_offset 0x00000000 \
  --ramdisk_offset 0x00000000 \
  --tags_offset 0x00000000 \
  --os_version 8.0.0 \
  --os_patch_level "2020-11" \
  --header_version 0 \
  -o Mu-bbf100.img \
  ||_error "\nFailed to create Android Boot Image!\n"
