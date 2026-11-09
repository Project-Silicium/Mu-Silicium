#ifndef _EXYNOS7885_USB_H_
#define _EXYNOS7885_USB_H_

//
// CLK
//
#define EXYNOS7885_CMU_FSYS_BASE            0x13400000

//
// PMU
//
#define EXYNOS7885_PMU_BASE             0x11C80000
#define EXYNOS7885_PMU_USBDRD_PHY_CTRL  (EXYNOS7885_PMU_BASE + 0x0674)
#define EXYNOS7885_PMU_PHY_ENABLE       BIT0

//
// PHY and Link
//
#define EXYNOS7885_USBDRD_LINK_BASE     0x13600000
#define EXYNOS7885_USBDRD_CTRL_BASE     0x135D0000

//
// USBCON Registers.
//
#define EXYNOS_USBCON_CTRL_VER          0x00
#define EXYNOS_USBCON_LINK_CTRL         0x04
#define EXYNOS_USBCON_LINK_PORT         0x08
#define EXYNOS_USBCON_CLKRST            0x20
#define EXYNOS_USBCON_SSPPLLCTL         0x30
#define EXYNOS_USBCON_COMBO_PMA_CTRL    0x48
#define EXYNOS_USBCON_UTMI              0x50
#define EXYNOS_USBCON_HSP               0x54
#define EXYNOS_USBCON_HSP_TUNE          0x58
#define EXYNOS_USBCON_HSP_TEST          0x5C

//
// LINK_CTRL
//
#define LINKCTRL_FORCE_RXELECIDLE           BIT18
#define LINKCTRL_FORCE_PHYSTATUS            BIT17
#define LINKCTRL_FORCE_PIPE_EN              BIT16
#define LINKCTRL_FORCE_QACT                 BIT8
#define LINKCTRL_BUS_FILTER_BYPASS(x)       (((x) & 0xF) << 4)

//
// LINK_PORT
//
#define LINKPORT_HOST_NUM_U3_SHIFT          16
#define LINKPORT_HOST_NUM_U3_MASK           (0xFU << LINKPORT_HOST_NUM_U3_SHIFT)

//
// CLKRST
//
#define CLKRST_PHY20_SW_POR                 BIT13
#define CLKRST_PHY20_SW_POR_SEL             BIT12
#define CLKRST_PHY_SW_RST                   BIT3
#define CLKRST_PHY_RESET_SEL                BIT2
#define CLKRST_PORT_RST                     BIT1
#define CLKRST_LINK_SW_RST                  BIT0

//
// SSPPLLCTL
//
#define SSPPLLCTL_FSEL_MASK                 0x7
#define SSPPLLCTL_FSEL_50MHZ                7

//
// COMBO_PMA_CTRL (SECPMACTL)
//
#define SECPMACTL_PMA_LOW_PWR               BIT4

//
// UTMI
//
#define UTMI_FORCE_VBUSVALID                BIT5
#define UTMI_FORCE_BVALID                   BIT4
#define UTMI_DP_PULLDOWN                    BIT3
#define UTMI_DM_PULLDOWN                    BIT2
#define UTMI_FORCE_SUSPEND                  BIT1
#define UTMI_FORCE_SLEEP                    BIT0

//
// HSP
//
#define HSP_FSV_OUT_EN                      BIT24
#define HSP_VBUSVLDEXTSEL                   BIT13
#define HSP_VBUSVLDEXT                      BIT12
#define HSP_EN_UTMISUSPEND                  BIT9
#define HSP_COMMONONN                       BIT8

//
// HSP_TUNE (HSPPARACON)
//
#define HSP_TUNE_TXVREF_MASK                (0xFU << 28)
#define HSP_TUNE_TXVREF(x)                  (((UINT32)(x) & 0xF) << 28)
#define HSP_TUNE_TXRISE_MASK                (0x3U << 24)
#define HSP_TUNE_TXRISE(x)                  (((x) & 0x3) << 24)
#define HSP_TUNE_TXRES_MASK                 (0x3U << 21)
#define HSP_TUNE_TXRES(x)                   (((x) & 0x3) << 21)
#define HSP_TUNE_TXPREEMPA_PLUS             BIT20
#define HSP_TUNE_TXPREEMPA_MASK             (0x3U << 18)
#define HSP_TUNE_TXPREEMPA(x)               (((x) & 0x3) << 18)
#define HSP_TUNE_HSXV_MASK                  (0x3U << 16)
#define HSP_TUNE_HSXV(x)                    (((x) & 0x3) << 16)
#define HSP_TUNE_TXFSLS_MASK                (0xFU << 12)
#define HSP_TUNE_TXFSLS(x)                  (((x) & 0xF) << 12)
#define HSP_TUNE_SQRX_MASK                  (0x7U << 8)
#define HSP_TUNE_SQRX(x)                    (((x) & 0x7) << 8)
#define HSP_TUNE_OTG_MASK                   (0x7U << 4)
#define HSP_TUNE_OTG(x)                     (((x) & 0x7) << 4)
#define HSP_TUNE_COMPDIS_MASK               (0x7U << 0)
#define HSP_TUNE_COMPDIS(x)                 (((x) & 0x7) << 0)

//
// HSP_TEST
//
#define HSP_TEST_SIDDQ                      BIT24
#define HSP_TEST_LINESTATE_MASK             (0x3U << 20)
#define HSP_TEST_LINESTATE(x)               (((x) & HSP_TEST_LINESTATE_MASK) >> 20)

//
// CMU_FSYS
//
#define CMU_FSYS_PLL_LOCKTIME_PLL_USB       0x0000
#define CMU_FSYS_PLL_CON0_PLL_USB           0x01A0
#define CMU_FSYS_GAT_USB20PHY_CLKCORE       0x200C
#define CMU_FSYS_GAT_USB30DRD_ACLK_20PHYCTRL 0x2068
#define CMU_FSYS_GAT_USB30DRD_BUS_CLK_EARLY 0x2074
#define CMU_FSYS_GAT_USB30DRD_REF_CLK       0x2078

//
// PLL_USB
//
#define PLL_USB_ENABLE                      BIT31
#define PLL_USB_LOCKED                      BIT29
#define PLL_USB_MUX_FOUT                    BIT4
#define PLL_USB_PMS_MASK                    ((0x1FFU << 16) | (0x3FU << 8) | 0x7U)
#define PLL_USB_PMS_50MHZ                   ((400U << 16) | (13U << 8) | 4U)
#define PLL_USB_PDIV                        13
#define PLL_USB_LOCK_FACTOR                 150

#define CMU_GATE_ENABLE                     BIT21

#endif /* _EXYNOS7885_USB_H_ */
