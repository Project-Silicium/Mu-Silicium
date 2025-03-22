#include <Library/SettingsHandlerLib.h>

#include "DeviceSettings.h"

//
// Device Setting Data
//
STATIC SETTING_DATA DefaultDeviceSettingData = {
  DEVICE_SETTING_DATA_REVISION,
  {
    //
    // End of Settings
    //
    {0xFF, 0xFFFFFFFF}
  }
};

//
// Device Setting Attributes
//
STATIC SETTING_ATTRIBUTE_DATA DeviceSettingAttributes[] = {
  {"Terminator", 0xFF, {0}, TRUE, TRUE}
};

SETTING_DATA
GetDefaultDeviceSettingData ()
{
  return DefaultDeviceSettingData;
}

SETTING_ATTRIBUTE_DATA*
GetDeviceSettingAttributes ()
{
  return DeviceSettingAttributes;
}

EFI_STATUS
UpdateDeviceSettingData (
  IN  UINT8        Revision,
  IN  SETTING_DATA OldSettingData,
  OUT SETTING_DATA NewSettingData)
{
  // Nothing to Update yet.
  return EFI_SUCCESS;
}
