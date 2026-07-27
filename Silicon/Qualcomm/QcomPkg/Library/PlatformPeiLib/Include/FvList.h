#ifndef _FV_LIST_H_
#define _FV_LIST_H_

//
// FV List Limits
//
#define MAX_FV_ENTRIES                              8
#define FV_LIST_STRUCTURE_SIZE                      0x4A

//
// FV List Structure
//
#define FV_LIST_LOCK_HANDLE(ListAddr, Index)       (*(UINT64 *)((ListAddr) + 0x18 + ((Index) * 0x48)))

#endif /* _FV_LIST_H_ */
