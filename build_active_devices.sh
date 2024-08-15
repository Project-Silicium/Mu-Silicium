#!/bin/bash

# Function to display Help Message
function _help(){
	echo "Usage: ./build_active_devices.sh [-r <Build Mode>]"
	echo
	echo "Build Project Mu UEFI for active Devices"
	echo
	echo "Options:"
	echo "	--release <Build Mode>, -r <Build Mode>:    Release mode for building, 'RELEASE' is the default or use 'DEBUG' alternatively."
	echo "	--help, -h:                                 Shows this Help."
	echo
	echo "MainPage: https://github.com/Project-Silicium/Mu-Silicium"
	exit 1
}

# Functions to display the Message Type (Error or Warning)
function _error(){ echo -e "\033[1;31m${@}\033[0m" >&2;exit 1; }
function _warn(){ echo -e "\033[0;33m${@}\033[0m" >&2; }
# Check if any args were given
OPTS="$(getopt -o hr: -l help,release: -n 'build_uefi.sh' -- "$@")"||exit 1
eval set -- "${OPTS}"
while true
do	case "${1}" in
		-h|--help) _help 0;shift;;
		-r|--release) TARGET_BUILD_MODE="${2}";shift 2;;
		--) shift;break;;
		*) _help 1;;
	esac
done

# Set Release Type of UEFI
case "${TARGET_BUILD_MODE}" in
	DEBUG) _TARGET_BUILD_MODE=DEBUG;;
	*) _TARGET_BUILD_MODE=RELEASE;;
esac

# Parse active devices from Status.md
DEVICES=$(awk '/**State: Active**/,/**Codename:/ {if ($0 ~ /**Codename:/) print substr($2, 1, length($2)-2)}' Status.md)

# Build UEFI for active Devices
for Device in $DEVICES; do
	# Include Device Config if it exists
	if [ -f "Resources/Configs/${Device}.conf" ]
	then source "Resources/Configs/${Device}.conf"
	else _error "\nDevice configuration not found from ${Device}!\nCheck if your .conf File is in the 'configs' Folder\n"
	fi

	# Check if the Device has Multiple Models
	if [ ${TARGET_MULTIPLE_MODELS} == "TRUE" ]; then
		# If device has it build device with models
		for ((Model = 0; Model < $TARGET_NUMBER_OF_MODELS; Model++)); do
			bash ./build_uefi.sh -d $Device -r $_TARGET_BUILD_MODE -m $Model || exit $?
		done
	else
		# If device don't have it just run build script
		bash ./build_uefi.sh -d $Device -r $_TARGET_BUILD_MODE || exit $?
	fi
done