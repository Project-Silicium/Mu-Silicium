#!/bin/bash

# Build an Android kernel that is actually UEFI disguised as the Kernel
cat ./BootShim/BootShim.bin "./Build/miatollPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/MIATOLL_UEFI.fd" > "./Build/miatollPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/MIATOLL_UEFI.fd-bootshim"||exit 1
gzip -c < "./Build/miatollPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/MIATOLL_UEFI.fd-bootshim" > "./Build/miatollPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/MIATOLL_UEFI.fd-bootshim.gz"||exit 1
cat "./Build/miatollPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/MIATOLL_UEFI.fd-bootshim.gz" ./Resources/DTBs/miatoll.dtb > ./Resources/bootpayload.bin||exit 1

# Set correct Codename
if [ $TARGET_DEVICE_MODEL == 0 ]
then TARGET_MODEL_CODENAME="curtana-0"
elif [ $TARGET_DEVICE_MODEL == 1 ]
then TARGET_MODEL_CODENAME="curtana-1"
elif [ $TARGET_DEVICE_MODEL == 2 ]
then TARGET_MODEL_CODENAME="curtana-2"
elif [ $TARGET_DEVICE_MODEL == 3 ]
then TARGET_MODEL_CODENAME="joyeuse"
elif [ $TARGET_DEVICE_MODEL == 4 ]
then TARGET_MODEL_CODENAME="excalibur"
elif [ $TARGET_DEVICE_MODEL == 5 ]
then TARGET_MODEL_CODENAME="gram"
fi

# Create bootable Android boot.img
python3 ./Resources/Scripts/mkbootimg.py \
  --kernel ./Resources/bootpayload.bin \
  --ramdisk ./Resources/ramdisk \
  --kernel_offset 0x00000000 \
  --ramdisk_offset 0x00000000 \
  --tags_offset 0x00000000 \
  --os_version 13.0.0 \
  --os_patch_level "$(date '+%Y-%m')" \
  --header_version 1 \
  -o Mu-${TARGET_MODEL_CODENAME}.img \
  ||_error "\nFailed to create Android Boot Image!\n"