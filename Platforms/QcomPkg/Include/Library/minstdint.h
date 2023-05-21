#ifndef __MIN_STDINT_H__
#define __MIN_STDINT_H__

#include <Library/BaseLib.h>

typedef UINTN size_t;

typedef INT8  int8_t;
typedef INT16 int16_t;
typedef INT32 int32_t;
typedef INT64 int64_t;

typedef UINT8  uint8_t;
typedef UINT16 uint16_t;
typedef UINT32 uint32_t;
typedef UINT64 uint64_t;

typedef UINTN addr_t;
typedef UINTN paddr_t;

typedef UINT8  u8;
typedef UINT16 u16;
typedef UINT32 u32;
typedef UINT64 u64;

#ifndef INT16_MIN
#define INT16_MIN MIN_INT16
#endif

#ifndef INT16_MAX
#define INT16_MAX MAX_INT16
#endif

#ifndef UINT16_MIN
#define UINT16_MIN MIN_UINT16
#endif

#ifndef UINT16_MAX
#define UINT16_MAX MAX_UINT16
#endif

#ifndef INT32_MIN
#define INT32_MIN MIN_INT32
#endif

#ifndef INT32_MAX
#define INT32_MAX MAX_INT32
#endif

#ifndef UINT32_MIN
#define UINT32_MIN MIN_UINT32
#endif

#ifndef UINT32_MAX
#define UINT32_MAX MAX_UINT32
#endif

#ifndef UINT_MAX
#define UINT_MAX MAX_UINTN
#endif

#ifndef ULONG_MAX
#define ULONG_MAX (~0UL)
#endif

#ifndef LONG_MAX
#define LONG_MAX ((long)(~0UL >> 1))
#endif

#endif
