#ifndef _DWC3_H_
#define _DWC3_H_

//
// Global Registers
//
#define DWC3_GSBUSCFG0              0xC100
#define DWC3_GSBUSCFG1              0xC104
#define DWC3_GCTL                   0xC110
#define DWC3_GSTS                   0xC118
#define DWC3_GUCTL1                 0xC11C
#define DWC3_GSNPSID                0xC120
#define DWC3_GUCTL                  0xC12C
#define DWC3_GHWPARAMS0             0xC140
#define DWC3_GHWPARAMS1             0xC144
#define DWC3_GHWPARAMS2             0xC148
#define DWC3_GHWPARAMS3             0xC14C
#define DWC3_GHWPARAMS4             0xC150
#define DWC3_GHWPARAMS5             0xC154
#define DWC3_GHWPARAMS6             0xC158
#define DWC3_GHWPARAMS7             0xC15C
#define DWC3_GUCTL2                 0xC19C
#define DWC3_GUSB2PHYCFG(n)         (0xC200 + ((n) * 0x04))
#define DWC3_GUSB3PIPECTL(n)        (0xC2C0 + ((n) * 0x04))
#define DWC3_GTXFIFOSIZ(n)          (0xC300 + ((n) * 0x04))
#define DWC3_GRXFIFOSIZ(n)          (0xC380 + ((n) * 0x04))
#define DWC3_GEVNTADRLO(n)          (0xC400 + ((n) * 0x10))
#define DWC3_GEVNTADRHI(n)          (0xC404 + ((n) * 0x10))
#define DWC3_GEVNTSIZ(n)            (0xC408 + ((n) * 0x10))
#define DWC3_GEVNTCOUNT(n)          (0xC40C + ((n) * 0x10))
#define DWC3_GHWPARAMS8             0xC600
#define DWC3_GFLADJ                 0xC630

//
// GFLADJ
//
#define DWC3_GFLADJ_REFCLK_240MHZDECR_PLS1  BIT31
#define DWC3_GFLADJ_REFCLK_240MHZ_DECR(n)   ((n) << 24)
#define DWC3_GFLADJ_REFCLK_240MHZ_DECR_MASK DWC3_GFLADJ_REFCLK_240MHZ_DECR (0x7F)
#define DWC3_GFLADJ_REFCLK_LPM_SEL          BIT23
#define DWC3_GFLADJ_REFCLK_FLADJ(n)         ((n) << 8)
#define DWC3_GFLADJ_REFCLK_FLADJ_MASK       DWC3_GFLADJ_REFCLK_FLADJ (0x3FFF)
#define DWC3_GFLADJ_30MHZ_SDBND_SEL         BIT7
#define DWC3_GFLADJ_30MHZ_MASK              0x3F

//
// Device Registers
//
#define DWC3_DCFG                   0xC700
#define DWC3_DCTL                   0xC704
#define DWC3_DEVTEN                 0xC708
#define DWC3_DSTS                   0xC70C
#define DWC3_DGCMDPAR               0xC710
#define DWC3_DGCMD                  0xC714
#define DWC3_DALEPENA               0xC720

//
// Device Endpoint Command Registers. "n" is the Physical Endpoint Number.
//
#define DWC3_DEP_BASE(n)            (0xC800 + ((n) * 0x10))
#define DWC3_DEPCMDPAR2             0x00
#define DWC3_DEPCMDPAR1             0x04
#define DWC3_DEPCMDPAR0             0x08
#define DWC3_DEPCMD                 0x0C

//
// GSNPSID
//
#define DWC3_GSNPSID_MASK           0xFFFF0000
#define DWC3_GSNPSID_USB3           0x55330000
#define DWC3_GSNPSID_USB31          0x33310000
#define DWC3_REVISION_MASK          0x0000FFFF

//
// GCTL
//
#define DWC3_GCTL_PWRDNSCALE(n)     ((n) << 19)
#define DWC3_GCTL_PWRDNSCALE_MASK   DWC3_GCTL_PWRDNSCALE (0x1FFF)
#define DWC3_GCTL_U2RSTECN          BIT16
#define DWC3_GCTL_RAMCLKSEL(x)      (((x) & 0x3) << 6)
#define DWC3_GCTL_PRTCAP(n)         (((n) & (3 << 12)) >> 12)
#define DWC3_GCTL_PRTCAPDIR(n)      ((n) << 12)
#define DWC3_GCTL_PRTCAP_HOST       1
#define DWC3_GCTL_PRTCAP_DEVICE     2
#define DWC3_GCTL_PRTCAP_OTG        3
#define DWC3_GCTL_CORESOFTRESET     BIT11
#define DWC3_GCTL_SOFITPSYNC        BIT10
#define DWC3_GCTL_SCALEDOWN(n)      ((n) << 4)
#define DWC3_GCTL_SCALEDOWN_MASK    DWC3_GCTL_SCALEDOWN (3)
#define DWC3_GCTL_DISSCRAMBLE       BIT3
#define DWC3_GCTL_U2EXIT_LFPS       BIT2
#define DWC3_GCTL_GBLHIBERNATIONEN  BIT1
#define DWC3_GCTL_DSBLCLKGTNG       BIT0

