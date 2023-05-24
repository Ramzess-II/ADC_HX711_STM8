#include "ADC_HX711.h" 

int32_t adc_value = 0;
uint32_t adc_new_value = 0;
//uint32_t filtr_adc [17];
int8_t set_poz_bit;

uint32_t zero_adc = 0;
float cal_coef = 12.54;
float massa = 0;
int32_t massa_int = 0;

struct adc_union adc_flag = {0 , 0, 0, 0};

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

#pragma vector = SPI_RXNE_vector // set interrupt funcion 
__interrupt void SPI_RXNE_Handler (void) {
  uint32_t mirror = 0;
  if (set_poz_bit > 0) {
    mirror = (uint32_t) SPI_DR << set_poz_bit;
    adc_new_value |= mirror;
    set_poz_bit -= 8;
    SPI_DR = 0xFF;
  } else {
    adc_new_value |= SPI_DR;
    SPI_SR_RXNE = 0;
    deinit_SPI ();
    adc_flag.data_ok = true;
    adc_value = adc_new_value^0x800000;
    adc_value -= zero_adc;
    adc_flag.adc_work = true;
    //ADC_pin_manual();
  }
}

#pragma vector=EXTI2_vector  // set interrupt funcion 
__interrupt void EXTI2_Handler (void) {
  if (!READ_DOUT) {
    if (!adc_flag.stop_adc_measurement) {
      set_poz_bit = 16;
      adc_new_value = 0;
      ADC_pin_automat();
      init_SPI ();
      SPI_DR = 0x11;
    }
  }
}

void massa_display (void) {
  massa = (float)adc_value * cal_coef;
  massa_int = (int32_t) massa;
}

void zero_set (void) {
  zero_adc = adc_value; 
}

void new_kalib_koef (void){
  cal_coef = (float)CAL_MASS / adc_value;
}

void stop_adc_read (uint8_t flag) {
  if (flag) {
  } else {
  }
}

