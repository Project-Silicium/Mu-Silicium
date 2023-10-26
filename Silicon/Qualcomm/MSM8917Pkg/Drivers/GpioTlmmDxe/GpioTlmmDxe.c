#include <PiDxe.h>

#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/QcomGpioTlmm.h>

VOID
SetGpioConfig(
  UINT32  TlmmBaseAddr,
  UINT32  Gpio,
  UINT8   Func,
  UINT8   Pull,
  UINT8   Drvstr,
  BOOLEAN Enable)
{
  UINT32 Value = 0;

  Value |= Pull;
  Value |= Func << 2;
  Value |= Drvstr << 6;
  Value |= Enable << 9;

  MmioWrite32 (TlmmBaseAddr + 0x1004 + Gpio * 0x10, Value);
}

VOID
SetGpio(
  UINT32  TlmmBaseAddr,
  UINT32  Gpio,
  UINT32  Value)
{
  MmioWrite32 (TlmmBaseAddr + 0x4 + Gpio * 0x1000, Value);
}

UINT32
GetGpio(
  UINT32  TlmmBaseAddr,
  UINT32  Gpio)
{
  return MmioRead32 (TlmmBaseAddr + 0x4 + Gpio * 0x1000) & (1 << 0);
}

STATIC QCOM_GPIO_TLMM_PROTOCOL mInternalGpioTlmm = {
  SetGpioConfig,
  SetGpio,
  GetGpio
};

EFI_STATUS
EFIAPI
GpioTlmmDxeInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  Status = gBS->InstallMultipleProtocolInterfaces(&ImageHandle, &gQcomGpioTlmmProtocolGuid, &mInternalGpioTlmm, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Install Qcom GPIO Protocol! Status = %r\n", Status));
    ASSERT_EFI_ERROR(Status);
  }

  return Status;
}