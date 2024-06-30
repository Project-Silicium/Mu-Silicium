#!/bin/bash

# Copy BootShim + UEFI to a Payload File
cat ./BootShim/AARCH64/BootShim.bin "./Build/HAC-001Pkg/${_TARGET_BUILD_MODE}_CLANGPDB/FV/HAC-001_UEFI.fd" > ./Mu-HAC-001-${TARGET_DEVICE_MODEL}-Model.bin||exit 1
