
#include "init_stm.h"

void init_rcc (void) {
  CLK_CKDIVR_HSIDIV = 0; // select prescaler HSI
  
  CLK_PCKENR1 = 0xF0;    // Peripheral clock gating
  //CLK_CCOR_CCOSEL = 4;
  //CLK_CCOR_CCOEN = 1;
}

void init_gpio (void) {
  PB_DDR_DDR5 = 1;       // direction port 0 - input, 1 - output 
  PB_CR1_C15 = 1;        // control 0 - open drein, 1 - push-pull
  PB_CR2_C25 = 1;        // output speed 0 - 2MHz, 1 - 10MHz
 // PB_ODR_ODR5 = 1;     // output data registr  
  
  PC_DDR_DDR4 = 1;       // direction port 0 - input, 1 - output 
  PC_CR1_C14 = 1;        // control 0 - open drein, 1 - push-pull
  PC_CR2_C24 = 1;        // output speed 0 - 2MHz, 1 - 10MHz
  
}

void init_tim4 (void) {
  TIM4_PSCR_PSC =  2;   // prescaler    // 6 for 1kHz
  TIM4_ARR = 5;        // autoreoload  // 250
  //TIM4_IER_UIE = 1;   // interrupt enable
  //TIM4_CR1_ARPE = 1;  // enable auto preload for avtomatic new value 
  TIM4_CR1_CEN = 1;     // counter enable
}

void init_tim2 (void) {
  TIM2_PSCR = 2;        // prescaler
  TIM2_ARRH = 0;
  TIM2_ARRL = 56;       // 14us
  TIM2_CR1_ARPE = 1;    // enable auto preload for avtomatic new value 
 // TIM2_IER_UIE = 1;   // interrupt enable
  TIM2_CR1_CEN = 1;     // counter enable
}

void init_tim1 (void) {
  TIM1_PSCRH = 0;        // prescaler
  TIM1_PSCRL =0;        // prescaler 
  TIM1_ARRH = 0;
  TIM1_ARRL = 100;      // 1MHz
 //TIM1_IER_UIE = 1;    // interrupt enable
  TIM1_CR1_CEN = 1;     // counter enable
}

void beep_init (uint8_t div) {
      // BEEP_PIN PD4
  PD_DDR_DDR4 = 1;       // direction port 0 - input, 1 - output 
  PD_CR1_C14 = 1;        // control 0 - open drein, 1 - push-pull
  PD_CR2_C24 = 1;        // output speed 0 - 2MHz, 1 - 10MHz
  
  BEEP_CSR_BEEPEN = 1;
  BEEP_CSR_BEEPDIV = div;
}