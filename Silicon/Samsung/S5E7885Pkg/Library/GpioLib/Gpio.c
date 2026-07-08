#include <Library/GpioLib.h>

STATIC
EFI_GPIO_CONTROLLER_DATA
gGpioControllers[] = {
  // Pinctrl Address - Bank ID, Bank Number, Bank Offset
  {
    .Address    = 0x11CB0000,
    {
      {
        .Id     = BANK_ID_A,
        .Number = 0,
        .Offset = 0x40
      },
      {
        .Id     = BANK_ID_A,
        .Number = 1,
        .Offset = 0x60
      },
      {
        .Id     = BANK_ID_A,
        .Number = 2,
        .Offset = 0x80
      },
      {
        .Id     = BANK_ID_Q,
        .Number = 0,
        .Offset = 0xA0
      }
    }
  },
  {
    .Address    = 0x13430000,
    {
      {
        .Id     = BANK_ID_F,
        .Number = 0,
        .Offset = 0x0
      },
      {
        .Id     = BANK_ID_F,
        .Number = 2,
        .Offset = 0x20
      },
      {
        .Id     = BANK_ID_F,
        .Number = 3,
        .Offset = 0x40
      },
      {
        .Id     = BANK_ID_F,
        .Number = 4,
        .Offset = 0x60
      }
    }
  },
  {
    .Address    = 0x139B0000,
    {
      {
        .Id     = BANK_ID_C,
        .Number = 0,
        .Offset = 0x1C0
      },
      {
        .Id     = BANK_ID_C,
        .Number = 1,
        .Offset = 0x1E0
      },
      {
        .Id     = BANK_ID_C,
        .Number = 2,
        .Offset = 0x200
      },
      {
        .Id     = BANK_ID_G,
        .Number = 0,
        .Offset = 0x20
      },
      {
        .Id     = BANK_ID_G,
        .Number = 1,
        .Offset = 0x140
      },
      {
        .Id     = BANK_ID_G,
        .Number = 2,
        .Offset = 0x160
      },
      {
        .Id     = BANK_ID_G,
        .Number = 3,
        .Offset = 0x180
      },
      {
        .Id     = BANK_ID_G,
        .Number = 4,
        .Offset = 0x1A0
      },
      {
        .Id     = BANK_ID_P,
        .Number = 0,
        .Offset = 0x0
      },
      {
        .Id     = BANK_ID_P,
        .Number = 1,
        .Offset = 0x40
      },
      {
        .Id     = BANK_ID_P,
        .Number = 2,
        .Offset = 0x60
      },
      {
        .Id     = BANK_ID_P,
        .Number = 3,
        .Offset = 0x80
      },
      {
        .Id     = BANK_ID_P,
        .Number = 4,
        .Offset = 0xA0
      },
      {
        .Id     = BANK_ID_P,
        .Number = 5,
        .Offset = 0xC0
      },
      {
        .Id     = BANK_ID_P,
        .Number = 6,
        .Offset = 0xE0
      },
      {
        .Id     = BANK_ID_P,
        .Number = 7,
        .Offset = 0x100
      },
      {
        .Id     = BANK_ID_P,
        .Number = 8,
        .Offset = 0x120
      }
    }
  },
  {
    .Address    = 0x148F0000,
    {
      {
        .Id     = BANK_ID_B,
        .Number = 0,
        .Offset = 0x0
      },
      {
        .Id     = BANK_ID_B,
        .Number = 1,
        .Offset = 0x20
      },
      {
        .Id     = BANK_ID_B,
        .Number = 2,
        .Offset = 0x40
      }
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
