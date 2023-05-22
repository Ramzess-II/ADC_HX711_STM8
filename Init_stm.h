#ifndef __INIT_STM_H
#define __INIT_STM_H

#include <iostm8s103f3.h>
#include <stdint.h>               // library for uint8_t 

void init_rcc (void) ;
void init_gpio (void);
void init_tim4 (void);
void init_tim2 (void);
void init_tim1 (void);
void beep_init (uint8_t div);


#endif /* __INIT_STM_H */