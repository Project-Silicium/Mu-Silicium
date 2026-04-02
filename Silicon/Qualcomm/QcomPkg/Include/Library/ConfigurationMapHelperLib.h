#ifndef _CONFIGURATION_MAP_HELPER_LIB_H_
#define _CONFIGURATION_MAP_HELPER_LIB_H_

#include <Library/ConfigurationMapLib.h>

/**
  This Function Gets the Value (32) of the Specified Config Entry.

  @param[in]  EntryName                    - The Name of the Config Entry.
  @param[out] EntryValue                   - The Value (32) of the Specified Config Entry.

  @return EFI_SUCCESS                      - The Config Entry was Successfully Found.
  @return EFI_INVALID_PARAMETER            - One or more Parameters are NULL.
  @return EFI_NOT_FOUND                    - The Specified Config Entry does not Exist.
**/
EFI_STATUS
EFIAPI
LocateConfigurationEntry32 (
  IN  CHAR8  *EntryName,
  OUT UINT32 *EntryValue
  );

/**
  This Function Gets the Value (64) of the Specified Config Entry.

  @param[in]  EntryName                    - The Name of the Config Entry.
  @param[out] EntryValue                   - The Value (64) of the Specified Config Entry.

  @return EFI_SUCCESS                      - The Config Entry was Successfully Found.
  @return EFI_INVALID_PARAMETER            - One or more Parameters are NULL.
  @return EFI_NOT_FOUND                    - The Specified Config Entry does not Exist.
**/
EFI_STATUS
EFIAPI
LocateConfigurationEntry64 (
  IN  CHAR8  *EntryName,
  OUT UINT64 *EntryValue
  );

#endif /* _CONFIGURATION_MAP_HELPER_LIB_H_ */
