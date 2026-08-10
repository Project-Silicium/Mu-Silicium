#include <Library/MemoryMapHelperLib.h>
#include <Library/AssemblyUtilsLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>

#include "Include/XblHob.h"

UINT64
GetAdrpImm (IN EFI_PHYSICAL_ADDRESS Address)
{
  // Get Instruction
  UINT32 Instruction = ARM64_INSTRUCTION (Address);

  // Verify Instruction
  if ((Instruction & 0x9F000000) != 0x90000000) {
    return 0;
  }

  // Extract Immlo & Immhi Fields
  UINT32 Immlo = (Instruction >> 29) & 0x3;
  UINT32 Immhi = (Instruction >> 5)  & 0x7FFFF;

  // Create Base Imm Field
  INT64 Imm = (Immhi << 2) | Immlo;

  // Populate Remaining Bits
  if (Imm & (1ULL << 20)) {
    Imm |= ~((1ULL << 21) - 1);
  }

  // Return Imm Value
  return (Address & ~0xFFFULL) + (Imm << 12);
}

UINT64
GetAddImm (IN EFI_PHYSICAL_ADDRESS Address)
{
  // Get Instruction
  UINT32 Instruction = ARM64_INSTRUCTION (Address);

  // Verify Instruction
  if ((Instruction & 0xFF800000) != 0x91000000) {
    return 0;
  }

  // Extract Imm12 & Shift Field
  UINT64 Imm12 = (Instruction >> 10) & 0xFFF;
  UINT32 Shift = (Instruction >> 22) & 0x1;

  // Return Imm Value
  return Shift ? (Imm12 << 12) : Imm12;
}

VOID
GetXblHobAddresses (
  OUT EFI_PHYSICAL_ADDRESS *SchedulerInterfaceAddr,
  OUT EFI_PHYSICAL_ADDRESS *DtbExtensionAddr)
{
  EFI_MEMORY_REGION_DESCRIPTOR ReservedFd = {0};

  // Get XBL HOB PCDs
  EFI_PHYSICAL_ADDRESS SchedulerAddrPcd    = FixedPcdGet64 (PcdSchedulerInterfaceAddr);
  EFI_PHYSICAL_ADDRESS DtbExtensionAddrPcd = FixedPcdGet64 (PcdDtbExtensionAddr);

  // Use XBL HOB PCDs instead
  if (SchedulerAddrPcd != 0 || DtbExtensionAddrPcd != 0) {
    *SchedulerInterfaceAddr = SchedulerAddrPcd;
    *DtbExtensionAddr       = DtbExtensionAddrPcd;
    return;
  }

  // Locate Reserved FD Region
  LocateMemoryRegionByName ("FD Reserved", &ReservedFd);
  LocateMemoryRegionByName ("FD_Reserved", &ReservedFd);

  // Verify Reserved FD Region
  if (ReservedFd.Address == 0 && ReservedFd.Length == 0) {
    return;
  }

  // Set XBL HOB Details
  EFI_XBL_HOB_DETAILS HobDetails[] = {
    {
      .Address           = SchedulerInterfaceAddr,
      .ExpectedGuid      = &gEfiSchedulerInterfaceHobGuid,
      .AdrpGuidOffset    = 4,
      .AdrpAddrOffset    = 3,
      .AddGuidOffset     = 6,
      .AddAddrOffset     = 5
    },
    {
      .Address           = SchedulerInterfaceAddr,
      .ExpectedGuid      = &gEfiSchedulerInterfaceHobGuid,
      .AdrpGuidOffset    = 5,
      .AdrpAddrOffset    = 3,
      .AddGuidOffset     = 6,
      .AddAddrOffset     = 4
    },
    {
      .Address           = DtbExtensionAddr,
      .ExpectedGuid      = &gEfiDtbExtnHobGuid,
      .AdrpGuidOffset    = 6,
      .AdrpAddrOffset    = 3,
      .AddGuidOffset     = 7,
      .AddAddrOffset     = 4
    }
  };

  // Go thru Reserved FD Region
  for (EFI_PHYSICAL_ADDRESS Current = ReservedFd.Address; Current < ReservedFd.Address + ReservedFd.Length; Current += ARM64_INSTRUCTION_LENGTH) {
    // Check for Basic HOB Registration Function
    if (*(UINT64 *)Current != 0xA9017BFDD10083FF) {
      continue;
    }

    // Verify 8th Function Instruction
    if ((ARM64_INSTRUCTION (Current + ARM64_TOTAL_INSTRUCTION_LENGTH (8)) & 0xFFFFFFE0) != 0x52800100) {
      continue;
    }

    // Go thru each Target HOB
    for (UINT8 i = 0; i < ARRAY_SIZE (HobDetails); i++) {
      // Set Default Values
      UINT64 BasePage = 0;
      UINT64 Offset   = 0;

      // Get HOB GUID Location
      BasePage = GetAdrpImm (Current + ARM64_TOTAL_INSTRUCTION_LENGTH (HobDetails[i].AdrpGuidOffset));
      Offset   = GetAddImm  (Current + ARM64_TOTAL_INSTRUCTION_LENGTH (HobDetails[i].AddGuidOffset));

      // Verify Base Page & Offset
      if (BasePage == 0 || Offset == 0) {
        continue;
      }

      // Set HOB GUID
      EFI_GUID *HobGuid = (EFI_GUID *)(BasePage + Offset);

      // Compare HOB GUIDs
      if (!CompareGuid (HobGuid, HobDetails[i].ExpectedGuid)) {
        continue;
      }

      // Get HOB Address
      BasePage = GetAdrpImm (Current + ARM64_TOTAL_INSTRUCTION_LENGTH (HobDetails[i].AdrpAddrOffset));
      Offset   = GetAddImm  (Current + ARM64_TOTAL_INSTRUCTION_LENGTH (HobDetails[i].AddAddrOffset));

      // Set HOB Address
      if (BasePage != 0 && Offset != 0) {
        *HobDetails[i].Address = (EFI_PHYSICAL_ADDRESS)(BasePage + Offset);
      }
    }
  }
}
