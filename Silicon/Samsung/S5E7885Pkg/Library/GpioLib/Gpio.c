#include <Library/GpioLib.h>

STATIC
EFI_GPIO_CONTROLLER_DATA
gGpioControllers[] = {
  // Pinctrl Address - Bank ID, Bank Number, Bank Offset
  {
    0x11CB0000,
    {
      {BANK_ID_A, 0, 0x0040},
      {BANK_ID_A, 1, 0x0060},
      {BANK_ID_A, 2, 0x0080},
      {BANK_ID_Q, 0, 0x00A0}
    }
  },
  {
    0x13430000,
    {
      {BANK_ID_F, 0, 0x0000},
      {BANK_ID_F, 2, 0x0020},
      {BANK_ID_F, 3, 0x0040},
      {BANK_ID_F, 4, 0x0060}
    }
  },
  {
    0x139B0000,
    {
      {BANK_ID_C, 0, 0x01C0},
      {BANK_ID_C, 1, 0x01E0},
      {BANK_ID_C, 2, 0x0200},
      {BANK_ID_G, 0, 0x0020},
      {BANK_ID_G, 1, 0x0140},
      {BANK_ID_G, 2, 0x0160},
      {BANK_ID_G, 3, 0x0180},
      {BANK_ID_G, 4, 0x01A0},
      {BANK_ID_P, 0, 0x0000},
      {BANK_ID_P, 1, 0x0040},
      {BANK_ID_P, 2, 0x0060},
      {BANK_ID_P, 3, 0x0080},
      {BANK_ID_P, 4, 0x00A0},
      {BANK_ID_P, 5, 0x00C0},
      {BANK_ID_P, 6, 0x00E0},
      {BANK_ID_P, 7, 0x0100},
      {BANK_ID_P, 8, 0x0120}
    }
  },
  {
    0x148F0000,
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
