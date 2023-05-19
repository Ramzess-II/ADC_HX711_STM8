#include "DS18B20.h" 

struct DS18B20_union {
  uint8_t read_ok :1;
  uint8_t write_ok :1;
};

struct DS18B20_union DS18B20_flag = {0, 0};

void read_DS18B20 (void) {
  if (DS18B20_flag.read_ok) {
  
  }
}

void write_DS18B20 (void) {
  if (DS18B20_flag.write_ok) {
  
  }
}

void start_send_DS18B20 (void) {

}