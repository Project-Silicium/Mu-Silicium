#include <Library/ClockImplLib.h>

typedef enum {
  // Fixed Clocks
  TOP_CLK32K,
  TOP_CLK26M,
  TOP_ULPOSC,

  // Plls
  APMIXED_ARMPLL_LL,
  APMIXED_ARMPLL_BL0,
  APMIXED_CCIPLL,
  APMIXED_MPLL,
  APMIXED_MAINPLL,
  APMIXED_UNIVPLL,
  APMIXED_MSDCPLL,
  APMIXED_MMPLL,
  APMIXED_NPUPLL,
  APMIXED_MFGPLL,
  APMIXED_TVDPLL,
  APMIXED_APLL1,
  APMIXED_APLL2,
  APMIXED_USBPLL,

  // Muxes
  TOP_AXI_SEL,
  TOP_SPM_SEL,
  TOP_SCP_SEL,
  TOP_BUS_AXIMEM_SEL,
  TOP_DISP_SEL,
  TOP_MDP_SEL,
  TOP_IMG1_SEL,
  TOP_IPE_SEL,
  TOP_CAM_SEL,
  TOP_MFG_REF_SEL,
  TOP_MFG_PLL_SEL,
  TOP_CAMTG_SEL,
  TOP_CAMTG2_SEL,
  TOP_CAMTG3_SEL,
  TOP_CAMTG4_SEL,
  TOP_CAMTG5_SEL,
  TOP_CAMTG6_SEL,
  TOP_UART_SEL,
  TOP_SPI_SEL,
  TOP_MSDC50_0_HCLK_SEL,
  TOP_MSDC50_0_SEL,
  TOP_MSDC30_1_SEL,
  TOP_AUDIO_SEL,
  TOP_AUD_INTBUS_SEL,
  TOP_PWRAP_ULPOSC_SEL,
  TOP_ATB_SEL,
  TOP_SSPM_SEL,
  TOP_SCAM_SEL,
  TOP_DISP_PWM_SEL,
  TOP_USB_SEL,
  TOP_I2C_SEL,
  TOP_SENINF_SEL,
  TOP_SENINF1_SEL,
  TOP_SENINF2_SEL,
  TOP_SENINF3_SEL,
  TOP_DXCC_SEL,
  TOP_AUD_ENGEN1_SEL,
  TOP_AUD_ENGEN2_SEL,
  TOP_AES_UFSFDE_SEL,
  TOP_UFS_SEL,
  TOP_AUD_1_SEL,
  TOP_AUD_2_SEL,
  TOP_DPMAIF_MAIN_SEL,
  TOP_VENC_SEL,
  TOP_VDEC_SEL,
  TOP_CAMTM_SEL,
  TOP_PWM_SEL,
  TOP_AUDIO_H_SEL,
  TOP_SPMI_MST_SEL,
  TOP_DVFSRC_SEL,
  TOP_AES_MSDCFDE_SEL,
  TOP_MCUPM_SEL,
  TOP_DSI_OCC_SEL,

  // Factors
  TOP_MFGPLL,
  TOP_MAINPLL_D4,
  TOP_MAINPLL_D4_D2,
  TOP_MAINPLL_D4_D4,
  TOP_MAINPLL_D4_D8,
  TOP_MAINPLL_D4_D16,
  TOP_MAINPLL_D5,
  TOP_MAINPLL_D5_D2,
  TOP_MAINPLL_D5_D4,
  TOP_MAINPLL_D5_D8,
  TOP_MAINPLL_D6,
  TOP_MAINPLL_D6_D2,
  TOP_MAINPLL_D6_D4,
  TOP_MAINPLL_D7,
  TOP_MAINPLL_D7_D2,
  TOP_MAINPLL_D7_D4,
  TOP_MAINPLL_D7_D8,
  TOP_MAINPLL_D9,
  TOP_UNIVPLL_D4,
  TOP_UNIVPLL_D4_D2,
  TOP_UNIVPLL_D4_D4,
  TOP_UNIVPLL_D4_D8,
  TOP_UNIVPLL_D5,
  TOP_UNIVPLL_D5_D2,
  TOP_UNIVPLL_D5_D4,
  TOP_UNIVPLL_D6,
  TOP_UNIVPLL_D6_D2,
  TOP_UNIVPLL_D6_D4,
  TOP_UNIVPLL_D6_D8,
  TOP_UNIVPLL_D6_D16,
  TOP_UNIVPLL_D7,
  TOP_UNIVPLL_192M_D2,
  TOP_UNIVPLL_192M_D4,
  TOP_UNIVPLL_192M_D8,
  TOP_UNIVPLL_192M_D16,
  TOP_UNIVPLL_192M_D32,
  TOP_APLL1,
  TOP_APLL1_D2,
  TOP_APLL1_D4,
  TOP_APLL1_D8,
  TOP_APLL2,
  TOP_APLL2_D2,
  TOP_APLL2_D4,
  TOP_APLL2_D8,
  TOP_MMPLL_D4_D2,
  TOP_MMPLL_D5_D2,
  TOP_MMPLL_D6,
  TOP_MMPLL_D6_D2,
  TOP_MMPLL_D7,
  TOP_MMPLL_D9,
  TOP_NPUPLL,
  TOP_TVDPLL,
  TOP_MSDCPLL,
  TOP_MSDCPLL_D2,
  TOP_MSDCPLL_D4,
  TOP_CLKRTC,
  TOP_TCK_26M_MX9,
  TOP_F26M_CK_D2,
  TOP_OSC_D2,
  TOP_OSC_D4,
  TOP_OSC_D8,
  TOP_OSC_D16,
  TOP_OSC_D10,
  TOP_OSC_D20,
  TOP_F26M,
  TOP_AXI,
  TOP_DISP,
  TOP_MDP,
  TOP_IMG1,
  TOP_IPE,
  TOP_CAM,
  TOP_MFG_REF,
  TOP_MFG_PLL,
  TOP_UART,
  TOP_SPI,
  TOP_MSDC50_0,
  TOP_MSDC30_1,
  TOP_AUDIO,
  TOP_PWRAP_ULPOSC,
  TOP_DISP_PWM,
  TOP_USB,
  TOP_I2C,
  TOP_AUD_ENGEN1,
  TOP_AUD_ENGEN2,
  TOP_AES_UFSFDE,
  TOP_UFS,
  TOP_DPMAIF_MAIN,
  TOP_VENC,
  TOP_VDEC,
  TOP_CAMTM,
  TOP_PWM,
  TOP_AUDIO_H,
  TOP_DSI_OCC,
  TOP_I2C_PSEUDO,
  TOP_APDMA,

  // Gates
  IFRAO_PMIC_TMR,
  IFRAO_PMIC_AP,
  IFRAO_GCE,
  IFRAO_GCE2,
  IFRAO_THERM,
  IFRAO_I2C_PSEUDO,
  IFRAO_APDMA_PSEUDO,
  IFRAO_PWM_HCLK,
  IFRAO_PWM1,
  IFRAO_PWM2,
  IFRAO_PWM3,
  IFRAO_PWM4,
  IFRAO_PWM,
  IFRAO_UART0,
  IFRAO_UART1,
  IFRAO_UART2,
  IFRAO_UART3,
  IFRAO_GCE_26M,
  IFRAO_BTIF,
  IFRAO_SPI0,
  IFRAO_MSDC0,
  IFRAO_MSDC1,
  IFRAO_MSDC0_SRC,
  IFRAO_AUXADC,
  IFRAO_CPUM,
  IFRAO_CCIF1_AP,
  IFRAO_CCIF1_MD,
  IFRAO_AUXADC_MD,
  IFRAO_MSDC1_SRC,
  IFRAO_MSDC0_AES,
  IFRAO_CCIF_AP,
  IFRAO_DEBUGSYS,
  IFRAO_AUDIO,
  IFRAO_CCIF_MD,
  IFRAO_SSUSB,
  IFRAO_CLDMA_BCLK,
  IFRAO_AUDIO_26M_BCLK,
  IFRAO_SPI1,
  IFRAO_SPI2,
  IFRAO_SPI3,
  IFRAO_UNIPRO_SYSCLK,
  IFRAO_UNIPRO_TICK,
  IFRAO_UFS_SAP_BCLK,
  IFRAO_SPI4,
  IFRAO_SPI5,
  IFRAO_CQ_DMA,
  IFRAO_UFS,
  IFRAO_UFS_AES,
  IFRAO_AP_MSDC0,
  IFRAO_MD_MSDC0,
  IFRAO_CCIF5_MD,
  IFRAO_CCIF2_AP,
  IFRAO_CCIF2_MD,
  IFRAO_FBIST2FPC,
  IFRAO_DPMAIF_MAIN,
  IFRAO_CCIF4_AP,
  IFRAO_CCIF4_MD,
  IFRAO_SPI6_CK,
  IFRAO_SPI7_CK,
  IFRAO_66MP_BUS_MCLK_CKP,
  IFRAO_AP_DMA,
  MAX_CLOCK_ID,
} MT6789_CLOCK_ID;

STATIC CONST UINT32 TopAxiSelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_MAINPLL_D4_D4,
	TOP_MAINPLL_D7_D2,
	TOP_MAINPLL_D4_D2,
	TOP_MAINPLL_D5_D2,
	TOP_MAINPLL_D6_D2,
	TOP_OSC_D4
};

STATIC CONST UINT32 TopSpmSelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_OSC_D10,
	TOP_MAINPLL_D7_D4,
	TOP_CLKRTC
};

STATIC CONST UINT32 TopScpSelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_UNIVPLL_D4,
	TOP_NPUPLL,
	TOP_MAINPLL_D6,
	TOP_UNIVPLL_D6,
	TOP_MAINPLL_D4_D2,
	TOP_MAINPLL_D4,
	TOP_MAINPLL_D7
};

STATIC CONST UINT32 TopBusAximemSelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_MAINPLL_D7_D2,
	TOP_MAINPLL_D4_D2,
	TOP_MAINPLL_D5_D2,
	TOP_MAINPLL_D6
};

STATIC CONST UINT32 TopDispSelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_UNIVPLL_D6_D2,
	TOP_MAINPLL_D5_D2,
	TOP_MMPLL_D6_D2,
	TOP_UNIVPLL_D5_D2,
	TOP_UNIVPLL_D4_D2,
	TOP_MMPLL_D7,
	TOP_UNIVPLL_D6,
	TOP_MAINPLL_D4,
	TOP_MMPLL_D5_D2
};

STATIC CONST UINT32 TopMdpSelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_MAINPLL_D5_D2,
	TOP_MMPLL_D6_D2,
	TOP_MAINPLL_D4_D2,
	TOP_MMPLL_D4_D2,
	TOP_MAINPLL_D6,
	TOP_MAINPLL_D5,
	TOP_MAINPLL_D4,
	TOP_TVDPLL,
	TOP_UNIVPLL_D4,
	TOP_MMPLL_D5_D2
};

STATIC CONST UINT32 TopImg1SelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_UNIVPLL_D4,
	TOP_TVDPLL,
	TOP_MAINPLL_D4,
	TOP_UNIVPLL_D5,
	TOP_MMPLL_D6,
	TOP_UNIVPLL_D6,
	TOP_MAINPLL_D6,
	TOP_MMPLL_D4_D2,
	TOP_MAINPLL_D4_D2,
	TOP_MMPLL_D6_D2,
	TOP_MMPLL_D5_D2
};

STATIC CONST UINT32 TopIpeSelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_MAINPLL_D4,
	TOP_MMPLL_D6,
	TOP_UNIVPLL_D6,
	TOP_MAINPLL_D6,
	TOP_UNIVPLL_D4_D2,
	TOP_MAINPLL_D4_D2,
	TOP_MMPLL_D6_D2,
	TOP_MMPLL_D5_D2
};

