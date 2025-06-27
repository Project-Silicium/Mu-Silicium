#!/bin/bash

# Package UEFI Image as Android Kernel
cat ./BootShim/BootShim.bin "./Build/b4qPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/B4Q_UEFI.fd" > "./Build/b4qPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/B4Q_UEFI.fd-bootshim"||exit 1
gzip -c < "./Build/b4qPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/B4Q_UEFI.fd-bootshim" > "./Build/b4qPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/B4Q_UEFI.fd-bootshim.gz"||exit 1
cat "./Build/b4qPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/B4Q_UEFI.fd-bootshim.gz" ./Resources/DTBs/b4q.dtb > ./Resources/bootpayload.bin||exit 1

# Create Android boot.img
python3 ./Resources/Scripts/mkbootimg.py \
  --kernel ./Resources/bootpayload.bin \
  --ramdisk ./Resources/ramdisk \
  --os_version 15.0.0 \
  --os_patch_level "$(date '+%Y-%m')" \
  --header_version 4 \
  -o boot.img \
  ||_error "\nFailed to create Android Boot Image!\n"

# Compress UEFI Boot Image
tar -c boot.img -f Mu-b4q.tar||exit 1
mv boot.img Mu-b4q.img||exit 1
