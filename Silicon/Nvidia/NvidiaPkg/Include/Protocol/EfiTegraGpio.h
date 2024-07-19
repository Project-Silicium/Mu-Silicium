#ifndef _EFI_TEGRA_GPIO_H_
#define _EFI_TEGRA_GPIO_H_

//
// Global GUID for the Tegra Gpio Protocol
//
#define EFI_TEGRA_GPIO_PROTOCOL_GUID { 0xF5A3B253, 0x587D, 0x4EE5, { 0xA3, 0x4A, 0xF4, 0x86, 0x63, 0x1D, 0x9F, 0x2E } }

//
// Declare forward reference to the Tegra Gpio Protocol
//
typedef struct _EFI_TEGRA_GPIO_PROTOCOL EFI_TEGRA_GPIO_PROTOCOL;

//
// Tegra Gpio Ports
//
#define TEGRA_GPIO_PORT_A  0
#define TEGRA_GPIO_PORT_B  1
#define TEGRA_GPIO_PORT_C  2
#define TEGRA_GPIO_PORT_D  3
#define TEGRA_GPIO_PORT_E  4
#define TEGRA_GPIO_PORT_F  5
#define TEGRA_GPIO_PORT_G  6
#define TEGRA_GPIO_PORT_H  7
#define TEGRA_GPIO_PORT_I  8
#define TEGRA_GPIO_PORT_J  9
#define TEGRA_GPIO_PORT_K  10
#define TEGRA_GPIO_PORT_L  11
#define TEGRA_GPIO_PORT_M  12
#define TEGRA_GPIO_PORT_N  13
#define TEGRA_GPIO_PORT_O  14
#define TEGRA_GPIO_PORT_P  15
#define TEGRA_GPIO_PORT_Q  16
#define TEGRA_GPIO_PORT_R  17
#define TEGRA_GPIO_PORT_S  18
#define TEGRA_GPIO_PORT_T  19
#define TEGRA_GPIO_PORT_U  20
#define TEGRA_GPIO_PORT_V  21
#define TEGRA_GPIO_PORT_W  22
#define TEGRA_GPIO_PORT_X  23
#define TEGRA_GPIO_PORT_Y  24
#define TEGRA_GPIO_PORT_Z  25
#define TEGRA_GPIO_PORT_AA 26
#define TEGRA_GPIO_PORT_BB 27
#define TEGRA_GPIO_PORT_CC 28
#define TEGRA_GPIO_PORT_DD 29
#define TEGRA_GPIO_PORT_EE 30
#define TEGRA_GPIO_PORT_FF 31

//
// Small Code to Get GPIO as Number
//
#define TEGRA_GPIO(port, offset) ((TEGRA_GPIO_PORT_##port * 8) + offset)

/**
  This Function Gets the Current Configuration of the Defined GPIO.
  
  @param[in]  Gpio        The Gpio.
  
  @return Type            The Configuration Type.
**/
typedef
INT32
(EFIAPI *EFI_GET_CONFIG) (
  IN UINTN Gpio
  );

/**
  This Function Sets the Defined Configuration to the defined GPIO.

  @param[in]  Gpio        The Gpio.
  @param[in]  Type        The Configuration.

  @return EFI_SUCCESS The Configuration was set Successfully.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_CONFIG) (
  IN UINTN Gpio,
  IN INT32 Type
  );

/**
  This Function Gets the Current Direction of the Defined GPIO.

  @param[in]  Gpio        The Gpio.

  @return Direction       The Direction.
**/
typedef
INT32
(EFIAPI *EFI_GET_DIRECTION) (
  IN UINTN Gpio
  );

/**
  This Function Sets the Defined Direction to the Defined GPIO.

  @param[in]  Gpio        The Gpio.
  @param[in]  Direction   The Direction.

  @return EFI_SUCCESS     The Direction was set Successfully.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_DIRECTION) (
  IN UINTN Gpio,
  IN INT32 Direction
  );

/**
  This Function Sets the Level of the Defined GPIO.

  @param[in]  Gpio        The Gpio.
  @param[in]  High        The Level, TRUE if High, FALSE if Low.
**/
typedef
VOID
(EFIAPI *EFI_SET_LEVEL) (
  IN UINTN   Gpio,
  IN BOOLEAN High
  );

/**
  This Function gets the Current State of the defined GPIO.

  @param[in]  Gpio        The Gpio.

  @return State           The Current State.
**/
typedef
INT32
(EFIAPI *EFI_GET_STATE) (
  IN UINTN Gpio
  );

//
// Define Protocol Functions
//
struct _EFI_TEGRA_GPIO_PROTOCOL {
  EFI_GET_CONFIG    GetConfig;
  EFI_SET_CONFIG    SetConfig;
  EFI_GET_DIRECTION GetDirection;
  EFI_SET_DIRECTION SetDirection;
  EFI_SET_LEVEL     SetLevel;
  EFI_GET_STATE     GetState;
};

extern EFI_GUID gEfiTegraGpioProtocolGuid;

#endif /* _TEGRA_GPIO_H_ */