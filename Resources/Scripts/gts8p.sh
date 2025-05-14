#!/bin/bash

# Package UEFI Image as Android Kernel
cat ./BootShim/BootShim.bin "./Build/gts8pPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/GTS8P_UEFI.fd" > "./Build/gts8pPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/GTS8P_UEFI.fd-bootshim"||exit 1
gzip -c < "./Build/gts8pPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/GTS8P_UEFI.fd-bootshim" > "./Build/gts8pPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/GTS8P_UEFI.fd-bootshim.gz"||exit 1
cat "./Build/gts8pPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/GTS8P_UEFI.fd-bootshim.gz" ./Resources/DTBs/gts8p.dtb > ./Resources/bootpayload.bin||exit 1

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
tar -c boot.img -f Mu-gts8p.tar||exit 1
mv boot.img Mu-gts8p.img||exit 1
