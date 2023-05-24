#include "DS18B20.h" 
#include "main.h"
#include "ADC_HX711.h" 

int8_t temperature; 

struct DS18B20_union {
  uint8_t presence :1;
};

struct DS18B20_union DS18B20_flag = {0};

void init_DS18B20_pin (void) {
  // OUT_DATA
  PD_DDR_DDR2 = 1;       // direction port 0 - input, 1 - output 
  PD_CR1_C12 = 0;        // control 0 - open drein, 1 - push-pull
  PD_CR2_C22 = 1;        // output speed 0 - 2MHz, 1 - 10MHz
  PD_ODR_ODR2 = 1;       // output data registr 
  // INPT_DATA
  PD_DDR_DDR3 = 0;       // direction port 0 - input, 1 - output
  PD_CR1_C13 = 0;        // control 0 - floating, 1 - Input with pull-up
  PD_CR2_C23 = 0;        // External interrupt enabled
}

void init_sensor_DS18B20 (void) {
  stop_adc_read(true);
  DS_PIN_OUT_OFF;
  DELAY_CYCLES(1248);
  DS_PIN_OUT_ON;
  DELAY_CYCLES(208);
  if (DS_PIN_READ) {
    DS18B20_flag.presence = true;
  }
  stop_adc_read(false);
  DELAY_CYCLES(1000);
}

uint8_t read_DS18B20 (void) {
  uint8_t data;
  for (int i = 0; i < 8; i++) {
        DS_PIN_OUT_OFF;
        DELAY_CYCLES(39);
        DS_PIN_OUT_ON;
        DELAY_CYCLES(2);
        data = data >> 1;
        if (DS_PIN_READ) {
         data  |= 0x80;
        }
        DELAY_CYCLES(120);
  }
  return data;
}

void write_DS18B20 (uint8_t data) {
  for (int i = 0; i < 8; i++) {
    if (data & 0x01){
        DS_PIN_OUT_OFF;
        DELAY_CYCLES(39);
        DS_PIN_OUT_ON;
        DELAY_CYCLES(117);
    } else {
        DS_PIN_OUT_OFF;
        DELAY_CYCLES(300);
        DS_PIN_OUT_ON;
        DELAY_CYCLES(2);
    }
    data >>= 1;
  }
}

void read_temp (void) {
  uint8_t lsb = 0;
  uint8_t msb = 0;
  init_sensor_DS18B20();
  write_DS18B20 (NOID);
  write_DS18B20 (READ_DATA);
  lsb = read_DS18B20();
  msb = read_DS18B20();
  temperature = (uint8_t) (lsb >> 4) | (msb << 4);
}

void start_conversion (void){
  init_sensor_DS18B20();
  write_DS18B20 (NOID);
  write_DS18B20 (T_CONVERT);
}

uint8_t DS18B20_crc (uint16_t adress)
{
	uint8_t crc = 0;			// ?????????? ??? ?????????? CRC
	for (uint8_t i=0; i<8; i++)	// ?????? CRC 8-?? ????, 9-? ???? ??? CRC
	{
		// ?????????? CRC ?????? ?????
		crc = crc ^ (*(uint16_t*)(adress+i));
		for (uint8_t j=0; j<8; j++)
		{
			if (crc & 0x01) crc = (crc >> 1) ^ 0x8C;
			else crc >>= 1;
		}
	}
	return crc;	// ????????? CRC
        
}        