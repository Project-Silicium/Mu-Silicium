#!/bin/bash

# Build an Android kernel that is actually UEFI disguised as the Kernel
cat ./BootShim/BootShim.bin "./Build/senna_bcPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/SENNA_BC_UEFI.fd" > "./Build/senna_bcPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/SENNA_BC_UEFI.fd-bootshim"||exit 1
gzip -c < "./Build/senna_bcPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/SENNA_BC_UEFI.fd-bootshim" > "./Build/senna_bcPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/SENNA_BC_UEFI.fd-bootshim.gz"||exit 1
cat "./Build/senna_bcPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/SENNA_BC_UEFI.fd-bootshim.gz" ./Resources/DTBs/senna_bc.dtb > ./Resources/bootpayload.bin||exit 1

# Create bootable Android boot.img
python3 ./Resources/Scripts/mkbootimg.py \
  --kernel ./Resources/bootpayload.bin \
  --ramdisk ./Resources/ramdisk \
  --kernel_offset 0x00000000 \
  --ramdisk_offset 0x00000000 \
  --tags_offset 0x00000000 \
  --os_version 12.0.0 \
  --os_patch_level "$(date '+%Y-%m')" \
  --header_version 4 \
  -o Mu-senna_bc.img \
  ||_error "\nFailed to create Android Boot Image!\n"
