#ifndef _HSI2C_H_
#define _HSI2C_H_

//
// HSI2C MMIO
//
#define HSI2C_MMIO_LENGTH               0x1000

//
// HSI2C Clock Speeds
//
#define I2C_STAND_TX_CLOCK              100000
#define I2C_FS_TX_CLOCK                 400000
#define I2C_FAST_PLUS_TX_CLOCK          1000000
#define I2C_HS_TX_CLOCK                 2500000

//
// Controller Register Bits
//
#define HSI2C_FUNC_MODE_I2C             BIT0
#define HSI2C_MASTER                    BIT3
#define HSI2C_RXCHON                    BIT6
#define HSI2C_TXCHON                    BIT7
#define HSI2C_EXT_MSB                   BIT29
#define HSI2C_EXT_ADDR                  BIT30
#define HSI2C_SW_RST                    BIT31

//
// Trailing Controller Register Bits
//
#define HSI2C_TRAILING_COUNT            0xFFFFFF

//
// Config Register Bits
//
#define HSI2C_FTL_CYCLE_SDA_MASK       (7 << 13)
#define HSI2C_FTL_CYCLE_SCL_MASK       (7 << 16)
#define HSI2C_FILTER_EN_SDA             BIT27
#define HSI2C_FILTER_EN_SCL             BIT28
#define HSI2C_HS_MODE                   BIT29
#define HSI2C_10BIT_ADDR_MODE           BIT30
#define HSI2C_AUTO_MODE                 BIT31

//
// Timeout Register Bits
//
#define HSI2C_TIMEOUT_EN                BIT31

//
// Address Register Bits
//
#define EXYNOS_FIFO_SIZE               (16)
#define FIFO_TRIG_CRITERIA             (8)

//
// FIFO Controller Register Bits
//
#define HSI2C_FIFO_MAX                  0x40
#define HSI2C_RXFIFO_TRIGGER_LEVEL(x)  (x << 4)
#define HSI2C_TXFIFO_TRIGGER_LEVEL(x)  (x << 16)
#define HSI2C_RXFIFO_EN                 BIT0
#define HSI2C_TXFIFO_EN                 BIT1

//
// Auto Configuration Register Bits
//
#define HSI2C_READ_WRITE                BIT16
#define HSI2C_STOP_AFTER_TRANS          BIT17
#define HSI2C_MASTER_RUN                BIT31

//
// Interrupt Enable Register Bits
//
#define HSI2C_INT_TX_ALMOSTEMPTY_EN     BIT0
#define HSI2C_INT_RX_ALMOSTFULL_EN      BIT1
#define HSI2C_INT_TRAILING_EN           BIT6
#define HSI2C_INT_TRANSFER_DONE         BIT7
#define HSI2C_INT_I2C_EN                BIT9
#define HSI2C_INT_CHK_TRANS_STATE      (15 << 8)

//
// FIFO Status Register Bits
//
#define HSI2C_TX_FIFO_LVL(x)           ((x >> 0)  & 0x7F)
#define HSI2C_RX_FIFO_LVL(x)           ((x >> 16) & 0x7F)
#define HSI2C_TX_FIFO_LVL_MASK         (0x7F << 0)
#define HSI2C_RX_FIFO_LVL_MASK         (0x7F << 16)
#define HSI2C_TX_FIFO_FULL              BIT7
#define HSI2C_TX_FIFO_EMPTY             BIT8
#define HSI2C_RX_FIFO_FULL              BIT23
#define HSI2C_RX_FIFO_EMPTY             BIT24
#define HSI2C_FIFO_EMPTY               (HSI2C_RX_FIFO_EMPTY | HSI2C_TX_FIFO_EMPTY)

//
// Timing Masks
//
#define TIMING_MASK_1(x)               ((0xFF << (x)) & 0xFF)
#define TIMING_MASK_2(x)               (((0xFFFFFFFF >> (x)) << (x)) & 0xFF)

//
// Address Register Bits
//
#define HSI2C_SLV_ADDR_SLV(x)          ((x & 0x3FF) << 0)
#define HSI2C_SLV_ADDR_MAS(x)          ((x & 0x3FF) << 10)
#define HSI2C_MASTER_ID(x)             ((x & 0x0FF) << 24)
#define MASTER_ID(x)                   ((x & 0x007) + 8)

//
// HSI2C Bus
//
typedef struct {
  UINT32 ctrl;
  UINT32 fifo_ctrl;
  UINT32 trail_ctrl;
  UINT16 Reserved1[10];
  UINT32 int_enable;
  UINT32 int_status;
  UINT16 Reserved2[4];
  UINT32 fifo_status;
  UINT32 tx_data;
  UINT32 rx_data;
  UINT16 Reserved3[2];
  UINT32 conf;
  UINT32 auto_conf;
  UINT32 timeout;
  UINT32 cmd;
  UINT32 trans_status;
  UINT32 timing_hs1;
  UINT32 timing_hs2;
  UINT32 timing_hs3;
  UINT32 timing_fs1;
  UINT32 timing_fs2;
  UINT32 timing_fs3;
  UINT32 timing_sla;
  UINT32 address;
  UINT16 Reserved4[40];
  UINT32 usi_con;
} EFI_HSI2C_BUS;

#endif /* _HSI2C_H_ */
