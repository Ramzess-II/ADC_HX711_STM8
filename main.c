#include "main.h"

int i = 0;
uint8_t tim_to_delay;

struct flags {
  uint8_t flag_led :1;
  uint8_t flag_2 :1;
};

struct flags flag;

int main( void )
{
  init_rcc ();
  init_tim4();
  init_tim2();
  init_tim1();
  init_gpio();
  init_DS18B20_pin();
  beep_init(0x1E);
  asm("rim"); //Enable interrupt
 
  PC_ODR_ODR4 = 0;
  
  init_ADC_pin();
  init_sensor_DS18B20();

  
  for (;;) {
    PC_ODR_ODR4 = !PC_ODR_ODR4;
     DELAY_CYCLES(1);
  }
}

#pragma vector=TIM4_OVR_UIF_vector  // set interrupt funcion 
__interrupt void TIM4_OVR_UIF_Handler (void) {
    read_data_ADC();
    TIM4_SR_UIF = 0;
}

#pragma vector=TIM2_OVR_UIF_vector  // set interrupt funcion 
__interrupt void TIM2_OVR_UIF_Handler (void) {
    TIM2_SR1_UIF = 0;
}

#pragma vector=TIM1_OVR_UIF_vector  // set interrupt funcion 
__interrupt void TIM1_OVR_UIF_Handler (void) {
    if (tim_to_delay ) tim_to_delay --;
    else TIM1_IER_UIE = 0;         // interrupt disable
    //PC_ODR_ODR4 = !PC_ODR_ODR4;
    TIM1_SR1_UIF = 0;
}

void delay_us (uint8_t time){
  tim_to_delay = time  ;
  TIM1_CNTRL = 0;           // reset timer counter
  TIM1_IER_UIE = 1;         // interrupt enable
  while (tim_to_delay != 0);
}
