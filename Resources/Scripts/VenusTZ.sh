#!/bin/bash

# Copy BootShim + UEFI to a Payload File
cat ./BootShim/ARM/BootShim.bin "./Build/VenusTZPkg/${TARGET_BUILD_MODE}_CLANGDWARF/FV/VENUSTZ_UEFI.fd" > ./Mu-VenusTZ.bin||exit 1

# Copy UEFILoader to Root Directory
cp "./Build/VenusTZPkg/${TARGET_BUILD_MODE}_CLANGDWARF/ARM/UEFILoader.efi" ./UEFILoader.efi
