#ifndef _GPIO_IMPL_LIB_H_
#define _GPIO_IMPL_LIB_H_

typedef struct {
  UINT32 SetOffset;
  UINT32 ResetOffset;
  UINT32 DirOffset;
  UINT32 DataOutOffset;
  UINT32 DataInOffset;
  UINT32 ModeOffset;
  UINT32 MaxPin;
} MTK_GPIO_PLATFORM_INFO;

extern MTK_GPIO_PLATFORM_INFO gPlatformInfo;

#endif /* _GPIO_IMPL_LIB_H_ */