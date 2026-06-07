#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/IoLib.h>

#include <Library/PmicWrapperImplLib.h>

#include <Protocol/MtkPmicWrapper.h>

typedef enum {
  WacsFsmIdle = 0,
  WacsFsmWfVldClr = 6,
} WACS_FSM_STATE;

STATIC EFI_MEMORY_REGION_DESCRIPTOR mPmicWrapperRegion;

STATIC
UINT32
PmicWrapperRead(
  IN  UINT16 Reg)
{
  return MmioRead32 (mPmicWrapperRegion.Address + gPlatformInfo.RegMap[Reg]);
}

STATIC
VOID
PmicWrapperWrite(
  IN  UINT16 Reg,
  IN  UINT32 Value)
{
  MmioWrite32 (mPmicWrapperRegion.Address + gPlatformInfo.RegMap[Reg], Value);
}

STATIC
WACS_FSM_STATE
WacsGetFsm ()
{
  // Read Data
  return (PmicWrapperRead (PmicWrapperWacs2RData) >> 16) & 0x7;
}

STATIC
VOID
WacsWaitFor (
  IN WACS_FSM_STATE Fsm)
{
  // Wait until FSM reaches requested state
  while (WacsGetFsm () != Fsm)
  {
    MicroSecondDelay (100);
  }
}

STATIC
VOID
WacsCommand (
  IN UINT32  Address,
  IN UINT32  Data,
  IN BOOLEAN IsWrite)
{
  // Wait until FSM reaches idle state
  WacsWaitFor (WacsFsmIdle);

  // Write Command
  PmicWrapperWrite (PmicWrapperWacs2Cmd, (IsWrite << 31) | ((Address >> 1) << 16) | Data);
}

STATIC
VOID
PmicWrapperImplRead (
  IN  UINT16  Address,
  OUT UINT16 *Value)
{
  UINT32 Result;

  // Send read transaction and wait until result become valid
  WacsCommand (Address, 0, FALSE);
  WacsWaitFor (WacsFsmWfVldClr);

  // Read data
  Result = PmicWrapperRead (PmicWrapperWacs2RData);
  PmicWrapperWrite (PmicWrapperWacs2VldClr, 1);

  *Value = (Result & 0xFFFF);
}

STATIC
VOID
PmicWrapperImplWrite (
  IN  UINT16 Address,
  OUT UINT16 Value)
{
  // Send write transaction
  WacsCommand (Address, Value, TRUE);
}

STATIC MTK_PMIC_WRAPPER_PROTOCOL mPmicWrapper = {
  PmicWrapperImplRead,
  PmicWrapperImplWrite
};

EFI_STATUS
EFIAPI
InitPmicWrapper (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;
  UINT32     InitState;

  // Locate PMIC Wrapper Memory Region
  Status = LocateMemoryRegionByName ("PMIC Wrapper", &mPmicWrapperRegion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate PMIC Wrapper Memory Region! Status = %r\n", Status));
    return Status;
  }

  // Read init state of PMIC Wrapper
  InitState = PmicWrapperRead (PmicWrapperInitDone2);
  if (InitState != 1) {
    DEBUG ((EFI_D_ERROR, "PMIC Wrapper Not Initialized!\n", Status));
    Status = EFI_NOT_READY;
    ASSERT_EFI_ERROR (Status);
  }

  // Register PMIC Wrapper Protocol
  Status = gBS->InstallMultipleProtocolInterfaces (&ImageHandle, &gMediaTekPmicWrapperProtocolGuid, &mPmicWrapper, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register PMIC Wrapper Protocol! Status = %r\n", Status));
    return Status;
  }

  return Status;
}