#!/bin/bash

# Build an Android kernel that is actually UEFI disguised as the Kernel
cat ./BootShim/BootShim.bin "./Build/veuxPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/VEUX_UEFI.fd" > "./Build/veuxPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/VEUX_UEFI.fd-bootshim"||exit 1
gzip -c < "./Build/veuxPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/VEUX_UEFI.fd-bootshim" > "./Build/veuxPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/VEUX_UEFI.fd-bootshim.gz"||exit 1
cat "./Build/veuxPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/VEUX_UEFI.fd-bootshim.gz" ./Resources/DTBs/veux.dtb > ./Resources/bootpayload.bin||exit 1

# Create bootable Android boot.img
python3 ./Resources/Scripts/mkbootimg.py \
  --kernel ./Resources/bootpayload.bin \
  --ramdisk ./Resources/ramdisk \
  --kernel_offset 0x00000000 \
  --ramdisk_offset 0x00000000 \
  --tags_offset 0x00000000 \
  --os_version 14.0.0 \
  --os_patch_level "$(date '+%Y-%m')" \
  --header_version 3 \
  -o Mu-veux.img \
  ||_error "\nFailed to create Android Boot Image!\n"
