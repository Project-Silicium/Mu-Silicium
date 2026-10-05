#include <Library/GpioBankLib.h>

STATIC
EFI_GPIO_BANK
mGpioBanks[] = {
  //
  // ALIVE
  //
  {
    .CtrlNum = 0,
    .Id      = BANK_ID_A,
    .Number  = 0,
    .Offset  = 0x40
  },
  {
    .CtrlNum = 0,
    .Id      = BANK_ID_A,
    .Number  = 1,
    .Offset  = 0x60
  },
  {
    .CtrlNum = 0,
    .Id      = BANK_ID_A,
    .Number  = 2,
    .Offset  = 0x80
  },
  {
    .CtrlNum = 0,
    .Id      = BANK_ID_Q,
    .Number  = 0,
    .Offset  = 0xA0
  },

  //
  // FSYS
  //
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_F,
    .Number  = 0,
    .Offset  = 0x0
  },
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_F,
    .Number  = 2,
    .Offset  = 0x20
  },
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_F,
    .Number  = 3,
    .Offset  = 0x40
  },
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_F,
    .Number  = 4,
    .Offset  = 0x60
  },

  //
  // TOP
  //
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_C,
    .Number  = 0,
    .Offset  = 0x1C0
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_C,
    .Number  = 1,
    .Offset  = 0x1E0
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_C,
    .Number  = 2,
    .Offset  = 0x200
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_G,
    .Number  = 0,
    .Offset  = 0x20
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_G,
    .Number  = 1,
    .Offset  = 0x140
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_G,
    .Number  = 2,
    .Offset  = 0x160
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_G,
    .Number  = 3,
    .Offset  = 0x180
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_G,
    .Number  = 4,
    .Offset  = 0x1A0
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_P,
    .Number  = 0,
    .Offset  = 0x0
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_P,
    .Number  = 1,
    .Offset  = 0x40
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_P,
    .Number  = 2,
    .Offset  = 0x60
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_P,
    .Number  = 3,
    .Offset  = 0x80
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_P,
    .Number  = 4,
    .Offset  = 0xA0
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_P,
    .Number  = 5,
    .Offset  = 0xC0
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_P,
    .Number  = 6,
    .Offset  = 0xE0
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_P,
    .Number  = 7,
    .Offset  = 0x100
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_P,
    .Number  = 8,
    .Offset  = 0x120
  },

  //
  // DISPAUD
  //
  {
    .CtrlNum = 3,
    .Id      = BANK_ID_B,
    .Number  = 0,
    .Offset  = 0x0
  },
  {
    .CtrlNum = 3,
    .Id      = BANK_ID_B,
    .Number  = 1,
    .Offset  = 0x20
  },
  {
    .CtrlNum = 3,
    .Id      = BANK_ID_B,
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
