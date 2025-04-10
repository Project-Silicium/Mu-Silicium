#ifndef _EFI_CCIC_H_
#define _EFI_CCIC_H_

//
// Global GUID for the Ccic Protocol
//
#define EFI_CCIC_PROTOCOL_GUID { 0xBFAB1BE9, 0x653F, 0x4624, { 0xAE, 0x2D, 0x7B, 0x7F, 0x00, 0xCC, 0x91, 0xF3 } }

//
// Declare forward reference to the Ccic Protocol
//
typedef struct _EFI_CCIC_PROTOCOL EFI_CCIC_PROTOCOL;

/**
  This Function reads ADC.

  @param[out] Value                        - The Output from ADC Read.

  @return EFI_SUCCESS                      - Successfully Read ADC.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_READ_ADC_1) (
  OUT UINT32 *Value
  );

/**
  This Function reads ADC.

  @param[out] Value                        - The Output from ADC Read.

  @return EFI_SUCCESS                      - Successfully Read ADC.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_READ_ADC_2) (
  OUT UINT32 *Value
  );

/**
  This Function reads ADC.

  @param[out] Value                        - The Output from ADC Read.

  @return EFI_SUCCESS                      - Successfully Read ADC.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_READ_ADC_3) (
  OUT UINT32 *Value
  );

//
// Define Protocol Functions
//
struct _EFI_CCIC_PROTOCOL {
  // Same Name but can Give different Outputs.
  EFI_READ_ADC_1 ReadAdc1;
  EFI_READ_ADC_2 ReadAdc2;
  EFI_READ_ADC_3 ReadAdc3;
};

extern EFI_GUID gEfiCcicProtocolGuid;

#endif /* _EFI_CCIC_H_ */
