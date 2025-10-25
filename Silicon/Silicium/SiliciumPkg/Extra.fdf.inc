  INF MdeModulePkg/Universal/Acpi/BootGraphicsResourceTableDxe/BootGraphicsResourceTableDxe.inf
  INF MdeModulePkg/Universal/SetupBrowserDxe/SetupBrowserDxe.inf
  INF PcBdsPkg/MsBootPolicy/MsBootPolicy.inf

  INF SiliciumPkg/Applications/XenonGUI/XenonGUI.inf
  INF SiliciumPkg/UFP/ufpdevicefw.inf

  INF SiliciumPkg/Drivers/MsButtonServiceDxe/MsButtonServiceDxe.inf

  FILE FREEFORM = PCD(gOemPkgTokenSpaceGuid.PcdLogoFile) {
    SECTION RAW = SiliciumPkg/Include/Resources/BootLogo.bmp
    SECTION UI = "Boot Logo"
  }

  FILE FREEFORM = PCD(gSiliciumPkgTokenSpaceGuid.PcdNoBootOSFile) {
    SECTION RAW = SiliciumPkg/Include/Resources/NoBootOS.bmp
    SECTION UI = "No Operating System"
  }

  FILE FREEFORM = PCD(gSiliciumPkgTokenSpaceGuid.PcdMsdDefaultFile) {
    SECTION RAW = SiliciumPkg/Include/Resources/MassStorage_Default.bmp
    SECTION UI = "Mass Storage Default"
  }

  FILE FREEFORM = PCD(gSiliciumPkgTokenSpaceGuid.PcdMsdConnectedFile) {
    SECTION RAW = SiliciumPkg/Include/Resources/MassStorage_Connected.bmp
    SECTION UI = "Mass Storage Connected"
  }

  FILE FREEFORM = PCD(gSiliciumPkgTokenSpaceGuid.PcdMsdDisconnectedFile) {
    SECTION RAW = SiliciumPkg/Include/Resources/MassStorage_Disconnected.bmp
    SECTION UI = "Mass Storage Disconnected"
  }
