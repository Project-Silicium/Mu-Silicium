#ifndef _BOOT_GRAPHICS_PROVIDER_H_
#define _BOOT_GRAPHICS_PROVIDER_H_

#include <Library/PcdLib.h>

//
// Boot Graphics
//
struct {
  BOOT_GRAPHIC  Id;
  EFI_GUID     *FileGuid;
} BootGraphics[] = {
  // Graphic ID - Graphic File GUID
  {BG_SYSTEM_LOGO,          PcdGetPtr (PcdLogoFile)},
  {BG_CRITICAL_OVER_TEMP,   PcdGetPtr (PcdThermalFile)},
  {BG_CRITICAL_LOW_BATTERY, PcdGetPtr (PcdLowBatteryFile)},
  {BG_NO_BOOT_OS,           FixedPcdGetPtr (PcdNoBootOSFile)},
  {BG_MASS_STORAGE,         FixedPcdGetPtr (PcdMassStorageFile)}
};

#endif /* _BOOT_GRAPHICS_PROVIDER_H_ */
