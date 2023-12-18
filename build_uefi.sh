#!/bin/bash

# Function to display Help Message
function _help(){
	echo "Usage: ./build_uefi.sh -d <Codename> [-t <Tool Chain> -a <ARCH Type> -r <Build Mode> -m <RAM Size>]"
	echo
	echo "Build Project Mu UEFI for Qualcomm Snapdragon Platforms."
	echo
	echo "Options:"
	echo "	--device <Codename>, -d <Codename>:         Build a Device."
	echo "	--release <Build Mode>, -r <Build Mode>:    Release mode for building, 'RELEASE' is the default or use 'DEBUG' alternatively."
	echo "	--tool-chain <Tool Chain>, -t <Tool Chain>: Define wich Tool Chain to use, 'CLANG38' is default."
	echo "	--help, -h:                                 Shows this Help."
	echo "	--memory <RAM Size>, -m <RAM Size>:         Define how much Memory your Device has."
	echo "  --arch <ARCH Type>, -a <ARCH Type>:         Define Wich ARCH Type to use for Target Device."
	echo "  --acpi, -A:                                 Use iasl to recompile device specific ACPI tables present on ACPI.inc."
	echo
	echo "MainPage: https://github.com/Robotix22/Mu-Qcom"
	exit 1
}

# Functions to display the Message Type (Error, Warning or Notification)
function _error(){ echo -e "\033[1;31m${@}\033[0m" >&2;exit 1; }
function _warn(){ echo -e "\033[0;33m${@}\033[0m" >&2; }
function _ntf(){ echo -e "\033[0;32m${@}\033[0m" >&2; }

# Set Default Defines
TARGET_BUILD_MODE="RELEASE"
MULTIPLE_RAM_SIZE="FALSE"
MULTIPLE_ARCH_TYPE="FALSE"
TOOL_CHAIN_TAG="CLANG38"

