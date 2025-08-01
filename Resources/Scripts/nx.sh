#!/bin/bash

# Copy BootShim + UEFI to a Payload File
cat ./BootShim/BootShim.bin "./Build/nxPkg/${TARGET_BUILD_MODE}_CLANGPDB/FV/NX_UEFI.fd" > ./Mu-icosa.bin||exit 1
