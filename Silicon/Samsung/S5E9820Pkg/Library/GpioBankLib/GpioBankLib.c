#include <Library/GpioBankLib.h>

#include "Pinctrl.h"

STATIC
EFI_GPIO_BANK_DATA
gGpioBanks[] = {
  // GPA Banks
  {
    // Bank ID
    GPIO_BANK_ID_A,
    {
      // Bank Number, Offset, Pinctrl Address
      {0, 0x0000, EFI_PINCTRL_ALIVE_ADDRESS},
      {1, 0x0020, EFI_PINCTRL_ALIVE_ADDRESS},
      {2, 0x0040, EFI_PINCTRL_ALIVE_ADDRESS},
      {3, 0x0060, EFI_PINCTRL_ALIVE_ADDRESS},
      {4, 0x0080, EFI_PINCTRL_ALIVE_ADDRESS}
    }
  },

  // GPB Banks
  {
    // Bank ID
    GPIO_BANK_ID_B,
    {
      // Bank Number, Offset, Pinctrl Address
      {0, 0x0000, EFI_PINCTRL_AUD_ADDRESS},
      {1, 0x0020, EFI_PINCTRL_AUD_ADDRESS},
      {2, 0x0040, EFI_PINCTRL_AUD_ADDRESS}
    }
  },

  // GPC Banks
  {
    // Bank ID
    GPIO_BANK_ID_C,
    {
      // Bank Number, Offset, Pinctrl Address
      {0, 0x0060, EFI_PINCTRL_PERIC1_ADDRESS},
      {1, 0x0080, EFI_PINCTRL_PERIC1_ADDRESS}
    }
  },

  // GPD Banks
  {
    // Bank ID
    GPIO_BANK_ID_D,
    {
      // Bank Number, Offset, Pinctrl Address
      {0, 0x00A0, EFI_PINCTRL_PERIC1_ADDRESS}
    }
  },

  // GPF Banks
  {
    // Bank ID
    GPIO_BANK_ID_F,
    {
      // Bank Number, Offset, Pinctrl Address
      {0, 0x0000, EFI_PINCTRL_FSYS0_ADDRESS},
      {1, 0x0000, EFI_PINCTRL_FSYS1_ADDRESS},
      {2, 0x0020, EFI_PINCTRL_FSYS1_ADDRESS},
      {3, 0x0040, EFI_PINCTRL_FSYS1_ADDRESS}
    }
  },

  // GPG Banks
  {
    // Bank ID
    GPIO_BANK_ID_G,
    {
      // Bank Number, Offset, Pinctrl Address
      {0, 0x0080, EFI_PINCTRL_PERIC0_ADDRESS},
      {1, 0x00A0, EFI_PINCTRL_PERIC0_ADDRESS},
      {2, 0x00C0, EFI_PINCTRL_PERIC0_ADDRESS},
      {3, 0x00C0, EFI_PINCTRL_PERIC1_ADDRESS},
      {4, 0x00E0, EFI_PINCTRL_PERIC0_ADDRESS}
    }
  },

  // GPH Banks
  {
    // Bank ID
    GPIO_BANK_ID_H,
    {
      // Bank Number, Offset, Pinctrl Address
      {0, 0x00E0, EFI_PINCTRL_PERIC1_ADDRESS},
      {0, 0x0100, EFI_PINCTRL_PERIC1_ADDRESS}
    }
  },

  // GPM Banks
  {
    // Bank ID
    GPIO_BANK_ID_M,
    {
      // Bank Number, Offset, Pinctrl Address
      {0,  0x0000, EFI_PINCTRL_CMGP_ADDRESS},
      {1,  0x0020, EFI_PINCTRL_CMGP_ADDRESS},
      {2,  0x0040, EFI_PINCTRL_CMGP_ADDRESS},
      {3,  0x0060, EFI_PINCTRL_CMGP_ADDRESS},
      {4,  0x0080, EFI_PINCTRL_CMGP_ADDRESS},
      {5,  0x00A0, EFI_PINCTRL_CMGP_ADDRESS},
      {6,  0x00C0, EFI_PINCTRL_CMGP_ADDRESS},
      {7,  0x00E0, EFI_PINCTRL_CMGP_ADDRESS},
      {8,  0x0100, EFI_PINCTRL_CMGP_ADDRESS},
      {9,  0x0120, EFI_PINCTRL_CMGP_ADDRESS},
      {10, 0x0140, EFI_PINCTRL_CMGP_ADDRESS},
      {11, 0x0160, EFI_PINCTRL_CMGP_ADDRESS},
      {12, 0x0180, EFI_PINCTRL_CMGP_ADDRESS},
      {13, 0x01A0, EFI_PINCTRL_CMGP_ADDRESS},
      {14, 0x01C0, EFI_PINCTRL_CMGP_ADDRESS},
      {15, 0x01E0, EFI_PINCTRL_CMGP_ADDRESS},
      {16, 0x0200, EFI_PINCTRL_CMGP_ADDRESS},
      {17, 0x0220, EFI_PINCTRL_CMGP_ADDRESS},
      {18, 0x0240, EFI_PINCTRL_CMGP_ADDRESS},
      {19, 0x0260, EFI_PINCTRL_CMGP_ADDRESS},
      {20, 0x0280, EFI_PINCTRL_CMGP_ADDRESS},
      {21, 0x02A0, EFI_PINCTRL_CMGP_ADDRESS},
      {22, 0x02C0, EFI_PINCTRL_CMGP_ADDRESS},
      {23, 0x02E0, EFI_PINCTRL_CMGP_ADDRESS},
      {24, 0x0300, EFI_PINCTRL_CMGP_ADDRESS},
      {25, 0x0320, EFI_PINCTRL_CMGP_ADDRESS},
      {26, 0x0340, EFI_PINCTRL_CMGP_ADDRESS},
      {27, 0x0360, EFI_PINCTRL_CMGP_ADDRESS},
      {28, 0x0380, EFI_PINCTRL_CMGP_ADDRESS},
      {29, 0x03A0, EFI_PINCTRL_CMGP_ADDRESS},
      {30, 0x03C0, EFI_PINCTRL_CMGP_ADDRESS},
      {31, 0x03E0, EFI_PINCTRL_CMGP_ADDRESS}
    }
  },

  // GPP Banks
  {
    // Bank ID
    GPIO_BANK_ID_P,
    {
      // Bank Number, Offset, Pinctrl Address
      {0, 0x0000, EFI_PINCTRL_PERIC0_ADDRESS},
      {1, 0x0020, EFI_PINCTRL_PERIC0_ADDRESS},
      {2, 0x0040, EFI_PINCTRL_PERIC0_ADDRESS},
      {3, 0x0060, EFI_PINCTRL_PERIC0_ADDRESS},
      {4, 0x0000, EFI_PINCTRL_PERIC1_ADDRESS},
      {5, 0x0020, EFI_PINCTRL_PERIC1_ADDRESS},
      {6, 0x0040, EFI_PINCTRL_PERIC1_ADDRESS}
    }
  },

  // GPQ Banks
  {
    // Bank ID
    GPIO_BANK_ID_Q,
    {
      // Bank Number, Offset, Pinctrl Address
      {0, 0x00A0, EFI_PINCTRL_ALIVE_ADDRESS}
    }
  },

  // GPV Banks
  {
    // Bank ID
    GPIO_BANK_ID_V,
    {
      // Bank Number, Offset, Pinctrl Address
      {0, 0x0000, EFI_PINCTRL_VTS_ADDRESS}
    }
  }
};

EFI_GPIO_BANK_DATA*
GetPlatformBankData ()
{
  return gGpioBanks;
}
