#ifndef __FLASH_H
#define __FLASH_H

#include <iostm8s103f3.h>
#include <stdint.h>               // library for uint8_t
#include "delay.h"

#define START_EEPROM_ADRESS       0x004000

void unlook_eeprom (void);
void data_eeprom_write (void);
float int_to_float ( int32_t dat);
int32_t float_to_int (float dat);
void devide_int (uint8_t *adr, int32_t data);
void data_eeprom_read (void);
int32_t make_int (uint8_t *adr);

struct flash_data {
  int32_t zero_adc;                  // код нуля АЦП
  float cal_coef;                    // калибровочный коэфициент
  int32_t weight_push;               // вес натяжения в граммах 
  int32_t max_weight;                // максимальный вес которым натянули резину  ( во время калибровки сбросить в ноль) читать со скоростью датчика и если был перегруз то переписать память             
  int32_t calib_massa;               // вес калибровки в граммах
};

#endif /* __FLASH_H */