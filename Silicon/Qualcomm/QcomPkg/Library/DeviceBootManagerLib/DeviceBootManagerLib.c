/** @file
  DeviceBootManager  - Ms Device specific extensions to BdsDxe.

  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi.h>

#include <DfciSystemSettingTypes.h>

#include <Guid/EventGroup.h>
#include <Guid/GlobalVariable.h>
#include <Guid/MsBootMenuGuid.h>
#include <Guid/DfciPacketHeader.h>

#include <Protocol/OnScreenKeyboard.h>
#include <Protocol/TpmPpProtocol.h>

#include <Library/BaseMemoryLib.h>
#include <Library/ConsoleMsgLib.h>
#include <Library/DebugLib.h>
#include <Library/DeviceBootManagerLib.h>
#include <Library/DevicePathLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MsBootManagerSettingsLib.h>
#include <Library/MsBootOptionsLib.h>
#include <Library/MsBootOptions.h>
#include <Library/MsBootPolicyLib.h>
#include <Library/MsBootPolicy.h>
#include <Library/BootGraphicsLib.h>
#include <Library/BootGraphics.h>
#include <Library/GraphicsConsoleHelperLib.h>
#include <Library/MsPlatformDevicesLib.h>
#include <Library/MsPlatformPowerCheckLib.h>
#include <Library/MsNetworkDependencyLib.h>
#include <Library/PcdLib.h>
#include <Library/PrintLib.h>
#include <Library/PowerServicesLib.h>
#include <Library/ThermalServicesLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MsNVBootReasonLib.h>
#include <Library/MuTelemetryHelperLib.h>
#include <Library/VariablePolicyHelperLib.h>
#include <Library/UpdateFacsHardwareSignatureLib.h>

#include <Settings/BootMenuSettings.h>
#include <Settings/DfciSettings.h>

STATIC EFI_EVENT mPreReadyToBootEvent;
STATIC EFI_EVENT mPostReadyToBootEvent;

STATIC
VOID
ThermalFailureShutdown (VOID)
{
  EFI_STATUS  Status     = EFI_SUCCESS;
  EFI_EVENT   TimerEvent = NULL;         // Should set to a value
  UINTN       index      = 0;
  UINT32      WaitTime   = PcdGet32 (PcdShutdownGraphicDisplayTime);

  // Display the too hot picture
  Status = DisplayBootGraphic (BG_CRITICAL_OVER_TEMP);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Display THOT Picture! Status = %r\n", __FUNCTION__, Status));
  }

  // Wait a few seconds
  // Create an event
  Status = gBS->CreateEvent (EVT_TIMER, 0, NULL, NULL, &TimerEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Create Timer Event! Status = %r\n", __FUNCTION__, Status));
  }

  // Set timer using the event
  gBS->SetTimer (TimerEvent, TimerRelative, WaitTime);

  // Wait for event to fire
  Status = gBS->WaitForEvent (1, &TimerEvent, &index);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to wait for the Timer Event! Status = %r\n" ,__FUNCTION__,  Status));
  }

  gRT->ResetSystem (EfiResetShutdown, EFI_SUCCESS, 0, NULL);
}

STATIC
VOID
PowerFailureShutdown (VOID)
{
  EFI_STATUS  Status     = EFI_SUCCESS;
  EFI_EVENT   TimerEvent = NULL;        // Should set to a value
  UINTN       index      = 0;
  UINT32      WaitTime   = PcdGet32 (PcdShutdownGraphicDisplayTime);

  // Display the low battery picture
  Status = DisplayBootGraphic (BG_CRITICAL_LOW_BATTERY);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Display LBAT Picture! Status = %r\n", __FUNCTION__, Status));
  }

  // Wait a few seconds
  // Create an event
  Status = gBS->CreateEvent (EVT_TIMER, 0, NULL, NULL, &TimerEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Create Timer Event! Status = %r\n", __FUNCTION__, Status));
  }

  // Set timer using the event
  gBS->SetTimer (TimerEvent, TimerRelative, WaitTime);

  // Wait for event to fire
  Status = gBS->WaitForEvent (1, &TimerEvent, &index);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to wait for the Timer Event! Status = %r\n" ,__FUNCTION__,  Status));
  }

  gRT->ResetSystem (EfiResetShutdown, EFI_SUCCESS, 0, NULL);
}

STATIC
EFI_STATUS
MsPreBootChecks (VOID)
{
  EFI_STATUS  Status      = EFI_SUCCESS;
  EFI_STATUS  WaitStatus  = EFI_SUCCESS;
  EFI_EVENT   TimerEvent  = NULL;
  BOOLEAN     ThermalGood = TRUE;
  BOOLEAN     PowerGood   = TRUE;
  INT32       RetryCount  = PcdGet32 (PcdPowerAndThermalRetries);
  UINTN       index       = 0;
  UINT32      WaitTime    = PcdGet32 (PcdThermalControlRetryWait);

  UINT32  ThermalFailureCount = 1;

  // Create an event
  Status = gBS->CreateEvent (EVT_TIMER, 0, NULL, NULL, &TimerEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Create Timer Event! Status %r\n", __FUNCTION__, Status));
    goto CleanUp;
  }

  do {
    // Check to see if the Power situation is good
    Status = SystemPowerCheck (PowerCaseBoot, &PowerGood);

    if ((!EFI_ERROR (Status)) && (!PowerGood)) {
      Status = SystemPowerMitigate (PowerCaseBoot);

      if (EFI_ERROR (Status)) {
        PowerGood = FALSE;
        goto CleanUp;
      }

      // There should be enough power to boot so fall through to next test.
      // Thermal mitigation may burn battery so we will check power once
      // more before booting.
    } else {
      // change - error retrieving power state should not stop boot
      PowerGood = TRUE;
    }

    // Check to see if the thermal situation is good
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

      // Wait for cooling to have an effect but not so long we completely
      // drain the battery. ToDo: should consider adding some UI to let the
      // user know what is going on.
      if (RetryCount > 0) {
        // Set timer using the event
        gBS->SetTimer (TimerEvent, TimerRelative, WaitTime);

        // Wait for event to fire
        WaitStatus = gBS->WaitForEvent (1, &TimerEvent, &index);
        if (EFI_ERROR (WaitStatus)) {
          DEBUG ((EFI_D_ERROR, "%a: Failed to Wait for Timer Event! Status = %r\n", __FUNCTION__, WaitStatus));
        }
      }
    } else {
      // change - error retrieving thermal should not stop boot
      ThermalGood = TRUE;
    }
  } while ((RetryCount-- > 0) && ((!ThermalGood) || (!PowerGood)));

CleanUp:
  if (TimerEvent != NULL) {
    gBS->CloseEvent (TimerEvent);
  }

  if (!ThermalGood) {
    LogTelemetry (TRUE, NULL, EFI_COMPUTING_UNIT_HOST_PROCESSOR | EFI_CU_HP_EC_THERMAL, NULL, NULL, 0, 0);
    ThermalFailureShutdown ();    // Should never return from this function
  }

  if (!PowerGood) {
    LogTelemetry (TRUE, NULL, EFI_COMPUTING_UNIT_HOST_PROCESSOR | EFI_CU_HP_EC_LOW_VOLTAGE, NULL, NULL, 0, 0);
    PowerFailureShutdown ();      // Should never return from this function
  }

  return Status;
}

/**
  Lock the required boot variables if LockBootOrder is enabled
**/
STATIC
VOID
BdsBootLockBootVariables (VOID)
{
  EFI_STATUS                      Status              = EFI_SUCCESS;
  BOOLEAN                         EnableBootOrderLock = FALSE;
  EDKII_VARIABLE_POLICY_PROTOCOL *VarPolicyProtocol   = NULL;
  UINT16                         *BootOrder           = 0;
  UINTN                           BootOrderSize       = 0;
  CHAR16                          OptionName[sizeof ("Boot####")];
  UINTN                           i                   = 0;
  STATIC BOOLEAN                  AlreadyLocked       = FALSE;

  if (AlreadyLocked) {
    // This can happen as we may call ready to boot a number of times;
    return;
  }

  Status = GetBootManagerSetting (DFCI_SETTING_ID__BOOT_ORDER_LOCK, &EnableBootOrderLock);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to get BootOrderLock Setting! Status = %r\n", __FUNCTION__, Status));
    return;
  }

  if (!EnableBootOrderLock) {
    return;
  }

  Status = gBS->LocateProtocol (&gEdkiiVariablePolicyProtocolGuid, NULL, (VOID **)&VarPolicyProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to :ocate Variable Policy Protocol! Status = %r\n", __FUNCTION__, Status));
    return;
  }

  Status = RegisterBasicVariablePolicy (VarPolicyProtocol, &gEfiGlobalVariableGuid, EFI_BOOT_ORDER_VARIABLE_NAME, VARIABLE_POLICY_NO_MIN_SIZE, VARIABLE_POLICY_NO_MAX_SIZE, VARIABLE_POLICY_NO_MUST_ATTR, VARIABLE_POLICY_NO_CANT_ATTR, VARIABLE_POLICY_TYPE_LOCK_NOW);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Unable to Lock BootOrder! Status = %r\n", __FUNCTION__, Status));
  }

  // Delete BootNext as a locked BootNext is a bad thing
  gRT->SetVariable (EFI_BOOT_NEXT_VARIABLE_NAME, &gEfiGlobalVariableGuid, 0, 0, NULL);

  Status = RegisterBasicVariablePolicy (VarPolicyProtocol, &gEfiGlobalVariableGuid, EFI_BOOT_NEXT_VARIABLE_NAME, VARIABLE_POLICY_NO_MIN_SIZE, VARIABLE_POLICY_NO_MAX_SIZE, VARIABLE_POLICY_NO_MUST_ATTR, VARIABLE_POLICY_NO_CANT_ATTR, VARIABLE_POLICY_TYPE_LOCK_NOW);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Unable to Lock BootOrder! Status = %r\n", __FUNCTION__, Status));
  }

  GetVariable2 (L"BootOrder", &gEfiGlobalVariableGuid, (VOID **)&BootOrder, &BootOrderSize);
  if (BootOrder == NULL) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate BootOrder! Status = %r\n", __FUNCTION__, Status));
    return;
  }

  for (i = 0; i < (BootOrderSize / sizeof (UINT16)); i++) {
    UnicodeSPrint (OptionName, sizeof (OptionName), L"Boot%04x", BootOrder[i]);

    Status = RegisterBasicVariablePolicy (VarPolicyProtocol, &gEfiGlobalVariableGuid, OptionName, VARIABLE_POLICY_NO_MIN_SIZE, VARIABLE_POLICY_NO_MAX_SIZE, VARIABLE_POLICY_NO_MUST_ATTR, VARIABLE_POLICY_NO_CANT_ATTR, VARIABLE_POLICY_TYPE_LOCK_NOW);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Unable to Lock %s! Status = %r\n", __FUNCTION__, OptionName, Status));
    }
  }
}