//
// GUSB2PHYCFG
//
#define DWC3_GUSB2PHYCFG_PHYSOFTRST         BIT31
#define DWC3_GUSB2PHYCFG_U2_FREECLK_EXISTS  BIT30
#define DWC3_GUSB2PHYCFG_ENBLSLPM           BIT8
#define DWC3_GUSB2PHYCFG_SUSPHY             BIT6
#define DWC3_GUSB2PHYCFG_ULPI_UTMI          BIT4
#define DWC3_GUSB2PHYCFG_PHYIF(n)           ((n) << 3)
#define DWC3_GUSB2PHYCFG_PHYIF_MASK         DWC3_GUSB2PHYCFG_PHYIF (1)
#define DWC3_GUSB2PHYCFG_USBTRDTIM(n)       ((n) << 10)
#define DWC3_GUSB2PHYCFG_USBTRDTIM_MASK     DWC3_GUSB2PHYCFG_USBTRDTIM (0xF)

#define UTMI_PHYIF_8_BIT                    0
#define UTMI_PHYIF_16_BIT                   1
#define USBTRDTIM_UTMI_8_BIT                9
#define USBTRDTIM_UTMI_16_BIT               5

//
// GUSB3PIPECTL
//
#define DWC3_GUSB3PIPECTL_PHYSOFTRST    BIT31
#define DWC3_GUSB3PIPECTL_UX_EXIT_PX    BIT27
#define DWC3_GUSB3PIPECTL_REQP1P2P3     BIT24
#define DWC3_GUSB3PIPECTL_DEP1P2P3(n)   ((n) << 19)
#define DWC3_GUSB3PIPECTL_DEP1P2P3_EN   DWC3_GUSB3PIPECTL_DEP1P2P3 (1)
#define DWC3_GUSB3PIPECTL_SUSPHY        BIT17

//
// GEVNTSIZ / GEVNTCOUNT
//
#define DWC3_GEVNTSIZ_INTMASK       BIT31
#define DWC3_GEVNTSIZ_SIZE(n)       ((n) & 0xFFFF)
#define DWC3_GEVNTCOUNT_MASK        0xFFFC
#define DWC3_GEVNTCOUNT_EHB         BIT31

//
// GHWPARAMS
//
#define DWC3_GHWPARAMS0_MODE(n)     ((n) & 0x3)
#define DWC3_GHWPARAMS0_MODE_GADGET 0
#define DWC3_GHWPARAMS0_MODE_HOST   1
#define DWC3_GHWPARAMS0_MODE_DRD    2
#define DWC3_GHWPARAMS0_MDWIDTH(n)  (((n) >> 8) & 0xFF)
#define DWC3_GHWPARAMS1_EN_PWROPT(n)   (((n) & (3 << 24)) >> 24)
#define DWC3_GHWPARAMS1_EN_PWROPT_HIB  2
#define DWC3_GHWPARAMS3_HSPHY_IFC(n)   (((n) & (3 << 2)) >> 2)
#define DWC3_GHWPARAMS3_HSPHY_IFC_UTMI 1
#define DWC3_GHWPARAMS3_NUM_EPS(n)     (((n) & (0x3F << 12)) >> 12)
#define DWC3_GHWPARAMS3_NUM_IN_EPS(n)  (((n) & (0x1F << 18)) >> 18)

//
// DCFG
//
#define DWC3_DCFG_DEVADDR(a)        ((a) << 3)
#define DWC3_DCFG_DEVADDR_MASK      DWC3_DCFG_DEVADDR (0x7F)
#define DWC3_DCFG_SPEED_MASK        (7 << 0)
#define DWC3_DCFG_SUPERSPEED_PLUS   (5 << 0)
#define DWC3_DCFG_SUPERSPEED        (4 << 0)
#define DWC3_DCFG_HIGHSPEED         (0 << 0)
#define DWC3_DCFG_FULLSPEED         BIT0
#define DWC3_DCFG_LOWSPEED          (2 << 0)
#define DWC3_DCFG_NUMP_SHIFT        17
#define DWC3_DCFG_NUMP(n)           (((n) >> DWC3_DCFG_NUMP_SHIFT) & 0x1F)
#define DWC3_DCFG_NUMP_MASK         (0x1F << DWC3_DCFG_NUMP_SHIFT)
#define DWC3_DCFG_LPM_CAP           BIT22

