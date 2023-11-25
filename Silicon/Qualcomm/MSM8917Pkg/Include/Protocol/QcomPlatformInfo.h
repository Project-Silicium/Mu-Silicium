#ifndef _QCOM_PLATFORM_INFO_H_
#define _QCOM_PLATFORM_INFO_H_

#include <Device/Board.h>

//
// Global ID for the Smem Protocol
//
#define QCOM_PLATFORM_INFO_PROTOCOL_GUID { 0xb4dd5113, 0xcd78, 0x4142, { 0xaf, 0x1f, 0x7e, 0xc2, 0x51, 0xea, 0x4e, 0x38 } }

//
// Declare forward referenced to the Smem Protocol
//
typedef struct _QCOM_PLATFORM_INFO_PROTOCOL QCOM_PLATFORM_INFO_PROTOCOL;

/**
  This Functions Gets the Platform ID.

  @return Platform ID
**/
typedef
UINT32
(EFIAPI *EFI_GET_PLATFORM_ID)();

/**
  This Functions Gets the Foundry ID.

  @return Foundry ID
**/
typedef
UINT32
(EFIAPI *EFI_GET_FOUNDRY_ID)();

/**
  This Functions Gets the Chip Serial Number.

  @return Chip Serial Number
**/
typedef
UINT32
(EFIAPI *EFI_GET_CHIP_SERIAL_NUMBER)();

/**
  This Functions Gets the Platform Version.

  @return Platform Version
**/
typedef
UINT32
(EFIAPI *EFI_GET_PLATFORM_VERSION)();

/**
  This Functions Gets the Platform HW.

  @return Platform HW
**/
typedef
UINT32
(EFIAPI *EFI_GET_PLATFORM_HW)();

/**
  This Functions Gets the Platform Subtype.

  @return Platform Subtype
**/
typedef
UINT32
(EFIAPI *EFI_GET_PLATFORM_SUBTYPE)();

/**
  This Functions Gets the Target.

  @return Target
**/
typedef
UINT32
(EFIAPI *EFI_GET_TARGET)();

/**
  This Functions Gets the Baseband.

  @return Baseband
**/
typedef
UINT32
(EFIAPI *EFI_GET_BASEBAND)();

/**
  This Functions Gets the Platform HLOS Subtype.

  @return Platform HLOS Subtype
**/
typedef
UINT32
(EFIAPI *EFI_GET_PLATFORM_HLOS_SUBTYPE)();

/**
  This Functions Gets the Numbers of Pmics.

  @return Numbers of Pmics
**/
typedef
UINT32
(EFIAPI *EFI_GET_NUM_PMIC)();

/**
  This Functions Gets the Pmic Array Offset.

  @return Pmic Array Offset
**/
typedef
UINT32
(EFIAPI *EFI_GET_PMIC_ARRAY_OFFSET)();

//
// Define Protocol Functions
//
struct _QCOM_PLATFORM_INFO_PROTOCOL {
  EFI_GET_PLATFORM_ID           GetPlatformID;
  EFI_GET_FOUNDRY_ID            GetFoundryID;
  EFI_GET_CHIP_SERIAL_NUMBER    GetChipSerialNumber;
  EFI_GET_PLATFORM_VERSION      GetPlatformVersion;
  EFI_GET_PLATFORM_HW           GetPlatformHW;
  EFI_GET_PLATFORM_SUBTYPE      GetPlatformSubtype;
  EFI_GET_TARGET                GetTarget;
  EFI_GET_BASEBAND              GetBaseband;
  EFI_GET_PLATFORM_HLOS_SUBTYPE GetPlatformHlosSubtype;
  EFI_GET_NUM_PMIC              GetNumPmic;
  EFI_GET_PMIC_ARRAY_OFFSET     GetPmicArrayOffset;
};

extern EFI_GUID gPlatformInfoProtocolGuid;

#endif /* _QCOM_PLATFORM_INFO_H_ */