#!/bin/bash

# Build an Android kernel that is actually UEFI disguised as the Kernel
cat ./BootShim/AARCH64/BootShim.bin "./Build/balePkg/${_TARGET_BUILD_MODE}_CLANGPDB/FV/BALE_UEFI.fd" > "./Build/balePkg/${_TARGET_BUILD_MODE}_CLANGPDB/FV/BALE_UEFI.fd-bootshim"||exit 1
gzip -c < "./Build/balePkg/${_TARGET_BUILD_MODE}_CLANGPDB/FV/BALE_UEFI.fd-bootshim" > "./Build/balePkg/${_TARGET_BUILD_MODE}_CLANGPDB/FV/BALE_UEFI.fd-bootshim.gz"||exit 1
cat "./Build/balePkg/${_TARGET_BUILD_MODE}_CLANGPDB/FV/BALE_UEFI.fd-bootshim.gz" ./Resources/DTBs/bale.dtb > ./Resources/bootpayload.bin||exit 1

# Check which model and type are building
if [ $TARGET_DEVICE_MODEL == 0 ]
then MODEL_NAME="GT-Neo6"
elif [ $TARGET_DEVICE_MODEL == 1 ]
then MODEL_NAME="GT6"
fi

if [ $_TARGET_BUILD_MODE == DEBUG ]
then BUILD_TYPE="Debug"
elif [ $_TARGET_BUILD_MODE == RELEASE ]
then BUILD_TYPE="Release"
fi

# Create bootable Android boot.img
python3 ./Resources/Scripts/mkbootimg.py \
  --kernel ./Resources/bootpayload.bin \
  --ramdisk ./Resources/ramdisk \
  --kernel_offset 0x00000000 \
  --ramdisk_offset 0x00000000 \
  --tags_offset 0x00000000 \
  --os_version 15.0.0 \
  --os_patch_level "$(date '+%Y-%m')" \
  --header_version 4 \
  -o Mu-bale-${MODEL_NAME}-${BUILD_TYPE}.img \
  ||_error "\nFailed to create Android Boot Image!\n"
