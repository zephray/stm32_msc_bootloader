#ifndef __KEY_H__
#define	__KEY_H__

#include "main.h"

#define KEY_ON	1
#define KEY_OFF	0

void Key_GPIO_Config(void);
u8 Key_Scan(GPIO_TypeDef* GPIOx,u16 GPIO_Pin);

#endif /* __LED_H */

