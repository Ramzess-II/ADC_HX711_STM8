#ifndef __MAIN_H
#define __MAIN_H

#define  TIM_UART_SEND   100       // частота отправки по юарту
#define  MIN_FREQ        5000      // минимальная частота пиканья 
#define  MAX_FREQ        500       // freq buzer
#define  DEVIDER_MASS    3         // вес разделить на DEVIDER_MASS для начала пиканья 

#include <iostm8s103f3.h>
#include <stdint.h>               // либа для использования uint8_t 
#include <stdlib.h>               // for abs 

#include "init_stm.h"
#include "ADC_HX711.h" 
#include "DS18B20.h" 
#include "delay.h"
#include "Uart.h"

void adc_set_pulse (uint8_t pulse);
void set_buzer (uint8_t set_qualiti, uint32_t set_duty);
void delay_ms (uint32_t time);
void transmit_uart (void);
int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max);
void check_beep (void);

#endif /* __MAIN_H */