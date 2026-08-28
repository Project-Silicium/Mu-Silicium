#include <Library/DebugLib.h>
#include <Library/PlatformUsbfnLib.h>

EFI_STATUS
UsbfnPlatformGetData (OUT USBFN_PLATFORM_DATA *PlatformData)
{
  (VOID)PlatformData;

  DEBUG ((EFI_D_ERROR, "USBFN: PlatformUsbfnLib not implemented for this SoC\n"));

  return EFI_UNSUPPORTED;
}

EFI_STATUS
UsbfnPlatformPhyInit (VOID)
{
  DEBUG ((EFI_D_ERROR, "USBFN: PlatformUsbfnLib not implemented for this SoC\n"));

  return EFI_UNSUPPORTED;
}

EFI_STATUS
UsbfnPlatformPhyExit (VOID)
{
  return EFI_UNSUPPORTED;
}

BOOLEAN
UsbfnPlatformIsVbusPresent (VOID)
{
  return FALSE;
}
