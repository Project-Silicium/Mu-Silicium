#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/MsdcImplLib.h>

#include <Protocol/MtkGpio.h>
#include <Protocol/MtkClock.h>
#include <Protocol/MtkPmic.h>

MSDC_PLATFORM_INFO gPlatformInfo = {
  .NumberOfHosts = 2,
  .UseTop = TRUE,
  .MsdcPadTuneReg = 0xf0,
  .TuningStep = {32, 64},
  .AsyncFifo = TRUE,
  .BusyCheck = TRUE,
  .StopClkFix = TRUE,
  .EnhanceRx = TRUE
};

STATIC MTK_GPIO_PROTOCOL *mGpio = NULL;
STATIC MTK_CLOCK_PROTOCOL *mClock = NULL;
STATIC MTK_PMIC_PROTOCOL *mPmic = NULL;

VOID
GetSourceClockRate (
  UINT32 Index,
  UINTN *Hz)
{
  EFI_STATUS Status;
  UINT32 ClockId;

  // Get Clock Id
  Status = mClock->GetId(Index == 0 ? "TOP_MSDC50_0" : "TOP_MSDC30_1", &ClockId);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Get Clock Id! Status = %r\n", Status));
    ASSERT_EFI_ERROR (Status);
  }

  // Get Clock Frequency
  Status = mClock->GetFrequency(ClockId, Hz);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Get Clock Frequency! Status = %r\n", Status));
    ASSERT_EFI_ERROR (Status);
  }
}

VOID
SourceClockControl (
  UINT32 Index,
  BOOLEAN Enable)
{
  EFI_STATUS Status;
  UINT32 ClockId;

  if (Index == 0) {
    // Get Clock Id
    Status = mClock->GetId("INFRA_FAES_FDE", &ClockId);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to Get Clock Id! Status = %r\n", Status));
      ASSERT_EFI_ERROR (Status);
    }

    // Enable Clock
    Status = mClock->SetEnable(ClockId, Enable);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to Enable Clock! Status = %r\n", Status));
      ASSERT_EFI_ERROR (Status);
    }
  }

  // Get Clock Id
  Status = mClock->GetId(Index == 0 ? "INFRA_MSDC0_SRC" : "INFRA_MSDC1_SRC", &ClockId);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Get Clock Id! Status = %r\n", Status));
    ASSERT_EFI_ERROR (Status);
  }

  // Enable Clock
  Status = mClock->SetEnable(ClockId, Enable);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Enable Clock! Status = %r\n", Status));
    ASSERT_EFI_ERROR (Status);
  }
}

VOID
ClockControl (
  UINT32 Index,
  BOOLEAN Enable)
{
  EFI_STATUS Status;
  UINT32 ClockId;

  // Get Clock Id
  Status = mClock->GetId(Index == 0 ? "TOP_MSDC50_0" : "TOP_MSDC30_1", &ClockId);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Get Clock Id! Status = %r\n", Status));
    ASSERT_EFI_ERROR (Status);
  }

  // Enable Clock
  Status = mClock->SetEnable(ClockId, Enable);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Enable Clock! Status = %r\n", Status));
    ASSERT_EFI_ERROR (Status);
  }

  // Get Clock Id
  Status = mClock->GetId(Index == 0 ? "INFRA_MSDC0" : "INFRA_MSDC1", &ClockId);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Get Clock Id! Status = %r\n", Status));
    ASSERT_EFI_ERROR (Status);
  }

  // Enable Clock
  Status = mClock->SetEnable(ClockId, Enable);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Enable Clock! Status = %r\n", Status));
    ASSERT_EFI_ERROR (Status);
  }
}

VOID
PowerControl (
  UINT32 Index,
  BOOLEAN Enable)
{
  EFI_STATUS Status;

  if (Index == 0 && FixedPcdGetBool(PcdStorageIsEMMC)) {
    // Enable VEMC LDO
    Status = mPmic->RegulatorSetEnable("ldo_vemc", Enable);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to Enable LDO VEMC! Status = %r\n", Status));
      ASSERT_EFI_ERROR (Status);
    }
  } else {
    // Enable VMCH LDO
    Status = mPmic->RegulatorSetEnable("ldo_vmch", Enable);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to Enable LDO VMCH! Status = %r\n", Status));
      ASSERT_EFI_ERROR (Status);
    }

    // Enable VMC LDO
    Status = mPmic->RegulatorSetEnable("ldo_vmc", Enable);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to Enable LDO VMC! Status = %r\n", Status));
      ASSERT_EFI_ERROR (Status);
    }
  }
}

VOID
InitGpio (
  UINT32 Index)
{
  if (Index == 0 && FixedPcdGetBool(PcdStorageIsEMMC)) {
    // Setup EMMC GPIO
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
  else {
    // Setup SDCard GPIO
    mGpio->SetMode (161, 1); // DAT0
    mGpio->SetMode (162, 1); // DAT1
    mGpio->SetMode (163, 1); // DAT2
    mGpio->SetMode (164, 1); // DAT3
    mGpio->SetMode (170, 1); // CMD
    mGpio->SetMode (171, 1); // CLK
  }
}

EFI_STATUS
EFIAPI
MsdcLibConstructor (VOID)
{
  EFI_STATUS Status;

  // Locate Gpio Protocol
  Status = gBS->LocateProtocol (&gMediaTekGpioProtocolGuid, NULL, (VOID **)&mGpio);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Locate GPIO Protocol! Status = %r\n", Status));
    ASSERT_EFI_ERROR (Status);
  }

  // Locate Clock Protocol
  Status = gBS->LocateProtocol (&gMediaTekClockProtocolGuid, NULL, (VOID **)&mClock);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Locate Clock Protocol! Status = %r\n", Status));
    ASSERT_EFI_ERROR (Status);
  }

  // Locate PMIC Protocol
  Status = gBS->LocateProtocol (&gMediaTekPmicProtocolGuid, NULL, (VOID **)&mPmic);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Locate PMIC Protocol! Status = %r\n", Status));
    ASSERT_EFI_ERROR (Status);
  }

  // Set VMCH LDO Voltage
  Status = mPmic->RegulatorSetVoltage("ldo_vmch", 3300000);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Set VMCH LDO Voltage! Status = %r\n", Status));
    ASSERT_EFI_ERROR (Status);
  }

  // Set VMC LDO Voltage
  Status = mPmic->RegulatorSetVoltage("ldo_vmc", 3300000);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Set VMC LDO Voltage! Status = %r\n", Status));
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}