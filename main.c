#include "main.h"

int i = 0;
struct flags {
  uint8_t flag_led :1;
  uint8_t flag_2 :1;
};

struct flags flag;

int main( void )
{
  init_rcc ();
  init_tim4();
  init_gpio();
  init_ADC_pin();
  
  asm("rim"); //Enable interrupt
  flag.flag_led = 0;
  
  for (;;) {
    if (i >= 1000) {
      flag.flag_led = !flag.flag_led; 
      i = 0;
    } 
    if (flag.flag_led ) {
      PB_ODR_ODR5 = 0;
    }else {
      PB_ODR_ODR5 = 1;
    }
  }
}

#pragma vector=TIM4_OVR_UIF_vector  // set interrupt funcion 
__interrupt void TIM4_OVR_UIF_Handler (void) {
    read_data_ADC();
}


