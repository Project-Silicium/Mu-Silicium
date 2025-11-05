#include <Library/DebugLib.h>
#include <Library/AslUpdateLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/RFSProtectionLib.h>
#include <Library/IoLib.h>

#include <Protocol/EFIChipInfo.h>
#include <Protocol/EFIPlatformInfo.h>
#include <Protocol/EFISmem.h>

//
// Global Variables
//
STATIC EFI_CHIPINFO_PROTOCOL     *mChipInfoProtocol;
STATIC EFI_PLATFORMINFO_PROTOCOL *mPlatformInfoProtocol;
STATIC EFI_SMEM_PROTOCOL         *mSmemProtocol;
STATIC CHAR8                      SIDS[EFICHIPINFO_MAX_ID_LENGTH];

EFI_STATUS
LocateQcomProtocols ()
{
  EFI_STATUS Status;

  // Locate Chip Info Protocol
  Status = gBS->LocateProtocol (&gEfiChipInfoProtocolGuid, NULL, (VOID *)&mChipInfoProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate Chip Info Protocol! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Locate Platform Info Protocol
  Status = gBS->LocateProtocol (&gEfiPlatformInfoProtocolGuid, NULL, (VOID *)&mPlatformInfoProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate Platform Info Protocol! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Locate SMEM Protocol
  Status = gBS->LocateProtocol (&gEfiSMEMProtocolGuid, NULL, (VOID *)&mSmemProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate SMEM Protocol! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
GetChipInfos (
  OUT EFIChipInfoVersionType      *Version,
  OUT EFIChipInfoIdType           *Id,
  OUT EFIChipInfoFamilyType       *Family,
  OUT EFIChipInfoModemType        *ModemInfos,
  OUT EFIChipInfoSerialNumType    *SerialNumber,
  OUT EFIChipInfoQFPROMChipIdType *QfpromId)
{
  EFI_STATUS                  Status;
  EFIChipInfoVersionType      ChipVersion;
  EFIChipInfoIdType           ChipId;
  EFIChipInfoFamilyType       ChipFamily;
  EFIChipInfoModemType        ChipModemInfos;
  EFIChipInfoSerialNumType    ChipSerialNumber;
  EFIChipInfoQFPROMChipIdType QfpromChipId;

  // Get Chip Version
  Status = mChipInfoProtocol->GetChipVersion (mChipInfoProtocol, &ChipVersion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to get Chip Version! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Get Chip ID
  Status = mChipInfoProtocol->GetChipId (mChipInfoProtocol, &ChipId);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to get Chip ID! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Get Chip ID String
  Status = mChipInfoProtocol->GetChipIdString (mChipInfoProtocol, SIDS, EFICHIPINFO_MAX_ID_LENGTH);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to get Chip ID String! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Get Chip Family
  Status = mChipInfoProtocol->GetChipFamily (mChipInfoProtocol, &ChipFamily);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to get Chip Family! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Get Modem Support Infos
  Status = mChipInfoProtocol->GetModemSupport (mChipInfoProtocol, &ChipModemInfos);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to get Modem Support Infos! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Get Chip Serial Number
  Status = mChipInfoProtocol->GetSerialNumber (mChipInfoProtocol, &ChipSerialNumber);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to get Chip Serial Number! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Get QFPROM Chip ID
  Status = mChipInfoProtocol->GetQFPROMChipId (mChipInfoProtocol, &QfpromChipId);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to get QFPROM Chip ID! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Pass Data
  *Version      = ChipVersion;
  *Id           = ChipId;
  *Family       = ChipFamily;
  *ModemInfos   = ChipModemInfos;
  *SerialNumber = ChipSerialNumber;
  *QfpromId     = QfpromChipId;

  return EFI_SUCCESS;
}

EFI_STATUS
GetPlatformInfos (OUT UINT32 *PlatformSubType)
{
  EFI_STATUS                          Status;
  EFI_PLATFORMINFO_PLATFORM_INFO_TYPE PlatformInfos;

  // Get Platform Infos
  Status = mPlatformInfoProtocol->GetPlatformInfo (mPlatformInfoProtocol, &PlatformInfos);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to get Platform Infos! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Pass Data
  *PlatformSubType = PlatformInfos.subtype;

  return EFI_SUCCESS;
}

EFI_STATUS
GetSmemInfos (OUT UINT64 *Address)
{
  EFI_STATUS Status;
  UINT32     SmemSize;

  // Get Address
  Status = mSmemProtocol->GetFunc (137, &SmemSize, (VOID *)Address);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to get Address from SMEM! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
GetMemoryRegionInfos (
  OUT UINT32 *MpssEfsAddress,
  OUT UINT32 *MpssEfsLength,
  OUT UINT32 *RFMB, // ?
  OUT UINT32 *RFMS, // ?
  OUT UINT32 *RFAB, // ?
  OUT UINT32 *RFAS, // ?
  OUT UINT32 *TgcmAddress,
  OUT UINT32 *TgcmLength)
{
  EFI_STATUS                      Status;
  EFI_MEMORY_REGION_DESCRIPTOR_EX MpssEfsRegion;
  EFI_MEMORY_REGION_DESCRIPTOR_EX AdspEfsRegion;
  EFI_MEMORY_REGION_DESCRIPTOR_EX TgcmRegion;

  // Set Default Values
  *MpssEfsAddress = 0;
  *MpssEfsLength  = 0;
  *RFMB           = 0;
  *RFMS           = 0;
  *RFAB           = 0;
  *RFAS           = 0;
  *TgcmAddress    = 0xDEADBEEF;
  *TgcmLength     = 0xBEEFDEAD;

  // Locate "MPSS_EFS" Memory Region
  Status = LocateMemoryMapAreaByName ("MPSS_EFS", &MpssEfsRegion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate 'MPSS_EFS' Memory Region! Status = %r\n", __FUNCTION__, Status));
  } else {
    // Pass Data
    *MpssEfsAddress = (UINT32)MpssEfsRegion.Address;
    *MpssEfsLength  = (UINT32)MpssEfsRegion.Length;

    // Configure MPSS Permissions
    RFSLocateAndProtectSharedArea ();
  }

  // Locate "ADSP_EFS" Memory Region
  Status = LocateMemoryMapAreaByName ("ADSP_EFS", &AdspEfsRegion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate 'ADSP_EFS' Memory Region! Status = %r\n", __FUNCTION__, Status));
  } else {
    // Pass Data
    *RFMB = (UINT32)AdspEfsRegion.Address + (UINT32)AdspEfsRegion.Length / 2;
    *RFMS = (UINT32)AdspEfsRegion.Length / 2;
    *RFAB = (UINT32)AdspEfsRegion.Address;
    *RFAS = (UINT32)AdspEfsRegion.Length / 2;
  }

  // Locate "TGCM" Memory Region
  Status = LocateMemoryMapAreaByName ("TGCM", &TgcmRegion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate 'TGCM' Memory Region! Status = %r\n", __FUNCTION__, Status));
  } else {
    // Pass Data
    *TgcmAddress = (UINT32)TgcmRegion.Address;
    *TgcmLength  = (UINT32)TgcmRegion.Length;
  }

  return EFI_SUCCESS;
}

VOID
PlatformUpdateAcpiTables ()
{
  EFI_STATUS Status;
  UINT32     SIDV;
  UINT32     SOID;
  UINT16     SDFE;
  UINT16     SIDM;
  UINT32     SOSN1;
  UINT32     SOSN2;
  UINT32     PLST;
  UINT64     SOSI;
  UINT32     RMTB;
  UINT32     RMTX;
  UINT32     RFMB;
  UINT32     RFMS;
  UINT32     RFAB;
  UINT32     RFAS;
  UINT32     TCMA;
  UINT32     TCML;

  // Locate needed Protocols
  Status = LocateQcomProtocols ();
  if (EFI_ERROR (Status)) {
    return;
  }

  // Get needed Chip Infos
  Status = GetChipInfos (&SIDV, &SOID, (EFIChipInfoFamilyType *)&SDFE, (EFIChipInfoModemType *)&SIDM, &SOSN1, &SOSN2);
  if (EFI_ERROR (Status)) {
    return;
  }

  // Set Major and Minor Chip Version
  UINT16 SVMJ = (UINT16)((SIDV >> 16) & 0xFFFF);
  UINT16 SVMI = (UINT16)(SIDV & 0xFFFF);

  // Combine QFPROM Chip ID & Chip Serial Number
  UINT64 SOSN = ((UINT64)SOSN2 << 32) | SOSN1;

  // Get needed Platform Infos
  Status = GetPlatformInfos (&PLST);
  if (EFI_ERROR (Status)) {
    return;
  }

  // Get needed SMEM Infos
  Status = GetSmemInfos (&SOSI);
  if (EFI_ERROR (Status)) {
    return;
  }

  // Get needed Memory Regions
  Status = GetMemoryRegionInfos (&RMTB, &RMTX, &RFMB, &RFMS, &RFAB, &RFAS, &TCMA, &TCML);
  if (EFI_ERROR (Status)) {
    return;
  }

  // Set Default Values
  UINT32 STOR = 0x1;
  UINT32 SUFS = 0xFFFFFFFF;
  UINT32 PUS3 = 0x1;
  UINT32 SUS3 = 0xFFFFFFFF;
  UINT32 PRP0 = 0;
  UINT32 PRP1 = 0;

  // Get Values from Memory
  UINT32 SIDT = (MmioRead32 (0x221C21B8) & 0xFF00000) >> 20;
  UINT32 SJTG = MmioRead32 (0x221C21B8) & 0xFFFFF;
  UINT32 EMUL = MmioRead32 (0x01FC8004) & 0x3;

  // Update DSDT / SSDT Names
  UpdateNameAslCode (SIGNATURE_32 ('E', 'M', 'U', 'L'), &EMUL, 4);
  UpdateNameAslCode (SIGNATURE_32 ('P', 'L', 'S', 'T'), &PLST, 4);
  UpdateNameAslCode (SIGNATURE_32 ('P', 'U', 'S', '3'), &PUS3, 4);
  UpdateNameAslCode (SIGNATURE_32 ('R', 'F', 'A', 'B'), &RFAB, 4);
  UpdateNameAslCode (SIGNATURE_32 ('R', 'F', 'A', 'S'), &RFAS, 4);
  UpdateNameAslCode (SIGNATURE_32 ('R', 'F', 'M', 'B'), &RFMB, 4);
  UpdateNameAslCode (SIGNATURE_32 ('R', 'F', 'M', 'S'), &RFMS, 4);
  UpdateNameAslCode (SIGNATURE_32 ('R', 'M', 'T', 'B'), &RMTB, 4);
  UpdateNameAslCode (SIGNATURE_32 ('R', 'M', 'T', 'X'), &RMTX, 4);
  UpdateNameAslCode (SIGNATURE_32 ('P', 'R', 'P', '0'), &PRP0, 4);
  UpdateNameAslCode (SIGNATURE_32 ('P', 'R', 'P', '1'), &PRP1, 4);
  UpdateNameAslCode (SIGNATURE_32 ('S', 'D', 'F', 'E'), &SDFE, 2);
  UpdateNameAslCode (SIGNATURE_32 ('S', 'I', 'D', 'M'), &SIDM, 2);
  UpdateNameAslCode (SIGNATURE_32 ('S', 'I', 'D', 'S'), &SIDS, EFICHIPINFO_MAX_ID_LENGTH);
  UpdateNameAslCode (SIGNATURE_32 ('S', 'I', 'D', 'T'), &SIDT, 4);
  UpdateNameAslCode (SIGNATURE_32 ('S', 'I', 'D', 'V'), &SIDV, 4);
  UpdateNameAslCode (SIGNATURE_32 ('S', 'J', 'T', 'G'), &SJTG, 4);
  UpdateNameAslCode (SIGNATURE_32 ('S', 'O', 'I', 'D'), &SOID, 4);
  UpdateNameAslCode (SIGNATURE_32 ('S', 'O', 'S', 'N'), &SOSN, 8);
  UpdateNameAslCode (SIGNATURE_32 ('S', 'O', 'S', 'I'), &SOSI, 8);
  UpdateNameAslCode (SIGNATURE_32 ('S', 'T', 'O', 'R'), &STOR, 4);
  UpdateNameAslCode (SIGNATURE_32 ('S', 'U', 'F', 'S'), &SUFS, 4);
  UpdateNameAslCode (SIGNATURE_32 ('S', 'U', 'S', '3'), &SUS3, 4);
  UpdateNameAslCode (SIGNATURE_32 ('S', 'V', 'M', 'I'), &SVMI, 2);
  UpdateNameAslCode (SIGNATURE_32 ('S', 'V', 'M', 'J'), &SVMJ, 2);
  UpdateNameAslCode (SIGNATURE_32 ('T', 'C', 'M', 'A'), &TCMA, 4);
  UpdateNameAslCode (SIGNATURE_32 ('T', 'C', 'M', 'L'), &TCML, 4);
}
