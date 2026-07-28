#include <Library/BaseLib.h>
#include <Library/ConfigurationMapLib.h>
#include <Library/ConfigurationMapHelperLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/SerialPortLib.h>

#include "ShimLibraries/EFIShim.h"
#include "ShimLibraries/EFISerialPort.h"
#include "ShimLibraries/EFIUefiConfig.h"
#include "PlatformPei.h"

//
// UEFI Config Library
//
EFI_UEFI_CONFIG_LIBRARY
ConfigLib = {
  0x10002,
  LocateMemoryRegionByName,
  LocateConfigurationEntryString,
  LocateConfigurationEntry32,
  LocateConfigurationEntry64, 
  LocateMemoryRegionByAddress
};

//
// Serial Port Library
//
EFI_SERIAL_PORT_LIBRARY
SioLib = {
  0x10001,
  SerialPortRead,
  SerialPortWrite,
  SerialPortPoll,
  SerialPortDrain,
  SerialPortFlush,
  SerialPortControl,
  SerialPortSetAttributes
};

EFI_STATUS
ShimInstallLib (
  IN CHAR8  *LibName,
  IN UINT32  LibVersion,
  IN VOID   *LibIntf)
{
  return EFI_SUCCESS;
}

EFI_SHIM_LIBRARY_INSTANCE_DATA
gShimLibraryInstanceData[] = {
  // Library Instance Name, Library Instance Pointer
  {"UEFI Config Lib", &ConfigLib},
  {"SerialPort Lib",  &SioLib}
};

EFI_STATUS
ShimLoadLib (
  IN  CHAR8   *LibName,
  IN  UINT32   LibVersion,
  OUT VOID   **LibIntf)
{
  // Very Parameters
  if (LibName == NULL || LibIntf == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Go thru each Shim Library Instance
  for (UINT8 i = 0; i < ARRAY_SIZE (gShimLibraryInstanceData); i++) {
    // Compare Library Instance Names
    if (AsciiStriCmp (LibName, gShimLibraryInstanceData[i].LibName)) {
      continue;
    }

    // Pass Library Instance
    *LibIntf = gShimLibraryInstanceData[i].LibIntf;

    return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
}
