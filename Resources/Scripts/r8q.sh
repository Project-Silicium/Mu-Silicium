#!/bin/bash

# Build an Android kernel that is actually UEFI disguised as the Kernel
cat ./BootShim/BootShim.bin "./Build/r8qPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/R8Q_UEFI.fd" > "./Build/r8qPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/R8Q_UEFI.fd-bootshim"||exit 1
gzip -c < "./Build/r8qPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/R8Q_UEFI.fd-bootshim" > "./Build/r8qPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/R8Q_UEFI.fd-bootshim.gz"||exit 1
cat "./Build/r8qPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/R8Q_UEFI.fd-bootshim.gz" ./Resources/DTBs/r8q.dtb > ./Resources/bootpayload.bin||exit 1

# Create bootable Android boot.img
python3 ./Resources/Scripts/mkbootimg.py \
  --kernel ./Resources/bootpayload.bin \
  --ramdisk ./Resources/ramdisk \
  --os_version 13.0.0 \
  --os_patch_level "$(date '+%Y-%m')" \
  --header_version 1 \
  -o boot.img \
  ||_error "\nFailed to create Android Boot Image!\n"

# Compress UEFI Boot Image
tar -c boot.img -f Mu-r8q.tar||exit 1
mv boot.img Mu-r8q.img||exit 1