function _acpi(){
	echo -e "\nUpdating device ACPI tables\n"
	echo -e "PROGRESS - ACPI updater" > ./Build/acpi.log
	TARGET_DEVICE_VENDOR=$(grep TARGET_DEVICE_VENDOR ./configs/$TARGET_DEVICE.conf | tr -d 'TARGET_DEVICE_VENDOR=' | tr -d '"')
	if [ -f ./Platforms/$TARGET_DEVICE_VENDOR/${TARGET_DEVICE}Pkg/Include/ACPI.inc ]; then
		TABLES="$(grep "SECTION RAW" ./Platforms/$TARGET_DEVICE_VENDOR/${TARGET_DEVICE}Pkg/Include/ACPI.inc | sed '/#.*SECTION RAW/d' | grep $TARGET_DEVICE | awk '{print $4}')"
		for TABLE in $TABLES; do
			echo "INFO - $TABLE" >> ./Build/acpi.log
			TABLE_DIR=$(dirname $TABLE)
			TABLE_NAME=$(basename $TABLE)
			TABLE_NAME_ASL=$(basename $TABLE | sed 's/.aml$/.asl/g')
			TABLE_NAME_DSL=$(basename $TABLE | sed 's/.aml$/.dsl/g')
			[ -f ./Platforms/$TARGET_DEVICE_VENDOR/$TABLE_DIR/$TABLE_NAME ] && mv -f ./Platforms/$TARGET_DEVICE_VENDOR/$TABLE_DIR/$TABLE_NAME ./Platforms/$TARGET_DEVICE_VENDOR/$TABLE_DIR/$TABLE_NAME.tablebkp > /dev/null 2>&1
			if [ -f ./Platforms/$TARGET_DEVICE_VENDOR/$TABLE_DIR/$TABLE_NAME_ASL ]; then
				RUN=$(iasl ./Platforms/$TARGET_DEVICE_VENDOR/$TABLE_DIR/$TABLE_NAME_ASL 2>&1 >/dev/null)
				if [ $(echo "$RUN" | grep Error |wc -c) -gt 1 ]; then
					_warn "Could not update $TABLE_NAME: $RUN"
					echo "DEBUG - Could not update $TABLE_NAME: $RUN" >> ./Build/acpi.log
					sleep 3
				else
					_ntf "$TABLE_NAME updated successfully"
					echo "DEBUG - $TABLE_NAME updated successfully" >> ./Build/acpi.log
				fi
			elif [ -f ./Platforms/$TARGET_DEVICE_VENDOR/$TABLE_DIR/$TABLE_NAME_DSL ]; then
				RUN=$(iasl ./Platforms/$TARGET_DEVICE_VENDOR/$TABLE_DIR/$TABLE_NAME_DSL 2>&1 >/dev/null)
				if [ $(echo "$RUN" | grep Error |wc -c) -gt 1 ]; then
					_warn "Could not update $TABLE_NAME: $RUN"
					echo "DEBUG - Could not update $TABLE_NAME: $RUN" >> ./Build/acpi.log
					sleep 3
				else
					_ntf "$TABLE_NAME updated successfully"
					echo "DEBUG - $TABLE_NAME updated successfully" >> ./Build/acpi.log
				fi
			else
				_warn "Could not find $TABLE_NAME source file on $TABLE_DIR"
				echo "DEBUG - Could not find $TABLE_NAME source file on $TABLE_DIR" >> ./Build/acpi.log
				sleep 3
			fi
			if [ $(echo "$RUN" | grep Error |wc -c) -gt 1 ] && [ -f ./Platforms/$TARGET_DEVICE_VENDOR/$TABLE_DIR/$TABLE_NAME.tablebkp ]; then
				mv -f ./Platforms/$TARGET_DEVICE_VENDOR/$TABLE_DIR/$TABLE_NAME.tablebkp ./Platforms/$TARGET_DEVICE_VENDOR/$TABLE_DIR/$TABLE_NAME > /dev/null 2>&1
			else
				rm -f ./Platforms/$TARGET_DEVICE_VENDOR/$TABLE_DIR/$TABLE_NAME.tablebkp > /dev/null 2>&1
			fi
			echo
		done
		echo -e "ACPI update done\n\n"
		echo -e "PROGRESS - ACPI update done" >> ./Build/acpi.log
		sleep 2
	else
		_error "\nThere's no ACPI.inc for $TARGET_DEVICE\nAborting...\n"
		echo -e "CRITICAL - There's no ACPI.inc for $TARGET_DEVICE" >> ./Build/acpi.log
	fi
}

# Check if any args were given
OPTS="$(getopt -o d:hfbc:r:t:m:a:A -l device:,help,release:,tool-chain:,memory:,arch:,acpi -n 'build_uefi.sh' -- "$@")"||exit 1
eval set -- "${OPTS}"
while true
do	case "${1}" in
		-d|--device) TARGET_DEVICE="${2}";shift 2;;
		-h|--help) _help 0;shift;;
		-r|--release) TARGET_BUILD_MODE="${2}";shift 2;;
		-t|--tool-chain) TOOL_CHAIN_TAG="${2}";shift 2;;
		-m|--memory) TARGET_RAM_SIZE="${2}";shift 2;;
		-a|--arch) TARGET_ARCH_TYPE="${2}";shift 2;;
		-A|--acpi) _acpi;shift;;
		--) shift;break;;
		*) _help 1;;
	esac
done


# If no Device arg is present, Display the Help Message
if [ -z ${TARGET_DEVICE} ]
then _help
fi

# Set Release Type of UEFI
case "${TARGET_BUILD_MODE}" in
	RELEASE) _TARGET_BUILD_MODE=RELEASE;;
	*) _TARGET_BUILD_MODE=DEBUG;;
esac

# Include Device Config if it exists
if [ -f "configs/${TARGET_DEVICE}.conf" ]
then source "configs/${TARGET_DEVICE}.conf"
else _error "\nDevice configuration not found!\nCheck if your .conf File is in the 'configs' Folder\n"
fi

# Check for ARCH Configs and Parameters
if [ -z ${TARGET_ARCH_TYPE} ]
then TARGET_ARCH_TYPE=${DEFAULT_ARCH_TYPE}
else 
	if [ ${MULTIPLE_ARCH_TYPE} == FALSE ]
	then TARGET_ARCH_TYPE=${DEFAULT_ARCH_TYPE}
	fi
