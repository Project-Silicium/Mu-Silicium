#include <Protocol/MtkGpio.h>

MTK_GPIO_PLATFORM_INFO PlatformInfo = {
  .SetOffset     = 0x4,
  .ResetOffset   = 0x8,
  .DirOffset     = 0x0,
  .DataOutOffset = 0x100,
  .DataInOffset  = 0x200,
  .ModeOffset    = 0x300,
  .MaxPin        = 179,
};