STATIC CONST UINT32 TopCamSelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_MAINPLL_D4,
	TOP_MMPLL_D6,
	TOP_UNIVPLL_D4,
	TOP_UNIVPLL_D5,
	TOP_UNIVPLL_D6,
	TOP_MMPLL_D7,
	TOP_UNIVPLL_D4_D2,
	TOP_MAINPLL_D4_D2,
	TOP_NPUPLL
};

STATIC CONST UINT32 TopMfgRefSelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_MAINPLL_D6_D2,
	TOP_MAINPLL_D6,
	TOP_MAINPLL_D5_D2
};

STATIC CONST UINT32 TopMfgPllSelParents[] = {
	TOP_MFG_REF,
	TOP_MFGPLL
};

STATIC CONST UINT32 TopCamtgSelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_UNIVPLL_192M_D8,
	TOP_UNIVPLL_D6_D8,
	TOP_UNIVPLL_192M_D4,
	TOP_UNIVPLL_D6_D16,
	TOP_F26M_CK_D2,
	TOP_UNIVPLL_192M_D16,
	TOP_UNIVPLL_192M_D32
};

STATIC CONST UINT32 TopCamtg2SelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_UNIVPLL_192M_D8,
	TOP_UNIVPLL_D6_D8,
	TOP_UNIVPLL_192M_D4,
	TOP_UNIVPLL_D6_D16,
	TOP_F26M_CK_D2,
	TOP_UNIVPLL_192M_D16,
	TOP_UNIVPLL_192M_D32
};

STATIC CONST UINT32 TopCamtg3SelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_UNIVPLL_192M_D8,
	TOP_UNIVPLL_D6_D8,
	TOP_UNIVPLL_192M_D4,
	TOP_UNIVPLL_D6_D16,
	TOP_F26M_CK_D2,
	TOP_UNIVPLL_192M_D16,
	TOP_UNIVPLL_192M_D32
};

STATIC CONST UINT32 TopCamtg4SelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_UNIVPLL_192M_D8,
	TOP_UNIVPLL_D6_D8,
	TOP_UNIVPLL_192M_D4,
	TOP_UNIVPLL_D6_D16,
	TOP_F26M_CK_D2,
	TOP_UNIVPLL_192M_D16,
	TOP_UNIVPLL_192M_D32
};

STATIC CONST UINT32 TopCamtg5SelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_UNIVPLL_192M_D8,
	TOP_UNIVPLL_D6_D8,
	TOP_UNIVPLL_192M_D4,
	TOP_UNIVPLL_D6_D16,
	TOP_F26M_CK_D2,
	TOP_UNIVPLL_192M_D16,
	TOP_UNIVPLL_192M_D32
};

STATIC CONST UINT32 TopCamtg6SelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_UNIVPLL_192M_D8,
	TOP_UNIVPLL_D6_D8,
	TOP_UNIVPLL_192M_D4,
	TOP_UNIVPLL_D6_D16,
	TOP_F26M_CK_D2,
	TOP_UNIVPLL_192M_D16,
	TOP_UNIVPLL_192M_D32
};

STATIC CONST UINT32 TopUartSelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_UNIVPLL_D6_D8
};

STATIC CONST UINT32 TopSpiSelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_MAINPLL_D5_D4,
	TOP_MAINPLL_D6_D4,
	TOP_MSDCPLL_D4,
	TOP_MSDCPLL_D2,
	TOP_MAINPLL_D6_D2,
	TOP_MAINPLL_D4_D4,
	TOP_UNIVPLL_D5_D4
};

STATIC CONST UINT32 TopMsdc50HclkSelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_MAINPLL_D4_D2,
	TOP_MAINPLL_D6_D2
};

STATIC CONST UINT32 TopMsdc50SelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_MSDCPLL,
	TOP_MSDCPLL_D2,
	TOP_UNIVPLL_D4_D4,
	TOP_MAINPLL_D6_D2,
	TOP_UNIVPLL_D4_D2
};

STATIC CONST UINT32 TopMsdc30SelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_UNIVPLL_D6_D2,
	TOP_MAINPLL_D6_D2,
	TOP_MAINPLL_D7_D2,
	TOP_MSDCPLL_D2
};

STATIC CONST UINT32 TopAudioSelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_MAINPLL_D5_D8,
	TOP_MAINPLL_D7_D8,
	TOP_MAINPLL_D4_D16
};

STATIC CONST UINT32 TopAudIntbusSelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_MAINPLL_D4_D4,
	TOP_MAINPLL_D7_D4
};

STATIC CONST UINT32 TopPwrapUlposcSelParents[] = {
	TOP_OSC_D10,
	TOP_TCK_26M_MX9,
	TOP_OSC_D4,
	TOP_OSC_D8,
	TOP_OSC_D16
};

STATIC CONST UINT32 TopAtbSelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_MAINPLL_D4_D2,
	TOP_MAINPLL_D5_D2
};

STATIC CONST UINT32 TopSspmSelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_MAINPLL_D7_D2,
	TOP_MAINPLL_D6_D2,
	TOP_MAINPLL_D5_D2,
	TOP_MAINPLL_D9,
	TOP_MAINPLL_D4_D2
};

STATIC CONST UINT32 TopScamSelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_MAINPLL_D5_D4
};

STATIC CONST UINT32 TopDispPwmSelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_UNIVPLL_D6_D4,
	TOP_OSC_D2,
	TOP_OSC_D4,
	TOP_OSC_D16
};

STATIC CONST UINT32 TopUsbSelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_UNIVPLL_D5_D4,
	TOP_UNIVPLL_D6_D4,
	TOP_UNIVPLL_D5_D2
};

STATIC CONST UINT32 TopI2cSelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_MAINPLL_D4_D8,
	TOP_UNIVPLL_D5_D4
};

STATIC CONST UINT32 TopSeninfSelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_UNIVPLL_D4_D4,
	TOP_UNIVPLL_D6_D2,
	TOP_UNIVPLL_D4_D2,
	TOP_NPUPLL,
	TOP_MMPLL_D7,
	TOP_MMPLL_D6,
	TOP_UNIVPLL_D5
};

STATIC CONST UINT32 TopSeninf1SelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_UNIVPLL_D4_D4,
	TOP_UNIVPLL_D6_D2,
	TOP_UNIVPLL_D4_D2,
	TOP_NPUPLL,
	TOP_MMPLL_D7,
	TOP_MMPLL_D6,
	TOP_UNIVPLL_D5
};

STATIC CONST UINT32 TopSeninf2SelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_UNIVPLL_D4_D4,
	TOP_UNIVPLL_D6_D2,
	TOP_UNIVPLL_D4_D2,
	TOP_NPUPLL,
	TOP_MMPLL_D7,
	TOP_MMPLL_D6,
	TOP_UNIVPLL_D5
};

STATIC CONST UINT32 TopSeninf3SelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_UNIVPLL_D4_D4,
	TOP_UNIVPLL_D6_D2,
	TOP_UNIVPLL_D4_D2,
	TOP_NPUPLL,
	TOP_MMPLL_D7,
	TOP_MMPLL_D6,
	TOP_UNIVPLL_D5
};

STATIC CONST UINT32 TopDxccSelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_MAINPLL_D4_D2,
	TOP_MAINPLL_D4_D4,
	TOP_MAINPLL_D4_D8
};

STATIC CONST UINT32 TopAudEngen1SelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_APLL1_D2,
	TOP_APLL1_D4,
	TOP_APLL1_D8
};

STATIC CONST UINT32 TopAudEngen2SelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_APLL2_D2,
	TOP_APLL2_D4,
	TOP_APLL2_D8
};

STATIC CONST UINT32 TopAesUfsfdeSelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_MAINPLL_D4,
	TOP_MAINPLL_D4_D2,
	TOP_MAINPLL_D6,
	TOP_MAINPLL_D4_D4,
	TOP_UNIVPLL_D4_D2,
	TOP_UNIVPLL_D6
};

STATIC CONST UINT32 TopUfsSelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_MAINPLL_D4_D4,
	TOP_MAINPLL_D4_D8,
	TOP_UNIVPLL_D4_D4,
	TOP_MAINPLL_D6_D2,
	TOP_MAINPLL_D5_D2,
	TOP_MSDCPLL_D2
};

STATIC CONST UINT32 TopAud1SelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_APLL1
};

STATIC CONST UINT32 TopAud2SelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_APLL2
};

STATIC CONST UINT32 TopDpmaifMainSelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_UNIVPLL_D4_D4,
	TOP_MAINPLL_D6,
	TOP_MAINPLL_D4_D2,
	TOP_UNIVPLL_D4_D2
};

STATIC CONST UINT32 TopVencSelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_MMPLL_D7,
	TOP_MAINPLL_D6,
	TOP_UNIVPLL_D4_D2,
	TOP_MAINPLL_D4_D2,
	TOP_UNIVPLL_D6,
	TOP_MMPLL_D6,
	TOP_MAINPLL_D5_D2,
	TOP_MAINPLL_D6_D2,
	TOP_MMPLL_D9,
	TOP_UNIVPLL_D4_D4,
	TOP_MAINPLL_D4,
	TOP_UNIVPLL_D4,
	TOP_UNIVPLL_D5,
	TOP_UNIVPLL_D5_D2,
	TOP_MAINPLL_D5
};

STATIC CONST UINT32 TopVdecSelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_UNIVPLL_192M_D2,
	TOP_UNIVPLL_D5_D4,
	TOP_MAINPLL_D5,
	TOP_MAINPLL_D5_D2,
	TOP_MMPLL_D6_D2,
	TOP_UNIVPLL_D5_D2,
	TOP_MAINPLL_D4_D2,
	TOP_UNIVPLL_D4_D2,
	TOP_UNIVPLL_D7,
	TOP_MMPLL_D7,
	TOP_MMPLL_D6,
	TOP_UNIVPLL_D5,
	TOP_MAINPLL_D4,
	TOP_UNIVPLL_D4,
	TOP_UNIVPLL_D6
};

STATIC CONST UINT32 TopCamtmSelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_UNIVPLL_D7,
	TOP_UNIVPLL_D6_D2,
	TOP_UNIVPLL_D4_D2
};

STATIC CONST UINT32 TopPwmSelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_UNIVPLL_D4_D8
};

STATIC CONST UINT32 TopAudioHSelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_UNIVPLL_D7,
	TOP_APLL1,
	TOP_APLL2
};

STATIC CONST UINT32 TopSpmiMstSelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_F26M_CK_D2,
	TOP_OSC_D8,
	TOP_OSC_D10,
	TOP_OSC_D16,
	TOP_OSC_D20,
	TOP_CLKRTC
};

STATIC CONST UINT32 TopDvfsrcSelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_OSC_D10
};

STATIC CONST UINT32 TopAesMsdcfdeSelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_MAINPLL_D4_D2,
	TOP_MAINPLL_D6,
	TOP_MAINPLL_D4_D4,
	TOP_UNIVPLL_D4_D2,
	TOP_UNIVPLL_D6
};

STATIC CONST UINT32 TopMcupmSelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_MAINPLL_D6_D4,
	TOP_MAINPLL_D6_D2
};

STATIC CONST UINT32 TopDsiOccSelParents[] = {
	TOP_TCK_26M_MX9,
	TOP_MAINPLL_D6_D2,
	TOP_UNIVPLL_D5_D2,
	TOP_UNIVPLL_D4_D2
};

STATIC CONST UINT32 TopApllI2s0MckSelParents[] = {
	TOP_AUD_1_SEL,
	TOP_AUD_2_SEL
};

STATIC CONST UINT32 TopApllI2s1MckSelParents[] = {
	TOP_AUD_1_SEL,
	TOP_AUD_2_SEL
};

