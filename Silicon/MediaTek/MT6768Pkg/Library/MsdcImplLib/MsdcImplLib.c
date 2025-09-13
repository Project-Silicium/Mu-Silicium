#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/MsdcImplLib.h>

#include <Protocol/MtkGpio.h>

MSDC_PLATFORM_INFO PlatformInfo = {
  .NumberOfHosts = 1,
  .UseTop = TRUE,
  .MsdcPadTuneReg = 0xf0,
  .TuningStep = {32, 64},
  .AsyncFifo = TRUE,
  .BusyCheck = TRUE,
  .StopClkFix = TRUE,
  .EnhanceRx = TRUE
};

STATIC MTK_GPIO_PROTOCOL *mGpio = NULL;

VOID
GetSourceClockRate (
  UINT32 Index,
  UINTN *Hz)
{
  // TODO: Unhardcode after writing clock driver
  *Hz = Index == 0 ? 192000000 : 384000000;
}

VOID
SourceClockControl (
  UINT32 Index,
  BOOLEAN Enable)
{
  // TODO: Implement after writing clock driver
}

VOID
ClockControl (
  UINT32 Index,
  BOOLEAN Enable)
{
  // TODO: Implement after writing clock driver
}

VOID
PowerControl (
  UINT32 Index,
  BOOLEAN Enable)
{
  // TODO: Implement after adding ldo control to pmic driver
}

VOID
InitGpio (
  UINT32 Index)
{
  EFI_STATUS Status;

  if (mGpio == NULL) {
    Status = gBS->LocateProtocol (&gMediaTekGpioProtocolGuid, NULL, (VOID **)&mGpio);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to Locate GPIO Protocol! Status = %r\n", Status));
      ASSERT_EFI_ERROR (Status);
    }
  }

  if (Index == 0)
  {
    mGpio->SetMode (123, 1); // DAT0
    mGpio->SetMode (128, 1); // DAT1
    mGpio->SetMode (125, 1); // DAT2
    mGpio->SetMode (132, 1); // DAT3
    mGpio->SetMode (126, 1); // DAT4
    mGpio->SetMode (129, 1); // DAT5
    mGpio->SetMode (127, 1); // DAT6
    mGpio->SetMode (130, 1); // DAT7
    mGpio->SetMode (122, 1); // CMD
    mGpio->SetMode (124, 1); // CLK
  }
}