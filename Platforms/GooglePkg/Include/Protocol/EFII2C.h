#ifndef __EFII2C_H__
#define __EFII2C_H__
#include <Uefi.h>
#define QCOM_I2C_PROTOCOL_GUID                                                 \
  {                                                                            \
    0xb27ae8b1, 0x3e10, 0x4d07,                                                \
    {                                                                          \
      0xab, 0x5c, 0xeb, 0x9a, 0x6d, 0xc6, 0xfa, 0x8f                           \
    }                                                                          \
  }
#define I2C_FLAG_START      0x00000001
#define I2C_FLAG_STOP       0x00000002
#define I2C_FLAG_WRITE      0x00000004
#define I2C_FLAG_READ       0x00000008
#define I2C_FLAG_TIMESTAMP  0x00000010
#define I2C_STANDARD_MODE_FREQ_KHZ          100
#define I2C_FAST_MODE_FREQ_KHZ              400
#define I2C_FAST_MODE_PLUS_FREQ_KHZ         1000
#define I3C_SDR_DATA_RATE_12500_KHZ         12500
#define I3C_SDR_DATA_RATE_10000_KHZ         10000
#define I2C_WRITE_TRANSFER  (I2C_FLAG_START | I2C_FLAG_WRITE | I2C_FLAG_STOP)
#define I2C_READ_TRANSFER   (I2C_FLAG_START | I2C_FLAG_READ  | I2C_FLAG_STOP)
#define I2C_WR_RD_WRITE     (I2C_FLAG_START | I2C_FLAG_WRITE)
#define I2C_WR_RD_READ      I2C_READ_TRANSFER
#define I2C_ADDRESS_QUERY   (I2C_FLAG_START)
#define I2C_STOP_COMMAND    (I2C_FLAG_STOP)
#define I2C_BUS_CLEAR       0
#define I2C_WR_RD_READ      I2C_READ_TRANSFER
#define I2C_TRANSFER_MASK      (I2C_FLAG_WRITE | I2C_FLAG_READ)
#define I2C_SUCCESS(x)  (x == I2C_SUCCESS)
#define I2C_ERROR(x)    (x != I2C_SUCCESS)
#define VALID_FLAGS(x) (\
    ((x & I2C_TRANSFER_MASK) == I2C_FLAG_READ) ||\
    ((x & I2C_TRANSFER_MASK) == I2C_FLAG_WRITE)\
  )

typedef enum {
  I2C_SUCCESS = 0,
  I2C_ERROR_INVALID_PARAMETER,
  I2C_ERROR_UNSUPPORTED_CORE_INSTANCE,
  I2C_ERROR_API_INVALID_EXECUTION_LEVEL,
  I2C_ERROR_API_NOT_SUPPORTED,
  I2C_ERROR_API_ASYNC_MODE_NOT_SUPPORTED,
  I2C_ERROR_API_PROTOCOL_MODE_NOT_SUPPORTED,
  I2C_ERROR_HANDLE_ALLOCATION,
  I2C_ERROR_HW_INFO_ALLOCATION,
  I2C_ERROR_BUS_NOT_IDLE,
  I2C_ERROR_TRANSFER_TIMEOUT,
  I2C_ERROR_MEM_ALLOC_FAIL,
  I2C_ERROR_INPUT_FIFO_OVER_RUN,
  I2C_ERROR_OUTPUT_FIFO_UNDER_RUN,
  I2C_ERROR_INPUT_FIFO_UNDER_RUN,
  I2C_ERROR_OUTPUT_FIFO_OVER_RUN,
  I2C_ERROR_COMMAND_OVER_RUN,
  I2C_ERROR_COMMAND_ILLEGAL,
  I2C_ERROR_COMMAND_FAIL,
  I2C_ERROR_INVALID_CMD_OPCODE,
  I2C_ERROR_START_STOP_UNEXPECTED,
  I2C_ERROR_DATA_NACK,
  I2C_ERROR_ADDR_NACK,
  I2C_ERROR_ARBITRATION_LOST,
  I2C_ERROR_PLATFORM_INIT_FAIL,
  I2C_ERROR_PLATFORM_DEINIT_FAIL,
  I2C_ERROR_PLATFORM_CRIT_SEC_FAIL,
  I2C_ERROR_PLATFORM_SIGNAL_FAIL,
  I2C_ERROR_PLATFORM_GET_CONFIG_FAIL,
  I2C_ERROR_PLATFORM_GET_CLOCK_CONFIG_FAIL,
  I2C_ERROR_PLATFORM_REG_INT_FAIL,
  I2C_ERROR_PLATFORM_DE_REG_INT_FAIL,
  I2C_ERROR_PLATFORM_CLOCK_ENABLE_FAIL,
  I2C_ERROR_PLATFORM_GPIO_ENABLE_FAIL,
  I2C_ERROR_PLATFORM_CLOCK_DISABLE_FAIL,
  I2C_ERROR_PLATFORM_GPIO_DISABLE_FAIL,
  I2C_TRANSFER_CANCELED,
  I2C_TRANSFER_FORCE_TERMINATED,
  I2C_TRANSFER_COMPLETED,
  I2C_TRANSFER_INVALID,
  I2C_ERROR_HANDLE_ALREADY_IN_QUEUE,
  I2C_ERROR_DMA_REG_FAIL,
  I2C_ERROR_DMA_EV_CHAN_ALLOC_FAIL,
  I2C_ERROR_DMA_TX_CHAN_ALLOC_FAIL,
  I2C_ERROR_DMA_RX_CHAN_ALLOC_FAIL,
  I2C_ERROR_DMA_TX_CHAN_START_FAIL,
  I2C_ERROR_DMA_RX_CHAN_START_FAIL,
  I2C_ERROR_DMA_TX_CHAN_STOP_FAIL,
  I2C_ERROR_DMA_RX_CHAN_STOP_FAIL,
  I2C_ERROR_DMA_TX_CHAN_RESET_FAIL,
  I2C_ERROR_DMA_RX_CHAN_RESET_FAIL,
  I2C_ERROR_DMA_EV_CHAN_DE_ALLOC_FAIL,
  I2C_ERROR_DMA_TX_CHAN_DE_ALLOC_FAIL,
  I2C_ERROR_DMA_RX_CHAN_DE_ALLOC_FAIL,
  I2C_ERROR_DMA_INSUFFICIENT_RESOURCES,
  I2C_ERROR_DMA_PROCESS_TRANSFER_FAIL,
  I2C_ERROR_DMA_EVT_OTHER,
  I2C_ERROR_FW_LOAD_FALIURE,
  I2C_ERROR_INVALID_FW_VERSION,
} I2C_STATUS;

