#ifndef _ARM_SMMU_DETACH_LIB_H_
#define _ARM_SMMU_DETACH_LIB_H_

VOID
ArmSmmuDetach (
  IN CONST UINT16 *SkipStreams,
  IN UINT8         SkipStreamCount
  );

#endif /* _ARM_SMMU_DETACH_LIB_H_ */
