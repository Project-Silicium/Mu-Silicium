/** @file

  Patches NTOSKRNL to not cause a SError when reading/writing ACTLR_EL1
  Patches NTOSKRNL to not cause a SError when reading/writing AMCNTENSET0_EL0
  Patches NTOSKRNL to not cause a bugcheck when attempting to use
  PSCI_MEMPROTECT Due to an issue in QHEE

  Based on https://github.com/SamuelTulach/rainbow

  Copyright (c) 2021 Samuel Tulach
  Copyright (c) 2022-2023 DuoWoA authors

  SPDX-License-Identifier: MIT

**/
#include "KernelErrataPatcherLib.h"

STATIC EFI_EXIT_BOOT_SERVICES mOriginalEfiExitBootServices = NULL;
EFI_EVENT                     mReadyToBootEvent;

// Please see ./ShellCode/Reference/ShellCode.c for what this does
UINT8 OslArm64TransferToKernelShellCode[] = {
    0xEE, 0x03, 0x00, 0xAA, 0xC8, 0x0D, 0x41, 0xF8, 0x1F, 0x01, 0x0E, 0xEB,
    0xA0, 0x08, 0x00, 0x54, 0x07, 0x05, 0x82, 0x52, 0x07, 0xA7, 0xBA, 0x72,
    0x06, 0x55, 0x9A, 0x52, 0x66, 0xA7, 0xBA, 0x72, 0x0A, 0xFB, 0x9D, 0x52,
    0xEA, 0x5C, 0xA5, 0x72, 0xAB, 0x5A, 0x80, 0xD2, 0x0B, 0x00, 0xA3, 0xF2,
    0x6B, 0x00, 0xC0, 0xF2, 0x0B, 0x50, 0xFA, 0xF2, 0x6D, 0x00, 0x80, 0xD2,
    0x0D, 0x50, 0xBA, 0xF2, 0x4D, 0x00, 0xC0, 0xF2, 0x0D, 0x50, 0xFA, 0xF2,
    0x30, 0x00, 0x80, 0xD2, 0x10, 0x50, 0xBA, 0xF2, 0x10, 0x48, 0xC0, 0xF2,
    0x10, 0x00, 0xE3, 0xF2, 0x11, 0x78, 0x80, 0x52, 0xF1, 0xCB, 0xBA, 0x72,
    0x4F, 0x00, 0x80, 0xD2, 0x0F, 0x50, 0xBA, 0xF2, 0x2F, 0x00, 0xC0, 0xF2,
    0x0F, 0x50, 0xFA, 0xF2, 0xEC, 0x03, 0x84, 0x52, 0x6C, 0xA0, 0xBA, 0x72,
    0x09, 0x01, 0x80, 0x52, 0x09, 0x50, 0xBA, 0x72, 0x21, 0x00, 0x00, 0x14,
    0x63, 0x00, 0x0A, 0x0B, 0x7F, 0x04, 0x00, 0x71, 0x89, 0x01, 0x00, 0x54,
    0x43, 0x00, 0x40, 0xF9, 0x7F, 0x00, 0x0B, 0xEB, 0x60, 0x02, 0x00, 0x54,
    0x7F, 0x00, 0x0D, 0xEB, 0x01, 0x01, 0x00, 0x54, 0x43, 0x04, 0x40, 0xF9,
    0x63, 0xF8, 0x58, 0x92, 0x7F, 0x00, 0x10, 0xEB, 0x81, 0x00, 0x00, 0x54,
    0x51, 0x40, 0x1E, 0xB8, 0x02, 0x00, 0x00, 0x14, 0x4C, 0x00, 0x00, 0xB9,
    0x42, 0x10, 0x00, 0x91, 0x5F, 0x00, 0x05, 0xEB, 0x82, 0x01, 0x00, 0x54,
    0x43, 0x00, 0x40, 0xB9, 0x7F, 0x00, 0x07, 0x6B, 0x64, 0x10, 0x46, 0x7A,
    0x61, 0xFD, 0xFF, 0x54, 0x49, 0x00, 0x00, 0xB9, 0xF8, 0xFF, 0xFF, 0x17,
    0x43, 0x04, 0x40, 0xF9, 0x7F, 0x00, 0x0F, 0xEB, 0xA1, 0xFE, 0xFF, 0x54,
    0x51, 0x00, 0x1E, 0xB8, 0xF3, 0xFF, 0xFF, 0x17, 0x08, 0x01, 0x40, 0xF9,
    0x1F, 0x01, 0x0E, 0xEB, 0x08, 0x00, 0x00, 0x14, 0x02, 0x19, 0x40, 0xF9,
    0x05, 0x41, 0x40, 0xB9, 0xA5, 0x00, 0x02, 0x8B, 0x5F, 0x00, 0x05, 0xEB,
    0xC3, 0xFD, 0xFF, 0x54, 0xF8, 0xFF, 0xFF, 0x17, 0x00, 0x00, 0x00, 0x14};

VOID KernelErrataPatcherApplyReadACTLREL1Patches(
    EFI_PHYSICAL_ADDRESS Base, UINTN Size)
{
  // Fix up #0 (28 10 38 D5 -> 08 00 80 D2) (ACTRL_EL1 Register Read)
  UINT8                FixedInstruction0[] = {0x08, 0x00, 0x80, 0xD2};
  EFI_PHYSICAL_ADDRESS IllegalInstruction0 =
      FindPattern(Base, Size, "28 10 38 D5");

  if (IllegalInstruction0 != 0) {
    FirmwarePrint("mrs x8, actlr_el1         -> 0x%p\n", IllegalInstruction0);

    CopyMemory(
        IllegalInstruction0, (EFI_PHYSICAL_ADDRESS)FixedInstruction0,
        sizeof(FixedInstruction0));
  }
}

