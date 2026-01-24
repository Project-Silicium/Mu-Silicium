#ifndef _EFI_USB_MSD_H_
#define _EFI_USB_MSD_H_

#include <Protocol/BlockIo.h>
#include <Protocol/EFIUsbfnIo.h>

//
// Global GUID for the USB Mass Storage Device Protocol
//
#define EFI_USB_MSD_PROTOCOL_GUID { 0xC8591FAF, 0xDBCC, 0x479E, { 0x9E, 0xF2, 0xFD, 0x08, 0x5B, 0xC3, 0x7B, 0xC7 } }

//
// Declare forward reference to the USB Mass Storage Device Protocol
//
typedef struct _EFI_USB_MSD_PROTOCOL EFI_USB_MSD_PROTOCOL;

/**
  This Function Assigns a BLKIO Protocol for MSD Usage.

  @param[in] This                          - Pointer to this Protocol.
  @param[in] BlkIo                         - Pointer to the BLKIO Protocol.
  @param[in] Lun                           - The Slot to Assign to.

  @return EFI_SUCCESS                      - Successfully Assigned BLKIO Protocol.
  @return EFI_INVALID_PARAMETER            - The "This" Argument is NULL.
  ...
**/
typedef
EFI_STATUS
(EFIAPI *EFI_ASSIGN_BLK_IO_HANDLE) (
  IN EFI_USB_MSD_PROTOCOL  *This,
  IN EFI_BLOCK_IO_PROTOCOL *BlkIo,
  IN UINT32                 Lun
  );

/**
  This Function gets the Max Count of UFS LUNs.

  @param[in]  This                         - Pointer to this Protocol.
  @param[out] Count                        - Number of UFS LUNs.

  @return EFI_SUCCESS                      - Successfully Assigned BLKIO Protocol.
  @return EFI_INVALID_PARAMETER            - One or more Arguments are NULL.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_QUERY_MAX_LUN) (
  IN  EFI_USB_MSD_PROTOCOL *This,
  OUT UINT8                *Count
  );

/**
  This Function Handles the USB Events during MSD Usage.

  @param[in] This                          - Pointer to this Protocol.

  @return EFI_SUCCESS                      - Successfully Handled USB Events.
  @return EFI_INVALID_PARAMETER            - The "This" Argument is NULL.
  @return EFI_PROTOCOL_ERROR               - ?
  @return EFI_NOT_READY                    - ?
  ...
**/
typedef
EFI_STATUS
(EFIAPI *EFI_EVENT_HANDLER) (
  IN EFI_USB_MSD_PROTOCOL *This
  );

/**
  This Function Starts the USB MSD Device.

  @param[in] This                          - Pointer to this Protocol.

  @return EFI_SUCCESS                      - Successfully Started the USB MSD Device.
  @return EFI_INVALID_PARAMETER            - The "This" Argument is NULL.
  ...
**/
typedef
EFI_STATUS
(EFIAPI *EFI_START_DEVICE) (
  IN EFI_USB_MSD_PROTOCOL *This
  );

/**
  This Function Stops the USB MSD Device.

  @param[in] This                          - Pointer to this Protocol.

  @return EFI_SUCCESS                      - Successfully Stoped the USB MSD Device.
  @return EFI_INVALID_PARAMETER            - The "This" Argument is NULL.
  @return EFI_NOT_READY                    - The USB MSD Device is not Running.
  @return EFI_PROTOCOL_ERROR               - ?
  ...
**/
typedef
EFI_STATUS
(EFIAPI *EFI_STOP_DEVICE) (
  IN EFI_USB_MSD_PROTOCOL *This
  );

/**
  This Function gets the USB Device Speed.

  @param[in]  This                         - Pointer to this Protocol.
  @param[out] Speed                        - The USB Device Speed.

  @return EFI_SUCCESS                      - Successfully got USB Device Speed.
  @return EFI_INVALID_PARAMETER            - One or more Arguments are NULL.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_GET_DEVICE_SPEED) (
  IN  EFI_USB_MSD_PROTOCOL *This,
  OUT EFI_USB_BUS_SPEED    *Speed
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
  EFI_GET_DEVICE_SPEED      GetDeviceSpeed;
  VOID                     *UnmountHandle;
  VOID                     *MountHandle;
  VOID                     *FindPartitions;
};

extern EFI_GUID gEfiUsbMsdProtocolGuid;

#endif /* _EFI_USB_MSD_H_ */
