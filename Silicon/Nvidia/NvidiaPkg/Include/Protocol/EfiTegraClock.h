#ifndef _EFI_TEGRA_CLOCK_H_
#define _EFI_TEGRA_CLOCK_H_

#include <TegraClockIDs.h>

//
// Global GUID for the Tegra Clock Protocol
//
#define EFI_TEGRA_CLOCK_PROTOCOL_GUID { 0x870380F6, 0x3013, 0x4F09, { 0x8F, 0x99, 0x82, 0xC5, 0x70, 0x18, 0x52, 0xE0 } }

//
// Declare forward reference to the Tegra Clock Protocol
//
typedef struct _EFI_TEGRA_CLOCK_PROTOCOL EFI_TEGRA_CLOCK_PROTOCOL;

/**
  This Function Retuns the Rate of the Defined Clock.

  @param[in]  ClockID         - The ID of the Clock.
  @param[out] ClockRate       - The Rate of the Clock.

  @return EFI_SUCCESS         - Successfully Got the Clock Rate.
  @return EFI_NOT_FOUND       - The Defined Clock ID does not Exist on this Device.
  @return EFI_DEVICE_ERROR    - Failed to Get PLL for defined Clock.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_GET_CLOCK_RATE) (
  IN  UINT8   ClockID,
  OUT UINT32 *ClockRate
  );

//
// Define Protocol Functions
//
struct _EFI_TEGRA_CLOCK_PROTOCOL {
  EFI_GET_CLOCK_RATE GetClockRate;
};

extern EFI_GUID gEfiTegraClockProtocolGuid;

#endif /* _EFI_TEGRA_CLOCK_H_ */