/**
  Copyright (c) 2017, Andrey Warkentin <andrey.warkentin@gmail.com>
  Copyright (c) 2018, Bingxing Wang <uefi-oss-projects@imbushuo.net>
  Copyright (c) Microsoft Corporation. All rights reserved.
  Copyright (c) 2012, Apple Inc. All rights reserved.
  Copyright (c) 2013 Linaro.org

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD
  License which accompanies this distribution.  The full text of the license may
  be found at http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/

#ifndef _TABLE_DEFINITIONS_H_
#define _TABLE_DEFINITIONS_H_

//
// SMBIOS TYPE0
//
EFI_SMBIOS_TYPE0
mSmBiosType0 = {
  //
  // TYPE0 Table
  //
  .Table = {
    //
    // Table Header
    //
    .Hdr = {
      .Type   = EFI_SMBIOS_TYPE_BIOS_INFORMATION,
      .Length = sizeof (SMBIOS_TABLE_TYPE0)
    },

    //
    // Table String Indexes
    //
    .Vendor          = 1,
    .BiosVersion     = 2,
    .BiosReleaseDate = 3,

    //
    // Minor & Major Release
    //
    .SystemBiosMajorRelease                 = 0xFF,
    .SystemBiosMinorRelease                 = 0xFF,
    .EmbeddedControllerFirmwareMajorRelease = 0xFF,
    .EmbeddedControllerFirmwareMinorRelease = 0xFF,

    //
    // BIOS Characteristics
    //
    .BiosCharacteristics = {
      .PciIsSupported            = 1,
      .PlugAndPlayIsSupported    = 1,
      .BiosIsUpgradable          = 1,
      .SelectableBootIsSupported = 1
    },

    //
    // Extended BIOSCharacteristics
    //
    .BIOSCharacteristicsExtensionBytes = { 
      [0] = BIT0,
      [1] = BIT2 | BIT3
    }
  },

  //
  // TYPE0 Table Strings
  //
  .Strings = {
    [0] = "Not Specified",
    [1] = "Not Specified",
    [2] = "Not Specified"
  }
};

//
// SMBIOS TYPE1
//
EFI_SMBIOS_TYPE1
mSmBiosType1 = {
  //
  // TYPE1 Table
  //
  .Table = {
    //
    // Table Header
    //
    .Hdr = {
      .Type   = EFI_SMBIOS_TYPE_SYSTEM_INFORMATION,
      .Length = sizeof (SMBIOS_TABLE_TYPE1)
    },

    //
    // Table String Indexes
    //
    .Manufacturer = 1,
    .ProductName  = 2,
    .Version      = 3,
    .SerialNumber = 4,
    .SKUNumber    = 5,
    .Family       = 6,

    //
    // System Wake Up Type
    //
    .WakeUpType = SystemWakeupTypePowerSwitch
  },

  //
  // TYPE1 Table Strings
  //
  .Strings = {
    [0] = "Not Specified",
    [1] = "Not Specified",
    [2] = "Not Specified",
    [3] = "Not Specified",
    [4] = "Not Specified",
    [5] = "Not Specified"
  }
};

//
// SMBIOS TYPE2
//
EFI_SMBIOS_TYPE2
mSmBiosType2 = {
  //
  // TYPE2 Table
  //
  .Table = {
    //
    // Table Header
    //
    .Hdr = {
      .Type   = EFI_SMBIOS_TYPE_BASEBOARD_INFORMATION,
      .Length = sizeof (SMBIOS_TABLE_TYPE2)
    },

    //
    // Table String Indexes
    //
    .Manufacturer      = 1,
    .ProductName       = 2,
    .Version           = 3,
    .SerialNumber      = 4,
    .AssetTag          = 5,
    .LocationInChassis = 6,

    //
    // Board Type
    //
    .BoardType = BaseBoardTypeMotherBoard,

    //
    // Board Feature Flags
    //
    .FeatureFlag = {
      .Motherboard = 1
    }
  },

  //
  // TYPE2 Table Strings
  //
  .Strings = {
    [0] = "Not Specified",
    [1] = "Not Specified",
    [2] = "Not Specified",
    [3] = "Not Specified",
    [4] = "Not Specified",
    [5] = "Not Specified"
  }
};

//
// SMBIOS TYPE3
//
EFI_SMBIOS_TYPE3
mSmBiosType3 = {
  //
  // TYPE3 Table
  //
  .Table = {
    //
    // Table Header
    //
    .Hdr = {
      .Type   = EFI_SMBIOS_TYPE_SYSTEM_ENCLOSURE,
      .Length = sizeof (SMBIOS_TABLE_TYPE3)
    },

    //
    // Table String Indexes
    //
    .Manufacturer = 1,
    .Version      = 2,
    .SerialNumber = 3,
    .AssetTag     = 4,

    //
    // Chassis Details
    //
    .Type   = MiscChassisTypeUnknown,
    .Height = ChassisHeightUnspecified,

    //
    // Chassis States
    //
    .BootupState      = ChassisStateSafe,
    .PowerSupplyState = ChassisStateSafe,
    .ThermalState     = ChassisStateSafe,
    .SecurityStatus   = ChassisSecurityStatusNone
  },

  //
  // TYPE3 Table Strings
  //
  .Strings = {
    [0] = "Not Specified",
    [1] = "Not Specified",
    [2] = "Not Specified",
    [3] = "Not Specified"
  }
};

//
// SMBIOS TYPE4
//
EFI_SMBIOS_TYPE4
mSmBiosType4 = {
  //
  // TYPE4 Table
  //
  .Table = {
    //
    // Table Header
    //
    .Hdr = {
      .Type   = EFI_SMBIOS_TYPE_PROCESSOR_INFORMATION,
      .Length = sizeof (SMBIOS_TABLE_TYPE4)
    },

    //
    // Table String Indexes
    //
    .Socket                = 1,
    .ProcessorManufacturer = 2,
    .ProcessorVersion      = 3,
    .SerialNumber          = 4,
    .AssetTag              = 5,
    .PartNumber            = 6,
    .SocketType            = 7,

    //
    // Processor Details
    //
    .ProcessorType    = CentralProcessor,
    .ProcessorFamily  = ProcessorFamilyIndicatorFamily2,
    .ProcessorFamily2 = ProcessorFamilyARM,
    .ProcessorUpgrade = ProcessorUpgradeOther,

    //
    // Processor Characteristics
    //
    .ProcessorCharacteristics = BIT2 | BIT9,
    .Status                   = BIT0 | BIT1,

    //
    // Processor Cache Handles
    //
    .L1CacheHandle = 0xFFFF,
    .L2CacheHandle = 0xFFFF,
    .L3CacheHandle = 0xFFFF
  },

  //
  // TYPE4 Table Strings
  //
  .Strings = {
    [0] = "Not Specified",
    [1] = "Not Specified",
    [2] = "Not Specified",
    [3] = "Not Specified",
    [4] = "Not Specified",
    [5] = "Not Specified",
    [6] = "Not Specified"
  }
};

//
// SMBIOS TYPE7 (L1I)
//
EFI_SMBIOS_TYPE7
mSmBiosType7_L1I = {
  //
  // TYPE7 Table
  //
  .Table = {
    //
    // Table Header
    //
    .Hdr = {
      .Type   = EFI_SMBIOS_TYPE_CACHE_INFORMATION,
      .Length = sizeof (SMBIOS_TABLE_TYPE7)
    },

    //
    // Table String Indexes
    //
    .SocketDesignation = 1,

    //
    // Cache Details
    //
    .CacheConfiguration  = BIT8 | BIT7 | (0 << 0),
    .ErrorCorrectionType = CacheErrorUnknown,
    .SystemCacheType     = CacheTypeInstruction,
    .Associativity       = CacheAssociativityUnknown,

    //
    // Supported SRAM Types
    //
    .SupportedSRAMType = {
      .Unknown = 1
    },

    //
    // Current SRAM Type
    //
    .CurrentSRAMType = {
      .Unknown = 1
    }
  },

  //
  // TYPE7 Table Strings
  //
  .Strings = {
    [0] = "Not Specified"
  }
};

//
// SMBIOS TYPE7 (L1D)
//
EFI_SMBIOS_TYPE7
mSmBiosType7_L1D = {
  //
  // TYPE7 Table
  //
  .Table = {
    //
    // Table Header
    //
    .Hdr = {
      .Type   = EFI_SMBIOS_TYPE_CACHE_INFORMATION,
      .Length = sizeof (SMBIOS_TABLE_TYPE7)
    },

    //
    // Table String Indexes
    //
    .SocketDesignation = 1,

    //
    // Cache Details
    //
    .CacheConfiguration  = BIT8 | BIT7 | (0 << 0),
    .ErrorCorrectionType = CacheErrorUnknown,
    .SystemCacheType     = CacheTypeData,
    .Associativity       = CacheAssociativityUnknown,

    //
    // Supported SRAM Types
    //
    .SupportedSRAMType = {
      .Unknown = 1
    },

    //
    // Current SRAM Type
    //
    .CurrentSRAMType = {
      .Unknown = 1
    }
  },

  //
  // TYPE7 Table Strings
  //
  .Strings = {
    [0] = "Not Specified"
  }
};

//
// SMBIOS TYPE7 (L2)
//
EFI_SMBIOS_TYPE7
mSmBiosType7_L2 = {
  //
  // TYPE7 Table
  //
  .Table = {
    //
    // Table Header
    //
    .Hdr = {
      .Type   = EFI_SMBIOS_TYPE_CACHE_INFORMATION,
      .Length = sizeof (SMBIOS_TABLE_TYPE7)
    },

    //
    // Table String Indexes
    //
    .SocketDesignation = 1,

    //
    // Cache Details
    //
    .CacheConfiguration  = BIT8 | BIT7 | (1 << 0),
    .ErrorCorrectionType = CacheErrorUnknown,
    .SystemCacheType     = CacheTypeUnified,
    .Associativity       = CacheAssociativityUnknown,

    //
    // Supported SRAM Types
    //
    .SupportedSRAMType = {
      .Unknown = 1
    },

    //
    // Current SRAM Type
    //
    .CurrentSRAMType = {
      .Unknown = 1
    }
  },

  //
  // TYPE7 Table Strings
  //
  .Strings = {
    [0] = "Not Specified"
  }
};

//
// SMBIOS TYPE7 (L3)
//
EFI_SMBIOS_TYPE7
mSmBiosType7_L3 = {
  //
  // TYPE7 Table
  //
  .Table = {
    //
    // Table Header
    //
    .Hdr = {
      .Type   = EFI_SMBIOS_TYPE_CACHE_INFORMATION,
      .Length = sizeof (SMBIOS_TABLE_TYPE7)
    },

    //
    // Table String Indexes
    //
    .SocketDesignation = 1,

    //
    // Cache Details
    //
    .CacheConfiguration  = BIT8 | BIT7 | (2 << 0),
    .ErrorCorrectionType = CacheErrorUnknown,
    .SystemCacheType     = CacheTypeUnified,
    .Associativity       = CacheAssociativityUnknown,

    //
    // Supported SRAM Types
    //
    .SupportedSRAMType = {
      .Unknown = 1
    },

    //
    // Current SRAM Type
    //
    .CurrentSRAMType = {
      .Unknown = 1
    }
  },

  //
  // TYPE7 Table Strings
  //
  .Strings = {
    [0] = "Not Specified"
  }
};

//
// SMBIOS TYPE16
//
EFI_SMBIOS_TYPE16
mSmBiosType16 = {
  //
  // TYPE16 Table
  //
  .Table = {
    //
    // Table Header
    //
    .Hdr = {
      .Type   = EFI_SMBIOS_TYPE_PHYSICAL_MEMORY_ARRAY, 
      .Length = sizeof (SMBIOS_TABLE_TYPE16)
    },

    //
    // Memory Error Information Handle
    //
    .MemoryErrorInformationHandle = 0xFFFE,

    //
    // Memory Location
    //
    .Location = MemoryArrayLocationSystemBoard,

    //
    // Memory Details
    //
    .Use                   = MemoryArrayUseSystemMemory,
    .MemoryErrorCorrection = MemoryErrorCorrectionUnknown,

    //
    // Amount of Memory Devices
    //
    .NumberOfMemoryDevices = 1
  }
};

//
// SMBIOS TYPE17
//
EFI_SMBIOS_TYPE17
mSmBiosType17 = {
  //
  // TYPE17 Table
  //
  .Table = {
    //
    // Table Header
    //
    .Hdr = {
      .Type   = EFI_SMBIOS_TYPE_MEMORY_DEVICE,
      .Length = sizeof (SMBIOS_TABLE_TYPE17)
    },

    //
    // Table String Indexes
    //
    .DeviceLocator   = 1,
    .BankLocator     = 2,
    .Manufacturer    = 3,
    .SerialNumber    = 4,
    .AssetTag        = 5,
    .PartNumber      = 6,
    .FirmwareVersion = 7,

    //
    // Memory Error Information
    //
    .MemoryErrorInformationHandle = 0xFFFE,

    //
    // Memory Width
    //
    .TotalWidth = 0xFFFF,
    .DataWidth  = 0xFFFF,

    //
    // Memory Sizes
    //
    .Size            = 0xFFFF,
    .VolatileSize    = 0xFFFFFFFFFFFFFFFF,
    .CacheSize       = 0xFFFFFFFFFFFFFFFF,

    //
    // Memory Details
    //
    .FormFactor       = MemoryFormFactorUnknown,
    .MemoryType       = MemoryTypeUnknown,
    .MemoryTechnology = MemoryTechnologyDram,
    .DeviceSet        = 0xFF,

    //
    // Memory Type Details
    //
    .TypeDetail = {
      .Unknown = 1
    },

    //
    // Memory Operating Capabilities
    //
    .MemoryOperatingModeCapability = {
      .Bits = {
        .Unknown = 1
      }
    }
  },

  //
  // TYPE17 Table Strings
  //
  .Strings = {
    [0] = "Not Specified",
    [1] = "Not Specified",
    [2] = "Not Specified",
    [3] = "Not Specified",
    [4] = "Not Specified",
    [5] = "Not Specified",
    [6] = "Not Specified"
  }
};

//
// SMBIOS TYPE19
//
EFI_SMBIOS_TYPE19
mSmBiosType19 = {
  //
  // TYPE19 Table
  //
  .Table = {
    //
    // Table Header
    //
    .Hdr = {
      .Type   = EFI_SMBIOS_TYPE_MEMORY_ARRAY_MAPPED_ADDRESS,
      .Length = sizeof (SMBIOS_TABLE_TYPE19)
    }
  }
};

#endif /* _TABLE_DEFINITIONS_H_ */
