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
ACTIVE_DEVICES=$(awk '/**Codename:**/ { if (DEVICE_STATE == "A" ) { sub (/<br>$/, "", $2); print $2} DEVICE_STATE = ""; next} /**State:**/ { DEVICE_STATE = ($2 == "Active" ? "A" : ""); next} {DEVICE_STATE = ""}' Status.md)

# Build UEFI Images
for TARGET_DEVICE in $ACTIVE_DEVICES; do
	# Get Number of Models
	TARGET_NUMBER_OF_MODELS=$(awk -v TARGET="$TARGET_DEVICE" '/**Codename:**/ {if ($2 ~ TARGET) {FOUND = 1} else {FOUND = 0} next} FOUND && /**Models:**/ {COUNT = $2; sub(/<br>$/, "", COUNT); print COUNT; exit} /**State:**/ {FOUND = 0} END {if (!FOUND || !COUNT) {print 1}}' Status.md)

	# Build each Device Model
	for ((TARGET_MODEL = 0; TARGET_MODEL < $TARGET_NUMBER_OF_MODELS; TARGET_MODEL++)); do
		bash ./build_uefi.sh -d $TARGET_DEVICE -r $TARGET_BUILD_MODE -c -m $TARGET_MODEL || exit $?
	done
done
