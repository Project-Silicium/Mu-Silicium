#ifndef _EFI_VIB_H_
#define _EFI_VIB_H_

//
// Global GUID for the Vibrator Protocol
//
#define EFI_VIB_PROTOCOL_GUID { 0xAA2E776D, 0x8C9C, 0x4268, { 0x99, 0x8A, 0x8B, 0x0A, 0x13, 0xBF, 0x05, 0x01 } }

//
// Declare forward reference to the Vibrator Protocol
//
typedef struct _EFI_VIB_PROTOCOL EFI_VIB_PROTOCOL;

/**
  This Function inits the Device Vibrator.

  @param[in] Value1                        - Unknown.
  @param[in] Value2                        - Unknown.

  @return EFI_SUCCESS                      - Successfully Init Device Vibrator.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_VIBRATOR_INIT) (
  IN UINT16 Value1,
  IN UINT16 Value2
  );

/**
  This Function Enables/Disables the Device Vibrator.

  @param[in] Enable                        - The Enable Status.

  @return EFI_SUCCESS                      - Successfully Enabled/Disabled the Device Vibrator.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_VIBRATOR_STATE) (
  IN BOOLEAN Enable
  );

/**
  This Function gets Data about the Device Vibrator.

  @param[in]  Value                        - Unknown.
  @param[out] Data                         - The Device Vibrator Data.

  @return EFI_SUCCESS                      - Successfully got the Device Vibrator Data.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_GET_VIBRATOR_DATA) (
  IN  UINT32 Value, // Unused
  OUT UINT32 Data
  );

//
// Define Protocol Functions
//
struct _EFI_VIB_PROTOCOL {
  EFI_VIBRATOR_INIT      LateInit;
  EFI_SET_VIBRATOR_STATE SetState;
  EFI_GET_VIBRATOR_DATA  GetData;
};

extern EFI_GUID gEfiVibratorProtocolGuid;

#endif /* _EFI_VIB_H_ */
