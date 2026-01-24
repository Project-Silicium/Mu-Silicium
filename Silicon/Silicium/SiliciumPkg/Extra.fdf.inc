  INF MdeModulePkg/Universal/Acpi/BootGraphicsResourceTableDxe/BootGraphicsResourceTableDxe.inf
  INF MdeModulePkg/Universal/SetupBrowserDxe/SetupBrowserDxe.inf
  INF PcBdsPkg/MsBootPolicy/MsBootPolicy.inf

  INF SiliciumPkg/Applications/XenonGUI/XenonGUI.inf
  INF SiliciumPkg/UFP/ufpdevicefw.inf

  INF SiliciumPkg/Drivers/MsButtonServiceDxe/MsButtonServiceDxe.inf

!if $(DISABLE_SECURITY) == 0
  FILE FREEFORM = PCD(gSiliciumPkgTokenSpaceGuid.PcdCustomSiPolicyGuid) {
    SECTION RAW = SiliciumPkg/Include/Resources/SecureBoot/SiPolicy.p7b
    SECTION UI = "SiPolicy.p7b"
  }
!endif

  FILE FREEFORM = PCD(gOemPkgTokenSpaceGuid.PcdLogoFile) {
    SECTION RAW = SiliciumPkg/Include/Resources/BootLogo.bmp
    SECTION UI = "Boot Logo"
  }

  FILE FREEFORM = PCD(gSiliciumPkgTokenSpaceGuid.PcdNoBootOSFile) {
    SECTION RAW = SiliciumPkg/Include/Resources/NoBootOS.bmp
    SECTION UI = "No Operating System"
  }

  FILE FREEFORM = PCD(gSiliciumPkgTokenSpaceGuid.PcdMassStorageFile) {
    SECTION RAW = SiliciumPkg/Include/Resources/MassStorage.bmp
    SECTION UI = "Mass Storage"
  }
