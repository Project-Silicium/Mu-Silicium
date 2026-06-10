#ifndef _HSI2C_H_
#define _HSI2C_H_

//
// HSI2C MMIO
//
#define HSI2C_MMIO_LENGTH          0x1000

//
// Controller Register Bits
//
#define HSI2C_FUNC_MODE_I2C        BIT0
#define HSI2C_MASTER               BIT3
#define HSI2C_RXCHON               BIT6
#define HSI2C_TXCHON               BIT7
#define HSI2C_EXT_MSB              BIT29
#define HSI2C_EXT_ADDR             BIT30
#define HSI2C_SW_RST               BIT31

//
// Trailing Controller Register Bits
//
#define HSI2C_TRAILING_COUNT       0xFFFFFF

//
// Config Register Bits
//
#define HSI2C_FTL_CYCLE_SDA_MASK   (7 << 13)
#define HSI2C_FTL_CYCLE_SCL_MASK   (7 << 16)
#define HSI2C_FILTER_EN_SDA        BIT27
#define HSI2C_FILTER_EN_SCL        BIT28
#define HSI2C_HS_MODE              BIT29
#define HSI2C_10BIT_ADDR_MODE      BIT30
#define HSI2C_AUTO_MODE            BIT31

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
