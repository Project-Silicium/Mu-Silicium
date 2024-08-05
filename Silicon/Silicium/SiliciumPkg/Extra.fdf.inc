  INF MsGraphicsPkg/GopOverrideDxe/GopOverrideDxe.inf
  INF DfciPkg/SettingsManager/SettingsManagerDxe.inf
  INF MsGraphicsPkg/MsUiTheme/Dxe/MsUiThemeProtocol.inf
  INF MsGraphicsPkg/RenderingEngineDxe/RenderingEngineDxe.inf
  INF MsGraphicsPkg/DisplayEngineDxe/DisplayEngineDxe.inf
  INF OemPkg/BootMenu/BootMenu.inf
  INF SiliciumPkg/Applications/FrontPage/FrontPage.inf
  INF PcBdsPkg/MsBootPolicy/MsBootPolicy.inf
  INF MsGraphicsPkg/OnScreenKeyboardDxe/OnScreenKeyboardDxe.inf
  INF MsGraphicsPkg/SimpleWindowManagerDxe/SimpleWindowManagerDxe.inf

  INF MdeModulePkg/Universal/Acpi/BootGraphicsResourceTableDxe/BootGraphicsResourceTableDxe.inf

  INF SiliciumPkg/Drivers/MsButtonServiceDxe/MsButtonServiceDxe.inf

  FILE FREEFORM = PCD(gOemPkgTokenSpaceGuid.PcdLogoFile) {
    SECTION RAW = SiliciumPkg/Include/Resources/BootLogo.bmp
    SECTION UI = "Boot Logo"
  }

  FILE FREEFORM = PCD(gOemPkgTokenSpaceGuid.PcdLowBatteryFile) {
    SECTION RAW = OemPkg/FrontPage/Resources/LBAT.bmp
    SECTION UI = "Low Battery"
  }

  FILE FREEFORM = PCD(gOemPkgTokenSpaceGuid.PcdThermalFile) {
    SECTION RAW = OemPkg/FrontPage/Resources/THOT.bmp
    SECTION UI = "Too Hot"
  }

  FILE FREEFORM = PCD(gSiliciumPkgTokenSpaceGuid.PcdNoBootOSFile) {
    SECTION RAW = SiliciumPkg/Include/Resources/NoBootOS.bmp
    SECTION UI = "No Operating System"
  }

  FILE FREEFORM = PCD(gSiliciumPkgTokenSpaceGuid.PcdMsdWarningFile) {
    SECTION RAW = SiliciumPkg/Include/Resources/MassStorage_Warning.bmp
    SECTION UI = "Mass Storage Warning"
  }

  FILE FREEFORM = PCD(gSiliciumPkgTokenSpaceGuid.PcdMsdConnectedFile) {
    SECTION RAW = SiliciumPkg/Include/Resources/MassStorage_Connected.bmp
    SECTION UI = "Mass Storage Connected"
  }

  FILE FREEFORM = PCD(gSiliciumPkgTokenSpaceGuid.PcdMsdDisconnectedFile) {
    SECTION RAW = SiliciumPkg/Include/Resources/MassStorage_Disconnected.bmp
    SECTION UI = "Mass Storage Disconnected"
  }

  FILE FREEFORM = PCD(gSiliciumPkgTokenSpaceGuid.PcdMsdErrorFile) {
    SECTION RAW = SiliciumPkg/Include/Resources/MassStorage_Error.bmp
    SECTION UI = "Mass Storage Error"
  }

  FILE FREEFORM = PCD(gOemPkgTokenSpaceGuid.PcdFrontPageLogoFile) {
    SECTION RAW = SiliciumPkg/Applications/FrontPage/Resources/FrontpageLogo.bmp
    SECTION UI = "FrontPage Logo"
  }

  FILE FREEFORM = PCD(gOemPkgTokenSpaceGuid.PcdVolumeUpIndicatorFile) {
    SECTION RAW = SiliciumPkg/Applications/FrontPage/Resources/VolumeUp.bmp
    SECTION UI = "VolumeUpIndicator"
  }

  FILE FREEFORM = PCD(gOemPkgTokenSpaceGuid.PcdFirmwareSettingsIndicatorFile) {
    SECTION RAW = SiliciumPkg/Applications/FrontPage/Resources/FirmwareSettings.bmp
    SECTION UI = "FirmwareSettingsIndicator"
  }

  FILE FREEFORM = PCD(gOemPkgTokenSpaceGuid.PcdBootFailIndicatorFile) {
    SECTION RAW = SiliciumPkg/Applications/FrontPage/Resources/NoBoot.bmp
    SECTION UI = "BootFailIndicator"
  }