//
// DCTL
//
#define DWC3_DCTL_RUN_STOP          BIT31
#define DWC3_DCTL_CSFTRST           BIT30
#define DWC3_DCTL_LSFTRST           BIT29
#define DWC3_DCTL_HIRD_THRES(n)     ((n) << 24)
#define DWC3_DCTL_HIRD_THRES_MASK   (0x1F << 24)
#define DWC3_DCTL_KEEP_CONNECT      BIT19
#define DWC3_DCTL_L1_HIBER_EN       BIT18
#define DWC3_DCTL_CRS               BIT17
#define DWC3_DCTL_CSS               BIT16
#define DWC3_DCTL_INITU2ENA         BIT12
#define DWC3_DCTL_ACCEPTU2ENA       BIT11
#define DWC3_DCTL_INITU1ENA         BIT10
#define DWC3_DCTL_ACCEPTU1ENA       BIT9
#define DWC3_DCTL_ULSTCHNGREQ_MASK  (0x0F << 5)
#define DWC3_DCTL_ULSTCHNGREQ(n)    (((n) << 5) & DWC3_DCTL_ULSTCHNGREQ_MASK)
#define DWC3_DCTL_TSTCTRL_MASK      (0x0F << 1)

//
// DEVTEN
//
#define DWC3_DEVTEN_EVNTOVERFLOWEN  BIT11
#define DWC3_DEVTEN_CMDCMPLTEN      BIT10
#define DWC3_DEVTEN_ERRTICERREN     BIT9
#define DWC3_DEVTEN_SOFEN           BIT7
#define DWC3_DEVTEN_EOPFEN          BIT6
#define DWC3_DEVTEN_WKUPEVTEN       BIT4
#define DWC3_DEVTEN_ULSTCNGEN       BIT3
#define DWC3_DEVTEN_CONNECTDONEEN   BIT2
#define DWC3_DEVTEN_USBRSTEN        BIT1
#define DWC3_DEVTEN_DISCONNEVTEN    BIT0

//
// DSTS
//
#define DWC3_DSTS_COREIDLE          BIT23
#define DWC3_DSTS_DEVCTRLHLT        BIT22
#define DWC3_DSTS_USBLNKST_MASK     (0x0F << 18)
#define DWC3_DSTS_USBLNKST(n)       (((n) & DWC3_DSTS_USBLNKST_MASK) >> 18)
#define DWC3_DSTS_RXFIFOEMPTY       BIT17
#define DWC3_DSTS_SOFFN_MASK        (0x3FFF << 3)
#define DWC3_DSTS_CONNECTSPD        (7 << 0)
#define DWC3_DSTS_SUPERSPEED_PLUS   (5 << 0)
#define DWC3_DSTS_SUPERSPEED        (4 << 0)
#define DWC3_DSTS_HIGHSPEED         (0 << 0)
#define DWC3_DSTS_FULLSPEED         BIT0
#define DWC3_DSTS_LOWSPEED          (2 << 0)

//
// SuperSpeed Link States
//
#define DWC3_LINK_STATE_MASK        0x0F
#define DWC3_LINK_STATE_U0          0x00
#define DWC3_LINK_STATE_U1          0x01
#define DWC3_LINK_STATE_U2          0x02
#define DWC3_LINK_STATE_U3          0x03
#define DWC3_LINK_STATE_SS_DIS      0x04
#define DWC3_LINK_STATE_RESET       0x0E
#define DWC3_LINK_STATE_RESUME      0x0F

//
// USB 2.0 Link States. The Field is the same, the Encoding is not.
//
#define DWC3_LINK_STATE_HS_ON            0x00
#define DWC3_LINK_STATE_HS_SLEEP         0x02
#define DWC3_LINK_STATE_HS_SUSPEND       0x03
#define DWC3_LINK_STATE_HS_DISCONNECTED  0x04
#define DWC3_LINK_STATE_HS_EARLY_SUSPEND 0x05
#define DWC3_LINK_STATE_HS_RESET         0x06
#define DWC3_LINK_STATE_HS_RESUME        0x07

