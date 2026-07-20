#include <Library/DebugLib.h>
#include <Library/SmBiosTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/BaseLib.h>

#include "SmBiosTable.h"

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
  PlatformUpdateSmBiosType4  (&SmBiosTables->Type4);
  PlatformUpdateSmBiosType17 (&SmBiosTables->Type17);
}

VOID
PlatformSmBiosCleanUp (IN OUT EFI_SMBIOS_TABLES *SmBiosTables)
{
  return;
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
