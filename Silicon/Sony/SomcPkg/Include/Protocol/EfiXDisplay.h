#ifndef _EFI_X_DISPLAY_H_
#define _EFI_X_DISPLAY_H_

//
// Global GUID for the XDisplay Protocol
//
#define EFI_X_DISPLAY_PROTOCOL_GUID { 0xB78B4A4C, 0xE092, 0x47C5, { 0xAA, 0xF6, 0xBF, 0x03, 0x74, 0xDA, 0x3E, 0xF7 } } 

//
// Splash Types
//
#define SPLASH_SONY_LOGO                  1
#define SPLASH_CHARGING                   2
#define SPLASH_NO_CHARGING                3
#define SPLASH_VERIFIED_BOOT_ORANGE_STATE 4
#define SPLASH_VERIFIED_BOOT_RED_STATE    5
#define SPLASH_XCS_LOCK                   6

//
// Declare forward reference to the XDisplay Protocol
//
typedef struct _EFI_X_DISPLAY_PROTOCOL EFI_X_DISPLAY_PROTOCOL;

/**
  This Function Inits the Primary Display.

  @param[in] DeviceTreeAddr  The Device Tree Address for XDisplay.

  @return    Status          The EFI_STATUS Returned by the Function.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_DISPLAY_INIT) (
  IN UINT8 DeviceTreeAddr
  );

/**
  This Function Draws The Defined Splash Type on the Display.

  @param[in] SplashType      The Splash Type to Display.

  @return    Status          The EFI_STATUS Returned by the Function.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_DISPLAY_DRAW) (
  IN UINTN SplashType
  );

/**
  This Function Turns off the Display.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_DISPLAY_OFF) (VOID);

//
// Define Protocol Functions
//
struct _EFI_X_DISPLAY_PROTOCOL {
  UINT64            Revision;
  EFI_DISPLAY_INIT  DisplayInit;
  VOID             *UnknownFunc1;
  EFI_DISPLAY_DRAW  DisplayDraw;
  EFI_DISPLAY_OFF   DisplayOff;

  // NOTE: There are way more Functions.
};

extern EFI_GUID gXDisplayProtocolGuid;

#endif /* _EFI_X_DISPLAY_H_ */