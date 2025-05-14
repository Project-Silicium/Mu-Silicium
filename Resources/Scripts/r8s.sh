#!/bin/bash

cat ./BootShim/BootShim.bin "./Build/r8sPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/R8S_UEFI.fd" > "./Resources/bootpayload.bin"||exit 1

python3 ./Resources/Scripts/mkbootimg.py \
  --kernel ./Resources/bootpayload.bin \
  --ramdisk ./Resources/ramdisk \
  --dtb "./Resources/DTBs/r8s.dt" \
  --tags_offset 0x00000100 \
  --second_offset 0xf0000000 \
  --ramdisk_offset 0x01000000 \
  --pagesize 2048 \
  --os_version "13.0.0" \
  --os_patch_level "2099-12" \
  --kernel_offset 0x00008000 \
  --header_version 2  \
  --dtb_offset 0x00000000 \
  --cmdline "buildvariant=userdebug" \
  --board "" \
  --base 0x10000000 \
  -o "boot.img" \
  ||_error "\nFailed to create Android Boot Image!\n"

# Compress Boot Image in a tar File for Odin/heimdall Flash
tar -c boot.img -f Mu-r8s.tar||exit 1
mv boot.img Mu-r8s.img||exit 1
