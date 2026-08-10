#ifndef _XBL_HOB_H_
#define _XBL_HOB_H_

//
// XBL HOB Details
//
typedef struct {
  EFI_PHYSICAL_ADDRESS *Address;
  CONST EFI_GUID       *ExpectedGuid;
  UINT8                 AdrpGuidOffset;
  UINT8                 AdrpAddrOffset;
  UINT8                 AddGuidOffset;
  UINT8                 AddAddrOffset;
} EFI_XBL_HOB_DETAILS;

#endif /* _XBL_HOB_H_ */