typedef enum {
  I2C = 0,
  SMBUS,
  I3C_SDR,
  I3C_HDR_DDR,
  I3C_BROADCAST_CCC,
  I3C_DIRECT_CCC,
  I3C_IBI_READ,
} I2C_MODE;

typedef struct {
  UINT32      BusFrequency; // kHZ
  UINT32      SlaveAddress;
  I2C_MODE    Mode;
  UINT32      SlaveMaxClockStretch; // us
  UINT32      CoreConfiguration1;
  UINT32      CoreConfiguration2;
} I2C_SLAVE_CONFIG;

typedef struct {
  UINT8   *Buffer;
  UINT32  Length;
  UINT32  Flags;
} I2C_DESCRIPTOR;

#define I2C_DXE_REVISION 0x0000000000010000
typedef struct _EFI_QCOM_I2C_PROTOCOL EFI_QCOM_I2C_PROTOCOL;
extern EFI_GUID gQcomI2CProtocolGuid;

typedef
VOID
(*I2C_CALLBACK) (
  IN UINT32 TransferStatus,
  IN UINT32 Transferred,
  IN VOID   *Context
  );

typedef
I2C_STATUS
(EFIAPI *EFI_I2C_OPEN) (
  IN UINT32 Instance,
  OUT VOID  **I2cHandle
  );

typedef
I2C_STATUS
(EFIAPI *EFI_I2C_READ) (
  IN VOID             *I2cHandle,
  IN I2C_SLAVE_CONFIG *Config,
  IN UINT16           Offset,
  IN UINT16           OffsetLength,
  OUT UINT8           *Buffer,
  IN UINT16           BufferLength,
  OUT UINT32          *Read,
  IN UINT32           Timeout // ms
  );

typedef
I2C_STATUS
(EFIAPI *EFI_I2C_WRITE) (
  IN VOID             *I2cHandle,
  IN I2C_SLAVE_CONFIG *Config,
  IN UINT16           Offset,
  IN UINT16           OffsetLength,
  IN UINT8            *Buffer,
  IN UINT16           BufferLength,
  OUT UINT32          *Written,
  IN UINT32           Timeout // ms
  );

typedef
I2C_STATUS
(EFIAPI *EFI_I2C_TRANSFER) (
  IN VOID             *I2cHandle,
  IN I2C_SLAVE_CONFIG *Config,
  IN I2C_DESCRIPTOR   *Descriptor,
  IN UINT16           NumDescriptors,
  IN I2C_CALLBACK     Callback,
  IN VOID             *Context   OPTIONAL,
  IN UINT32           Delay, // us
  OUT UINT32          *Transfered
  );

typedef
I2C_STATUS
(EFIAPI *EFI_I2C_CLOSE) (
  IN VOID *I2cHandle
  );

struct _EFI_QCOM_I2C_PROTOCOL{
  UINT64           Revision;
  EFI_I2C_OPEN     Open;
  EFI_I2C_READ     Read;
  EFI_I2C_WRITE    Write;
  EFI_I2C_TRANSFER Transfer;
  EFI_I2C_CLOSE    Close;
};

#endif
