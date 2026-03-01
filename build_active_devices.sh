#!/bin/bash

# Help Message Function
function _help(){
	echo "Usage: ./build_active_devices.sh [-r <Build Mode>]"
	echo
	echo "Build Project Silicium UEFI for all active Devices."
	echo
	echo "Options:"
	echo "	--release <Build Mode>, -r <Build Mode>:    Defines the Release Type of the Build."
	echo "	--disable-secureboot, -i:                   Disables Secure Boot."
	echo "	--help, -h:                                 Shows this Help."
	echo
	echo "MainPage: https://github.com/Project-Silicium/Mu-Silicium"
	exit 1
}

# Message Functions (Error & Warning)
function _error(){ echo -e "\033[1;31m${@}\033[0m" >&2;exit 1; }
function _warn(){ echo -e "\033[0;33m${@}\033[0m" >&2; }

# Check for Parameters
OPTS="$(getopt -o r:ih -l release:,disable-secureboot,help -n 'build_active_devices.sh' -- "$@")"||exit 1
eval set -- "${OPTS}"

# Set Default Security Argument
TARGET_DISABLE_SECUREBOOT=0

# Parse Parameters
while true
do case "${1}" in
		-r|--release) TARGET_BUILD_MODE="${2}";shift 2;;
		-i|--disable-secureboot) TARGET_DISABLE_SECUREBOOT=1;shift;;
		-h|--help) _help 0;shift;;
		--) shift;break;;
		*) _help 1;;
	esac
done

# Parse Active Devices from Status.md
ACTIVE_DEVICES=$(grep -A 2 "\*\*State:\*\* Active" Status.md | grep "\*\*Codename:\*\*" | sed 's/.*\*\*Codename:\*\* //; s/<[^>]*>//g')

# Build UEFI Images
for TARGET_DEVICE in $ACTIVE_DEVICES; do
	# Get Number of Models
	TARGET_NUMBER_OF_MODELS=$(grep -A 5 "\*\*Codename:\*\* $TARGET_DEVICE" Status.md | grep "\*\*Models:\*\*" | sed 's/.*\*\*Models:\*\* //; s/<[^>]*>//g')

	# Verify Model
	TARGET_NUMBER_OF_MODELS=${TARGET_NUMBER_OF_MODELS:-1}

	# Build each Device Model
	for ((TARGET_MODEL = 0; TARGET_MODEL < $TARGET_NUMBER_OF_MODELS; TARGET_MODEL++)); do
		if [[ $TARGET_DISABLE_SECUREBOOT == 1 ]]
		then bash ./build_uefi.sh -d $TARGET_DEVICE -r $TARGET_BUILD_MODE -c -i -m $TARGET_MODEL || exit $?
		else bash ./build_uefi.sh -d $TARGET_DEVICE -r $TARGET_BUILD_MODE -c -m $TARGET_MODEL || exit $?
		fi
	done
done
