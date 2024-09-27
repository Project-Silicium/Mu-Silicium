[LibraryClasses]
  #
  # Functions that should contain base logic for querying, setting, and verifying user passwords.
  #
  PasswordStoreLib|OemPkg/Library/PasswordStoreLib/PasswordStoreLib.inf

  #
  # Supports DFCI Groups.
  #
  XmlTreeQueryLib|XmlSupportPkg/Library/XmlTreeQueryLib/XmlTreeQueryLib.inf
  XmlTreeLib|XmlSupportPkg/Library/XmlTreeLib/XmlTreeLib.inf

  #
  # This library is for showing the main system boot graphics.
  #
  BootGraphicsLib|MsGraphicsPkg/Library/BootGraphicsLib/BootGraphicsLib.inf

  #
  # BootGraphicsProviderLib that uses PCDs and raw files in the firmware volumes
  #
  BootGraphicsProviderLib|SiliciumPkg/Library/BootGraphicsProviderLib/BootGraphicsProviderLib.inf

  #
  # This library contains access functions for dealing with the copying of an MS_UI_THEME
  #
  MsUiThemeCopyLib|MsGraphicsPkg/Library/MsUiThemeCopyLib/MsUiThemeCopyLib.inf

  #
  # This is the Thermal Services Library that encapsulates the platform specific thermal information.
  #
  ThermalServicesLib|PcBdsPkg/Library/ThermalServicesLibNull/ThermalServicesLibNull.inf

  #
  # Null Power Services Library class to support Platforms that dont have battery
  #
  PowerServicesLib|PcBdsPkg/Library/PowerServicesLibNull/PowerServicesLibNull.inf

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
  DeviceBootManagerLib|SiliciumPkg/Library/DeviceBootManagerLib/DeviceBootManagerLib.inf
  MsPlatformDevicesLib|SiliciumPkg/Library/MsPlatformDevicesLib/MsPlatformDevicesLib.inf

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
  MsBootOptionsLib|SiliciumPkg/Library/MsBootOptionsLib/MsBootOptionsLib.inf

  #
  # A shared place for all colors used in this package
  #
  MsColorTableLib|SiliciumPkg/Library/MsColorTableLib/MsColorTableLib.inf

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
  # Library to support Device disable Settings  (get/set)
  #
  MsBootManagerSettingsLib|OemPkg/Library/MsBootManagerSettingsDxeLib/MsBootManagerSettingsDxeLib.inf

  #
  # Library to provide Dialog Boxes in a Simple Window Manager environment.
  #
  SwmDialogsLib|MsGraphicsPkg/Library/SwmDialogsLib/SwmDialogs.inf

  #
  # Library that platform implemented to determine boot type
  #
  MsBootPolicyLib|SiliciumPkg/Library/MsBootPolicyLib/MsBootPolicyLib.inf

  #
  # Supplies the theme for this platform to the UEFI settings UI
  #
  MsUiThemeLib|MsGraphicsPkg/Library/MsUiThemeLib/Dxe/MsUiThemeLib.inf
  UIToolKitLib|MsGraphicsPkg/Library/SimpleUIToolKit/SimpleUIToolKit.inf

  #
  # DFCI / XML / JSON Libraries
  #
  DfciUiSupportLib|OemPkg/Library/DfciUiSupportLib/DfciUiSupportLib.inf # Supports DFCI Groups.
  DfciV1SupportLib|DfciPkg/Library/DfciV1SupportLibNull/DfciV1SupportLibNull.inf # Backwards compatibility with DFCI V1 functions.
  DfciDeviceIdSupportLib|OemPkg/Library/DfciDeviceIdSupportLib/DfciDeviceIdSupportLib.inf
  DfciGroupLib|OemPkg/Library/DfciGroupLib/DfciGroups.inf
  DfciRecoveryLib|DfciPkg/Library/DfciRecoveryLib/DfciRecoveryLib.inf

  #
  # Zero Touch is part of DFCI
  #
  ZeroTouchSettingsLib|ZeroTouchPkg/Library/ZeroTouchSettings/ZeroTouchSettings.inf

  #
  # Libraries that understands the MsXml Settings Schema and providers helper functions
  #
  DfciXmlIdentitySchemaSupportLib|DfciPkg/Library/DfciXmlIdentitySchemaSupportLib/DfciXmlIdentitySchemaSupportLib.inf
  DfciXmlDeviceIdSchemaSupportLib|DfciPkg/Library/DfciXmlDeviceIdSchemaSupportLib/DfciXmlDeviceIdSchemaSupportLib.inf
  DfciXmlSettingSchemaSupportLib|DfciPkg/Library/DfciXmlSettingSchemaSupportLib/DfciXmlSettingSchemaSupportLib.inf
  DfciXmlPermissionSchemaSupportLib|DfciPkg/Library/DfciXmlPermissionSchemaSupportLib/DfciXmlPermissionSchemaSupportLib.inf
  DfciSettingChangedNotificationLib|DfciPkg/Library/DfciSettingChangedNotificationLib/DfciSettingChangedNotificationLibNull.inf

[PcdsFeatureFlag]
  gDfciPkgTokenSpaceGuid.PcdDfciEnabled|FALSE

[PcdsFixedAtBuild]
  # The GUID of Frontpage.inf from MU_OEM_SAMPLE
  # 4042708A-0F2D-4823-AC60-0D77B3111889
  gEfiMdeModulePkgTokenSpaceGuid.PcdBootManagerMenuFile|{ 0x8A, 0x70, 0x42, 0x40, 0x2D, 0x0F, 0x23, 0x48, 0xAC, 0x60, 0x0D, 0x77, 0xB3, 0x11, 0x18, 0x89 }

  gMsGraphicsPkgTokenSpaceGuid.PcdUiThemeInDxe|TRUE

  gOemPkgTokenSpaceGuid.PcdUefiVersionNumber|0x0000030
  gOemPkgTokenSpaceGuid.PcdUefiBuildDate|0x20240927

  gOemPkgTokenSpaceGuid.PcdPostBackgroundColor|0x0

[LibraryClasses.common.DXE_DRIVER, LibraryClasses.common.UEFI_DRIVER, LibraryClasses.common.UEFI_APPLICATION, LibraryClasses.common.DXE_CORE]
  BaseCryptLib|CryptoPkg/Library/BaseCryptLibNull/BaseCryptLibNull.inf

[Components]
  #
  # Application that presents and manages FrontPage.
  #
  SiliciumPkg/Applications/FrontPage/FrontPage.inf

  #
  # Application that presents & manages the Boot Menu Setup on Front Page.
  #
  OemPkg/BootMenu/BootMenu.inf
  PcBdsPkg/MsBootPolicy/MsBootPolicy.inf

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

  #
  # AuthManager provides authentication for DFCI.
  #
  DfciPkg/IdentityAndAuthManager/IdentityAndAuthManagerDxe.inf
  DfciPkg/AuthManagerNull/AuthManagerNull.inf

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
