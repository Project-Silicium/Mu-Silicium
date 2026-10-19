#include <Library/GpioBankLib.h>

STATIC
EFI_GPIO_BANK
mGpioBanks[] = {
  //
  // PERCI0
  //
  {
    .CtrlNum = 0,
    .Id      = BANK_ID_B,
    .Number  = 1,
    .Offset  = 0x80
  },
  {
    .CtrlNum = 0,
    .Id      = BANK_ID_E,
    .Number  = 7,
    .Offset  = 0xA0
  },
  {
    .CtrlNum = 0,
    .Id      = BANK_ID_F,
    .Number  = 1,
    .Offset  = 0xC0
  },
  {
    .CtrlNum = 0,
    .Id      = BANK_ID_D,
    .Number  = 0,
    .Offset  = 0x0
  },
  {
    .CtrlNum = 0,
    .Id      = BANK_ID_D,
    .Number  = 1,
    .Offset  = 0x20
  },
  {
    .CtrlNum = 0,
    .Id      = BANK_ID_D,
    .Number  = 2,
    .Offset  = 0x40
  },
  {
    .CtrlNum = 0,
    .Id      = BANK_ID_D,
    .Number  = 3,
    .Offset  = 0x60
  },

  //
  // PERIC1
  //
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_C,
    .Number  = 0,
    .Offset  = 0x20
  },
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_C,
    .Number  = 1,
    .Offset  = 0x40
  },
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_C,
    .Number  = 2,
    .Offset  = 0x60
  },
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_C,
    .Number  = 3,
    .Offset  = 0x80
  },
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_K,
    .Number  = 0,
    .Offset  = 0xA0
  },
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_E,
    .Number  = 5,
    .Offset  = 0xC0
  },
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_E,
    .Number  = 6,
    .Offset  = 0xE0
  },
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_E,
    .Number  = 2,
    .Offset  = 0x100
  },
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_E,
    .Number  = 3,
    .Offset  = 0x120
  },
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_E,
    .Number  = 4,
    .Offset  = 0x140
  },
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_E,
    .Number  = 1,
    .Offset  = 0x180
  },
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_F,
    .Number  = 0,
    .Offset  = 0x160
  },
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_G,
    .Number  = 0,
    .Offset  = 0x1A0
  },
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_B,
    .Number  = 0,
    .Offset  = 0x0
  },

  //
  // FSYS0
  //
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_I,
    .Number  = 0,
    .Offset  = 0x0
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_I,
    .Number  = 1,
    .Offset  = 0x20
  },

  //
  // FSYS1
  //
  {
    .CtrlNum = 3,
    .Id      = BANK_ID_J,
    .Number  = 1,
    .Offset  = 0x0
  },
  {
    .CtrlNum = 3,
    .Id      = BANK_ID_J,
    .Number  = 0,
    .Offset  = 0x20
  },

  //
  // VTS
  //
  {
    .CtrlNum = 4,
    .Id      = BANK_ID_H,
    .Number  = 2,
    .Offset  = 0x0
  },

  //
  // BUSC
  //
  {
    .CtrlNum = 5,
    .Id      = BANK_ID_B,
    .Number  = 2,
    .Offset  = 0x0
  },

  //
  // ALIVE
  //
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_A,
    .Number  = 0,
    .Offset  = 0x20
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_A,
    .Number  = 1,
    .Offset  = 0x40
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_A,
    .Number  = 2,
    .Offset  = 0x60
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_A,
    .Number  = 3,
    .Offset  = 0x80
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_A,
    .Number  = 4,
    .Offset  = 0xA0
  },

  //
  // ABOX
  //
  {
    .CtrlNum = 7,
    .Id      = BANK_ID_H,
    .Number  = 0,
    .Offset  = 0x0
  },
  {
    .CtrlNum = 7,
    .Id      = BANK_ID_H,
    .Number  = 1,
    .Offset  = 0x20
  },
  {
    .CtrlNum = 7,
    .Id      = BANK_ID_H,
    .Number  = 2,
    .Offset  = 0x40
  }
};

VOID
GetGpioBanks (
  OUT EFI_GPIO_BANK **Bank,
  OUT UINT8          *Count)
{
  // Pass Data
  *Bank  = mGpioBanks;
  *Count = ARRAY_SIZE (mGpioBanks);
}