EFI_STATUS
EFIAPI
KernelErrataPatcherExitBootServices(
    IN EFI_HANDLE ImageHandle, IN UINTN MapKey,
    IN EFI_PHYSICAL_ADDRESS fwpKernelSetupPhase1)
{
  // Might be called multiple times by winload in a loop failing few times
  gBS->ExitBootServices = mOriginalEfiExitBootServices;
  gBS->Hdr.CRC32        = 0;
  gBS->CalculateCrc32(gBS, sizeof(EFI_BOOT_SERVICES), &gBS->Hdr.CRC32);

  FirmwarePrint(
      "OslFwpKernelSetupPhase1   -> (phys) 0x%p\n", fwpKernelSetupPhase1);

  UINTN                tempSize = SCAN_MAX;
  EFI_PHYSICAL_ADDRESS winloadBase =
      LocateWinloadBase(fwpKernelSetupPhase1, &tempSize);

  EFI_STATUS Status = UnprotectWinload(winloadBase + EFI_PAGE_SIZE, tempSize);
  if (EFI_ERROR(Status)) {
    FirmwarePrint(
        "Could not unprotect winload -> (phys) 0x%p (size) 0x%p %r\n",
        winloadBase, tempSize, Status);

    goto exit;
  }

  // Fix up winload.efi
  // This fixes Boot Debugger
  FirmwarePrint(
      "Patching OsLoader         -> (phys) 0x%p (size) 0x%p\n",
      fwpKernelSetupPhase1, SCAN_MAX);

  KernelErrataPatcherApplyReadACTLREL1Patches(fwpKernelSetupPhase1, SCAN_MAX);

  EFI_PHYSICAL_ADDRESS OslArm64TransferToKernelAddr =
      winloadBase + 0xC00 +
      NT_OS_LOADER_ARM64_TRANSFER_TO_KERNEL_FUNCTION_OFFSET;
  EFI_PHYSICAL_ADDRESS NewOslArm64TransferToKernelAddr =
      OslArm64TransferToKernelAddr - sizeof(OslArm64TransferToKernelShellCode);

  for (EFI_PHYSICAL_ADDRESS current = OslArm64TransferToKernelAddr;
       current < OslArm64TransferToKernelAddr + SCAN_MAX;
       current += sizeof(UINT32)) {

    if (ARM64_BRANCH_LOCATION_INSTRUCTION(
            current, OslArm64TransferToKernelAddr) == *(UINT32 *)current) {
      FirmwarePrint(
          "Patching bl OsLoaderArm64TransferToKernel -> (phys) 0x%p\n",
          current);

      *(UINT32 *)current = ARM64_BRANCH_LOCATION_INSTRUCTION(
          current, NewOslArm64TransferToKernelAddr);

      FirmwarePrint(
          "Patching OsLoaderArm64TransferToKernel -> (phys) 0x%p\n",
          OslArm64TransferToKernelAddr);

      // Copy shell code right before the OsLoaderArm64TransferToKernelFunction
      CopyMemory(
          NewOslArm64TransferToKernelAddr,
          (EFI_PHYSICAL_ADDRESS)OslArm64TransferToKernelShellCode,
          sizeof(OslArm64TransferToKernelShellCode));

      break;
    }
  }

  FirmwarePrint(
      "Protecting winload        -> (phys) 0x%p (size) 0x%p\n", winloadBase,
      tempSize);

  Status = ReProtectWinload(winloadBase + EFI_PAGE_SIZE, tempSize);
  if (EFI_ERROR(Status)) {
    FirmwarePrint(
        "Could not reprotect winload -> (phys) 0x%p (size) 0x%p %r\n",
        winloadBase, tempSize, Status);

    goto exit;
  }

exit:
  FirmwarePrint(
      "OslFwpKernelSetupPhase1   <- (phys) 0x%p\n", fwpKernelSetupPhase1);

  // Call the original
  return gBS->ExitBootServices(ImageHandle, MapKey);
}

VOID EFIAPI ReadyToBootHandler(IN EFI_EVENT Event, IN VOID *Context)
{
  PERF_CALLBACK_BEGIN(&gEfiEventReadyToBootGuid);

  mOriginalEfiExitBootServices = gBS->ExitBootServices;
  gBS->ExitBootServices        = ExitBootServicesWrapper;
  gBS->Hdr.CRC32               = 0;
  gBS->CalculateCrc32(gBS, sizeof(EFI_BOOT_SERVICES), &gBS->Hdr.CRC32);

  gBS->CloseEvent(mReadyToBootEvent);

  PERF_CALLBACK_END(&gEfiEventReadyToBootGuid);
}

EFI_STATUS
EFIAPI
KernelErrataPatcherLibConstructor(
    IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  Status = gBS->CreateEventEx(
      EVT_NOTIFY_SIGNAL, TPL_CALLBACK, ReadyToBootHandler, NULL,
      &gEfiEventReadyToBootGuid, &mReadyToBootEvent);
  if (EFI_ERROR(Status)) {
    DEBUG(
        (DEBUG_ERROR,
         "KernelErrataPatcherLibConstructor: Failed to create event %r\n",
         Status));
    ASSERT(FALSE);
  }

  return InitMemoryAttributeProtocol();
}
