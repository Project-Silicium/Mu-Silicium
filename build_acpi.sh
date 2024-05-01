#!/bin/bash

# Function to display Help Message
function _help(){
	echo "Usage: ./build_acpi.sh -d <Codename> [-a]"
	echo
	echo "Build ACPI of a Specific Device."
	echo
	echo "Options:"
	echo "	--device <Codename>, -d <Codename>:         The Codename of the Device you want to Build the ACPI."
    echo "  --asl, -a:                                  Use asl.exe instead of iasl to compile ACPI Tables."
    echo "	--help, -h:                                 Shows this Help."
	echo
	echo "MainPage: https://github.com/Robotix22/Mu-Silicium"
	exit 1
}

# Functions to display the Message Type (Error, Warning or Notification)
function _error(){ echo -e "\033[1;31m${@}\033[0m" >&2;exit 1; }
function _warn(){ echo -e "\033[0;33m${@}\033[0m" >&2; }
function _ntf(){ echo -e "\033[0;32m${@}\033[0m" >&2; }

# Check if any args were given
OPTS="$(getopt -o d:ah -l device:,asl,help -n 'build_acpi.sh' -- "$@")"||exit 1
eval set -- "${OPTS}"
while true
do	case "${1}" in
		-d|--device) TARGET_DEVICE="${2}";shift 2;;
        -a|--asl) USE_ASL="TRUE";shift;;
		-h|--help) _help 0;shift;;
		--) shift;break;;
		*) _help 1;;
	esac
done

# If no Device arg is present, Display the Help Message
if [ -z ${TARGET_DEVICE} ]
then _help
fi

echo -e "\nUpdating device ACPI tables...\n"
[[ -d Build ]] || mkdir Build
echo -e "PROGRESS - ACPI updater" > ./Build/acpi.log
TARGET_DEVICE_VENDOR=$(grep TARGET_DEVICE_VENDOR ./Resources/Configs/$TARGET_DEVICE.conf | sed 's/\"/ /g' | awk '{print $2}')
[ -z $USE_ASL ] && USE_ASL=$(grep USE_ASL ./Resources/Configs/$TARGET_DEVICE.conf | tr -d 'USE_ASL=')
if [ -f ./Platforms/$TARGET_DEVICE_VENDOR/${TARGET_DEVICE}Pkg/Include/ACPI.inc ]; then
	TABLES="$(grep "SECTION RAW" ./Platforms/$TARGET_DEVICE_VENDOR/${TARGET_DEVICE}Pkg/Include/ACPI.inc | sed '/#.*SECTION RAW/d' | grep $TARGET_DEVICE | awk '{print $4}')"
	for TABLE in $TABLES; do
		echo "INFO - $TABLE" >> ./Build/acpi.log
		TABLE_DIR=$(dirname $TABLE)
		TABLE_NAME=$(basename $TABLE)
		TABLE_NAME_ASL=$(basename $TABLE | sed 's/.aml$/.asl/g')
		TABLE_NAME_DSL=$(basename $TABLE | sed 's/.aml$/.dsl/g')
		[ -f ./Platforms/$TARGET_DEVICE_VENDOR/$TABLE_DIR/$TABLE_NAME ] && mv -f ./Platforms/$TARGET_DEVICE_VENDOR/$TABLE_DIR/$TABLE_NAME ./Platforms/$TARGET_DEVICE_VENDOR/$TABLE_DIR/$TABLE_NAME.tablebkp > /dev/null 2>&1
		if [ -f ./Platforms/$TARGET_DEVICE_VENDOR/$TABLE_DIR/$TABLE_NAME_ASL ] || [ -f ./Platforms/$TARGET_DEVICE_VENDOR/$TABLE_DIR/$TABLE_NAME_DSL ]; then
			[ -f ./Platforms/$TARGET_DEVICE_VENDOR/$TABLE_DIR/$TABLE_NAME_ASL ] && TABLE_NAME_OUT=$TABLE_NAME_ASL
			[ -f ./Platforms/$TARGET_DEVICE_VENDOR/$TABLE_DIR/$TABLE_NAME_DSL ] && TABLE_NAME_OUT=$TABLE_NAME_DSL
			if [ "$USE_ASL" == "TRUE" ]; then
				if [[ $(grep -i Microsoft /proc/version) ]]; then
					chmod 777 ./tools/asl.exe > /dev/null 2>&1
					echo -e "Running asl.exe using Windows itself...\n"
					RUN=$(./tools/asl.exe ./Platforms/$TARGET_DEVICE_VENDOR/$TABLE_DIR/$TABLE_NAME_OUT)
				else
					echo -e "Running asl.exe using wine...\n"
					RUN=$(wine ./tools/asl.exe ./Platforms/$TARGET_DEVICE_VENDOR/$TABLE_DIR/$TABLE_NAME_OUT)
				fi
			else
				echo -e "Running iasl...\n"
				RUN=$(iasl ./Platforms/$TARGET_DEVICE_VENDOR/$TABLE_DIR/$TABLE_NAME_OUT 2>&1 >/dev/null)
			fi
			if [ $(echo "$RUN" | grep Error |wc -c) -gt 1 ]; then
				_warn "Could not update $TABLE_NAME: $RUN"
				echo "DEBUG - Could not update $TABLE_NAME: $RUN" >> ./Build/acpi.log
				sleep 3
			else
				_ntf "$TABLE_NAME updated successfully!"
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
	echo -e "ACPI update done!\n\n"
	echo -e "PROGRESS - ACPI update done" >> ./Build/acpi.log
	sleep 2
else
	_error "\nThere's no ACPI.inc for $TARGET_DEVICE\nAborting...\n"
	echo -e "CRITICAL - There's no ACPI.inc for $TARGET_DEVICE" >> ./Build/acpi.log
fi
