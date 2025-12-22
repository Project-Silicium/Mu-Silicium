#!/bin/bash

# Set Device SKU
if [ $TARGET_MODEL == 0 ]
then TARGET_MODEL_SKU="icosa"
elif [ $TARGET_MODEL == 1 ]
then TARGET_MODEL_SKU="iowa"
fi

# Copy BootShim + UEFI to a Payload File
cat ./BootShim/BootShim.bin "./Build/nxPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/NX_UEFI.fd" > ./Mu-${TARGET_MODEL_SKU}.bin||exit 1
