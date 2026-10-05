#include <Library/GpioBankLib.h>

STATIC
EFI_GPIO_BANK
mGpioBanks[] = {
  //
  // PERIC0
  //
  {
    .CtrlNum = 0,
    .Id      = BANK_ID_C,
    .Number  = 0,
    .Offset  = 0x20
  },
  {
    .CtrlNum = 0,
    .Id      = BANK_ID_C,
    .Number  = 1,
    .Offset  = 0x40
  },
  {
    .CtrlNum = 0,
    .Id      = BANK_ID_G,
    .Number  = 0,
    .Offset  = 0x60
  },
  {
    .CtrlNum = 0,
    .Id      = BANK_ID_G,
    .Number  = 1,
    .Offset  = 0x80
  },
  {
    .CtrlNum = 0,
    .Id      = BANK_ID_P,
    .Number  = 4,
    .Offset  = 0x0
  },

  //
  // PERIC1
  //
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_C,
    .Number  = 2,
    .Offset  = 0x20
  },
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_C,
    .Number  = 4,
    .Offset  = 0x40
  },
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_C,
    .Number  = 5,
    .Offset  = 0x60
  },
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_C,
    .Number  = 10,
    .Offset  = 0x100
  },
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_G,
    .Number  = 2,
    .Offset  = 0x80
  },
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_P,
    .Number  = 7,
    .Offset  = 0xA0
  },
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_P,
    .Number  = 8,
    .Offset  = 0xC0
  },
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_P,
    .Number  = 9,
    .Offset  = 0xE0
  },
  {
    .CtrlNum = 1,
    .Id      = BANK_ID_P,
    .Number  = 10,
    .Offset  = 0x0
  },

  //
  // PERIC2
  //
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_B,
    .Number  = 0,
    .Offset  = 0x0
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_B,
    .Number  = 1,
    .Offset  = 0x20
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_B,
    .Number  = 2,
    .Offset  = 0x40
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_B,
    .Number  = 3,
    .Offset  = 0x60
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_C,
    .Number  = 3,
    .Offset  = 0x160
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_C,
    .Number  = 6,
    .Offset  = 0x180
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_C,
    .Number  = 7,
    .Offset  = 0x1A0
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_C,
    .Number  = 8,
    .Offset  = 0x1C0
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_C,
    .Number  = 9,
    .Offset  = 0x1E0
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_G,
    .Number  = 3,
    .Offset  = 0x200
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_P,
    .Number  = 0,
    .Offset  = 0x80
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_P,
    .Number  = 1,
    .Offset  = 0xA0
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_P,
    .Number  = 2,
    .Offset  = 0xC0
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_P,
    .Number  = 3,
    .Offset  = 0xE0
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_P,
    .Number  = 5,
    .Offset  = 0x100
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_P,
    .Number  = 6,
    .Offset  = 0x120
  },
  {
    .CtrlNum = 2,
    .Id      = BANK_ID_P,
    .Number  = 11,
    .Offset  = 0x140
  },

  //
  // ALIVE
  //
  {
    .CtrlNum = 3,
    .Id      = BANK_ID_A,
    .Number  = 0,
    .Offset  = 0x0
  },
  {
    .CtrlNum = 3,
    .Id      = BANK_ID_A,
    .Number  = 1,
    .Offset  = 0x20
  },
  {
    .CtrlNum = 3,
    .Id      = BANK_ID_A,
    .Number  = 2,
    .Offset  = 0x40
  },
  {
    .CtrlNum = 3,
    .Id      = BANK_ID_A,
    .Number  = 3,
    .Offset  = 0x60
  },
  {
    .CtrlNum = 3,
    .Id      = BANK_ID_A,
    .Number  = 4,
    .Offset  = 0x80
  },
  {
    .CtrlNum = 3,
    .Id      = BANK_ID_Q,
    .Number  = 1,
    .Offset  = 0xA0
  },
  {
    .CtrlNum = 3,
    .Id      = BANK_ID_Q,
    .Number  = 2,
    .Offset  = 0xC0
  },
  {
    .CtrlNum = 3,
    .Id      = BANK_ID_Q,
    .Number  = 3,
    .Offset  = 0xE0
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
  // CHUBVTS
  //
  {
    .CtrlNum = 5,
    .Id      = BANK_ID_B,
    .Number  = 5,
    .Offset  = 0xC0
  },
  {
    .CtrlNum = 5,
    .Id      = BANK_ID_H,
    .Number  = 0,
    .Offset  = 0x0
  },
  {
    .CtrlNum = 5,
    .Id      = BANK_ID_H,
    .Number  = 1,
    .Offset  = 0x20
  },
  {
    .CtrlNum = 5,
    .Id      = BANK_ID_H,
    .Number  = 2,
    .Offset  = 0x40
  },
  {
    .CtrlNum = 5,
    .Id      = BANK_ID_H,
    .Number  = 3,
    .Offset  = 0x60
  },
  {
    .CtrlNum = 5,
    .Id      = BANK_ID_H,
    .Number  = 6,
    .Offset  = 0x80
  },
  {
    .CtrlNum = 5,
    .Id      = BANK_ID_H,
    .Number  = 7,
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
    .Offset  = 0x260
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_M,
    .Number  = 15,
    .Offset  = 0x1C0
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_M,
    .Number  = 16,
    .Offset  = 0x1E0
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_M,
    .Number  = 17,
    .Offset  = 0x200
  },
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_M,
    .Number  = 18,
    .Offset  = 0x220
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
    .Id      = BANK_ID_Q,
    .Number  = 0,
    .Offset  = 0x240
  },

  //
  // UFS
  //
  {
    .CtrlNum = 7,
    .Id      = BANK_ID_F,
    .Number  = 1,
    .Offset  = 0x0
  },

  //
  // HSI1UFS
  //
  {
    .CtrlNum = 8,
    .Id      = BANK_ID_F,
    .Number  = 2,
    .Offset  = 0x0
  },

  //
  // HSI1
  //
  {
    .CtrlNum = 9,
    .Id      = BANK_ID_F,
    .Number  = 0,
    .Offset  = 0x0
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
