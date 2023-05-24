#include "main.h"

extern struct adc_union adc_flag;
extern int8_t temperature;

uint32_t delay_ms_t = 0;
uint16_t tim_to_delay = 0;
uint16_t tim_to_update_ds18b20 = 0;
uint8_t count_buzer = 0;


struct flags {
  uint8_t start_conv :1;
  uint8_t read_sensor :1;
  uint8_t pin_but_reset :1;
  uint8_t beep_flag :1;
};

struct flags flag = {0,0,0,0};

int main( void )
{
  init_rcc ();
  init_gpio();
  //init_tim4();
  init_tim2();
  init_tim1();
  init_DS18B20_pin();
  beep_init(0x54);
  asm("rim"); //Enable interrupt
  init_SPI_first();
  ADC_pin_init();
  SCK_PIN_OFF;
  DS_PIN_OUT_ON;
  if (!PC_IDR_IDR3) {
    while (adc_flag.adc_work == 0);
    adc_flag.adc_work = false;
    delay_ms(1000);
    set_buzer (3, 5000);
    delay_ms(3000);
    set_buzer (1, 5000);
    while (1) {
      if (PC_IDR_IDR3) flag.pin_but_reset = true;
      adc_set_pulse (2);
      if (!PC_IDR_IDR3 && flag.pin_but_reset) {
        flag.pin_but_reset = false;
        zero_set ();
        delay_ms(1000);
        set_buzer (3, 5000);
        delay_ms(3000);
        set_buzer (1, 5000);
        delay_ms(1000);
        break;
      }
    }

    while (1) {
       if (PC_IDR_IDR3) flag.pin_but_reset = true;
      adc_set_pulse (2);
      if (!PC_IDR_IDR3 && flag.pin_but_reset) {
        new_kalib_koef ();
        set_buzer (5, 5000);
        delay_ms(3000);
        break;
      }
    }
  }
  
  
  for (;;) {
    adc_set_pulse (2);
    if (flag.start_conv) {
      flag.start_conv = false;
      start_conversion ();
    }
      if (flag.read_sensor) {
      flag.read_sensor = false;
      read_temp ();
    }
    
  }
}

void adc_set_pulse (uint8_t pulse) { 
  if(adc_flag.data_ok) {
    DELAY_CYCLES(5);
    for (uint8_t i = 0; i < pulse; i ++) {
        SCK_PIN_ON;
        DELAY_CYCLES(1);
        SCK_PIN_OFF;
        DELAY_CYCLES(1);
    }
    ADC_pin_manual(); 
    adc_flag.data_ok = false;
    massa_display();
  }
}

void set_buzer (uint8_t set_qualiti, uint16_t set_duty) {
  count_buzer = set_qualiti << 1;
  TIM2_ARRH = set_duty >> 8;
  TIM2_ARRL = set_duty;       // set period
  TIM2_CNTRH = 0;
  TIM2_CNTRL = 0;
  TIM2_SR1_UIF = 0;
  TIM2_IER_UIE = 1;          // interrupt enable
}

#pragma vector=TIM1_OVR_UIF_vector  // set interrupt funcion 
__interrupt void TIM1_OVR_UIF_Handler (void) {
    if (delay_ms_t) delay_ms_t --;
    if (tim_to_update_ds18b20 == 0) flag.start_conv = true;
    if (tim_to_update_ds18b20 == 32767) flag.read_sensor = true;
    tim_to_update_ds18b20 ++;
    TIM1_SR1_UIF = 0;
}

#pragma vector=TIM2_OVR_UIF_vector  // set interrupt funcion 
__interrupt void TIM2_OVR_UIF_Handler (void) {
  if (count_buzer) {
    flag.beep_flag = !flag.beep_flag;
    beep_set(flag.beep_flag);
    PC_ODR_ODR4 = !PC_ODR_ODR4;
    count_buzer --;
  } else {
    PIN_BUZZER_OFF;
    beep_set(false);
    TIM2_IER_UIE = 0;      // interrupt disable
  }
    TIM2_SR1_UIF = 0;
}

void delay_ms (uint32_t time) {
  delay_ms_t = time;
  while (delay_ms_t != 0);
}

/*
#pragma vector=TIM4_OVR_UIF_vector  // set interrupt funcion 
__interrupt void TIM4_OVR_UIF_Handler (void) {
    TIM4_SR_UIF = 0;
}
*/



