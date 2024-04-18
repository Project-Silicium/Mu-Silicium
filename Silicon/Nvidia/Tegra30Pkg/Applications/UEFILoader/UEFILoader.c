// Copyright (c) 2019 - 2020, Bingxing Wang and other project authors. All rights reserved.<BR>
// Copyright (c) 2021 - 2021, Leander Wollersberger. All rights reserved.<BR>

#include <Library/ArmLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/FileHandleLib.h>

#include <Guid/FileInfo.h>

#include <Protocol/LoadedImage.h>
#include <Protocol/SimpleTextOut.h>

#include "UEFILoader.h"

// Adopted from Kernel Code. Thanks Leander :)
typedef struct parking_protocol_mailbox {
  UINT32 CpuID;
  UINT32 Reserved;
  UINT64 EntryPoint;
} ParkingMailboxProtocol;

typedef struct cpu_mailbox_entry {
  ParkingMailboxProtocol *Mailbox;
  UINT32                  MailboxAddress;
  UINT8                   GicVersion;
  UINT8                   GicCpuID;
} CpuMailboxEntry;

VOID
StartSecondaryCores (UINTN Cpu)
{
  CpuMailboxEntry CpuMailboxEntries[4];

  // Set CPU 0 Values
  CpuMailboxEntries[0].GicCpuID       = 0;
  CpuMailboxEntries[0].GicVersion     = 1;
  CpuMailboxEntries[0].MailboxAddress = 0x82001000;
  CpuMailboxEntries[0].Mailbox        = (ParkingMailboxProtocol *)0x82001000U;

  // Set CPU 1 Values
  CpuMailboxEntries[1].GicCpuID       = 1;
  CpuMailboxEntries[1].GicVersion     = 1;
  CpuMailboxEntries[1].MailboxAddress = 0x82002000;
  CpuMailboxEntries[1].Mailbox        = (ParkingMailboxProtocol *)0x82002000U;

  // Set CPU 2 Values
  CpuMailboxEntries[2].GicCpuID       = 2;
  CpuMailboxEntries[2].GicVersion     = 1;
  CpuMailboxEntries[2].MailboxAddress = 0x82003000;
  CpuMailboxEntries[2].Mailbox        = (ParkingMailboxProtocol *)0x82003000U;

  // Set CPU 3 Values
  CpuMailboxEntries[3].GicCpuID       = 3;
  CpuMailboxEntries[3].GicVersion     = 1;
  CpuMailboxEntries[3].MailboxAddress = 0x82004000;
  CpuMailboxEntries[3].Mailbox        = (ParkingMailboxProtocol *)0x82004000U;

  // Let the Secondary Core use the Payload Loaded by UEFI
  MmioWrite32 ((UINT32)(&CpuMailboxEntries[1].Mailbox->EntryPoint), 0x93200000U);
  MmioWrite32 ((UINT32)(&CpuMailboxEntries[2].Mailbox->EntryPoint), 0x93300000U);
  MmioWrite32 ((UINT32)(&CpuMailboxEntries[3].Mailbox->EntryPoint), 0x93400000U);

  // Apply Changes
  ArmDataMemoryBarrier          ();
  ArmDataSynchronizationBarrier ();

  // Set CPU ID
  MmioWrite32 ((UINT32)(&CpuMailboxEntries[Cpu].Mailbox->CpuID), Cpu);

  // Apply Changes
  ArmDataMemoryBarrier          ();
  ArmDataSynchronizationBarrier ();

  // Interrupt Magic.
  // Interrupt According to ACPI PP 0x00FE0000
  // Register: 0xF00
  // Base:     0x50041000
  MmioWrite32 (0x50041F00U, 0x00FE0000U); // Send Interrupt to Trigger Secondary Core Unpark
  MmioWrite32 (0x50041F00U, 0x00FE0000U);
  MmioWrite32 (0x50041F00U, 0x00FE0000U);

  // Loop that waits for the Mailbox to be Cleared when Secondary Core Starts
  while (MmioRead32 ((UINT32)(&CpuMailboxEntries[Cpu].Mailbox->EntryPoint)) != 0);
}

