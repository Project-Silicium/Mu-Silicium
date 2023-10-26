#ifndef _QCOM_GPIO_TLMM_H_
#define _QCOM_GPIO_TLMM_H_

//
// Global ID for the Gpa Protocol
//
#define QCOM_GPIO_TLMM_PROTOCOL_GUID { 0x8054947b, 0x3223, 0x407a, { 0xa1, 0xcc, 0x31, 0x22, 0x2f, 0x80, 0x66, 0x40 } }

//
// Declare forward reference to the Gpa Protocol
//
typedef struct _QCOM_GPIO_TLMM_PROTOCOL QCOM_GPIO_TLMM_PROTOCOL;

/**
  This Function Sets the Gpio Configuration to the defined Gpio.

  @param TlmmBaseAddr The Tlmm (Pinctrl) Base Address.
  @param Gpio         The Gpio.
  @param Func         The Function for the Gpio.
  @param Pull         The Pull Type for the Gpio.
  @param Drvstr       The Drvstr for the Gpio.
  @param Enable       The Value to enable the Gpio or Not.
**/
typedef 
VOID
(EFIAPI *EFI_SET_GPIO_CONFIG)(
  UINT32  TlmmBaseAddr,
  UINT32  Gpio,
  UINT8   Func,
  UINT8   Pull,
  UINT8   Drvstr,
  BOOLEAN Enable
  );

/**
  This Function Sets the Gpio State to the defined Gpio.

  @param TlmmBaseAddr The Tlmm (Pinctrl) Base Address.
  @param Gpio         The Gpio.
  @param Value        The Gpio State for the Gpio.
**/
typedef 
VOID
(EFIAPI *EFI_SET_GPIO)(
  UINT32  TlmmBaseAddr,
  UINT32  Gpio,
  UINT32  Value
  );

/**
  This Function Gets the Current Gpio State of the defined Gpio.

  @param TlmmBaseAddr The Tlmm (Pinctrl) Base Address.
  @param Gpio         The Gpio.

  @retval             Gpio State.
**/
typedef 
UINT32
(EFIAPI *EFI_GET_GPIO)(
  UINT32  TlmmBaseAddr,
  UINT32  Gpio
  );

//
// Define Protocol Functions
//
struct _QCOM_GPIO_TLMM_PROTOCOL {
  EFI_SET_GPIO_CONFIG      SetGpioConfig;
  EFI_SET_GPIO             SetGpio;
  EFI_GET_GPIO             GetGpio;
};

extern EFI_GUID gQcomGpioTlmmProtocolGuid;

#endif /* _QCOM_GPIO_TLMM_H_ */