//
// Device Endpoint Command Register
//
#define DWC3_DEPCMD_PARAM_SHIFT     16
#define DWC3_DEPCMD_PARAM(x)        ((x) << DWC3_DEPCMD_PARAM_SHIFT)
#define DWC3_DEPCMD_GET_RSC_IDX(x)  (((x) >> DWC3_DEPCMD_PARAM_SHIFT) & 0x7F)
#define DWC3_DEPCMD_STATUS(x)       (((x) >> 12) & 0x0F)
#define DWC3_DEPCMD_HIPRI_FORCERM   BIT11
#define DWC3_DEPCMD_CLEARPENDIN     BIT11
#define DWC3_DEPCMD_CMDACT          BIT10
#define DWC3_DEPCMD_CMDIOC          BIT8

#define DWC3_DEPCMD_DEPSTARTCFG       0x09
#define DWC3_DEPCMD_ENDTRANSFER       0x08
#define DWC3_DEPCMD_UPDATETRANSFER    0x07
#define DWC3_DEPCMD_STARTTRANSFER     0x06
#define DWC3_DEPCMD_CLEARSTALL        0x05
#define DWC3_DEPCMD_SETSTALL          0x04
#define DWC3_DEPCMD_GETEPSTATE        0x03
#define DWC3_DEPCMD_SETTRANSFRESOURCE 0x02
#define DWC3_DEPCMD_SETEPCONFIG       0x01
#define DWC3_DEPCMD_CMD(x)            ((x) & 0xF)

#define DWC3_DEPCMD_TYPE_CONTROL    0
#define DWC3_DEPCMD_TYPE_ISOC       1
#define DWC3_DEPCMD_TYPE_BULK       2
#define DWC3_DEPCMD_TYPE_INTR       3

#define DWC3_DALEPENA_EP(n)         (1U << (n))

//
// DEPCFG Command Parameter 0
//
#define DWC3_DEPCFG_EP_TYPE(n)          (((n) & 0x3) << 1)
#define DWC3_DEPCFG_MAX_PACKET_SIZE(n)  (((n) & 0x7FF) << 3)
#define DWC3_DEPCFG_FIFO_NUMBER(n)      (((n) & 0x1F) << 17)
#define DWC3_DEPCFG_BURST_SIZE(n)       (((n) & 0xF) << 22)
#define DWC3_DEPCFG_DATA_SEQ_NUM(n)     ((n) << 26)
#define DWC3_DEPCFG_ACTION_INIT         (0U << 30)
#define DWC3_DEPCFG_ACTION_RESTORE      BIT30
#define DWC3_DEPCFG_ACTION_MODIFY       (2U << 30)

//
// DEPCFG Command Parameter 1
//
#define DWC3_DEPCFG_INT_NUM(n)          (((n) & 0x1F) << 0)
#define DWC3_DEPCFG_XFER_COMPLETE_EN    BIT8
#define DWC3_DEPCFG_XFER_IN_PROGRESS_EN BIT9
#define DWC3_DEPCFG_XFER_NOT_READY_EN   BIT10
#define DWC3_DEPCFG_FIFO_ERROR_EN       BIT11
#define DWC3_DEPCFG_STREAM_EVENT_EN     BIT13
#define DWC3_DEPCFG_BINTERVAL_M1(n)     (((n) & 0xFF) << 16)
#define DWC3_DEPCFG_STREAM_CAPABLE      BIT24
#define DWC3_DEPCFG_EP_NUMBER(n)        (((n) & 0x1F) << 25)
#define DWC3_DEPCFG_BULK_BASED          BIT30
#define DWC3_DEPCFG_FIFO_BASED          BIT31

//
// DEPXFERCFG Command Parameter 0
//
#define DWC3_DEPXFERCFG_NUM_XFER_RES(n) ((n) & 0xFFFF)

#define DWC3_TRB_SIZE_MASK          0x00FFFFFF
#define DWC3_TRB_SIZE_LENGTH(n)     ((n) & DWC3_TRB_SIZE_MASK)
#define DWC3_TRB_SIZE_TRBSTS(n)     (((n) & (0x0FU << 28)) >> 28)

#define DWC3_TRBSTS_OK              0
#define DWC3_TRBSTS_MISSED_ISOC     1
#define DWC3_TRBSTS_SETUP_PENDING   2
#define DWC3_TRB_STS_XFER_IN_PROG   4

