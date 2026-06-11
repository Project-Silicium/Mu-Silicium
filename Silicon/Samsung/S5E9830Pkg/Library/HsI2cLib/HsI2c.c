#include <Library/HsI2cLib.h>

STATIC
EFI_HSI2C_BUS_DATA
gHsI2cBusData[] = {
  {
    {
      .Function   = FUNCTION_2,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 0,
      .SdaBankNum = 0,
      .SclPin     = 0,
      .SdaPin     = 1
    },
    {
      .Freq       = 1000000,
      .SpeedMode  = FAST_PLUS_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_3,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 0,
      .SdaBankNum = 0,
      .SclPin     = 2,
      .SdaPin     = 3
    },
    {
      .Freq       = 1000000,
      .SpeedMode  = FAST_PLUS_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_2,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 0,
      .SdaBankNum = 0,
      .SclPin     = 4,
      .SdaPin     = 5
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_3,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 0,
      .SdaBankNum = 0,
      .SclPin     = 6,
      .SdaPin     = 7
    },
    {
      .Freq       = 1000000,
      .SpeedMode  = FAST_PLUS_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_2,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 1,
      .SdaBankNum = 1,
      .SclPin     = 0,
      .SdaPin     = 1
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_3,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 1,
      .SdaBankNum = 1,
      .SclPin     = 2,
      .SdaPin     = 3
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_2,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 1,
      .SdaBankNum = 1,
      .SclPin     = 4,
      .SdaPin     = 5
    },
    {
      .Freq       = 100000,
      .SpeedMode  = STAND_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_3,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 1,
      .SdaBankNum = 1,
      .SclPin     = 6,
      .SdaPin     = 7
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_2,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 2,
      .SdaBankNum = 2,
      .SclPin     = 0,
      .SdaPin     = 1
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_3,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 2,
      .SdaBankNum = 2,
      .SclPin     = 2,
      .SdaPin     = 3
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_2,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 2,
      .SdaBankNum = 2,
      .SclPin     = 4,
      .SdaPin     = 5
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_3,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 2,
      .SdaBankNum = 2,
      .SclPin     = 6,
      .SdaPin     = 7
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_2,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 5,
      .SdaBankNum = 5,
      .SclPin     = 0,
      .SdaPin     = 1
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_3,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 5,
      .SdaBankNum = 5,
      .SclPin     = 2,
      .SdaPin     = 3
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_2,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 5,
      .SdaBankNum = 5,
      .SclPin     = 4,
      .SdaPin     = 5
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_3,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 5,
      .SdaBankNum = 5,
      .SclPin     = 6,
      .SdaPin     = 7
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_2,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 6,
      .SdaBankNum = 6,
      .SclPin     = 0,
      .SdaPin     = 1
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_3,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 6,
      .SdaBankNum = 6,
      .SclPin     = 2,
      .SdaPin     = 3
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_2,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 6,
      .SdaBankNum = 6,
      .SclPin     = 4,
      .SdaPin     = 5
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_3,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 6,
      .SdaBankNum = 6,
      .SclPin     = 6,
      .SdaPin     = 7
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_2,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 7,
      .SdaBankNum = 7,
      .SclPin     = 0,
      .SdaPin     = 1
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_3,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 7,
      .SdaBankNum = 7,
      .SclPin     = 2,
      .SdaPin     = 3
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_2,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 7,
      .SdaBankNum = 7,
      .SclPin     = 4,
      .SdaPin     = 5
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_3,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 7,
      .SdaBankNum = 7,
      .SclPin     = 6,
      .SdaPin     = 7
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_2,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 8,
      .SdaBankNum = 8,
      .SclPin     = 0,
      .SdaPin     = 1
    },
    {
      .Freq       = 1000000,
      .SpeedMode  = FAST_PLUS_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_3,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 8,
      .SdaBankNum = 8,
      .SclPin     = 2,
      .SdaPin     = 3
    },
    {
      .Freq       = 400000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_2,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 3,
      .SdaBankNum = 3,
      .SclPin     = 0,
      .SdaPin     = 1
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_3,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 3,
      .SdaBankNum = 3,
      .SclPin     = 2,
      .SdaPin     = 3
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_2,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 3,
      .SdaBankNum = 3,
      .SclPin     = 4,
      .SdaPin     = 5
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_3,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 3,
      .SdaBankNum = 3,
      .SclPin     = 6,
      .SdaPin     = 7
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_2,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 4,
      .SdaBankNum = 4,
      .SclPin     = 0,
      .SdaPin     = 1
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_3,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 4,
      .SdaBankNum = 4,
      .SclPin     = 2,
      .SdaPin     = 3
    },
    {
      .Freq       = 400000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_2,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 8,
      .SdaBankNum = 8,
      .SclPin     = 4,
      .SdaPin     = 5
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_3,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 8,
      .SdaBankNum = 8,
      .SclPin     = 6,
      .SdaPin     = 7
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_2,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 9,
      .SdaBankNum = 9,
      .SclPin     = 0,
      .SdaPin     = 1
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_3,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 9,
      .SdaBankNum = 9,
      .SclPin     = 2,
      .SdaPin     = 3
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_2,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 9,
      .SdaBankNum = 9,
      .SclPin     = 4,
      .SdaPin     = 5
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_3,
      .SclBankId  = BANK_ID_P,
      .SdaBankId  = BANK_ID_P,
      .SclBankNum = 9,
      .SdaBankNum = 9,
      .SclPin     = 6,
      .SdaPin     = 7
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_2,
      .SclBankId  = BANK_ID_M,
      .SdaBankId  = BANK_ID_M,
      .SclBankNum = 0,
      .SdaBankNum = 1,
      .SclPin     = 0,
      .SdaPin     = 0
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_3,
      .SclBankId  = BANK_ID_M,
      .SdaBankId  = BANK_ID_M,
      .SclBankNum = 2,
      .SdaBankNum = 3,
      .SclPin     = 0,
      .SdaPin     = 0
    },
    {
      .Freq       = 400000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_2,
      .SclBankId  = BANK_ID_M,
      .SdaBankId  = BANK_ID_M,
      .SclBankNum = 4,
      .SdaBankNum = 5,
      .SclPin     = 0,
      .SdaPin     = 0
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_3,
      .SclBankId  = BANK_ID_M,
      .SdaBankId  = BANK_ID_M,
      .SclBankNum = 6,
      .SdaBankNum = 7,
      .SclPin     = 0,
      .SdaPin     = 0
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_2,
      .SclBankId  = BANK_ID_M,
      .SdaBankId  = BANK_ID_M,
      .SclBankNum = 8,
      .SdaBankNum = 9,
      .SclPin     = 0,
      .SdaPin     = 0
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_3,
      .SclBankId  = BANK_ID_M,
      .SdaBankId  = BANK_ID_M,
      .SclBankNum = 10,
      .SdaBankNum = 11,
      .SclPin     = 0,
      .SdaPin     = 0
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_2,
      .SclBankId  = BANK_ID_M,
      .SdaBankId  = BANK_ID_M,
      .SclBankNum = 12,
      .SdaBankNum = 13,
      .SclPin     = 0,
      .SdaPin     = 0
    },
    {
      .Freq       = 200000000,
      .SpeedMode  = FAST_SPD
    }
  },
  {
    {
      .Function   = FUNCTION_3,
      .SclBankId  = BANK_ID_M,
      .SdaBankId  = BANK_ID_M,
      .SclBankNum = 14,
      .SdaBankNum = 15,
      .SclPin     = 0,
      .SdaPin     = 0
    },
    {
      .Freq       = 1000000,
      .SpeedMode  = FAST_PLUS_SPD
    }
  }
};

VOID
GetHsI2cBusData (
  OUT EFI_HSI2C_BUS_DATA **Data,
  OUT UINT8               *Count)
{
  // Pass Data
  *Data  = gHsI2cBusData;
  *Count = ARRAY_SIZE (gHsI2cBusData);
}
