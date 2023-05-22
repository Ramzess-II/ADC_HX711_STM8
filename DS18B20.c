#include "DS18B20.h" 
#include "main.h"
#include "ADC_HX711.h" 

struct DS18B20_union {
  uint8_t presence :1;
};

struct DS18B20_union DS18B20_flag = {0};

void init_DS18B20_pin (void) {
  // OUT_DATA
  PC_DDR_DDR7 = 1;       // direction port 0 - input, 1 - output 
  PC_CR1_C17 = 0;        // control 0 - open drein, 1 - push-pull
  PC_CR2_C27 = 1;        // output speed 0 - 2MHz, 1 - 10MHz
  PC_ODR_ODR7 = 1;       // output data registr 
  // INPT_DATA
  PC_DDR_DDR6 = 0;       // direction port 0 - input, 1 - output
  PC_CR1_C16 = 0;        // control 0 - floating, 1 - Input with pull-up
  PC_CR2_C26 = 0;        // External interrupt enabled
}

void init_sensor_DS18B20 (void) {
  stop_adc_read(true);
  DS_PIN_OUT_OFF;
  delay_us(250);
  DS_PIN_OUT_ON;
  delay_us(80);
  if (DS_PIN_READ) {
    DS18B20_flag.presence = true;
  }
  stop_adc_read(false);
}

void read_DS18B20 (void) {
  
  
}

void write_DS18B20 (void) {
  
  
}