#define DWC3_TRB_CTRL_HWO           BIT0
#define DWC3_TRB_CTRL_LST           BIT1
#define DWC3_TRB_CTRL_CHN           BIT2
#define DWC3_TRB_CTRL_CSP           BIT3
#define DWC3_TRB_CTRL_TRBCTL(n)     (((n) & 0x3F) << 4)
#define DWC3_TRB_CTRL_ISP_IMI       BIT10
#define DWC3_TRB_CTRL_IOC           BIT11
#define DWC3_TRB_CTRL_SID_SOFN(n)   (((n) & 0xFFFF) << 14)

#define DWC3_TRBCTL_NORMAL            DWC3_TRB_CTRL_TRBCTL (1)
#define DWC3_TRBCTL_CONTROL_SETUP     DWC3_TRB_CTRL_TRBCTL (2)
#define DWC3_TRBCTL_CONTROL_STATUS2   DWC3_TRB_CTRL_TRBCTL (3)
#define DWC3_TRBCTL_CONTROL_STATUS3   DWC3_TRB_CTRL_TRBCTL (4)
#define DWC3_TRBCTL_CONTROL_DATA      DWC3_TRB_CTRL_TRBCTL (5)
#define DWC3_TRBCTL_ISOCHRONOUS_FIRST DWC3_TRB_CTRL_TRBCTL (6)
#define DWC3_TRBCTL_ISOCHRONOUS       DWC3_TRB_CTRL_TRBCTL (7)
#define DWC3_TRBCTL_LINK_TRB          DWC3_TRB_CTRL_TRBCTL (8)

//
// Event Buffer Entries
//
#define DWC3_EVENT_IS_ENDPOINT(e)       (((e) & BIT0) == 0)

//
// Endpoint Specific Event Layout
//
#define DWC3_DEPEVT_EPNUMBER(e)         (((e) >> 1) & 0x1F)
#define DWC3_DEPEVT_TYPE(e)             (((e) >> 6) & 0x0F)
#define DWC3_DEPEVT_STATUS(e)           (((e) >> 12) & 0x0F)
#define DWC3_DEPEVT_PARAMETERS(e)       (((e) >> 16) & 0xFFFF)

#define DWC3_DEPEVT_XFERCOMPLETE        0x01
#define DWC3_DEPEVT_XFERINPROGRESS      0x02
#define DWC3_DEPEVT_XFERNOTREADY        0x03
#define DWC3_DEPEVT_RXTXFIFOEVT         0x04
#define DWC3_DEPEVT_STREAMEVT           0x06
#define DWC3_DEPEVT_EPCMDCMPLT          0x07

#define DEPEVT_STATUS_BUSERR            BIT0
#define DEPEVT_STATUS_SHORT             BIT1
#define DEPEVT_STATUS_IOC               BIT2
#define DEPEVT_STATUS_LST               BIT3

#define DEPEVT_STATUS_TRANSFER_ACTIVE   BIT3
#define DEPEVT_STATUS_CONTROL_DATA      1
#define DEPEVT_STATUS_CONTROL_STATUS    2
#define DEPEVT_STATUS_CONTROL_PHASE(n)  ((n) & 3)

#define DEPEVT_TRANSFER_NO_RESOURCE     1
#define DEPEVT_TRANSFER_BUS_EXPIRY      2

#define DWC3_DEVT_CATEGORY(e)           (((e) >> 1) & 0x7F)
#define DWC3_DEVT_TYPE(e)               (((e) >> 8) & 0x0F)
#define DWC3_DEVT_INFO(e)               (((e) >> 16) & 0x1FF)

#define DWC3_EVENT_TYPE_DEV             0

#define DWC3_DEVICE_EVENT_DISCONNECT          0
#define DWC3_DEVICE_EVENT_RESET               1
#define DWC3_DEVICE_EVENT_CONNECT_DONE        2
#define DWC3_DEVICE_EVENT_LINK_STATUS_CHANGE  3
#define DWC3_DEVICE_EVENT_WAKEUP              4
#define DWC3_DEVICE_EVENT_HIBER_REQ           5
#define DWC3_DEVICE_EVENT_EOPF                6
#define DWC3_DEVICE_EVENT_SOF                 7
#define DWC3_DEVICE_EVENT_ERRATIC_ERROR       9
#define DWC3_DEVICE_EVENT_CMD_CMPL            10
#define DWC3_DEVICE_EVENT_OVERFLOW            11

//
// Transfer Request Block
//
#pragma pack(1)
typedef struct {
  UINT32 BufferPtrLow;
  UINT32 BufferPtrHigh;
  UINT32 Size;
  UINT32 Control;
} DWC3_TRB;
#pragma pack()

#endif /* _DWC3_H_ */
