#!/bin/bash

# Set correct Codename
if [ $TARGET_DEVICE_MODEL == 0 ]
then TARGET_MODEL_CODENAME="icosa"
elif [ $TARGET_DEVICE_MODEL == 1 ]
then TARGET_MODEL_CODENAME="iowa"
fi

# Copy BootShim + UEFI to a Payload File
cat ./BootShim/AARCH64/BootShim.bin "./Build/HAC-001Pkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/HAC-001_UEFI.fd" > ./Mu-${TARGET_MODEL_CODENAME}.bin||exit 1
