#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>

EFI_STATUS
LocateConfigurationEntryString (
  IN  CHAR8 *EntryName,
  OUT CHAR8 *EntryValue,
  IN  UINTN *EntryValueLength)
{
  // Verify Parameters
  if (EntryName == NULL || EntryValue == NULL || EntryValueLength == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Get Platform Type
  CHAR8 *PlatformType = FixedPcdGetPtr (PcdPlatformType);

  // Verify Platform Type
  ASSERT (PlatformType != "NULL");

  // Compare Entry Names
  if (!AsciiStriCmp (EntryName, "OsTypeString")) {
    // Convert Entry Value to String
    AsciiStrCpyS (EntryValue, *EntryValueLength, PlatformType);

    return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
}
