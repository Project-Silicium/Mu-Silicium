#ifndef _GPIO_BUTTONS_H_
#define _GPIO_BUTTONS_H_

typedef enum {
  NoButtons = 0,
  VolUpButton = 1,
  VolDownButton = 2
} BUTTON_STATE;

typedef struct {
  MS_BUTTON_SERVICES_PROTOCOL ButtonServicesProtocol;
  BUTTON_STATE                ButtonState;
} GPIO_BUTTON_SERVICES_PROTOCOL;

GPIO_BUTTON_SERVICES_PROTOCOL *gBsp;

#define MS_BSP_FROM_BSP(a)  \
  BASE_CR(a, GPIO_BUTTON_SERVICES_PROTOCOL, ButtonServicesProtocol)

#endif /* _GPIO_BUTTONS_H_ */
