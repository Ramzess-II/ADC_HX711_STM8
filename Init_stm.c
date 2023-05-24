
#include "init_stm.h"

void init_rcc (void) {
  CLK_CKDIVR_HSIDIV = 0; // select prescaler HSI
  CLK_PCKENR1 = 0xFE;    // Peripheral clock gating
  //CLK_CCOR_CCOSEL = 4; // sck out pin
  //CLK_CCOR_CCOEN = 1;
}

void init_gpio (void) {
  // PC4 Buzzer
  PC_DDR_DDR4 = 1;       // direction port 0 - input, 1 - output 
  PC_CR1_C14 = 1;        // control 0 - open drein, 1 - push-pull
  PC_CR2_C24 = 1;        // output speed 0 - 2MHz, 1 - 10MHz
  PC_ODR_ODR4 = 0;       // output data registr  
  
  // PC3 Button
  PC_DDR_DDR3 = 0;       // direction port 0 - input, 1 - output 
  PC_CR1_C13 = 1;        // control 0 - floating, 1 - Input with pull-up
  PC_CR2_C23 = 0;        // External interrupt enabled
}

void init_tim4 (void) {
  TIM4_PSCR_PSC =  2;   // prescaler    // 6 for 1kHz
  TIM4_ARR = 5;        // autoreoload  // 250
  //TIM4_IER_UIE = 1;   // interrupt enable
  //TIM4_CR1_ARPE = 1;  // enable auto preload for avtomatic new value 
  TIM4_CR1_CEN = 1;     // counter enable
}

void init_tim2 (void) {
  TIM2_PSCR = 10;        // prescaler
  TIM2_CR1_ARPE = 1;    // enable auto preload for avtomatic new value 
  TIM2_ARRH = 10;
  TIM2_ARRL = 100;      // 14us
 // TIM2_IER_UIE = 1;   // interrupt enable
  TIM2_CR1_CEN = 1;     // counter enable
}

void init_tim1 (void) {  // delay
  TIM1_PSCRH = 0;       // prescaler
  TIM1_PSCRL = 16;      // prescaler 
  TIM1_ARRH = 0x03;
  TIM1_ARRL = 0xE8;     // 1kHz
  TIM1_IER_UIE = 1;     // interrupt enable
  TIM1_CR1_CEN = 1;     // counter enable
}

void beep_init (uint8_t div) {
      // BEEP_PIN PD4
  PD_DDR_DDR4 = 1;       // direction port 0 - input, 1 - output 
  PD_CR1_C14 = 1;        // control 0 - open drein, 1 - push-pull
  PD_CR2_C24 = 1;        // output speed 0 - 2MHz, 1 - 10MHz
  BEEP_CSR_BEEPDIV = div;
  //BEEP_CSR_BEEPEN = 1;
}

void beep_set (uint8_t flag) {
  if (flag) BEEP_CSR_BEEPEN = 1;
  else BEEP_CSR_BEEPEN = 0;
}