#include <Library/ClockImplLib.h>

typedef enum {
  // Fixed Clocks
  TOP_F_FRTC,
  TOP_CLK26M,
  TOP_DMPLL,

  // PLLs
  AP_ARMPLL,
  AP_ARMPLL_L,
  AP_CCIPLL,
  AP_UNIV2PLL,
  AP_MFGPLL,
  AP_MAINPLL,
  AP_APLL1,
  AP_MMPLL,
  AP_MPLL,
  AP_MSDCPLL,

  // Muxes
  TOP_AXI_SEL,
  TOP_MEM_SEL,
  TOP_MM_SEL,
  TOP_SCP_SEL,
  TOP_MFG_SEL,
  TOP_ATB_SEL,
  TOP_CAMTG_SEL,
  TOP_CAMTG1_SEL,
  TOP_CAMTG2_SEL,
  TOP_CAMTG3_SEL,
  TOP_UART_SEL,
  TOP_SPI_SEL,
  TOP_MSDC50_0_HCLK_SEL,
  TOP_MSDC50_0_SEL,
  TOP_MSDC30_1_SEL,
  TOP_AUDIO_SEL,
  TOP_AUD_INTBUS_SEL,
  TOP_AUD_1_SEL,
  TOP_AUD_ENGEN1_SEL,
  TOP_SSPM_SEL,
  TOP_DXCC_SEL,
  TOP_USB_TOP_SEL,
  TOP_SPM_SEL,
  TOP_I2C_SEL,
  TOP_PWM_SEL,
  TOP_SENINF_SEL,
  TOP_AES_FDE_SEL,
  TOP_CAMTM_SEL,
  TOP_VENC_SEL,
  TOP_CAM_SEL,

  // Factors
  TOP_CLK13M,
  TOP_SYSPLL,
  TOP_SYSPLL_D2,
  TOP_SYSPLL1_D2,
  TOP_SYSPLL1_D4,
  TOP_SYSPLL1_D8,
  TOP_SYSPLL1_D16,
  TOP_SYSPLL_D3,
  TOP_SYSPLL2_D2,
  TOP_SYSPLL2_D4,
  TOP_SYSPLL2_D8,
  TOP_SYSPLL_D5,
  TOP_SYSPLL3_D2,
  TOP_SYSPLL3_D4,
  TOP_SYSPLL_D7,
  TOP_SYSPLL4_D2,
  TOP_SYSPLL4_D4,
  TOP_USB20_192M,
  TOP_USB20_192M_D4,
  TOP_USB20_192M_D8,
  TOP_USB20_192M_D16,
  TOP_USB20_192M_D32,
  TOP_UNIVPLL,
  TOP_UNIVPLL_D2,
  TOP_UNIVPLL1_D2,
  TOP_UNIVPLL1_D4,
  TOP_UNIVPLL_D3,
  TOP_UNIVPLL2_D2,
  TOP_UNIVPLL2_D4,
  TOP_UNIVPLL2_D8,
  TOP_UNIVPLL2_D32,
  TOP_UNIVPLL_D5,
  TOP_UNIVPLL3_D2,
  TOP_UNIVPLL3_D4,
  TOP_MMPLL,
  TOP_MMPLL_D2,
  TOP_MPLL,
  TOP_DA_MPLL_104M_DIV,
  TOP_DA_MPLL_52M_DIV,
  TOP_MFGPLL,
  TOP_MSDCPLL,
  TOP_MSDCPLL_D2,
  TOP_APLL1,
  TOP_APLL1_D2,
  TOP_APLL1_D4,
  TOP_APLL1_D8,
  TOP_F_F26M,
  TOP_AXI,
  TOP_MM,
  TOP_SCP,
  TOP_MFG,
  TOP_F_FUART,
  TOP_SPI,
  TOP_MSDC50_0_HCLK,
  TOP_MSDC50_0,
  TOP_MSDC30_1,
  TOP_AUDIO,
  TOP_AUD_1,
  TOP_AUD_ENGEN1,
  TOP_SSPM,
  TOP_DXCC,
  TOP_I2C,
  TOP_F_FPWM,
  TOP_F_FSENINF,
  TOP_AES_FDE,
  TOP_VENC,
  TOP_CAM,
  TOP_F_BIST2FPC,
  TOP_ARMPLL_DIVIDER_PLL0,
  TOP_ARMPLL_DIVIDER_PLL1,
  TOP_ARMPLL_DIVIDER_PLL2,
  TOP_DA_USB20_48M_DIV,
  TOP_DA_UNIV_48M_DIV,

  // Gates
  TOP_MD_32K,
  TOP_MD_26M,
  TOP_MD2_32K,
  TOP_MD2_26M,
  TOP_ARMPLL_DIVIDER_PLL0_EN,
  TOP_ARMPLL_DIVIDER_PLL1_EN,
  TOP_ARMPLL_DIVIDER_PLL2_EN,
  TOP_FMEM_OCC_DRC_EN,
  TOP_USB20_48M_EN,
  TOP_UNIVPLL_48M_EN,
  TOP_MPLL_104M_EN,
  TOP_MPLL_52M_EN,
  TOP_F_UFS_MP_SAP_CFG_EN,
  TOP_F_BIST2FPC_EN,
  INFRA_TOPAXI_DISABLE,
  INFRA_PERI_DCM_RG_FORCE_CLKOFF,
  INFRA_PMIC_TMR,
  INFRA_PMIC_AP,
  INFRA_PMIC_MD,
  INFRA_PMIC_CONN,
  INFRA_SCP_CORE,
  INFRA_SEJ,
  INFRA_APXGPT,
  INFRA_ICUSB,
  INFRA_GCE,
  INFRA_THERM,
  INFRA_I2C_AP,
  INFRA_I2C_CCU,
  INFRA_I2C_SSPM,
  INFRA_I2C_RSV,
  INFRA_PWM_HCLK,
  INFRA_PWM1,
  INFRA_PWM2,
  INFRA_PWM3,
  INFRA_PWM4,
  INFRA_PWM5,
  INFRA_PWM,
  INFRA_UART0,
  INFRA_UART1,
  INFRA_GCE_26M,
  INFRA_CQ_DMA_FPC,
  INFRA_BTIF,
  INFRA_SPI0,
  INFRA_MSDC0,
  INFRA_MSDC1,
  INFRA_DVFSRC,
  INFRA_GCPU,
  INFRA_TRNG,
  INFRA_AUXADC,
  INFRA_CPUM,
  INFRA_CCIF1_AP,
  INFRA_CCIF1_MD,
  INFRA_AUXADC_MD,
  INFRA_AP_DMA,
  INFRA_XIU,
  INFRA_DEVICE_APC,
  INFRA_CCIF_AP,
  INFRA_DEBUGTOP,
  INFRA_AUDIO,
  INFRA_CCIF_MD,
  INFRA_DXCC_SEC_CORE,
  INFRA_DXCC_AO,
  INFRA_DRAMC_F26M,
  INFRA_RG_PWM_FBCLK6,
  INFRA_CLDMA_BCLK,
  INFRA_AUDIO_26M_BCLK,
  INFRA_SPI1,
  INFRA_I2C4,
  INFRA_MODEM_TEMP_SHARE,
  INFRA_SPI2,
  INFRA_SPI3,
  INFRA_SSPM,
  INFRA_I2C5,
  INFRA_I2C5_ARBITER,
  INFRA_I2C5_IMM,
  INFRA_I2C1_ARBITER,
  INFRA_I2C1_IMM,
  INFRA_I2C2_ARBITER,
  INFRA_I2C2_IMM,
  INFRA_SPI4,
  INFRA_SPI5,
  INFRA_CQ_DMA,
  INFRA_FAES_FDE,
  INFRA_MSDC0_SELF,
  INFRA_MSDC1_SELF,
  INFRA_SSPM_26M_SELF,
  INFRA_SSPM_32K_SELF,
  INFRA_I2C6,
  INFRA_AP_MSDC0,
  INFRA_MD_MSDC0,
  INFRA_MSDC0_SRC,
  INFRA_MSDC1_SRC,
  INFRA_SEJ_F13M,
  INFRA_AES_TOP0_BCLK,
  INFRA_MCU_PM_BCLK,
  INFRA_CCIF2_AP,
  INFRA_CCIF2_MD,
  INFRA_CCIF3_AP,
  INFRA_CCIF3_MD,

  MAX_CLOCK_ID,
} MT6768_CLOCK_ID;

STATIC CONST UINT32 TopAxiSelParents[] = {
  TOP_CLK26M,
  TOP_SYSPLL_D7,
  TOP_SYSPLL1_D4,
  TOP_SYSPLL3_D2,
};

STATIC CONST UINT32 TopMemSelParents[] = {
  TOP_CLK26M,
  TOP_DMPLL,
  TOP_APLL1,
};

STATIC CONST UINT32 TopMmSelParents[] = {
  TOP_CLK26M,
  TOP_MMPLL,
  TOP_SYSPLL1_D2,
  TOP_SYSPLL_D5,
  TOP_SYSPLL1_D4,
  TOP_UNIVPLL_D5,
  TOP_UNIVPLL1_D2,
  TOP_MMPLL_D2,
};

STATIC CONST UINT32 TopScpSelParents[] = {
  TOP_CLK26M,
  TOP_SYSPLL4_D2,
  TOP_UNIVPLL2_D2,
  TOP_SYSPLL1_D2,
  TOP_UNIVPLL1_D2,
  TOP_SYSPLL_D3,
  TOP_UNIVPLL_D3,
};

STATIC CONST UINT32 TopMfgSelParents[] = {
  TOP_CLK26M,
  TOP_MFGPLL,
  TOP_SYSPLL_D3,
  TOP_UNIVPLL_D3,
};

STATIC CONST UINT32 TopAtbSelParents[] = {
  TOP_CLK26M,
  TOP_SYSPLL1_D4,
  TOP_SYSPLL1_D2,
};

STATIC CONST UINT32 TopCamTgSelParents[] = {
  TOP_CLK26M,
  TOP_USB20_192M_D8,
  TOP_UNIVPLL2_D8,
  TOP_USB20_192M_D4,
  TOP_UNIVPLL2_D32,
  TOP_USB20_192M_D16,
  TOP_USB20_192M_D32,
};

