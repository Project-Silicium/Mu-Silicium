#ifndef _GPIO_BANK_LIB_H_
#define _GPIO_BANK_LIB_H_

//
// GPIO Bank IDs
//
enum {
  GPIO_BANK_ID_A,
  GPIO_BANK_ID_B,
  GPIO_BANK_ID_C,
  GPIO_BANK_ID_D,
  GPIO_BANK_ID_E,
  GPIO_BANK_ID_F,
  GPIO_BANK_ID_G,
  GPIO_BANK_ID_H,
  GPIO_BANK_ID_I,
  GPIO_BANK_ID_J,
  GPIO_BANK_ID_K,
  GPIO_BANK_ID_L,
  GPIO_BANK_ID_M,
  GPIO_BANK_ID_N,
  GPIO_BANK_ID_O,
  GPIO_BANK_ID_P,
  GPIO_BANK_ID_Q,
  GPIO_BANK_ID_R,
  GPIO_BANK_ID_S,
  GPIO_BANK_ID_T,
  GPIO_BANK_ID_U,
  GPIO_BANK_ID_V,
  GPIO_BANK_ID_W,
  GPIO_BANK_ID_X,
  GPIO_BANK_ID_Y,
  GPIO_BANK_ID_Z
};

//
// GPIO Bank
//
typedef struct {
  UINT8                Number;
  UINT16               Offset;
  EFI_PHYSICAL_ADDRESS Address;
} EFI_GPIO_BANK_VALUES;

//
// GPIO Bank Data
//
typedef struct {
  UINT8                Id;
  EFI_GPIO_BANK_VALUES Bank[50];
} EFI_GPIO_BANK_DATA;

//
// Functions
//
EFI_GPIO_BANK_DATA*
GetPlatformBankData ();

#endif /* _GPIO_BANK_LIB_H_ */
