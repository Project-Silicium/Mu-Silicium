#!/bin/bash

# Package UEFI Image as Android Kernel
cat ./BootShim/BootShim.bin "./Build/r0qPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/R0Q_UEFI.fd" > "./Build/r0qPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/R0Q_UEFI.fd-bootshim"||exit 1
gzip -c < "./Build/r0qPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/R0Q_UEFI.fd-bootshim" > "./Build/r0qPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/R0Q_UEFI.fd-bootshim.gz"||exit 1
cat "./Build/r0qPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/R0Q_UEFI.fd-bootshim.gz" ./Resources/DTBs/r0q.dtb > ./Resources/bootpayload.bin||exit 1

# Create Android boot.img
python3 ./Resources/Scripts/mkbootimg.py \
  --kernel ./Resources/bootpayload.bin \
  --ramdisk ./Resources/ramdisk \
  --base 0x00000000 \
  --kernel_offset 0x00008000 \
  --ramdisk_offset 0x02000000 \
  --tags_offset 0x00000000 \
  --second_offset 0x00000000 \
  --os_version 12.0.0 \
  --os_patch_level 2025-04 \
  --header_version 4 \
  --pagesize 4096 \
  -o boot.img \
  ||_error "\nFailed to create Android Boot Image!\n"

# Compress UEFI Boot Image
tar -c boot.img -f Mu-r0q.tar||exit 1
mv boot.img Mu-r0q.img||exit 1
