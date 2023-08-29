/**@file This module implements the Microsoft Boot Policy.

Copyright (C) Microsoft Corporation. All rights reserved.
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "MsBootPolicy.h"

#define USB_DRIVE_SECOND_CHANCE_DELAY_S  6

static BOOT_SEQUENCE  mDefaultBootSequence[] = {
  MsBootHDD,
  MsBootUSB,
  MsBootPXE4,
  MsBootPXE6,
  MsBootDone
};
static BOOT_SEQUENCE  mUsbBootSequence[] = {
  MsBootUSB,
  MsBootDone
};
static BOOT_SEQUENCE  mPXEBootSequence[] = {
  MsBootPXE4,
  MsBootPXE6,
  MsBootDone
};
static BOOT_SEQUENCE  mSddBootSequence[] = {
  MsBootHDD,
  MsBootDone
};

// Device Path filter routines
typedef
BOOLEAN
(EFIAPI *FILTER_ROUTINE)(
  IN  EFI_DEVICE_PATH_PROTOCOL *DevicePath
  );

BOOLEAN EFIAPI
CheckDeviceNode (
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath,
  UINT8                     Type,
  UINT8                     SubType
  )
{
  while (!IsDevicePathEndType (DevicePath)) {
    if ((DevicePathType (DevicePath) == Type) &&
        (DevicePathSubType (DevicePath) == SubType))
    {
      return TRUE;
    }

    DevicePath = NextDevicePathNode (DevicePath);
  }

  return FALSE;
}

BOOLEAN EFIAPI
IsDevicePathUSB (
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  )
{
  return (CheckDeviceNode (DevicePath, MESSAGING_DEVICE_PATH, MSG_USB_DP) ||
          MsBootPolicyLibIsDevicePathUsb (DevicePath));
}

BOOLEAN EFIAPI
IsDevicePathIPv4 (
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  )
{
  return CheckDeviceNode (DevicePath, MESSAGING_DEVICE_PATH, MSG_IPv4_DP);
}

BOOLEAN EFIAPI
IsDevicePathIPv6 (
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  )
{
  return CheckDeviceNode (DevicePath, MESSAGING_DEVICE_PATH, MSG_IPv6_DP);
}

BOOLEAN EFIAPI
FilterOnlyUSB (
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  )
{
  return (TRUE == IsDevicePathUSB (DevicePath));
}

BOOLEAN EFIAPI
FilterNoUSB (
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  )
{
  return (FALSE == IsDevicePathUSB (DevicePath));
}

BOOLEAN EFIAPI
FilterOnlyIPv4 (
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  )
{
  return (TRUE == IsDevicePathIPv4 (DevicePath));
}

BOOLEAN EFIAPI
FilterOnlyIPv6 (
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  )
{
  return (TRUE == IsDevicePathIPv6 (DevicePath));
}

VOID
FilterHandles (
  EFI_HANDLE      *HandleBuffer,
  UINTN           *HandleCount,
  FILTER_ROUTINE  KeepHandleFilter
  )
{
  UINTN                     Index;
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;
  EFI_STATUS                Status;

  for (Index = 0; Index < *HandleCount;) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiDevicePathProtocolGuid,
                    (VOID **)&DevicePath
                    );

    if (EFI_ERROR (Status) ||               // Remove handles that don't have device path
        (!KeepHandleFilter (DevicePath)))   // TRUE keeps handle, FALSE deletes handle
    {
      (*HandleCount)--;
      CopyMem (&HandleBuffer[Index], &HandleBuffer[Index + 1], (*HandleCount-Index) * sizeof (EFI_HANDLE *));
      continue;
    }

    Index++;
  }

  return;
}

BOOLEAN
CompareDevicePathAgtB (
  EFI_DEVICE_PATH_PROTOCOL  *DevicePathA,
  EFI_DEVICE_PATH_PROTOCOL  *DevicePathB
  )
{
  UINTN            LengthA;
  UINTN            LengthB;
  UINTN            CompareLength;
  INTN             Result = 0;
  USB_DEVICE_PATH  *UsbPathA;
  USB_DEVICE_PATH  *UsbPathB;
  PCI_DEVICE_PATH  *PciPathA;
  PCI_DEVICE_PATH  *PciPathB;

  LengthA = GetDevicePathSize (DevicePathA);
  LengthB = GetDevicePathSize (DevicePathB);

  CompareLength = LengthA;    // CompareLength=MIN(LengthA,LengthB);
  if (LengthB < LengthA) {
    CompareLength = LengthB;
  }

  while (!IsDevicePathEnd (DevicePathA) &&
         !IsDevicePathEnd (DevicePathB))
  {
    Result = CompareMem (DevicePathA, DevicePathB, DevicePathNodeLength (DevicePathA));
    if (Result != 0) {
      // Note - Device paths are not sortable in binary.  Node fields are sortable,
      //        but may not in the proper order in memory. Only some node types are
      //        of interest at this time.  All others can use a binary compare for now.
      if ((DevicePathType (DevicePathA) == DevicePathType (DevicePathB)) &&
          (DevicePathSubType (DevicePathA) == DevicePathSubType (DevicePathB)))
      {
        switch (DevicePathType (DevicePathA)) {
          case HARDWARE_DEVICE_PATH:
            switch (DevicePathSubType (DevicePathA)) {
              case HW_PCI_DP:
                PciPathA = (PCI_DEVICE_PATH *)DevicePathA;
                PciPathB = (PCI_DEVICE_PATH *)DevicePathB;
                Result   = PciPathA->Device - PciPathB->Device;
                if (Result == 0) {
                  Result = PciPathA->Function - PciPathB->Function;
                }

              default:
                Result = CompareMem (DevicePathA, DevicePathB, DevicePathNodeLength (DevicePathA));
            }

          case MESSAGING_DEVICE_PATH:
            switch (DevicePathSubType (DevicePathA)) {
              case MSG_USB_DP:
                UsbPathA = (USB_DEVICE_PATH *)DevicePathA;
                UsbPathB = (USB_DEVICE_PATH *)DevicePathB;
                Result   = UsbPathA->InterfaceNumber - UsbPathB->InterfaceNumber;
                if (Result == 0) {
                  Result = UsbPathA->ParentPortNumber - UsbPathB->ParentPortNumber;
                }

              default:
                Result = CompareMem (DevicePathA, DevicePathB, DevicePathNodeLength (DevicePathA));
            }

          default:
            Result = CompareMem (DevicePathA, DevicePathB, DevicePathNodeLength (DevicePathA));
            break;
        }
      } else {
        Result = CompareMem (DevicePathA, DevicePathB, CompareLength);
      }

      if (Result != 0) {
        break;           // Exit while loop.
      }
    }

    DevicePathA = NextDevicePathNode (DevicePathA);
    DevicePathB = NextDevicePathNode (DevicePathB);
    LengthA     = GetDevicePathSize (DevicePathA);
    LengthB     = GetDevicePathSize (DevicePathB);

    CompareLength = LengthA;      // CompareLength=MIN(LengthA,LengthB);
    if (LengthB < LengthA) {
      CompareLength = LengthB;
    }
  }

  return (Result >= 0);
}

VOID
DisplayDevicePaths (
  EFI_HANDLE  *HandleBuffer,
  UINTN       HandleCount
  )
{
  UINTN   i;
  UINT16  *Tmp;

  for (i = 0; i < HandleCount; i++) {
    Tmp = ConvertDevicePathToText (DevicePathFromHandle (HandleBuffer[i]), TRUE, TRUE);
    if (Tmp != NULL) {
      DEBUG ((DEBUG_INFO, "%3d %s", i, Tmp));    // Output newline in different call as
    } else {
      DEBUG ((DEBUG_INFO, "%3d NULL\n", i));
    }

    DEBUG ((DEBUG_INFO, "\n"));    // Device Paths can be longer than DEBUG limit.
    if (Tmp != NULL) {
      FreePool (Tmp);
    }
  }
}

VOID
SortHandles (
  EFI_HANDLE  *HandleBuffer,
  UINTN       HandleCount
  )
{
  UINTN                     Index;
  EFI_DEVICE_PATH_PROTOCOL  *DevicePathA;
  EFI_DEVICE_PATH_PROTOCOL  *DevicePathB;
  EFI_HANDLE                *TempHandle;
  BOOLEAN                   Swap;
  UINTN                     SwapCount;

  DEBUG ((DEBUG_INFO, "%a\n", __FUNCTION__));
  if (HandleCount < 2) {
    return;
  }

  SwapCount = 0;
  DEBUG ((DEBUG_INFO, "SortHandles - Before sorting\n"));
  DisplayDevicePaths (HandleBuffer, HandleCount);
  do {
    Swap = FALSE;
    for (Index = 0; Index < (HandleCount - 1); Index++) {
      DevicePathA = DevicePathFromHandle (HandleBuffer[Index]);
      DevicePathB = DevicePathFromHandle (HandleBuffer[Index + 1]);
      if ((DevicePathA == NULL) || (DevicePathB == NULL)) {
        ASSERT (DevicePathA != NULL);
        ASSERT (DevicePathB != NULL);
        return;
      }

      if (CompareDevicePathAgtB (DevicePathA, DevicePathB)) {
        TempHandle              = HandleBuffer[Index];
        HandleBuffer[Index]     = HandleBuffer[Index + 1];
        HandleBuffer[Index + 1] = TempHandle;
        Swap                    = TRUE;
      }
    }

    if (Swap) {
      SwapCount++;
    }
  } while ((Swap == TRUE) && (SwapCount < 50));

  DEBUG ((DEBUG_INFO, "SortHandles - After sorting\n"));
  DisplayDevicePaths (HandleBuffer, HandleCount);
  DEBUG ((DEBUG_INFO, "Exit %a, SwapCount = %d\n", __FUNCTION__, SwapCount));
  return;
}

EFI_STATUS
SelectAndBootDevice (
  EFI_GUID        *ByGuid,
  FILTER_ROUTINE  ByFilter
  )
{
  EFI_STATUS                    Status;
  EFI_HANDLE                    *Handles;
  UINTN                         HandleCount;
  UINTN                         Index;
  EFI_BOOT_MANAGER_LOAD_OPTION  BootOption;
  CHAR16                        *TmpStr;
  EFI_DEVICE_PATH_PROTOCOL      *DevicePath;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  ByGuid,
                  NULL,
                  &HandleCount,
                  &Handles
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unable to locate any %g handles - code=%r\n", ByGuid, Status));
    return Status;
  }

  DEBUG ((DEBUG_INFO, "Found %d handles\n", HandleCount));
  DisplayDevicePaths (Handles, HandleCount);
  FilterHandles (Handles, &HandleCount, ByFilter);
  DEBUG ((DEBUG_INFO, "%d handles survived filtering\n", HandleCount));
  if (HandleCount == 0) {
    DEBUG ((DEBUG_WARN, "No handles survived filtering!\n"));
    return EFI_NOT_FOUND;
  }

  SortHandles (Handles, HandleCount);

  Status = EFI_DEVICE_ERROR;
  for (Index = 0; Index < HandleCount; Index++) {
    DevicePath = DevicePathFromHandle (Handles[Index]);
    if (DevicePath == NULL) {
      DEBUG ((DEBUG_ERROR, "DevicePathFromHandle(%p) FAILED\n", Handles[Index]));
      continue;
    }

    TmpStr = ConvertDevicePathToText (DevicePath, TRUE, TRUE);
    if (TmpStr == NULL) {
      DEBUG ((DEBUG_ERROR, "ConvertDevicePathToText(%p) FAILED ", DevicePath));
      continue;
    }

    DEBUG ((DEBUG_INFO, "Selecting device %s", TmpStr));
    DEBUG ((DEBUG_INFO, "\n"));
    if (MsBootPolicyLibIsDeviceBootable (Handles[Index])) {
      EfiBootManagerInitializeLoadOption (
        &BootOption,
        LoadOptionNumberUnassigned,
        LoadOptionTypeBoot,
        LOAD_OPTION_ACTIVE,
        L"MsTemp",
        DevicePathFromHandle (Handles[Index]),
        NULL,
        0
        );

      if ((ByFilter == FilterOnlyIPv4) || (ByFilter == FilterOnlyIPv6) || (ByFilter == FilterOnlyUSB)) {
        DEBUG ((DEBUG_INFO, "Attempting alternate boot...\n"));
        Status = SetAltBoot ();
        if (EFI_ERROR (Status) != FALSE) {
          DEBUG ((DEBUG_ERROR, "Alternate boot set failed %r...\n", Status));
        }
      }

      EfiBootManagerBoot (&BootOption);
      Status = BootOption.Status;

      EfiBootManagerFreeLoadOption (&BootOption);
      // if EFI_SUCCESS, device was booted, and the return is back to setup
      if (Status == EFI_SUCCESS) {
        break;
      }
    } else {
      DEBUG ((DEBUG_WARN, "Device %s\n", TmpStr));
      DEBUG ((DEBUG_WARN, " was blocked from booting\n"));
    }

    FreePool (TmpStr);
  }

  return Status;
}

/**
  Cleans the LoadOption Variables Boot####/Driver#### based on the LoadOptionType.
  Removes any Boot####/Driver#### that is not present in the BootOrder/DriverOrder

  @param  OptionType    ENUM either LoadOptionTypeBoot/LoadOptionTypeDriver

  @return Status        EFI_SUCCESS if at least one variable was deleted using SetVariable.
                        EFI_NOT_FOUND if the variable list has been traversed entirely and no option variable was deleted.
                        Any other EF_ERROR will mean an error executing the function itself.
**/
EFI_STATUS
CleanLoadOptions (
  IN  EFI_BOOT_MANAGER_LOAD_OPTION_TYPE  OptionType
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  CHAR16      *Name  = NULL;
  EFI_GUID    Guid;
  UINTN       NameSize;
  UINTN       NewNameSize;
  CHAR16      *ArrayList;
  UINT16      Count = 0;
  UINTN       OrderNumber;
  UINTN       OptionCount;
  UINT16      *OptionOrder;
  UINTN       OptionOrderSize;
  BOOLEAN     Flag = FALSE;
  CHAR16      OrderName[sizeof (L"Driver####")];
  CHAR16      *OptionName   = NULL;
  CHAR16      *OptionFormat = NULL;
  CHAR16      *StrStart     = NULL;
  UINTN       OptionLength  = 0;

  NameSize  = sizeof (CHAR16);
  Name      = AllocateZeroPool (NameSize);
  ArrayList = NULL;

  DEBUG ((DEBUG_INFO, "%a Entry \n", __FUNCTION__));

  // Based on Load option type create strings for string comparisons

  if (OptionType == LoadOptionTypeBoot) {
    OptionName   = L"Boot####";
    OptionFormat = L"Boot%04x";
    StrStart     = L"Boot";
    OptionLength = StrLen (OptionName) + 1;
  } else {
    OptionName   = L"Driver####";
    OptionFormat = L"Driver%04x";
    StrStart     = L"Driver";
  }

  while (TRUE) {
    NewNameSize = NameSize;
    Status      = gRT->GetNextVariableName (&NewNameSize, Name, &Guid);

    if (Status == EFI_BUFFER_TOO_SMALL) {
      Name = ReallocatePool (NameSize, NewNameSize, Name);
      ASSERT (Name != NULL);
      Status   = gRT->GetNextVariableName (&NewNameSize, Name, &Guid);
      NameSize = NewNameSize;
    }

    if (Status == EFI_NOT_FOUND) {
      break;
    }

    ASSERT_EFI_ERROR (Status);

    if (!Name) {
      continue;
    }

    if (!CompareGuid (&Guid, &gEfiGlobalVariableGuid) || (StrSize (Name) != StrSize (OptionName)) || (StrnCmp (Name, StrStart, StrLen (StrStart)) != 0) || (StrCmp (Name, L"BootNext") == 0)) {
      continue;
    }

    // Add this variable to your array list.
    ArrayList = ReallocatePool (Count * OptionLength * sizeof (CHAR16), (Count + 1) * OptionLength * sizeof (CHAR16), ArrayList);

    ASSERT (ArrayList != NULL);

    StrCpyS (&ArrayList[Count * OptionLength], OptionLength, Name);

    Count++;
  }

  GetVariable2 (
    L"BootOrder",
    &gEfiGlobalVariableGuid,
    (VOID **)&OptionOrder,
    &OptionOrderSize
    );

  for (OptionCount = 0; OptionCount < Count; OptionCount++) {
    Flag = FALSE;

    for (OrderNumber = 0; OrderNumber < OptionOrderSize / sizeof (UINT16); OrderNumber++) {
      UnicodeSPrint (OrderName, sizeof (OrderName), OptionFormat, OptionOrder[OrderNumber]);

      if ((OrderName != NULL) && (StrCmp (OrderName, &ArrayList[OptionCount * OptionLength]) == 0)) {
        Flag = TRUE;
        break;
      }
    }

    // If Flag has not been set to TRUE, then the Boot####/Driver#### stored in ArrayList was not found in BootOrderVariable. So Delete it.
    if (Flag == FALSE) {
      Status = gRT->SetVariable (
                      &ArrayList[OptionCount * OptionLength],
                      &gEfiGlobalVariableGuid,
                      0,
                      0,
                      NULL
                      );
      DEBUG ((DEBUG_INFO, "%a Deleting the Unused load option %s\n", __FUNCTION__, &ArrayList[OptionCount * OptionLength]));
    }
  }

  // Clean all Pointers
  if (Name != NULL) {
    FreePool (Name);
  }

  if (ArrayList) {
    FreePool (ArrayList);
  }

  return Status;
}

