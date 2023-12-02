  INF MsGraphicsPkg/GopOverrideDxe/GopOverrideDxe.inf
  INF MsGraphicsPkg/MsUiTheme/Dxe/MsUiThemeProtocol.inf
  INF MsGraphicsPkg/RenderingEngineDxe/RenderingEngineDxe.inf
  INF MsGraphicsPkg/DisplayEngineDxe/DisplayEngineDxe.inf
  INF QcomPkg/Applications/BootMenu/BootMenu.inf
  INF QcomPkg/Applications/FrontPage/FrontPage.inf
  INF PcBdsPkg/MsBootPolicy/MsBootPolicy.inf
  INF MdeModulePkg/Universal/BootManagerPolicyDxe/BootManagerPolicyDxe.inf
  INF MsGraphicsPkg/OnScreenKeyboardDxe/OnScreenKeyboardDxe.inf
  INF MsGraphicsPkg/SimpleWindowManagerDxe/SimpleWindowManagerDxe.inf
  INF MsGraphicsPkg/MsEarlyGraphics/Dxe/MsEarlyGraphics.inf

  INF MsCorePkg/AcpiRGRT/AcpiRgrt.inf

  FILE FREEFORM = PCD(gOemPkgTokenSpaceGuid.PcdLogoFile) {
    SECTION RAW = OemPkg/FrontPage/Resources/BootLogo.bmp
    SECTION UI = "Logo"
  }

  FILE APPLICATION = PCD(gPcBdsPkgTokenSpaceGuid.PcdShellFile) {
    SECTION PE32 = $(OUTPUT_DIRECTORY)/$(TARGET)_$(TOOL_CHAIN_TAG)/$(ARCH)/Shell.efi
    SECTION UI = "Shell"
  }

  FILE FREEFORM = PCD(gQcomPkgTokenSpaceGuid.PcdNoBootOSFile) {
    SECTION RAW = QcomPkg/Include/Resources/NoBootOS.bmp
  }

  FILE FREEFORM = PCD(gOemPkgTokenSpaceGuid.PcdFrontPageLogoFile) {
    SECTION RAW = QcomPkg/Applications/FrontPage/Resources/FrontpageLogo.bmp
  }

  FILE FREEFORM = PCD(gOemPkgTokenSpaceGuid.PcdLowBatteryFile) {
    SECTION RAW = QcomPkg/Include/Resources/LBAT.bmp
  }

  FILE FREEFORM = PCD(gOemPkgTokenSpaceGuid.PcdThermalFile) {
    SECTION RAW = QcomPkg/Include/Resources/THOT.bmp
  }

  FILE FREEFORM = PCD(gOemPkgTokenSpaceGuid.PcdVolumeUpIndicatorFile) {
    SECTION RAW = QcomPkg/Applications/FrontPage/Resources/VolumeUp.bmp
  }

  FILE FREEFORM = PCD(gOemPkgTokenSpaceGuid.PcdFirmwareSettingsIndicatorFile) {
    SECTION RAW = QcomPkg/Applications/FrontPage/Resources/FirmwareSettings.bmp
  }

  FILE FREEFORM = PCD(gOemPkgTokenSpaceGuid.PcdBootFailIndicatorFile) {
    SECTION RAW = QcomPkg/Applications/FrontPage/Resources/NoBoot.bmp
  }
