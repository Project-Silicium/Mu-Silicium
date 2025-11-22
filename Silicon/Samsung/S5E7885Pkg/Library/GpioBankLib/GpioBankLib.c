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
      {0, 0x0040, EFI_PINCTRL_ALIVE_ADDRESS},
      {1, 0x0060, EFI_PINCTRL_ALIVE_ADDRESS},
      {2, 0x0080, EFI_PINCTRL_ALIVE_ADDRESS}
    }
  },

  // GPB Banks
  {
    // Bank ID
    GPIO_BANK_ID_B,
    {
      // Bank Number, Offset, Pinctrl Address
      {0, 0x0000, EFI_PINCTRL_DISPAUD_ADDRESS},
      {1, 0x0020, EFI_PINCTRL_DISPAUD_ADDRESS},
      {2, 0x0040, EFI_PINCTRL_DISPAUD_ADDRESS}
    }
  },

  // GPC Banks
  {
    // Bank ID
    GPIO_BANK_ID_C,
    {
      // Bank Number, Offset, Pinctrl Address
      {0, 0x01C0, EFI_PINCTRL_TOP_ADDRESS},
      {1, 0x01E0, EFI_PINCTRL_TOP_ADDRESS},
      {2, 0x0200, EFI_PINCTRL_TOP_ADDRESS}
    }
  },

  // GPF Banks
  {
    // Bank ID
    GPIO_BANK_ID_F,
    {
      // Bank Number, Offset, Pinctrl Address
      {0, 0x0000, EFI_PINCTRL_FSYS_ADDRESS},
      {2, 0x0020, EFI_PINCTRL_FSYS_ADDRESS},
      {3, 0x0040, EFI_PINCTRL_FSYS_ADDRESS},
      {4, 0x0060, EFI_PINCTRL_FSYS_ADDRESS}
    }
  },

  // GPG Banks
  {
    // Bank ID
    GPIO_BANK_ID_G,
    {
      // Bank Number, Offset, Pinctrl Address
      {0, 0x0020, EFI_PINCTRL_TOP_ADDRESS},
      {1, 0x0140, EFI_PINCTRL_TOP_ADDRESS},
      {2, 0x0160, EFI_PINCTRL_TOP_ADDRESS},
      {3, 0x0180, EFI_PINCTRL_TOP_ADDRESS},
      {4, 0x01A0, EFI_PINCTRL_TOP_ADDRESS}
    }
  },

  // GPP Banks
  {
    // Bank ID
    GPIO_BANK_ID_P,
    {
      // Bank Number, Offset, Pinctrl Address
      {0, 0x0000, EFI_PINCTRL_TOP_ADDRESS},
      {1, 0x0040, EFI_PINCTRL_TOP_ADDRESS},
      {2, 0x0060, EFI_PINCTRL_TOP_ADDRESS},
      {3, 0x0080, EFI_PINCTRL_TOP_ADDRESS},
      {4, 0x00A0, EFI_PINCTRL_TOP_ADDRESS},
      {5, 0x00C0, EFI_PINCTRL_TOP_ADDRESS},
      {6, 0x00E0, EFI_PINCTRL_TOP_ADDRESS},
      {7, 0x0100, EFI_PINCTRL_TOP_ADDRESS},
      {8, 0x0120, EFI_PINCTRL_TOP_ADDRESS}
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
  }
};

EFI_GPIO_BANK_DATA*
GetPlatformBankData ()
{
  return gGpioBanks;
}
