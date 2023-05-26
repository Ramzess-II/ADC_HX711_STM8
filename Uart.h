#ifndef __UART_H
#define __UART_H

#include <iostm8s103f3.h>
#include <stdint.h>               // library for uint8_t
#include "delay.h"

void init_uarts (void);
void uart_transmit_data (uint8_t count_byte);


struct uart_union {
  uint8_t rx_data_ok :1;
  uint8_t tx_busy :1;
};

#endif /* __UART_H */
