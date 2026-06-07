#include <Library/GpioLib.h>

STATIC
EFI_GPIO_CONTROLLER_DATA
gGpioControllers[] = {
  // Pinctrl Address - Bank ID, Bank Number, Bank Offset
  {
    0x10530000,
    {
      {BANK_ID_M, 0, 0x0000}
    }
  },
  {
    0x13750000,
    {
      {BANK_ID_R, 0, 0x0000},
      {BANK_ID_R, 1, 0x0020},
      {BANK_ID_R, 2, 0x0040},
      {BANK_ID_R, 3, 0x0060},
      {BANK_ID_R, 4, 0x0080}
    }
  },
  {
    0x139B0000,
    {
      {BANK_ID_B, 0, 0x0000},
      {BANK_ID_C, 0, 0x0020},
      {BANK_ID_C, 1, 0x0040},
      {BANK_ID_C, 4, 0x0060},
      {BANK_ID_C, 5, 0x0080},
      {BANK_ID_C, 6, 0x00A0},
      {BANK_ID_C, 8, 0x00C0},
      {BANK_ID_C, 9, 0x00E0},
      {BANK_ID_D, 1, 0x0100},
      {BANK_ID_D, 2, 0x0120},
      {BANK_ID_D, 3, 0x0140},
      {BANK_ID_D, 4, 0x0160},
      {BANK_ID_E, 0, 0x01A0},
      {BANK_ID_F, 0, 0x01C0},
      {BANK_ID_F, 1, 0x01E0},
      {BANK_ID_F, 2, 0x0200},
      {BANK_ID_F, 3, 0x0220},
      {BANK_ID_F, 4, 0x0240}
    }
  },
  {
    0x139C0000,
    {
      {BANK_ID_C, 2, 0x0000}
    }
  },
  {
    0x139D0000,
    {
      {BANK_ID_C, 3, 0x0000}
    }
  },
  {
    0x139E0000,
    {
      {BANK_ID_C, 7, 0x0000}
    }
  },
  {
    0x139F0000,
    {
      {BANK_ID_A, 0, 0x0040},
      {BANK_ID_A, 1, 0x0060},
      {BANK_ID_A, 2, 0x0080},
      {BANK_ID_Q, 0, 0x00C0}
    }
  },
  {
    0x148C0000,
    {
      {BANK_ID_Z, 0, 0x0000},
      {BANK_ID_Z, 1, 0x0020},
      {BANK_ID_Z, 2, 0x0040}
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