EFI_STATUS
JumpToUEFI (EFI_HANDLE ImageHandle)
{
  EFI_STATUS             Status     = EFI_SUCCESS;
  EFI_MEMORY_DESCRIPTOR *MemMap     = 0;
  UINT32                 DesVersion = 0;
  UINTN                  MemMapSize = 0;
  UINTN                  MapKey     = 0;
  UINTN                  DesSize    = 0;

  // Get Memory Map
  gBS->GetMemoryMap (&MemMapSize, MemMap, &MapKey, &DesSize, &DesVersion);

  // Exit Boot Services
  Status = gBS->ExitBootServices (ImageHandle, MapKey);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[DEBUG] %a (%d): Failed to Exit Boot Services! Status = %r\n", __FUNCTION__, __LINE__, Status));
    goto exit;
  }

  // Deinit Original UEFI for Custom UEFI to takeover
  ArmDisableFiq                 ();
  ArmDisableInterrupts          ();
  ArmDisableCachesAndMmu        ();
  ArmInvalidateInstructionCache ();
  ArmInvalidateTlb              ();

  // Disable Gic
  MmioWrite32 ((UINT32)FixedPcdGet32(PcdGicRedistributorsBase), 0);

  // This should trigger an SMC, Jump to the Payload and Output Stuff Hopefully.
  ArmCallSmcHelper (0, 0, 0, 0);

  // Something is Wrong.
  // The Code should not reach here.
  CpuDeadLoop ();

exit:
  return Status;
}

