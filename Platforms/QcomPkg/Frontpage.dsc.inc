[LibraryClasses.common]
  #
  # Libraries to support Shell
  #
  ShellLib|ShellPkg/Library/UefiShellLib/UefiShellLib.inf
  ShellCommandLib|ShellPkg/Library/UefiShellCommandLib/UefiShellCommandLib.inf
  ShellCEntryLib|ShellPkg/Library/UefiShellCEntryLib/UefiShellCEntryLib.inf
  HandleParsingLib|ShellPkg/Library/UefiHandleParsingLib/UefiHandleParsingLib.inf
  BcfgCommandLib|ShellPkg/Library/UefiShellBcfgCommandLib/UefiShellBcfgCommandLib.inf
  NetLib|NetworkPkg/Library/DxeNetLib/DxeNetLib.inf

  #
  # Contains intrinsics for crypto operations
  #
  IntrinsicLib|CryptoPkg/Library/IntrinsicLib/IntrinsicLib.inf

  #
  # contains NULL routines to support UI operations associated with DFCI.
  #
  DfciUiSupportLib|OemPkg/Library/DfciUiSupportLib/DfciUiSupportLib.inf

  #
  # contains crypto support functions for the DFCI recovery feature.
  #
  DfciRecoveryLib|DfciPkg/Library/DfciRecoveryLib/DfciRecoveryLib.inf

  #
  # Contains openSSL library used by BaseCryptoLib
  #
  OpensslLib|CryptoPkg/Library/OpensslLib/OpensslLib.inf

  #
  # Functions that should contain base logic for querying, setting, and verifying user passwords.
  #
  PasswordStoreLib|OemPkg/Library/PasswordStoreLib/PasswordStoreLib.inf
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/BaseCryptLib.inf

  #
  # Supports DFCI Groups.
  #
  DfciGroupLib|OemPkg/Library/DfciGroupLib/DfciGroups.inf
  XmlTreeQueryLib|XmlSupportPkg/Library/XmlTreeQueryLib/XmlTreeQueryLib.inf
  XmlTreeLib|XmlSupportPkg/Library/XmlTreeLib/XmlTreeLib.inf

  #
  # Backwards compatibility with DFCI V1 functions.
  #
  DfciV1SupportLib|DfciPkg/Library/DfciV1SupportLibNull/DfciV1SupportLibNull.inf
  FltUsedLib|MdePkg/Library/FltUsedLib/FltUsedLib.inf

  #
  # Library instance that understands the MsXml Settings Schema and providers helper functions
  #
  DfciXmlSettingSchemaSupportLib|DfciPkg/Library/DfciXmlSettingSchemaSupportLib/DfciXmlSettingSchemaSupportLib.inf

  #
  # Library instance that understands the MsXml Permission Schema and providers helper functions
  #
  DfciXmlPermissionSchemaSupportLib|DfciPkg/Library/DfciXmlPermissionSchemaSupportLib/DfciXmlPermissionSchemaSupportLib.inf

  #
  # Library instance that understands the MsXml Device Id Schema and providers helper functions
  #
  DfciXmlDeviceIdSchemaSupportLib|DfciPkg/Library/DfciXmlDeviceIdSchemaSupportLib/DfciXmlDeviceIdSchemaSupportLib.inf

  #
  # Library instance that understands the MsXml Device Id Schema and providers helper functions
  #
  DfciXmlIdentitySchemaSupportLib|DfciPkg/Library/DfciXmlIdentitySchemaSupportLib/DfciXmlIdentitySchemaSupportLib.inf

  #
  # Library instance that understands Zero Touch
  #
  ZeroTouchSettingsLib|ZeroTouchPkg/Library/ZeroTouchSettings/ZeroTouchSettings.inf
  SafeIntLib|MdePkg/Library/BaseSafeIntLib/BaseSafeIntLib.inf
  BmpSupportLib|MdeModulePkg/Library/BaseBmpSupportLib/BaseBmpSupportLib.inf

  #
  # This library contains access functions for dealing with the copying of an MS_UI_THEME
  #
  MsUiThemeCopyLib|MsGraphicsPkg/Library/MsUiThemeCopyLib/MsUiThemeCopyLib.inf

  #
  # Library to provide platform version information
  #
  MuUefiVersionLib|OemPkg/Library/MuUefiVersionLib/MuUefiVersionLib.inf

  #
  # This is the Thermal Services Library that encapsulates the platform specific thermal information.
  #
  ThermalServicesLib|PcBdsPkg/Library/ThermalServicesLibNull/ThermalServicesLibNull.inf

  #
  # Empty Platform Library of power check.
  #
  MsPlatformPowerCheckLib|PcBdsPkg/Library/MsPlatformPowerCheckLibNull/MsPlatformPowerCheckLibNull.inf

  #
  # Library to help set the console to known points (low res, native res)
  #
  GraphicsConsoleHelperLib|PcBdsPkg/Library/GraphicsConsoleHelperLib/GraphicsConsoleHelper.inf
  DeviceStateLib|MdeModulePkg/Library/DeviceStateLib/DeviceStateLib.inf

  #
  # Device specific actions in support of PlatformBootManagerLib.
  # Sort of a wrapper for MsPlatformDevicesLib, which needs to be written for each platform.
  #
  DeviceBootManagerLib|QcomPkg/Library/DeviceBootManagerLib/DeviceBootManagerLib.inf

  #
  # LIbrary to display the On screen notifications for the platform
  #
  UiRectangleLib|MsGraphicsPkg/Library/BaseUiRectangleLib/BaseUiRectangleLib.inf

  #
  # display the device state notifications for the platform using color bars
  #
  DisplayDeviceStateLib|MsGraphicsPkg/Library/ColorBarDisplayDeviceStateLib/ColorBarDisplayDeviceStateLib.inf

  #
  # interfaces with alternate boot variable
  #
  MsAltBootLib|OemPkg/Library/MsAltBootLib/MsAltBootLib.inf

  #
  # Library that is attached BdsDxe to implement Microsoft extensions to UefiBootManagerLib.
  #
  MsBootOptionsLib|QcomPkg/Library/MsBootOptionsLib/MsBootOptionsLib.inf

  #
  # A shared place for all colors used in this package
  #
  MsColorTableLib|MsGraphicsPkg/Library/MsColorTableLib/MsColorTableLib.inf

  #
  # Library that is attached to drivers that require networking.
  #
  MsNetworkDependencyLib|PcBdsPkg/Library/MsNetworkDependencyLib/MsNetworkDependencyLib.inf

  #
  # Library to provide interface on Reboot Reason non volatile varialbles
  #
  MsNVBootReasonLib|OemPkg/Library/MsNVBootReasonLib/MsNVBootReasonLib.inf

  #
  # An architecture agnostic math library providing reasonable approximations for various functions in software
  #
  MathLib|MsCorePkg/Library/MathLib/MathLib.inf

  #
  # This library is for showing the main system boot graphics.
  #
  BootGraphicsLib|QcomPkg/Library/BootGraphicsLib/BootGraphicsLib.inf

  #
  # Library to support Device disable Settings  (get/set)
  #
  MsBootManagerSettingsLib|OemPkg/Library/MsBootManagerSettingsDxeLib/MsBootManagerSettingsDxeLib.inf

  #
  # Library to provide Dialog Boxes in a Simple Window Manager environment.
  #
  SwmDialogsLib|MsGraphicsPkg/Library/SwmDialogsLib/SwmDialogs.inf

  #
  # BootGraphicsProviderLib that uses PCDs and raw files in the firmware volumes
  #
  BootGraphicsProviderLib|QcomPkg/Library/BootGraphicsProviderLib/BootGraphicsProviderLib.inf

  #
  # Library that platform implemented to determine boot type
  #
  MsBootPolicyLib|QcomPkg/Library/MsBootPolicyLib/MsBootPolicyLib.inf
  BmpSupportLib|MdeModulePkg/Library/BaseBmpSupportLib/BaseBmpSupportLib.inf

  #
  # Supplies the theme for this platform to the UEFI settings UI
  #
  MsUiThemeLib|MsGraphicsPkg/Library/MsUiThemeLib/Dxe/MsUiThemeLib.inf
  SafeIntLib|MdePkg/Library/BaseSafeIntLib/BaseSafeIntLib.inf

  #
  # Library for determining if Manufacturing mode should be enabled on this boot. 
  # This libray is what everyone should use to determine which mode we are in.
  # It can be called from PEI or DXE.
  #
  UIToolKitLib|MsGraphicsPkg/Library/SimpleUIToolKit/SimpleUIToolKit.inf
  ResetUtilityLib|MdeModulePkg/Library/ResetUtilityLib/ResetUtilityLib.inf
  BaseBinSecurityLib|MdePkg/Library/BaseBinSecurityLibNull/BaseBinSecurityLibNull.inf
  SecurityLockAuditLib|MdeModulePkg/Library/SecurityLockAuditDebugMessageLib/SecurityLockAuditDebugMessageLib.inf
  MsWheaEarlyStorageLib|QcomPkg/Library/MsWheaEarlyStorageLib/MsWheaEarlyStorageLib.inf
  CheckHwErrRecHeaderLib|MsWheaPkg/Library/CheckHwErrRecHeaderLib/CheckHwErrRecHeaderLib.inf
  MuTelemetryHelperLib|MsWheaPkg/Library/MuTelemetryHelperLib/MuTelemetryHelperLib.inf
  ParserRegistryLib|MsWheaPkg/Library/ParserRegistryLib/ParserRegistryLib.inf
  GenericSectionParserLib|MsWheaPkg/Library/GenericSectionParserLib/GenericSectionParserLib.inf
  Hash2CryptoLib|SecurityPkg/Library/DxeHash2CryptoLib/DxeHash2CryptoLib.inf
  MsPlatformEarlyGraphicsLib|QcomPkg/Library/MsPlatformEarlyGraphicsLib/MsEarlyGraphicsLib.inf