/**
  Enable the On Screen Keyboard
**/
STATIC
VOID
EnableOSK (VOID)
{
  EFI_STATUS                         Status      = EFI_SUCCESS;
  MS_ONSCREEN_KEYBOARD_PROTOCOL     *OSKProtocol = NULL;
  MS_SIMPLE_WINDOW_MANAGER_PROTOCOL *SWMProtocol = NULL;

  //
  // Enable on-screen keyboard icon auto-display mode.  This will cause the keyboard
  // icon to be presented when an application (launched by BDS) waits on the keyboards
  // Simple Text In (or Extended) event.
  //

  // Locate the on-screen keyboard (OSK) protocol
  Status = gBS->LocateProtocol (&gMsOSKProtocolGuid, NULL, (VOID **)&OSKProtocol);

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate OSK Protocol! Status = %r\n", __FUNCTION__, Status));
  } else {
    //
    // Configure the OSK size, position, and state (75% of screen width, bottom right
    // position, docked) - this is most compatible with the Windows 10 English-localized
    // Bitlocker PIN screen.  The keyboard is not displayed by default.
    //
    OSKProtocol->ShowKeyboard            (OSKProtocol, FALSE);
    OSKProtocol->SetKeyboardSize         (OSKProtocol, 75);
    OSKProtocol->SetKeyboardPosition     (OSKProtocol, BottomRight, Docked);
    OSKProtocol->ShowDockAndCloseButtons (OSKProtocol, TRUE);

    //
    // Configure the OSK icon position (used to launch on-screen keyboard).  The keyboard
    // icon is not displayed by default.
    //
    OSKProtocol->ShowKeyboardIcon        (OSKProtocol, FALSE);
    OSKProtocol->SetKeyboardIconPosition (OSKProtocol, BottomRight);

    //
    // Configure the OSK icon for auto-enable and self-refresh modes.
    //
    OSKProtocol->SetKeyboardMode         (OSKProtocol, (OSK_MODE_AUTOENABLEICON | OSK_MODE_SELF_REFRESH));
  }

  //
  // Disable the mouse pointer.  This allows a 3rd party application to render the pointer themselves
  // if they wish.  If the on-screen keyboard auto activates or if we launch our own application that
  // requires mouse pointer (ex: FrontPage), they will re-enable our mouse pointer.
  //

  // Locate the Simple Window Manager (SWM) protocol
  Status = gBS->LocateProtocol (&gMsSWMProtocolGuid, NULL, (VOID **)&SWMProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate SWM Protocol! Status = %r\n", Status));
  } else {
    // Disable the mouse pointer.
    SWMProtocol->EnableMousePointer (SWMProtocol, FALSE);
  }
}

