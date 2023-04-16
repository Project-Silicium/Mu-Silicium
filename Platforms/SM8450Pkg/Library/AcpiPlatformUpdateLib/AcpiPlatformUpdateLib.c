#include <Uefi.h>

#include <Library/AcpiPlatformUpdateLib.h>
//#include <Library/AslUpdateLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#include <Library/MemoryMapHelperLib.h>
#include <Library/RFSProtectionLib.h>

#include <Protocol/EFIChipInfo.h>
#include <Protocol/EFIPlatformInfo.h>
#include <Protocol/EFISmem.h>

VOID
PlatformUpdateAcpiTables(VOID)
{
  // Also configure MPSS Permissions!

  // Allow MPSS and HLOS to access the allocated RFS Shared Memory Region
  // Normally this would be done by a driver in Linux
  // TODO: Move to a better place!
  RFSLocateAndProtectSharedArea();
}
