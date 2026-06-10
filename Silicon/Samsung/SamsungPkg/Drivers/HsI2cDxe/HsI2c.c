#include <Library/DebugLib.h>
#include <Library/MemoryAllocationHelperLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/HsI2cLib.h>
#include <Library/IoLib.h>

#include <Protocol/EFIUsi.h>
#include <Protocol/EFIGpio.h>
//#include <Protocol/EFIHsI2c.h>

#include "HsI2c.h"

//
// Global Protocols
//
STATIC EFI_USI_PROTOCOL  *mUsiProtocol;
STATIC EFI_GPIO_PROTOCOL *mGpioProtocol;

//
// Global Variables
//
STATIC EFI_HSI2C_BUS_DATA *BusData;
STATIC UINT8               BusCount;

EFI_STATUS
SetupBusGpio (IN EFI_HSI2C_GPIO_DATA *Gpio)
{
  EFI_STATUS Status;

  // Configure SCL GPIO Pin
  Status = mGpioProtocol->SetPinFunction (Gpio->SclBankId, Gpio->SclBankNum, Gpio->SclPin, Gpio->Function);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Configure SDA GPIO Pin
  Status = mGpioProtocol->SetPinFunction (Gpio->SdaBankId, Gpio->SdaBankNum, Gpio->SdaPin, Gpio->Function);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

VOID
ResetConfig (IN EFI_HSI2C_BUS *Bus)
{
  UINT32 Config;

  // Get Current HSI2C Config
  Config = MmioRead32 ((UINTN)&Bus->conf);

  // Re-set Master & Trailing Count
  MmioWrite32 ((UINTN)&Bus->ctrl,       HSI2C_MASTER);
  MmioWrite32 ((UINTN)&Bus->trail_ctrl, HSI2C_TRAILING_COUNT);

  // HIGH_SPD Check here.

  // Set Auto Mode
  Config |= HSI2C_AUTO_MODE;

  // Write new HSI2C Config
  MmioWrite32 ((UINTN)&Bus->conf, Config);
}

VOID
SetTiming (
  IN EFI_HSI2C_BUS        *Bus,
  IN EFI_HSI2C_CLOCK_DATA *Clock)
{
  UINT32 op_clk;
  UINT32 utemp;
  UINT32 fs_div;

  // Save Clock Frequency
  UINT32 ClockFreq = Clock->Freq;

  switch ((UINT8)Clock->SpeedMode) {
    case FAST_SPD:
      op_clk = 400000;
      fs_div = ClockFreq / (op_clk * 15);
      fs_div &= 0xFF;

      utemp = MmioRead32 ((UINTN)&Bus->timing_fs3);
      utemp &= ~0x00FF0000;
      utemp |= fs_div << 16;
      MmioWrite32 ((UINTN)&Bus->timing_fs3, utemp);

      utemp = MmioRead32 ((UINTN)&Bus->timing_fs1);
      utemp &= ~0x00FF0000;
      MmioWrite32 ((UINTN)&Bus->timing_fs1, utemp);
  }
}

VOID
ResetController (IN EFI_HSI2C_BUS *Bus)
{
  // Reset Controller
  MmioOr32  ((UINTN)&Bus->ctrl,  HSI2C_SW_RST);
  MmioAnd32 ((UINTN)&Bus->ctrl, ~HSI2C_SW_RST);
}

EFI_STATUS
HsI2cInitBus (IN UINT8 BusNumber)
{
  EFI_STATUS           Status;
  EFI_PHYSICAL_ADDRESS BusAddress;

  // Verify Bus Number
  if (BusNumber >= BusCount) {
    return EFI_NOT_FOUND;
  }

  // Check Init Flag
  if (BusData[BusNumber].Initialized) {
    return EFI_SUCCESS;
  }

  // Get HSI2C Controller Address
  Status = mUsiProtocol->GetControllerAddr (BusNumber, BUS_I2C, &BusAddress);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Map HSI2C Bus Memory
  Status = MapMemoryRegion (BusAddress, HSI2C_MMIO_LENGTH, EfiMemoryMappedIO);
  if (EFI_ERROR (Status)) {
    return EFI_ALREADY_STARTED;
  }

  DEBUG ((EFI_D_WARN, "Memory Setup Done\n"));

  // Setup HSI2C Bus GPIO
  Status = SetupBusGpio (&BusData[BusNumber].Gpio);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DEBUG ((EFI_D_WARN, "GPIO Setup Done\n"));

  // Set USI Mode
  Status = mUsiProtocol->SetMode (BusAddress, MODE_I2C);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Get HSI2C Bus
  EFI_HSI2C_BUS *Bus = (EFI_HSI2C_BUS *)BusAddress;

  // Reset USI CON
  MmioWrite32 ((UINTN)&Bus->usi_con, 0x0);

  DEBUG ((EFI_D_WARN, "USI Setup Done\n"));

  // Reset HSI2C Controller
  ResetController (Bus);

  DEBUG ((EFI_D_WARN, "Controller Reset Done\n"));

  // Set HSI2C Timing
  SetTiming (Bus, &BusData[BusNumber].Clock);

  DEBUG ((EFI_D_WARN, "Timing Setup Done\n"));

  // Reset HSI2C Config
  ResetConfig (Bus);

  DEBUG ((EFI_D_WARN, "Config Reset Done\n"));

  // Toggle Init Flag
  BusData[BusNumber].Initialized = TRUE;

  return EFI_SUCCESS;
}

/*
STATIC EFI_HSI2C_PROTOCOL mHsI2c = {
  HsI2cInitBus,
  HsI2cRead,
  HsI2cWrite
};
*/

EFI_STATUS
EFIAPI
RegisterHsI2c (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Get HSI2C Bus Data
  GetHsI2cBusData (&BusData, &BusCount);

  // Verify Bus Count
  if (!BusCount) {
    return EFI_UNSUPPORTED;
  }

  // Locate USI Protocol
  Status = gBS->LocateProtocol (&gEfiUsiProtocolGuid, NULL, (VOID *)&mUsiProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate USI Protocol!\n"));
    return Status;
  }

  // Locate GPIO Protocol
  Status = gBS->LocateProtocol (&gEfiGpioProtocolGuid, NULL, (VOID *)&mGpioProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate GPIO Protocol!\n"));
    return Status;
  }

  /*
  // Register HSI2C Protocol
  Status = gBS->InstallProtocolInterface (&ImageHandle, &gEfiHsI2cProtocolGuid, EFI_NATIVE_INTERFACE, &mHsI2c);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register HSI2C Protocol!\n"));
    return Status;
  }
  */

  return EFI_SUCCESS;
}
