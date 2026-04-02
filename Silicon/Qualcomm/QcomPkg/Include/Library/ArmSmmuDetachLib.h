#ifndef _ARM_SMMU_DETACH_LIB_H_
#define _ARM_SMMU_DETACH_LIB_H_

/**
  This Function Detaches all IOMMU Domains from the SMMU.

  @param[in] SkipStreams                   - The SIDs to be Skiped.
  @param[in] SkipStreamCount               - The Number of SIDs to be Skiped.
**/
VOID
ArmSmmuDetach (
  IN CONST UINT16 *SkipStreams,
  IN UINT8         SkipStreamCount
  );

#endif /* _ARM_SMMU_DETACH_LIB_H_ */
