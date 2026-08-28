#ifndef _PLATFORM_USBFN_LIB_H_
#define _PLATFORM_USBFN_LIB_H_

#include <Uefi.h>
#include <Protocol/UsbFunctionIo.h>

//
// UTMI+ Data Bus Width of the attached USB 2.0 PHY.
//
typedef enum {
  UsbfnUtmiWidth8Bit = 0,
  UsbfnUtmiWidth16Bit
} USBFN_UTMI_WIDTH;

//
// Everything the generic DWC3 Driver needs to know about the SoC it runs on.
//
// Each S5EXXXXPkg fills this in from its own PlatformUsbfnLib instance, which
// keeps every Address, Clock and PHY Quirk out of the Controller Driver.
//
typedef struct {
  //
  // MMIO Base of the DWC3 Controller. The Global Register Block lives at
  // Offset 0xC100 of this Address.
  //
  EFI_PHYSICAL_ADDRESS ControllerBase;

  //
  // Highest Bus Speed the SoC Integration is able to run at. The Driver
  // never negotiates above this. USB 3.x requires the Combo PHY to be
  // brought up, so most Platforms report UsbBusSpeedHigh here.
  //
  EFI_USB_BUS_SPEED    MaximumSpeed;

  //
  // UTMI+ Interface Width. Selects GUSB2PHYCFG.PHYIF and the matching
  // USBTRDTIM Turnaround Time.
  //
  USBFN_UTMI_WIDTH     UtmiWidth;

  //
  // Clear when the UTMI+ PHY has no free running Clock, which makes the
  // Driver clear GUSB2PHYCFG.U2_FREECLK_EXISTS. Getting this wrong upsets
  // Frame Timing and Suspend Handling.
  //
  BOOLEAN              Usb2FreeClockExists;

  //
  // GFLADJ. The Reference Clock feeding the Controller is rarely the 30 MHz
  // the Reset Value assumes, and High Speed Frame Timing drifts if it is
  // left alone. The Driver applies Value over Mask when Mask is non zero.
  //
  UINT32               FrameLengthAdjustMask;
  UINT32               FrameLengthAdjustValue;

  //
  // Set when the SuperSpeed PHY is not brought up. The Driver then leaves
  // GUSB3PIPECTL.SUSPHY set and skips the PIPE3 PHY Soft Reset.
  //
  BOOLEAN              SuperSpeedPhyDisabled;

  //
  // Number of Physical Endpoints (2 * Logical Endpoints) the Controller
  // implements. Read from GHWPARAMS3 when left at 0.
  //
  UINT8                NumEndpoints;

  //
  // USB Device Descriptor Identity reported through GetVendorIdProductId().
  //
  UINT16               VendorId;
  UINT16               ProductId;
} USBFN_PLATFORM_DATA;

/**
  Returns the SoC Specific Configuration of the DWC3 Controller.

  @param[out] PlatformData                 - The SoC Specific Configuration.

  @return EFI_SUCCESS                      - Successfully returned the Configuration.
  @return EFI_INVALID_PARAMETER            - The "PlatformData" Parameter is NULL.
  @return EFI_UNSUPPORTED                  - The SoC has no USB Function Controller.
**/
EFI_STATUS
UsbfnPlatformGetData (OUT USBFN_PLATFORM_DATA *PlatformData);

/**
  Powers up the USB Block and brings the PHY out of Reset.

  This is called before the Controller is touched, so it has to take care of
  Power Domains, Clocks, PHY Isolation and the PHY Bring-up Sequence itself.

  @return EFI_SUCCESS                      - The PHY is ready.
  @return EFI_DEVICE_ERROR                 - The PHY failed to come up.
**/
EFI_STATUS
UsbfnPlatformPhyInit (VOID);

/**
  Puts the PHY back into Reset and re-isolates the USB Block.

  @return EFI_SUCCESS                      - The PHY was shut down.
**/
EFI_STATUS
UsbfnPlatformPhyExit (VOID);

/**
  Tells whether a USB Host is currently attached.

  Platforms without a way to sample VBUS should return TRUE, in which case
  the Driver falls back to the Link State reported by the Controller.

  @return TRUE                             - VBUS is present.
  @return FALSE                            - VBUS is absent.
**/
BOOLEAN
UsbfnPlatformIsVbusPresent (VOID);

#endif /* _PLATFORM_USBFN_LIB_H_ */
