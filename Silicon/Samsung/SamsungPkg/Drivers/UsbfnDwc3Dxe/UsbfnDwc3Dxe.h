#ifndef _USBFN_DWC3_DXE_H_
#define _USBFN_DWC3_DXE_H_

#include <Uefi.h>

#include <Device/Dwc3.h>
#include <Library/PlatformUsbfnLib.h>

#include <Protocol/UsbFunctionIo.h>

#define DWC3_MAX_PHYSICAL_EP      16
#define DWC3_MAX_LOGICAL_EP       (DWC3_MAX_PHYSICAL_EP / 2)

#define DWC3_PHYS_EP(EpIndex, Direction) \
  (((EpIndex) << 1) | (((Direction) == EfiUsbEndpointDirectionDeviceTx) ? 1 : 0))

#define DWC3_EP0_OUT              0
#define DWC3_EP0_IN               1

#define DWC3_EVENT_BUFFER_SIZE    SIZE_4KB

#define DWC3_TRB_PER_EP           4

#define DWC3_EP0_BOUNCE_SIZE      SIZE_1KB

#define DWC3_MAX_TRANSFER_SIZE    (DWC3_TRB_SIZE_MASK & ~(SIZE_4KB - 1))

#define DWC3_RESET_TIMEOUT_US     500000
#define DWC3_CMD_TIMEOUT_US       500000
#define DWC3_RUNSTOP_TIMEOUT_US   500000

#define DWC3_START_XFER_ATTEMPTS  3

#define DWC3_EP0_ARM_CONFIGURE    1
#define DWC3_EP0_ARM_BUS_RESET    2
#define DWC3_EP0_ARM_STATUS_DONE  3
#define DWC3_EP0_ARM_STALL        4
#define DWC3_EP0_ARM_CONNECT_DONE 5

#define DWC3_MESSAGE_QUEUE_DEPTH  32

#define DWC3_EVENT_TRACE_BUDGET   48

#define DWC3_MAX_TRANSFER_BUFFERS 16

#define DWC3_DEV_SIGNATURE  SIGNATURE_32 ('D', 'W', 'C', '3')
#define DWC3_DEV_FROM_PROTOCOL(a) \
  CR (a, DWC3_DEV, UsbfnIo, DWC3_DEV_SIGNATURE)

typedef struct {
  EFI_USBFN_MESSAGE         Message;
  EFI_USBFN_MESSAGE_PAYLOAD Payload;
} DWC3_MESSAGE;

typedef struct {
  VOID  *Buffer;
  UINTN  Pages;
} DWC3_TRANSFER_BUFFER;

typedef enum {
  Dwc3Ep0StateSetup = 0,
  Dwc3Ep0StateData,
  Dwc3Ep0StateStatus
} DWC3_EP0_STATE;

typedef struct {
  BOOLEAN                      Enabled;
  BOOLEAN                      Stalled;
  BOOLEAN                      TransferStarted;

  UINT8                        PhysEpNum;
  UINT8                        EpType;
  UINT16                       MaxPacketSize;
  UINT8                        ResourceIndex;

  DWC3_TRB                    *TrbPool;
  EFI_PHYSICAL_ADDRESS         TrbPoolPhys;

  VOID                        *TransferBuffer;
  UINTN                        TransferTrbLength;

  BOOLEAN                      ZeroLengthTermination;
} DWC3_ENDPOINT;

