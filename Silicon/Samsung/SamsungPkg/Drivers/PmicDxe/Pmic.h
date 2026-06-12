#ifndef _PMIC_H_
#define _PMIC_H_

// =========================================================================
// PMIC Init Functions
// =========================================================================

EFI_STATUS
InitS2mps18 (
  IN EFI_SPEEDY_PROTOCOL *SpeedyProtocol,
  IN UINT8                BusNumber
  );

EFI_STATUS
InitS2mps19 (
  IN EFI_SPEEDY_PROTOCOL *SpeedyProtocol,
  IN UINT8                BusNumber
  );

EFI_STATUS
InitS2mps22 (
  IN EFI_SPEEDY_PROTOCOL *SpeedyProtocol,
  IN UINT8                BusNumber
  );

// =========================================================================
// PMIC Init Function Pointers
// =========================================================================

typedef
EFI_STATUS
(EFIAPI *EFI_SPEEDY_PMIC_INIT) (
  IN EFI_SPEEDY_PROTOCOL *SpeedyProtocol,
  IN UINT8                BusNumber
  );

typedef
EFI_STATUS
(EFIAPI *EFI_HSI2C_PMIC_INIT) (
  IN EFI_HSI2C_PROTOCOL *HsI2cProtocol,
  IN UINT8               BusNumber,
  IN UINT32              Slave
  );

// =========================================================================
// PMIC Init Lists
// =========================================================================

struct {
  EFI_PMIC_ID          Id;
  EFI_SPEEDY_PMIC_INIT Init;
} SpeedyPmic[MAX_SPEEDY_PMIC_COUNT] = {
  {
    .Id   = ID_S2MPS18,
    .Init = InitS2mps18
  },
  {
    .Id   = ID_S2MPS19,
    .Init = InitS2mps19
  },
  {
    .Id   = ID_S2MPS22,
    .Init = InitS2mps22
  }
};

struct {
  EFI_PMIC_ID         Id;
  EFI_HSI2C_PMIC_INIT Init;
} HsI2cPmic[MAX_HSI2C_PMIC_COUNT] = {
  {
    .Id   = 0,
    .Init = NULL
  }
};

#endif /* _PMIC_H_ */
