#ifndef _SMBIOS_TABLE_LIB_H_
#define _SMBIOS_TABLE_LIB_H_

#include <IndustryStandard/SmBios.h>

//
// Combined TYPE0
//
typedef struct {
  SMBIOS_TABLE_TYPE0  Table;
  CHAR8              *Strings[4];
} EFI_SMBIOS_TYPE0;

//
// Combined TYPE1
//
typedef struct {
  SMBIOS_TABLE_TYPE1  Table;
  CHAR8              *Strings[7];
} EFI_SMBIOS_TYPE1;

//
// Combined TYPE2
//
typedef struct {
  SMBIOS_TABLE_TYPE2  Table;
  CHAR8              *Strings[7];
} EFI_SMBIOS_TYPE2;

//
// Combined TYPE3
//
typedef struct {
  SMBIOS_TABLE_TYPE3  Table;
  CHAR8              *Strings[5];
} EFI_SMBIOS_TYPE3;

//
// Combined TYPE4
//
typedef struct {
  SMBIOS_TABLE_TYPE4  Table;
  CHAR8              *Strings[8];
} EFI_SMBIOS_TYPE4;

//
// Combined TYPE7
//
typedef struct {
  SMBIOS_TABLE_TYPE7  Table;
  CHAR8              *Strings[2];
} EFI_SMBIOS_TYPE7;

//
// Combined TYPE16
//
typedef struct {
  SMBIOS_TABLE_TYPE16  Table;
  CHAR8               *Strings[1];
} EFI_SMBIOS_TYPE16;

//
// Combined TYPE17
//
typedef struct {
  SMBIOS_TABLE_TYPE17  Table;
  CHAR8               *Strings[8];
} EFI_SMBIOS_TYPE17;

//
// Combined TYPE19
//
typedef struct {
  SMBIOS_TABLE_TYPE19  Table;
  CHAR8               *Strings[1];
} EFI_SMBIOS_TYPE19;

//
// SMBIOS Tables
//
typedef struct {
  EFI_SMBIOS_TYPE0  Type0;
  EFI_SMBIOS_TYPE1  Type1;
  EFI_SMBIOS_TYPE2  Type2;
  EFI_SMBIOS_TYPE3  Type3;
  EFI_SMBIOS_TYPE4  Type4;
  EFI_SMBIOS_TYPE7  Type7_L1I;
  EFI_SMBIOS_TYPE7  Type7_L1D;
  EFI_SMBIOS_TYPE7  Type7_L2;
  EFI_SMBIOS_TYPE7  Type7_L3;
  EFI_SMBIOS_TYPE16 Type16;
  EFI_SMBIOS_TYPE17 Type17;
  EFI_SMBIOS_TYPE19 Type19;
} EFI_SMBIOS_TABLES;

/**
  This Function Applies Platform Specific Updates to the Passed SMBIOS Tables.

  @param[in, out] SmBiosTables             - The SMBIOS Tables.
**/
VOID
PlatformUpdateSmBiosTables (
  IN OUT EFI_SMBIOS_TABLES *SmBiosTables
  );

/**
  This Function does Platform Specific Clean Up.

  @param[in, out] SmBiosTables             - The SMBIOS Tables.
**/
VOID
PlatformSmBiosCleanUp (
  IN OUT EFI_SMBIOS_TABLES *SmBiosTables
  );

#endif /* _SMBIOS_TABLE_LIB_H_ */
