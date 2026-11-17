#ifndef _I2C_H_
#define _I2C_H_

#define I2C_MMIO_LENGTH              0x1000

#define I2C_TRANSFER_TIMEOUT_US      100000
#define I2C_BUS_IDLE_TIMEOUT_US      5000
#define I2C_TRANSFER_RETRIES         5

//
// IICCON Bits
//
#define I2C_CON_ACK_ENABLE           BIT7
#define I2C_CON_DIVIDE_BY_512        BIT6
#define I2C_CON_INTERRUPT_ENABLE     BIT5
#define I2C_CON_INTERRUPT_PENDING    BIT4
#define I2C_CON_SCALE_MASK           0xF

//
// IICSTAT Bits
//
#define I2C_STAT_MASTER_RECEIVE      (2 << 6)
#define I2C_STAT_MASTER_TRANSMIT     (3 << 6)
#define I2C_STAT_START               BIT5
#define I2C_STAT_BUS_BUSY            BIT5
#define I2C_STAT_TXRX_ENABLE         BIT4
#define I2C_STAT_ARBITRATION_LOST    BIT3
#define I2C_STAT_LAST_BIT            BIT0

//
// IICLC Bits
//
#define I2C_LINE_CONTROL_FILTER      BIT2
#define I2C_LINE_CONTROL_DELAY_MASK  0x3

typedef struct {
  UINT32 Control;
  UINT32 Status;
  UINT32 Address;
  UINT32 Data;
  UINT32 LineControl;
} EFI_I2C_BUS;

#endif /* _I2C_H_ */
