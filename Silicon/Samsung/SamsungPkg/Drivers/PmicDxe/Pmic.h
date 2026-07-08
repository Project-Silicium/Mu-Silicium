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
// PMIC Set WTSR Functions
// =========================================================================

EFI_STATUS
S2mps18SetWtsr (
  IN BOOLEAN Enable
  );

EFI_STATUS
S2mps19SetWtsr (
  IN BOOLEAN Enable
  );

// =========================================================================
// PMIC Set SMPL Functions
// =========================================================================

EFI_STATUS
S2mps18SetSmpl (
  IN BOOLEAN Enable
  );

EFI_STATUS
S2mps19SetSmpl (
  IN BOOLEAN Enable
  );

// =========================================================================
// PMIC Power Down Functions
// =========================================================================

EFI_STATUS
S2mps22PowerDown ();

// =========================================================================
// PMIC Regulator Functions
// =========================================================================

EFI_STATUS
S2mps18SetLdo (
  IN UINT8             LdoNumber,
  IN EFI_PMIC_LDO_MODE Mode,
  IN BOOLEAN           Enable
  );

EFI_STATUS
S2mps19SetLdo (
  IN UINT8             LdoNumber,
  IN EFI_PMIC_LDO_MODE Mode,
  IN BOOLEAN           Enable
  );

EFI_STATUS
S2mps22SetLdo (
  IN UINT8             LdoNumber,
  IN EFI_PMIC_LDO_MODE Mode,
  IN BOOLEAN           Enable
  );

// =========================================================================
// PMIC Regulator Function Pointers
// =========================================================================

typedef
EFI_STATUS
(EFIAPI *EFI_PMIC_REGULATOR_SET_BUCK) (
  IN UINT8   BuckNumber,
  IN BOOLEAN Enable
  );

typedef
EFI_STATUS
(EFIAPI *EFI_PMIC_REGULATOR_SET_LDO) (
  IN UINT8             LdoNumber,
  IN EFI_PMIC_LDO_MODE Mode,
  IN BOOLEAN           Enable
  );

// =========================================================================
// PMIC Extra Function Pointers
// =========================================================================

typedef
EFI_STATUS
(EFIAPI *EFI_PMIC_EXTRA_SET_WTSR) (
  IN BOOLEAN Enable
  );

typedef
EFI_STATUS
(EFIAPI *EFI_PMIC_EXTRA_SET_SMPL) (
  IN BOOLEAN Enable
  );

typedef
EFI_STATUS
(EFIAPI *EFI_PMIC_EXTRA_POWER_DOWN) ();

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
// PMIC Regulator List
// =========================================================================

struct {
  EFI_PMIC_ID                 Id;
  EFI_PMIC_REGULATOR_SET_BUCK SetBuck;
  EFI_PMIC_REGULATOR_SET_LDO  SetLdo;
} SupportedPmicRegulator[] = {
  {
    .Id      = ID_S2MPS18,
    .SetBuck = NULL,
    .SetLdo  = S2mps18SetLdo,
  },
  {
    .Id      = ID_S2MPS19,
    .SetBuck = NULL,
    .SetLdo  = S2mps19SetLdo,
  },
  {
    .Id      = ID_S2MPS22,
    .SetBuck = NULL,
    .SetLdo  = S2mps22SetLdo,
  }
};

// =========================================================================
// PMIC Extra List
// =========================================================================

struct {
  EFI_PMIC_ID               Id;
  EFI_PMIC_EXTRA_SET_WTSR   SetWtsr;
  EFI_PMIC_EXTRA_SET_SMPL   SetSmpl;
  EFI_PMIC_EXTRA_POWER_DOWN PowerDown;
} SupportedPmicExtra[] = {
  {
    .Id        = ID_S2MPS18,
    .SetWtsr   = S2mps18SetWtsr,
    .SetSmpl   = S2mps18SetSmpl,
    .PowerDown = NULL
  },
  {
    .Id        = ID_S2MPS19,
    .SetWtsr   = S2mps19SetWtsr,
    .SetSmpl   = S2mps19SetSmpl,
    .PowerDown = NULL
  },
  {
    .Id        = ID_S2MPS22,
    .SetWtsr   = NULL,
    .SetSmpl   = NULL,
    .PowerDown = S2mps22PowerDown
  }
};

// =========================================================================
// PMIC Init List
// =========================================================================

struct {
  EFI_PMIC_ID          Id;
  EFI_SPEEDY_PMIC_INIT SpeedyInit;
  EFI_HSI2C_PMIC_INIT  HsI2cInit;
} SupportedPmic[] = {
  {
    .Id         = ID_S2MPS18,
    .SpeedyInit = InitS2mps18,
    .HsI2cInit  = NULL
  },
  {
    .Id         = ID_S2MPS19,
    .SpeedyInit = InitS2mps19,
    .HsI2cInit  = NULL
  },
  {
    .Id         = ID_S2MPS22,
    .SpeedyInit = InitS2mps22,
    .HsI2cInit  = NULL
  }
};

#endif /* _PMIC_H_ */
