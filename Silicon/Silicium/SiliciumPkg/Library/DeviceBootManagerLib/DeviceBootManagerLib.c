/**
  DeviceBootManagerLib - Generic Extension to BdsDxe.

  Copyright (C) Microsoft Corporation. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <DfciSystemSettingTypes.h>

#include <Library/DebugLib.h>
#include <Library/DeviceBootManagerLib.h>
#include <Library/MsBootManagerSettingsLib.h>
#include <Library/MsBootOptionsLib.h>
#include <Library/MsBootPolicyLib.h>
#include <Library/BootGraphicsLib.h>
#include <Library/BootGraphics.h>
#include <Library/GraphicsConsoleHelperLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MsPlatformDevicesLib.h>
#include <Library/MsPlatformPowerCheckLib.h>
#include <Library/PcdLib.h>
#include <Library/PrintLib.h>
#include <Library/PowerServicesLib.h>
#include <Library/ThermalServicesLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MsNVBootReasonLib.h>
#include <Library/VariablePolicyHelperLib.h>
#include <Library/UpdateFacsHardwareSignatureLib.h>

#include <Guid/GlobalVariable.h>

#include <Protocol/OnScreenKeyboard.h>

#include <Settings/BootMenuSettings.h>

STATIC EFI_EVENT mPreReadyToBootEvent;
STATIC EFI_EVENT mPostReadyToBootEvent;

STATIC
VOID
ThermalFailureShutdown ()
{
  EFI_STATUS  Status     = EFI_SUCCESS;
  EFI_EVENT   TimerEvent = NULL;

  // Display the Too Hot Picture
  Status = DisplayBootGraphic (BG_CRITICAL_OVER_TEMP);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Display THOT Picture! Status = %r\n", __FUNCTION__, Status));
  }

  // Create a Timer Event
  Status = gBS->CreateEvent (EVT_TIMER, 0, NULL, NULL, &TimerEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Create Timer Event! Status = %r\n", __FUNCTION__, Status));
  }

  // Set Timer using the Timer Event
  gBS->SetTimer (TimerEvent, TimerRelative, PcdGet32 (PcdShutdownGraphicDisplayTime));

  // Wait for the Event to Fire
  Status = gBS->WaitForEvent (1, &TimerEvent, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to wait for the Timer Event! Status = %r\n" ,__FUNCTION__,  Status));
  }

  // Shutdown Device
  gRT->ResetSystem (EfiResetShutdown, EFI_SUCCESS, 0, NULL);

  DEBUG ((EFI_D_ERROR, "Failed to Shutdown System! System Halted.\n"));

  // Halt System
  CpuDeadLoop ();
}

STATIC
VOID
PowerFailureShutdown ()
{
  EFI_STATUS  Status     = EFI_SUCCESS;
  EFI_EVENT   TimerEvent = NULL;

  // Display the Low Battery Picture
  Status = DisplayBootGraphic (BG_CRITICAL_LOW_BATTERY);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Display LBAT Picture! Status = %r\n", __FUNCTION__, Status));
  }

  // Create a Timer Event
  Status = gBS->CreateEvent (EVT_TIMER, 0, NULL, NULL, &TimerEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Create Timer Event! Status = %r\n", __FUNCTION__, Status));
  }

  // Set Timer using the Timer Event
  gBS->SetTimer (TimerEvent, TimerRelative, PcdGet32 (PcdShutdownGraphicDisplayTime));

  // Wait for the Event to Fire
  Status = gBS->WaitForEvent (1, &TimerEvent, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to wait for the Timer Event! Status = %r\n" ,__FUNCTION__,  Status));
  }

  // Shutdown Device
  gRT->ResetSystem (EfiResetShutdown, EFI_SUCCESS, 0, NULL);

  // Halt System
  CpuDeadLoop ();
}

STATIC
EFI_STATUS
MsPreBootChecks ()
{
  EFI_STATUS  Status              = EFI_SUCCESS;
  EFI_STATUS  WaitStatus          = EFI_SUCCESS;
  EFI_EVENT   TimerEvent          = NULL;
  BOOLEAN     ThermalGood         = TRUE;
  BOOLEAN     PowerGood           = TRUE;
  INT32       RetryCount          = PcdGet32 (PcdPowerAndThermalRetries);
  UINT32      ThermalFailureCount = 1;

  // Create a Timer Event
  Status = gBS->CreateEvent (EVT_TIMER, 0, NULL, NULL, &TimerEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Create Timer Event! Status %r\n", __FUNCTION__, Status));
    goto CleanUp;
  }

  do {
    // Check if the Device Battery isn't too Empty
    Status = SystemPowerCheck (PowerCaseBoot, &PowerGood);

    if ((!EFI_ERROR (Status)) && (!PowerGood)) {
      // Mitigate System Power
      Status = SystemPowerMitigate (PowerCaseBoot);

      if (EFI_ERROR (Status)) {
        PowerGood = FALSE;
        goto CleanUp;
      }
    } else {
      PowerGood = TRUE;
    }

    // Check if the Device isn't Too Hot
    Status = SystemThermalCheck (ThermalCaseBoot, &ThermalGood);

    if ((!EFI_ERROR (Status)) && (!ThermalGood)) {
      if (ThermalFailureCount == 1) {
        // TODO: Add Active Coling Event here
      }

      if (ThermalFailureCount == 2) {
        // TODO: Add Passive Colling Event here
      }

      if (ThermalFailureCount < 3) {
        ThermalFailureCount++;
      }

      if (RetryCount > 0) {
        // Set Timer using the Timer Event
        gBS->SetTimer (TimerEvent, TimerRelative, PcdGet32 (PcdThermalControlRetryWait));

        // Wait for the Event to Fire
        WaitStatus = gBS->WaitForEvent (1, &TimerEvent, 0);
        if (EFI_ERROR (WaitStatus)) {
          DEBUG ((EFI_D_ERROR, "%a: Failed to Wait for Timer Event! Status = %r\n", __FUNCTION__, WaitStatus));
        }
      }
    } else {
      ThermalGood = TRUE;
    }
  } while ((RetryCount-- > 0) && ((!ThermalGood) || (!PowerGood)));

CleanUp:
  if (TimerEvent != NULL) {
    gBS->CloseEvent (TimerEvent);
  }

  if (!ThermalGood) {
    ThermalFailureShutdown ();
  }

  if (!PowerGood) {
    PowerFailureShutdown ();
  }

  return Status;
}

STATIC
VOID
BdsBootLockBootVariables ()
{
  EFI_STATUS                      Status              = EFI_SUCCESS;
  EDKII_VARIABLE_POLICY_PROTOCOL *VarPolicyProtocol   = NULL;
  CHAR16                          OptionName[sizeof ("Boot####")];
  UINT16                         *BootOrder           = 0;
  UINTN                           BootOrderSize       = 0;
  BOOLEAN                         EnableBootOrderLock = FALSE;
  STATIC BOOLEAN                  AlreadyLocked       = FALSE;

  if (AlreadyLocked) {
    return;
  }

  // Get Boot Order Lock Setting from DFCI
  Status = GetBootManagerSetting (DFCI_SETTING_ID__BOOT_ORDER_LOCK, &EnableBootOrderLock);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to get BootOrderLock Setting! Status = %r\n", __FUNCTION__, Status));
    return;
  }

  if (!EnableBootOrderLock) {
    return;
  }

  // Locate Variable Policy Protocol
  Status = gBS->LocateProtocol (&gEdkiiVariablePolicyProtocolGuid, NULL, (VOID **)&VarPolicyProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate Variable Policy Protocol! Status = %r\n", __FUNCTION__, Status));
    return;
  }

  // Lock BootOrder Variable
  Status = RegisterBasicVariablePolicy (VarPolicyProtocol, &gEfiGlobalVariableGuid, EFI_BOOT_ORDER_VARIABLE_NAME, VARIABLE_POLICY_NO_MIN_SIZE, VARIABLE_POLICY_NO_MAX_SIZE, VARIABLE_POLICY_NO_MUST_ATTR, VARIABLE_POLICY_NO_CANT_ATTR, VARIABLE_POLICY_TYPE_LOCK_NOW);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Unable to Lock BootOrder! Status = %r\n", __FUNCTION__, Status));
  }

  // Delete BootNext as a Locked BootNext is a Bad Thing
  gRT->SetVariable (EFI_BOOT_NEXT_VARIABLE_NAME, &gEfiGlobalVariableGuid, 0, 0, NULL);

  // Lock BootNext Variable
  Status = RegisterBasicVariablePolicy (VarPolicyProtocol, &gEfiGlobalVariableGuid, EFI_BOOT_NEXT_VARIABLE_NAME, VARIABLE_POLICY_NO_MIN_SIZE, VARIABLE_POLICY_NO_MAX_SIZE, VARIABLE_POLICY_NO_MUST_ATTR, VARIABLE_POLICY_NO_CANT_ATTR, VARIABLE_POLICY_TYPE_LOCK_NOW);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Unable to Lock BootNext! Status = %r\n", __FUNCTION__, Status));
  }

  // Get BootOrder Variable
  GetVariable2 (L"BootOrder", &gEfiGlobalVariableGuid, (VOID **)&BootOrder, &BootOrderSize);
  if (BootOrder == NULL) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Get BootOrder Variable! Status = %r\n", __FUNCTION__, Status));
    return;
  }

  // Lock all Boot Options
  for (UINTN i = 0; i < (BootOrderSize / sizeof (UINT16)); i++) {
    UnicodeSPrint (OptionName, sizeof (OptionName), L"Boot%04x", BootOrder[i]);

    Status = RegisterBasicVariablePolicy (VarPolicyProtocol, &gEfiGlobalVariableGuid, OptionName, VARIABLE_POLICY_NO_MIN_SIZE, VARIABLE_POLICY_NO_MAX_SIZE, VARIABLE_POLICY_NO_MUST_ATTR, VARIABLE_POLICY_NO_CANT_ATTR, VARIABLE_POLICY_TYPE_LOCK_NOW);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Unable to Lock %s! Status = %r\n", __FUNCTION__, OptionName, Status));
    }
  }
}

STATIC
VOID
EnableOSK ()
{
  EFI_STATUS                         Status      = EFI_SUCCESS;
  MS_ONSCREEN_KEYBOARD_PROTOCOL     *OSKProtocol = NULL;
  MS_SIMPLE_WINDOW_MANAGER_PROTOCOL *SWMProtocol = NULL;

  // Locate OSK Protocol
  Status = gBS->LocateProtocol (&gMsOSKProtocolGuid, NULL, (VOID **)&OSKProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate OSK Protocol! Status = %r\n", __FUNCTION__, Status));
  } else {
    //
    // Configure the OSK Size, Position, and State (75% of Screen Width, Bottom right
    // Position, Docked) - this is most Compatible with the Windows 10 English-Localized
    // Bitlocker PIN Screen. The Keyboard is not Displayed by Default.
    //
    OSKProtocol->ShowKeyboard            (OSKProtocol, FALSE);
    OSKProtocol->SetKeyboardSize         (OSKProtocol, 75);
    OSKProtocol->SetKeyboardPosition     (OSKProtocol, BottomRight, Docked);
    OSKProtocol->ShowDockAndCloseButtons (OSKProtocol, TRUE);

    //
    // Configure the OSK Icon Position (used to Launch OSK). The Keyboard
    // Icon is not Displayed by Default.
    //
    OSKProtocol->ShowKeyboardIcon        (OSKProtocol, FALSE);
    OSKProtocol->SetKeyboardIconPosition (OSKProtocol, BottomRight);

    //
    // Configure the OSK Icon for Auto-Enable and Self-Refresh Modes.
    //
    OSKProtocol->SetKeyboardMode         (OSKProtocol, (OSK_MODE_AUTOENABLEICON | OSK_MODE_SELF_REFRESH));
  }

  // Locate SWM Protocol
  Status = gBS->LocateProtocol (&gMsSWMProtocolGuid, NULL, (VOID **)&SWMProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate SWM Protocol! Status = %r\n", Status));
  } else {
    // Disable the Mouse Pointer.
    SWMProtocol->EnableMousePointer (SWMProtocol, FALSE);
  }
}

STATIC
VOID
EFIAPI
PreReadyToBoot (
  IN EFI_EVENT Event,
  IN VOID     *Context)
{
  BdsBootLockBootVariables ();
  EnableOSK ();

  gBS->CloseEvent (Event);
}

STATIC
VOID
EFIAPI
PostReadyToBoot (
  IN EFI_EVENT Event,
  IN VOID     *Context)
{
  // Do Nothing
}

EFI_STATUS
EFIAPI
DeviceBootManagerConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Register OnReadyToBoot Event
  Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL, TPL_CALLBACK, PreReadyToBoot, NULL, &gEfiEventPreReadyToBootGuid, &mPreReadyToBootEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Register OnReadyToBoot Event! Status = %r\n", __FUNCTION__, Status));
  }

  // Register OnPostReadyToBoot Event
  Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL, TPL_CALLBACK, PostReadyToBoot, NULL, &gEfiEventPostReadyToBootGuid, &mPostReadyToBootEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Eegister OnPostReadyToBoot Event! Status = %r\n", Status));
  }

  return EFI_SUCCESS;
}

EFI_DEVICE_PATH_PROTOCOL**
EFIAPI
DeviceBootManagerOnDemandConInConnect () { return GetPlatformConnectOnConInList (); }

VOID
EFIAPI
DeviceBootManagerBdsEntry ()
{
  // Signal that BDS Started
  EfiEventGroupSignal (&gMsStartOfBdsNotifyGuid);

  // Signal that DFCI Started
  EfiEventGroupSignal (&gDfciStartOfBdsNotifyGuid);

  UpdateRebootReason ();
}

EFI_HANDLE
EFIAPI
DeviceBootManagerBeforeConsole (
  EFI_DEVICE_PATH_PROTOCOL  **DevicePath,
  BDS_CONSOLE_CONNECT_ENTRY **PlatformConsoles)
{
  MsBootOptionsLibRegisterDefaultBootOptions ();

  *PlatformConsoles = GetPlatformConsoleList ();

  return GetPlatformPreferredConsole (DevicePath);
}

EFI_DEVICE_PATH_PROTOCOL**
EFIAPI
DeviceBootManagerAfterConsole ()
{
  EFI_STATUS Status;

  // Check if System is Good to Go
  MsPreBootChecks ();

  // Check System Power Level
  PlatformPowerLevelCheck ();

  // Display Boot Logo on Screen
  Status = DisplayBootGraphic (BG_SYSTEM_LOGO);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Display Boot Logo! Status = %r\n", __FUNCTION__, Status));
  }

  return GetPlatformConnectList ();
}

VOID
EFIAPI
DeviceBootManagerProcessBootCompletion (IN EFI_BOOT_MANAGER_LOAD_OPTION *BootOption)
{
  EFI_STATUS  Status        = EFI_SUCCESS;
  EFI_STATUS  RestartStatus = EFI_SUCCESS;
  BOOLEAN     MsBootNext    = FALSE;

  // Get MsBootNext Variable
  Status = gRT->GetVariable (L"MsBootNext", &gMsBootMenuFormsetGuid, NULL, 0, &MsBootNext);
  if (!EFI_ERROR (Status)) {
    // Set MsBootNext Variable
    Status = gRT->SetVariable (L"MsBootNext", &gMsBootMenuFormsetGuid, EFI_VARIABLE_BOOTSERVICE_ACCESS, 0, NULL);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Set MsBootNext Variable! Status = %r\n", __FUNCTION__, Status));
    }
  }

  RestartStatus = BootOption->Status;

  if (BootOption->Status == OEM_PREVIOUS_SECURITY_VIOLATION) {
    RestartStatus = EFI_SECURITY_VIOLATION;
  }

  if (MsBootNext) {
    SetRebootReason (RestartStatus);
  }

  SetRebootReason (BootOption->Status);

  // Set Graphics Console to Native Res
  Status = SetGraphicsConsoleMode (GCM_NATIVE_RES);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Unabled to set Console Mode! Status = %r\n", __FUNCTION__, Status));
  }
}

EFI_STATUS
EFIAPI
DeviceBootManagerPriorityBoot (EFI_BOOT_MANAGER_LOAD_OPTION *BootOption)
{
  EFI_STATUS  Status        = EFI_SUCCESS;
  BOOLEAN     FrontPageBoot = MsBootPolicyLibIsSettingsBoot ();
  BOOLEAN     SpecificApp   = MsBootPolicyLibIsAltBoot ();

  MsBootPolicyLibClearBootRequests ();

  //
  // There are four cases:
  //   1.  Nothing pressed.           - Return EFI_NOT_FOUND.
  //   2.  FrontPageBoot              - Load FrontPage.
  //   3.  SpecificApp                - Load SoC Specific App.
  //

  if (SpecificApp) {
    Status = MsBootOptionsLibGetDefaultBootApp (BootOption, "VOL-");
  } else if (FrontPageBoot) {
    Status = MsBootOptionsLibGetBootManagerMenu (BootOption, "VOL+");
    SetRebootReason (OEM_REBOOT_TO_SETUP_KEY);
  } else {
    Status = EFI_NOT_FOUND;
  }

  return Status;
}

VOID
EFIAPI
DeviceBootManagerUnableToBoot ()
{
  EFI_STATUS Status;

  // Display No Boot OS Logo
  Status = DisplayBootGraphic (BG_NO_BOOT_OS);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Display No Boot OS Logo! Status = %r\n", __FUNCTION__, Status));
    Print (L"No Operating System Found! Please make sure that an OS is Installed.\n");
  }

  // Wait 10s
  gBS->Stall(10000000);

  // Shutdown
  gRT->ResetSystem (EfiResetShutdown, EFI_SUCCESS, 0, NULL);

  // Do Cpu Dead Loop
  CpuDeadLoop ();
}
