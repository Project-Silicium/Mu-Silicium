#ifndef _KBC_REGISTERS_H_
#define _KBC_REGISTERS_H_

// Repeat Delay
#define MS2NS(ms) (((UINT64)(ms)) * 1000000ULL)

// KBC Registers
#define KBC_MAX_KPENT   8
#define KBC_RPT_DLY_0   0x2C
#define KBC_INIT_DLY_0  0x28
#define KBC_KP_ENT0_0   0x30
#define KBC_INT_0       0x4

// Char Key Codes
#define CHAR_APOSTROPHE 0x0027

typedef struct {
  KEY_CONTEXT EfiKeyContext;
  UINTN       Gpio;
} KEY_CONTEXT_PRIVATE;

UINTN gBitmapScanCodes[BITMAP_NUM_WORDS(0x18)]    = {0};
UINTN gBitmapUnicodeChars[BITMAP_NUM_WORDS(0x7F)] = {0};

// Scan Code Key Map
STATIC UINT16 KeyMapScanCode[16][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {SCAN_F2, SCAN_F1, SCAN_ESC, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, SCAN_F11, SCAN_F10, 0, 0, 0, 0, SCAN_DOWN},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {SCAN_F7, SCAN_F6, SCAN_F5, 0, 0, 0, 0, 0},
  {SCAN_F8, SCAN_F9, 0, 0, 0, SCAN_LEFT, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, SCAN_UP},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, SCAN_DELETE, SCAN_PAGE_UP, SCAN_INSERT, SCAN_F12, SCAN_PAGE_DOWN, SCAN_RIGHT}
};

// Unicode Char Key Map
STATIC CHAR16 KeyMapUnicodeChar[16][8] = {
  {0, CHAR_TAB, '`', '1', 'q', 'a', 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, '3', 'e', 'd', 'c', ' '},
  {0, 0, 0, '2', 'w', 's', 'x', 'z'},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {'g', 't', '5', '4', 'r', 'f', 'v', 'b'},
  {'h', 'y', '6', '7', 'u', 'j', 'm', 'n'},
  {0, 0, 0, '9', 'o', 'l', '.', 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, '8', 'i', 'k', ',', 0},
  {0, 0, '\\', 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {'-', 0, '0', 'p', 0, ';', '/', 0},
  {0, 0, '=', CHAR_BACKSPACE, 0, 0, CHAR_APOSTROPHE, CHAR_CARRIAGE_RETURN},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

/*
// Unicode Char Key Map (Shift State)
STATIC CHAR16 KeyMapUnicodeCharShift[16][8] = {
  {0, CHAR_TAB, '~', '!', 'Q', 'A', 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, '#', 'E', 'D', 'C', ' '},
  {0, 0, 0, '@', 'W', 'S', 'X', 'Z'},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {'G', 'T', '%', '$', 'R', 'F', 'V', 'B'},
  {'H', 'Y', '^', '&', 'U', 'J', 'M', 'N'},
  {0, 0, 0, '(', 'O', 'L', '>', 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, '*', 'I', 'K', '<', 0},
  {0, 0, '|', 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {'_', 0, ')', 'P', 0, ':', '?', 0},
  {0, 0, '+', CHAR_BACKSPACE, 0, 0, '"', CHAR_CARRIAGE_RETURN},
  {0, 0, 0, 0, 0, 0, 0, 0}
};
*/

#endif /* _KBC_REGISTERS_H_ */
