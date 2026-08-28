#ifndef _EXYNOS9830_USB_H_
#define _EXYNOS9830_USB_H_

//
// USBCON Registers, relative to EXYNOS9830_USBDRD_CTRL_BASE.
//
#define EXYNOS_USBCON_CTRL_VER          0x00
#define EXYNOS_USBCON_LINK_CTRL         0x04
#define EXYNOS_USBCON_LINK_PORT         0x08
#define EXYNOS_USBCON_CLKRST            0x20
#define EXYNOS_USBCON_PWR               0x24
#define EXYNOS_USBCON_DUALPHYSEL        0x28
#define EXYNOS_USBCON_COMBO_PMA_CTRL    0x48
#define EXYNOS_USBCON_UTMI              0x50
#define EXYNOS_USBCON_HSP               0x54
#define EXYNOS_USBCON_HSP_TUNE          0x58
#define EXYNOS_USBCON_HSP_TEST          0x5C

//
// LINK_CTRL
//
#define LINKCTRL_PIPE3_FORCE_RX_ELEC_IDLE   BIT18
#define LINKCTRL_PIPE3_FORCE_PHY_STATUS     BIT17
#define LINKCTRL_PIPE3_FORCE_EN             BIT16
#define LINKCTRL_DIS_QACT_BUSPEND           BIT13
#define LINKCTRL_DIS_QACT_LINKGATE          BIT12
#define LINKCTRL_DIS_QACT_ID0               BIT11
#define LINKCTRL_DIS_QACT_VBUS_VALID        BIT10
#define LINKCTRL_DIS_QACT_BVALID            BIT9
#define LINKCTRL_FORCE_QACT                 BIT8
#define LINKCTRL_BUS_FILTER_BYPASS(x)       (((x) & 0xF) << 4)
#define LINKCTRL_BUS_FILTER_BYPASS_MASK     (0xF << 4)

//
// LINK_PORT
//
#define LINKPORT_HUB_PORT_SEL_OCD_U3        BIT3
#define LINKPORT_HUB_PORT_SEL_OCD_U2        BIT2

//
// CLKRST
//
#define CLKRST_PHY20_SW_RST                 BIT13
#define CLKRST_PHY20_RST_SEL                BIT12
#define CLKRST_LINK_PCLK_SEL                BIT7
#define CLKRST_PHYCLOCKSEL                  BIT6
#define CLKRST_REFCLK_SEL                   BIT4
#define CLKRST_PHY30_SW_RST                 BIT3
#define CLKRST_PHY30_RST_SEL                BIT2
#define CLKRST_PORT_RST                     BIT1
#define CLKRST_LINK_SW_RST                  BIT0

//
// PWR
//
#define PWR_PIPE3_POWERDOWN                 (0xF << 4)
#define PWR_FORCE_POWERDOWN_EN              BIT3
#define PWR_TEST_POWERDOWN_SSP              BIT1
#define PWR_TEST_POWERDOWN_HSP              BIT0

//
// COMBO_PMA_CTRL
//
#define PMA_LOW_PWR                         BIT4
#define PMA_TRSV_SW_RST                     BIT3
#define PMA_CMN_SW_RST                      BIT2
#define PMA_INIT_SW_RST                     BIT1
#define PMA_APB_SW_RST                      BIT0

//
// UTMI
//
#define UTMI_OPMODE_CTRL_EN                 BIT8
#define UTMI_FORCE_OPMODE(x)                (((x) & 0x3) << 6)
#define UTMI_FORCE_VBUSVALID                BIT5
#define UTMI_FORCE_BVALID                   BIT4
#define UTMI_DP_PULLDOWN                    BIT3
#define UTMI_DM_PULLDOWN                    BIT2
#define UTMI_FORCE_SUSPEND                  BIT1
#define UTMI_FORCE_SLEEP                    BIT0

//
// HSP
//
#define HSP_VBUSVLDEXTSEL                   BIT13
#define HSP_VBUSVLDEXT                      BIT12
#define HSP_EN_UTMISUSPEND                  BIT9
#define HSP_COMMONONN                       BIT8
#define HSP_HS_SQUELCH                      BIT20
#define HSP_FSVMINUS                        BIT17
#define HSP_FSVPLUS                         BIT16

//
// HSP_TUNE
//
#define HSP_TUNE_TXVREF_MASK                (0xFU << 28)
#define HSP_TUNE_TXVREF(x)                  (((UINT32)(x) & 0xF) << 28)
#define HSP_TUNE_TXRISE_MASK                (0x3 << 24)
#define HSP_TUNE_TXRISE(x)                  (((x) & 0x3) << 24)
#define HSP_TUNE_TXRES_MASK                 (0x3 << 21)
#define HSP_TUNE_TXRES(x)                   (((x) & 0x3) << 21)
#define HSP_TUNE_TXPREEMPA_PLUS             BIT20
#define HSP_TUNE_TXPREEMPA_MASK             (0x3 << 18)
#define HSP_TUNE_TXPREEMPA(x)               (((x) & 0x3) << 18)
#define HSP_TUNE_HSXV_MASK                  (0x3 << 16)
#define HSP_TUNE_HSXV(x)                    (((x) & 0x3) << 16)
#define HSP_TUNE_TXFSLS_MASK                (0xF << 12)
#define HSP_TUNE_TXFSLS(x)                  (((x) & 0xF) << 12)
#define HSP_TUNE_SQRX_MASK                  (0x7 << 8)
#define HSP_TUNE_SQRX(x)                    (((x) & 0x7) << 8)
#define HSP_TUNE_OTG_MASK                   (0x7 << 4)
#define HSP_TUNE_OTG(x)                     (((x) & 0x7) << 4)
#define HSP_TUNE_COMPDIS_MASK               (0x7 << 0)
#define HSP_TUNE_COMPDIS(x)                 (((x) & 0x7) << 0)

#define HSP_TEST_SIDDQ                      BIT24
#define HSP_TEST_LINESTATE_MASK             (0x3 << 20)
#define HSP_TEST_LINESTATE(x)               (((x) & (0x3 << 20)) >> 20)

#define EXYNOS9830_TUNE_TX_PRE_EMP          0x3
#define EXYNOS9830_TUNE_TX_VREF             0xF
#define EXYNOS9830_TUNE_RX_SQRX             0x7
#define EXYNOS9830_TUNE_COMPDIS             0x7
#define EXYNOS9830_TUNE_TX_RES              0x3

#endif /* _EXYNOS9830_USB_H_ */
