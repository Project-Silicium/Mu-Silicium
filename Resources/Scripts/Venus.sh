#!/bin/bash

# Copy BootShim + UEFI to a Payload File
cat ./BootShim/ARM/BootShim.bin "./Build/VenusPkg/${_TARGET_BUILD_MODE}_CLANGDWARF/FV/VENUS_UEFI.fd" > ./Mu-Venus.bin||exit 1

# Copy UEFILoader to Root Directory
cp "./Build/VenusPkg/${_TARGET_BUILD_MODE}_CLANGDWARF/ARM/UEFILoader.efi" ./UEFILoader.efi
