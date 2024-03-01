#ifndef _EFI_RESETCFG_H_
#define _EFI_RESETCFG_H_

//
// Global GUID for the ResetCfg Protocol
//
#define EFI_RESETCFG_PROTOCOL_GUID { 0x157465CC, 0x023C, 0x4369, { 0x89, 0x9C, 0x4B, 0xF6, 0x24, 0x8E, 0x22, 0xA6 } }

//
// Declare forward reference to the ResetCfg Protocol
//
typedef struct _EFI_RESETCFG_PROTOCOL EFI_RESETCFG_PROTOCOL;

//
// Define Protocol Functions
//
struct _EFI_RESETCFG_PROTOCOL {
  UINT64          Revision;
  
  // TODO: Add Functions
};

extern EFI_GUID gResetCfgProtocolGuid;

#endif /* _EFI_RESETCFG_H_ */
