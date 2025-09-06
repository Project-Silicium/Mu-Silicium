#!/bin/bash

gzip -c < "./Build/lancelotPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/LANCELOT_UEFI.fd" > ./Resources/bootpayload.bin||exit 1

python3 ./Resources/Scripts/mkbootimg.py \
  --kernel ./Resources/bootpayload.bin \
  --ramdisk ./Resources/ramdisk \
  --dtb "./Resources/DTBs/lancelot.dtb" \
  --tags_offset 0x0bc08000 \
  --second_offset 0xbff88000 \
  --ramdisk_offset 0x07c08000 \
  --pagesize 2048 \
  --kernel_offset 0x00008000 \
  --header_version 2  \
  --dtb_offset 0x0bc08000 \
  --cmdline "bootopt=64S3,32N2,64N2" \
  --base 0x40078000 \
  -o "Mu-lancelot.img" \
  ||_error "\nFailed to create Android Boot Image!\n"