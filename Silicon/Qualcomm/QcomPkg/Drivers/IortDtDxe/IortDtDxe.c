#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/FdtLib.h>

//
// Global Variables
//
STATIC CHAR8 *XblDtMemoryName[] = {"DT BLOB", "XBL DT", "XBL_DT"};

EFI_STATUS
ModifyDtIort (IN CONST VOID *XblDt)
{
  INT32 FdtStatus;

  // Get UFS IORT Node Offset
  INT32 UfsIortNodeOffset = FdtPathOffset (XblDt, "/soc/iort/NAMEDNODE_UFS_95MEM");

  // Verify Node Offset
  if (UfsIortNodeOffset < 0) {
    DEBUG ((EFI_D_ERROR, "Failed to Find UFS IORT Node!\n"));
    return EFI_ABORTED;
  }

  // Set UFS Cache Coherency Value
  UINT32 UfsCacheCoherency = CpuToFdt32 (1);

  // Enable UFS Cache Coherency
  FdtStatus = FdtSetProperty ((VOID *)XblDt, UfsIortNodeOffset, "CacheCoherency", &UfsCacheCoherency, sizeof (UfsCacheCoherency));
  if (FdtStatus < 0) {
    DEBUG ((EFI_D_ERROR, "Failed to Enable UFS Cache Coherency! FDT Status = %a\n", FdtStrerror (FdtStatus)));
    return EFI_ABORTED;
  }

  DEBUG ((EFI_D_WARN, "Successfully Enabled UFS Cache Coherency\n"));

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
DiscoverDt (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_MEMORY_REGION_DESCRIPTOR_EX XblDtRegion;

  // Locate XBL DT Memory Region
  for (UINT8 i = 0; i < ARRAY_SIZE (XblDtMemoryName); i++) {
    LocateMemoryMapAreaByName (XblDtMemoryName[i], &XblDtRegion);
  }

  // Verify XBL DT Memory Region
  if (!XblDtRegion.Address) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate XBL DT Memory Region!\n"));
    return EFI_NOT_FOUND;
  }

  // Find XBL DT
  for (EFI_PHYSICAL_ADDRESS Current = XblDtRegion.Address; Current < XblDtRegion.Address + XblDtRegion.Length; Current += 4) {
    // Set DT Memory Location
    CONST VOID *XblDt = (CONST VOID *)Current;

    // Verify DT Header
    if (FdtCheckHeader (XblDt) < 0) {
      continue;
    }

    // Modify IORT in DT
    return ModifyDtIort (XblDt);
  }

  DEBUG ((EFI_D_ERROR, "Failed to Find the XBL DT!\n"));

  return EFI_NOT_FOUND;
}
