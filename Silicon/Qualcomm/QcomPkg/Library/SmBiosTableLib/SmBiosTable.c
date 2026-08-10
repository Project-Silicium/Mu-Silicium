#include <Library/DebugLib.h>
#include <Library/SmBiosTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/BaseLib.h>

#include "SmBiosTable.h"

VOID
PlatformUpdateSmBiosType1 (IN OUT EFI_SMBIOS_TYPE1 *Type1)
{
  EFI_STATUS  Status;
  CHAR8      *ChipIdString;

  // Allocate Memory
  ChipIdString = AllocateZeroPool (sizeof (CHAR8) * 16);
  if (ChipIdString == NULL) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Allocate Memory for Chip ID!\n", __FUNCTION__));
    return;
  }

  // Get Chip ID String
  Status = GetChipIdString (ChipIdString);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to get Chip ID String! Status = %r\n", __FUNCTION__, Status));

    // Free Buffer
    FreePool (ChipIdString);
  } else {
    // Update System Family String
    Type1->Strings[5] = ChipIdString;
  }
}

VOID
PlatformUpdateSmBiosType4 (IN OUT EFI_SMBIOS_TYPE4 *Type4)
{
  EFI_STATUS Status;
  UINT16     MaxSpeed;
  UINT16     CurrentSpeed;

  // Get Last Cluster
  UINT32 LastCluster = FixedPcdGet32 (PcdClusterCount) - 1;

  // Get Last Cluster Speeds
  Status = GetClusterSpeeds (LastCluster, &MaxSpeed, &CurrentSpeed);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to get Cluster %u Speeds! Status = %r\n", __FUNCTION__, LastCluster, Status));
  } else {
    // Update CPU Speeds
    Type4->Table.MaxSpeed     = MaxSpeed;
    Type4->Table.CurrentSpeed = CurrentSpeed;
  }

  // Update Processor Manufacturer Strings
  Type4->Strings[0] = "Qualcomm";
  Type4->Strings[1] = "Qualcomm";
}

VOID
PlatformUpdateSmBiosType17 (IN OUT EFI_SMBIOS_TYPE17 *Type17)
{
  EFI_STATUS         Status;
  MEMORY_DEVICE_TYPE MemoryType;
  UINT32             MaxSpeed;
  UINT32             CurrentSpeed;
  UINT8              ManufacturerId;

  // Get DDR Details
  Status = GetDdrDetails (&ManufacturerId, &MemoryType);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to get DDR Details! Status = %r\n", __FUNCTION__, Status));
  } else {
    // Update Memory Type
    Type17->Table.MemoryType = MemoryType;

    // Update Manufacturer ID
    Type17->Table.ModuleManufacturerID                    = ManufacturerId;
    Type17->Table.MemorySubsystemControllerManufacturerID = ManufacturerId;

    // Update Manufacturer String
    Type17->Strings[2] = ConvertManufacturerId (ManufacturerId);
  }

  // Get DDR Speed
  Status = GetDdrSpeeds (&MaxSpeed, &CurrentSpeed);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to get DDR Speeds! Status = %r\n", __FUNCTION__, Status));
  } else {
    // Check Max Speed
    if (MaxSpeed < 0xFFFF) {
      // Update Max Memory Speed
      Type17->Table.Speed = (UINT16)MaxSpeed;
    } else {
      // Update Max Memory Speed
      Type17->Table.Speed         = 0xFFFF;
      Type17->Table.ExtendedSpeed = MaxSpeed;
    }

    // Check Current Speed
    if (CurrentSpeed < 0xFFFF) {
      // Update Current Memory Speed
      Type17->Table.ConfiguredMemoryClockSpeed = (UINT16)CurrentSpeed;
    } else {
      // Update Current Memory Speed
      Type17->Table.ConfiguredMemoryClockSpeed    = 0xFFFF;
      Type17->Table.ExtendedConfiguredMemorySpeed = CurrentSpeed;
    }
  }
}

VOID
PlatformUpdateSmBiosTables (IN OUT EFI_SMBIOS_TABLES *SmBiosTables)
{
  // Update SMBIOS Tables
  PlatformUpdateSmBiosType1  (SmBiosTables->Type1);
  PlatformUpdateSmBiosType4  (SmBiosTables->Type4);
  PlatformUpdateSmBiosType17 (SmBiosTables->Type17);
}

VOID
PlatformSmBiosType1CleanUp (IN OUT EFI_SMBIOS_TYPE1 *Type1)
{
  // Get System Family String
  CHAR8 *SystemFamily = Type1->Strings[5];

  // Free System Family Buffer
  if (SystemFamily != NULL && AsciiStrCmp (SystemFamily, "Not Specified") != 0) {
    FreePool (SystemFamily);
  }
}

VOID
PlatformSmBiosCleanUp (IN OUT EFI_SMBIOS_TABLES *SmBiosTables)
{
  // Do TYPE1 Clean Up
  PlatformSmBiosType1CleanUp (SmBiosTables->Type1);
}

EFI_STATUS
EFIAPI
SmBiosTablesLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  // Initialize Utility Functions
  InitializeUtilities ();

  return EFI_SUCCESS;
}
