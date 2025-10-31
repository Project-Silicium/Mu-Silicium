#!/bin/bash

# Package UEFI Image as Android Kernel
cat ./BootShim/BootShim.bin "./Build/starqltechnPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/STARQLTECHN_UEFI.fd" > "./Build/starqltechnPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/STARQLTECHN_UEFI.fd-bootshim"||exit 1
gzip -c < "./Build/starqltechnPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/STARQLTECHN_UEFI.fd-bootshim" > "./Build/starqltechnPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/STARQLTECHN_UEFI.fd-bootshim.gz"||exit 1
cat "./Build/starqltechnPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/STARQLTECHN_UEFI.fd-bootshim.gz" ./Resources/DTBs/starqltechn.dtb > ./Resources/bootpayload.bin||exit 1

# Create Android boot.img
python3 ./Resources/Scripts/mkbootimg.py \
  --kernel ./Resources/bootpayload.bin \
  --ramdisk ./Resources/ramdisk \
  --kernel_offset 0x00000000 \
  --ramdisk_offset 0x00000000 \
  --tags_offset 0x00000000 \
  --os_version 10.0.0 \
  --os_patch_level "$(date '+%Y-%m')" \
  --header_version 1 \
  -o boot.img \
  ||_error "\nFailed to create Android Boot Image!\n"

# Compress UEFI Boot Image
tar -c boot.img -f Mu-starqltechn.tar||exit 1
mv boot.img Mu-starqltechn.img||exit 1
