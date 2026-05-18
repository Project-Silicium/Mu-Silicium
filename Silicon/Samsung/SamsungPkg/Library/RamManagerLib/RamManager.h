#ifndef _RAM_MANAGER_H_
#define _RAM_MANAGER_H_

EFI_STATUS
GetSecureDramRange (
  OUT EFI_PHYSICAL_ADDRESS *Address,
  OUT UINT64               *Length
  );

EFI_STATUS
GetSecurePgtblRange (
  OUT EFI_PHYSICAL_ADDRESS *Address,
  OUT UINT64               *Length
  );

EFI_STATUS
GetHarxRange (
  OUT EFI_PHYSICAL_ADDRESS *Address,
  OUT UINT64               *Length
  );

EFI_STATUS
GetDrmPluginRange (
  OUT EFI_PHYSICAL_ADDRESS *Address,
  OUT UINT64               *Length
  );

#endif /* _RAM_MANAGER_H_ */