EFI_STATUS
LoadPayloadIntoMemory (
  IN  EFI_PHYSICAL_ADDRESS  MemoryAddress,
  IN  CHAR16               *FileName,
  OUT UINTN                *FileSize,
  IN  EFI_HANDLE            ImageHandle)
{
  EFI_STATUS                       Status              = EFI_SUCCESS;
  EFI_GUID                         SFSPGuid            = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
  EFI_GUID                         LoadedImageProtocol = LOADED_IMAGE_PROTOCOL;
  EFI_LOADED_IMAGE                *LoadedImage         = NULL;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SFSProtocol         = NULL;
  EFI_FILE_PROTOCOL               *FileProtocol        = NULL;
  EFI_FILE_PROTOCOL               *File                = NULL;
  EFI_FILE_INFO                   *FileInfo            = NULL;
  UINTN                            FileInfoSize        = 0;

  // Locate Loaded Image Handle Protocol
  Status = gBS->HandleProtocol (ImageHandle, &LoadedImageProtocol, (VOID *)&LoadedImage);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[DEBUG] %a (%d): Failed to Locate Loaded Image Handle Protocol! Status = %r\n", __FUNCTION__, __LINE__, Status));
    goto exit;
  }

  DEBUG ((EFI_D_WARN, "[DEBUG] %a (%d): Successfully Located Loaded Image Handle Protocol.\n", __FUNCTION__, __LINE__));

  // Locate SFS Handle Protocol
  Status = gBS->HandleProtocol (LoadedImage->DeviceHandle, &SFSPGuid, (VOID *)&SFSProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[DEBUG] %a (%d): Failed to Locate SFS Handle Protocol! Status = %r\n", __FUNCTION__, __LINE__, Status));
    goto exit;
  }

  DEBUG ((EFI_D_WARN, "[DEBUG] %a (%d): Successfully Located SFS Handle Protocol.\n", __FUNCTION__, __LINE__));

  // Open Current Volume (The Volume where the Application is)
  Status = SFSProtocol->OpenVolume (SFSProtocol, &FileProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[DEBUG] %a (%d): Failed to Open Current Volume! Status = %r\n", __FUNCTION__, __LINE__, Status));
    goto exit;
  }

  DEBUG ((EFI_D_WARN, "[DEBUG] %a (%d): Successfully Opend Current Volume.\n", __FUNCTION__, __LINE__));

  // Open Payload File
  Status = FileProtocol->Open (FileProtocol, &File, FileName, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY | EFI_FILE_HIDDEN | EFI_FILE_SYSTEM);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[DEBUG] %a (%d): Failed to Open %s! Status = %r\n", __FUNCTION__, __LINE__, FileName, Status));
    goto exit;
  }

  DEBUG ((EFI_D_WARN, "[DEBUG] %a (%d): Successfully Opend %s.\n", __FUNCTION__, __LINE__, FileName));

  // Get the Infos about the Payload File
  Status = File->GetInfo (File, &gEfiFileInfoGuid, &FileInfoSize, (VOID *)FileInfo);
  if (Status == EFI_BUFFER_TOO_SMALL) {
    // Allocate Memory for the Payload
    Status = gBS->AllocatePool (EfiLoaderData, FileInfoSize, (VOID **)&FileInfo);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "[DEBUG] %a (%d) (%d): Failed to Allocate Memory! Status = %r\n", __FUNCTION__, __LINE__, Status));
      goto exit;
    }

    DEBUG ((EFI_D_WARN, "[DEBUG] %a (%d): Successfully Allocated Memory.\n", __FUNCTION__, __LINE__));

    SetMem ((VOID *)FileInfo, FileInfoSize, 0xFF);

    // Get the Infos about the Payload File (Again)
    Status = File->GetInfo (File, &gEfiFileInfoGuid, &FileInfoSize, (VOID *)FileInfo);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "[DEBUG] %a (%d): Failed to Get File Infos from %s! Status = %r\n", __FUNCTION__, __LINE__, FileName, Status));
      goto exit;
    }

    DEBUG ((EFI_D_WARN, "[DEBUG] %a (%d): Successfully Got File Infos from %s.\n", __FUNCTION__, __LINE__, FileName));
  } else if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[DEBUG] %a (%d): Failed to Get File Infos from %s! Status = %r\n", __FUNCTION__, __LINE__, FileName, Status));
    goto exit;
  } else {
    DEBUG ((EFI_D_WARN, "[DEBUG] %a (%d): Successfully Got File Infos from %s.\n", __FUNCTION__, __LINE__, FileName));
  }

  // Check if Payload isn't too large
  if (FileInfo->FileSize > 0xFFFFFFFF) {
    DEBUG ((EFI_D_ERROR, "[DEBUG] %a (%d): %s is too Large! Size: 0x%x", __FUNCTION__, __LINE__, FileName, FileInfo->FileSize));
    Status = EFI_OUT_OF_RESOURCES;
    goto exit;
  }

  Print (L"Size of %s: 0x%x\n", FileName, FileInfo->FileSize);

  // Allocate Memory for the Payload (Again)
  Status = gBS->AllocatePages (AllocateAddress, EfiLoaderData, EFI_SIZE_TO_PAGES(FileInfo->FileSize), &MemoryAddress);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[DEBUG] %a (%d): Failed to Allocate Memory! Status = %r\n", __FUNCTION__, __LINE__, Status));
    goto exit;
  }

  DEBUG ((EFI_D_WARN, "[DEBUG] %a (%d): Successfully Allocated Memory.\n", __FUNCTION__, __LINE__));

  SetMem ((EFI_PHYSICAL_ADDRESS *)MemoryAddress, FileInfo->FileSize, 0xFF);

  // Read Payload into Memory
  Status = File->Read (File, (UINTN *)&FileInfo->FileSize, (EFI_PHYSICAL_ADDRESS *)MemoryAddress);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[DEBUG] %a (%d): Failed to Write %s into Memory! Status = %r\n", __FUNCTION__, __LINE__, FileName, Status));
    goto exit;
  }

  DEBUG ((EFI_D_WARN, "[DEBUG] %a (%d): Successfully Written %s into Memory.\n", __FUNCTION__, __LINE__, FileName));

  *FileSize = FileInfo->FileSize;

  Print (L"%s in Memory at: 0x%x\n", FileName, MemoryAddress);

exit:
  return Status;
}

