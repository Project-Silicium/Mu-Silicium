#ifndef __PLATFORM_UFS_LIB_H__
#define __PLATFORM_UFS_LIB_H__

#include <Library/UfsHostBridge.h>
#include <Library/UfsCalAdapterLib.h>

EFI_STATUS UfsBoardInit (struct UfsHost *Ufs);
UfsCalError UfsCalInit (struct UfsCalParam *p);
UfsCalError UfsCalPreLink (struct UfsCalParam *p);
UfsCalError UfsCalPostLink (struct UfsCalParam *p);
UfsCalError UfsCalPrePmc (struct UfsCalParam *p);
UfsCalError UfsCalPostPmc (struct UfsCalParam *p);
UINT8 UfsCalGetTargetBoard(VOID);

#endif /* __PLATFORM_UFS_LIB_H__ */
