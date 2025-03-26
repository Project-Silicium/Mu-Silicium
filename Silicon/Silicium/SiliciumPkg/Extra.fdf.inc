  INF MdeModulePkg/Universal/Acpi/BootGraphicsResourceTableDxe/BootGraphicsResourceTableDxe.inf
  INF MdeModulePkg/Universal/SetupBrowserDxe/SetupBrowserDxe.inf
  INF PcBdsPkg/MsBootPolicy/MsBootPolicy.inf

  INF SiliciumPkg/Applications/XenonGUI/XenonGUI.inf

  INF SiliciumPkg/Drivers/MsButtonServiceDxe/MsButtonServiceDxe.inf

  FILE FREEFORM = PCD(gOemPkgTokenSpaceGuid.PcdLogoFile) {
    SECTION RAW = SiliciumPkg/Include/Resources/BootLogo.bmp
    SECTION UI = "Boot Logo"
  }

  FILE FREEFORM = PCD(gSiliciumPkgTokenSpaceGuid.PcdNoBootOSFile) {
    SECTION RAW = SiliciumPkg/Include/Resources/NoBootOS.bmp
    SECTION UI = "No Operating System"
  }

  FILE FREEFORM = PCD(gSiliciumPkgTokenSpaceGuid.PcdMsdSelectLunFile) {
    SECTION RAW = SiliciumPkg/Include/Resources/MassStorage_Select_Lun.bmp
    SECTION UI = "Mass Storage Select LUN"
  }

  FILE FREEFORM = PCD(gSiliciumPkgTokenSpaceGuid.PcdMsdConnectedFile) {
    SECTION RAW = SiliciumPkg/Include/Resources/MassStorage_Connected.bmp
    SECTION UI = "Mass Storage Connected"
  }

  FILE FREEFORM = PCD(gSiliciumPkgTokenSpaceGuid.PcdMsdDisconnectedFile) {
    SECTION RAW = SiliciumPkg/Include/Resources/MassStorage_Disconnected.bmp
    SECTION UI = "Mass Storage Disconnected"
  }

  FILE FREEFORM = PCD(gSiliciumPkgTokenSpaceGuid.PcdMsdUnknownStateFile) {
    SECTION RAW = SiliciumPkg/Include/Resources/MassStorage_Unknown_State.bmp
    SECTION UI = "Mass Storage Unknown State"
  }
