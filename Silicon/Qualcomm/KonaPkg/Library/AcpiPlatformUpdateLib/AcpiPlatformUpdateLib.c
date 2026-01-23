#include <Library/AslUpdateLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/EFIChipInfo.h>
#include <Protocol/EFIPlatformInfo.h>
#include <Protocol/EFISmem.h>

VOID
PlatformUpdateAcpiTables ()
{
  EFI_STATUS                          Status;
  EFI_MEMORY_REGION_DESCRIPTOR_EX     TGCMRegion;
  EFI_CHIPINFO_PROTOCOL              *mChipInfoProtocol;
  EFI_PLATFORMINFO_PROTOCOL          *mPlatformInfoProtocol;
  EFI_SMEM_PROTOCOL                  *mSmemProtocol;
  EFI_PLATFORMINFO_PLATFORM_INFO_TYPE PlatformInfo;

  UINT32  SOID                            = 0;
  UINT32  STOR                            = 0x1;
  UINT32  SIDV                            = 0;
  UINT16  SDFE                            = 0;
  UINT16  SIDM                            = 0;
  UINT32 PUS3                             = FixedPcdGetBool(PcdStorageIsUfs3);
  UINT32 *pSIDT                           = (UINT32 *)0x784198;
  UINT32  SIDT                            = (*pSIDT & 0xFF00000) >> 20;
  UINT32 *pSJTG                           = (UINT32 *)0x784198;
  UINT32  SJTG                            = *pSJTG & 0xFFFFF;
  UINT32 *pEMUL                           = (UINT32 *)0x1FC8004;
  UINT32  EMUL                            = *pEMUL & 0x3;
  UINT32  SOSN1                           = 0;
  UINT32  SOSN2                           = 0;
  UINT64  SOSI                            = 0;
  UINT32  PRP0                            = 0;
  UINT32  PRP1                            = 0;
  UINT32  PRP2                            = 0;
  CHAR8   SIDS[EFICHIPINFO_MAX_ID_LENGTH] = {0};
  UINT32  TCMA                            = 0;
  UINT32  TCML                            = 0;
  UINT32  SmemSize                        = 0;

  // Locate Chip Info Protocol
  Status  = gBS->LocateProtocol (&gEfiChipInfoProtocolGuid, NULL, (VOID *)&mChipInfoProtocol);

  // Locate SMEM Protocol
  Status |= gBS->LocateProtocol (&gEfiSMEMProtocolGuid, NULL, (VOID *)&mSmemProtocol);

  // Locate Platform Info Protocol
  Status |= gBS->LocateProtocol (&gEfiPlatformInfoProtocolGuid, NULL, (VOID *)&mPlatformInfoProtocol);

  if (EFI_ERROR (Status)) {
    return;
  }

  mChipInfoProtocol->GetChipId       (mChipInfoProtocol, (EFIChipInfoIdType *)&SOID);
  mChipInfoProtocol->GetChipVersion  (mChipInfoProtocol, &SIDV);
  mChipInfoProtocol->GetChipFamily   (mChipInfoProtocol, (EFIChipInfoFamilyType *)&SDFE);
  mChipInfoProtocol->GetModemSupport (mChipInfoProtocol, (EFIChipInfoModemType *)&SIDM);
  mChipInfoProtocol->GetSerialNumber (mChipInfoProtocol, (EFIChipInfoSerialNumType *)&SOSN1);
  mChipInfoProtocol->GetQFPROMChipId (mChipInfoProtocol, (EFIChipInfoQFPROMChipIdType *)&SOSN2);
  mChipInfoProtocol->GetChipIdString (mChipInfoProtocol, SIDS, EFICHIPINFO_MAX_ID_LENGTH);

  mSmemProtocol->GetFunc (137, &SmemSize, (VOID *)&SOSI);

  mPlatformInfoProtocol->GetPlatformInfo (mPlatformInfoProtocol, &PlatformInfo);

  UINT16 SVMJ = (UINT16)((SIDV >> 16) & 0xFFFF);
  UINT16 SVMI = (UINT16)(SIDV & 0xFFFF);
  UINT64 SOSN = ((UINT64)SOSN2 << 32) | SOSN1;
  UINT32 PLST = PlatformInfo.subtype;

  if (!EFI_ERROR(LocateMemoryMapAreaByName("TGCM", &TGCMRegion))) {
    TCMA = (UINT32)TGCMRegion.Address;
    TCML = (UINT32)TGCMRegion.Length;
  } else {
    TCMA = 0xDEADBEEF;
    TCML = 0xBEEFDEAD;
  }

  UpdateNameAslCode (SIGNATURE_32('S', 'O', 'I', 'D'), &SOID, 4);
  UpdateNameAslCode (SIGNATURE_32('S', 'T', 'O', 'R'), &STOR, 4);
  UpdateNameAslCode (SIGNATURE_32('S', 'I', 'D', 'V'), &SIDV, 4);
  UpdateNameAslCode (SIGNATURE_32('S', 'V', 'M', 'J'), &SVMJ, 2);
  UpdateNameAslCode (SIGNATURE_32('S', 'V', 'M', 'I'), &SVMI, 2);
  UpdateNameAslCode (SIGNATURE_32('S', 'D', 'F', 'E'), &SDFE, 2);
  UpdateNameAslCode (SIGNATURE_32('S', 'I', 'D', 'M'), &SIDM, 2);
  UpdateNameAslCode (SIGNATURE_32('P', 'U', 'S', '3'), &PUS3, 4);
  UpdateNameAslCode (SIGNATURE_32('S', 'I', 'D', 'T'), &SIDT, 4);
  UpdateNameAslCode (SIGNATURE_32('S', 'J', 'T', 'G'), &SJTG, 4);
  UpdateNameAslCode (SIGNATURE_32('E', 'M', 'U', 'L'), &EMUL, 4);
  UpdateNameAslCode (SIGNATURE_32('S', 'O', 'S', 'N'), &SOSN, 8);
  UpdateNameAslCode (SIGNATURE_32('P', 'L', 'S', 'T'), &PLST, 4);
  UpdateNameAslCode (SIGNATURE_32('T', 'C', 'M', 'A'), &TCMA, 4);
  UpdateNameAslCode (SIGNATURE_32('T', 'C', 'M', 'L'), &TCML, 4);
  UpdateNameAslCode (SIGNATURE_32('S', 'O', 'S', 'I'), &SOSI, 8);
  UpdateNameAslCode (SIGNATURE_32('P', 'R', 'P', '0'), &PRP0, 4);
  UpdateNameAslCode (SIGNATURE_32('P', 'R', 'P', '1'), &PRP1, 4);
  UpdateNameAslCode (SIGNATURE_32('P', 'R', 'P', '2'), &PRP2, 4);
  UpdateNameAslCode (SIGNATURE_32('S', 'I', 'D', 'S'), &SIDS, EFICHIPINFO_MAX_ID_LENGTH);
}