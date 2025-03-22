#include <libfdt.h>

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/PcdLib.h>

//
// Global Variables
//
STATIC ARM_MEMORY_REGION_DESCRIPTOR_EX DtbBlobRegion;

EFI_STATUS
MoveModdedDtb (
  IN UINT8  *ModdedDeviceTree,
  IN UINT64  OriginalDeviceTreeAddress)
{
  UINT8 FdtStatus;

  // Populate FDT Structure
  CONST VOID *XblDeviceTree = (CONST VOID *)ModdedDeviceTree;

  // Check DTB Header
  FdtStatus = fdt_check_header (XblDeviceTree);
  if (FdtStatus) {
    DEBUG ((EFI_D_ERROR, "Modded XBL is not a Valid DTB!\n"));
    return EFI_UNSUPPORTED;
  }

  // Check DTB Size
  if (fdt_totalsize (XblDeviceTree) > DtbBlobRegion.Address + DtbBlobRegion.Length - OriginalDeviceTreeAddress) {
    DEBUG ((EFI_D_ERROR, "The Modded XBL DTB is too Large!\n"));
    return EFI_BUFFER_TOO_SMALL;
  }

  // Clear "DT BLOB" Memory Region
  ZeroMem ((VOID *)DtbBlobRegion.Address, DtbBlobRegion.Length);

  // Move Modded XBL DTB
  FdtStatus = fdt_move (XblDeviceTree, (VOID *)OriginalDeviceTreeAddress, fdt_totalsize (XblDeviceTree));
  if (FdtStatus) {
    DEBUG ((EFI_D_ERROR, "Failed to Move Modded DTB to 0x%llx! FDT Status = %d\n", OriginalDeviceTreeAddress, FdtStatus));
    ASSERT (FALSE);
  }

  DEBUG ((EFI_D_WARN, "Successfully Moved Modded XBL DTB to 0x%llx\n", OriginalDeviceTreeAddress));

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
PrepareMove (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS  Status        = EFI_SUCCESS;
  UINT64      XblDtbAddress = 0;
  UINTN       ModdedDtbSize = 0;
  UINT8      *ModdedDtb     = NULL;

  // Locate "DT BLOB" Memory Region
  Status = LocateMemoryMapAreaByName ("DT BLOB", &DtbBlobRegion);
  if (EFI_ERROR (Status)) {
    // Locate "XBL DT" Memory Region
    Status = LocateMemoryMapAreaByName ("XBL DT", &DtbBlobRegion);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Locate 'DT BLOB' and 'XBL DT' Memory Region! Status = %r\n", Status));
      return Status;
    }
  }

  // Get Modded DTB
  Status = GetSectionFromFv (FixedPcdGetPtr (PcdModdedXblDeviceTreeFile), EFI_SECTION_RAW, 0, (VOID **)&ModdedDtb, &ModdedDtbSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Get Modded XBL DTB! Status = %r\n", Status));
    return Status;
  }

  // Find Original XBL DTB
  for (UINT64 CurrentAddress = DtbBlobRegion.Address; CurrentAddress < DtbBlobRegion.Address + DtbBlobRegion.Length; CurrentAddress += 4) {
    // Check Header
    if (*(UINT32 *)(CurrentAddress) == 0xEDFE0DD0) {
      // Save Memory Address
      XblDtbAddress = CurrentAddress;
      break;
    }
  }

  // Check XBL DTB Location
  if (!XblDtbAddress) {
    DEBUG ((EFI_D_ERROR, "No XBL DTB was Found!\n"));
    return EFI_NOT_FOUND;
  }

  // Move Modded DTB
  return MoveModdedDtb (ModdedDtb, XblDtbAddress);
}
