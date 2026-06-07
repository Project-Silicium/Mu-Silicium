#ifndef _MTK_PMIC_WRAPPER_H_
#define _MTK_PMIC_WRAPPER_H_

//
// Declare forward Reference to the PMIC Wrapper Protocol
//
typedef struct _MTK_PMIC_WRAPPER_PROTOCOL MTK_PMIC_WRAPPER_PROTOCOL;

/**
  This Function Reads from PMIC Wrapper.

  @param[in]  Address                      - The Address.
  @param[out] Data                         - The Data.
**/
typedef
VOID
(EFIAPI *MTK_PMIC_WRAPPER_READ) (
  IN  UINT16  Address,
  OUT UINT16 *Value
  );

/**
  This Function Writes to PMIC Wrapper.

  @param[in] Addr                          - The Address.
  @param[in] Data                          - The Data.
**/
typedef
VOID
(EFIAPI *MTK_PMIC_WRAPPER_WRITE) (
  IN  UINT16 Address,
  IN  UINT16 Data
  );

//
// Define Protocol Functions
//
struct _MTK_PMIC_WRAPPER_PROTOCOL {
  MTK_PMIC_WRAPPER_READ  Read;
  MTK_PMIC_WRAPPER_WRITE Write;
};

extern EFI_GUID gMediaTekPmicWrapperProtocolGuid;

#endif /* _MTK_PMIC_WRAPPER_H_ */