STATIC CONST UINT32 TopApllI2s2MckSelParents[] = {
	TOP_AUD_1_SEL,
	TOP_AUD_2_SEL
};

STATIC CONST UINT32 TopApllI2s3MckSelParents[] = {
	TOP_AUD_1_SEL,
	TOP_AUD_2_SEL
};

STATIC CONST UINT32 TopApllI2s4MckSelParents[] = {
	TOP_AUD_1_SEL,
	TOP_AUD_2_SEL
};

STATIC CONST UINT32 TopApllI2s5MckSelParents[] = {
	TOP_AUD_1_SEL,
	TOP_AUD_2_SEL
};

STATIC CONST UINT32 TopApllI2s6MckSelParents[] = {
	TOP_AUD_1_SEL,
	TOP_AUD_2_SEL
};

STATIC CONST UINT32 TopApllI2s7MckSelParents[] = {
	TOP_AUD_1_SEL,
	TOP_AUD_2_SEL
};

STATIC CONST UINT32 TopApllI2s8MckSelParents[] = {
	TOP_AUD_1_SEL,
	TOP_AUD_2_SEL
};

STATIC CONST UINT32 TopApllI2s9MckSelParents[] = {
	TOP_AUD_1_SEL,
	TOP_AUD_2_SEL
};

