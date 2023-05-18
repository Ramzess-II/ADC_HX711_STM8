
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
  TIM4_PSCR_PSC =  6;   // prescaler
  TIM4_ARR = 250;       // autoreoload  10069 Hz
  TIM4_IER_UIE = 1;     // interrupt enable
  //TIM4_CR1_ARPE = 1;  // enable auto preload
  TIM4_CR1_CEN = 1;     // counter enable
  
}