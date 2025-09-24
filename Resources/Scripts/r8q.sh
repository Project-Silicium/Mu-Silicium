#!/bin/bash

# Package UEFI Image as Android Kernel
cat ./BootShim/BootShim.bin "./Build/r8qPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/R8Q_UEFI.fd" > "./Build/r8qPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/R8Q_UEFI.fd-bootshim"||exit 1
gzip -c < "./Build/r8qPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/R8Q_UEFI.fd-bootshim" > "./Build/r8qPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/R8Q_UEFI.fd-bootshim.gz"||exit 1
cat "./Build/r8qPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/R8Q_UEFI.fd-bootshim.gz" ./Resources/DTBs/r8q.dtb > ./Resources/bootpayload.bin||exit 1

# Create Android boot.img
python3 ./Resources/Scripts/mkbootimg.py \
  --kernel ./Resources/bootpayload.bin \
  --ramdisk ./Resources/ramdisk \
  --base 0x00000000 \
  --kernel_offset 0x00008000 \
  --ramdisk_offset 0x01000000 \
  --tags_offset 0x00000100 \
  --second_offset 0x00000000 \
  --os_version 15.0.0 \
  --os_patch_level 2025-04 \
  --header_version 2 \
  --dtb ./Resources/DTBs/r8q.dtb \
  --dtb_offset 0x01f00000 \
  --pagesize 4096 \
  -o boot.img \
  ||_error "\nFailed to create Android Boot Image!\n"

# Compress UEFI Boot Image
tar -c boot.img -f Mu-r8q.tar||exit 1
mv boot.img Mu-r8q.img||exit 1
