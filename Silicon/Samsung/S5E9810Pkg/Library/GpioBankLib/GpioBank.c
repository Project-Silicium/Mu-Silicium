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

  //
  // VTS
  //
  {
    .CtrlNum = 4,
    .Id      = BANK_ID_T,
    .Number  = 0,
    .Offset  = 0x0
  },

  //
  // CHUB
  //
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
  {
    .CtrlNum = 6,
    .Id      = BANK_ID_Q,
    .Number  = 0,
    .Offset  = 0xA0
  },

  //
  // CMGP
  //
  {
    .CtrlNum = 7,
    .Id      = BANK_ID_M,
    .Number  = 0,
    .Offset  = 0x0
  },
  {
    .CtrlNum = 7,
    .Id      = BANK_ID_M,
    .Number  = 1,
    .Offset  = 0x0020
  },
  {
    .CtrlNum = 7,
    .Id      = BANK_ID_M,
    .Number  = 2,
    .Offset  = 0x0040
  },
  {
    .CtrlNum = 7,
    .Id      = BANK_ID_M,
    .Number  = 3,
    .Offset  = 0x0060
  },
  {
    .CtrlNum = 7,
    .Id      = BANK_ID_M,
    .Number  = 4,
    .Offset  = 0x0080
  },
  {
    .CtrlNum = 7,
    .Id      = BANK_ID_M,
    .Number  = 5,
    .Offset  = 0x00A0
  },
  {
    .CtrlNum = 7,
    .Id      = BANK_ID_M,
    .Number  = 6,
    .Offset  = 0x00C0
  },
  {
    .CtrlNum = 7,
    .Id      = BANK_ID_M,
    .Number  = 7,
    .Offset  = 0x00E0
  },
  {
    .CtrlNum = 7,
    .Id      = BANK_ID_M,
    .Number  = 10,
    .Offset  = 0x0100
  },
  {
    .CtrlNum = 7,
    .Id      = BANK_ID_M,
    .Number  = 11,
    .Offset  = 0x0120
  },
  {
    .CtrlNum = 7,
    .Id      = BANK_ID_M,
    .Number  = 12,
    .Offset  = 0x0140
  },
  {
    .CtrlNum = 7,
    .Id      = BANK_ID_M,
    .Number  = 13,
    .Offset  = 0x0160
  },
  {
    .CtrlNum = 7,
    .Id      = BANK_ID_M,
    .Number  = 14,
    .Offset  = 0x0180
  },
  {
    .CtrlNum = 7,
    .Id      = BANK_ID_M,
    .Number  = 15,
    .Offset  = 0x01A0
  },
  {
    .CtrlNum = 7,
    .Id      = BANK_ID_M,
    .Number  = 16,
    .Offset  = 0x01C0
  },
  {
    .CtrlNum = 7,
    .Id      = BANK_ID_M,
    .Number  = 17,
    .Offset  = 0x01E0
  },
  {
    .CtrlNum = 7,
    .Id      = BANK_ID_M,
    .Number  = 40,
    .Offset  = 0x0200
  },
  {
    .CtrlNum = 7,
    .Id      = BANK_ID_M,
    .Number  = 41,
    .Offset  = 0x0220
  },
  {
    .CtrlNum = 7,
    .Id      = BANK_ID_M,
    .Number  = 42,
    .Offset  = 0x0240
  },
  {
    .CtrlNum = 7,
    .Id      = BANK_ID_M,
    .Number  = 43,
    .Offset  = 0x0260
  },

  //
  // AUD
  //
  {
    .CtrlNum = 8,
    .Id      = BANK_ID_B,
    .Number  = 0,
    .Offset  = 0x0
  },
  {
    .CtrlNum = 8,
    .Id      = BANK_ID_B,
    .Number  = 1,
    .Offset  = 0x20
  },
  {
    .CtrlNum = 8,
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
