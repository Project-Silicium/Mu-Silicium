#!/bin/bash

# Build an Android kernel that is actually UEFI disguised as the Kernel
cat ./BootShim/AARCH64/BootShim.bin "./Build/ginkgoPkg/${_TARGET_BUILD_MODE}_CLANGPDB/FV/GINKGO_UEFI.fd" > "./Build/ginkgoPkg/${_TARGET_BUILD_MODE}_CLANGPDB/FV/GINKGO_UEFI.fd-bootshim"||exit 1
gzip -c < "./Build/ginkgoPkg/${_TARGET_BUILD_MODE}_CLANGPDB/FV/GINKGO_UEFI.fd-bootshim" > "./Build/ginkgoPkg/${_TARGET_BUILD_MODE}_CLANGPDB/FV/GINKGO_UEFI.fd-bootshim.gz"||exit 1
cat "./Build/ginkgoPkg/${_TARGET_BUILD_MODE}_CLANGPDB/FV/GINKGO_UEFI.fd-bootshim.gz" ./Resources/DTBs/ginkgo.dtb > ./Resources/bootpayload.bin||exit 1

# Create bootable Android boot.img
python3 ./Resources/Scripts/mkbootimg.py \
  --kernel ./Resources/bootpayload.bin \
  --ramdisk ./Resources/ramdisk \
  --base 0x00000000 \
  --kernel_offset 0x00008000 \
  --ramdisk_offset 0x01000000 \
  --second_offset 0x00F00000 \
  --tags_offset 0x00000100 \
  --pagesize 4096 \
  --os_version 13.0.0 \
  --os_patch_level "$(date '+%Y-%m')" \
  --header_version 0 \
  -o Mu-ginkgo.img \
  ||_error "\nFailed to create Android Boot Image!\n"
