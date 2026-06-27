#ifndef _KEY_CALLBACK_H_
#define _KEY_CALLBACK_H_

//
// Key Notify Data
//
typedef struct {
  UINT8  ScanCode;
  VOID  *NotifyHandle;
} EFI_KEY_NOTIFY_DATA;

#endif /* _KEY_CALLBACK_H_ */
