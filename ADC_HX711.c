#include "ADC_HX711.h" 

#define SCK_PIN_ON        PD_ODR_ODR2 = 1     // PD2
#define SCK_PIN_OFF       PD_ODR_ODR2 = 0     // PD2
#define READ_DOUT         PD_IDR_IDR3         // PD3


void init_ADC_pin (void) {
  // SCK_PIN PD2
  PD_DDR_DDR2 = 1;       // direction port 0 - input, 1 - output 
  PD_CR1_C12 = 1;        // control 0 - open drein, 1 - push-pull
  PD_CR2_C22 = 1;        // output speed 0 - 2MHz, 1 - 10MHz
  // DOUT_PIN PD1
  PD_DDR_DDR3 = 0;       // direction port 0 - input, 1 - output 
  PD_CR1_C13 = 1;        // control 0 - floating, 1 - Input with pull-up
  PD_CR2_C23 = 1;        // External interrupt enabled
  EXTI_CR1_PDIS = 2;     // inerrupt falling portD
}

void read_data_ADC (char quantity ) {
  
}

#pragma vector=EXTI3_vector  // set interrupt funcion 
__interrupt void EXTI2_Handler (void) {
  if (READ_DOUT) {
    SCK_PIN_ON;
  }
}