#!/bin/bash

# Copy BootShim + UEFI to a Payload File
cat ./BootShim/ARM/BootShim.bin "./Build/oemkPkg/${TARGET_BUILD_MODE}_CLANGDWARF/FV/OEMK_UEFI.fd" > ./Mu-oemk.bin||exit 1

# Copy UEFILoader to Root Directory
cp "./Build/oemkPkg/${TARGET_BUILD_MODE}_CLANGDWARF/ARM/UEFILoader.efi" ./UEFILoader.efi
