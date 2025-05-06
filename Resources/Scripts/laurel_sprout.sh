#!/bin/bash

# Build an Android kernel that is actually UEFI disguised as the Kernel
cat ./BootShim/AARCH64/BootShim.bin "./Build/laurel_sproutPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/LAUREL_SPROUT_UEFI.fd" > "./Build/laurel_sproutPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/LAUREL_SPROUT_UEFI.fd-bootshim"||exit 1
gzip -c < "./Build/laurel_sproutPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/LAUREL_SPROUT_UEFI.fd-bootshim" > "./Build/laurel_sproutPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/LAUREL_SPROUT_UEFI.fd-bootshim.gz"||exit 1
cat "./Build/laurel_sproutPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/LAUREL_SPROUT_UEFI.fd-bootshim.gz" ./Resources/DTBs/laurel_sprout.dtb > ./Resources/bootpayload.bin||exit 1

# Create bootable Android boot.img
python3 ./Resources/Scripts/mkbootimg.py \
  --kernel ./Resources/bootpayload.bin \
  --ramdisk ./Resources/ramdisk \
  --kernel_offset 0x00000000 \
  --ramdisk_offset 0x00000000 \
  --tags_offset 0x00000000 \
  --os_version 13.0.0 \
  --os_patch_level "$(date '+%Y-%m')" \
  --header_version 1 \
  -o Mu-laurel_sprout.img \
  ||_error "\nFailed to create Android Boot Image!\n"
