#include <Library/HsI2cLib.h>

STATIC
EFI_HSI2C_BUS_DATA
gHsI2cBusData[] = {
  [0] = {
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

  [1] = {
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

  [2] = {
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

  [3] = {
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

  [4] = {
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

  [5] = {
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

  [6] = {
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

  [7] = {
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

  [8] = {
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

  [9] = {
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

  [10] = {
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

  [11] = {
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

  [12] = {
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

  [13] = {
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

  [14] = {
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

  [15] = {
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

  [16] = {
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

  [17] = {
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

  [18] = {
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

  [19] = {
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

  [20] = {
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

  [21] = {
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

  [22] = {
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

  [23] = {
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

  [24] = {
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

  [25] = {
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

  [26] = {
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

  [27] = {
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

  [28] = {
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

  [29] = {
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

  [30] = {
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

  [31] = {
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

  [32] = {
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

  [33] = {
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

  [34] = {
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

  [35] = {
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

  [36] = {
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

  [37] = {
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

  [38] = {
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

  [39] = {
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

  [40] = {
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

  [41] = {
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

  [42] = {
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

  [43] = {
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

  [44] = {
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

  [45] = {
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
