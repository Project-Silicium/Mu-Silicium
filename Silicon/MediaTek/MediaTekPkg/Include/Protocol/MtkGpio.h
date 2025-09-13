#ifndef _MTK_GPIO_H_
#define _MTK_GPIO_H_

typedef struct {
    UINT32 SetOffset;
    UINT32 ResetOffset;
    UINT32 DirOffset;
    UINT32 DataOutOffset;
    UINT32 DataInOffset;
    UINT32 ModeOffset;
    UINT32 MaxPin;
} MTK_GPIO_PLATFORM_INFO;

//
// Declare forward Reference to the GPIO Protocol
//
typedef struct _MTK_GPIO_PROTOCOL MTK_GPIO_PROTOCOL;

/**
  This Function Gets direction of the defined pin.

  @param[in]  Pin                          - The Pin.
  @param[out] Direction                    - The Direction of the Pin.
**/
typedef
EFI_STATUS
(EFIAPI *MTK_GPIO_GET_DIR) (
  IN  UINT32   Pin,
  OUT BOOLEAN *Direction
  );

/**
  This Function Sets direction of the defined pin.

  @param[in] Pin                          - The Pin.
  @param[in] Direction                    - The Direction of the Pin.
**/
typedef
EFI_STATUS
(EFIAPI *MTK_GPIO_SET_DIR) (
  IN UINT32  Pin,
  IN BOOLEAN Direction
  );

/**
  This Function Gets state of the defined pin.

  @param[in]  Pin                          - The Pin.
  @param[out] State                        - The State of the Pin.
**/
typedef
EFI_STATUS
(EFIAPI *MTK_GPIO_GET_PIN) (
  IN  UINT32   Pin,
  OUT BOOLEAN *State
  );

/**
  This Function Sets state of the defined pin.

  @param[in] Pin                          - The Pin.
  @param[in] State                        - The State of the Pin.
**/
typedef
EFI_STATUS
(EFIAPI *MTK_GPIO_SET_PIN) (
  IN UINT32  Pin,
  IN BOOLEAN State
  );

/**
  This Function Sets mode of the defined pin.

  @param[in] Pin                          - The Pin.
  @param[in] Mode                         - The Mode of the Pin.
**/
typedef
EFI_STATUS
(EFIAPI *MTK_GPIO_SET_MODE) (
  IN UINT32 Pin,
  IN UINT32 Mode
  );

//
// Define Protocol Functions
//
struct _MTK_GPIO_PROTOCOL {
  MTK_GPIO_GET_DIR  GetDir;
  MTK_GPIO_SET_DIR  SetDir;
  MTK_GPIO_GET_PIN  GetPin;
  MTK_GPIO_SET_PIN  SetPin;
  MTK_GPIO_SET_MODE SetMode;
};

extern EFI_GUID gMediaTekGpioProtocolGuid;

#endif /* _MTK_GPIO_H_ */