/**
  Pre ready to boot callback to lock bds variables.

  @param  Event                 Event whose notification function is being invoked.
  @param  Context               The pointer to the notification function's context,
                                which is implementation-dependent.
**/
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
  return;
}

/**
  Post ready to boot callback to print memory map, and update FACS hardware signature.
  For booting the internal shell, set the video resolution to low.

  @param  Event                 Event whose notification function is being invoked.
  @param  Context               The pointer to the notification function's context,
                                which is implementation-dependent.
**/
STATIC
VOID
EFIAPI
PostReadyToBoot (
  IN EFI_EVENT Event,
  IN VOID     *Context)
{
  BOOLEAN         StartNetworkStack = FALSE;
  EFI_STATUS      Status            = EFI_SUCCESS;
  STATIC BOOLEAN  FirstPass         = TRUE;

  if (FirstPass) {
    FirstPass = FALSE;
    Status    = GetBootManagerSetting (DFCI_SETTING_ID__START_NETWORK, &StartNetworkStack);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Unable to get Start Network Setting! Status = %r\n", __FUNCTION__, Status));
    } else {
      if (StartNetworkStack) {
        // This will unblock the network stack.
        StartNetworking ();

        // ConnectAll - Convert to ConnetLess in phase 2 (Work Item 1544)
        EfiBootManagerConnectAll ();
      }
    }

    UpdateFacsHardwareSignature (DefaultFacsHwSigAlgorithm);
  }

  return;
}