typedef struct {
  UINT32                       Signature;
  EFI_USBFN_IO_PROTOCOL        UsbfnIo;
  EFI_HANDLE                   Handle;

  USBFN_PLATFORM_DATA          Platform;
  UINTN                        Base;
  BOOLEAN                      IsUsb31;
  UINT8                        NumPhysicalEps;

  BOOLEAN                      Started;
  BOOLEAN                      Suspended;
  UINT8                        LinkState;
  EFI_USB_BUS_SPEED            Speed;

  UINT8                       *EventBuffer;
  EFI_PHYSICAL_ADDRESS         EventBufferPhys;
  UINT32                       EventPos;
  UINT32                       EventTraceBudget;

  UINT8                       *Ep0Buffer;
  EFI_PHYSICAL_ADDRESS         Ep0BufferPhys;
  DWC3_EP0_STATE               Ep0State;

  BOOLEAN                      ThreeStageSetup;

  BOOLEAN                      Ep0ExpectIn;
  BOOLEAN                      Ep0StatusStarted;
  EFI_USB_DEVICE_REQUEST       SetupPacket;
  BOOLEAN                      Ep0Bounced;
  VOID                        *Ep0BounceTarget;
  UINTN                        Ep0BounceLength;

  DWC3_ENDPOINT                Endpoints[DWC3_MAX_PHYSICAL_EP];
  DWC3_TRANSFER_BUFFER         TransferBuffers[DWC3_MAX_TRANSFER_BUFFERS];

  DWC3_MESSAGE                 Messages[DWC3_MESSAGE_QUEUE_DEPTH];
  UINT32                       MessageHead;
  UINT32                       MessageTail;
} DWC3_DEV;

EFI_STATUS
Dwc3CoreInit (IN DWC3_DEV *Dev);

VOID
Dwc3CoreExit (IN DWC3_DEV *Dev);

EFI_STATUS
Dwc3RunStop (
  IN DWC3_DEV *Dev,
  IN BOOLEAN   Run
  );

VOID
Dwc3EventBufferSetup (IN DWC3_DEV *Dev);

VOID
Dwc3EventBufferCleanup (IN DWC3_DEV *Dev);

VOID *
Dwc3AllocateDmaBuffer (
  IN  UINTN                 Size,
  OUT EFI_PHYSICAL_ADDRESS *PhysicalAddress
  );

VOID
Dwc3FreeDmaBuffer (
  IN VOID  *Buffer,
  IN UINTN  Size
  );

EFI_STATUS
Dwc3SendEpCommand (
  IN  DWC3_DEV *Dev,
  IN  UINT8     PhysEpNum,
  IN  UINT32    Command,
  IN  UINT32    Param0,
  IN  UINT32    Param1,
  IN  UINT32    Param2,
  OUT UINT32   *CommandRegister OPTIONAL
  );

EFI_STATUS
Dwc3EnableEndpoint (
  IN DWC3_DEV *Dev,
  IN UINT8     PhysEpNum,
  IN UINT8     EpType,
  IN UINT16    MaxPacketSize,
  IN BOOLEAN   Modify
  );

VOID
Dwc3DisableEndpoint (
  IN DWC3_DEV *Dev,
  IN UINT8     PhysEpNum
  );

EFI_STATUS
Dwc3StartTransfer (
  IN DWC3_DEV *Dev,
  IN UINT8     PhysEpNum,
  IN UINT64    BufferPhys,
  IN UINT32    Length,
  IN UINT32    TrbType
  );

VOID
Dwc3EndTransfer (
  IN DWC3_DEV *Dev,
  IN UINT8     PhysEpNum
  );

EFI_STATUS
Dwc3SetHalt (
  IN DWC3_DEV *Dev,
  IN UINT8     PhysEpNum,
  IN BOOLEAN   Stall
  );

VOID
Dwc3Ep0OutStart (IN DWC3_DEV *Dev);

VOID
Dwc3Ep0ResetState (IN DWC3_DEV *Dev);

VOID
Dwc3Ep0StallAndRestart (IN DWC3_DEV *Dev);

EFI_STATUS
Dwc3ProcessEvents (IN DWC3_DEV *Dev);

BOOLEAN
Dwc3PopMessage (
  IN  DWC3_DEV     *Dev,
  OUT DWC3_MESSAGE *Message
  );

#endif /* _USBFNDWC3DXE_H_ */
