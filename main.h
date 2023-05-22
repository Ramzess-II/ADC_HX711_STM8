#ifndef __MAIN_H
#define __MAIN_H

#define true     1
#define false    0
#define DELAY_CYCLES(n) \
do { \
  unsigned int _delay_counter = (n); \
  for (; _delay_counter > 0; _delay_counter--) { \
    asm("nop"); \
  } \
} while(0)

#include <iostm8s103f3.h>
#include <stdint.h>               // library for uint8_t 

#include "init_stm.h"
#include "ADC_HX711.h" 
#include "DS18B20.h" 

void delay_us (uint8_t time);



#endif /* __MAIN_H */