/**
  Constructor   - This runs when BdsDxe is loaded, before BdsArch protocol is published

  @return         EFI_STATUS
**/
EFI_STATUS
EFIAPI
DeviceBootManagerConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Install Pre-ReadyToBoot callback to note when the variables need to be locked
  Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL, TPL_CALLBACK, PreReadyToBoot, NULL, &gEfiEventPreReadyToBootGuid, &mPreReadyToBootEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Register OnReadyToBoot Event! Status = %r\n", __FUNCTION__, Status));
  }

  Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL, TPL_CALLBACK, PostReadyToBoot, NULL, &gEfiEventPostReadyToBootGuid, &mPostReadyToBootEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Eegister OnReadyToBoot Event! Status = %r\n", Status));
  }

  // Constructor MUST return EFI_SUCCESS as a failure can result in an unusable system
  return EFI_SUCCESS;
}

/**
  OnDemandConInConnect
**/
EFI_DEVICE_PATH_PROTOCOL**
EFIAPI
DeviceBootManagerOnDemandConInConnect (VOID)
{
  return GetPlatformConnectOnConInList ();
}

/**
  Do the device specific action at start of BdsEntry (callback into BdsArch from DXE Dispatcher)
**/
VOID
EFIAPI
DeviceBootManagerBdsEntry (
  VOID
  )
{
  EfiEventGroupSignal (&gMsStartOfBdsNotifyGuid);

  //
  // Also signal the DFCI version of this event.
  //
  EfiEventGroupSignal (&gDfciStartOfBdsNotifyGuid);

  UpdateRebootReason ();
}

/**
  Do the device specific action before the console is connected.

  Such as:

      Initialize the platform boot order
      Supply Console information

  Returns value from GetPlatformPreffered Console, which will be the handle and device path
  of the device console
**/
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

/**
  Do the device specific action after the console is connected.
**/
EFI_DEVICE_PATH_PROTOCOL**
EFIAPI
DeviceBootManagerAfterConsole (VOID)
{
  EFI_STATUS       Status   = EFI_SUCCESS;
  EFI_BOOT_MODE    BootMode = 0;
  TPM_PP_PROTOCOL *TpmPp    = NULL;

  MsPreBootChecks ();

  PlatformPowerLevelCheck ();

  Status = DisplayBootGraphic (BG_SYSTEM_LOGO);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Display Boot Logo! Status = %r\n", __FUNCTION__, Status));
  }

  ConsoleMsgLibDisplaySystemInfoOnConsole ();

  BootMode = GetBootModeHob ();

  if (BootMode != BOOT_ON_FLASH_UPDATE) {
    Status = gBS->LocateProtocol (&gTpmPpProtocolGuid, NULL, (VOID **)&TpmPp);

    if (!EFI_ERROR (Status) && (TpmPp != NULL)) {
      TpmPp->PromptForConfirmation (TpmPp);
    }
  }

  return GetPlatformConnectList ();
}

