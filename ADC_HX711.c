#include "ADC_HX711.h" 

extern struct flash_data flash ; 

int32_t adc_value = 0;                       // это текущее значение АЦП
uint32_t adc_new_value = 0;                  // а это то что мы получаем из SPI
int8_t set_poz_bit;                          // это для установки байта в который мы считываем данные из АЦП, так как читаем по 8 бит

float massa = 0;                             // масса в флоат
int32_t massa_int = 0;                       // масса в инте 

struct adc_union adc_flag = {0 , 0, 0, 0};   // инициализация флагов

void ADC_pin_init (void) {
    // SCK_PIN PC5
  PC_DDR_DDR5 = 1;       // direction port 0 - input, 1 - output 
  PC_CR1_C15 = 1;        // control 0 - open drein, 1 - push-pull
  PC_CR2_C25 = 1;        // output speed 0 - 2MHz, 1 - 10MHz
  // DOUT_PIN PC7
  PC_DDR_DDR7 = 0;       // direction port 0 - input, 1 - output 
  PC_CR1_C17 = 1;        // control 0 - floating, 1 - Input with pull-up
  PC_CR2_C27 = 1;        // External interrupt enabled
  EXTI_CR1_PCIS = 2;     // inerrupt falling portC
}

void ADC_pin_manual (void) {
  // DOUT_PIN PC7
  PC_DDR_DDR7 = 0;       // direction port 0 - input, 1 - output 
  PC_CR1_C17 = 1;        // control 0 - floating, 1 - Input with pull-up
  PC_CR2_C27 = 1;        // External interrupt enabled
}

void ADC_pin_automat (void) {
  // DOUT_PIN PC7
  PC_DDR_DDR7 = 0;       // direction port 0 - input, 1 - output 
  PC_CR1_C17 = 1;        // control 0 - floating, 1 - Input with pull-up
  PC_CR2_C27 = 0;        // External interrupt disabled
}

void init_SPI_first (void) {
  SPI_CR1_BR = 4;        // clok prescaler
  SPI_CR1_MSTR = 1;      // master mode
  SPI_CR1_CPHA = 1;      // clock phase
  SPI_CR2_RXONLY = 0;    // Only recive
}

void init_SPI (void) {
  SPI_CR1_SPE = 1;       // spi enable
  SPI_ICR_RXIE = 1;      // interrupt enable
}

void deinit_SPI (void) {
  SPI_CR1_SPE = 0;       // spi enable
  SPI_ICR_RXIE = 0;      // interrupt enable
}

#pragma vector = SPI_RXNE_vector                 // set interrupt funcion 
__interrupt void SPI_RXNE_Handler (void) {       // прерывание от SPI
  uint32_t mirror = 0;                           // зеркало для упрощения понимания кода
  if (set_poz_bit > 0) {                         // если мы еще не в нуле то считываем данные дальше 
    mirror = (uint32_t) SPI_DR << set_poz_bit;   // сдвигаем полученый байт на столько,  на какой позиции мы находимся 
    adc_new_value |= mirror;                     // а так же добавляем полученый байт в нашу переменную 
    set_poz_bit -= 8;                            // отнимаем 8 так как у нас считывается по 1 байту
    SPI_DR = 0xFF;                               // и шлем какой то мусор в дата, чтоб совершить обмен по SPI
  } else {
    adc_new_value |= SPI_DR;                     // и когда у нас последний нулевой байт считаем его в самую младшую часть
    deinit_SPI ();                               // выключим SPI и прерывание от него 
    adc_flag.data_ok = true;                     // поднимем флаг что данные приняли (это для калибровки в начале) 
    adc_value = adc_new_value^0x800000;          // преобразуем число со знаком в число без знака
    adc_flag.adc_work = true;                    // поднимем флаг что нужно послать дополнительные импульсы в АЦП
    //ADC_pin_manual();                          // это выполним уже там где посылаем импульсы 
  }
  SPI_SR_RXNE = 0;                               //  сбросим флаг прерывания по SPI
}

#pragma vector=EXTI2_vector                      // set interrupt funcion 
__interrupt void EXTI2_Handler (void) {          // прерывание от ножки МК
  if (!READ_DOUT) {                              // точно ли это наша ножка вызвала прерывание 
    if (!adc_flag.stop_adc_measurement) {        // проверим не запрещена ли работа АЦП
      set_poz_bit = 16;                          // установим смещение для байтво в коде АЦП 
      adc_new_value = 0;                         // сбросим в ноль код АЦП
      ADC_pin_automat();                         // переключим пины для использование их SPI
      init_SPI ();                               // включим SPI и прерывания
      SPI_DR = 0x11;                             // запустим процес чтения 
    }
  }
}

void massa_display (void) {                                       // преобразуем код АЦП в массу 
  massa = (float) (adc_value - flash.zero_adc)* flash.cal_coef;   // отнимем код нуля и умножим на калибровочный коэфициент 
  massa_int = (int32_t) massa;                                    // а так же сделаем инт для удобной передачи 
}

void zero_set (void) {                                // установка нуля 
  flash.zero_adc = adc_value; 
}

void new_kalib_koef (int32_t cal){                   // установка калибровочного коэфициента
  flash.cal_coef = (float) cal / (adc_value - flash.zero_adc);
}



void stop_adc_read (uint8_t flag) {                  // остановить считывание АЦП ??? 
  if (flag) {
  } else {
  }
}

