#ifndef __MAIN_H
#define __MAIN_H

#define  TIM_UART_SEND   100       // частота отправки по юарту
#define  TIM_18B20_READ  16000     // частота опроса датчика DS18B20
#define  MIN_FREQ        5000      // минимальная частота пиканья 
#define  MAX_FREQ        500       // freq buzer
#define  DEVIDER_MASS    3         // вес разделить на DEVIDER_MASS для начала пиканья 
#define  ERR_BLINK       250       // частота моргания светика при ошибке 

#include <iostm8s103f3.h>
#include <stdint.h>               // либа для использования uint8_t 
#include <stdlib.h>               // for abs 

#include "init_stm.h"
#include "delay.h"
#include "Uart.h"
#include "Flash.h"
#include "DS18B20.h"
#include "ADC_HX711.h" 

void adc_set_pulse (uint8_t pulse);
void set_buzer (uint8_t set_qualiti, uint32_t set_duty);
void delay_ms (uint32_t time);
void transmit_uart (void);
int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max);
void check_beep (void);
void blink_err_led (void);
void bt_set_AT_Comand (void);
void parsing_uart (void);

#endif /* __MAIN_H */



/*               Инфо 
пины для датчика DS18B20 PD2 и PD3 соеденены вместе, 
пины АЦП подключены к аппаратному SPI PC5 SCK и PC7 DOUT, 
пин светика PС4
пин бузера PD4
пины юарта аппараные PD5 TX и PD6 RX,
пин кнопки PC3 
если не находит датчик температуры или АЦП будет моргать светик 
*/
