#ifndef __ADC_HX711_H
#define __ADC_HX711_H

#include <iostm8s103f3.h>
#include <stdint.h>               // library for uint8_t 
#include "delay.h"
#include "Flash.h"

#define SCK_PIN_ON        PC_ODR_ODR5 = 1     // PC5
#define SCK_PIN_OFF       PC_ODR_ODR5 = 0     // PC5
#define READ_DOUT         PC_IDR_IDR7         // PC7

#define GAIN_ADC 26   // 25 chanel A128, 26 chanel B64, 27 chanel B32

void stop_adc_read (uint8_t flag);
void ADC_pin_init (void) ;
void ADC_pin_manual (void);
void ADC_pin_automat (void);
void init_SPI_first (void);
void init_SPI (void);
void deinit_SPI (void);
void zero_set (void);
void massa_display (void);
void new_kalib_koef (int32_t cal);

struct adc_union {
  uint8_t adc_work :1;
  uint8_t data_ok :1;
  uint8_t high_low_sck :1;
  uint8_t stop_adc_measurement :1;
};


#endif /* __ADC_HX711_H */