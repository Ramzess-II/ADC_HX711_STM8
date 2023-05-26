#ifndef __DS18B20_H
#define __DS18B20_H

#include <iostm8s103f3.h>
#include <stdint.h>               // library for uint8_t 
#include "delay.h"

#define NOID 0xCC 
#define T_CONVERT 0x44
#define READ_DATA 0xBE

#define DS_PIN_OUT_ON   PD_ODR_ODR2 = 1;
#define DS_PIN_OUT_OFF  PD_ODR_ODR2 = 0;
#define DS_PIN_READ     PD_IDR_IDR3

/*#define DELAY_US(cnt) {        \
uint32_t delay = cnt;         \
asm __volatile__(            \
   "mov r5, #8;"              \
   "mul r6, %[us_cnt], r5;"   \
   "sub r6, #3;"              \
   "calc_us%=:"               \
     "subs r6, #1;"           \
     "bne calc_us%=;"         \
   ::                         \
    [us_cnt] "r" (delay)      \
   : "r5", "r6", "memory"     \
     ) }*/


 
void init_DS18B20_pin (void);
void init_sensor_DS18B20 (void);
uint8_t read_DS18B20 (void);
void write_DS18B20 (uint8_t data);
void start_conversion (void);
void read_temp (void);


#endif /* __DS18B20_H */
