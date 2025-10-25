#ifndef _MASS_STORAGE_H_
#define _MASS_STORAGE_H_

//
// UFS LUN Data
//
typedef struct {
  EFI_DEVICE_PATH_PROTOCOL *DevicePath;
  EFI_BLOCK_IO_PROTOCOL    *BlockIoProtocol;
} EFI_UFS_LUN_DATA;

#endif /* _MASS_STORAGE_H_ */
