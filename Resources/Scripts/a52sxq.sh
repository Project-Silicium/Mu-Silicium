#!/bin/bash

# Build an Android kernel that is actually UEFI disguised as the Kernel
cat ./BootShim/AARCH64/BootShim.bin "./Build/a52sxqPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/A52SXQ_UEFI.fd" > "./Build/a52sxqPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/A52SXQ_UEFI.fd-bootshim"||exit 1
gzip -c < "./Build/a52sxqPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/A52SXQ_UEFI.fd-bootshim" > "./Build/a52sxqPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/A52SXQ_UEFI.fd-bootshim.gz"||exit 1
cat "./Build/a52sxqPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/A52SXQ_UEFI.fd-bootshim.gz" ./Resources/DTBs/a52sxq.dtb > ./Resources/bootpayload.bin||exit 1

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
  -o Mu-a52sxq.img \
  ||_error "\nFailed to create Android Boot Image!\n"
