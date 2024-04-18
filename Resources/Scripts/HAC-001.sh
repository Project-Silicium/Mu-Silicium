#!/bin/bash

# Copy BootShim + UEFI to a Payload File
cat ./BootShim/AARCH64/BootShim.bin "./Build/HAC-001Pkg/${_TARGET_BUILD_MODE}_CLANGDWARF/FV/HAC-001_UEFI.fd" > ./Mu-HAC-001.bin||exit 1
