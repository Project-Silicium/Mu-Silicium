#include <Library/GpioLib.h>

STATIC
EFI_GPIO_CONTROLLER_DATA
gGpioControllers[] = {
  // Pinctrl Address - Bank ID, Bank Number, Bank Offset
  {
    .Address    = 0x10830000,
    {
      {
        .Id     = BANK_ID_C,
        .Number = 0,
        .Offset = 0x20
      },
      {
        .Id     = BANK_ID_C,
        .Number = 1,
        .Offset = 0x40
      },
      {
        .Id     = BANK_ID_G,
        .Number = 0,
        .Offset = 0x60
      },
      {
        .Id     = BANK_ID_G,
        .Number = 1,
        .Offset = 0x80
      },
      {
        .Id     = BANK_ID_P,
        .Number = 4,
        .Offset = 0x0
      }
    }
  },
  {
    .Address    = 0x11030000,
    {
      {
        .Id     = BANK_ID_C,
        .Number = 2,
        .Offset = 0x20
      },
      {
        .Id     = BANK_ID_C,
        .Number = 4,
        .Offset = 0x40
      },
      {
        .Id     = BANK_ID_C,
        .Number = 5,
        .Offset = 0x60
      },
      {
        .Id     = BANK_ID_C,
        .Number = 10,
        .Offset = 0x100
      },
      {
        .Id     = BANK_ID_G,
        .Number = 2,
        .Offset = 0x80
      },
      {
        .Id     = BANK_ID_P,
        .Number = 7,
        .Offset = 0xA0
      },
      {
        .Id     = BANK_ID_P,
        .Number = 8,
        .Offset = 0xC0
      },
      {
        .Id     = BANK_ID_P,
        .Number = 9,
        .Offset = 0xE0
      },
      {
        .Id     = BANK_ID_P,
        .Number = 10,
        .Offset = 0x0
      }
    }
  },
  {
    .Address    = 0x11830000,
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
      },
      {
        .Id     = BANK_ID_B,
        .Number = 3,
        .Offset = 0x60
      },
      {
        .Id     = BANK_ID_C,
        .Number = 3,
        .Offset = 0x160
      },
      {
        .Id     = BANK_ID_C,
        .Number = 6,
        .Offset = 0x180
      },
      {
        .Id     = BANK_ID_C,
        .Number = 7,
        .Offset = 0x1A0
      },
      {
        .Id     = BANK_ID_C,
        .Number = 8,
        .Offset = 0x1C0
      },
      {
        .Id     = BANK_ID_C,
        .Number = 9,
        .Offset = 0x1E0
      },
      {
        .Id     = BANK_ID_G,
        .Number = 3,
        .Offset = 0x200
      },
      {
        .Id     = BANK_ID_P,
        .Number = 0,
        .Offset = 0x80
      },
      {
        .Id     = BANK_ID_P,
        .Number = 1,
        .Offset = 0xA0
      },
      {
        .Id     = BANK_ID_P,
        .Number = 2,
        .Offset = 0xC0
      },
      {
        .Id     = BANK_ID_P,
        .Number = 3,
        .Offset = 0xE0
      },
      {
        .Id     = BANK_ID_P,
        .Number = 5,
        .Offset = 0x100
      },
      {
        .Id     = BANK_ID_P,
        .Number = 6,
        .Offset = 0x120
      },
      {
        .Id     = BANK_ID_P,
        .Number = 11,
        .Offset = 0x140
      }
    }
  },
  {
    .Address    = 0x12850000,
    {
      {
        .Id     = BANK_ID_A,
        .Number = 0,
        .Offset = 0x0
      },
      {
        .Id     = BANK_ID_A,
        .Number = 1,
        .Offset = 0x20
      },
      {
        .Id     = BANK_ID_A,
        .Number = 2,
        .Offset = 0x40
      },
      {
        .Id     = BANK_ID_A,
        .Number = 3,
        .Offset = 0x60
      },
      {
        .Id     = BANK_ID_A,
        .Number = 4,
        .Offset = 0x80
      },
      {
        .Id     = BANK_ID_Q,
        .Number = 1,
        .Offset = 0xA0
      },
      {
        .Id     = BANK_ID_Q,
        .Number = 2,
        .Offset = 0xC0
      },
      {
        .Id     = BANK_ID_Q,
        .Number = 3,
        .Offset = 0xE0
      }
    }
  },
  {
    .Address    = 0x13020000,
    {
      {
        .Id     = BANK_ID_V,
        .Number = 0,
        .Offset = 0x0
      }
    }
  },
  {
    .Address    = 0x13EA0000,
    {
      {
        .Id     = BANK_ID_B,
        .Number = 5,
        .Offset = 0xC0
      },
      {
        .Id     = BANK_ID_H,
        .Number = 0,
        .Offset = 0x0
      },
      {
        .Id     = BANK_ID_H,
        .Number = 1,
        .Offset = 0x20
      },
      {
        .Id     = BANK_ID_H,
        .Number = 2,
        .Offset = 0x40
      },
      {
        .Id     = BANK_ID_H,
        .Number = 3,
        .Offset = 0x60
      },
      {
        .Id     = BANK_ID_H,
        .Number = 6,
        .Offset = 0x80
      },
      {
        .Id     = BANK_ID_H,
        .Number = 7,
        .Offset = 0xA0
      }
    }
  },
  {
    .Address    = 0x14030000,
    {
      {
        .Id     = BANK_ID_M,
        .Number = 0,
        .Offset = 0x0
      },
      {
        .Id     = BANK_ID_M,
        .Number = 1,
        .Offset = 0x20
      },
      {
        .Id     = BANK_ID_M,
        .Number = 2,
        .Offset = 0x40
      },
      {
        .Id     = BANK_ID_M,
        .Number = 3,
        .Offset = 0x60
      },
      {
        .Id     = BANK_ID_M,
        .Number = 4,
        .Offset = 0x80
      },
      {
        .Id     = BANK_ID_M,
        .Number = 5,
        .Offset = 0xA0
      },
      {
        .Id     = BANK_ID_M,
        .Number = 6,
        .Offset = 0xC0
      },
      {
        .Id     = BANK_ID_M,
        .Number = 7,
        .Offset = 0xE0
      },
      {
        .Id     = BANK_ID_M,
        .Number = 8,
        .Offset = 0x100
      },
      {
        .Id     = BANK_ID_M,
        .Number = 9,
        .Offset = 0x120
      },
      {
        .Id     = BANK_ID_M,
        .Number = 10,
        .Offset = 0x140
      },
      {
        .Id     = BANK_ID_M,
        .Number = 11,
        .Offset = 0x160
      },
      {
        .Id     = BANK_ID_M,
        .Number = 12,
        .Offset = 0x180
      },
      {
        .Id     = BANK_ID_M,
        .Number = 13,
        .Offset = 0x1A0
      },
      {
        .Id     = BANK_ID_M,
        .Number = 14,
        .Offset = 0x260
      },
      {
        .Id     = BANK_ID_M,
        .Number = 15,
        .Offset = 0x1C0
      },
      {
        .Id     = BANK_ID_M,
        .Number = 16,
        .Offset = 0x1E0
      },
      {
        .Id     = BANK_ID_M,
        .Number = 17,
        .Offset = 0x200
      },
      {
        .Id     = BANK_ID_M,
        .Number = 18,
        .Offset = 0x220
      },
      {
        .Id     = BANK_ID_M,
        .Number = 20,
        .Offset = 0x280
      },
      {
        .Id     = BANK_ID_M,
        .Number = 21,
        .Offset = 0x2A0
      },
      {
        .Id     = BANK_ID_M,
        .Number = 22,
        .Offset = 0x2C0
      },
      {
        .Id     = BANK_ID_Q,
        .Number = 0,
        .Offset = 0x240
      }
    }
  },
  {
    .Address    = 0x17040000,
    {
      {
        .Id     = BANK_ID_F,
        .Number = 1,
        .Offset = 0x0
      }
    }
  },
  {
    .Address    = 0x17060000,
    {
      {
        .Id     = BANK_ID_F,
        .Number = 2,
        .Offset = 0x0
      }
    }
  },
  {
    .Address    = 0x18030000,
    {
      {
        .Id     = BANK_ID_F,
        .Number = 0,
        .Offset = 0x0
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