/**
  ProcessBootCompletion
**/
VOID
EFIAPI
DeviceBootManagerProcessBootCompletion (IN EFI_BOOT_MANAGER_LOAD_OPTION *BootOption)
{
  EFI_STATUS  Status        = EFI_SUCCESS;
  EFI_STATUS  RestartStatus = EFI_SUCCESS;
  UINTN       BufferSize    = 0;
  BOOLEAN     MsBootNext    = FALSE;

  BufferSize = sizeof (MsBootNext);
  Status     = gRT->GetVariable (L"MsBootNext", &gMsBootMenuFormsetGuid, NULL, &BufferSize, &MsBootNext);
  if (!EFI_ERROR (Status)) {
    Status = gRT->SetVariable (L"MsBootNext", &gMsBootMenuFormsetGuid, EFI_VARIABLE_BOOTSERVICE_ACCESS, 0, NULL);
  }

  RestartStatus = BootOption->Status;
  if (OEM_PREVIOUS_SECURITY_VIOLATION == BootOption->Status) {
    RestartStatus = EFI_SECURITY_VIOLATION;
  }

  if (MsBootNext) {
    SetRebootReason (RestartStatus);
  }

  SetRebootReason (BootOption->Status);
  Status = SetGraphicsConsoleMode (GCM_NATIVE_RES);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Unabled to set Console Mode! Status = %r\n", __FUNCTION__, Status));
  }

  return;
}

/**
  Check for HardKeys during boot.  If the hard keys are pressed, builds
  a boot option for the specific hard key setting.

  @param BootOption   - Boot Option filled in based on which hard key is pressed

  @return EFI_STATUS  - EFI_NOT_FOUND - no hard key pressed, no BootOption
                        EFI_SUCCESS   - BootOption is valid
                        other error   - Unable to build BootOption
**/
EFI_STATUS
EFIAPI
DeviceBootManagerPriorityBoot (EFI_BOOT_MANAGER_LOAD_OPTION *BootOption)
{
  EFI_STATUS  Status        = EFI_SUCCESS;
  BOOLEAN     FrontPageBoot = MsBootPolicyLibIsSettingsBoot ();
  BOOLEAN     SlotSwitch    = MsBootPolicyLibSlotSwitch ();

  MsBootPolicyLibClearBootRequests ();

  //
  // There are four cases:
  //   1.  Nothing pressed.             return EFI_NOT_FOUND
  //   2.  FrontPageBoot                load FrontPage
  //   3.  SlotSwitch                   Load Slot Switch App
  //   4   Both indicators are present  Load NetworkUnlock
  //

  if (SlotSwitch) {
    // Alternate boot or Network Unlock option
    if (FrontPageBoot) {
      DEBUG ((EFI_D_INFO, "[Bds] both detected. NetworkUnlock\n"));
      Status = MsBootOptionsLibGetDefaultBootApp (BootOption, "NS");
    } else {
      DEBUG ((EFI_D_INFO, "[Bds] Slot Switch\n"));
      Status = MsBootOptionsLibSlotSwitchApp (BootOption, "VOL-");
    }
  } else if (FrontPageBoot) {
    // Front Page Boot Option
    DEBUG ((EFI_D_INFO, "[Bds] enter Front Page\n"));
    Status = MsBootOptionsLibGetBootManagerMenu (BootOption, "VOL+");
    SetRebootReason (OEM_REBOOT_TO_SETUP_KEY);
  } else {
    Status = EFI_NOT_FOUND;
  }

  return Status;
}

/**
  This is called from BDS right before going into front page
  when no bootable devices/options found
**/
VOID
EFIAPI
DeviceBootManagerUnableToBoot (VOID)
{
  EFI_STATUS Status;

  // Set Native Resolution
  Status = SetGraphicsConsoleMode (GCM_NATIVE_RES);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Unabled to set Console Mode! Status = %r\n", __FUNCTION__, Status));
  }

  // Display No Boot OS Logo
  Status = DisplayBootGraphic (BG_NO_BOOT_OS);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Display No Boot OS Logo! Status = %r\n", __FUNCTION__, Status));
  }

  // Wait 10s
  gBS->Stall(10000000);

  // Shutdown
  gRT->ResetSystem (EfiResetShutdown, EFI_SUCCESS, 0, NULL);

  // Do Cpu Dead Loop
  CpuDeadLoop ();
}