[PcdsFixedAtBuild.common]
  # a PCD that controls the enumeration and connection of ConIn's. When true, ConIn is only connected once a console input is requests 
  gEfiMdeModulePkgTokenSpaceGuid.PcdConInConnectOnDemand|TRUE

  # Enable SHELL to build instead of just taking the binary
  gEfiMdePkgTokenSpaceGuid.PcdUefiLibMaxPrintBufferSize|16000
  gEfiShellPkgTokenSpaceGuid.PcdShellProfileMask|0x1f    # All profiles

  gMsGraphicsPkgTokenSpaceGuid.PcdUiThemeInDxe|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdBootManagerInBootOrder|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdPlatformRecoverySupport|FALSE
  # The GUID of Frontpage.inf from MU_OEM_SAMPLE
  # 4042708A-0F2D-4823-AC60-0D77B3111889
  gEfiMdeModulePkgTokenSpaceGuid.PcdBootManagerMenuFile|{ 0x8A, 0x70, 0x42, 0x40, 0x2D, 0x0F, 0x23, 0x48, 0xAC, 0x60, 0x0D, 0x77, 0xB3, 0x11, 0x18, 0x89 }

  gOemPkgTokenSpaceGuid.PcdPostBackgroundColor|0x0

[Components.AARCH64]
  #
  # Spoofs button press to automatically boot to FrontPage.
  #
  OemPkg/FrontpageButtonsVolumeUp/FrontpageButtonsVolumeUp.inf

  #
  # Application that presents and manages FrontPage.
  #
  OemPkg/FrontPage/FrontPage.inf

  #
  # Application that presents & manages the Boot Menu Setup on Front Page.
  #
  OemPkg/BootMenu/BootMenu.inf
  PcBdsPkg/MsBootPolicy/MsBootPolicy.inf
  MdeModulePkg/Universal/BootManagerPolicyDxe/BootManagerPolicyDxe.inf

  #
  # Library to support Remote/IT/Admin Settings such as DFCI
  #
  MdeModulePkg/Universal/RegularExpressionDxe/RegularExpressionDxe.inf

  #
  # Main component of DFCI. Manages and enforces all DFCI settings.
  #
  DfciPkg/SettingsManager/SettingsManagerDxe.inf {
        #Platform should add all it settings libs here
  <LibraryClasses>
        NULL|ZeroTouchPkg/Library/ZeroTouchSettings/ZeroTouchSettings.inf
        NULL|DfciPkg/Library/DfciPasswordProvider/DfciPasswordProvider.inf
        NULL|DfciPkg/Library/DfciSettingsLib/DfciSettingsLib.inf
        NULL|DfciPkg/Library/DfciVirtualizationSettings/DfciVirtualizationSettings.inf
        NULL|DfciPkg/Library/DfciWpbtSettingLib/DfciWpbtSetting.inf
        NULL|DfciPkg/Library/DfciAssetTagSettingLib/DfciAssetTagSetting.inf
        DfciSettingPermissionLib|DfciPkg/Library/DfciSettingPermissionLib/DfciSettingPermissionLib.inf
        NULL|OemPkg/Library/MsBootManagerSettingsDxeLib/MsBootManagerSettingsDxeLib.inf
  <PcdsFeatureFlag>
     gDfciPkgTokenSpaceGuid.PcdSettingsManagerInstallProvider|TRUE
  }

  MsCorePkg/MuCryptoDxe/MuCryptoDxe.inf

  #
  # AuthManager provides authentication for DFCI.
  #
  DfciPkg/IdentityAndAuthManager/IdentityAndAuthManagerDxe.inf
  DfciPkg/AuthManagerNull/AuthManagerNull.inf

  #
  # Processes ingoing and outgoing DFCI settings requests.
  #
  DfciPkg/DfciManager/DfciManager.inf

  #
  # Manages windows and fonts to be drawn by the RenderingEngine.
  #
  MsGraphicsPkg/SimpleWindowManagerDxe/SimpleWindowManagerDxe.inf

  #
  # Produces EfiGraphicsOutputProtocol to draw graphics to the screen.
  #
  MsGraphicsPkg/RenderingEngineDxe/RenderingEngineDxe.inf

  #
  # Finds and takes ownership of gEfiGraphicsOutputProtocolGuid so RenderingEngine can use it.
  #
  MsGraphicsPkg/GopOverrideDxe/GopOverrideDxe.inf

  #
  # Driver for On Screen Keyboard.
  #
  MsGraphicsPkg/OnScreenKeyboardDxe/OnScreenKeyboardDxe.inf

  #
  # Installs protocol to share the UI theme. If PcdUiThemeInDxe, this will involve calling the PlatformThemeLib directly.
  # Otherwise, the theme will have been generated in PEI and it will be located on a HOB.
  #
  MsGraphicsPkg/MsUiTheme/Dxe/MsUiThemeProtocol.inf

  #
  # Produces FORM DISPLAY ENGINE protocol. Handles input, displays strings.
  #
  MsGraphicsPkg/DisplayEngineDxe/DisplayEngineDxe.inf
  MsGraphicsPkg/MsEarlyGraphics/Dxe/MsEarlyGraphics.inf

  MsWheaPkg/HwErrBert/HwErrBert.inf
  MsWheaPkg/MsWheaReport/Dxe/MsWheaReportDxe.inf

  # Hardware Health (Menu) application
  MsWheaPkg/HwhMenu/HwhMenu.inf

  MsCorePkg/AcpiRGRT/AcpiRgrt.inf

  DfciPkg/Application/DfciMenu/DfciMenu.inf

#######################################
#  Shell
#######################################

  ShellPkg/Application/Shell/Shell.inf {
    <LibraryClasses>
      NULL|ShellPkg/Library/UefiShellLevel2CommandsLib/UefiShellLevel2CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellLevel1CommandsLib/UefiShellLevel1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellLevel3CommandsLib/UefiShellLevel3CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellDriver1CommandsLib/UefiShellDriver1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellInstall1CommandsLib/UefiShellInstall1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellDebug1CommandsLib/UefiShellDebug1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellNetwork1CommandsLib/UefiShellNetwork1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellNetwork2CommandsLib/UefiShellNetwork2CommandsLib.inf
    <PcdsFixedAtBuild>
     gEfiShellPkgTokenSpaceGuid.PcdShellLibAutoInitialize|FALSE
  }
