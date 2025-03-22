#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/RFSProtectionLib.h>

#include <Protocol/EFIScm.h>
#include <Protocol/scm_sip_interface.h>

#define MAX_DESTINATION_VMS 3

EFI_STATUS
EFIAPI
RFSProtectSharedArea (
  UINT64 EfsBaseAddr,
  UINT64 EfsSizeAddr)
{
  EFI_STATUS             Status                                 = EFI_SUCCESS;
  hyp_memprot_ipa_info_t IpaInfo;
  QCOM_SCM_PROTOCOL     *mScmProtocol                           = NULL;
  UINT64                 ParameterArray[SCM_MAX_NUM_PARAMETERS] = {0};
  UINT64                 Results[SCM_MAX_NUM_RESULTS]           = {0};
  UINT32                 DataSize                               = 0;
  UINT32                 SourceVM                               = AC_VM_HLOS;
  VOID                  *Data                                   = NULL;
  hyp_memprot_assign_t  *Assign                                 = (hyp_memprot_assign_t *)ParameterArray;

  //
  // Allow both Windows / Linux and Modem Subsystem to Access the Shared Memory Region.
  // This is needed otherwise the Modem Subsystem will Crash when Attempting to Read Data.
  //
  hyp_memprot_dstVM_perm_info_t dstVM_perm_info[MAX_DESTINATION_VMS] = {
    {
      AC_VM_HLOS, 
      (VM_PERM_R | VM_PERM_W), 
      (UINT64)NULL, 
      0
    },
    {
      AC_VM_MSS_MSA, 
      (VM_PERM_R | VM_PERM_W), 
      (UINT64)NULL, 
      0
    },
    {
      AC_VM_MSS_NAV,
      (VM_PERM_R | VM_PERM_W),
      (UINT64)NULL,
      0
    }
  };

  // Locate SCM Protocol
  Status = gBS->LocateProtocol (&gQcomScmProtocolGuid, NULL, (VOID *)&mScmProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate SCM Protocol! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Fill the Address Details
  IpaInfo.IPAaddr = EfsBaseAddr;
  IpaInfo.IPAsize = EfsSizeAddr;

  DataSize = sizeof(hyp_memprot_ipa_info_t) + sizeof(SourceVM) + (MAX_DESTINATION_VMS * sizeof(hyp_memprot_dstVM_perm_info_t)) +  4;

  Data = AllocateZeroPool (DataSize);
  if (Data == NULL) {
    DEBUG ((EFI_D_ERROR, "%a: Not Enough Memory!\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  Assign->IPAinfolist = (UINT64)Data;

  CopyMem ((VOID *)Assign->IPAinfolist, &IpaInfo, sizeof(hyp_memprot_ipa_info_t));

  Assign->IPAinfolistsize = sizeof(hyp_memprot_ipa_info_t);
  Assign->sourceVMlist = (UINT64)Data + sizeof(hyp_memprot_ipa_info_t);

  CopyMem ((VOID *)Assign->sourceVMlist, &SourceVM, sizeof(SourceVM));

  Assign->srcVMlistsize = sizeof(SourceVM);
  Assign->destVMlist = (UINT64)Data + sizeof(hyp_memprot_ipa_info_t) + sizeof(SourceVM) + 4;

  CopyMem ((VOID *)Assign->destVMlist, dstVM_perm_info, MAX_DESTINATION_VMS * sizeof(hyp_memprot_dstVM_perm_info_t));

  Assign->destVMlistsize = MAX_DESTINATION_VMS * sizeof(hyp_memprot_dstVM_perm_info_t);
  Assign->spare          = 0;

  // Send Hypervisor Call
  Status = mScmProtocol->ScmSipSysCall (mScmProtocol, HYP_MEM_PROTECT_ASSIGN, HYP_MEM_PROTECT_ASSIGN_PARAM_ID, ParameterArray, Results);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Send the Hypervisor Call! Status = %r\n", __FUNCTION__, Status));
  }

  return Status;
}

EFI_STATUS
EFIAPI
RFSLocateAndProtectSharedArea ()
{
  ARM_MEMORY_REGION_DESCRIPTOR_EX MpssEfs;

  if (!EFI_ERROR (LocateMemoryMapAreaByName ("MPSS_EFS", &MpssEfs))) {
    return RFSProtectSharedArea(MpssEfs.Address, MpssEfs.Length);
  }

  return EFI_NOT_FOUND;
}
