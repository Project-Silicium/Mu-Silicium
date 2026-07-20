#ifndef _SMBIOS_TABLE_H_
#define _SMBIOS_TABLE_H_

#include <IndustryStandard/SmBios.h>

//
// Day Numbers
//
#define DATE_DAY0 (__DATE__[4] == ' ' ? '0' : __DATE__[4])
#define DATE_DAY1 (__DATE__[5])

//
// 1st Month Number
//
#define DATE_MONTH0                                                               \
  ((__DATE__[0] == 'J' && __DATE__[1] == 'a')                       ? '0' :       \
   (__DATE__[0] == 'F')                                             ? '0' :       \
   (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'r') ? '0' :       \
   (__DATE__[0] == 'A' && __DATE__[1] == 'p')                       ? '0' :       \
   (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'y') ? '0' :       \
   (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'n') ? '0' :       \
   (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'l') ? '0' :       \
   (__DATE__[0] == 'A' && __DATE__[1] == 'u')                       ? '0' :       \
   (__DATE__[0] == 'S')                                             ? '0' :       \
   (__DATE__[0] == 'O')                                             ? '1' :       \
   (__DATE__[0] == 'N')                                             ? '1' :       \
   (__DATE__[0] == 'D')                                             ? '1' : '0')

//
// 2nd Month Number
//
#define DATE_MONTH1                                                               \
  ((__DATE__[0] == 'J' && __DATE__[1] == 'a')                       ? '1' :       \
   (__DATE__[0] == 'F')                                             ? '2' :       \
   (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'r') ? '3' :       \
   (__DATE__[0] == 'A' && __DATE__[1] == 'p')                       ? '4' :       \
   (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'y') ? '5' :       \
   (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'n') ? '6' :       \
   (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'l') ? '7' :       \
   (__DATE__[0] == 'A' && __DATE__[1] == 'u')                       ? '8' :       \
   (__DATE__[0] == 'S')                                             ? '9' :       \
   (__DATE__[0] == 'O')                                             ? '0' :       \
   (__DATE__[0] == 'N')                                             ? '1' :       \
   (__DATE__[0] == 'D')                                             ? '2' : '0')

//
// Year Numbers
//
#define DATE_YEAR0 (__DATE__[7])
#define DATE_YEAR1 (__DATE__[8])
#define DATE_YEAR2 (__DATE__[9])
#define DATE_YEAR3 (__DATE__[10])

//
// UEFI Firmware Build Date
//
#define SMBIOS_BUILD_DATE ((CHAR16[]) {             \
  DATE_MONTH0, DATE_MONTH1, '/',                    \
  DATE_DAY0,   DATE_DAY1,   '/',                    \
  DATE_YEAR0,  DATE_YEAR1,  DATE_YEAR2, DATE_YEAR3, \
  '\0'                                              \
})

//
// SMBIOS Cache Associativity Type
//
typedef struct {
  UINT32                   Ways;
  CACHE_ASSOCIATIVITY_DATA Type;
} SMBIOS_CACHE_ASSOCIATIVITY_TYPE;

//
// Utility Functions
//
UINTN
GetMidr ();

BOOLEAN
IsArmv9 ();

VOID
GetCpuCacheDetails (
  IN  UINT8                     Level,
  IN  BOOLEAN                   InstructionCache,
  OUT UINT32                   *Size,
  OUT CACHE_ASSOCIATIVITY_DATA *Associativity
  );

#endif /* _SMBIOS_TABLE_H_ */
