#include "Uart.h"

volatile uint8_t uart_rx_buf [20];        // буфер приема
volatile uint8_t uart_tx_buf [20];        // буфер передачи
volatile uint8_t rx_count = 0;            // счетчик принятых байт
volatile uint8_t tx_count = 0;            // счетчик переданных байт
volatile uint8_t tx_count_send = 0;       // переменная для хранения сколько байт нужно передать
struct uart_union uart_flag = {0, 0};     // флаги юарта

void init_uarts (void) {  // инициализация пинов
    // PD5 TX
  PD_DDR_DDR5 = 1;       // direction port 0 - input, 1 - output 
  PD_CR1_C15 = 1;        // control 0 - open drein, 1 - push-pull
  //PD_CR2_C25 = 1;      // output speed 0 - 2MHz, 1 - 10MHz
  
    // PD6 RX
  PD_DDR_DDR6 = 0;       // direction port 0 - input, 1 - output 
  PD_CR1_C16 = 1;        // control 0 - floating, 1 - Input with pull-up
  //PD_CR2_C26 = 0;      // External interrupt enabled\
  
  UART1_BRR1 = 0x69;      // младший и старший байт баудрейта
  UART1_BRR2 = 0x03;      // baud
  UART1_CR2_TEN = 1;      // transmitter enable
  UART1_CR2_REN = 1;      // receive enable
  UART1_CR2_RIEN = 1;     // receiver interrupt 
  UART1_CR2_ILIEN = 1;    // idle interrupt enable 
}

void uart_transmit_data (uint8_t count_byte) {        // передача по юарту
  if (!uart_flag.tx_busy) {                           // проверим занятость юарта
    uart_flag.tx_busy = true;                         // установим занятость юарта
    tx_count_send = count_byte;                       // запишем сколько байт нужно передать 
    UART1_DR = uart_tx_buf [tx_count];                // передадим первую посылку
    tx_count ++;                                      // увеличим переменную передачи
    UART1_CR2_TIEN = 1;                               // включим прерывание по передаче
  }
}

#pragma vector=UART1_R_RXNE_vector                    // прерывание по приему
__interrupt void UART1_R_RXNE_Handler (void){         // функция приема
  uint8_t status = UART1_SR;             // считаем статусный регистр  
  if (status & 0x20) {                   // rx interrupt 
    uint8_t mirror = rx_count;           // because volatile data! 
    uart_rx_buf [mirror] = UART1_DR;     // тут из за того что volatile означает что доступ к переменной может быть и из прерывания и из программы нужно получать доступ к ней только в одном месте
    if (rx_count < 20) rx_count ++ ;     // пока данных меньше 20 принимаем
    else rx_count = 0;                   // если больше то начинаем складывать заново 
  }
  if (status & 0x10) {                   // idle interrupt 
    uart_flag.rx_data_ok = true;         // поднимем флаг что данные приняты
    status = UART1_DR;                   // считаем UART1_DR чтоб сбросить idle
    rx_count = 0;                        // отправим в ноль счетчик принятых байт
  }
}

#pragma vector=UART1_T_TXE_vector 
__interrupt void UART1_T_TXE_Handler (void){  // прерывание по передаче 
  uint8_t mirror = tx_count_send;         // because volatile data! 
  if (tx_count <= mirror) {               // пока не передали все байты 
    UART1_DR = uart_tx_buf [tx_count];    // передаем
    tx_count ++;                          // увиличиваем счетчик 
  } else {
    tx_count = 0;                         // если передали последний 
    uart_flag.tx_busy = false;            // освободим юарт 
    UART1_CR2_TIEN = 0;                   // выключим прерывание 
  }
}