/**
Pauses for a defined number of seconds to allow USB mass storage devices enumerate
through hubs that may take a long time (hundreds of ms) to power up and enumerate.
Our test system is forced to use hubs on BB of some units due to lack of connectivity
options.

No params
No return value.  On failure the delay just doesn't happen, which is not fatal.
**/
STATIC
void
PauseToLetUsbDrivesEnumerateThroughHubs (
  VOID
  )
{
  EFI_STATUS  Status;
  EFI_EVENT   PauseEvent;
  UINTN       SignalIndex;

  Status = gBS->CreateEvent (EVT_TIMER, TPL_NOTIFY, NULL, NULL, &PauseEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Could not create event! %r\n", Status));
    return;
  }

  // 100ns units. so *10 = us, then *1000 = ms, then *1000 again = s
  Status = gBS->SetTimer (PauseEvent, TimerRelative, 10 * 1000 * 1000 * USB_DRIVE_SECOND_CHANCE_DELAY_S);
  if (!EFI_ERROR (Status)) {
    Status = gBS->WaitForEvent (1, &PauseEvent, &SignalIndex);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Wait for Event failed! %r\n", Status));
    }
  }

  gBS->CloseEvent (PauseEvent);
}

/**
  Entry to Ms Boot Policy.

  @param ImageHandle     The image handle.
  @param SystemTable     The system table.
  @return Status         From internal routine or boot object

 **/
