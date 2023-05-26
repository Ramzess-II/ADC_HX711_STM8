#ifndef __INIT_STM_H
#define __INIT_STM_H

#include <iostm8s103f3.h>
#include <stdint.h>               // library for uint8_t
#include "delay.h"

#define PIN_BUTTON       PC_IDR_IDR3  // PC3
#define PIN_LED_ON       PC_ODR_ODR4 = 1  // PC4
#define PIN_LED_OFF      PC_ODR_ODR4 = 0

void init_rcc (void) ;
void init_gpio (void);
void init_tim4 (void);
void init_tim2 (void);
void init_tim1 (void);
void beep_init (uint8_t div);
void beep_set (uint8_t flag);

#endif /* __INIT_STM_H */