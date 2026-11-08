#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DeviceBootManagerLib.h>
#include <Library/RfsProtectionLib.h>
#include <Protocol/EFIScm.h>

STATIC
VOID
DummyNotify (
  IN EFI_EVENT Event,
  IN VOID     *Context) 
{
  return;
}

EFI_HANDLE
EFIAPI
DeviceBootManagerBeforeConsole (
  OUT EFI_DEVICE_PATH_PROTOCOL  **DevicePath,
  OUT BDS_CONSOLE_CONNECT_ENTRY **PlatformConsoles)
{
  return NULL;
}

EFI_DEVICE_PATH_PROTOCOL**
EFIAPI
DeviceBootManagerAfterConsole ()
{
  EFI_STATUS Status;
  EFI_EVENT  InitEvent;

  // Create SD Card Init Event
  Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL, TPL_CALLBACK, DummyNotify, NULL, &gSdCardInitGuid, &InitEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Create SD Card Init Event! Status = %r\n", __FUNCTION__, Status));
    return NULL;
  }

  // Init SD Card
  gBS->SignalEvent (InitEvent);
  gBS->CloseEvent  (InitEvent);

  return NULL;
}

EFI_DEVICE_PATH_PROTOCOL**
EFIAPI
DeviceBootManagerOnDemandConInConnect ()
{
  EFI_STATUS Status;
  EFI_EVENT  InitEvent;

  // Create USB Controller Start Event
  Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL, TPL_CALLBACK, DummyNotify, NULL, &gUsbControllerInitGuid, &InitEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Create USB Port Controller Start Event! Status = %r\n", __FUNCTION__, Status));
    return NULL;
  }

  // Start USB Controller
  gBS->SignalEvent (InitEvent);
  gBS->CloseEvent  (InitEvent);

  return NULL;
}

VOID
EFIAPI
DeviceBootManagerProcessBootCompletion (OUT EFI_BOOT_MANAGER_LOAD_OPTION *BootOption)
{
  return;
}

EFI_STATUS
EFIAPI
DeviceBootManagerPriorityBoot (IN OUT EFI_BOOT_MANAGER_LOAD_OPTION *BootOption)
{
  return EFI_SUCCESS;
}

VOID
EFIAPI
DeviceBootManagerUnableToBoot ()
{
  return;
}

STATIC
VOID
ExitBootServicesCallback (
  IN EFI_EVENT Event,
  IN VOID     *Context)
{
  EFI_STATUS Status;
  QCOM_SCM_PROTOCOL *ScmProtocol;

  // Locate SCM Protocol
  Status = gBS->LocateProtocol (&gQcomScmProtocolGuid, NULL, (VOID *)&ScmProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate SCM Protocol! Status = %r\n", __FUNCTION__, Status));
    return;
  }

  // Call Exit Boot Services Callback
  Status = ScmProtocol->ScmExitBootServicesHandler(ScmProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Call Exit Boot Services Callback! Status = %r\n", __FUNCTION__, Status));
    return;
  }

  return;
}

STATIC
VOID
EFIAPI
RegisterEBSCallback ()
{
  EFI_STATUS Status;
  EFI_EVENT  EBSEvent;

  // Create Exit Boot Services Event
  Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL, TPL_CALLBACK, ExitBootServicesCallback, NULL, &gEfiEventExitBootServicesGuid, &EBSEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Create ExitBootServices Event! Status = %r\n", __FUNCTION__, Status));
  }

  return;
}

VOID
EFIAPI
DeviceBootManagerBdsEntry ()
{
  // Protect Shared RFS Area
  ProtectRfsSharedArea ();

  // Register EBS Callback
  RegisterEBSCallback ();
}
