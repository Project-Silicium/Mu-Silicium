#ifndef _EFI_UEFI_CONFIG_H_
#define _EFI_UEFI_CONFIG_H_

/**
  This Function gets the String Value of the specified Config Entry.

  @param[in]  EntryName                    - The Name of the Config Entry.
  @param[out] EntryValue                   - The Value of the specified Config Entry.
  @param[in]  EntryValueLength             - The Length of the Config Entry Value String.

  @return EFI_SUCCESS                      - The Config Entry Value was Successfully Found.
  @return EFI_INVALID_PARAMETER            - One or more Arguments are NULL.
  @return EFI_NOT_FOUND                    - The Specified Config Entry does not Exist.
**/
typedef
EFI_STATUS
(*EFI_GET_CONFIG_STRING) (
  IN  CHAR8 *EntryName,
  OUT CHAR8 *EntryValue,
  IN  UINTN *EntryValueLength
  );

/**
  This Function gets the Value (32) of the specified Config Entry.

  @param[in]  EntryName                    - The Name of the Config Entry.
  @param[out] EntryValue                   - The Value (32) of the specified Config Entry.

  @return EFI_SUCCESS                      - The Config Entry Value was Successfully Found.
  @return EFI_INVALID_PARAMETER            - One or more Arguments are NULL.
  @return EFI_NOT_FOUND                    - The Specified Config Entry does not Exist.
**/
typedef
EFI_STATUS
(*EFI_GET_CONFIG_VAL) (
  IN  CHAR8  *EntryName,
  OUT UINT32 *EntryValue
  );

/**
  This Function gets the Value (64) of the specified Config Entry.

  @param[in]  EntryName                    - The Name of the Config Entry.
  @param[out] EntryValue                   - The Value (64) of the specified Config Entry.

  @return EFI_SUCCESS                      - The Config Entry Value was Successfully Found.
  @return EFI_INVALID_PARAMETER            - One or more Arguments are NULL.
  @return EFI_NOT_FOUND                    - The Specified Config Entry does not Exist.
**/
typedef
EFI_STATUS
(*EFI_GET_CONFIG_VAL64) (
  IN  CHAR8  *EntryName,
  OUT UINT64 *EntryValue
  );

/**
  This Function gets the Details of the specified Memory Region by Name.

  @param[in]  RegionName                   - The Name of the Memory Region.
  @param[out] MemoryRegionDescriptor       - The Details of the specified Memory Region.

  @return EFI_SUCCESS                      - The Memory Region Details were Successfully Found.
  @return EFI_INVALID_PARAMETER            - One or more Arguments are NULL.
  @return EFI_NOT_FOUND                    - The Specified Memory Region does not Exist.
**/
typedef
EFI_STATUS
(*EFI_GET_MEM_INFO_BY_NAME) (
  IN  CHAR8                        *RegionName,
  OUT EFI_MEMORY_REGION_DESCRIPTOR *MemoryRegionDescriptor
  );

/**
  This Function gets the Details of the specified Memory Region by Base Address.

  @param[in]  RegionAddress                - The Base Address of the Memory Region.
  @param[out] MemoryRegionDescriptor       - The Details of the specified Memory Region.

  @return EFI_SUCCESS                      - The Memory Region Details were Successfully Found.
  @return EFI_INVALID_PARAMETER            - One or more Arguments are NULL.
  @return EFI_NOT_FOUND                    - The Specified Memory Region does not Exist.
**/
typedef
EFI_STATUS
(*EFI_GET_MEM_INFO_BY_ADDRESS) (
  IN  UINT64                        RegionAddress,
  OUT EFI_MEMORY_REGION_DESCRIPTOR *MemoryRegionDescriptor
  );

//
// UEFI Config Library
//
typedef struct {
  UINT32                      LibVersion;
  EFI_GET_MEM_INFO_BY_NAME    GetMemInfoByName;
  EFI_GET_CONFIG_STRING       GetCfgInfoString;
  EFI_GET_CONFIG_VAL          GetCfgInfoVal;
  EFI_GET_CONFIG_VAL64        GetCfgInfoVal64;
  EFI_GET_MEM_INFO_BY_ADDRESS GetMemInfoByAddress;
} EFI_UEFI_CONFIG_LIBRARY;

#endif /* _EFI_UEFI_CONFIG_H_ */
