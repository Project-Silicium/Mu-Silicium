#include <Library/ConfigurationMapHelperLib.h>
#include <Library/ConfigurationMapLib.h>

EFI_STATUS
EFIAPI
LocateConfigurationMapUINT32ByName (
  IN  CHAR8  *Key,
  OUT UINT32 *Value)
{
  // Get Configuratio Map
  EFI_PCONFIGURATION_ENTRY_DESCRIPTOR_EX ConfigurationDescriptorEx = GetConfigurationMap ();

  // Run through each Configuration Descriptor
  while (ConfigurationDescriptorEx->Value != 0xFFFFFFFF) {
    if (AsciiStriCmp (Key, ConfigurationDescriptorEx->Name) == 0) {
      *Value = (UINT32)(ConfigurationDescriptorEx->Value & 0xFFFFFFFF);
      return EFI_SUCCESS;
    }

    ConfigurationDescriptorEx++;
  }

  return EFI_NOT_FOUND;
}

EFI_STATUS
EFIAPI
LocateConfigurationMapUINT64ByName (
  IN  CHAR8  *Key,
  OUT UINT64 *Value)
{
  // Get Configuratio Map
  EFI_PCONFIGURATION_ENTRY_DESCRIPTOR_EX ConfigurationDescriptorEx = GetConfigurationMap ();

  // Run through each Configuration Descriptor
  while (ConfigurationDescriptorEx->Value != 0xFFFFFFFF) {
    if (AsciiStriCmp (Key, ConfigurationDescriptorEx->Name) == 0) {
      *Value = ConfigurationDescriptorEx->Value;
      return EFI_SUCCESS;
    }

    ConfigurationDescriptorEx++;
  }

  return EFI_NOT_FOUND;
}
