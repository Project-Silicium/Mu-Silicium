#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/IoLib.h>

#include <Library/GpioImplLib.h>

#include <Protocol/MtkGpio.h>

#define PIN_OFFSET(pin) (((pin) / 32) * 0x10)

#define PIN_MODE_OFFSET(pin) (((pin) / 8) * 0x10)
#define PIN_MODE_BIT(pin) (((pin) % 8) * 4)

STATIC EFI_MEMORY_REGION_DESCRIPTOR mPinctrlRegion;

STATIC
UINT32
GpioRead(
  IN  UINT32 Reg)
{
  return MmioRead32 (mPinctrlRegion.Address + Reg);
}

STATIC
VOID
GpioWrite(
  IN  UINT32 Reg,
  IN  UINT32 Value)
{
  MmioWrite32 (mPinctrlRegion.Address + Reg, Value);
}

EFI_STATUS
GpioGetDir(
  IN  UINT32   Pin,
  OUT BOOLEAN *Direction)
{
  UINT32 Value;

  if (Pin > gPlatformInfo.MaxPin)
    return EFI_INVALID_PARAMETER;

  // Read Pin Direction
  Value = GpioRead (gPlatformInfo.DirOffset + PIN_OFFSET(Pin));
  *Direction = !!(Value & (1 << (Pin % 32)));

  return EFI_SUCCESS;
}

EFI_STATUS
GpioSetDir(
  IN UINT32  Pin,
  IN BOOLEAN Direction)
{
  UINT32 Value, Offset;

  if (Pin > gPlatformInfo.MaxPin)
    return EFI_INVALID_PARAMETER;

  // Set Pin Direction
  Offset = PIN_OFFSET (Pin) + (Direction ? gPlatformInfo.ResetOffset : gPlatformInfo.SetOffset);
  Value = GpioRead (Offset);
  Value |= (1 << (Pin % 32));
  GpioWrite (Offset, Value);

  return EFI_SUCCESS;
}

EFI_STATUS
GpioGetState(
  IN  UINT32   Pin,
  OUT BOOLEAN *State)
{
  UINT32 Value;
  BOOLEAN Direction;

  if (Pin > gPlatformInfo.MaxPin)
    return EFI_INVALID_PARAMETER;

  // Read Pin State
  GpioGetDir (Pin, &Direction);
  Value = GpioRead ((Direction ? gPlatformInfo.DataInOffset : gPlatformInfo.DataOutOffset) + PIN_OFFSET(Pin));
  *State = !!(Value & (1 << (Pin % 32)));

  return EFI_SUCCESS;
}

EFI_STATUS
GpioSetState(
  IN UINT32  Pin,
  IN BOOLEAN State)
{
  UINT32 Value, Offset;

  if (Pin > gPlatformInfo.MaxPin)
    return EFI_INVALID_PARAMETER;

  // Set Pin State
  Offset = gPlatformInfo.DataOutOffset + (State ? gPlatformInfo.SetOffset : gPlatformInfo.ResetOffset) + PIN_OFFSET(Pin);
  Value = GpioRead (Offset);
  Value |= (1 << (Pin % 32));
  GpioWrite (Offset, Value);

  return EFI_SUCCESS;
}

EFI_STATUS
GpioSetMode(
  IN UINT32 Pin,
  IN UINT32 Mode)
{
  UINT32 SetValue, ResetValue, Offset;
  UINT32 ModeBits = (Mode << PIN_MODE_BIT (Pin));

  if (Pin > gPlatformInfo.MaxPin)
    return EFI_INVALID_PARAMETER;

  Offset = gPlatformInfo.ModeOffset + PIN_MODE_OFFSET (Pin);

  // Read current Mode
  SetValue = GpioRead (Offset + gPlatformInfo.SetOffset);
  ResetValue = GpioRead (Offset + gPlatformInfo.ResetOffset);

  // Set new Mode
  SetValue |= ModeBits;
  ResetValue |= (~ModeBits) & (0x7 << PIN_MODE_BIT (Pin));

  // Write new Mode
  GpioWrite (Offset + gPlatformInfo.SetOffset, SetValue);
  GpioWrite (Offset + gPlatformInfo.ResetOffset, ResetValue);

  return EFI_SUCCESS;
}

STATIC MTK_GPIO_PROTOCOL mGpio = {
  GpioGetDir,
  GpioSetDir,
  GpioGetState,
  GpioSetState,
  GpioSetMode
};

EFI_STATUS
EFIAPI
InitGpioDriver (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Locate Pinctrl Memory Region
  Status = LocateMemoryRegionByName ("Pinctrl", &mPinctrlRegion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate Pinctrl Memory Region! Status = %r\n", Status));
    return Status;
  }

  // Register GPIO Protocol
  Status = gBS->InstallMultipleProtocolInterfaces (&ImageHandle, &gMediaTekGpioProtocolGuid, &mGpio, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register Gpio Protocol! Status = %r\n", Status));
    return Status;
  }

  return Status;
}