#include <Library/GpioLib.h>

STATIC
EFI_GPIO_CONTROLLER_DATA
gGpioControllers[] = {
  // Pinctrl Address - Bank ID, Bank Number, Bank Offset
  {
    .Address    = 0x10430000,
    {
      {
        .Id     = BANK_ID_G,
        .Number = 0,
        .Offset = 0x80
      },
      {
        .Id     = BANK_ID_G,
        .Number = 1,
        .Offset = 0xA0
      },
      {
        .Id     = BANK_ID_G,
        .Number = 2,
        .Offset = 0xC0
      },
      {
        .Id     = BANK_ID_P,
        .Number = 0,
        .Offset = 0x0
      },
      {
        .Id     = BANK_ID_P,
        .Number = 1,
        .Offset = 0x20
      },
      {
        .Id     = BANK_ID_P,
        .Number = 2,
        .Offset = 0x40
      },
      {
        .Id     = BANK_ID_P,
        .Number = 3,
        .Offset = 0x60
      }
    }
  },
  {
    .Address    = 0x10830000,
    {
      {
        .Id     = BANK_ID_C,
        .Number = 0,
        .Offset = 0x60
      },
      {
        .Id     = BANK_ID_C,
        .Number = 1,
        .Offset = 0x80
      },
      {
        .Id     = BANK_ID_D,
        .Number = 0,
        .Offset = 0xA0
      },
      {
        .Id     = BANK_ID_G,
        .Number = 3,
        .Offset = 0xC0
      },
      {
        .Id     = BANK_ID_P,
        .Number = 4,
        .Offset = 0x0
      },
      {
        .Id     = BANK_ID_P,
        .Number = 5,
        .Offset = 0x20
      },
      {
        .Id     = BANK_ID_P,
        .Number = 6,
        .Offset = 0x40
      }
    }
  },
  {
    .Address    = 0x11050000,
    {
      {
        .Id     = BANK_ID_F,
        .Number = 0,
        .Offset = 0x0
      }
    }
  },
  {
    .Address    = 0x11430000,
    {
      {
        .Id     = BANK_ID_F,
        .Number = 1,
        .Offset = 0x0
      },
      {
        .Id     = BANK_ID_F,
        .Number = 2,
        .Offset = 0x20
      }
    }
  },
  {
    .Address    = 0x13880000,
    {
      {
        .Id     = BANK_ID_T,
        .Number = 0,
        .Offset = 0x0
      }
    }
  },
  {
    .Address    = 0x13A80000,
    {
      {
        .Id     = BANK_ID_H,
        .Number = 0,
        .Offset = 0x0
      },
      {
        .Id     = BANK_ID_H,
        .Number = 1,
        .Offset = 0x20
      }
    }
  },
  {
    .Address    = 0x14050000,
    {
      {
        .Id     = BANK_ID_A,
        .Number = 0,
        .Offset = 0x20
      },
      {
        .Id     = BANK_ID_A,
        .Number = 1,
        .Offset = 0x40
      },
      {
        .Id     = BANK_ID_A,
        .Number = 2,
        .Offset = 0x60
      },
      {
        .Id     = BANK_ID_A,
        .Number = 3,
        .Offset = 0x80
      },
      {
        .Id     = BANK_ID_A,
        .Number = 4,
        .Offset = 0xA0
      },
      {
        .Id     = BANK_ID_Q,
        .Number = 0,
        .Offset = 0xA0
      }
    }
  },
  {
    .Address    = 0x14220000,
    {
      {
        .Id     = BANK_ID_M,
        .Number = 0,
        .Offset = 0x0
      },
      {
        .Id     = BANK_ID_M,
        .Number = 1,
        .Offset = 0x0020
      },
      {
        .Id     = BANK_ID_M,
        .Number = 2,
        .Offset = 0x0040
      },
      {
        .Id     = BANK_ID_M,
        .Number = 3,
        .Offset = 0x0060
      },
      {
        .Id     = BANK_ID_M,
        .Number = 4,
        .Offset = 0x0080
      },
      {
        .Id     = BANK_ID_M,
        .Number = 5,
        .Offset = 0x00A0
      },
      {
        .Id     = BANK_ID_M,
        .Number = 6,
        .Offset = 0x00C0
      },
      {
        .Id     = BANK_ID_M,
        .Number = 7,
        .Offset = 0x00E0
      },
      {
        .Id     = BANK_ID_M,
        .Number = 10,
        .Offset = 0x0100
      },
      {
        .Id     = BANK_ID_M,
        .Number = 11,
        .Offset = 0x0120
      },
      {
        .Id     = BANK_ID_M,
        .Number = 12,
        .Offset = 0x0140
      },
      {
        .Id     = BANK_ID_M,
        .Number = 13,
        .Offset = 0x0160
      },
      {
        .Id     = BANK_ID_M,
        .Number = 14,
        .Offset = 0x0180
      },
      {
        .Id     = BANK_ID_M,
        .Number = 15,
        .Offset = 0x01A0
      },
      {
        .Id     = BANK_ID_M,
        .Number = 16,
        .Offset = 0x01C0
      },
      {
        .Id     = BANK_ID_M,
        .Number = 17,
        .Offset = 0x01E0
      },
      {
        .Id     = BANK_ID_M,
        .Number = 40,
        .Offset = 0x0200
      },
      {
        .Id     = BANK_ID_M,
        .Number = 41,
        .Offset = 0x0220
      },
      {
        .Id     = BANK_ID_M,
        .Number = 42,
        .Offset = 0x0240
      },
      {
        .Id     = BANK_ID_M,
        .Number = 43,
        .Offset = 0x0260
      }
    }
  },
  {
    .Address    = 0x17C60000,
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
