#include <Library/GpioLib.h>

STATIC
EFI_GPIO_CONTROLLER_DATA
gGpioControllers[] = {
  // Pinctrl Address - Bank ID, Bank Number, Bank Offset
  {
    0x10430000,
    {
      {BANK_ID_G, 0, 0x0080},
      {BANK_ID_G, 1, 0x00A0},
      {BANK_ID_G, 2, 0x00C0},
      {BANK_ID_P, 0, 0x0000},
      {BANK_ID_P, 1, 0x0020},
      {BANK_ID_P, 2, 0x0040},
      {BANK_ID_P, 3, 0x0060}
    }
  },
  {
    0x10830000,
    {
      {BANK_ID_C, 0, 0x0060},
      {BANK_ID_C, 1, 0x0080},
      {BANK_ID_D, 0, 0x00A0},
      {BANK_ID_G, 3, 0x00C0},
      {BANK_ID_P, 4, 0x0000},
      {BANK_ID_P, 5, 0x0020},
      {BANK_ID_P, 6, 0x0040}
    }
  },
  {
    0x11050000,
    {
      {BANK_ID_F, 0, 0x0000}
    }
  },
  {
    0x11430000,
    {
      {BANK_ID_F, 1, 0x0000},
      {BANK_ID_F, 2, 0x0020}
    }
  },
  {
    0x13880000,
    {
      {BANK_ID_T, 0, 0x0000}
    }
  },
  {
    0x13A80000,
    {
      {BANK_ID_H, 0, 0x0000},
      {BANK_ID_H, 1, 0x0020}
    }
  },
  {
    0x14050000,
    {
      {BANK_ID_A, 0, 0x0020},
      {BANK_ID_A, 1, 0x0040},
      {BANK_ID_A, 2, 0x0060},
      {BANK_ID_A, 3, 0x0080},
      {BANK_ID_A, 4, 0x00A0},
      {BANK_ID_Q, 0, 0x00A0}
    }
  },
  {
    0x14220000,
    {
      {BANK_ID_M, 0,  0x0000},
      {BANK_ID_M, 1,  0x0020},
      {BANK_ID_M, 2,  0x0040},
      {BANK_ID_M, 3,  0x0060},
      {BANK_ID_M, 4,  0x0080},
      {BANK_ID_M, 5,  0x00A0},
      {BANK_ID_M, 6,  0x00C0},
      {BANK_ID_M, 7,  0x00E0},
      {BANK_ID_M, 10, 0x0100},
      {BANK_ID_M, 11, 0x0120},
      {BANK_ID_M, 12, 0x0140},
      {BANK_ID_M, 13, 0x0160},
      {BANK_ID_M, 14, 0x0180},
      {BANK_ID_M, 15, 0x01A0},
      {BANK_ID_M, 16, 0x01C0},
      {BANK_ID_M, 17, 0x01E0},
      {BANK_ID_M, 40, 0x0200},
      {BANK_ID_M, 41, 0x0220},
      {BANK_ID_M, 42, 0x0240},
      {BANK_ID_M, 43, 0x0260}
    }
  },
  {
    0x17C60000,
    {
      {BANK_ID_B, 0, 0x0000},
      {BANK_ID_B, 1, 0x0020},
      {BANK_ID_B, 2, 0x0040}
    }
  }
};

VOID
GetGpioControllerData (
  OUT EFI_GPIO_CONTROLLER_DATA **Data,
  OUT UINT8                     *Count)
{
  // Pass Data
  *Data  = gGpioControllers;
  *Count = ARRAY_SIZE (gGpioControllers);
}
