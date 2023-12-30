[LibraryClasses.common]
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
  BootGraphicsLib|QcomPkg/Library/BootGraphicsLib/BootGraphicsLib.inf

  #
  # BootGraphicsProviderLib that uses PCDs and raw files in the firmware volumes
  #
  BootGraphicsProviderLib|QcomPkg/Library/BootGraphicsProviderLib/BootGraphicsProviderLib.inf

  #
  # This library contains access functions for dealing with the copying of an MS_UI_THEME
  #
  MsUiThemeCopyLib|MsGraphicsPkg/Library/MsUiThemeCopyLib/MsUiThemeCopyLib.inf

  #
  # Library to provide platform version information
  #
  MuUefiVersionLib|OemPkg/Library/MuUefiVersionLib/MuUefiVersionLib.inf

  #
  # Null ConsoleMsg Libary
  #
  ConsoleMsgLib|PcBdsPkg/Library/ConsoleMsgLibNull/ConsoleMsgLibNull.inf

  #
  # This is the Thermal Services Library that encapsulates the platform specific thermal information.
  #
  ThermalServicesLib|PcBdsPkg/Library/ThermalServicesLibNull/ThermalServicesLibNull.inf

  #
  # Power Services Library class to support Platforms that have a battery
  #
  PowerServicesLib|QcomPkg/Library/PowerServicesLib/PowerServicesLib.inf

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
  MsPlatformDevicesLib|QcomPkg/Library/MsPlatformDevicesLib/MsPlatformDevicesLib.inf

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
  MsColorTableLib|QcomPkg/Library/MsColorTableLib/MsColorTableLib.inf

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
  MsBootPolicyLib|QcomPkg/Library/MsBootPolicyLib/MsBootPolicyLib.inf

  #
  # Supplies the theme for this platform to the UEFI settings UI
  #
  MsUiThemeLib|MsGraphicsPkg/Library/MsUiThemeLib/Dxe/MsUiThemeLib.inf

  #
  # Library for determining if Manufacturing mode should be enabled on this boot. 
  # This libray is what everyone should use to determine which mode we are in.
  # It can be called from PEI or DXE.
  #
  UIToolKitLib|MsGraphicsPkg/Library/SimpleUIToolKit/SimpleUIToolKit.inf
  MsWheaEarlyStorageLib|QcomPkg/Library/MsWheaEarlyStorageLib/MsWheaEarlyStorageLib.inf
  CheckHwErrRecHeaderLib|MsWheaPkg/Library/CheckHwErrRecHeaderLib/CheckHwErrRecHeaderLib.inf
  ParserRegistryLib|MsWheaPkg/Library/ParserRegistryLib/ParserRegistryLib.inf
  Hash2CryptoLib|SecurityPkg/Library/DxeHash2CryptoLib/DxeHash2CryptoLib.inf
  MsPlatformEarlyGraphicsLib|QcomPkg/Library/MsPlatformEarlyGraphicsLib/MsEarlyGraphicsLib.inf

[PcdsFixedAtBuild.common]
  # The GUID of Frontpage.inf from MU_OEM_SAMPLE
  # 4042708A-0F2D-4823-AC60-0D77B3111889
  gEfiMdeModulePkgTokenSpaceGuid.PcdBootManagerMenuFile|{ 0x8A, 0x70, 0x42, 0x40, 0x2D, 0x0F, 0x23, 0x48, 0xAC, 0x60, 0x0D, 0x77, 0xB3, 0x11, 0x18, 0x89 }

  gOemPkgTokenSpaceGuid.PcdUefiVersionNumber|0x0000022
  gOemPkgTokenSpaceGuid.PcdUefiBuildDate|0x20231217

  gOemPkgTokenSpaceGuid.PcdPostBackgroundColor|0x0

[LibraryClasses.AARCH64.DXE_DRIVER, LibraryClasses.AARCH64.UEFI_DRIVER, LibraryClasses.AARCH64.UEFI_APPLICATION, LibraryClasses.AARCH64.DXE_CORE]
  BaseCryptLib|CryptoPkg/Library/BaseCryptLibNull/BaseCryptLibNull.inf
  TlsLib|CryptoPkg/Library/TlsLibNull/TlsLibNull.inf

[Components.AARCH64]
  #
  # Application that presents and manages FrontPage.
  #
  QcomPkg/Applications/FrontPage/FrontPage.inf

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

  DfciPkg/Application/DfciMenu/DfciMenu.inf

  CryptoPkg/Driver/Bin/CryptoDriverBin_STANDARD_Dxe_$(TARGET)_AARCH64.inf 

  CryptoPkg/Library/BaseCryptLibOnProtocolPpi/DxeCryptLib.inf {
    <PcdsFixedAtBuild>
     !include CryptoPkg/Driver/Bin/Crypto.pcd.STANDARD.inc.dsc
  }