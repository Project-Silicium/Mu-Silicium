#include <Library/GpioBankLib.h>

STATIC
EFI_GPIO_BANK
mGpioBanks[] = {
  //
  // MIF
  //
  {
    .CtrlNum = 0,
    .Id      = BANK_ID_M,
    .Number  = 0,
    .Offset  = 0x0
  },

  //
  // FSYS
  //
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_R,
    .Number  = 0,
    .Offset  = 0x0
  },
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_R,
    .Number  = 1,
    .Offset  = 0x20
  },
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_R,
    .Number  = 2,
    .Offset  = 0x40
  },
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_R,
    .Number  = 3,
    .Offset  = 0x60
  },
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_R,
    .Number  = 4,
    .Offset  = 0x80
  },

  //
  // TOP
  //
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_B,
    .Number  = 0,
    .Offset  = 0x0
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_C,
    .Number  = 0,
    .Offset  = 0x20
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_C,
    .Number  = 1,
    .Offset  = 0x40
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_C,
    .Number  = 4,
    .Offset  = 0x60
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_C,
    .Number  = 5,
    .Offset  = 0x80
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_C,
    .Number  = 6,
    .Offset  = 0xA0
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_C,
    .Number  = 8,
    .Offset  = 0xC0
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_C,
    .Number  = 9,
    .Offset  = 0xE0
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_D,
    .Number  = 1,
    .Offset  = 0x100
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_D,
    .Number  = 2,
    .Offset  = 0x120
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_D,
    .Number  = 3,
    .Offset  = 0x140
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_D,
    .Number  = 4,
    .Offset  = 0x160
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_E,
    .Number  = 0,
    .Offset  = 0x1A0
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_F,
    .Number  = 0,
    .Offset  = 0x1C0
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_F,
    .Number  = 1,
    .Offset  = 0x1E0
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_F,
    .Number  = 2,
    .Offset  = 0x200
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_F,
    .Number  = 3,
    .Offset  = 0x220
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_F,
    .Number  = 4,
    .Offset  = 0x240
  },

  //
  // NFC
  //
  {
    .CtrlNum = 3,
    .Id      = BANK_ID_C,
    .Number  = 2,
    .Offset  = 0x0
  },

  //
  // TOUCH
  //
  {
    .CtrlNum = 4,
    .Id      = BANK_ID_C,
    .Number  = 3,
    .Offset  = 0x0
  },

  //
  // EsE
  //
  {
    .CtrlNum = 5,
    .Id      = BANK_ID_C,
    .Number  = 7,
    .Offset  = 0x0
  },

  //
  // ALIVE
  //
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_A,
    .Number  = 0,
    .Offset  = 0x40
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_A,
    .Number  = 1,
    .Offset  = 0x60
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_A,
    .Number  = 2,
    .Offset  = 0x80
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_Q,
    .Number  = 0,
    .Offset  = 0xC0
  },

  //
  // DISPAUD
  //
  {
    .CtrlNum = 7,
    .Id      = BANK_ID_Z,
    .Number  = 0,
    .Offset  = 0x0
  },
  {
    .CtrlNum = 7,
    .Id      = BANK_ID_Z,
    .Number  = 1,
    .Offset  = 0x20
  },
  {
    .CtrlNum = 7,
    .Id      = BANK_ID_Z,
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
