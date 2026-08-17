#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/RfsProtectionLib.h>
#include <Library/BaseMemoryLib.h>

#include <Protocol/EFIScm.h>
#include <Protocol/scm_sip_interface.h>

//
// Global Variables
//
STATIC EFI_MEMORY_REGION_DESCRIPTOR   MpssEfsRegion         = {0};
STATIC QCOM_SCM_PROTOCOL             *mScmProtocol          = NULL;
STATIC hyp_memprot_dstVM_perm_info_t *DestinationVmList     = NULL;
STATIC UINTN                          DestinationVmListSize = 0;

//
// Destination VM Info List
//
STATIC CONST hyp_memprot_dstVM_perm_info_t DestinationVmInfoList[3] = {
  {
    .dstVM     = AC_VM_HLOS, 
    .dstVMperm = (VM_PERM_R | VM_PERM_W)
  },
  {
    .dstVM     = AC_VM_MSS_MSA, 
    .dstVMperm = (VM_PERM_R | VM_PERM_W)
  },
  {
    .dstVM     = AC_VM_MSS_NAV,
    .dstVMperm = (VM_PERM_R | VM_PERM_W)
  }
};

EFI_STATUS
ProtectRfsSharedArea ()
{
  EFI_STATUS              Status                                 = EFI_SUCCESS;
  hyp_memprot_ipa_info_t  IpaInfo                                = {0};
  UINT64                  ParameterArray[SCM_MAX_NUM_PARAMETERS] = {0};
  UINT64                  Results[SCM_MAX_NUM_RESULTS]           = {0};
  VOID                   *Data                                   = NULL;

  // Verify Destination VM List
  if (DestinationVmList == NULL) {
    return EFI_NOT_READY;
  }

  // Set Assign Pointer
  hyp_memprot_assign_t *Assign = (hyp_memprot_assign_t *)ParameterArray;

  // Set Source VM
  UINT32 SourceVm = AC_VM_HLOS;

  // Allocate Assign Data
  Data = AllocateZeroPool (sizeof (hyp_memprot_ipa_info_t) + sizeof (SourceVm) + DestinationVmListSize + 4);
  if (Data == NULL) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Allocate Memory for the Assign Data!\n", __FUNCTION__));
    Status = EFI_OUT_OF_RESOURCES;
    goto exit;
  }

  // Set IPA Memory Range
  IpaInfo.IPAaddr = MpssEfsRegion.Address;
  IpaInfo.IPAsize = MpssEfsRegion.Length;

  // Set IPA Info Data
  Assign->IPAinfolist     = (UINT64)Data;
  Assign->IPAinfolistsize = sizeof (hyp_memprot_ipa_info_t);

  // Copy IPA Info Data
  CopyMem ((VOID *)Assign->IPAinfolist, &IpaInfo, Assign->IPAinfolistsize);

  // Set Source VM Data
  Assign->sourceVMlist  = (UINT64)Data + Assign->IPAinfolistsize;
  Assign->srcVMlistsize = sizeof (SourceVm);

  // Copy Source VM Data
  CopyMem ((VOID *)Assign->sourceVMlist, &SourceVm, Assign->srcVMlistsize);

  // Set Destination VM Data
  Assign->destVMlist     = (UINT64)Data + Assign->IPAinfolistsize + Assign->srcVMlistsize + 4;
  Assign->destVMlistsize = DestinationVmListSize;

  // Copy Destination VM Data
  CopyMem ((VOID *)Assign->destVMlist, DestinationVmList, DestinationVmListSize);

  // Send Hypervisor Call
  Status = mScmProtocol->ScmSipSysCall (mScmProtocol, HYP_MEM_PROTECT_ASSIGN, HYP_MEM_PROTECT_ASSIGN_PARAM_ID, ParameterArray, Results);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Modify RFS Protection! Status = %r\n", __FUNCTION__, Status));
  }

exit:
  // Free Destination VM List Buffer
  FreePool (DestinationVmList);

  // Free Data Buffer
  if (Data != NULL) {
    FreePool (Data);
  }

  return Status;
}

EFI_STATUS
EFIAPI
PrepareRfsProtection (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;
  UINT8      DestinationVmCount;

  // Locate SCM Protocol
  Status = gBS->LocateProtocol (&gQcomScmProtocolGuid, NULL, (VOID *)&mScmProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate SCM Protocol! Status = %r\n", __FUNCTION__, Status));
    return EFI_SUCCESS;
  }

  // Locate "MPSS EFS" Memory Region
  LocateMemoryRegionByName ("MPSS EFS", &MpssEfsRegion);
  LocateMemoryRegionByName ("MPSS_EFS", &MpssEfsRegion);

  // Verify Region Presense
  if (MpssEfsRegion.Address == 0 && MpssEfsRegion.Length == 0) {
    return EFI_SUCCESS;
  }

  // Get Number of Destination VMs
  DestinationVmCount = FixedPcdGet8 (PcdDestinationVmCount);
  if (DestinationVmCount == 0 || DestinationVmCount > 3) {
    return EFI_SUCCESS;
  }

  // Set Destination VM List Size
  DestinationVmListSize = sizeof (hyp_memprot_dstVM_perm_info_t) * DestinationVmCount;

  // Allocate Memory
  DestinationVmList = AllocateZeroPool (DestinationVmListSize);
  if (DestinationVmList == NULL) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Allocate Memory for the Destination VM List!\n", __FUNCTION__));
    return EFI_SUCCESS;
  }

  // Populate Destination VM List
  for (UINT8 i = 0; i < DestinationVmCount; i++) {
    DestinationVmList[i] = DestinationVmInfoList[i];
  }

  return EFI_SUCCESS;
}
