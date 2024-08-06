#ifndef _EFI_USB_MSD_H_
#define _EFI_USB_MSD_H_

#include <Protocol/BlockIo.h>

//
// Global GUID for the USB Mass Storage Device Protocol
//
#define EFI_USB_MSD_PROTOCOL_GUID { 0xC8591FAF, 0xDBCC, 0x479E, { 0x9E, 0xF2, 0xFD, 0x08, 0x5B, 0xC3, 0x7B, 0xC7 } }

//
// Declare forward reference to the USB Mass Storage Device Protocol
//
typedef struct _EFI_USB_MSD_PROTOCOL EFI_USB_MSD_PROTOCOL;

/**
  This Function Assigns a New BLK IO Protocol.

  @param[in] This          - The USB Mass Storage Device Protocol.
  @param[in] BlkIo         - The BLK IO Protocol.
  @param[in] Lun           - The LUN.

  @return    Status        - The EFI_STATUS returned by the Function.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_ASSIGN_BLK_IO_HANDLE) (
  IN EFI_USB_MSD_PROTOCOL  *This,
  IN EFI_BLOCK_IO_PROTOCOL *BlkIo,
  IN UINT32                 Lun
  );

/**
  This Function Gets the Max Lun's Value.

  NOTE: I think here should be some Parameters, But I didn't find any.

  @return Status           - The EFI_STATUS returned by the Function.         
**/
typedef
EFI_STATUS
(EFIAPI *EFI_QUERY_MAX_LUN) (VOID);

/**
  This Function Does something with Event Handling (Not Sure).

  @param[in] This          - The USB Mass Storage Device Protocol.

  @return    Status        - The EFI_STATUS returned by the Function.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_EVENT_HANDLER) (
  IN EFI_USB_MSD_PROTOCOL *This
  );

/**
  This Function Starts the USB Mass Storage Device.

  @param[in] This          - The USB Mass Storage Device Protocol.

  @return    Status        - The EFI_STATUS returned by the Function.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_START_DEVICE) (
  IN EFI_USB_MSD_PROTOCOL *This
  );

/**
  This Function Stops the USB Mass Storage Device.

  @param[in] This          - The USB Mass Storage Device Protocol.

  @return    Status        - The EFI_STATUS returned by the Function.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_STOP_DEVICE) (
  IN EFI_USB_MSD_PROTOCOL *This
  );

//
// Define Protocol Functions
//
struct _EFI_USB_MSD_PROTOCOL {
  UINT32                    Revision;
  EFI_ASSIGN_BLK_IO_HANDLE  AssignBlkIoHandle;
  EFI_QUERY_MAX_LUN         QueryMaxLun;
  EFI_EVENT_HANDLER         EventHandler;
  EFI_START_DEVICE          StartDevice;
  EFI_STOP_DEVICE           StopDevice;

  // NOTE: There are maybe more Functions.
};

extern EFI_GUID gEfiUsbMsdProtocolGuid;

#endif /* _EFI_USB_MSD_H_ */
