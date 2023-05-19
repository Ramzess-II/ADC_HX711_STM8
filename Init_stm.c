
#include "init_stm.h"

void init_rcc (void) {
  CLK_CKDIVR_HSIDIV = 0; // select prescaler HSI
}

void init_gpio (void) {
  PB_DDR_DDR5 = 1;       // direction port 0 - input, 1 - output 
  PB_CR1_C15 = 1;        // control 0 - open drein, 1 - push-pull
  PB_CR2_C25 = 1;        // output speed 0 - 2MHz, 1 - 10MHz
 // PB_ODR_ODR5 = 1;     // output data registr  
}

void init_tim4 (void) {
  TIM4_PSCR_PSC =  2;   // prescaler    // 6 for 1kHz
  TIM4_ARR = 20;        // autoreoload  // 250
  TIM4_IER_UIE = 1;     // interrupt enable
  //TIM4_CR1_ARPE = 1;  // enable auto preload for avtomatic new value 
  TIM4_CR1_CEN = 1;     // counter enable
}

void init_tim2 (void) {
  TIM2_PSCR = 2;        // prescaler
  TIM2_ARRH = 0;
  TIM2_ARRL = 60;
  //TIM2_IER_UIE = 1;     // interrupt enable
  TIM2_CR1_CEN = 1;     // counter enable
}

void init_tim1 (void) {
  TIM1_PSCRH = 0;        // prescaler
  TIM1_PSCRL = 2;        // prescaler
  TIM1_ARRH = 0;
  TIM1_ARRL = 60;
  TIM1_IER_UIE = 1;     // interrupt enable
  TIM1_CR1_CEN = 1;     // counter enable
}