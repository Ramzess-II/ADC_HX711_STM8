#ifndef __ADC_HX711_H
#define __ADC_HX711_H

#include <iostm8s103f3.h>
#include <stdint.h>               // library for uint8_t 

#define SCK_PIN_ON        PD_ODR_ODR2 = 1     // PD2
#define SCK_PIN_OFF       PD_ODR_ODR2 = 0     // PD2
#define SCK_PIN_READ      PD_IDR_IDR2         // PD2
#define READ_DOUT         PD_IDR_IDR3         // PD3

#define GAIN_ADC 26   // 25 chanel A128, 26 chanel B64, 27 chanel B32
#define true     1
#define false    0

void init_ADC_pin (void);
void read_data_ADC (void );
void stop_adc_read (uint8_t flag);


#endif /* __ADC_HX711_H */