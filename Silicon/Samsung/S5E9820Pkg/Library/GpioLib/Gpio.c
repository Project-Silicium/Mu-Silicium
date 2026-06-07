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
      {BANK_ID_G, 4, 0x00E0},
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
      {BANK_ID_H, 0, 0x00E0},
      {BANK_ID_H, 1, 0x0100},
      {BANK_ID_P, 4, 0x0000},
      {BANK_ID_P, 5, 0x0020},
      {BANK_ID_P, 6, 0x0040}
    }
  },
  {
    0x13030000,
    {
      {BANK_ID_F, 0, 0x0000}
    }
  },
  {
    0x13C40000,
    {
      {BANK_ID_F, 1, 0x0000},
      {BANK_ID_F, 2, 0x0020},
      {BANK_ID_F, 3, 0x0040}
    }
  },
  {
    0x15580000,
    {
      {BANK_ID_V, 0, 0x0000}
    }
  },
  {
    0x15850000,
    {
      {BANK_ID_A, 0, 0x0000},
      {BANK_ID_A, 1, 0x0020},
      {BANK_ID_A, 2, 0x0040},
      {BANK_ID_A, 3, 0x0060},
      {BANK_ID_A, 4, 0x0080},
      {BANK_ID_Q, 0, 0x00A0}
    }
  },
  {
    0x15C30000,
    {
      {BANK_ID_M, 0,  0x0000},
      {BANK_ID_M, 1,  0x0020},
      {BANK_ID_M, 2,  0x0040},
      {BANK_ID_M, 3,  0x0060},
      {BANK_ID_M, 4,  0x0080},
      {BANK_ID_M, 5,  0x00A0},
      {BANK_ID_M, 6,  0x00C0},
      {BANK_ID_M, 7,  0x00E0},
      {BANK_ID_M, 8,  0x0100},
      {BANK_ID_M, 9,  0x0120},
      {BANK_ID_M, 10, 0x0140},
      {BANK_ID_M, 11, 0x0160},
      {BANK_ID_M, 12, 0x0180},
      {BANK_ID_M, 13, 0x01A0},
      {BANK_ID_M, 14, 0x01C0},
      {BANK_ID_M, 15, 0x01E0},
      {BANK_ID_M, 16, 0x0200},
      {BANK_ID_M, 17, 0x0220},
      {BANK_ID_M, 18, 0x0240},
      {BANK_ID_M, 19, 0x0260},
      {BANK_ID_M, 20, 0x0280},
      {BANK_ID_M, 21, 0x02A0},
      {BANK_ID_M, 22, 0x02C0},
      {BANK_ID_M, 23, 0x02E0},
      {BANK_ID_M, 24, 0x0300},
      {BANK_ID_M, 25, 0x0320},
      {BANK_ID_M, 26, 0x0340},
      {BANK_ID_M, 27, 0x0360},
      {BANK_ID_M, 28, 0x0380},
      {BANK_ID_M, 29, 0x03A0},
      {BANK_ID_M, 30, 0x03C0},
      {BANK_ID_M, 31, 0x03E0}
    }
  },
  {
    0x18C60000,
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
