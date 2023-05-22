#ifndef __DS18B20_H
#define __DS18B20_H

#include <iostm8s103f3.h>
#include <stdint.h>               // library for uint8_t 

#define true            1
#define false           0

#define DS_PIN_OUT_ON   PC_ODR_ODR7 = 1;
#define DS_PIN_OUT_OFF  PC_ODR_ODR7 = 0;
#define DS_PIN_READ     PC_IDR_IDR6
 
void init_DS18B20_pin (void);
void init_sensor_DS18B20 (void);
void read_DS18B20 (void);
void write_DS18B20 (void);


#endif /* __DS18B20_H */
