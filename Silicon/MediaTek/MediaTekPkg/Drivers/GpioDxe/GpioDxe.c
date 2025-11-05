#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/TimerLib.h>
#include <Library/IoLib.h>

#include <Protocol/MtkGpio.h>
#include <Library/GpioImplLib.h>

#define PIN_OFFSET(pin) (((pin) / 32) * 0x10)

#define PIN_MODE_OFFSET(pin) (((pin) / 8) * 0x10)
#define PIN_MODE_BIT(pin) (((pin) % 8) * 4)

STATIC EFI_MEMORY_REGION_DESCRIPTOR_EX mPinctrlRegion;

UINT32
GpioRead(
  IN  UINT32 Reg)
{
  return MmioRead32 (mPinctrlRegion.Address + Reg);
}

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

  if (Pin > PlatformInfo.MaxPin)
    return EFI_INVALID_PARAMETER;

  Value = GpioRead (PlatformInfo.DirOffset + PIN_OFFSET(Pin));
  *Direction = (Value & (1 << (Pin % 32))) ? FALSE : TRUE;

  return EFI_SUCCESS;
}

EFI_STATUS
GpioSetDir(
  IN UINT32  Pin,
  IN BOOLEAN Direction)
{
  UINT32 Value, Offset;

  if (Pin > PlatformInfo.MaxPin)
    return EFI_INVALID_PARAMETER;

  Offset = PIN_OFFSET(Pin) + (Direction ? PlatformInfo.ResetOffset : PlatformInfo.SetOffset);
  Value = GpioRead (Offset);
  Value |= (1 << (Pin % 32));
  GpioWrite (Offset, Value);

  return EFI_SUCCESS;
}

EFI_STATUS
GpioGetPin(
  IN  UINT32   Pin,
  OUT BOOLEAN *State)
{
  UINT32 Value;
  BOOLEAN Direction;

  if (Pin > PlatformInfo.MaxPin)
    return EFI_INVALID_PARAMETER;

  GpioGetDir(Pin, &Direction);
  Value = GpioRead ((Direction ? PlatformInfo.DataInOffset : PlatformInfo.DataOutOffset) + PIN_OFFSET(Pin));
  *State = (Value & (1 << (Pin % 32))) ? TRUE : FALSE;

  return EFI_SUCCESS;
}

EFI_STATUS
GpioSetPin(
  IN UINT32  Pin,
  IN BOOLEAN State)
{
  UINT32 Value, Offset;

  if (Pin > PlatformInfo.MaxPin)
    return EFI_INVALID_PARAMETER;

  Offset = PlatformInfo.DataOutOffset + (State ? PlatformInfo.SetOffset : PlatformInfo.ResetOffset) + PIN_OFFSET(Pin);
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
  UINT32 ModeBits = (Mode << PIN_MODE_BIT(Pin));

  if (Pin > PlatformInfo.MaxPin)
    return EFI_INVALID_PARAMETER;

  Offset = PlatformInfo.ModeOffset + PIN_MODE_OFFSET(Pin);

  SetValue = GpioRead (Offset + PlatformInfo.SetOffset);
  ResetValue = GpioRead (Offset + PlatformInfo.ResetOffset);

  SetValue |= ModeBits;
  ResetValue |= (~ModeBits) & (0x7 << PIN_MODE_BIT(Pin));

  GpioWrite (Offset + PlatformInfo.SetOffset, SetValue);
  GpioWrite (Offset + PlatformInfo.ResetOffset, ResetValue);

  return EFI_SUCCESS;
}

STATIC MTK_GPIO_PROTOCOL mGpio = {
  GpioGetDir,
  GpioSetDir,
  GpioGetPin,
  GpioSetPin,
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
  Status = LocateMemoryMapAreaByName ("Pinctrl", &mPinctrlRegion);
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