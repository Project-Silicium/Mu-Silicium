#include <Library/GpioBankLib.h>

STATIC
EFI_GPIO_BANK
mGpioBanks[] = {
  //
  // PERIC0
  //
  {
    .CtrlNum = 0,
    .Id      = BANK_ID_G,
    .Number  = 0,
    .Offset  = 0x80
  },
  {
    .CtrlNum = 0,
    .Id      = BANK_ID_G,
    .Number  = 1,
    .Offset  = 0xA0
  },
  {
    .CtrlNum = 0,
    .Id      = BANK_ID_G,
    .Number  = 2,
    .Offset  = 0xC0
  },
  {
    .CtrlNum = 0,
    .Id      = BANK_ID_G,
    .Number  = 4,
    .Offset  = 0xE0
  },
  {
    .CtrlNum = 0,
    .Id      = BANK_ID_P,
    .Number  = 0,
    .Offset  = 0x0
  },
  {
    .CtrlNum = 0,
    .Id      = BANK_ID_P,
    .Number  = 1,
    .Offset  = 0x20
  },
  {
    .CtrlNum = 0,
    .Id      = BANK_ID_P,
    .Number  = 2,
    .Offset  = 0x40
  },
  {
    .CtrlNum = 0,
    .Id      = BANK_ID_P,
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
    .Offset  = 0x60
  },
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_C,
    .Number  = 1,
    .Offset  = 0x80
  },
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_D,
    .Number  = 0,
    .Offset  = 0xA0
  },
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_G,
    .Number  = 3,
    .Offset  = 0xC0
  },
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_H,
    .Number  = 0,
    .Offset  = 0xE0
  },
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_H,
    .Number  = 1,
    .Offset  = 0x100
  },
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_P,
    .Number  = 4,
    .Offset  = 0x0
  },
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_P,
    .Number  = 5,
    .Offset  = 0x20
  },
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_P,
    .Number  = 6,
    .Offset  = 0x40
  },

  //
  // FSYS0
  //
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_F,
    .Number  = 0,
    .Offset  = 0x0
  },

  //
  // FSYS1
  //
  {
    .CtrlNum = 3,
    .Id      = BANK_ID_F,
    .Number  = 1,
    .Offset  = 0x0
  },
  {
    .CtrlNum = 3,
    .Id      = BANK_ID_F,
    .Number  = 2,
    .Offset  = 0x20
  },
  {
    .CtrlNum = 3,
    .Id      = BANK_ID_F,
    .Number  = 3,
    .Offset  = 0x40
  },

  //
  // VTS
  //
  {
    .CtrlNum = 4,
    .Id      = BANK_ID_V,
    .Number  = 0,
    .Offset  = 0x0
  },

  //
  // ALIVE
  //
  {
    .CtrlNum = 5,
    .Id      = BANK_ID_A,
    .Number  = 0,
    .Offset  = 0x0
  },
  {
    .CtrlNum = 5,
    .Id      = BANK_ID_A,
    .Number  = 1,
    .Offset  = 0x20
  },
  {
    .CtrlNum = 5,
    .Id      = BANK_ID_A,
    .Number  = 2,
    .Offset  = 0x40
  },
  {
    .CtrlNum = 5,
    .Id      = BANK_ID_A,
    .Number  = 3,
    .Offset  = 0x60
  },
  {
    .CtrlNum = 5,
    .Id      = BANK_ID_A,
    .Number  = 4,
    .Offset  = 0x80
  },
  {
    .CtrlNum = 5,
    .Id      = BANK_ID_Q,
    .Number  = 0,
    .Offset  = 0xA0
  },

  //
  // CMGP
  //
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_M,
    .Number  = 0,
    .Offset  = 0x0
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_M,
    .Number  = 1,
    .Offset  = 0x20
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_M,
    .Number  = 2,
    .Offset  = 0x40
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_M,
    .Number  = 3,
    .Offset  = 0x60
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_M,
    .Number  = 4,
    .Offset  = 0x80
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_M,
    .Number  = 5,
    .Offset  = 0xA0
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_M,
    .Number  = 6,
    .Offset  = 0xC0
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_M,
    .Number  = 7,
    .Offset  = 0xE0
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_M,
    .Number  = 8,
    .Offset  = 0x100
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_M,
    .Number  = 9,
    .Offset  = 0x120
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_M,
    .Number  = 10,
    .Offset  = 0x140
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_M,
    .Number  = 11,
    .Offset  = 0x160
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_M,
    .Number  = 12,
    .Offset  = 0x180
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_M,
    .Number  = 13,
    .Offset  = 0x1A0
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_M,
    .Number  = 14,
    .Offset  = 0x1C0
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_M,
    .Number  = 15,
    .Offset  = 0x1E0
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_M,
    .Number  = 16,
    .Offset  = 0x200
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_M,
    .Number  = 17,
    .Offset  = 0x220
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_M,
    .Number  = 18,
    .Offset  = 0x240
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_M,
    .Number  = 19,
    .Offset  = 0x260
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_M,
    .Number  = 20,
    .Offset  = 0x280
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_M,
    .Number  = 21,
    .Offset  = 0x2A0
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_M,
    .Number  = 22,
    .Offset  = 0x2C0
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_M,
    .Number  = 23,
    .Offset  = 0x2E0
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_M,
    .Number  = 24,
    .Offset  = 0x300
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_M,
    .Number  = 25,
    .Offset  = 0x320
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_M,
    .Number  = 26,
    .Offset  = 0x340
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_M,
    .Number  = 27,
    .Offset  = 0x360
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_M,
    .Number  = 28,
    .Offset  = 0x380
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_M,
    .Number  = 29,
    .Offset  = 0x3A0
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_M,
    .Number  = 30,
    .Offset  = 0x3C0
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_M,
    .Number  = 31,
    .Offset  = 0x3E0
  },

  //
  // AUD
  //
  {
    .CtrlNum = 7,
    .Id      = BANK_ID_B,
    .Number  = 0,
    .Offset  = 0x0
  },
  {
    .CtrlNum = 7,
    .Id      = BANK_ID_B,
    .Number  = 1,
    .Offset  = 0x20
  },
  {
    .CtrlNum = 7,
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
