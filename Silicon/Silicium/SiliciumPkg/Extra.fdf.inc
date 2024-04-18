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

  FILE FREEFORM = PCD(gSiliciumPkgTokenSpaceGuid.PcdLogoFile_Small) {
    SECTION RAW = SiliciumPkg/Include/Resources/BootLogo_Small.bmp
    SECTION UI = "Logo Small"
  }

  FILE FREEFORM = PCD(gSiliciumPkgTokenSpaceGuid.PcdLogoFile_Medium) {
    SECTION RAW = SiliciumPkg/Include/Resources/BootLogo_Medium.bmp
    SECTION UI = "Logo Medium"
  }

  FILE FREEFORM = PCD(gSiliciumPkgTokenSpaceGuid.PcdLogoFile_Big) {
    SECTION RAW = SiliciumPkg/Include/Resources/BootLogo_Big.bmp
    SECTION UI = "Logo Big"
  }

  FILE FREEFORM = PCD(gSiliciumPkgTokenSpaceGuid.PcdNoBootOSFile_Small) {
    SECTION RAW = SiliciumPkg/Include/Resources/NoBootOS_Small.bmp
    SECTION UI = "No OS Logo Small"
  }

  FILE FREEFORM = PCD(gSiliciumPkgTokenSpaceGuid.PcdNoBootOSFile_Medium) {
    SECTION RAW = SiliciumPkg/Include/Resources/NoBootOS_Medium.bmp
    SECTION UI = "No OS Logo Medium"
  }

  FILE FREEFORM = PCD(gSiliciumPkgTokenSpaceGuid.PcdNoBootOSFile_Big) {
    SECTION RAW = SiliciumPkg/Include/Resources/NoBootOS_Big.bmp
    SECTION UI = "No OS Logo Big"
  }

  FILE FREEFORM = PCD(gSiliciumPkgTokenSpaceGuid.PcdLowBatteryFile_Small) {
    SECTION RAW = SiliciumPkg/Include/Resources/LBAT_Small.bmp
    SECTION UI = "Low Battery Small"
  }

  FILE FREEFORM = PCD(gSiliciumPkgTokenSpaceGuid.PcdLowBatteryFile_Medium) {
    SECTION RAW = SiliciumPkg/Include/Resources/LBAT_Medium.bmp
    SECTION UI = "Low Battery Medium"
  }

  FILE FREEFORM = PCD(gSiliciumPkgTokenSpaceGuid.PcdLowBatteryFile_Big) {
    SECTION RAW = SiliciumPkg/Include/Resources/LBAT_Big.bmp
    SECTION UI = "Low Battery Big"
  }

  FILE FREEFORM = PCD(gSiliciumPkgTokenSpaceGuid.PcdThermalFile_Small) {
    SECTION RAW = SiliciumPkg/Include/Resources/THOT_Small.bmp
    SECTION UI = "Too Hot Small"
  }

  FILE FREEFORM = PCD(gSiliciumPkgTokenSpaceGuid.PcdThermalFile_Medium) {
    SECTION RAW = SiliciumPkg/Include/Resources/THOT_Medium.bmp
    SECTION UI = "Too Hot Medium"
  }

  FILE FREEFORM = PCD(gSiliciumPkgTokenSpaceGuid.PcdThermalFile_Big) {
    SECTION RAW = SiliciumPkg/Include/Resources/THOT_Big.bmp
    SECTION UI = "Too Hot Big"
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
