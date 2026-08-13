#include <Library/MemoryMapHelperLib.h>
#include <Library/AssemblyUtilsLib.h>
#include <Library/BaseMemoryLib.h>

VOID
ParseAdrpInstruction (
  IN  UINT32                Instruction,
  IN  UINT64                InstructionLocation,
  OUT EFI_PHYSICAL_ADDRESS *BaseAddress,
  OUT UINT32               *Register)
{
  // Extract IMMHI & IMMLO
  UINT32 ImmHi = (Instruction >> 5)  & 0x7FFFF;
  UINT32 ImmLo = (Instruction >> 29) & 0x3;

  // Create IMM21
  INT32 Imm21 = (ImmHi << 2) | ImmLo;

  // Populate Remaining Bits
  if (Imm21 & 0x100000) { 
    Imm21 |= 0xFFE00000; 
  }

  // Pass Values
  *BaseAddress = (InstructionLocation & ~0xFFF) + ((INT64)Imm21 << 12);
  *Register    = Instruction & 0x1F;
}

VOID
ParseAddInstruction (
  IN  UINT32  Instruction,
  OUT UINT32 *AddressOffset,
  OUT UINT32 *SourceRegister,
  OUT UINT32 *TargetRegister)
{
  // Pass Values
  *AddressOffset  = (Instruction >> 10) & 0xFFF;
  *SourceRegister = (Instruction >> 5)  & 0x1F;
  *TargetRegister = Instruction         & 0x1F;
}

EFI_PHYSICAL_ADDRESS
ParseHobFunction (
  IN EFI_PHYSICAL_ADDRESS  FunctionStart,
  IN EFI_GUID             *ProtocolGuid)
{
  EFI_PHYSICAL_ADDRESS ProtocolAddress  = 0;
  UINT64               AdrpRegValue[32] = {0};
  BOOLEAN              GuidMatched      = FALSE;

  // Go thru the HOB Function
  for (EFI_PHYSICAL_ADDRESS Current = FunctionStart;; Current += ARM64_INSTRUCTION_LENGTH) {
    // Get Current Instruction
    UINT32 Instruction = ARM64_INSTRUCTION (Current);

    // Check for Return Instruction
    if (Instruction == 0xD65F03C0) {
      break;
    }

    // Check for ADRP Instruction
    if ((Instruction & 0x9F000000) == 0x90000000) {
      EFI_PHYSICAL_ADDRESS BaseAddress;
      UINT32               Register;

      // Parse ADRP Instruction
      ParseAdrpInstruction (Instruction, Current, &BaseAddress, &Register);

      // Save ADRP Value
      AdrpRegValue[Register] = BaseAddress;
    }

    // Check for ADD Instruction
    if ((Instruction & 0xFFC00000) == 0x91000000) {
      UINT32 AddressOffset;
      UINT32 SourceRegister;
      UINT32 TargetRegister;

      // Parse ADD Instruction
      ParseAddInstruction (Instruction, &AddressOffset, &SourceRegister, &TargetRegister);

      // Verify ADRP Instruction Value
      if (AdrpRegValue[TargetRegister] == 0) {
        continue;
      }

      // Append Address Offset
      AdrpRegValue[TargetRegister] += AddressOffset;

      // Compare Protocol GUIDs
      if (CompareGuid ((EFI_GUID *)AdrpRegValue[TargetRegister], ProtocolGuid) != 0) {
        GuidMatched = TRUE;
        continue;
      }

      // Set Protocol Address
      ProtocolAddress = AdrpRegValue[TargetRegister];
    }
  }

  // Return Protocol Address
  return GuidMatched == TRUE ? ProtocolAddress : 0;
}

VOID
GetXblHobAddresses (
  OUT EFI_PHYSICAL_ADDRESS *SchedulerInterfaceAddr,
  OUT EFI_PHYSICAL_ADDRESS *DtbExtensionAddr)
{
  EFI_MEMORY_REGION_DESCRIPTOR ReservedFd = {0};

  // Locate Reserved FD Region
  LocateMemoryRegionByName ("FD Reserved", &ReservedFd);
  LocateMemoryRegionByName ("FD_Reserved", &ReservedFd);

  // Verify Reserved FD Region
  if (ReservedFd.Address == 0 && ReservedFd.Length == 0) {
    return;
  }

  // Go thru the Reserved FD Region
  for (EFI_PHYSICAL_ADDRESS Current = ReservedFd.Address; Current < ReservedFd.Address + ReservedFd.Length; Current += ARM64_INSTRUCTION_LENGTH) {
    // Check for Basic HOB Registration Function
    if (*(UINT64 *)Current != 0xA9017BFDD10083FF) {
      continue;
    }

    // Verify 8th Function Instruction
    if ((ARM64_INSTRUCTION (Current + ARM64_TOTAL_INSTRUCTION_LENGTH (8)) & 0xFFFFFFE0) != 0x52800100) {
      continue;
    }

    // Get Scheduler Protocol Address
    if (*SchedulerInterfaceAddr == 0) {
      *SchedulerInterfaceAddr = ParseHobFunction (Current, &gEfiSchedulerInterfaceHobGuid);
    }

    // Get DTB Extension Protocol Address
    if (*DtbExtensionAddr == 0) {
      *DtbExtensionAddr = ParseHobFunction (Current, &gEfiDtbExtensionHobGuid);
    }
  }
}