STATIC CONST UINT32 TopUartSelParents[] = {
  TOP_CLK26M,
  TOP_UNIVPLL2_D8,
};

STATIC CONST UINT32 TopSpiSelParents[] = {
  TOP_CLK26M,
  TOP_SYSPLL3_D2,
  TOP_SYSPLL4_D2,
  TOP_SYSPLL2_D4,
};

STATIC CONST UINT32 TopMsdc50HclkSelParents[] = {
  TOP_CLK26M,
  TOP_SYSPLL1_D2,
  TOP_UNIVPLL1_D4,
  TOP_SYSPLL2_D2,
};

STATIC CONST UINT32 TopMsdc50SelParents[] = {
  TOP_CLK26M,
  TOP_MSDCPLL,
  TOP_SYSPLL2_D2,
  TOP_SYSPLL4_D2,
  TOP_UNIVPLL1_D2,
  TOP_SYSPLL1_D2,
  TOP_UNIVPLL_D5,
  TOP_UNIVPLL1_D4,
};

STATIC CONST UINT32 TopMsdc30SelParents[] = {
  TOP_CLK26M,
  TOP_MSDCPLL_D2,
  TOP_UNIVPLL2_D2,
  TOP_SYSPLL2_D2,
  TOP_SYSPLL1_D4,
  TOP_UNIVPLL1_D4,
  TOP_USB20_192M_D4,
  TOP_SYSPLL2_D4,
};

STATIC CONST UINT32 TopAudioSelParents[] = {
  TOP_CLK26M,
  TOP_SYSPLL3_D4,
  TOP_SYSPLL4_D4,
  TOP_SYSPLL1_D16,
};

STATIC CONST UINT32 TopAudIntBusSelParents[] = {
  TOP_CLK26M,
  TOP_SYSPLL1_D4,
  TOP_SYSPLL4_D2,
};

STATIC CONST UINT32 TopAud1SelParents[] = {
  TOP_CLK26M,
  TOP_APLL1,
};

STATIC CONST UINT32 TopAudEngEn1SelParents[] = {
  TOP_CLK26M,
  TOP_APLL1_D2,
  TOP_APLL1_D4,
  TOP_APLL1_D8,
};

STATIC CONST UINT32 TopSSPMSelParents[] = {
  TOP_CLK26M,
  TOP_SYSPLL1_D2,
  TOP_SYSPLL_D3,
};

STATIC CONST UINT32 TopDXCCSelParents[] = {
  TOP_CLK26M,
  TOP_SYSPLL1_D2,
  TOP_SYSPLL1_D4,
  TOP_SYSPLL1_D8,
};

STATIC CONST UINT32 TopUSBTopSelParents[] = {
  TOP_CLK26M,
  TOP_UNIVPLL3_D4,
};

STATIC CONST UINT32 TopSpmSelParents[] = {
  TOP_CLK26M,
  TOP_SYSPLL1_D8,
};

STATIC CONST UINT32 TopI2CSelParents[] = {
  TOP_CLK26M,
  TOP_UNIVPLL3_D4,
  TOP_UNIVPLL3_D2,
  TOP_SYSPLL1_D8,
  TOP_SYSPLL2_D8,
};

STATIC CONST UINT32 TopPwmSelParents[] = {
  TOP_CLK26M,
  TOP_UNIVPLL3_D4,
  TOP_SYSPLL1_D8,
};

STATIC CONST UINT32 TopSeninfSelParents[] = {
  TOP_CLK26M,
  TOP_UNIVPLL1_D4,
  TOP_UNIVPLL1_D2,
  TOP_UNIVPLL2_D2,
};

STATIC CONST UINT32 TopAesFdeSelParents[] = {
  TOP_CLK26M,
  TOP_MSDCPLL,
  TOP_UNIVPLL_D3,
  TOP_UNIVPLL2_D2,
  TOP_UNIVPLL1_D2,
  TOP_SYSPLL1_D2,
};

STATIC CONST UINT32 TopCamTmSelParents[] = {
  TOP_CLK26M,
  TOP_UNIVPLL1_D4,
  TOP_UNIVPLL1_D2,
  TOP_UNIVPLL2_D2,
};

STATIC CONST UINT32 TopVencSelParents[] = {
  TOP_CLK26M,
  TOP_MMPLL,
  TOP_SYSPLL1_D2,
  TOP_SYSPLL_D5,
  TOP_SYSPLL1_D4,
  TOP_SYSPLL_D3,
  TOP_UNIVPLL_D3,
  TOP_UNIVPLL1_D2,
};

STATIC CONST UINT32 TopCamSelParents[] = {
  TOP_CLK26M,
  TOP_SYSPLL_D2,
  TOP_SYSPLL1_D2,
  TOP_SYSPLL_D5,
  TOP_MMPLL,
  TOP_UNIVPLL_D5,
  TOP_UNIVPLL1_D2,
  TOP_MMPLL_D2,
};