fi

# Correct ARCH Parameter
case "${TARGET_ARCH_TYPE}" in
	ARM) _TARGET_ARCH_TYPE=ARM;;
	*) _TARGET_ARCH_TYPE=AARCH64;;
esac

# Check if Device has more that one Static RAM Size
if [ -z ${TARGET_RAM_SIZE} ]; then
	if [ ${MULTIPLE_RAM_SIZE} == TRUE ]
	then _warn "\nThe Device you chose has more than one RAM Size!\nUse -m or --memory to define how much RAM your Device has.\n" && exit 0
	fi
fi

# Delete Output Files if present
rm -r Conf &> /dev/null
rm ./BootShim/${_TARGET_ARCH_TYPE}/BootShim.bin &> /dev/null
rm ./BootShim/${_TARGET_ARCH_TYPE}/BootShim.elf &> /dev/null
rm ./ImageResources/bootpayload.bin &> /dev/null
rm Mu-${TARGET_DEVICE}.img &> /dev/null

# Compile BootShim
cd BootShim/${_TARGET_ARCH_TYPE}
make UEFI_BASE=${TARGET_FD_BASE} UEFI_SIZE=${TARGET_FD_SIZE}||_error "\nFailed to Compile BootShim!\n"
cd ../..

if [ ${_TARGET_ARCH_TYPE} == ARM ]; then
	# Apply Project Mu Patches
	cp ./MuPatches/Compile.patch ./Mu_Basecore/
	cd Mu_Basecore
	git apply Compile.patch
	cd ..
fi

# Start the Real Build of the UEFI
stuart_setup -c "Platforms/${TARGET_DEVICE_VENDOR}/${TARGET_DEVICE}Pkg/PlatformBuild.py" "TOOL_CHAIN_TAG=${TOOL_CHAIN_TAG}"||_error "\nFailed to Setup UEFI Env!\n"
stuart_update -c "Platforms/${TARGET_DEVICE_VENDOR}/${TARGET_DEVICE}Pkg/PlatformBuild.py" "TOOL_CHAIN_TAG=${TOOL_CHAIN_TAG}"||_error "\nFailed to Update UEFI Env!\n"
stuart_build -c "Platforms/${TARGET_DEVICE_VENDOR}/${TARGET_DEVICE}Pkg/PlatformBuild.py" "TOOL_CHAIN_TAG=${TOOL_CHAIN_TAG}" "TARGET=${_TARGET_BUILD_MODE}" "RAM_SIZE=${TARGET_RAM_SIZE}" "FD_BASE=${TARGET_FD_BASE}" "FD_SIZE=${TARGET_FD_SIZE}" "FD_BLOCKS=${TARGET_FD_BLOCKS}" "TARGET_ARCH=${_TARGET_ARCH_TYPE}"||_error "\nFailed to Compile UEFI!\n"

# Execute Device Specific Boot Image Creation
if [ -f "configs/${TARGET_DEVICE}.sh" ]
then source configs/${TARGET_DEVICE}.sh
else _warn "\nImage Creation Script of ${TARGET_DEVICE} has not been Found!\nNo Boot Image Was Generated.\n"
fi

if [[ ${STATUS} != "STABLE" ]]; then
	if [[ ${STATUS} == "UNSTABLE" ]];
	then _warn "\n${TARGET_DEVICE} is marked as Unstable.\nThings are expected to be broken.\n"
	elif [[ ${STATUS} == "LIMITED" ]];
	then _warn "\n${TARGET_DEVICE} is marked as Limited.\nIt's stable but does not have much functionality\n"
	fi
fi

git fetch &> /dev/null
UPDATE_CHECK=$(git status)
if [[ ${UPDATE_CHECK} == *"git pull"* ]];
then _warn "\nYou are using an old Version of Mu-Qcom.\nThis Image may be unstable.\n"
fi

[ -f ./Build/acpi.log ] && sed -i '1 r ./Build/acpi.log' ./Build/BUILDLOG_${TARGET_DEVICE}Pkg.txt && rm -f ./Build/acpi.log
