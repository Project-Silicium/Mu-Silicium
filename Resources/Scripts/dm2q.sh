#!/bin/bash

# Package UEFI Image as Android Kernel
cat ./BootShim/BootShim.bin "./Build/dm2qPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/DM2Q_UEFI.fd" > "./Build/dm2qPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/DM2Q_UEFI.fd-bootshim"||exit 1
gzip -c < "./Build/dm2qPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/DM2Q_UEFI.fd-bootshim" > "./Build/dm2qPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/DM2Q_UEFI.fd-bootshim.gz"||exit 1
cat "./Build/dm2qPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/DM2Q_UEFI.fd-bootshim.gz" ./Resources/DTBs/dm2q.dtb > ./Resources/bootpayload.bin||exit 1

# Create Android boot.img
python3 ./Resources/Scripts/mkbootimg.py \
  --kernel ./Resources/bootpayload.bin \
  --ramdisk ./Resources/ramdisk \
  --base 0x00000000 \
  --kernel_offset 0x00008000 \
  --ramdisk_offset 0x02000000 \
  --tags_offset 0x01e00000 \
  --second_offset 0x00000000 \
  --os_version 13.0.0 \
  --os_patch_level 2025-12 \
  --header_version 2 \
  --dtb ./Resources/DTBs/dm2q.dtb \
  --pagesize 4096 \
  -o boot.img \
  ||_error "\nFailed to create Android Boot Image!\n"

# Compress UEFI Boot Image
tar -c boot.img -f Mu-dm2q.tar||exit 1
mv boot.img Mu-dm2q.img||exit 1
