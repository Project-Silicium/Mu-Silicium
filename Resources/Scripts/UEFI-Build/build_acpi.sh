#!/bin/bash

# Set Default Values
COMPILE_DEVICE_DSDT="TRUE"
COMPILE_SOC_ACPI="TRUE"

# Set Tools Path
TOOL_PATH="${PWD}/tools"

# Set Mu-Silicium Path
SILICIUM_HOME_PATH="${PWD}"

# Check if Target Device & SoC has ACPI Source
[[ -d "./Silicium-ACPI/Platforms/${TARGET_DEVICE_VENDOR}/${TARGET_DEVICE}" ]] || COMPILE_DEVICE_DSDT="FALSE"
[[ -d "./Silicium-ACPI/SoCs/${TARGET_SOC_VENDOR}/${TARGET_SOC}" ]] || COMPILE_SOC_ACPI="FALSE"

# Delete Device Output Files
if [ ${COMPILE_DEVICE_DSDT} == "TRUE" ]; then
	rm "./Silicium-ACPI/Platforms/${TARGET_DEVICE_VENDOR}/${TARGET_DEVICE}/DSDT.aml" &> /dev/null
	rm "./Silicium-ACPI/Platforms/${TARGET_DEVICE_VENDOR}/${TARGET_DEVICE}/DSDT.pre" &> /dev/null
fi

# Delete SoC Output Files
if [ ${COMPILE_SOC_ACPI} == "TRUE" ]; then
	rm ./Silicium-ACPI/SoCs/${TARGET_SOC_VENDOR}/${TARGET_SOC}/*.aml &> /dev/null
	rm ./Silicium-ACPI/SoCs/${TARGET_SOC_VENDOR}/${TARGET_SOC}/*.pre &> /dev/null
fi

# Compile Device DSDT
if [ ${COMPILE_DEVICE_DSDT} == "TRUE" ]; then
	cd Silicium-ACPI/Platforms/${TARGET_DEVICE_VENDOR}/${TARGET_DEVICE}

	if [[ $(grep -i Microsoft /proc/version) ]]; then
		${TOOL_PATH}/asl.exe DSDT.asl || (${TOOL_PATH}/asl.exe DSDT.dsl || (iasl DSDT.asl || (iasl DSDT.dsl || _error "\nFailed to Compile ${TARGET_DEVICE} DSDT Table!\n")))
	else
		wine ${TOOL_PATH}/asl.exe DSDT.asl || (wine ${TOOL_PATH}/asl.exe DSDT.dsl || (iasl DSDT.asl || (iasl DSDT.dsl || _error "\nFailed to Compile ${TARGET_DEVICE} DSDT Table!\n")))
	fi

	cd ${SILICIUM_HOME_PATH}
fi

# Compile SoC ACPI Tables
if [ ${COMPILE_SOC_ACPI} == "TRUE" ]; then
	cd Silicium-ACPI/SoCs/${TARGET_SOC_VENDOR}/${TARGET_SOC}

	iasl *.dsl || _error "\nFailed to Compile ${TARGET_SOC} ACPI Tables!\n"

	cd ${SILICIUM_HOME_PATH}
fi
