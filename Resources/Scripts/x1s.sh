#!/bin/bash

cat ./BootShim/AARCH64/BootShim.bin "./Build/x1sPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/X1S_UEFI.fd" > "./Resources/bootpayload.bin"||exit 1

python3 ./Resources/Scripts/mkbootimg.py \
  --kernel ./Resources/bootpayload.bin \
  --ramdisk ./Resources/ramdisk \
  --dtb "./Resources/DTBs/x1s.dt" \
  --tags_offset 0x00000100 \
  --second_offset 0xf0000000 \
  --ramdisk_offset 0x01000000 \
  --pagesize 2048 \
  --os_version "13.0.0" \
  --os_patch_level "2023-07" \
  --kernel_offset 0x00008000 \
  --header_version 2  \
  --dtb_offset 0x00000000 \
  --cmdline "buildvariant=userdebug" \
  --board "" \
  --base 0x10000000 \
  -o "boot.img" \
  ||_error "\nFailed to create Android Boot Image!\n"

# Compress Boot Image in a tar File for Odin/heimdall Flash
tar -c boot.img -f Mu-x1s.tar||exit 1
mv boot.img Mu-x1s.img||exit 1
