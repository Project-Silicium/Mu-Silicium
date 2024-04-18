#include <Library/CacheMaintenanceLib.h>
#include <Library/MemoryMapHelperLib.h>

#include "PStore.h"

STATIC ARM_MEMORY_REGION_DESCRIPTOR_EX PStoreMemoryRegion;

VOID
WriteMemory (UINT8 Buffer)
{
  UINTN  Size   = PStoreMemoryRegion.Length - sizeof(UINTN);
  UINT8 *Base   = (UINT8 *)PStoreMemoryRegion.Address;
  UINTN *Offset = (UINTN *)((UINTN)Base + Size);

  *Offset       = *Offset % Size;
  Base[*Offset] = Buffer;
  *Offset       = (*Offset + 1) % Size;

  WriteBackInvalidateDataCacheRange (Base, Size);
}

BOOLEAN
MemoryLogInit ()
{
  EFI_STATUS Status;

  // Get PStore Base Address
  Status = LocateMemoryMapAreaByName ("PStore", &PStoreMemoryRegion);
  if (EFI_ERROR (Status)) { return FALSE; }

  return TRUE;
}