MTK_CLOCK_DESC gClocks[MAX_CLOCK_ID] = {
  // Fixed Clocks
  {
    .Id    = TOP_F_FRTC,
    .Name  = "TOP_F_FRTC",
    .Type  = ClockTypeFixed,
    .Fixed = 32768,
  },
  {
    .Id    = TOP_CLK26M,
    .Name  = "TOP_CLK26M",
    .Type  = ClockTypeFixed,
    .Fixed = 26000000,
  },
  {
    .Id    = TOP_DMPLL,
    .Name  = "TOP_DMPLL",
    .Type  = ClockTypeFixed,
    .Fixed = 26000000,
  },

  // Pll
  {
    .Id         = AP_ARMPLL,
    .Name       = "AP_ARMPLL",
    .Controller = ClkApMixed,
    .Type       = ClockTypePll,
    .Pll        = {
      .BaseOffset   = 0x208,
      .PowerOffset  = 0x214,
      .EnMask       = BIT0,
      .ResetBarMask = 0,

      .PostDivOffset = 0x20C,
      .PostDivShift  = 24,

      .FMax = (3800ULL * 1000 * 1000),
      .FMin = (1500ULL * 1000 * 1000),

      .PcwOffset = 0x20C,
      .PcwShift  = 0,
      .PcwBits   = 22,
      .PcwiBits  = 8,
      .Parent    = TOP_CLK26M,
    },
  },
  {
    .Id         = AP_ARMPLL_L,
    .Name       = "AP_ARMPLL_L",
    .Controller = ClkApMixed,
    .Type       = ClockTypePll,
    .Pll        = {
      .BaseOffset   = 0x218,
      .PowerOffset  = 0x224,
      .EnMask       = BIT0,
      .ResetBarMask = 0,

      .PostDivOffset = 0x21C,
      .PostDivShift  = 24,

      .FMax = (3800ULL * 1000 * 1000),
      .FMin = (1500ULL * 1000 * 1000),

      .PcwOffset = 0x21C,
      .PcwShift  = 0,
      .PcwBits   = 22,
      .PcwiBits  = 8,
      .Parent    = TOP_CLK26M,
    },
  },
  {
    .Id         = AP_CCIPLL,
    .Name       = "AP_CCIPLL",
    .Controller = ClkApMixed,
    .Type       = ClockTypePll,
    .Pll        = {
      .BaseOffset   = 0x228,
      .PowerOffset  = 0x234,
      .EnMask       = BIT0,
      .ResetBarMask = 0,

      .PostDivOffset = 0x22C,
      .PostDivShift  = 24,

      .FMax = (3800ULL * 1000 * 1000),
      .FMin = (1500ULL * 1000 * 1000),

      .PcwOffset = 0x22C,
      .PcwShift  = 0,
      .PcwBits   = 22,
      .PcwiBits  = 8,
      .Parent    = TOP_CLK26M,
    },
  },
  {
    .Id         = AP_UNIV2PLL,
    .Name       = "AP_UNIV2PLL",
    .Controller = ClkApMixed,
    .Type       = ClockTypePll,
    .Pll        = {
      .BaseOffset   = 0x238,
      .PowerOffset  = 0x244,
      .EnMask       = BIT0,
      .ResetBarMask = BIT23,

      .PostDivOffset = 0x23C,
      .PostDivShift  = 24,

      .FMax = (3800ULL * 1000 * 1000),
      .FMin = (1500ULL * 1000 * 1000),

      .PcwOffset = 0x23C,
      .PcwShift  = 0,
      .PcwBits   = 22,
      .PcwiBits  = 8,
      .Parent    = TOP_CLK26M,
    },
  },
  {
    .Id         = AP_MFGPLL,
    .Name       = "AP_MFGPLL",
    .Controller = ClkApMixed,
    .Type       = ClockTypePll,
    .Pll        = {
      .BaseOffset   = 0x248,
      .PowerOffset  = 0x254,
      .EnMask       = BIT0,
      .ResetBarMask = BIT23,

      .PostDivOffset = 0x24C,
      .PostDivShift  = 24,

      .FMax = (3800ULL * 1000 * 1000),
      .FMin = (1500ULL * 1000 * 1000),

      .PcwOffset = 0x24C,
      .PcwShift  = 0,
      .PcwBits   = 22,
      .PcwiBits  = 8,
      .Parent    = TOP_CLK26M,
    },
  },
  {
    .Id         = AP_MAINPLL,
    .Name       = "AP_MAINPLL",
    .Controller = ClkApMixed,
    .Type       = ClockTypePll,
    .Pll        = {
      .BaseOffset   = 0x258,
      .PowerOffset  = 0x264,
      .EnMask       = BIT0,
      .ResetBarMask = 0,

      .PostDivOffset = 0x25C,
      .PostDivShift  = 24,

      .FMax = (3800ULL * 1000 * 1000),
      .FMin = (1500ULL * 1000 * 1000),

      .PcwOffset = 0x25C,
      .PcwShift  = 0,
      .PcwBits   = 22,
      .PcwiBits  = 8,
      .Parent    = TOP_CLK26M,
    },
  },
  {
    .Id         = AP_APLL1,
    .Name       = "AP_APLL1",
    .Controller = ClkApMixed,
    .Type       = ClockTypePll,
    .Pll        = {
      .BaseOffset   = 0x308,
      .PowerOffset  = 0x318,
      .EnMask       = BIT0,
      .ResetBarMask = BIT23,

      .PostDivOffset = 0x30C,
      .PostDivShift  = 24,

      .FMax = (3800ULL * 1000 * 1000),
      .FMin = (1500ULL * 1000 * 1000),

      .PcwOffset = 0x310,
      .PcwShift  = 0,
      .PcwBits   = 32,
    },
  },
  {
    .Id         = AP_MMPLL,
    .Name       = "AP_MMPLL",
    .Controller = ClkApMixed,
    .Type       = ClockTypePll,
    .Pll        = {
      .BaseOffset   = 0x31C,
      .PowerOffset  = 0x328,
      .EnMask       = BIT0,
      .ResetBarMask = BIT23,

      .PostDivOffset = 0x320,
      .PostDivShift  = 24,

      .FMax = (3800ULL * 1000 * 1000),
      .FMin = (1500ULL * 1000 * 1000),

      .PcwOffset = 0x320,
      .PcwShift  = 0,
      .PcwBits   = 22,
      .PcwiBits  = 8,
      .Parent    = TOP_CLK26M,
    },
  },
  {
    .Id         = AP_MPLL,
    .Name       = "AP_MPLL",
    .Controller = ClkApMixed,
    .Type       = ClockTypePll,
    .Pll        = {
      .BaseOffset   = 0x32C,
      .PowerOffset  = 0x338,
      .EnMask       = BIT0,
      .ResetBarMask = 0,

      .PostDivOffset = 0x330,
      .PostDivShift  = 24,

      .FMax = (3800ULL * 1000 * 1000),
      .FMin = (1500ULL * 1000 * 1000),

      .PcwOffset = 0x330,
      .PcwShift  = 0,
      .PcwBits   = 22,
      .PcwiBits  = 8,
      .Parent    = TOP_CLK26M,
    },
  },
  {
    .Id         = AP_MSDCPLL,
    .Name       = "AP_MSDCPLL",
    .Controller = ClkApMixed,
    .Type       = ClockTypePll,
    .Pll        = {
      .BaseOffset   = 0x33C,
      .PowerOffset  = 0x348,
      .EnMask       = BIT0,
      .ResetBarMask = BIT23,

      .PostDivOffset = 0x340,
      .PostDivShift  = 24,

      .FMax = (3800ULL * 1000 * 1000),
      .FMin = (1500ULL * 1000 * 1000),

      .PcwOffset = 0x340,
      .PcwShift  = 0,
      .PcwBits   = 22,
      .PcwiBits  = 8,
      .Parent    = TOP_CLK26M,
    },
  },

  // Muxes
  {
    .Id         = TOP_AXI_SEL,
    .Name       = "TOP_AXI_SEL",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeMuxGate,
    .MuxGate    =
    {
      .MuxOffset    = 0x40,
      .SetOffset    = 0x44,
      .ClearOffset  = 0x48,
      .UpdateOffset = 0x04,

      .MuxShift  = 0,
      .MuxWidth  = 2,
      .GateShift = 7,
      .UpdateShift  = 0,

      .Parents     = TopAxiSelParents,
      .ParentCount = ARRAY_SIZE (TopAxiSelParents),
    }
  },
  {
    .Id         = TOP_MEM_SEL,
    .Name       = "TOP_MEM_SEL",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeMuxGate,
    .MuxGate    =
    {
      .MuxOffset    = 0x40,
      .SetOffset    = 0x44,
      .ClearOffset  = 0x48,
      .UpdateOffset = 0x04,

      .MuxShift  = 8,
      .MuxWidth  = 2,
      .GateShift = 15,
      .UpdateShift  = 1,

      .Parents     = TopMemSelParents,
      .ParentCount = ARRAY_SIZE (TopMemSelParents),
    }
  },
  {
    .Id         = TOP_MM_SEL,
    .Name       = "TOP_MM_SEL",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeMuxGate,
    .MuxGate    =
    {
      .MuxOffset    = 0x40,
      .SetOffset    = 0x44,
      .ClearOffset  = 0x48,
      .UpdateOffset = 0x04,

      .MuxShift  = 16,
      .MuxWidth  = 3,
      .GateShift = 23,
      .UpdateShift  = 2,

      .Parents     = TopMmSelParents,
      .ParentCount = ARRAY_SIZE (TopMmSelParents),
    }
  },
  {
    .Id         = TOP_SCP_SEL,
    .Name       = "TOP_SCP_SEL",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeMuxGate,
    .MuxGate    =
    {
      .MuxOffset    = 0x40,
      .SetOffset    = 0x44,
      .ClearOffset  = 0x48,
      .UpdateOffset = 0x04,

      .MuxShift  = 24,
      .MuxWidth  = 3,
      .GateShift = 31,
      .UpdateShift = 3,

      .Parents     = TopScpSelParents,
      .ParentCount = ARRAY_SIZE (TopScpSelParents),
    }
  },
  {
    .Id         = TOP_MFG_SEL,
    .Name       = "TOP_MFG_SEL",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeMuxGate,
    .MuxGate    =
    {
      .MuxOffset    = 0x50,
      .SetOffset    = 0x54,
      .ClearOffset  = 0x58,
      .UpdateOffset = 0x04,

      .MuxShift  = 0,
      .MuxWidth  = 2,
      .GateShift = 7,
      .UpdateShift = 4,

      .Parents     = TopMfgSelParents,
      .ParentCount = ARRAY_SIZE (TopMfgSelParents),
    }
  },
  {
    .Id         = TOP_ATB_SEL,
    .Name       = "TOP_ATB_SEL",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeMuxGate,
    .MuxGate    =
    {
      .MuxOffset    = 0x50,
      .SetOffset    = 0x54,
      .ClearOffset  = 0x58,
      .UpdateOffset = 0x04,

      .MuxShift  = 8,
      .MuxWidth  = 2,
      .GateShift = 15,
      .UpdateShift = 5,

      .Parents     = TopAtbSelParents,
      .ParentCount = ARRAY_SIZE (TopAtbSelParents),
    }
  },
  {
    .Id         = TOP_CAMTG_SEL,
    .Name       = "TOP_CAMTG_SEL",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeMuxGate,
    .MuxGate    =
    {
      .MuxOffset    = 0x50,
      .SetOffset    = 0x54,
      .ClearOffset  = 0x58,
      .UpdateOffset = 0x04,

      .MuxShift  = 16,
      .MuxWidth  = 3,
      .GateShift = 23,
      .UpdateShift = 6,

      .Parents     = TopCamTgSelParents,
      .ParentCount = ARRAY_SIZE (TopCamTgSelParents),
    }
  },
  {
    .Id         = TOP_CAMTG1_SEL,
    .Name       = "TOP_CAMTG1_SEL",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeMuxGate,
    .MuxGate    =
    {
      .MuxOffset    = 0x50,
      .SetOffset    = 0x54,
      .ClearOffset  = 0x58,
      .UpdateOffset = 0x04,

      .MuxShift  = 24,
      .MuxWidth  = 3,
      .GateShift = 31,
      .UpdateShift = 7,

      .Parents     = TopCamTgSelParents,
      .ParentCount = ARRAY_SIZE (TopCamTgSelParents),
    }
  },
  {
    .Id         = TOP_CAMTG2_SEL,
    .Name       = "TOP_CAMTG2_SEL",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeMuxGate,
    .MuxGate    =
    {
      .MuxOffset    = 0x60,
      .SetOffset    = 0x64,
      .ClearOffset  = 0x68,
      .UpdateOffset = 0x04,

      .MuxShift  = 0,
      .MuxWidth  = 3,
      .GateShift = 7,
      .UpdateShift = 8,

      .Parents     = TopCamTgSelParents,
      .ParentCount = ARRAY_SIZE (TopCamTgSelParents),
    }
  },
  {
    .Id         = TOP_CAMTG3_SEL,
    .Name       = "TOP_CAMTG3_SEL",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeMuxGate,
    .MuxGate    =
    {
      .MuxOffset    = 0x60,
      .SetOffset    = 0x64,
      .ClearOffset  = 0x68,
      .UpdateOffset = 0x04,

      .MuxShift  = 8,
      .MuxWidth  = 3,
      .GateShift = 15,
      .UpdateShift = 9,

      .Parents     = TopCamTgSelParents,
      .ParentCount = ARRAY_SIZE (TopCamTgSelParents),
    }
  },
  {
    .Id         = TOP_UART_SEL,
    .Name       = "TOP_UART_SEL",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeMuxGate,
    .MuxGate    =
    {
      .MuxOffset    = 0x60,
      .SetOffset    = 0x64,
      .ClearOffset  = 0x68,
      .UpdateOffset = 0x04,

      .MuxShift  = 16,
      .MuxWidth  = 1,
      .GateShift = 23,
      .UpdateShift = 10,

      .Parents     = TopUartSelParents,
      .ParentCount = ARRAY_SIZE (TopUartSelParents),
    }
  },
  {
    .Id         = TOP_SPI_SEL,
    .Name       = "TOP_SPI_SEL",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeMuxGate,
    .MuxGate    =
    {
      .MuxOffset    = 0x60,
      .SetOffset    = 0x64,
      .ClearOffset  = 0x68,
      .UpdateOffset = 0x04,

      .MuxShift  = 24,
      .MuxWidth  = 2,
      .GateShift = 31,
      .UpdateShift = 11,

      .Parents     = TopSpiSelParents,
      .ParentCount = ARRAY_SIZE (TopSpiSelParents),
    }
  },
  {
    .Id         = TOP_MSDC50_0_HCLK_SEL,
    .Name       = "TOP_MSDC50_0_HCLK_SEL",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeMuxGate,
    .MuxGate    =
    {
      .MuxOffset    = 0x70,
      .SetOffset    = 0x74,
      .ClearOffset  = 0x78,
      .UpdateOffset = 0x04,

      .MuxShift  = 0,
      .MuxWidth  = 2,
      .GateShift = 7,
      .UpdateShift = 12,

      .Parents     = TopMsdc50HclkSelParents,
      .ParentCount = ARRAY_SIZE (TopMsdc50HclkSelParents),
    }
  },
  {
    .Id         = TOP_MSDC50_0_SEL,
    .Name       = "TOP_MSDC50_0_SEL",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeMuxGate,
    .MuxGate    =
    {
      .MuxOffset    = 0x70,
      .SetOffset    = 0x74,
      .ClearOffset  = 0x78,
      .UpdateOffset = 0x04,

      .MuxShift  = 8,
      .MuxWidth  = 3,
      .GateShift = 15,
      .UpdateShift = 13,

      .Parents     = TopMsdc50SelParents,
      .ParentCount = ARRAY_SIZE (TopMsdc50SelParents),
    }
  },
  {
    .Id         = TOP_MSDC30_1_SEL,
    .Name       = "TOP_MSDC30_1_SEL",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeMuxGate,
    .MuxGate    =
    {
      .MuxOffset    = 0x70,
      .SetOffset    = 0x74,
      .ClearOffset  = 0x78,
      .UpdateOffset = 0x04,

      .MuxShift  = 16,
      .MuxWidth  = 3,
      .GateShift = 23,
      .UpdateShift = 14,

      .Parents     = TopMsdc30SelParents,
      .ParentCount = ARRAY_SIZE (TopMsdc30SelParents),
    }
  },
  {
    .Id         = TOP_AUDIO_SEL,
    .Name       = "TOP_AUDIO_SEL",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeMuxGate,
    .MuxGate    =
    {
      .MuxOffset    = 0x70,
      .SetOffset    = 0x74,
      .ClearOffset  = 0x78,
      .UpdateOffset = 0x04,

      .MuxShift  = 24,
      .MuxWidth  = 2,
      .GateShift = 31,
      .UpdateShift = 15,

      .Parents     = TopAudioSelParents,
      .ParentCount = ARRAY_SIZE (TopAudioSelParents),
    }
  },
  {
    .Id         = TOP_AUD_INTBUS_SEL,
    .Name       = "TOP_AUD_INTBUS_SEL",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeMuxGate,
    .MuxGate    =
    {
      .MuxOffset    = 0x80,
      .SetOffset    = 0x84,
      .ClearOffset  = 0x88,
      .UpdateOffset = 0x04,

      .MuxShift  = 0,
      .MuxWidth  = 2,
      .GateShift = 7,
      .UpdateShift = 16,

      .Parents     = TopAudIntBusSelParents,
      .ParentCount = ARRAY_SIZE (TopAudIntBusSelParents),
    }
  },
  {
    .Id         = TOP_AUD_1_SEL,
    .Name       = "TOP_AUD_1_SEL",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeMuxGate,
    .MuxGate    =
    {
      .MuxOffset    = 0x80,
      .SetOffset    = 0x84,
      .ClearOffset  = 0x88,
      .UpdateOffset = 0x04,

      .MuxShift  = 8,
      .MuxWidth  = 1,
      .GateShift = 15,
      .UpdateShift = 17,

      .Parents     = TopAud1SelParents,
      .ParentCount = ARRAY_SIZE (TopAud1SelParents),
    }
  },
  {
    .Id         = TOP_AUD_ENGEN1_SEL,
    .Name       = "TOP_AUD_ENGEN1_SEL",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeMuxGate,
    .MuxGate    =
    {
      .MuxOffset    = 0x80,
      .SetOffset    = 0x84,
      .ClearOffset  = 0x88,
      .UpdateOffset = 0x04,

      .MuxShift  = 16,
      .MuxWidth  = 2,
      .GateShift = 23,
      .UpdateShift = 18,

      .Parents     = TopAudEngEn1SelParents,
      .ParentCount = ARRAY_SIZE (TopAudEngEn1SelParents),
    }
  },
  {
    .Id         = TOP_SSPM_SEL,
    .Name       = "TOP_SSPM_SEL",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeMuxGate,
    .MuxGate    =
    {
      .MuxOffset    = 0x90,
      .SetOffset    = 0x94,
      .ClearOffset  = 0x98,
      .UpdateOffset = 0x04,

      .MuxShift  = 0,
      .MuxWidth  = 2,
      .GateShift = 7,
      .UpdateShift = 20,

      .Parents     = TopSSPMSelParents,
      .ParentCount = ARRAY_SIZE (TopSSPMSelParents),
    }
  },
  {
    .Id         = TOP_DXCC_SEL,
    .Name       = "TOP_DXCC_SEL",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeMuxGate,
    .MuxGate    =
    {
      .MuxOffset    = 0x90,
      .SetOffset    = 0x94,
      .ClearOffset  = 0x98,
      .UpdateOffset = 0x04,

      .MuxShift  = 8,
      .MuxWidth  = 2,
      .GateShift = 15,
      .UpdateShift = 21,

      .Parents     = TopDXCCSelParents,
      .ParentCount = ARRAY_SIZE (TopDXCCSelParents),
    }
  },
  {
    .Id         = TOP_USB_TOP_SEL,
    .Name       = "TOP_USB_TOP_SEL",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeMuxGate,
    .MuxGate    =
    {
      .MuxOffset    = 0x90,
      .SetOffset    = 0x94,
      .ClearOffset  = 0x98,
      .UpdateOffset = 0x04,

      .MuxShift  = 16,
      .MuxWidth  = 1,
      .GateShift = 23,
      .UpdateShift = 22,

      .Parents     = TopUSBTopSelParents,
      .ParentCount = ARRAY_SIZE (TopUSBTopSelParents),
    }
  },
  {
    .Id         = TOP_SPM_SEL,
    .Name       = "TOP_SPM_SEL",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeMuxGate,
    .MuxGate    =
    {
      .MuxOffset    = 0x90,
      .SetOffset    = 0x94,
      .ClearOffset  = 0x98,
      .UpdateOffset = 0x04,

      .MuxShift  = 24,
      .MuxWidth  = 1,
      .GateShift = 31,
      .UpdateShift = 23,

      .Parents     = TopSpmSelParents,
      .ParentCount = ARRAY_SIZE (TopSpmSelParents),
    }
  },
  {
    .Id         = TOP_I2C_SEL,
    .Name       = "TOP_I2C_SEL",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeMuxGate,
    .MuxGate    =
    {
      .MuxOffset    = 0xa0,
      .SetOffset    = 0xa4,
      .ClearOffset  = 0xa8,
      .UpdateOffset = 0x04,

      .MuxShift  = 0,
      .MuxWidth  = 3,
      .GateShift = 7,
      .UpdateShift = 24,

      .Parents     = TopI2CSelParents,
      .ParentCount = ARRAY_SIZE (TopI2CSelParents),
    }
  },
  {
    .Id         = TOP_PWM_SEL,
    .Name       = "TOP_PWM_SEL",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeMuxGate,
    .MuxGate    =
    {
      .MuxOffset    = 0xa0,
      .SetOffset    = 0xa4,
      .ClearOffset  = 0xa8,
      .UpdateOffset = 0x04,

      .MuxShift  = 8,
      .MuxWidth  = 2,
      .GateShift = 15,
      .UpdateShift = 25,

      .Parents     = TopPwmSelParents,
      .ParentCount = ARRAY_SIZE (TopPwmSelParents),
    }
  },
  {
    .Id         = TOP_SENINF_SEL,
    .Name       = "TOP_SENINF_SEL",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeMuxGate,
    .MuxGate    =
    {
      .MuxOffset    = 0xa0,
      .SetOffset    = 0xa4,
      .ClearOffset  = 0xa8,
      .UpdateOffset = 0x04,

      .MuxShift  = 16,
      .MuxWidth  = 2,
      .GateShift = 23,
      .UpdateShift = 26,

      .Parents     = TopSeninfSelParents,
      .ParentCount = ARRAY_SIZE (TopSeninfSelParents),
    }
  },
  {
    .Id         = TOP_AES_FDE_SEL,
    .Name       = "TOP_AES_FDE_SEL",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeMuxGate,
    .MuxGate    =
    {
      .MuxOffset    = 0xa0,
      .SetOffset    = 0xa4,
      .ClearOffset  = 0xa8,
      .UpdateOffset = 0x04,

      .MuxShift  = 24,
      .MuxWidth  = 3,
      .GateShift = 31,
      .UpdateShift = 27,

      .Parents     = TopAesFdeSelParents,
      .ParentCount = ARRAY_SIZE (TopAesFdeSelParents),
    }
  },
  {
    .Id         = TOP_CAMTM_SEL,
    .Name       = "TOP_CAMTM_SEL",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeMuxGate,
    .MuxGate    =
    {
      .MuxOffset    = 0xb0,
      .SetOffset    = 0xb4,
      .ClearOffset  = 0xb8,
      .UpdateOffset = 0x04,

      .MuxShift  = 8,
      .MuxWidth  = 2,
      .GateShift = 15,
      .UpdateShift = 29,

      .Parents     = TopCamTmSelParents,
      .ParentCount = ARRAY_SIZE (TopCamTmSelParents),
    }
  },
  {
    .Id         = TOP_VENC_SEL,
    .Name       = "TOP_VENC_SEL",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeMuxGate,
    .MuxGate    =
    {
      .MuxOffset    = 0xb0,
      .SetOffset    = 0xb4,
      .ClearOffset  = 0xb8,
      .UpdateOffset = 0x04,

      .MuxShift  = 16,
      .MuxWidth  = 3,
      .GateShift = 23,
      .UpdateShift = 30,

      .Parents     = TopVencSelParents,
      .ParentCount = ARRAY_SIZE (TopVencSelParents),
    }
  },
  {
    .Id         = TOP_CAM_SEL,
    .Name       = "TOP_CAM_SEL",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeMuxGate,
    .MuxGate    =
    {
      .MuxOffset    = 0xb0,
      .SetOffset    = 0xb4,
      .ClearOffset  = 0xb8,
      .UpdateOffset = 0x04,

      .MuxShift    = 24,
      .MuxWidth    = 3,
      .GateShift   = 31,
      .UpdateShift = 31,

      .Parents     = TopCamSelParents,
      .ParentCount = ARRAY_SIZE (TopCamSelParents),
    }
  },

  // Factors
  {
    .Id     = TOP_CLK13M,
    .Name   = "TOP_CLK13M",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_CLK26M,
      .Mult   = 1,
      .Div    = 2,
    }
  },
  {
    .Id     = TOP_SYSPLL,
    .Name   = "TOP_SYSPLL",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = AP_MAINPLL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_SYSPLL_D2,
    .Name   = "TOP_SYSPLL_D2",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = AP_MAINPLL,
      .Mult   = 1,
      .Div    = 2,
    }
  },
  {
    .Id     = TOP_SYSPLL1_D2,
    .Name   = "TOP_SYSPLL1_D2",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_SYSPLL_D2,
      .Mult   = 1,
      .Div    = 2,
    }
  },
  {
    .Id     = TOP_SYSPLL1_D4,
    .Name   = "TOP_SYSPLL1_D4",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_SYSPLL_D2,
      .Mult   = 1,
      .Div    = 4,
    }
  },
  {
    .Id     = TOP_SYSPLL1_D8,
    .Name   = "TOP_SYSPLL1_D8",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_SYSPLL_D2,
      .Mult   = 1,
      .Div    = 8,
    }
  },
  {
    .Id     = TOP_SYSPLL1_D16,
    .Name   = "TOP_SYSPLL1_D16",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_SYSPLL_D2,
      .Mult   = 1,
      .Div    = 16,
    }
  },
  {
    .Id     = TOP_SYSPLL_D3,
    .Name   = "TOP_SYSPLL_D3",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = AP_MAINPLL,
      .Mult   = 1,
      .Div    = 3,
    }
  },
  {
    .Id     = TOP_SYSPLL2_D2,
    .Name   = "TOP_SYSPLL2_D2",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_SYSPLL_D3,
      .Mult   = 1,
      .Div    = 2,
    }
  },
  {
    .Id     = TOP_SYSPLL2_D4,
    .Name   = "TOP_SYSPLL2_D4",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_SYSPLL_D3,
      .Mult   = 1,
      .Div    = 4,
    }
  },
  {
    .Id     = TOP_SYSPLL2_D8,
    .Name   = "TOP_SYSPLL2_D8",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_SYSPLL_D3,
      .Mult   = 1,
      .Div    = 8,
    }
  },
  {
    .Id     = TOP_SYSPLL_D5,
    .Name   = "TOP_SYSPLL_D5",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = AP_MAINPLL,
      .Mult   = 1,
      .Div    = 5,
    }
  },
  {
    .Id     = TOP_SYSPLL3_D2,
    .Name   = "TOP_SYSPLL3_D2",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_SYSPLL_D5,
      .Mult   = 1,
      .Div    = 2,
    }
  },
  {
    .Id     = TOP_SYSPLL3_D4,
    .Name   = "TOP_SYSPLL3_D4",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_SYSPLL_D5,
      .Mult   = 1,
      .Div    = 4,
    }
  },
  {
    .Id     = TOP_SYSPLL_D7,
    .Name   = "TOP_SYSPLL_D7",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = AP_MAINPLL,
      .Mult   = 1,
      .Div    = 7,
    }
  },
  {
    .Id     = TOP_SYSPLL4_D2,
    .Name   = "TOP_SYSPLL4_D2",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_SYSPLL_D7,
      .Mult   = 1,
      .Div    = 2,
    }
  },
  {
    .Id     = TOP_SYSPLL4_D4,
    .Name   = "TOP_SYSPLL4_D4",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_SYSPLL_D7,
      .Mult   = 1,
      .Div    = 4,
    }
  },
  {
    .Id     = TOP_USB20_192M,
    .Name   = "TOP_USB20_192M",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_UNIVPLL,
      .Mult   = 2,
      .Div    = 13,
    }
  },
  {
    .Id     = TOP_USB20_192M_D4,
    .Name   = "TOP_USB20_192M_D4",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_USB20_192M,
      .Mult   = 1,
      .Div    = 4,
    }
  },
  {
    .Id     = TOP_USB20_192M_D8,
    .Name   = "TOP_USB20_192M_D8",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_USB20_192M,
      .Mult   = 1,
      .Div    = 8,
    }
  },
  {
    .Id     = TOP_USB20_192M_D16,
    .Name   = "TOP_USB20_192M_D16",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_USB20_192M,
      .Mult   = 1,
      .Div    = 16,
    }
  },
  {
    .Id     = TOP_USB20_192M_D32,
    .Name   = "TOP_USB20_192M_D32",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_USB20_192M,
      .Mult   = 1,
      .Div    = 32,
    }
  },
  {
    .Id     = TOP_UNIVPLL,
    .Name   = "TOP_UNIVPLL",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = AP_UNIV2PLL,
      .Mult   = 1,
      .Div    = 2,
    }
  },
  {
    .Id     = TOP_UNIVPLL_D2,
    .Name   = "TOP_UNIVPLL_D2",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_UNIVPLL,
      .Mult   = 1,
      .Div    = 2,
    }
  },
  {
    .Id     = TOP_UNIVPLL1_D2,
    .Name   = "TOP_UNIVPLL1_D2",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_UNIVPLL_D2,
      .Mult   = 1,
      .Div    = 2,
    }
  },
  {
    .Id     = TOP_UNIVPLL1_D4,
    .Name   = "TOP_UNIVPLL1_D4",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_UNIVPLL_D2,
      .Mult   = 1,
      .Div    = 4,
    }
  },
  {
    .Id     = TOP_UNIVPLL_D3,
    .Name   = "TOP_UNIVPLL_D3",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_UNIVPLL,
      .Mult   = 1,
      .Div    = 3,
    }
  },
  {
    .Id     = TOP_UNIVPLL2_D2,
    .Name   = "TOP_UNIVPLL2_D2",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_UNIVPLL_D3,
      .Mult   = 1,
      .Div    = 2,
    }
  },
  {
    .Id     = TOP_UNIVPLL2_D4,
    .Name   = "TOP_UNIVPLL2_D4",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_UNIVPLL_D3,
      .Mult   = 1,
      .Div    = 4,
    }
  },
  {
    .Id     = TOP_UNIVPLL2_D8,
    .Name   = "TOP_UNIVPLL2_D8",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_UNIVPLL_D3,
      .Mult   = 1,
      .Div    = 8,
    }
  },
  {
    .Id     = TOP_UNIVPLL2_D32,
    .Name   = "TOP_UNIVPLL2_D32",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_UNIVPLL_D3,
      .Mult   = 1,
      .Div    = 32,
    }
  },
  {
    .Id     = TOP_UNIVPLL_D5,
    .Name   = "TOP_UNIVPLL_D5",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_UNIVPLL,
      .Mult   = 1,
      .Div    = 5,
    }
  },
  {
    .Id     = TOP_UNIVPLL3_D2,
    .Name   = "TOP_UNIVPLL3_D2",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_UNIVPLL_D5,
      .Mult   = 1,
      .Div    = 2,
    }
  },
  {
    .Id     = TOP_UNIVPLL3_D4,
    .Name   = "TOP_UNIVPLL3_D4",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_UNIVPLL_D5,
      .Mult   = 1,
      .Div    = 4,
    }
  },
  {
    .Id     = TOP_MMPLL,
    .Name   = "TOP_MMPLL",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = AP_MMPLL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_MMPLL_D2,
    .Name   = "TOP_MMPLL_D2",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_MMPLL,
      .Mult   = 1,
      .Div    = 2,
    }
  },
  {
    .Id     = TOP_MPLL,
    .Name   = "TOP_MPLL",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = AP_MPLL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_DA_MPLL_104M_DIV,
    .Name   = "TOP_DA_MPLL_104M_DIV",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_MPLL,
      .Mult   = 1,
      .Div    = 2,
    }
  },
  {
    .Id     = TOP_DA_MPLL_52M_DIV,
    .Name   = "TOP_DA_MPLL_52M_DIV",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_MPLL,
      .Mult   = 1,
      .Div    = 4,
    }
  },
  {
    .Id     = TOP_MFGPLL,
    .Name   = "TOP_MFGPLL",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = AP_MFGPLL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_MSDCPLL,
    .Name   = "TOP_MSDCPLL",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = AP_MSDCPLL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_MSDCPLL_D2,
    .Name   = "TOP_MSDCPLL_D2",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_MSDCPLL,
      .Mult   = 1,
      .Div    = 2,
    }
  },
  {
    .Id     = TOP_APLL1,
    .Name   = "TOP_APLL1",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = AP_APLL1,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_APLL1_D2,
    .Name   = "TOP_APLL1_D2",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_APLL1,
      .Mult   = 1,
      .Div    = 2,
    }
  },
  {
    .Id     = TOP_APLL1_D4,
    .Name   = "TOP_APLL1_D4",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_APLL1,
      .Mult   = 1,
      .Div    = 4,
    }
  },
  {
    .Id     = TOP_APLL1_D8,
    .Name   = "TOP_APLL1_D8",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_APLL1,
      .Mult   = 1,
      .Div    = 8,
    }
  },
  {
    .Id     = TOP_F_F26M,
    .Name   = "TOP_F_F26M",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_CLK26M,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_AXI,
    .Name   = "TOP_AXI",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_AXI_SEL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_MM,
    .Name   = "TOP_MM",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_MM_SEL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_SCP,
    .Name   = "TOP_SCP",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_SCP_SEL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_MFG,
    .Name   = "TOP_MFG",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_MFG_SEL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_F_FUART,
    .Name   = "TOP_F_FUART",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_UART_SEL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_SPI,
    .Name   = "TOP_SPI",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_SPI_SEL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_MSDC50_0_HCLK,
    .Name   = "TOP_MSDC50_0_HCLK",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_MSDC50_0_HCLK_SEL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_MSDC50_0,
    .Name   = "TOP_MSDC50_0",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_MSDC50_0_SEL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_MSDC30_1,
    .Name   = "TOP_MSDC30_1",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_MSDC30_1_SEL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_AUDIO,
    .Name   = "TOP_AUDIO",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_AUDIO_SEL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_AUD_1,
    .Name   = "TOP_AUD_1",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_AUD_1_SEL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_AUD_ENGEN1,
    .Name   = "TOP_AUD_ENGEN1",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_AUD_ENGEN1_SEL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_SSPM,
    .Name   = "TOP_SSPM",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_SSPM_SEL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_DXCC,
    .Name   = "TOP_DXCC",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_DXCC_SEL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_I2C,
    .Name   = "TOP_I2C",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_I2C_SEL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_F_FPWM,
    .Name   = "TOP_F_FPWM",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_PWM_SEL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_F_FSENINF,
    .Name   = "TOP_F_FSENINF",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_SENINF_SEL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_AES_FDE,
    .Name   = "TOP_AES_FDE",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_AES_FDE_SEL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_VENC,
    .Name   = "TOP_VENC",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_VENC_SEL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_CAM,
    .Name   = "TOP_CAM",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_CAM_SEL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_F_BIST2FPC,
    .Name   = "TOP_F_BIST2FPC",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_UNIVPLL2_D2,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_ARMPLL_DIVIDER_PLL0,
    .Name   = "TOP_ARMPLL_DIVIDER_PLL0",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_SYSPLL_D2,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_ARMPLL_DIVIDER_PLL1,
    .Name   = "TOP_ARMPLL_DIVIDER_PLL1",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_SYSPLL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_ARMPLL_DIVIDER_PLL2,
    .Name   = "TOP_ARMPLL_DIVIDER_PLL2",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_UNIVPLL_D2,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_DA_USB20_48M_DIV,
    .Name   = "TOP_DA_USB20_48M_DIV",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_USB20_192M_D4,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_DA_UNIV_48M_DIV,
    .Name   = "TOP_DA_UNIV_48M_DIV",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_USB20_192M_D4,
      .Mult   = 1,
      .Div    = 1,
    }
  },

  // Gates
  {
    .Id         = TOP_MD_32K,
    .Name       = "TOP_MD_32K",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0,
      .ClearOffset  = 0,
      .StatusOffset = 0,
      .GateShift    = 8,
      .Inverted     = FALSE,
      .Parent       = TOP_F_FRTC,
    },
  },
  {
    .Id         = TOP_MD_26M,
    .Name       = "TOP_MD_26M",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0,
      .ClearOffset  = 0,
      .StatusOffset = 0,
      .GateShift    = 9,
      .Inverted     = FALSE,
      .Parent       = TOP_F_F26M,
    },
  },
  {
    .Id         = TOP_MD2_32K,
    .Name       = "TOP_MD2_32K",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0,
      .ClearOffset  = 0,
      .StatusOffset = 0,
      .GateShift    = 10,
      .Inverted     = FALSE,
      .Parent       = TOP_F_FRTC,
    },
  },
  {
    .Id         = TOP_MD2_26M,
    .Name       = "TOP_MD2_26M",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0,
      .ClearOffset  = 0,
      .StatusOffset = 0,
      .GateShift    = 11,
      .Inverted     = FALSE,
      .Parent       = TOP_F_F26M,
    },
  },
  {
    .Id         = TOP_ARMPLL_DIVIDER_PLL0_EN,
    .Name       = "TOP_ARMPLL_DIVIDER_PLL0_EN",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x104,
      .ClearOffset  = 0x104,
      .StatusOffset = 0x104,
      .GateShift    = 3,
      .Inverted     = TRUE,
      .Parent       = TOP_ARMPLL_DIVIDER_PLL0,
    },
  },
  {
    .Id         = TOP_ARMPLL_DIVIDER_PLL1_EN,
    .Name       = "TOP_ARMPLL_DIVIDER_PLL1_EN",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x104,
      .ClearOffset  = 0x104,
      .StatusOffset = 0x104,
      .GateShift    = 4,
      .Inverted     = TRUE,
      .Parent       = TOP_ARMPLL_DIVIDER_PLL1,
    },
  },
  {
    .Id         = TOP_ARMPLL_DIVIDER_PLL2_EN,
    .Name       = "TOP_ARMPLL_DIVIDER_PLL2_EN",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x104,
      .ClearOffset  = 0x104,
      .StatusOffset = 0x104,
      .GateShift    = 5,
      .Inverted     = TRUE,
      .Parent       = TOP_ARMPLL_DIVIDER_PLL2,
    },
  },
  {
    .Id         = TOP_FMEM_OCC_DRC_EN,
    .Name       = "TOP_FMEM_OCC_DRC_EN",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x104,
      .ClearOffset  = 0x104,
      .StatusOffset = 0x104,
      .GateShift    = 6,
      .Inverted     = TRUE,
      .Parent       = TOP_UNIVPLL2_D2,
    },
  },
  {
    .Id         = TOP_USB20_48M_EN,
    .Name       = "TOP_USB20_48M_EN",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x104,
      .ClearOffset  = 0x104,
      .StatusOffset = 0x104,
      .GateShift    = 8,
      .Inverted     = TRUE,
      .Parent       = TOP_DA_USB20_48M_DIV,
    },
  },
  {
    .Id         = TOP_UNIVPLL_48M_EN,
    .Name       = "TOP_UNIVPLL_48M_EN",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x104,
      .ClearOffset  = 0x104,
      .StatusOffset = 0x104,
      .GateShift    = 9,
      .Inverted     = TRUE,
      .Parent       = TOP_DA_UNIV_48M_DIV,
    },
  },
  {
    .Id         = TOP_MPLL_104M_EN,
    .Name       = "TOP_MPLL_104M_EN",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x104,
      .ClearOffset  = 0x104,
      .StatusOffset = 0x104,
      .GateShift    = 10,
      .Inverted     = TRUE,
      .Parent       = TOP_DA_MPLL_104M_DIV,
    },
  },
  {
    .Id         = TOP_MPLL_52M_EN,
    .Name       = "TOP_MPLL_52M_EN",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x104,
      .ClearOffset  = 0x104,
      .StatusOffset = 0x104,
      .GateShift    = 11,
      .Inverted     = TRUE,
      .Parent       = TOP_DA_MPLL_52M_DIV,
    },
  },
  {
    .Id         = TOP_F_UFS_MP_SAP_CFG_EN,
    .Name       = "TOP_F_UFS_MP_SAP_CFG_EN",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x104,
      .ClearOffset  = 0x104,
      .StatusOffset = 0x104,
      .GateShift    = 12,
      .Inverted     = TRUE,
      .Parent       = TOP_F_F26M,
    },
  },
  {
    .Id         = TOP_F_BIST2FPC_EN,
    .Name       = "TOP_F_BIST2FPC_EN",
    .Controller = ClkTopCkGen,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x104,
      .ClearOffset  = 0x104,
      .StatusOffset = 0x104,
      .GateShift    = 16,
      .Inverted     = TRUE,
      .Parent       = TOP_F_BIST2FPC,
    },
  },
  {
    .Id         = INFRA_TOPAXI_DISABLE,
    .Name       = "INFRA_TOPAXI_DISABLE",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x200,
      .ClearOffset  = 0x200,
      .StatusOffset = 0x200,
      .GateShift    = 31,
      .Inverted     = TRUE,
      .Parent       = TOP_AXI,
    },
  },
  {
    .Id         = INFRA_PERI_DCM_RG_FORCE_CLKOFF,
    .Name       = "INFRA_PERI_DCM_RG_FORCE_CLKOFF",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x74,
      .ClearOffset  = 0x74,
      .StatusOffset = 0x74,
      .GateShift    = 2,
      .Inverted     = FALSE,
      .Parent       = TOP_AXI,
    },
  },
  {
    .Id         = INFRA_PMIC_TMR,
    .Name       = "INFRA_PMIC_TMR",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 0,
      .Inverted     = FALSE,
      .Parent       = TOP_F_F26M,
    },
  },
  {
    .Id         = INFRA_PMIC_AP,
    .Name       = "INFRA_PMIC_AP",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 1,
      .Inverted     = FALSE,
      .Parent       = TOP_F_F26M,
    },
  },
  {
    .Id         = INFRA_PMIC_MD,
    .Name       = "INFRA_PMIC_MD",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 2,
      .Inverted     = FALSE,
      .Parent       = TOP_F_F26M,
    },
  },
  {
    .Id         = INFRA_PMIC_CONN,
    .Name       = "INFRA_PMIC_CONN",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 3,
      .Inverted     = FALSE,
      .Parent       = TOP_F_F26M,
    },
  },
  {
    .Id         = INFRA_SCP_CORE,
    .Name       = "INFRA_SCP_CORE",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 4,
      .Inverted     = FALSE,
      .Parent       = TOP_SCP,
    },
  },
  {
    .Id         = INFRA_SEJ,
    .Name       = "INFRA_SEJ",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 5,
      .Inverted     = FALSE,
      .Parent       = TOP_AXI,
    },
  },
  {
    .Id         = INFRA_APXGPT,
    .Name       = "INFRA_APXGPT",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 6,
      .Inverted     = FALSE,
      .Parent       = TOP_AXI,
    },
  },
  {
    .Id         = INFRA_ICUSB,
    .Name       = "INFRA_ICUSB",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 8,
      .Inverted     = FALSE,
      .Parent       = TOP_AXI,
    },
  },
  {
    .Id         = INFRA_GCE,
    .Name       = "INFRA_GCE",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 9,
      .Inverted     = FALSE,
      .Parent       = TOP_AXI,
    },
  },
  {
    .Id         = INFRA_THERM,
    .Name       = "INFRA_THERM",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 10,
      .Inverted     = FALSE,
      .Parent       = TOP_AXI,
    },
  },
  {
    .Id         = INFRA_I2C_AP,
    .Name       = "INFRA_I2C_AP",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 11,
      .Inverted     = FALSE,
      .Parent       = TOP_I2C,
    },
  },
  {
    .Id         = INFRA_I2C_CCU,
    .Name       = "INFRA_I2C_CCU",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 12,
      .Inverted     = FALSE,
      .Parent       = TOP_I2C,
    },
  },
  {
    .Id         = INFRA_I2C_SSPM,
    .Name       = "INFRA_I2C_SSPM",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 13,
      .Inverted     = FALSE,
      .Parent       = TOP_I2C,
    },
  },
  {
    .Id         = INFRA_I2C_RSV,
    .Name       = "INFRA_I2C_RSV",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 14,
      .Inverted     = FALSE,
      .Parent       = TOP_I2C,
    },
  },
  {
    .Id         = INFRA_PWM_HCLK,
    .Name       = "INFRA_PWM_HCLK",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 15,
      .Inverted     = FALSE,
      .Parent       = TOP_AXI,
    },
  },
  {
    .Id         = INFRA_PWM1,
    .Name       = "INFRA_PWM1",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 16,
      .Inverted     = FALSE,
      .Parent       = TOP_F_FPWM,
    },
  },
  {
    .Id         = INFRA_PWM2,
    .Name       = "INFRA_PWM2",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 17,
      .Inverted     = FALSE,
      .Parent       = TOP_F_FPWM,
    },
  },
  {
    .Id         = INFRA_PWM3,
    .Name       = "INFRA_PWM3",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 18,
      .Inverted     = FALSE,
      .Parent       = TOP_F_FPWM,
    },
  },
  {
    .Id         = INFRA_PWM4,
    .Name       = "INFRA_PWM4",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 19,
      .Inverted     = FALSE,
      .Parent       = TOP_F_FPWM,
    },
  },
  {
    .Id         = INFRA_PWM5,
    .Name       = "INFRA_PWM5",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 20,
      .Inverted     = FALSE,
      .Parent       = TOP_F_FPWM,
    },
  },
  {
    .Id         = INFRA_PWM,
    .Name       = "INFRA_PWM",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 21,
      .Inverted     = FALSE,
      .Parent       = TOP_F_FPWM,
    },
  },
  {
    .Id         = INFRA_UART0,
    .Name       = "INFRA_UART0",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 22,
      .Inverted     = FALSE,
      .Parent       = TOP_F_FUART,
    },
  },
  {
    .Id         = INFRA_UART1,
    .Name       = "INFRA_UART1",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 23,
      .Inverted     = FALSE,
      .Parent       = TOP_F_FUART,
    },
  },
  {
    .Id         = INFRA_GCE_26M,
    .Name       = "INFRA_GCE_26M",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 27,
      .Inverted     = FALSE,
      .Parent       = TOP_F_F26M,
    },
  },
  {
    .Id         = INFRA_CQ_DMA_FPC,
    .Name       = "INFRA_CQ_DMA_FPC",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 28,
      .Inverted     = FALSE,
      .Parent       = TOP_AXI,
    },
  },
  {
    .Id         = INFRA_BTIF,
    .Name       = "INFRA_BTIF",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 31,
      .Inverted     = FALSE,
      .Parent       = TOP_AXI,
    },
  },
  {
    .Id         = INFRA_SPI0,
    .Name       = "INFRA_SPI0",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x88,
      .ClearOffset  = 0x8c,
      .StatusOffset = 0x94,
      .GateShift    = 1,
      .Inverted     = FALSE,
      .Parent       = TOP_SPI,
    },
  },
  {
    .Id         = INFRA_MSDC0,
    .Name       = "INFRA_MSDC0",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x88,
      .ClearOffset  = 0x8c,
      .StatusOffset = 0x94,
      .GateShift    = 2,
      .Inverted     = FALSE,
      .Parent       = TOP_MSDC50_0_HCLK,
    },
  },
  {
    .Id         = INFRA_MSDC1,
    .Name       = "INFRA_MSDC1",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x88,
      .ClearOffset  = 0x8c,
      .StatusOffset = 0x94,
      .GateShift    = 4,
      .Inverted     = FALSE,
      .Parent       = TOP_AXI,
    },
  },
  {
    .Id         = INFRA_DVFSRC,
    .Name       = "INFRA_DVFSRC",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x88,
      .ClearOffset  = 0x8c,
      .StatusOffset = 0x94,
      .GateShift    = 7,
      .Inverted     = FALSE,
      .Parent       = TOP_F_F26M,
    },
  },
  {
    .Id         = INFRA_GCPU,
    .Name       = "INFRA_GCPU",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x88,
      .ClearOffset  = 0x8c,
      .StatusOffset = 0x94,
      .GateShift    = 8,
      .Inverted     = FALSE,
      .Parent       = TOP_AXI,
    },
  },
  {
    .Id         = INFRA_TRNG,
    .Name       = "INFRA_TRNG",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x88,
      .ClearOffset  = 0x8c,
      .StatusOffset = 0x94,
      .GateShift    = 9,
      .Inverted     = FALSE,
      .Parent       = TOP_AXI,
    },
  },
  {
    .Id         = INFRA_AUXADC,
    .Name       = "INFRA_AUXADC",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x88,
      .ClearOffset  = 0x8c,
      .StatusOffset = 0x94,
      .GateShift    = 10,
      .Inverted     = FALSE,
      .Parent       = TOP_F_F26M,
    },
  },
  {
    .Id         = INFRA_CPUM,
    .Name       = "INFRA_CPUM",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x88,
      .ClearOffset  = 0x8c,
      .StatusOffset = 0x94,
      .GateShift    = 11,
      .Inverted     = FALSE,
      .Parent       = TOP_AXI,
    },
  },
  {
    .Id         = INFRA_CCIF1_AP,
    .Name       = "INFRA_CCIF1_AP",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x88,
      .ClearOffset  = 0x8c,
      .StatusOffset = 0x94,
      .GateShift    = 12,
      .Inverted     = FALSE,
      .Parent       = TOP_AXI,
    },
  },
  {
    .Id         = INFRA_CCIF1_MD,
    .Name       = "INFRA_CCIF1_MD",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x88,
      .ClearOffset  = 0x8c,
      .StatusOffset = 0x94,
      .GateShift    = 13,
      .Inverted     = FALSE,
      .Parent       = TOP_AXI,
    },
  },
  {
    .Id         = INFRA_AUXADC_MD,
    .Name       = "INFRA_AUXADC_MD",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x88,
      .ClearOffset  = 0x8c,
      .StatusOffset = 0x94,
      .GateShift    = 14,
      .Inverted     = FALSE,
      .Parent       = TOP_F_F26M,
    },
  },
  {
    .Id         = INFRA_AP_DMA,
    .Name       = "INFRA_AP_DMA",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x88,
      .ClearOffset  = 0x8c,
      .StatusOffset = 0x94,
      .GateShift    = 18,
      .Inverted     = FALSE,
      .Parent       = TOP_AXI,
    },
  },
  {
    .Id         = INFRA_XIU,
    .Name       = "INFRA_XIU",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x88,
      .ClearOffset  = 0x8c,
      .StatusOffset = 0x94,
      .GateShift    = 19,
      .Inverted     = FALSE,
      .Parent       = TOP_AXI,
    },
  },
  {
    .Id         = INFRA_DEVICE_APC,
    .Name       = "INFRA_DEVICE_APC",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x88,
      .ClearOffset  = 0x8c,
      .StatusOffset = 0x94,
      .GateShift    = 20,
      .Inverted     = FALSE,
      .Parent       = TOP_AXI,
    },
  },
  {
    .Id         = INFRA_CCIF_AP,
    .Name       = "INFRA_CCIF_AP",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x88,
      .ClearOffset  = 0x8c,
      .StatusOffset = 0x94,
      .GateShift    = 23,
      .Inverted     = FALSE,
      .Parent       = TOP_AXI,
    },
  },
  {
    .Id         = INFRA_DEBUGTOP,
    .Name       = "INFRA_DEBUGTOP",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x88,
      .ClearOffset  = 0x8c,
      .StatusOffset = 0x94,
      .GateShift    = 24,
      .Inverted     = FALSE,
      .Parent       = TOP_AXI,
    },
  },
  {
    .Id         = INFRA_AUDIO,
    .Name       = "INFRA_AUDIO",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x88,
      .ClearOffset  = 0x8c,
      .StatusOffset = 0x94,
      .GateShift    = 25,
      .Inverted     = FALSE,
      .Parent       = TOP_AXI,
    },
  },
  {
    .Id         = INFRA_CCIF_MD,
    .Name       = "INFRA_CCIF_MD",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x88,
      .ClearOffset  = 0x8c,
      .StatusOffset = 0x94,
      .GateShift    = 26,
      .Inverted     = FALSE,
      .Parent       = TOP_AXI,
    },
  },
  {
    .Id         = INFRA_DXCC_SEC_CORE,
    .Name       = "INFRA_DXCC_SEC_CORE",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x88,
      .ClearOffset  = 0x8c,
      .StatusOffset = 0x94,
      .GateShift    = 27,
      .Inverted     = FALSE,
      .Parent       = TOP_DXCC,
    },
  },
  {
    .Id         = INFRA_DXCC_AO,
    .Name       = "INFRA_DXCC_AO",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x88,
      .ClearOffset  = 0x8c,
      .StatusOffset = 0x94,
      .GateShift    = 28,
      .Inverted     = FALSE,
      .Parent       = TOP_DXCC,
    },
  },
  {
    .Id         = INFRA_DRAMC_F26M,
    .Name       = "INFRA_DRAMC_F26M",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0x88,
      .ClearOffset  = 0x8c,
      .StatusOffset = 0x94,
      .GateShift    = 31,
      .Inverted     = FALSE,
      .Parent       = TOP_F_F26M,
    },
  },
  {
    .Id         = INFRA_RG_PWM_FBCLK6,
    .Name       = "INFRA_RG_PWM_FBCLK6",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0xa4,
      .ClearOffset  = 0xa8,
      .StatusOffset = 0xac,
      .GateShift    = 0,
      .Inverted     = FALSE,
      .Parent       = TOP_F_F26M,
    },
  },
  {
    .Id         = INFRA_CLDMA_BCLK,
    .Name       = "INFRA_CLDMA_BCLK",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0xa4,
      .ClearOffset  = 0xa8,
      .StatusOffset = 0xac,
      .GateShift    = 3,
      .Inverted     = FALSE,
      .Parent       = TOP_AXI,
    },
  },
  {
    .Id         = INFRA_AUDIO_26M_BCLK,
    .Name       = "INFRA_AUDIO_26M_BCLK",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0xa4,
      .ClearOffset  = 0xa8,
      .StatusOffset = 0xac,
      .GateShift    = 4,
      .Inverted     = FALSE,
      .Parent       = TOP_F_F26M,
    },
  },
  {
    .Id         = INFRA_SPI1,
    .Name       = "INFRA_SPI1",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0xa4,
      .ClearOffset  = 0xa8,
      .StatusOffset = 0xac,
      .GateShift    = 6,
      .Inverted     = FALSE,
      .Parent       = TOP_SPI,
    },
  },
  {
    .Id         = INFRA_I2C4,
    .Name       = "INFRA_I2C4",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0xa4,
      .ClearOffset  = 0xa8,
      .StatusOffset = 0xac,
      .GateShift    = 7,
      .Inverted     = FALSE,
      .Parent       = TOP_I2C,
    },
  },
  {
    .Id         = INFRA_MODEM_TEMP_SHARE,
    .Name       = "INFRA_MODEM_TEMP_SHARE",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0xa4,
      .ClearOffset  = 0xa8,
      .StatusOffset = 0xac,
      .GateShift    = 8,
      .Inverted     = FALSE,
      .Parent       = TOP_F_F26M,
    },
  },
  {
    .Id         = INFRA_SPI2,
    .Name       = "INFRA_SPI2",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0xa4,
      .ClearOffset  = 0xa8,
      .StatusOffset = 0xac,
      .GateShift    = 9,
      .Inverted     = FALSE,
      .Parent       = TOP_SPI,
    },
  },
  {
    .Id         = INFRA_SPI3,
    .Name       = "INFRA_SPI3",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0xa4,
      .ClearOffset  = 0xa8,
      .StatusOffset = 0xac,
      .GateShift    = 10,
      .Inverted     = FALSE,
      .Parent       = TOP_SPI,
    },
  },
  {
    .Id         = INFRA_SSPM,
    .Name       = "INFRA_SSPM",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0xa4,
      .ClearOffset  = 0xa8,
      .StatusOffset = 0xac,
      .GateShift    = 15,
      .Inverted     = FALSE,
      .Parent       = TOP_SSPM,
    },
  },
  {
    .Id         = INFRA_I2C5,
    .Name       = "INFRA_I2C5",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0xa4,
      .ClearOffset  = 0xa8,
      .StatusOffset = 0xac,
      .GateShift    = 18,
      .Inverted     = FALSE,
      .Parent       = TOP_I2C,
    },
  },
  {
    .Id         = INFRA_I2C5_ARBITER,
    .Name       = "INFRA_I2C5_ARBITER",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0xa4,
      .ClearOffset  = 0xa8,
      .StatusOffset = 0xac,
      .GateShift    = 19,
      .Inverted     = FALSE,
      .Parent       = TOP_I2C,
    },
  },
  {
    .Id         = INFRA_I2C5_IMM,
    .Name       = "INFRA_I2C5_IMM",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0xa4,
      .ClearOffset  = 0xa8,
      .StatusOffset = 0xac,
      .GateShift    = 20,
      .Inverted     = FALSE,
      .Parent       = TOP_I2C,
    },
  },
  {
    .Id         = INFRA_I2C1_ARBITER,
    .Name       = "INFRA_I2C1_ARBITER",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0xa4,
      .ClearOffset  = 0xa8,
      .StatusOffset = 0xac,
      .GateShift    = 21,
      .Inverted     = FALSE,
      .Parent       = TOP_I2C,
    },
  },
  {
    .Id         = INFRA_I2C1_IMM,
    .Name       = "INFRA_I2C1_IMM",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0xa4,
      .ClearOffset  = 0xa8,
      .StatusOffset = 0xac,
      .GateShift    = 22,
      .Inverted     = FALSE,
      .Parent       = TOP_I2C,
    },
  },
  {
    .Id         = INFRA_I2C2_ARBITER,
    .Name       = "INFRA_I2C2_ARBITER",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0xa4,
      .ClearOffset  = 0xa8,
      .StatusOffset = 0xac,
      .GateShift    = 23,
      .Inverted     = FALSE,
      .Parent       = TOP_I2C,
    },
  },
  {
    .Id         = INFRA_I2C2_IMM,
    .Name       = "INFRA_I2C2_IMM",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0xa4,
      .ClearOffset  = 0xa8,
      .StatusOffset = 0xac,
      .GateShift    = 24,
      .Inverted     = FALSE,
      .Parent       = TOP_I2C,
    },
  },
  {
    .Id         = INFRA_SPI4,
    .Name       = "INFRA_SPI4",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0xa4,
      .ClearOffset  = 0xa8,
      .StatusOffset = 0xac,
      .GateShift    = 25,
      .Inverted     = FALSE,
      .Parent       = TOP_SPI,
    },
  },
  {
    .Id         = INFRA_SPI5,
    .Name       = "INFRA_SPI5",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0xa4,
      .ClearOffset  = 0xa8,
      .StatusOffset = 0xac,
      .GateShift    = 26,
      .Inverted     = FALSE,
      .Parent       = TOP_SPI,
    },
  },
  {
    .Id         = INFRA_CQ_DMA,
    .Name       = "INFRA_CQ_DMA",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0xa4,
      .ClearOffset  = 0xa8,
      .StatusOffset = 0xac,
      .GateShift    = 27,
      .Inverted     = FALSE,
      .Parent       = TOP_AXI,
    },
  },
  {
    .Id         = INFRA_FAES_FDE,
    .Name       = "INFRA_FAES_FDE",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0xa4,
      .ClearOffset  = 0xa8,
      .StatusOffset = 0xac,
      .GateShift    = 29,
      .Inverted     = FALSE,
      .Parent       = TOP_AES_FDE,
    },
  },
  {
    .Id         = INFRA_MSDC0_SELF,
    .Name       = "INFRA_MSDC0_SELF",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0xc0,
      .ClearOffset  = 0xc4,
      .StatusOffset = 0xc8,
      .GateShift    = 0,
      .Inverted     = FALSE,
      .Parent       = TOP_MSDC50_0,
    },
  },
  {
    .Id         = INFRA_MSDC1_SELF,
    .Name       = "INFRA_MSDC1_SELF",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0xc0,
      .ClearOffset  = 0xc4,
      .StatusOffset = 0xc8,
      .GateShift    = 1,
      .Inverted     = FALSE,
      .Parent       = TOP_MSDC50_0,
    },
  },
  {
    .Id         = INFRA_SSPM_26M_SELF,
    .Name       = "INFRA_SSPM_26M_SELF",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0xc0,
      .ClearOffset  = 0xc4,
      .StatusOffset = 0xc8,
      .GateShift    = 3,
      .Inverted     = FALSE,
      .Parent       = TOP_F_F26M,
    },
  },
  {
    .Id         = INFRA_SSPM_32K_SELF,
    .Name       = "INFRA_SSPM_32K_SELF",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0xc0,
      .ClearOffset  = 0xc4,
      .StatusOffset = 0xc8,
      .GateShift    = 4,
      .Inverted     = FALSE,
      .Parent       = TOP_F_FRTC,
    },
  },
  {
    .Id         = INFRA_I2C6,
    .Name       = "INFRA_I2C6",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0xc0,
      .ClearOffset  = 0xc4,
      .StatusOffset = 0xc8,
      .GateShift    = 6,
      .Inverted     = FALSE,
      .Parent       = TOP_I2C,
    },
  },
  {
    .Id         = INFRA_AP_MSDC0,
    .Name       = "INFRA_AP_MSDC0",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0xc0,
      .ClearOffset  = 0xc4,
      .StatusOffset = 0xc8,
      .GateShift    = 7,
      .Inverted     = FALSE,
      .Parent       = TOP_MSDC50_0,
    },
  },
  {
    .Id         = INFRA_MD_MSDC0,
    .Name       = "INFRA_MD_MSDC0",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0xc0,
      .ClearOffset  = 0xc4,
      .StatusOffset = 0xc8,
      .GateShift    = 8,
      .Inverted     = FALSE,
      .Parent       = TOP_MSDC50_0,
    },
  },
  {
    .Id         = INFRA_MSDC0_SRC,
    .Name       = "INFRA_MSDC0_SRC",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0xc0,
      .ClearOffset  = 0xc4,
      .StatusOffset = 0xc8,
      .GateShift    = 9,
      .Inverted     = FALSE,
      .Parent       = TOP_MSDC50_0,
    },
  },
  {
    .Id         = INFRA_MSDC1_SRC,
    .Name       = "INFRA_MSDC1_SRC",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0xc0,
      .ClearOffset  = 0xc4,
      .StatusOffset = 0xc8,
      .GateShift    = 10,
      .Inverted     = FALSE,
      .Parent       = TOP_MSDC30_1,
    },
  },
  {
    .Id         = INFRA_SEJ_F13M,
    .Name       = "INFRA_SEJ_F13M",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0xc0,
      .ClearOffset  = 0xc4,
      .StatusOffset = 0xc8,
      .GateShift    = 15,
      .Inverted     = FALSE,
      .Parent       = TOP_F_F26M,
    },
  },
  {
    .Id         = INFRA_AES_TOP0_BCLK,
    .Name       = "INFRA_AES_TOP0_BCLK",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0xc0,
      .ClearOffset  = 0xc4,
      .StatusOffset = 0xc8,
      .GateShift    = 16,
      .Inverted     = FALSE,
      .Parent       = TOP_AXI,
    },
  },
  {
    .Id         = INFRA_MCU_PM_BCLK,
    .Name       = "INFRA_MCU_PM_BCLK",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0xc0,
      .ClearOffset  = 0xc4,
      .StatusOffset = 0xc8,
      .GateShift    = 17,
      .Inverted     = FALSE,
      .Parent       = TOP_AXI,
    },
  },
  {
    .Id         = INFRA_CCIF2_AP,
    .Name       = "INFRA_CCIF2_AP",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0xc0,
      .ClearOffset  = 0xc4,
      .StatusOffset = 0xc8,
      .GateShift    = 18,
      .Inverted     = FALSE,
      .Parent       = TOP_AXI,
    },
  },
  {
    .Id         = INFRA_CCIF2_MD,
    .Name       = "INFRA_CCIF2_MD",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0xc0,
      .ClearOffset  = 0xc4,
      .StatusOffset = 0xc8,
      .GateShift    = 19,
      .Inverted     = FALSE,
      .Parent       = TOP_AXI,
    },
  },
  {
    .Id         = INFRA_CCIF3_AP,
    .Name       = "INFRA_CCIF3_AP",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0xc0,
      .ClearOffset  = 0xc4,
      .StatusOffset = 0xc8,
      .GateShift    = 20,
      .Inverted     = FALSE,
      .Parent       = TOP_AXI,
    },
  },
  {
    .Id         = INFRA_CCIF3_MD,
    .Name       = "INFRA_CCIF3_MD",
    .Controller = ClkInfraCfgAo,
    .Type       = ClockTypeGate,
    .Gate       =
    {
      .SetOffset    = 0xc0,
      .ClearOffset  = 0xc4,
      .StatusOffset = 0xc8,
      .GateShift    = 21,
      .Inverted     = FALSE,
      .Parent       = TOP_AXI,
    },
  }
};

UINTN gClockCount = ARRAY_SIZE (gClocks);
