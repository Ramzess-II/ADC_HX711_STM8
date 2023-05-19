#ifndef __DS18B20_H
#define __DS18B20_H


#include <iostm8s103f3.h>
#include <stdint.h>               // library for uint8_t 

void read_DS18B20 (void);
void write_DS18B20 (void);
void start_send_DS18B20 (void);


#endif /* __DS18B20_H */
