#ifndef _EFI_DISPLAY_PWR_EX_H_
#define _EFI_DISPLAY_PWR_EX_H_

//
// Global GUID for the Display Power State Ex Protocol
//
#define EFI_DISPLAY_PWR_EX_PROTOCOL_GUID { 0x7BFA4293, 0x7AA4, 0x4375, { 0xB6, 0x3C, 0xB6, 0xAA, 0xB7, 0x86, 0xC4, 0x3C } }

//
// Declare forward reference to the Display Power State Ex Protocol
//
typedef struct _EFI_DISPLAY_PWR_EX_PROTOCOL EFI_DISPLAY_PWR_EX_PROTOCOL;

//
// Display Types
//
#define DISPLAY_TYPE_PRIMARY   0
#define DISPLAY_TYPE_SECONDARY 1

//
// Display Power States
//
#define DISPLAY_POWER_STATE_OFF 0
#define DISPLAY_POWER_STATE_ON  1

/**
  This Function sets the defined Power State for the Display.

  @param[in] DisplayType                   - The Display Type.
  @param[in] PowerState                    - The New Power State for the Display.

  @return EFI_SUCCESS                      - Successfully set Display Power State.
  @return EFI_INVALID_PARAMETER            - One or all Paramters are Invalid.
  @return EFI_UNSUPPORTED                  - The Display does not Support this Function.
  @return EFI_DEVICE_ERROR                 - MDP returned a Error.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_DISPLAY_POWER_STATE) (
  IN INT32  DisplayType,
  IN UINT32 PowerState
  );

/**
  This Function gets the Current Display Power State.

  @param[in] DisplayType                   - The Display Type.
  @param[out] PowerState                   - The Current Display Power State.

  @return EFI_SUCCESS                      - Successfully got the Current Display Power State.
  @return EFI_UNSUPPORTED                  - ?
  @return EFI_DEVICE_ERROR                 - MDP returned a Error.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_GET_DISPLAY_POWER_STATE) (
  IN  INT32    DisplayType,
  OUT UINT32 *PowerState
  );

/**
  This Function sets the Brightness Level for the defined Display.

  @param[in] DisplayType                   - The Display Type.
  @param[in] Level                         - The Brightness Level. (0 - 100)

  @return EFI_SUCCESS                      - Successfully set Brightness Level.
  @return EFI_UNSUPPORTED                  - The Display Type is Unsupported.
  @return EFI_INVALID_PARAMETER            - The Brightness Level is Invalid.
  @return EFI_DEVICE_ERROR                 - MDP returned a Error.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_DISPLAY_BRIGHTNESS_LEVEL) (
  IN INT32  DisplayType,
  IN UINT32 Level
  );

/**
  This Function gets the Current Brightness Level of defined Display.

  @param[in]  DisplayType                  - The Display Type.
  @param[out] Level                        - The Current Brightness Level.

  @return EFI_SUCCESS                      - Succesfully got the current Brightness Level.
  @return EFI_UNSUPPORTED                  - The Display Type is Unsupported.
  @return EFI_DEVICE_ERROR                 - MDP returned a Error.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_GET_DISPLAY_BRIGHTNESS_LEVEL) (
  IN  INT32   DisplayType,
  OUT UINT32 *Level
  );

//
// Define Protocol Functions
//
struct _EFI_DISPLAY_PWR_EX_PROTOCOL {
  UINT32                           Revision;
  EFI_SET_DISPLAY_POWER_STATE      SetPowerState;
  EFI_GET_DISPLAY_POWER_STATE      GetPowerState;
  EFI_SET_DISPLAY_BRIGHTNESS_LEVEL SetBrightnessLevel;
  EFI_GET_DISPLAY_BRIGHTNESS_LEVEL GetBrightnessLevel;
};

extern EFI_GUID gEfiDisplayPowerStateExProtocolGuid;

#endif /* _EFI_DISPLAY_PWR_EX_H_ */
