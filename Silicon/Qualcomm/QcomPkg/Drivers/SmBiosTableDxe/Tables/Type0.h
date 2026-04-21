/**
  Copyright (c), 2017, Andrey Warkentin <andrey.warkentin@gmail.com>
  Copyright (c), 2018, Bingxing Wang <uefi-oss-projects@imbushuo.net>
  Copyright (c), Microsoft Corporation. All rights reserved.
  Copyright (c) 2012, Apple Inc. All rights reserved.<BR>
  Copyright (c) 2013 Linaro.org

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD
  License which accompanies this distribution.  The full text of the license may
  be found at http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/

#ifndef _TYPE_0_TABLE_H_
#define _TYPE_0_TABLE_H_

SMBIOS_TABLE_TYPE0 mBIOSInfoType0 = {
  {EFI_SMBIOS_TYPE_BIOS_INFORMATION, sizeof(SMBIOS_TABLE_TYPE0), 0},
  1,      // Vendor String
  2,      // BiosVersion String
  0xE000, // BiosSegment
  3,      // BiosReleaseDate String
  0x7F,   // BiosSize
  {
    0, // Reserved
    0, // Unknown
    0, // BiosCharacteristicsNotSupported
    0, // IsaIsSupported
    0, // McaIsSupported
    0, // EisaIsSupported
    1, // PciIsSupported
    0, // PcmciaIsSupported
    0, // PlugAndPlayIsSupported
    0, // ApmIsSupported
    1, // BiosIsUpgradable
    1, // BiosShadowingAllowed
    0, // VlVesaIsSupported
    0, // EscdSupportIsAvailable
    0, // BootFromCdIsSupported
    1, // SelectableBootIsSupported
    0, // RomBiosIsSocketed
    0, // BootFromPcmciaIsSupported
    1, // EDDSpecificationIsSupported
    0, // JapaneseNecFloppyIsSupported
    0, // JapaneseToshibaFloppyIsSupported
    0, // Floppy525_360IsSupported
    0, // Floppy525_12IsSupported
    0, // Floppy35_720IsSupported
    0, // Floppy35_288IsSupported
    1, // PrintScreenIsSupported
    1, // Keyboard8042IsSupported
    1, // SerialIsSupported
    0, // PrinterIsSupported
    0, // CgaMonoIsSupported
    0, // NecPc98
    0  // ReservedForVendor
  },
  {
    0x03, // AcpiIsSupported
          // UsbLegacyIsSupported
          // AgpIsSupported
          // I2OBootIsSupported
          // Ls120BootIsSupported
          // AtapiZipDriveBootIsSupported
          // Boot1394IsSupported
          // SmartBatteryIsSupported

    0x05, // BiosBootSpecIsSupported
          // FunctionKeyNetworkBootIsSupported
          // TargetContentDistributionEnabled
          // UefiSpecificationSupported
          // VirtualMachineSupported
          // ExtensionByte2Reserved
  },
  0x01, // SystemBiosMajorRelease
  0x05, // SystemBiosMinorRelease
  0xFF, // EmbeddedControllerFirmwareMajorRelease
  0xFF, // EmbeddedControllerFirmwareMinorRelease
  {
    0, // Size
    0  // Unit
  }
};

CHAR8 *mBIOSInfoType0Strings[] = {
  "Not Specified",
  "Not Specified",
  "Not Specified",
  NULL
};

#endif /* _TYPE_0_TABLE_H_ */