EFI_STATUS
EFIAPI
MsBootPolicyEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                    Status;
  EFI_STATUS                    GraphicStatus;
  BOOT_SEQUENCE                *BootSequence;             // Pointer to array of boot sequence entries
  UINTN                         Index;
  EFI_LOADED_IMAGE_PROTOCOL    *ImageInfo = NULL;
  EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput;
  CHAR8                        *Parameters;
  BOOLEAN                       EnableIPv6     = TRUE;
  BOOLEAN                       AltBootRequest = FALSE;
  UINT32                        DisplayHeight;
  INTN                          DestX;
  INTN                          DestY;

  // Locate Gop Protocol
  Status = gBS->HandleProtocol (gST->ConsoleOutHandle, &gEfiGraphicsOutputProtocolGuid, (VOID **)&GraphicsOutput);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a - Failed to find GOP on ConsoleOutHandle. %r\n", __FUNCTION__, Status));
    // failed on console out.  Try globally within system
    Status = gBS->LocateProtocol (&gEfiGraphicsOutputProtocolGuid, NULL, (VOID **)&GraphicsOutput);
  }

  // At one time, there was a use for loading MsBootPolicy.efi as a DXE_DRIVER in addition as an application.
  // This is no longer the case.  However, if we are called without parameters, still attempt a default boot.
  Status = gBS->HandleProtocol (ImageHandle, &gEfiLoadedImageProtocolGuid, (VOID **)&ImageInfo);
  ASSERT_EFI_ERROR (Status);
  if ((ImageInfo == NULL) ||
      (ImageInfo->LoadOptionsSize == 0) ||
      (ImageInfo->LoadOptions == NULL))
  {
    Parameters = "MS";
  } else {
    Parameters = (CHAR8 *)ImageInfo->LoadOptions;
  }

  DEBUG ((DEBUG_INFO, "%a parameter = %a\n", __FUNCTION__, Parameters));

  // Entry is from BDS as a boot option.
  switch (*Parameters) {
    case 'U':       // "USB"
      BootSequence = mUsbBootSequence;
      break;
    case 'P':       // "PXE"
      BootSequence = mPXEBootSequence;
      break;
    case 'S':       // "SDD"
      BootSequence = mSddBootSequence;
      break;
    case 'M':      // "MS" Default of SDD->USB->PXE  , "MA" Default of USB->PXE->SDD
    default:       // Try the default boot option is the parameter is messed up
      AltBootRequest = ('A' == *(Parameters + 1));
      Status         = MsBootPolicyLibGetBootSequence (&BootSequence, AltBootRequest);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Unable to get boot sequence - using hard coded sequence.\n"));
        BootSequence = mDefaultBootSequence;
      }

      break;
  }

  EfiBootManagerConnectAll ();     // Connect All is required for this type of boot

  CleanLoadOptions (LoadOptionTypeBoot);    // Insure there are no dangling BOOT#### options.

  Status = EFI_SUCCESS;
  DEBUG ((DEBUG_INFO, "%a starting with parm %c\n", __FUNCTION__, *Parameters));
  Index = 0;
  while (BootSequence[Index] != MsBootDone) {
    DEBUG ((DEBUG_INFO, "Attempting boot type %d\n", BootSequence[Index]));
    switch (BootSequence[Index]) {
      case MsBootPXE4:
        StartNetworking ();
        Status = SelectAndBootDevice (&gEfiLoadFileProtocolGuid, FilterOnlyIPv4);
        break;
      case MsBootPXE6:
        Status = GetBootManagerSetting (
                   DFCI_SETTING_ID__IPV6,
                   &EnableIPv6
                   );
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "%a unable to get IPv6 setting, using default\n", __FUNCTION__));
        }

        if (EnableIPv6) {
          StartNetworking ();
          Status = SelectAndBootDevice (&gEfiLoadFileProtocolGuid, FilterOnlyIPv6);
        } else {
          Status = EFI_DEVICE_ERROR;
        }

        break;
      case MsBootHDD:
        GraphicStatus = SetGraphicsConsoleMode (GCM_NATIVE_RES);
        if (EFI_ERROR (GraphicStatus) != FALSE) {
          DEBUG ((DEBUG_ERROR, "%a Unable to set console mode - %r\n", __FUNCTION__, GraphicStatus));
        }

        Status = SelectAndBootDevice (&gEfiSimpleFileSystemProtocolGuid, FilterNoUSB);
        break;
      case MsBootUSB:
        GraphicStatus = SetGraphicsConsoleMode (GCM_NATIVE_RES);
        if (EFI_ERROR (GraphicStatus) != FALSE) {
          DEBUG ((DEBUG_ERROR, "%a Unable to set console mode - %r\n", __FUNCTION__, GraphicStatus));
        }

        Status = SelectAndBootDevice (&gEfiSimpleFileSystemProtocolGuid, FilterOnlyUSB);
        if (Status == EFI_NOT_FOUND) {
          DEBUG ((DEBUG_WARN, "USB boot desired, but no USB devices found on first attempt\n"));
          // attempting USB boot but no USB devices were found.
          // USB enumeration through (crappy, slow) hubs may take a while, especially in
          // debug builds. wait a number of seconds and try one more time.
          PauseToLetUsbDrivesEnumerateThroughHubs ();
          Status = SelectAndBootDevice (&gEfiSimpleFileSystemProtocolGuid, FilterOnlyUSB);
          if (EFI_ERROR (Status)) {
            DEBUG ((DEBUG_WARN, "Second chance USB boot failed! Status = %r\n", Status));

            // Set Native Resolution
            GraphicStatus = SetGraphicsConsoleMode (GCM_NATIVE_RES);
            if (EFI_ERROR (GraphicStatus) != FALSE) {
                DEBUG ((DEBUG_ERROR, "%a Unable to set console mode - %r\n", __FUNCTION__, GraphicStatus));
            }

            // Set Display Location of No Boot OS Image
            DisplayHeight = GraphicsOutput->Mode->Info->HorizontalResolution;
            DestX         = (DisplayHeight - 334) / 2;
            DestY         = 20;

            // Display No Boot OS Logo
            GraphicStatus = DisplayPostBootGraphic (BG_NO_BOOT_OS, DestX, DestY);
            if (EFI_ERROR (GraphicStatus) != FALSE) {
                DEBUG ((DEBUG_ERROR, "%a Unable to set graphics - %r\n", __FUNCTION__, GraphicStatus));
            }

            // Do Cpu Dead Loop
            CpuDeadLoop ();
          }
        }

        break;
      default:
        DEBUG ((DEBUG_ERROR, "Invalid BootSequence value %x\n", BootSequence[Index]));
        Status = EFI_INVALID_PARAMETER;
        break;
    }

    // A Boot Option that returns EFI_SUCCESS, exits to the settings page.
    if (Status == EFI_SUCCESS) {
      break;
    }

    Index++;
  }

  // Need to populate GraphicStatus here, otherwise logo function return value will override boot result
  GraphicStatus = SetGraphicsConsoleMode (GCM_NATIVE_RES);
  if (EFI_ERROR (GraphicStatus) != FALSE) {
    DEBUG ((DEBUG_ERROR, "%a Unable to set console mode - %r\n", __FUNCTION__, GraphicStatus));
    goto Done;
  }

  GraphicStatus = DisplayBootGraphic (BG_SYSTEM_LOGO);
  if (EFI_ERROR (GraphicStatus) != FALSE) {
    DEBUG ((DEBUG_ERROR, "%a Unable to set graphics - %r\n", __FUNCTION__, GraphicStatus));
  }

Done:
  DEBUG ((DEBUG_INFO, "Exit boot policy. Index=%d, BS=%d, Status=%r\n", Index, BootSequence[Index], Status));
  return Status;
}
