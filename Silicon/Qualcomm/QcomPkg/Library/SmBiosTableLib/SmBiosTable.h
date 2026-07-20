#ifndef _SMBIOS_TABLE_H_
#define _SMBIOS_TABLE_H_

#include <IndustryStandard/SmBios.h>

//
// SMBIOS Memory Type
//
typedef struct {
  UINT8              RawType;
  MEMORY_DEVICE_TYPE Type;
} SMBIOS_MEMORY_TYPE;

//
// SMBIOS Manufacturer Name
//
typedef struct {
  UINT8  Id;
  CHAR8 *Name;
} SMBIOS_MANUFACTURER_NAME;

//
// Utility Functions
//
EFI_STATUS
GetClusterSpeeds (
  IN  UINT32  Cluster,
  OUT UINT16 *MaxSpeed,
  OUT UINT16 *CurrentSpeed
  );

EFI_STATUS
GetDdrDetails (
  OUT UINT8              *ManufacturerId,
  OUT MEMORY_DEVICE_TYPE *MemoryType
  );

EFI_STATUS
GetDdrSpeeds (
  OUT UINT32 *MaxSpeed,
  OUT UINT32 *CurrentSpeed
  );

CHAR8*
ConvertManufacturerId (
  IN UINT8 Id
  );

VOID
InitializeUtilities ();

#endif /* _SMBIOS_TABLE_H_ */
