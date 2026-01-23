#!/bin/bash

# Package UEFI Image as Android Kernel
cat ./BootShim/BootShim.bin "./Build/gta9pwifiPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/GTA9PWIFI_UEFI.fd" > "./Build/gta9pwifiPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/GTA9PWIFI_UEFI.fd-bootshim"||exit 1
gzip -c < "./Build/gta9pwifiPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/GTA9PWIFI_UEFI.fd-bootshim" > "./Build/gta9pwifiPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/GTA9PWIFI_UEFI.fd-bootshim.gz"||exit 1
cat "./Build/gta9pwifiPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/GTA9PWIFI_UEFI.fd-bootshim.gz" ./Resources/DTBs/gta9pwifi.dtb > ./Resources/bootpayload.bin||exit 1

# Create Android boot.img
python3 ./Resources/Scripts/mkbootimg.py \
  --kernel ./Resources/bootpayload.bin \
  --ramdisk ./Resources/ramdisk \
  --base 0x00000000 \
  --kernel_offset 0x00008000 \
  --ramdisk_offset 0x02000000 \
  --tags_offset 0x01e00000 \
  --second_offset 0x00000000 \
  --dtb_offset 0x01f00000 \
  --board SRPWD25B001 \
  --header_version 2 \
  --pagesize 4096 \
  --dtb ./Resources/DTBs/gta9pwifi.dtb \
  -o boot.img \
  ||_error "\nFailed to create Android Boot Image!\n"

# Compress UEFI Boot Image
tar -c boot.img -f Mu-gta9pwifi.tar||exit 1
mv boot.img Mu-gta9pwifi.img||exit 1