EFI_STATUS
EFIAPI
LoadUEFI (
  EFI_HANDLE        ImageHandle,
  EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;
  UINTN      FileSize;
  UINT8      SmcHandlerString[5];

  SmcHandlerString[0] = 0x02;
  SmcHandlerString[1] = 0x00;
  SmcHandlerString[2] = 0x50;
  SmcHandlerString[3] = 0xe3;
  SmcHandlerString[4] = 0xc8;

  // Fix Display Output
  Status = FixConsoleOutput ();
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[DEBUG] %a (%d): Failed to Fix Console Output! Status = %r\n", __FUNCTION__, __LINE__, Status));
    DEBUG ((EFI_D_WARN, "[DEBUG] %a (%d): Continue without Display.\n", __FUNCTION__, __LINE__));
  }

  // Print App Info
  Print (L"\n");
  Print (L"UEFI Loader Ver. %s\n", (CHAR16 *)PcdGetPtr(PcdFirmwareVersionString));
  Print (L"Built at %a on %a\n", __TIME__, __DATE__);
  Print (L"\n");

  // Disable Watch Dog Timer
  gBS->SetWatchdogTimer (0, 0, 0, NULL);

  // Unprotect TZ
  Print (L"Unprotect Trust Zone...\n");

  Status = UnprotectTZ ();
  if (EFI_ERROR (Status)) {
    Print (L"Failed to Unprotect Trust Zone! Status = %r\n", Status);
    goto exit;
  }

  Print (L"Trust Zone has been Successfully Unprotected.\n");
  Print (L"Writting Exploit Payload into Memory...\n");

  // Load Exploit Payload into Memory
  Status = LoadPayloadIntoMemory ((EFI_PHYSICAL_ADDRESS)0x93000000, L"ExploitSecureWorld.bin", &FileSize, ImageHandle);
  if (EFI_ERROR (Status)) {
    Print (L"Failed to Write Exploit Payload into Memory! Status = %r\n", Status);
    goto exit;
  }

  Print (L"Exploit Payload has been Successfully been Written into Memory.\n");
  Print (L"Writting Secondary Cores Payload into Memory...\n");

  // Load Secondary Cores Payload into Memory
  Status = LoadPayloadIntoMemory ((EFI_PHYSICAL_ADDRESS)0x93110000, L"SecondaryCores.bin", 0, ImageHandle);
  if (EFI_ERROR (Status)) {
    Print (L"Failed to Write Secondary Cores Payload into Memory! Status = %r\n", Status);
    goto exit;
  } else {
    DEBUG ((EFI_D_WARN, "[DEBUG] %a (%d): Successfully Written SecondaryCores.bin at 0x93110000.\n", __FUNCTION__, __LINE__));

    Status = LoadPayloadIntoMemory ((EFI_PHYSICAL_ADDRESS)0x93120000, L"SecondaryCores.bin", 0, ImageHandle);
    if (EFI_ERROR (Status)) {
      Print (L"Failed to Write Secondary Cores Payload into Memory! Status = %r\n", Status);
      goto exit;
    } else {
      DEBUG ((EFI_D_WARN, "[DEBUG] %a (%d): Successfully Written SecondaryCores.bin at 0x93120000.\n", __FUNCTION__, __LINE__));

      Status = LoadPayloadIntoMemory ((EFI_PHYSICAL_ADDRESS)0x93130000, L"SecondaryCores.bin", 0, ImageHandle);
      if (EFI_ERROR (Status)) {
        Print (L"Failed to Write Secondary Cores Payload into Memory! Status = %r\n", Status);
        goto exit;
      }

      DEBUG ((EFI_D_WARN, "[DEBUG] %a (%d): Successfully Written SecondaryCores.bin at 0x93130000.\n", __FUNCTION__, __LINE__));
    }
  }

  Print (L"Secondary Cores Payload has been Successfully been Written into Memory.\n");
  Print (L"Writting SMC Payload into Memory...\n");

  // Load SMC Payload into Memory
  Status = LoadPayloadIntoMemory ((EFI_PHYSICAL_ADDRESS)0x93200000, L"Smc.bin", 0, ImageHandle);
  if (EFI_ERROR (Status)) {
    Print (L"Failed to Write SMC Payload into Memory! Status = %r\n", Status);
    goto exit;
  } else {
    DEBUG ((EFI_D_WARN, "[DEBUG] %a (%d): Successfully Written Smc.bin at 0x93200000.\n", __FUNCTION__, __LINE__));
  
    Status = LoadPayloadIntoMemory ((EFI_PHYSICAL_ADDRESS)0x93300000, L"Smc.bin", 0, ImageHandle);
    if (EFI_ERROR (Status)) {
      Print (L"Failed to Write SMC Payload into Memory! Status = %r\n", Status);
      goto exit;
    } else {
      DEBUG ((EFI_D_WARN, "[DEBUG] %a (%d): Successfully Written Smc.bin at 0x93300000.\n", __FUNCTION__, __LINE__));

      Status = LoadPayloadIntoMemory ((EFI_PHYSICAL_ADDRESS)0x93400000, L"Smc.bin", 0, ImageHandle);
      if (EFI_ERROR (Status)) {
        Print (L"Failed to Write SMC Payload into Memory! Status = %r\n", Status);
        goto exit;
      }

      DEBUG ((EFI_D_WARN, "[DEBUG] %a (%d): Successfully Written Smc.bin at 0x93400000.\n", __FUNCTION__, __LINE__));
    }
  }

  Print (L"SMC Payload has been Successfully been Written into Memory.\n");
  Print (L"Writting UEFI Payload into Memory...\n");

  // Load UEFI Payload into Memory
  Status = LoadPayloadIntoMemory ((EFI_PHYSICAL_ADDRESS)0x84000000, L"UEFI.bin", 0, ImageHandle);
  if (EFI_ERROR (Status)) {
    Print (L"Failed to Write UEFI Payload into Memory! Status = %r\n", Status);
    goto exit;
  }

  Print (L"UEFI Payload has been Successfully been Written into Memory.\n");
  Print (L"Trying to Write Trust Zone...\n");

  // Disable MMU to get Access to TrustZone Memory by Disarming the Translation Table / Page Table
  // NO UEFI FROM HERE: Print (), ..., and so on
  ArmDisableFiq          ();
  ArmDisableInterrupts   ();
  ArmDisableCachesAndMmu ();

  // Copy Payload into TrustZone Memory.
  // The Memory needs to be Marked as Secure, as you can only Execute Secure Memory in Secure Mode so
  // we copy the Payload to TZ Memory
  VOID *mTZSmcHandlerString = memmem((VOID *)0x80000000, 0x200000, SmcHandlerString, sizeof(SmcHandlerString));
  memcpy_usr ((VOID *)(mTZSmcHandlerString), (CONST VOID *)0x93000000, (UINTN)FileSize);

  MmioWrite32 (0x82002880U, 0x93110000U);
  MmioWrite32 (0x82003880U, 0x93120000U);
  MmioWrite32 (0x82004880U, 0x93130000U);

  // Apply Changes
  ArmDataSynchronizationBarrier ();

  // Payload is now in Place. Enable MMU to use UEFI one last Time
  ArmEnableMmu              ();
  ArmEnableDataCache        ();
  ArmEnableInstructionCache ();
  ArmEnableFiq              ();
  ArmEnableInterrupts       ();

  // Print UEFI SMC Address
  DEBUG ((EFI_D_WARN, "[DEBUG] %a (%d): UEFI SMC Address: 0x%08x\n", __FUNCTION__, __LINE__, mTZSmcHandlerString));

  Print (L"Successfully Wrote Trust Zone.\n");

  // Start Secondary Cores
  Print (L"Starting Secondary Cores...\n\n");
  Print (L"If this takes too Long then Create a Hardware Interrupt.\n");
  Print (L"For Example Removing/Pluging In a USB Drive.\n\n");

  for (UINTN i = 1; i < 4; i++) {
    StartSecondaryCores (i);
    Print (L"Secondary Core %d Started.\n", i);
  }

  // Jump to UEFI Payload
  Print (L"Jumping to UEFI Payload...\n");

  Status = JumpToUEFI (ImageHandle);
  if (EFI_ERROR (Status)) {
    Print (L"Failed to Jump to UEFI! Status = %r\n", Status);
    goto exit;
  }

exit:
  Print (L"Reset your Device Manually!\n");

  // Do CPU Death Loop
  CpuDeadLoop();

  return Status;
}