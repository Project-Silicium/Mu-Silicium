#include <Library/AcpiPlatformUpdateLib.h>
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
  ARM_MEMORY_REGION_DESCRIPTOR_EX     MPSSEFSRegion;
  ARM_MEMORY_REGION_DESCRIPTOR_EX     ADSPEFSRegion;
  EFI_CHIPINFO_PROTOCOL              *mChipInfoProtocol;
  EFI_PLATFORMINFO_PROTOCOL          *mPlatformInfoProtocol;
  EFI_SMEM_PROTOCOL                  *mSmemProtocol;
  EFI_PLATFORMINFO_PLATFORM_INFO_TYPE PlatformInfo;

  UINT32 SOID                            = 0;
  UINT32 SIDV                            = 0;
  UINT16 SDFE                            = 0;
  UINT16 SIDM                            = 0;
  UINT32 SOSN1                           = 0;
  UINT32 SOSN2                           = 0;
  UINT64 SOSI                            = 0;
  CHAR8  SIDS[EFICHIPINFO_MAX_ID_LENGTH] = {0};
  UINT32 RMTB                            = 0;
  UINT32 RMTX                            = 0;
  UINT32 RFMB                            = 0;
  UINT32 RFMS                            = 0;
  UINT32 RFAB                            = 0;
  UINT32 RFAS                            = 0;
  UINT32 SmemSize                        = 0;

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

  if (!EFI_ERROR (LocateMemoryMapAreaByName ("MPSS_EFS", &MPSSEFSRegion))) {
    RMTB = MPSSEFSRegion.Address;
    RMTX = MPSSEFSRegion.Length;
  }

  if (!EFI_ERROR (LocateMemoryMapAreaByName ("ADSP_EFS", &ADSPEFSRegion))) {
    RFMB = (UINT32)ADSPEFSRegion.Address + (UINT32)ADSPEFSRegion.Length / 2;
    RFMS = (UINT32)ADSPEFSRegion.Length / 2;
    RFAB = (UINT32)ADSPEFSRegion.Address;
    RFAS = (UINT32)ADSPEFSRegion.Length / 2;
  }

  UpdateNameAslCode (SIGNATURE_32('S', 'O', 'I', 'D'), &SOID, 4);
  UpdateNameAslCode (SIGNATURE_32('S', 'I', 'D', 'V'), &SIDV, 4);
  UpdateNameAslCode (SIGNATURE_32('S', 'V', 'M', 'J'), &SVMJ, 2);
  UpdateNameAslCode (SIGNATURE_32('S', 'V', 'M', 'I'), &SVMI, 2);
  UpdateNameAslCode (SIGNATURE_32('S', 'D', 'F', 'E'), &SDFE, 2);
  UpdateNameAslCode (SIGNATURE_32('S', 'I', 'D', 'M'), &SIDM, 2);
  UpdateNameAslCode (SIGNATURE_32('S', 'O', 'S', 'N'), &SOSN, 8);
  UpdateNameAslCode (SIGNATURE_32('P', 'L', 'S', 'T'), &PLST, 4);
  UpdateNameAslCode (SIGNATURE_32('R', 'M', 'T', 'B'), &RMTB, 4);
  UpdateNameAslCode (SIGNATURE_32('R', 'M', 'T', 'X'), &RMTX, 4);
  UpdateNameAslCode (SIGNATURE_32('R', 'F', 'M', 'B'), &RFMB, 4);
  UpdateNameAslCode (SIGNATURE_32('R', 'F', 'M', 'S'), &RFMS, 4);
  UpdateNameAslCode (SIGNATURE_32('R', 'F', 'A', 'B'), &RFAB, 4);
  UpdateNameAslCode (SIGNATURE_32('R', 'F', 'A', 'S'), &RFAS, 4);
  UpdateNameAslCode (SIGNATURE_32('S', 'O', 'S', 'I'), &SOSI, 8);
  UpdateNameAslCode (SIGNATURE_32('S', 'I', 'D', 'S'), &SIDS, EFICHIPINFO_MAX_ID_LENGTH);
}