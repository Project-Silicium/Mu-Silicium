#!/bin/bash

# Help Message Function
function _help(){
	echo "Usage: ./build_active_devices.sh [-r <Build Mode>]"
	echo
	echo "Build Project Silicium UEFI for all active Devices."
	echo
	echo "Options:"
	echo "	--release <Build Mode>, -r <Build Mode>:    Defines the Release Type of the Build."
	echo "	--help, -h:                                 Shows this Help."
	echo
	echo "MainPage: https://github.com/Project-Silicium/Mu-Silicium"
	exit 1
}

# Message Functions (Error & Warning)
function _error(){ echo -e "\033[1;31m${@}\033[0m" >&2;exit 1; }
function _warn(){ echo -e "\033[0;33m${@}\033[0m" >&2; }

# Check for Parameters
OPTS="$(getopt -o r:h -l release:,help -n 'build_uefi.sh' -- "$@")"||exit 1
eval set -- "${OPTS}"

# Parse Parameters
while true
do case "${1}" in
		-r|--release) TARGET_BUILD_MODE="${2}";shift 2;;
		-h|--help) _help 0;shift;;
		--) shift;break;;
		*) _help 1;;
	esac
done

# Parse Active Devices from Status.md
DEVICES=$(awk '/**State: Active**/,/**Codename:/ {if ($0 ~ /**Codename:/) print substr($2, 1, length($2)-2)}' Status.md)

# Build UEFI Images
for Device in $DEVICES; do
	# Parse Device Config File
	if [ -f "Resources/Configs/${Device}.conf" ]
	then source "Resources/Configs/${Device}.conf"
	else _error "\nDevice configuration not found from ${Device}!\nCheck if your .conf File is in the 'configs' Folder\n"
	fi

	# Check Device Models
	if [ ${TARGET_MULTIPLE_MODELS} == 1 ]; then
		# Build each Device Model
		for ((Model = 0; Model < $TARGET_NUMBER_OF_MODELS; Model++)); do
			bash ./build_uefi.sh -d $Device -r $TARGET_BUILD_MODE -c -m $Model || exit $?
		done
	else
		# Build Main Device
		bash ./build_uefi.sh -d $Device -r $TARGET_BUILD_MODE -c || exit $?
	fi
done