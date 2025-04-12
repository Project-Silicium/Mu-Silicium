#ifndef _EFI_PMIC_H_
#define _EFI_PMIC_H_

//
// Global GUID for the PMIC Protocol
//
#define EFI_PMIC_PROTOCOL_GUID { 0x09723148, 0xBA1F, 0x4751, { 0x8F, 0x0D, 0x5B, 0xC6, 0xA0, 0xCC, 0xA2, 0x7A } }

//
// Declare forward Reference to the PMIC Protocol
//
typedef struct _EFI_PMIC_PROTOCOL EFI_PMIC_PROTOCOL;

/**
  This Function disables WTSR on the PMIC.
**/
typedef
VOID (EFIAPI *EFI_DISABLE_WTSR) ();

/**
  This Function disables SMPL on the PMIC.
**/
typedef
VOID (EFIAPI *EFI_DISABLE_SMPL) ();

/**
  This Function shuts the PMIC down.
**/
typedef
VOID (EFIAPI *EFI_SHUTDOWN_PMIC) ();

struct _EFI_PMIC_PROTOCOL {
  EFI_DISABLE_WTSR   DisableWTSR;
  EFI_DISABLE_SMPL   DisableSMPL;
  EFI_SHUTDOWN_PMIC  ShutdownPMIC;
};

extern EFI_GUID gEfiPMICProtocolGuid;

#endif /* _EFI_PMIC_H_ */