MTK_CLOCK_DESC gClocks[MAX_CLOCK_ID] = {
  // Fixed Clocks
  {
    .Id    = TOP_CLK32K,
    .Name  = "TOP_CLK32K",
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
    .Id    = TOP_ULPOSC,
    .Name  = "TOP_ULPOSC",
    .Type  = ClockTypeFixed,
    .Fixed = 0,
  },

  // Pll
  {
    .Id   = APMIXED_ARMPLL_LL,
    .Name = "APMIXED_ARMPLL_LL",
    .Controller = ClkApMixed,
    .Type = ClockTypePll,
    .Pll  =
    {
      .BaseOffset    = 0x208,
      .PowerOffset   = 0x214,
      .EnMask        = BIT0,
      .ResetBarMask  = 0,

      .PostDivOffset = 0x20C,
      .PostDivShift  = 24,

      .FMax          = (3800ULL * 1000 * 1000),
      .FMin          = (1500ULL * 1000 * 1000),

      .PcwOffset     = 0x20C,
      .PcwShift      = 0,
      .PcwBits       = 22,
      .PcwiBits      = 8,
      .Parent        = TOP_CLK26M,
    },
  },
  {
    .Id   = APMIXED_ARMPLL_BL0,
    .Name = "APMIXED_ARMPLL_BL0",
    .Controller = ClkApMixed,
    .Type = ClockTypePll,
    .Pll  =
    {
      .BaseOffset    = 0x218,
      .PowerOffset   = 0x224,
      .EnMask        = BIT0,
      .ResetBarMask  = 0,

      .PostDivOffset = 0x21C,
      .PostDivShift  = 24,

      .FMax          = (3800ULL * 1000 * 1000),
      .FMin          = (1500ULL * 1000 * 1000),

      .PcwOffset     = 0x21C,
      .PcwShift      = 0,
      .PcwBits       = 22,
      .PcwiBits      = 8,
      .Parent        = TOP_CLK26M,
    },
  },
  {
    .Id   = APMIXED_CCIPLL,
    .Name = "APMIXED_CCIPLL",
    .Controller = ClkApMixed,
    .Type = ClockTypePll,
    .Pll  =
    {
      .BaseOffset    = 0x258,
      .PowerOffset   = 0x264,
      .EnMask        = BIT0,
      .ResetBarMask  = 0,

      .PostDivOffset = 0x25C,
      .PostDivShift  = 24,

      .FMax          = (3800ULL * 1000 * 1000),
      .FMin          = (1500ULL * 1000 * 1000),

      .PcwOffset     = 0x25C,
      .PcwShift      = 0,
      .PcwBits       = 22,
      .PcwiBits      = 8,
      .Parent        = TOP_CLK26M,
    },
  },
  {
    .Id   = APMIXED_MPLL,
    .Name = "APMIXED_MPLL",
    .Controller = ClkApMixed,
    .Type = ClockTypePll,
    .Pll  =
    {
      .BaseOffset    = 0x390,
      .PowerOffset   = 0x39C,
      .EnMask        = BIT0,
      .ResetBarMask  = 0,

      .PostDivOffset = 0x394,
      .PostDivShift  = 24,

      .FMax          = (3800ULL * 1000 * 1000),
      .FMin          = (1500ULL * 1000 * 1000),

      .PcwOffset     = 0x394,
      .PcwShift      = 0,
      .PcwBits       = 22,
      .PcwiBits      = 8,
      .Parent        = TOP_CLK26M,
    },
  },
  {
    .Id   = APMIXED_MAINPLL,
    .Name = "APMIXED_MAINPLL",
    .Controller = ClkApMixed,
    .Type = ClockTypePll,
    .Pll  =
    {
      .BaseOffset    = 0x340,
      .PowerOffset   = 0x34C,
      .EnMask        = BIT0,
      .ResetBarMask  = BIT23,

      .PostDivOffset = 0x344,
      .PostDivShift  = 24,

      .FMax          = (3800ULL * 1000 * 1000),
      .FMin          = (1500ULL * 1000 * 1000),

      .PcwOffset     = 0x344,
      .PcwShift      = 0,
      .PcwBits       = 22,
      .PcwiBits      = 8,
      .Parent        = TOP_CLK26M,
    },
  },
  {
    .Id   = APMIXED_UNIVPLL,
    .Name = "APMIXED_UNIVPLL",
    .Controller = ClkApMixed,
    .Type = ClockTypePll,
    .Pll  =
    {
      .BaseOffset    = 0x308,
      .PowerOffset   = 0x314,
      .EnMask        = BIT0,
      .ResetBarMask  = BIT23,

      .PostDivOffset = 0x30C,
      .PostDivShift  = 24,

      .FMax          = (3800ULL * 1000 * 1000),
      .FMin          = (1500ULL * 1000 * 1000),

      .PcwOffset     = 0x30C,
      .PcwShift      = 0,
      .PcwBits       = 22,
      .PcwiBits      = 8,
      .Parent        = TOP_CLK26M,
    },
  },
  {
    .Id   = APMIXED_MSDCPLL,
    .Name = "APMIXED_MSDCPLL",
    .Controller = ClkApMixed,
    .Type = ClockTypePll,
    .Pll  =
    {
      .BaseOffset    = 0x350,
      .PowerOffset   = 0x35C,
      .EnMask        = BIT0,
      .ResetBarMask  = 0,

      .PostDivOffset = 0x354,
      .PostDivShift  = 24,

      .FMax          = (3800ULL * 1000 * 1000),
      .FMin          = (1500ULL * 1000 * 1000),

      .PcwOffset     = 0x354,
      .PcwShift      = 0,
      .PcwBits       = 22,
      .PcwiBits      = 8,
      .Parent        = TOP_CLK26M,
    },
  },
  {
    .Id   = APMIXED_MMPLL,
    .Name = "APMIXED_MMPLL",
    .Controller = ClkApMixed,
    .Type = ClockTypePll,
    .Pll  =
    {
      .BaseOffset    = 0x360,
      .PowerOffset   = 0x36C,
      .EnMask        = BIT0,
      .ResetBarMask  = BIT23,

      .PostDivOffset = 0x364,
      .PostDivShift  = 24,

      .FMax          = (3800ULL * 1000 * 1000),
      .FMin          = (1500ULL * 1000 * 1000),

      .PcwOffset     = 0x364,
      .PcwShift      = 0,
      .PcwBits       = 22,
      .PcwiBits      = 8,
      .Parent        = TOP_CLK26M,
    },
  },
  {
    .Id   = APMIXED_NPUPLL,
    .Name = "APMIXED_NPUPLL",
    .Controller = ClkApMixed,
    .Type = ClockTypePll,
    .Pll  =
    {
      .BaseOffset    = 0x3B4,
      .PowerOffset   = 0x3C0,
      .EnMask        = BIT0,
      .ResetBarMask  = 0,

      .PostDivOffset = 0x3B8,
      .PostDivShift  = 24,

      .FMax          = (3800ULL * 1000 * 1000),
      .FMin          = (1500ULL * 1000 * 1000),

      .PcwOffset     = 0x3B8,
      .PcwShift      = 0,
      .PcwBits       = 22,
      .PcwiBits      = 8,
      .Parent        = TOP_CLK26M,
    },
  },
  {
    .Id   = APMIXED_MFGPLL,
    .Name = "APMIXED_MFGPLL",
    .Controller = ClkApMixed,
    .Type = ClockTypePll,
    .Pll  =
    {
      .BaseOffset    = 0x268,
      .PowerOffset   = 0x274,
      .EnMask        = BIT0,
      .ResetBarMask  = 0,

      .PostDivOffset = 0x26C,
      .PostDivShift  = 24,

      .FMax          = (3800ULL * 1000 * 1000),
      .FMin          = (1500ULL * 1000 * 1000),

      .PcwOffset     = 0x26C,
      .PcwShift      = 0,
      .PcwBits       = 22,
      .PcwiBits      = 8,
      .Parent        = TOP_CLK26M,
    },
  },
  {
    .Id   = APMIXED_TVDPLL,
    .Name = "APMIXED_TVDPLL",
    .Controller = ClkApMixed,
    .Type = ClockTypePll,
    .Pll  =
    {
      .BaseOffset    = 0x380,
      .PowerOffset   = 0x38C,
      .EnMask        = BIT0,
      .ResetBarMask  = 0,

      .PostDivOffset = 0x384,
      .PostDivShift  = 24,

      .FMax          = (3800ULL * 1000 * 1000),
      .FMin          = (1500ULL * 1000 * 1000),

      .PcwOffset     = 0x384,
      .PcwShift      = 0,
      .PcwBits       = 22,
      .PcwiBits      = 8,
      .Parent        = TOP_CLK26M,
    },
  },
  {
    .Id   = APMIXED_APLL1,
    .Name = "APMIXED_APLL1",
    .Controller = ClkApMixed,
    .Type = ClockTypePll,
    .Pll  =
    {
      .BaseOffset    = 0x318,
      .PowerOffset   = 0x328,
      .EnMask        = BIT0,
      .ResetBarMask  = 0,

      .PostDivOffset = 0x31C,
      .PostDivShift  = 24,

      .FMax          = (3800ULL * 1000 * 1000),
      .FMin          = (1500ULL * 1000 * 1000),

      .PcwOffset     = 0x320,
      .PcwShift      = 0,
      .PcwBits       = 32,
      .PcwiBits      = 8,
      .Parent        = TOP_CLK26M,
    },
  },
  {
    .Id   = APMIXED_APLL2,
    .Name = "APMIXED_APLL2",
    .Controller = ClkApMixed,
    .Type = ClockTypePll,
    .Pll  =
    {
      .BaseOffset    = 0x32C,
      .PowerOffset   = 0x33C,
      .EnMask        = BIT0,
      .ResetBarMask  = 0,

      .PostDivOffset = 0x330,
      .PostDivShift  = 24,

      .FMax          = (3800ULL * 1000 * 1000),
      .FMin          = (1500ULL * 1000 * 1000),

      .PcwOffset     = 0x334,
      .PcwShift      = 0,
      .PcwBits       = 32,
      .PcwiBits      = 8,
      .Parent        = TOP_CLK26M,
    },
  },
  {
    .Id   = APMIXED_USBPLL,
    .Name = "APMIXED_USBPLL",
    .Controller = ClkApMixed,
    .Type = ClockTypePll,
    .Pll  =
    {
      .BaseOffset    = 0x3C4,
      .PowerOffset   = 0x3CC,
      .EnMask        = BIT0,
      .ResetBarMask  = BIT23,

      .PostDivOffset = 0x3C4,
      .PostDivShift  = 24,

      .FMax          = (3800ULL * 1000 * 1000),
      .FMin          = (1500ULL * 1000 * 1000),

      .PcwOffset     = 0x3C4,
      .PcwShift      = 0,
      .PcwBits       = 22,
      .PcwiBits      = 8,
      .Parent        = TOP_CLK26M,
    },
  },

  // Muxes
  {
    .Id   = TOP_AXI_SEL,
    .Name = "TOP_AXI_SEL",
    .Controller = ClkTopCkGen,
    .Type = ClockTypeMux,
    .MuxGate  =
    {
      .MuxOffset    = 0x10,
      .SetOffset    = 0x14,
      .ClearOffset  = 0x18,
      .UpdateOffset = 0x04,
      .MuxShift     = 0,
      .MuxWidth     = 3,
      .UpdateShift  = 0,
      .Parents      = TopAxiSelParents,
      .ParentCount  = ARRAY_SIZE (TopAxiSelParents),
    }
  },
  {
    .Id   = TOP_SPM_SEL,
    .Name = "TOP_SPM_SEL",
    .Controller = ClkTopCkGen,
    .Type = ClockTypeMux,
    .MuxGate  =
    {
      .MuxOffset    = 0x10,
      .SetOffset    = 0x14,
      .ClearOffset  = 0x18,
      .UpdateOffset = 0x04,
      .MuxShift     = 8,
      .MuxWidth     = 2,
      .UpdateShift  = 1,
      .Parents      = TopSpmSelParents,
      .ParentCount  = ARRAY_SIZE (TopSpmSelParents),
    }
  },
  {
    .Id      = TOP_SCP_SEL,
    .Name    = "TOP_SCP_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0x10,
      .SetOffset    = 0x14,
      .ClearOffset  = 0x18,
      .UpdateOffset = 0x04,
      .MuxShift     = 16,
      .MuxWidth     = 3,
      .GateShift    = 23,
      .UpdateShift  = 2,
      .Parents      = TopScpSelParents,
      .ParentCount  = ARRAY_SIZE (TopScpSelParents),
    }
  },
  {
    .Id   = TOP_BUS_AXIMEM_SEL,
    .Name = "TOP_BUS_AXIMEM_SEL",
    .Controller = ClkTopCkGen,
    .Type = ClockTypeMux,
    .MuxGate  =
    {
      .MuxOffset    = 0x10,
      .SetOffset    = 0x14,
      .ClearOffset  = 0x18,
      .UpdateOffset = 0x04,
      .MuxShift     = 24,
      .MuxWidth     = 3,
      .UpdateShift  = 3,
      .Parents      = TopBusAximemSelParents,
      .ParentCount  = ARRAY_SIZE (TopBusAximemSelParents),
    }
  },
  {
    .Id      = TOP_DISP_SEL,
    .Name    = "TOP_DISP_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0x20,
      .SetOffset    = 0x24,
      .ClearOffset  = 0x28,
      .UpdateOffset = 0x04,
      .MuxShift     = 0,
      .MuxWidth     = 4,
      .GateShift    = 7,
      .UpdateShift  = 4,
      .Parents      = TopDispSelParents,
      .ParentCount  = ARRAY_SIZE (TopDispSelParents),
    }
  },
  {
    .Id      = TOP_MDP_SEL,
    .Name    = "TOP_MDP_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0x20,
      .SetOffset    = 0x24,
      .ClearOffset  = 0x28,
      .UpdateOffset = 0x04,
      .MuxShift     = 8,
      .MuxWidth     = 4,
      .GateShift    = 15,
      .UpdateShift  = 5,
      .Parents      = TopMdpSelParents,
      .ParentCount  = ARRAY_SIZE (TopMdpSelParents),
    }
  },
  {
    .Id      = TOP_IMG1_SEL,
    .Name    = "TOP_IMG1_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0x20,
      .SetOffset    = 0x24,
      .ClearOffset  = 0x28,
      .UpdateOffset = 0x04,
      .MuxShift     = 16,
      .MuxWidth     = 4,
      .GateShift    = 23,
      .UpdateShift  = 6,
      .Parents      = TopImg1SelParents,
      .ParentCount  = ARRAY_SIZE (TopImg1SelParents),
    }
  },
  {
    .Id      = TOP_IPE_SEL,
    .Name    = "TOP_IPE_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0x30,
      .SetOffset    = 0x34,
      .ClearOffset  = 0x38,
      .UpdateOffset = 0x04,
      .MuxShift     = 0,
      .MuxWidth     = 4,
      .GateShift    = 7,
      .UpdateShift  = 8,
      .Parents      = TopIpeSelParents,
      .ParentCount  = ARRAY_SIZE (TopIpeSelParents),
    }
  },
  {
    .Id      = TOP_CAM_SEL,
    .Name    = "TOP_CAM_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0x30,
      .SetOffset    = 0x34,
      .ClearOffset  = 0x38,
      .UpdateOffset = 0x04,
      .MuxShift     = 16,
      .MuxWidth     = 4,
      .GateShift    = 23,
      .UpdateShift  = 10,
      .Parents      = TopCamSelParents,
      .ParentCount  = ARRAY_SIZE (TopCamSelParents),
    }
  },
  {
    .Id      = TOP_MFG_REF_SEL,
    .Name    = "TOP_MFG_REF_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0x50,
      .SetOffset    = 0x54,
      .ClearOffset  = 0x58,
      .UpdateOffset = 0x04,
      .MuxShift     = 16,
      .MuxWidth     = 2,
      .GateShift    = 23,
      .UpdateShift  = 18,
      .Parents      = TopMfgRefSelParents,
      .ParentCount  = ARRAY_SIZE (TopMfgRefSelParents),
    }
  },
  {
    .Id   = TOP_MFG_PLL_SEL,
    .Name = "TOP_MFG_PLL_SEL",
    .Controller = ClkTopCkGen,
    .Type = ClockTypeMux,
    .MuxGate  =
    {
      .MuxOffset    = 0x50,
      .SetOffset    = 0x54,
      .ClearOffset  = 0x58,
      .UpdateOffset = -1,
      .MuxShift     = 18,
      .MuxWidth     = 1,
      .UpdateShift  = -1,
      .Parents      = TopMfgPllSelParents,
      .ParentCount  = ARRAY_SIZE (TopMfgPllSelParents),
    }
  },
  {
    .Id      = TOP_CAMTG_SEL,
    .Name    = "TOP_CAMTG_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0x50,
      .SetOffset    = 0x54,
      .ClearOffset  = 0x58,
      .UpdateOffset = 0x04,
      .MuxShift     = 24,
      .MuxWidth     = 3,
      .GateShift    = 31,
      .UpdateShift  = 19,
      .Parents      = TopCamtgSelParents,
      .ParentCount  = ARRAY_SIZE (TopCamtgSelParents),
    }
  },
  {
    .Id      = TOP_CAMTG2_SEL,
    .Name    = "TOP_CAMTG2_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0x60,
      .SetOffset    = 0x64,
      .ClearOffset  = 0x68,
      .UpdateOffset = 0x04,
      .MuxShift     = 0,
      .MuxWidth     = 3,
      .GateShift    = 7,
      .UpdateShift  = 20,
      .Parents      = TopCamtg2SelParents,
      .ParentCount  = ARRAY_SIZE (TopCamtg2SelParents),
    }
  },
  {
    .Id      = TOP_CAMTG3_SEL,
    .Name    = "TOP_CAMTG3_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0x60,
      .SetOffset    = 0x64,
      .ClearOffset  = 0x68,
      .UpdateOffset = 0x04,
      .MuxShift     = 8,
      .MuxWidth     = 3,
      .GateShift    = 15,
      .UpdateShift  = 21,
      .Parents      = TopCamtg3SelParents,
      .ParentCount  = ARRAY_SIZE (TopCamtg3SelParents),
    }
  },
  {
    .Id      = TOP_CAMTG4_SEL,
    .Name    = "TOP_CAMTG4_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0x60,
      .SetOffset    = 0x64,
      .ClearOffset  = 0x68,
      .UpdateOffset = 0x04,
      .MuxShift     = 16,
      .MuxWidth     = 3,
      .GateShift    = 23,
      .UpdateShift  = 22,
      .Parents      = TopCamtg4SelParents,
      .ParentCount  = ARRAY_SIZE (TopCamtg4SelParents),
    }
  },
  {
    .Id      = TOP_CAMTG5_SEL,
    .Name    = "TOP_CAMTG5_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0x60,
      .SetOffset    = 0x64,
      .ClearOffset  = 0x68,
      .UpdateOffset = 0x04,
      .MuxShift     = 24,
      .MuxWidth     = 3,
      .GateShift    = 31,
      .UpdateShift  = 23,
      .Parents      = TopCamtg5SelParents,
      .ParentCount  = ARRAY_SIZE (TopCamtg5SelParents),
    }
  },
  {
    .Id      = TOP_CAMTG6_SEL,
    .Name    = "TOP_CAMTG6_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0x70,
      .SetOffset    = 0x74,
      .ClearOffset  = 0x78,
      .UpdateOffset = 0x04,
      .MuxShift     = 0,
      .MuxWidth     = 3,
      .GateShift    = 7,
      .UpdateShift  = 24,
      .Parents      = TopCamtg6SelParents,
      .ParentCount  = ARRAY_SIZE (TopCamtg6SelParents),
    }
  },
  {
    .Id      = TOP_UART_SEL,
    .Name    = "TOP_UART_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0x70,
      .SetOffset    = 0x74,
      .ClearOffset  = 0x78,
      .UpdateOffset = 0x04,
      .MuxShift     = 8,
      .MuxWidth     = 1,
      .GateShift    = 15,
      .UpdateShift  = 25,
      .Parents      = TopUartSelParents,
      .ParentCount  = ARRAY_SIZE (TopUartSelParents),
    }
  },
  {
    .Id      = TOP_SPI_SEL,
    .Name    = "TOP_SPI_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0x70,
      .SetOffset    = 0x74,
      .ClearOffset  = 0x78,
      .UpdateOffset = 0x04,
      .MuxShift     = 16,
      .MuxWidth     = 3,
      .GateShift    = 23,
      .UpdateShift  = 26,
      .Parents      = TopSpiSelParents,
      .ParentCount  = ARRAY_SIZE (TopSpiSelParents),
    }
  },
  {
    .Id      = TOP_MSDC50_0_HCLK_SEL,
    .Name    = "TOP_MSDC50_0_HCLK_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0x70,
      .SetOffset    = 0x74,
      .ClearOffset  = 0x78,
      .UpdateOffset = 0x04,
      .MuxShift     = 24,
      .MuxWidth     = 2,
      .GateShift    = 31,
      .UpdateShift  = 27,
      .Parents      = TopMsdc50HclkSelParents,
      .ParentCount  = ARRAY_SIZE (TopMsdc50HclkSelParents),
    }
  },
  {
    .Id      = TOP_MSDC50_0_SEL,
    .Name    = "TOP_MSDC50_0_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0x80,
      .SetOffset    = 0x84,
      .ClearOffset  = 0x88,
      .UpdateOffset = 0x04,
      .MuxShift     = 0,
      .MuxWidth     = 3,
      .GateShift    = 7,
      .UpdateShift  = 28,
      .Parents      = TopMsdc50SelParents,
      .ParentCount  = ARRAY_SIZE (TopMsdc50SelParents),
    }
  },
  {
    .Id      = TOP_MSDC30_1_SEL,
    .Name    = "TOP_MSDC30_1_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0x80,
      .SetOffset    = 0x84,
      .ClearOffset  = 0x88,
      .UpdateOffset = 0x04,
      .MuxShift     = 8,
      .MuxWidth     = 3,
      .GateShift    = 15,
      .UpdateShift  = 29,
      .Parents      = TopMsdc30SelParents,
      .ParentCount  = ARRAY_SIZE (TopMsdc30SelParents),
    }
  },
  {
    .Id      = TOP_AUDIO_SEL,
    .Name    = "TOP_AUDIO_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0x80,
      .SetOffset    = 0x84,
      .ClearOffset  = 0x88,
      .UpdateOffset = 0x08,
      .MuxShift     = 24,
      .MuxWidth     = 2,
      .GateShift    = 31,
      .UpdateShift  = 0,
      .Parents      = TopAudioSelParents,
      .ParentCount  = ARRAY_SIZE (TopAudioSelParents),
    }
  },
  {
    .Id      = TOP_AUD_INTBUS_SEL,
    .Name    = "TOP_AUD_INTBUS_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0x90,
      .SetOffset    = 0x94,
      .ClearOffset  = 0x98,
      .UpdateOffset = 0x08,
      .MuxShift     = 0,
      .MuxWidth     = 2,
      .GateShift    = 7,
      .UpdateShift  = 1,
      .Parents      = TopAudIntbusSelParents,
      .ParentCount  = ARRAY_SIZE (TopAudIntbusSelParents),
    }
  },
  {
    .Id      = TOP_PWRAP_ULPOSC_SEL,
    .Name    = "TOP_PWRAP_ULPOSC_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0x90,
      .SetOffset    = 0x94,
      .ClearOffset  = 0x98,
      .UpdateOffset = 0x08,
      .MuxShift     = 8,
      .MuxWidth     = 3,
      .GateShift    = 15,
      .UpdateShift  = 2,
      .Parents      = TopPwrapUlposcSelParents,
      .ParentCount  = ARRAY_SIZE (TopPwrapUlposcSelParents),
    }
  },
  {
    .Id      = TOP_ATB_SEL,
    .Name    = "TOP_ATB_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0x90,
      .SetOffset    = 0x94,
      .ClearOffset  = 0x98,
      .UpdateOffset = 0x08,
      .MuxShift     = 16,
      .MuxWidth     = 2,
      .GateShift    = 23,
      .UpdateShift  = 3,
      .Parents      = TopAtbSelParents,
      .ParentCount  = ARRAY_SIZE (TopAtbSelParents),
    }
  },
  {
    .Id   = TOP_SSPM_SEL,
    .Name = "TOP_SSPM_SEL",
    .Controller = ClkTopCkGen,
    .Type = ClockTypeMux,
    .MuxGate  =
    {
      .MuxOffset    = 0x90,
      .SetOffset    = 0x94,
      .ClearOffset  = 0x98,
      .UpdateOffset = 0x08,
      .MuxShift     = 24,
      .MuxWidth     = 3,
      .UpdateShift  = 4,
      .Parents      = TopSspmSelParents,
      .ParentCount  = ARRAY_SIZE (TopSspmSelParents),
    }
  },
  {
    .Id      = TOP_SCAM_SEL,
    .Name    = "TOP_SCAM_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0xA0,
      .SetOffset    = 0xA4,
      .ClearOffset  = 0xA8,
      .UpdateOffset = 0x08,
      .MuxShift     = 8,
      .MuxWidth     = 1,
      .GateShift    = 15,
      .UpdateShift  = 6,
      .Parents      = TopScamSelParents,
      .ParentCount  = ARRAY_SIZE (TopScamSelParents),
    }
  },
  {
    .Id      = TOP_DISP_PWM_SEL,
    .Name    = "TOP_DISP_PWM_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0xA0,
      .SetOffset    = 0xA4,
      .ClearOffset  = 0xA8,
      .UpdateOffset = 0x08,
      .MuxShift     = 16,
      .MuxWidth     = 3,
      .GateShift    = 23,
      .UpdateShift  = 7,
      .Parents      = TopDispPwmSelParents,
      .ParentCount  = ARRAY_SIZE (TopDispPwmSelParents),
    }
  },
  {
    .Id      = TOP_USB_SEL,
    .Name    = "TOP_USB_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0xA0,
      .SetOffset    = 0xA4,
      .ClearOffset  = 0xA8,
      .UpdateOffset = 0x08,
      .MuxShift     = 24,
      .MuxWidth     = 2,
      .GateShift    = 31,
      .UpdateShift  = 8,
      .Parents      = TopUsbSelParents,
      .ParentCount  = ARRAY_SIZE (TopUsbSelParents),
    }
  },
  {
    .Id      = TOP_I2C_SEL,
    .Name    = "TOP_I2C_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0xB0,
      .SetOffset    = 0xB4,
      .ClearOffset  = 0xB8,
      .UpdateOffset = 0x08,
      .MuxShift     = 8,
      .MuxWidth     = 2,
      .GateShift    = 15,
      .UpdateShift  = 10,
      .Parents      = TopI2cSelParents,
      .ParentCount  = ARRAY_SIZE (TopI2cSelParents),
    }
  },
  {
    .Id      = TOP_SENINF_SEL,
    .Name    = "TOP_SENINF_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0xB0,
      .SetOffset    = 0xB4,
      .ClearOffset  = 0xB8,
      .UpdateOffset = 0x08,
      .MuxShift     = 16,
      .MuxWidth     = 3,
      .GateShift    = 23,
      .UpdateShift  = 11,
      .Parents      = TopSeninfSelParents,
      .ParentCount  = ARRAY_SIZE (TopSeninfSelParents),
    }
  },
  {
    .Id      = TOP_SENINF1_SEL,
    .Name    = "TOP_SENINF1_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0xB0,
      .SetOffset    = 0xB4,
      .ClearOffset  = 0xB8,
      .UpdateOffset = 0x08,
      .MuxShift     = 24,
      .MuxWidth     = 3,
      .GateShift    = 31,
      .UpdateShift  = 12,
      .Parents      = TopSeninf1SelParents,
      .ParentCount  = ARRAY_SIZE (TopSeninf1SelParents),
    }
  },
  {
    .Id      = TOP_SENINF2_SEL,
    .Name    = "TOP_SENINF2_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0xC0,
      .SetOffset    = 0xC4,
      .ClearOffset  = 0xC8,
      .UpdateOffset = 0x08,
      .MuxShift     = 0,
      .MuxWidth     = 3,
      .GateShift    = 7,
      .UpdateShift  = 13,
      .Parents      = TopSeninf2SelParents,
      .ParentCount  = ARRAY_SIZE (TopSeninf2SelParents),
    }
  },
  {
    .Id      = TOP_SENINF3_SEL,
    .Name    = "TOP_SENINF3_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0xC0,
      .SetOffset    = 0xC4,
      .ClearOffset  = 0xC8,
      .UpdateOffset = 0x08,
      .MuxShift     = 8,
      .MuxWidth     = 3,
      .GateShift    = 15,
      .UpdateShift  = 14,
      .Parents      = TopSeninf3SelParents,
      .ParentCount  = ARRAY_SIZE (TopSeninf3SelParents),
    }
  },
  {
    .Id   = TOP_DXCC_SEL,
    .Name = "TOP_DXCC_SEL",
    .Controller = ClkTopCkGen,
    .Type = ClockTypeMux,
    .MuxGate  =
    {
      .MuxOffset    = 0xC0,
      .SetOffset    = 0xC4,
      .ClearOffset  = 0xC8,
      .UpdateOffset = 0x08,
      .MuxShift     = 24,
      .MuxWidth     = 2,
      .UpdateShift  = 16,
      .Parents      = TopDxccSelParents,
      .ParentCount  = ARRAY_SIZE (TopDxccSelParents),
    }
  },
  {
    .Id      = TOP_AUD_ENGEN1_SEL,
    .Name    = "TOP_AUD_ENGEN1_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0xD0,
      .SetOffset    = 0xD4,
      .ClearOffset  = 0xD8,
      .UpdateOffset = 0x08,
      .MuxShift     = 0,
      .MuxWidth     = 2,
      .GateShift    = 7,
      .UpdateShift  = 17,
      .Parents      = TopAudEngen1SelParents,
      .ParentCount  = ARRAY_SIZE (TopAudEngen1SelParents),
    }
  },
  {
    .Id      = TOP_AUD_ENGEN2_SEL,
    .Name    = "TOP_AUD_ENGEN2_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0xD0,
      .SetOffset    = 0xD4,
      .ClearOffset  = 0xD8,
      .UpdateOffset = 0x08,
      .MuxShift     = 8,
      .MuxWidth     = 2,
      .GateShift    = 15,
      .UpdateShift  = 18,
      .Parents      = TopAudEngen2SelParents,
      .ParentCount  = ARRAY_SIZE (TopAudEngen2SelParents),
    }
  },
  {
    .Id      = TOP_AES_UFSFDE_SEL,
    .Name    = "TOP_AES_UFSFDE_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0xD0,
      .SetOffset    = 0xD4,
      .ClearOffset  = 0xD8,
      .UpdateOffset = 0x08,
      .MuxShift     = 16,
      .MuxWidth     = 3,
      .GateShift    = 23,
      .UpdateShift  = 19,
      .Parents      = TopAesUfsfdeSelParents,
      .ParentCount  = ARRAY_SIZE (TopAesUfsfdeSelParents),
    }
  },
  {
    .Id      = TOP_UFS_SEL,
    .Name    = "TOP_UFS_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0xD0,
      .SetOffset    = 0xD4,
      .ClearOffset  = 0xD8,
      .UpdateOffset = 0x08,
      .MuxShift     = 24,
      .MuxWidth     = 3,
      .GateShift    = 31,
      .UpdateShift  = 20,
      .Parents      = TopUfsSelParents,
      .ParentCount  = ARRAY_SIZE (TopUfsSelParents),
    }
  },
  {
    .Id      = TOP_AUD_1_SEL,
    .Name    = "TOP_AUD_1_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0xE0,
      .SetOffset    = 0xE4,
      .ClearOffset  = 0xE8,
      .UpdateOffset = 0x08,
      .MuxShift     = 0,
      .MuxWidth     = 1,
      .GateShift    = 7,
      .UpdateShift  = 21,
      .Parents      = TopAud1SelParents,
      .ParentCount  = ARRAY_SIZE (TopAud1SelParents),
    }
  },
  {
    .Id      = TOP_AUD_2_SEL,
    .Name    = "TOP_AUD_2_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0xE0,
      .SetOffset    = 0xE4,
      .ClearOffset  = 0xE8,
      .UpdateOffset = 0x08,
      .MuxShift     = 8,
      .MuxWidth     = 1,
      .GateShift    = 15,
      .UpdateShift  = 22,
      .Parents      = TopAud2SelParents,
      .ParentCount  = ARRAY_SIZE (TopAud2SelParents),
    }
  },
  {
    .Id      = TOP_DPMAIF_MAIN_SEL,
    .Name    = "TOP_DPMAIF_MAIN_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0xE0,
      .SetOffset    = 0xE4,
      .ClearOffset  = 0xE8,
      .UpdateOffset = 0x08,
      .MuxShift     = 24,
      .MuxWidth     = 3,
      .GateShift    = 31,
      .UpdateShift  = 24,
      .Parents      = TopDpmaifMainSelParents,
      .ParentCount  = ARRAY_SIZE (TopDpmaifMainSelParents),
    }
  },
  {
    .Id      = TOP_VENC_SEL,
    .Name    = "TOP_VENC_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0xF0,
      .SetOffset    = 0xF4,
      .ClearOffset  = 0xF8,
      .UpdateOffset = 0x08,
      .MuxShift     = 0,
      .MuxWidth     = 4,
      .GateShift    = 7,
      .UpdateShift  = 25,
      .Parents      = TopVencSelParents,
      .ParentCount  = ARRAY_SIZE (TopVencSelParents),
    }
  },
  {
    .Id      = TOP_VDEC_SEL,
    .Name    = "TOP_VDEC_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0xF0,
      .SetOffset    = 0xF4,
      .ClearOffset  = 0xF8,
      .UpdateOffset = 0x08,
      .MuxShift     = 8,
      .MuxWidth     = 4,
      .GateShift    = 15,
      .UpdateShift  = 26,
      .Parents      = TopVdecSelParents,
      .ParentCount  = ARRAY_SIZE (TopVdecSelParents),
    }
  },
  {
    .Id      = TOP_CAMTM_SEL,
    .Name    = "TOP_CAMTM_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0xF0,
      .SetOffset    = 0xF4,
      .ClearOffset  = 0xF8,
      .UpdateOffset = 0x08,
      .MuxShift     = 16,
      .MuxWidth     = 2,
      .GateShift    = 23,
      .UpdateShift  = 27,
      .Parents      = TopCamtmSelParents,
      .ParentCount  = ARRAY_SIZE (TopCamtmSelParents),
    }
  },
  {
    .Id      = TOP_PWM_SEL,
    .Name    = "TOP_PWM_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0xF0,
      .SetOffset    = 0xF4,
      .ClearOffset  = 0xF8,
      .UpdateOffset = 0x08,
      .MuxShift     = 24,
      .MuxWidth     = 1,
      .GateShift    = 31,
      .UpdateShift  = 28,
      .Parents      = TopPwmSelParents,
      .ParentCount  = ARRAY_SIZE (TopPwmSelParents),
    }
  },
  {
    .Id      = TOP_AUDIO_H_SEL,
    .Name    = "TOP_AUDIO_H_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0x100,
      .SetOffset    = 0x104,
      .ClearOffset  = 0x108,
      .UpdateOffset = 0x8,
      .MuxShift     = 0,
      .MuxWidth     = 2,
      .GateShift    = 7,
      .UpdateShift  = 29,
      .Parents      = TopAudioHSelParents,
      .ParentCount  = ARRAY_SIZE (TopAudioHSelParents),
    }
  },
  {
    .Id   = TOP_SPMI_MST_SEL,
    .Name = "TOP_SPMI_MST_SEL",
    .Controller = ClkTopCkGen,
    .Type = ClockTypeMux,
    .MuxGate  =
    {
      .MuxOffset    = 0x100,
      .SetOffset    = 0x104,
      .ClearOffset  = 0x108,
      .UpdateOffset = 0x8,
      .MuxShift     = 8,
      .MuxWidth     = 3,
      .UpdateShift  = 30,
      .Parents      = TopSpmiMstSelParents,
      .ParentCount  = ARRAY_SIZE (TopSpmiMstSelParents),
    }
  },
  {
    .Id   = TOP_DVFSRC_SEL,
    .Name = "TOP_DVFSRC_SEL",
    .Controller = ClkTopCkGen,
    .Type = ClockTypeMux,
    .MuxGate  =
    {
      .MuxOffset    = 0x100,
      .SetOffset    = 0x104,
      .ClearOffset  = 0x108,
      .UpdateOffset = 0xc,
      .MuxShift     = 16,
      .MuxWidth     = 1,
      .UpdateShift  = 0,
      .Parents      = TopDvfsrcSelParents,
      .ParentCount  = ARRAY_SIZE (TopDvfsrcSelParents),
    }
  },
  {
    .Id      = TOP_AES_MSDCFDE_SEL,
    .Name    = "TOP_AES_MSDCFDE_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0x100,
      .SetOffset    = 0x104,
      .ClearOffset  = 0x108,
      .UpdateOffset = 0xc,
      .MuxShift     = 24,
      .MuxWidth     = 3,
      .GateShift    = 31,
      .UpdateShift  = 1,
      .Parents      = TopAesMsdcfdeSelParents,
      .ParentCount  = ARRAY_SIZE (TopAesMsdcfdeSelParents),
    }
  },
  {
    .Id   = TOP_MCUPM_SEL,
    .Name = "TOP_MCUPM_SEL",
    .Controller = ClkTopCkGen,
    .Type = ClockTypeMux,
    .MuxGate  =
    {
      .MuxOffset    = 0x110,
      .SetOffset    = 0x114,
      .ClearOffset  = 0x118,
      .UpdateOffset = 0xc,
      .MuxShift     = 0,
      .MuxWidth     = 2,
      .UpdateShift  = 2,
      .Parents      = TopMcupmSelParents,
      .ParentCount  = ARRAY_SIZE (TopMcupmSelParents),
    }
  },
  {
    .Id      = TOP_DSI_OCC_SEL,
    .Name    = "TOP_DSI_OCC_SEL",
    .Controller = ClkTopCkGen,
    .Type    = ClockTypeMuxGate,
    .MuxGate =
    {
      .MuxOffset    = 0x110,
      .SetOffset    = 0x114,
      .ClearOffset  = 0x118,
      .UpdateOffset = 0xc,
      .MuxShift     = 16,
      .MuxWidth     = 2,
      .GateShift    = 23,
      .UpdateShift  = 4,
      .Parents      = TopDsiOccSelParents,
      .ParentCount  = ARRAY_SIZE (TopDsiOccSelParents),
    }
  },

  // Factors
  {
    .Id     = TOP_MFGPLL,
    .Name   = "TOP_MFGPLL",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_MFGPLL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_MAINPLL_D4,
    .Name   = "TOP_MAINPLL_D4",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_MAINPLL,
      .Mult   = 1,
      .Div    = 4,
    }
  },
  {
    .Id     = TOP_MAINPLL_D4_D2,
    .Name   = "TOP_MAINPLL_D4_D2",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_MAINPLL,
      .Mult   = 1,
      .Div    = 8,
    }
  },
  {
    .Id     = TOP_MAINPLL_D4_D4,
    .Name   = "TOP_MAINPLL_D4_D4",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_MAINPLL,
      .Mult   = 1,
      .Div    = 16,
    }
  },
  {
    .Id     = TOP_MAINPLL_D4_D8,
    .Name   = "TOP_MAINPLL_D4_D8",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_MAINPLL,
      .Mult   = 1,
      .Div    = 32,
    }
  },
  {
    .Id     = TOP_MAINPLL_D4_D16,
    .Name   = "TOP_MAINPLL_D4_D16",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_MAINPLL,
      .Mult   = 1,
      .Div    = 64,
    }
  },
  {
    .Id     = TOP_MAINPLL_D5,
    .Name   = "TOP_MAINPLL_D5",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_MAINPLL,
      .Mult   = 1,
      .Div    = 5,
    }
  },
  {
    .Id     = TOP_MAINPLL_D5_D2,
    .Name   = "TOP_MAINPLL_D5_D2",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_MAINPLL,
      .Mult   = 1,
      .Div    = 10,
    }
  },
  {
    .Id     = TOP_MAINPLL_D5_D4,
    .Name   = "TOP_MAINPLL_D5_D4",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_MAINPLL,
      .Mult   = 1,
      .Div    = 20,
    }
  },
  {
    .Id     = TOP_MAINPLL_D5_D8,
    .Name   = "TOP_MAINPLL_D5_D8",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_MAINPLL,
      .Mult   = 1,
      .Div    = 40,
    }
  },
  {
    .Id     = TOP_MAINPLL_D6,
    .Name   = "TOP_MAINPLL_D6",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_MAINPLL,
      .Mult   = 1,
      .Div    = 6,
    }
  },
  {
    .Id     = TOP_MAINPLL_D6_D2,
    .Name   = "TOP_MAINPLL_D6_D2",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_MAINPLL,
      .Mult   = 1,
      .Div    = 12,
    }
  },
  {
    .Id     = TOP_MAINPLL_D6_D4,
    .Name   = "TOP_MAINPLL_D6_D4",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_MAINPLL,
      .Mult   = 1,
      .Div    = 24,
    }
  },
  {
    .Id     = TOP_MAINPLL_D7,
    .Name   = "TOP_MAINPLL_D7",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_MAINPLL,
      .Mult   = 1,
      .Div    = 7,
    }
  },
  {
    .Id     = TOP_MAINPLL_D7_D2,
    .Name   = "TOP_MAINPLL_D7_D2",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_MAINPLL,
      .Mult   = 1,
      .Div    = 14,
    }
  },
  {
    .Id     = TOP_MAINPLL_D7_D4,
    .Name   = "TOP_MAINPLL_D7_D4",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_MAINPLL,
      .Mult   = 1,
      .Div    = 28,
    }
  },
  {
    .Id     = TOP_MAINPLL_D7_D8,
    .Name   = "TOP_MAINPLL_D7_D8",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_MAINPLL,
      .Mult   = 1,
      .Div    = 56,
    }
  },
  {
    .Id     = TOP_MAINPLL_D9,
    .Name   = "TOP_MAINPLL_D9",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_MAINPLL,
      .Mult   = 1,
      .Div    = 9,
    }
  },
  {
    .Id     = TOP_UNIVPLL_D4,
    .Name   = "TOP_UNIVPLL_D4",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_UNIVPLL,
      .Mult   = 1,
      .Div    = 4,
    }
  },
  {
    .Id     = TOP_UNIVPLL_D4_D2,
    .Name   = "TOP_UNIVPLL_D4_D2",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_UNIVPLL,
      .Mult   = 1,
      .Div    = 8,
    }
  },
  {
    .Id     = TOP_UNIVPLL_D4_D4,
    .Name   = "TOP_UNIVPLL_D4_D4",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_UNIVPLL,
      .Mult   = 1,
      .Div    = 16,
    }
  },
  {
    .Id     = TOP_UNIVPLL_D4_D8,
    .Name   = "TOP_UNIVPLL_D4_D8",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_UNIVPLL,
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
      .Parent = APMIXED_UNIVPLL,
      .Mult   = 1,
      .Div    = 5,
    }
  },
  {
    .Id     = TOP_UNIVPLL_D5_D2,
    .Name   = "TOP_UNIVPLL_D5_D2",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_UNIVPLL,
      .Mult   = 1,
      .Div    = 10,
    }
  },
  {
    .Id     = TOP_UNIVPLL_D5_D4,
    .Name   = "TOP_UNIVPLL_D5_D4",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_UNIVPLL,
      .Mult   = 1,
      .Div    = 20,
    }
  },
  {
    .Id     = TOP_UNIVPLL_D6,
    .Name   = "TOP_UNIVPLL_D6",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_UNIVPLL,
      .Mult   = 1,
      .Div    = 6,
    }
  },
  {
    .Id     = TOP_UNIVPLL_D6_D2,
    .Name   = "TOP_UNIVPLL_D6_D2",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_UNIVPLL,
      .Mult   = 1,
      .Div    = 12,
    }
  },
  {
    .Id     = TOP_UNIVPLL_D6_D4,
    .Name   = "TOP_UNIVPLL_D6_D4",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_UNIVPLL,
      .Mult   = 1,
      .Div    = 24,
    }
  },
  {
    .Id     = TOP_UNIVPLL_D6_D8,
    .Name   = "TOP_UNIVPLL_D6_D8",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_UNIVPLL,
      .Mult   = 1,
      .Div    = 48,
    }
  },
  {
    .Id     = TOP_UNIVPLL_D6_D16,
    .Name   = "TOP_UNIVPLL_D6_D16",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_UNIVPLL,
      .Mult   = 1,
      .Div    = 96,
    }
  },
  {
    .Id     = TOP_UNIVPLL_D7,
    .Name   = "TOP_UNIVPLL_D7",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_UNIVPLL,
      .Mult   = 1,
      .Div    = 7,
    }
  },
  {
    .Id     = TOP_UNIVPLL_192M_D2,
    .Name   = "TOP_UNIVPLL_192M_D2",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_UNIVPLL,
      .Mult   = 1,
      .Div    = 26,
    }
  },
  {
    .Id     = TOP_UNIVPLL_192M_D4,
    .Name   = "TOP_UNIVPLL_192M_D4",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_UNIVPLL,
      .Mult   = 1,
      .Div    = 52,
    }
  },
  {
    .Id     = TOP_UNIVPLL_192M_D8,
    .Name   = "TOP_UNIVPLL_192M_D8",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_UNIVPLL,
      .Mult   = 1,
      .Div    = 104,
    }
  },
  {
    .Id     = TOP_UNIVPLL_192M_D16,
    .Name   = "TOP_UNIVPLL_192M_D16",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_UNIVPLL,
      .Mult   = 1,
      .Div    = 208,
    }
  },
  {
    .Id     = TOP_UNIVPLL_192M_D32,
    .Name   = "TOP_UNIVPLL_192M_D32",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_UNIVPLL,
      .Mult   = 1,
      .Div    = 416,
    }
  },
  {
    .Id     = TOP_APLL1,
    .Name   = "TOP_APLL1",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_APLL1,
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
      .Parent = APMIXED_APLL1,
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
      .Parent = APMIXED_APLL1,
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
      .Parent = APMIXED_APLL1,
      .Mult   = 1,
      .Div    = 8,
    }
  },
  {
    .Id     = TOP_APLL2,
    .Name   = "TOP_APLL2",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_APLL2,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_APLL2_D2,
    .Name   = "TOP_APLL2_D2",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_APLL2,
      .Mult   = 1,
      .Div    = 2,
    }
  },
  {
    .Id     = TOP_APLL2_D4,
    .Name   = "TOP_APLL2_D4",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_APLL2,
      .Mult   = 1,
      .Div    = 4,
    }
  },
  {
    .Id     = TOP_APLL2_D8,
    .Name   = "TOP_APLL2_D8",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_APLL2,
      .Mult   = 1,
      .Div    = 8,
    }
  },
  {
    .Id     = TOP_MMPLL_D4_D2,
    .Name   = "TOP_MMPLL_D4_D2",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_MMPLL,
      .Mult   = 1,
      .Div    = 8,
    }
  },
  {
    .Id     = TOP_MMPLL_D5_D2,
    .Name   = "TOP_MMPLL_D5_D2",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_MMPLL,
      .Mult   = 1,
      .Div    = 12,
    }
  },
  {
    .Id     = TOP_MMPLL_D6,
    .Name   = "TOP_MMPLL_D6",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_MMPLL,
      .Mult   = 1,
      .Div    = 6,
    }
  },
  {
    .Id     = TOP_MMPLL_D6_D2,
    .Name   = "TOP_MMPLL_D6_D2",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_MMPLL,
      .Mult   = 1,
      .Div    = 12,
    }
  },
  {
    .Id     = TOP_MMPLL_D7,
    .Name   = "TOP_MMPLL_D7",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_MMPLL,
      .Mult   = 1,
      .Div    = 7,
    }
  },
  {
    .Id     = TOP_MMPLL_D9,
    .Name   = "TOP_MMPLL_D9",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_MMPLL,
      .Mult   = 1,
      .Div    = 9,
    }
  },
  {
    .Id     = TOP_NPUPLL,
    .Name   = "TOP_NPUPLL",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_NPUPLL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_TVDPLL,
    .Name   = "TOP_TVDPLL",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_TVDPLL,
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
      .Parent = APMIXED_MSDCPLL,
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
      .Parent = APMIXED_MSDCPLL,
      .Mult   = 1,
      .Div    = 2,
    }
  },
  {
    .Id     = TOP_MSDCPLL_D4,
    .Name   = "TOP_MSDCPLL_D4",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = APMIXED_MSDCPLL,
      .Mult   = 1,
      .Div    = 4,
    }
  },
  {
    .Id     = TOP_CLKRTC,
    .Name   = "TOP_CLKRTC",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_CLK32K,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_TCK_26M_MX9,
    .Name   = "TOP_TCK_26M_MX9",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_CLK26M,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_F26M_CK_D2,
    .Name   = "TOP_F26M_CK_D2",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_CLK26M,
      .Mult   = 1,
      .Div    = 2,
    }
  },
  {
    .Id     = TOP_OSC_D2,
    .Name   = "TOP_OSC_D2",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_ULPOSC,
      .Mult   = 1,
      .Div    = 2,
    }
  },
  {
    .Id     = TOP_OSC_D4,
    .Name   = "TOP_OSC_D4",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_ULPOSC,
      .Mult   = 1,
      .Div    = 4,
    }
  },
  {
    .Id     = TOP_OSC_D8,
    .Name   = "TOP_OSC_D8",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_ULPOSC,
      .Mult   = 1,
      .Div    = 8,
    }
  },
  {
    .Id     = TOP_OSC_D16,
    .Name   = "TOP_OSC_D16",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_ULPOSC,
      .Mult   = 1,
      .Div    = 16,
    }
  },
  {
    .Id     = TOP_OSC_D10,
    .Name   = "TOP_OSC_D10",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_ULPOSC,
      .Mult   = 1,
      .Div    = 10,
    }
  },
  {
    .Id     = TOP_OSC_D20,
    .Name   = "TOP_OSC_D20",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_ULPOSC,
      .Mult   = 1,
      .Div    = 20,
    }
  },
  {
    .Id     = TOP_F26M,
    .Name   = "TOP_F26M",
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
    .Id     = TOP_DISP,
    .Name   = "TOP_DISP",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_DISP_SEL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_MDP,
    .Name   = "TOP_MDP",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_MDP_SEL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_IMG1,
    .Name   = "TOP_IMG1",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_IMG1_SEL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_IPE,
    .Name   = "TOP_IPE",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_IPE_SEL,
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
    .Id     = TOP_MFG_REF,
    .Name   = "TOP_MFG_REF",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_MFG_REF_SEL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_MFG_PLL,
    .Name   = "TOP_MFG_PLL",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_MFG_PLL_SEL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_UART,
    .Name   = "TOP_UART",
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
    .Id     = TOP_PWRAP_ULPOSC,
    .Name   = "TOP_PWRAP_ULPOSC",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_PWRAP_ULPOSC_SEL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_DISP_PWM,
    .Name   = "TOP_DISP_PWM",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_DISP_PWM_SEL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_USB,
    .Name   = "TOP_USB",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_USB_SEL,
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
    .Id     = TOP_AUD_ENGEN2,
    .Name   = "TOP_AUD_ENGEN2",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_AUD_ENGEN2_SEL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_AES_UFSFDE,
    .Name   = "TOP_AES_UFSFDE",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_AES_UFSFDE_SEL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_UFS,
    .Name   = "TOP_UFS",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_UFS_SEL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_DPMAIF_MAIN,
    .Name   = "TOP_DPMAIF_MAIN",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_DPMAIF_MAIN_SEL,
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
    .Id     = TOP_VDEC,
    .Name   = "TOP_VDEC",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_VDEC_SEL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_CAMTM,
    .Name   = "TOP_CAMTM",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_CAMTM_SEL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_PWM,
    .Name   = "TOP_PWM",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_PWM_SEL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_AUDIO_H,
    .Name   = "TOP_AUDIO_H",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_AUDIO_H_SEL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_DSI_OCC,
    .Name   = "TOP_DSI_OCC",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = TOP_DSI_OCC_SEL,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_I2C_PSEUDO,
    .Name   = "TOP_I2C_PSEUDO",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = IFRAO_I2C_PSEUDO,
      .Mult   = 1,
      .Div    = 1,
    }
  },
  {
    .Id     = TOP_APDMA,
    .Name   = "TOP_APDMA",
    .Type   = ClockTypeFactors,
    .Factor =
    {
      .Parent = IFRAO_APDMA_PSEUDO,
      .Mult   = 1,
      .Div    = 1,
    }
  },

  // Gates
  {
    .Id   = IFRAO_PMIC_TMR,
    .Name = "IFRAO_PMIC_TMR",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 0,
      .Parent       = TOP_PWRAP_ULPOSC,
    }
  },
  {
    .Id   = IFRAO_PMIC_AP,
    .Name = "IFRAO_PMIC_AP",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 1,
      .Parent       = TOP_PWRAP_ULPOSC,
    }
  },
  {
    .Id   = IFRAO_GCE,
    .Name = "IFRAO_GCE",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 8,
      .Parent       = TOP_AXI,
    }
  },
  {
    .Id   = IFRAO_GCE2,
    .Name = "IFRAO_GCE2",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 9,
      .Parent       = TOP_AXI,
    }
  },
  {
    .Id   = IFRAO_THERM,
    .Name = "IFRAO_THERM",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 10,
      .Parent       = TOP_AXI,
    }
  },
  {
    .Id   = IFRAO_I2C_PSEUDO,
    .Name = "IFRAO_I2C_PSEUDO",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 11,
      .Parent       = TOP_I2C,
    }
  },
  {
    .Id   = IFRAO_APDMA_PSEUDO,
    .Name = "IFRAO_APDMA_PSEUDO",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 12,
      .Parent       = TOP_AXI,
    }
  },
  {
    .Id   = IFRAO_PWM_HCLK,
    .Name = "IFRAO_PWM_HCLK",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 15,
      .Parent       = TOP_AXI,
    }
  },
  {
    .Id   = IFRAO_PWM1,
    .Name = "IFRAO_PWM1",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 16,
      .Parent       = TOP_PWM,
    }
  },
  {
    .Id   = IFRAO_PWM2,
    .Name = "IFRAO_PWM2",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 17,
      .Parent       = TOP_PWM,
    }
  },
  {
    .Id   = IFRAO_PWM3,
    .Name = "IFRAO_PWM3",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 18,
      .Parent       = TOP_PWM,
    }
  },
  {
    .Id   = IFRAO_PWM4,
    .Name = "IFRAO_PWM4",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 19,
      .Parent       = TOP_PWM,
    }
  },
  {
    .Id   = IFRAO_PWM,
    .Name = "IFRAO_PWM",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 21,
      .Parent       = TOP_PWM,
    }
  },
  {
    .Id   = IFRAO_UART0,
    .Name = "IFRAO_UART0",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 22,
      .Parent       = TOP_UART,
    }
  },
  {
    .Id   = IFRAO_UART1,
    .Name = "IFRAO_UART1",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 23,
      .Parent       = TOP_UART,
    }
  },
  {
    .Id   = IFRAO_UART2,
    .Name = "IFRAO_UART2",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 24,
      .Parent       = TOP_UART,
    }
  },
  {
    .Id   = IFRAO_UART3,
    .Name = "IFRAO_UART3",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 25,
      .Parent       = TOP_UART,
    }
  },
  {
    .Id   = IFRAO_GCE_26M,
    .Name = "IFRAO_GCE_26M",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 27,
      .Parent       = TOP_AXI,
    }
  },
  {
    .Id   = IFRAO_BTIF,
    .Name = "IFRAO_BTIF",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0x80,
      .ClearOffset  = 0x84,
      .StatusOffset = 0x90,
      .GateShift    = 31,
      .Parent       = TOP_AXI,
    }
  },
  {
    .Id   = IFRAO_SPI0,
    .Name = "IFRAO_SPI0",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0x88,
      .ClearOffset  = 0x8C,
      .StatusOffset = 0x94,
      .GateShift    = 1,
      .Parent       = TOP_SPI,
    }
  },
  {
    .Id   = IFRAO_MSDC0,
    .Name = "IFRAO_MSDC0",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0x88,
      .ClearOffset  = 0x8C,
      .StatusOffset = 0x94,
      .GateShift    = 2,
      .Parent       = TOP_AXI,
    }
  },
  {
    .Id   = IFRAO_MSDC1,
    .Name = "IFRAO_MSDC1",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0x88,
      .ClearOffset  = 0x8C,
      .StatusOffset = 0x94,
      .GateShift    = 4,
      .Parent       = TOP_AXI,
    }
  },
  {
    .Id   = IFRAO_MSDC0_SRC,
    .Name = "IFRAO_MSDC0_SRC",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0x88,
      .ClearOffset  = 0x8C,
      .StatusOffset = 0x94,
      .GateShift    = 6,
      .Parent       = TOP_MSDC50_0,
    }
  },
  {
    .Id   = IFRAO_AUXADC,
    .Name = "IFRAO_AUXADC",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0x88,
      .ClearOffset  = 0x8C,
      .StatusOffset = 0x94,
      .GateShift    = 10,
      .Parent       = TOP_F26M,
    }
  },
  {
    .Id   = IFRAO_CPUM,
    .Name = "IFRAO_CPUM",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0x88,
      .ClearOffset  = 0x8C,
      .StatusOffset = 0x94,
      .GateShift    = 11,
      .Parent       = TOP_AXI,
    }
  },
  {
    .Id   = IFRAO_CCIF1_AP,
    .Name = "IFRAO_CCIF1_AP",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0x88,
      .ClearOffset  = 0x8C,
      .StatusOffset = 0x94,
      .GateShift    = 12,
      .Parent       = TOP_AXI,
    }
  },
  {
    .Id   = IFRAO_CCIF1_MD,
    .Name = "IFRAO_CCIF1_MD",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0x88,
      .ClearOffset  = 0x8C,
      .StatusOffset = 0x94,
      .GateShift    = 13,
      .Parent       = TOP_AXI,
    }
  },
  {
    .Id   = IFRAO_AUXADC_MD,
    .Name = "IFRAO_AUXADC_MD",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0x88,
      .ClearOffset  = 0x8C,
      .StatusOffset = 0x94,
      .GateShift    = 14,
      .Parent       = TOP_F26M,
    }
  },
  {
    .Id   = IFRAO_MSDC1_SRC,
    .Name = "IFRAO_MSDC1_SRC",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0x88,
      .ClearOffset  = 0x8C,
      .StatusOffset = 0x94,
      .GateShift    = 16,
      .Parent       = TOP_MSDC30_1,
    }
  },
  {
    .Id   = IFRAO_MSDC0_AES,
    .Name = "IFRAO_MSDC0_AES",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0x88,
      .ClearOffset  = 0x8C,
      .StatusOffset = 0x94,
      .GateShift    = 17,
      .Parent       = TOP_MSDC50_0,
    }
  },
  {
    .Id   = IFRAO_CCIF_AP,
    .Name = "IFRAO_CCIF_AP",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0x88,
      .ClearOffset  = 0x8C,
      .StatusOffset = 0x94,
      .GateShift    = 23,
      .Parent       = TOP_AXI,
    }
  },
  {
    .Id   = IFRAO_DEBUGSYS,
    .Name = "IFRAO_DEBUGSYS",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0x88,
      .ClearOffset  = 0x8C,
      .StatusOffset = 0x94,
      .GateShift    = 24,
      .Parent       = TOP_AXI,
    }
  },
  {
    .Id   = IFRAO_AUDIO,
    .Name = "IFRAO_AUDIO",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0x88,
      .ClearOffset  = 0x8C,
      .StatusOffset = 0x94,
      .GateShift    = 25,
      .Parent       = TOP_AXI,
    }
  },
  {
    .Id   = IFRAO_CCIF_MD,
    .Name = "IFRAO_CCIF_MD",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0x88,
      .ClearOffset  = 0x8C,
      .StatusOffset = 0x94,
      .GateShift    = 26,
      .Parent       = TOP_AXI,
    }
  },
  {
    .Id   = IFRAO_SSUSB,
    .Name = "IFRAO_SSUSB",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0xA4,
      .ClearOffset  = 0xA8,
      .StatusOffset = 0xAC,
      .GateShift    = 1,
      .Parent       = TOP_USB,
    }
  },
  {
    .Id   = IFRAO_CLDMA_BCLK,
    .Name = "IFRAO_CLDMA_BCLK",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0xA4,
      .ClearOffset  = 0xA8,
      .StatusOffset = 0xAC,
      .GateShift    = 3,
      .Parent       = TOP_AXI,
    }
  },
  {
    .Id   = IFRAO_AUDIO_26M_BCLK,
    .Name = "IFRAO_AUDIO_26M_BCLK",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0xA4,
      .ClearOffset  = 0xA8,
      .StatusOffset = 0xAC,
      .GateShift    = 4,
      .Parent       = TOP_F26M,
    }
  },
  {
    .Id   = IFRAO_SPI1,
    .Name = "IFRAO_SPI1",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0xA4,
      .ClearOffset  = 0xA8,
      .StatusOffset = 0xAC,
      .GateShift    = 6,
      .Parent       = TOP_SPI,
    }
  },
  {
    .Id   = IFRAO_SPI2,
    .Name = "IFRAO_SPI2",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0xA4,
      .ClearOffset  = 0xA8,
      .StatusOffset = 0xAC,
      .GateShift    = 9,
      .Parent       = TOP_SPI,
    }
  },
  {
    .Id   = IFRAO_SPI3,
    .Name = "IFRAO_SPI3",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0xA4,
      .ClearOffset  = 0xA8,
      .StatusOffset = 0xAC,
      .GateShift    = 10,
      .Parent       = TOP_SPI,
    }
  },
  {
    .Id   = IFRAO_UNIPRO_SYSCLK,
    .Name = "IFRAO_UNIPRO_SYSCLK",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0xA4,
      .ClearOffset  = 0xA8,
      .StatusOffset = 0xAC,
      .GateShift    = 11,
      .Parent       = TOP_UFS,
    }
  },
  {
    .Id   = IFRAO_UNIPRO_TICK,
    .Name = "IFRAO_UNIPRO_TICK",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0xA4,
      .ClearOffset  = 0xA8,
      .StatusOffset = 0xAC,
      .GateShift    = 12,
      .Parent       = TOP_F26M,
    }
  },
  {
    .Id   = IFRAO_UFS_SAP_BCLK,
    .Name = "IFRAO_UFS_SAP_BCLK",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0xA4,
      .ClearOffset  = 0xA8,
      .StatusOffset = 0xAC,
      .GateShift    = 13,
      .Parent       = TOP_F26M,
    }
  },
  {
    .Id   = IFRAO_SPI4,
    .Name = "IFRAO_SPI4",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0xA4,
      .ClearOffset  = 0xA8,
      .StatusOffset = 0xAC,
      .GateShift    = 25,
      .Parent       = TOP_SPI,
    }
  },
  {
    .Id   = IFRAO_SPI5,
    .Name = "IFRAO_SPI5",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0xA4,
      .ClearOffset  = 0xA8,
      .StatusOffset = 0xAC,
      .GateShift    = 26,
      .Parent       = TOP_SPI,
    }
  },
  {
    .Id   = IFRAO_CQ_DMA,
    .Name = "IFRAO_CQ_DMA",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0xA4,
      .ClearOffset  = 0xA8,
      .StatusOffset = 0xAC,
      .GateShift    = 27,
      .Parent       = TOP_AXI,
    }
  },
  {
    .Id   = IFRAO_UFS,
    .Name = "IFRAO_UFS",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0xA4,
      .ClearOffset  = 0xA8,
      .StatusOffset = 0xAC,
      .GateShift    = 28,
      .Parent       = TOP_UFS,
    }
  },
  {
    .Id   = IFRAO_UFS_AES,
    .Name = "IFRAO_UFS_AES",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0xA4,
      .ClearOffset  = 0xA8,
      .StatusOffset = 0xAC,
      .GateShift    = 29,
      .Parent       = TOP_AES_UFSFDE,
    }
  },
  {
    .Id   = IFRAO_AP_MSDC0,
    .Name = "IFRAO_AP_MSDC0",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0xC0,
      .ClearOffset  = 0xC4,
      .StatusOffset = 0xC8,
      .GateShift    = 7,
      .Parent       = TOP_MSDC50_0,
    }
  },
  {
    .Id   = IFRAO_MD_MSDC0,
    .Name = "IFRAO_MD_MSDC0",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0xC0,
      .ClearOffset  = 0xC4,
      .StatusOffset = 0xC8,
      .GateShift    = 8,
      .Parent       = TOP_MSDC50_0,
    }
  },
  {
    .Id   = IFRAO_CCIF5_MD,
    .Name = "IFRAO_CCIF5_MD",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0xC0,
      .ClearOffset  = 0xC4,
      .StatusOffset = 0xC8,
      .GateShift    = 10,
      .Parent       = TOP_AXI,
    }
  },
  {
    .Id   = IFRAO_CCIF2_AP,
    .Name = "IFRAO_CCIF2_AP",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0xC0,
      .ClearOffset  = 0xC4,
      .StatusOffset = 0xC8,
      .GateShift    = 16,
      .Parent       = TOP_AXI,
    }
  },
  {
    .Id   = IFRAO_CCIF2_MD,
    .Name = "IFRAO_CCIF2_MD",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0xC0,
      .ClearOffset  = 0xC4,
      .StatusOffset = 0xC8,
      .GateShift    = 17,
      .Parent       = TOP_AXI,
    }
  },
  {
    .Id   = IFRAO_FBIST2FPC,
    .Name = "IFRAO_FBIST2FPC",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0xC0,
      .ClearOffset  = 0xC4,
      .StatusOffset = 0xC8,
      .GateShift    = 24,
      .Parent       = TOP_MSDC50_0,
    }
  },
  {
    .Id   = IFRAO_DPMAIF_MAIN,
    .Name = "IFRAO_DPMAIF_MAIN",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0xC0,
      .ClearOffset  = 0xC4,
      .StatusOffset = 0xC8,
      .GateShift    = 26,
      .Parent       = TOP_DPMAIF_MAIN,
    }
  },
  {
    .Id   = IFRAO_CCIF4_AP,
    .Name = "IFRAO_CCIF4_AP",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0xC0,
      .ClearOffset  = 0xC4,
      .StatusOffset = 0xC8,
      .GateShift    = 28,
      .Parent       = TOP_AXI,
    }
  },
  {
    .Id   = IFRAO_CCIF4_MD,
    .Name = "IFRAO_CCIF4_MD",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0xC0,
      .ClearOffset  = 0xC4,
      .StatusOffset = 0xC8,
      .GateShift    = 29,
      .Parent       = TOP_AXI,
    }
  },
  {
    .Id   = IFRAO_SPI6_CK,
    .Name = "IFRAO_SPI6_CK",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0xC0,
      .ClearOffset  = 0xC4,
      .StatusOffset = 0xC8,
      .GateShift    = 30,
      .Parent       = TOP_SPI,
    }
  },
  {
    .Id   = IFRAO_SPI7_CK,
    .Name = "IFRAO_SPI7_CK",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0xC0,
      .ClearOffset  = 0xC4,
      .StatusOffset = 0xC8,
      .GateShift    = 31,
      .Parent       = TOP_SPI,
    }
  },
  {
    .Id   = IFRAO_66MP_BUS_MCLK_CKP,
    .Name = "IFRAO_66MP_BUS_MCLK_CKP",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0xE0,
      .ClearOffset  = 0xE4,
      .StatusOffset = 0xE8,
      .GateShift    = 2,
      .Parent       = TOP_AXI,
    }
  },
  {
    .Id   = IFRAO_AP_DMA,
    .Name = "IFRAO_AP_DMA",
    .Controller = ClkInfraCfgAo,
    .Type = ClockTypeGate,
    .Gate =
    {
      .SetOffset    = 0xd0,
      .ClearOffset  = 0xd4,
      .StatusOffset = 0xd8,
      .GateShift    = 31,
      .Parent       = TOP_APDMA,
    }
  },
};

UINTN gClockCount = ARRAY_SIZE (gClocks);
