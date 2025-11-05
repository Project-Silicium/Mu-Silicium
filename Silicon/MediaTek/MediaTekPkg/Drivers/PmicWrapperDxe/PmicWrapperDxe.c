#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/IoLib.h>

#include <Protocol/MtkPmicWrapper.h>
#include <Library/PmicWrapperImplLib.h>

#define WACS_FSM_IDLE     0x0
#define WACS_FSM_REQ      0x2
#define WACS_FSM_WFVLDCLR 0x6

STATIC EFI_MEMORY_REGION_DESCRIPTOR_EX mPmicWrapperRegion;

UINT32
PmicWrapperRead(
  IN  MTK_PMIC_WRAPPER_REG Reg)
{
  return MmioRead32 (mPmicWrapperRegion.Address + PlatformInfo.RegMap[Reg]);
}

VOID
PmicWrapperWrite(
  IN  MTK_PMIC_WRAPPER_REG Reg,
  IN  UINT32               Value)
{
  MmioWrite32 (mPmicWrapperRegion.Address + PlatformInfo.RegMap[Reg], Value);
}

UINT32
WacsGetFsm ()
{
  return (PmicWrapperRead (PMIC_WRAPPER_WACS2_RDATA) >> 16) & 0x7;
}

VOID
WacsWaitFor (
  IN UINT32 Fsm)
{
  while (WacsGetFsm () != Fsm)
  {
    MicroSecondDelay (100);
  }
}

VOID
WacsCommand (
  IN UINT32  Address,
  IN UINT32  Data,
  IN BOOLEAN IsWrite)
{
  WacsWaitFor (WACS_FSM_IDLE);
  PmicWrapperWrite (PMIC_WRAPPER_WACS2_CMD, (IsWrite << 31) | ((Address >> 1) << 16) | Data);
}

VOID
PmicWrapperImplRead (
  IN  UINT16  Address,
  OUT UINT16 *Value)
{
  UINT32 Result;

  WacsCommand (Address, 0, FALSE);
  WacsWaitFor (WACS_FSM_WFVLDCLR);

  Result = PmicWrapperRead(PMIC_WRAPPER_WACS2_RDATA);
  PmicWrapperWrite (PMIC_WRAPPER_WACS2_VLDCLR, 1);

  *Value = (Result & 0xFFFF);
}

VOID
PmicWrapperImplWrite (
  IN  UINT16 Address,
  OUT UINT16 Value)
{
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
  Status = LocateMemoryMapAreaByName ("PMIC Wrapper", &mPmicWrapperRegion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate PMIC Wrapper Memory Region! Status = %r\n", Status));
    return Status;
  }

  // Read init state of PMIC Wrapper
  InitState = PmicWrapperRead (PMIC_WRAPPER_INIT_DONE2);
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