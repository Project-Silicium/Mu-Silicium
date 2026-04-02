#include <PiPei.h>

#include <Library/ConfigurationMapHelperLib.h>

EFI_STATUS
EFIAPI
LocateConfigurationEntry32 (
  IN  CHAR8  *EntryName,
  OUT UINT32 *EntryValue)
{
  EFI_CONFIGURATION_ENTRY_DESCRIPTOR *ConfigurationDescriptor;
  UINT8                               ConfigurationDescriptorCount;

  // Verify Parameter
  if (EntryName == NULL || EntryValue == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Get Configuratio Map
  GetConfigurationMap (&ConfigurationDescriptor, &ConfigurationDescriptorCount);

  // Go thru each Config Entry
  for (UINT8 i = 0; i < ConfigurationDescriptorCount; i++) {
    // Compare Entry Names
    if (AsciiStriCmp (EntryName, ConfigurationDescriptor[i].Name)) {
      continue;
    }

    // Pass Entry Value
    *EntryValue = (UINT32)(ConfigurationDescriptor[i].Value & 0xFFFFFFFF);

    return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
}

EFI_STATUS
EFIAPI
LocateConfigurationEntry64 (
  IN  CHAR8  *EntryName,
  OUT UINT64 *EntryValue)
{
  EFI_CONFIGURATION_ENTRY_DESCRIPTOR *ConfigurationDescriptor;
  UINT8                               ConfigurationDescriptorCount;

  // Verify Parameter
  if (EntryName == NULL || EntryValue == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Get Configuratio Map
  GetConfigurationMap (&ConfigurationDescriptor, &ConfigurationDescriptorCount);

  // Go thru each Config Entry
  for (UINT8 i = 0; i < ConfigurationDescriptorCount; i++) {
    // Compare Entry Names
    if (AsciiStriCmp (EntryName, ConfigurationDescriptor[i].Name)) {
      continue;
    }

    // Pass Entry Value
    *EntryValue = ConfigurationDescriptor[i].Value;

    return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
}
