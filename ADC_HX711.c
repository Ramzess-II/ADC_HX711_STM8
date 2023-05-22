#include "ADC_HX711.h" 


uint32_t adc_value = 0;
uint32_t adc_new_value = 0;

struct adc_union {
  uint8_t count_sck :6;
  uint8_t data_ok :1;
  uint8_t high_low_sck :1;
  uint8_t stop_adc_measurement :1;
};

struct adc_union adc_flag = {GAIN_ADC , 0, 0, 0};

void init_ADC_pin (void) {
  // SCK_PIN PD2
  PD_DDR_DDR2 = 1;       // direction port 0 - input, 1 - output 
  PD_CR1_C12 = 1;        // control 0 - open drein, 1 - push-pull
  PD_CR2_C22 = 1;        // output speed 0 - 2MHz, 1 - 10MHz
  SCK_PIN_OFF;
  // DOUT_PIN PD1
  PD_DDR_DDR3 = 0;       // direction port 0 - input, 1 - output 
  PD_CR1_C13 = 1;        // control 0 - floating, 1 - Input with pull-up
  PD_CR2_C23 = 1;        // External interrupt enabled
  EXTI_CR1_PDIS = 2;     // inerrupt falling portD
}

/*void read_data_ADC (void) {       // call in timer
  if (adc_flag.data_ok) {
    if (adc_flag.count_sck > 0) {
      if (!adc_flag.high_low_sck) {
        SCK_PIN_ON;
        adc_flag.high_low_sck = true;
      } else {
           if (READ_DOUT && adc_flag.count_sck <= 23) {
            adc_new_value |= (uint32_t)1 << adc_flag.count_sck;
          } 
          adc_flag.high_low_sck = false;
          adc_flag.count_sck --;
          SCK_PIN_OFF;
      }
    } else {
      TIM4_IER_UIE = 0;   // interrupt disable
      adc_flag.data_ok = false;
      adc_flag.count_sck = GAIN_ADC;
      adc_new_value = adc_new_value^0x800000;
      adc_value = adc_new_value;
      adc_new_value = 0;
      PD_CR2_C23 = 1;        // External interrupt enabled
    }
  } 
}*/

void read_data_ADC (void) {       // call in timer
  if (adc_flag.data_ok) {
    if (adc_flag.count_sck > 0) {
      if (SCK_PIN_READ) {
          if (READ_DOUT && (adc_flag.count_sck -1)) {
          adc_new_value |= (uint32_t)1 << adc_flag.count_sck - 2;
         } 
      } else {
          adc_flag.count_sck --;
      }   
      PD_ODR_ODR2 = !PD_ODR_ODR2;
    } else {
      TIM4_IER_UIE = 0;   // interrupt disable
      adc_flag.data_ok = false;
      adc_flag.count_sck = GAIN_ADC ;
      adc_new_value = adc_new_value^0x800000;
      adc_value = adc_new_value;
      adc_new_value = 0;
      SCK_PIN_OFF; 
      PD_CR2_C23 = 1;        // External interrupt enabled
    }
  } 
}



#pragma vector=EXTI3_vector  // set interrupt funcion 
__interrupt void EXTI2_Handler (void) {
  if (!READ_DOUT) {
    if (!adc_flag.stop_adc_measurement) {
      adc_flag.data_ok = true;
      adc_flag.high_low_sck = false;
      TIM4_CNTR = 0;      // reset timer counter
      TIM4_IER_UIE = 1;   // interrupt enable
      adc_new_value = 0;
      PD_CR2_C23 = 0;        // External interrupt disabled
    }
  }
}

void stop_adc_read (uint8_t flag) {
  if (flag) {
    adc_flag.stop_adc_measurement = true;
    TIM4_IER_UIE = 0;
  } else {
    adc_flag.stop_adc_measurement = false;
  }
}

