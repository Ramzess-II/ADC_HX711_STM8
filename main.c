#include "main.h"

extern struct adc_union adc_flag;
extern struct uart_union uart_flag;
extern struct flash_data flash ; 
extern struct DS18B20_union DS18B20_flag;
extern int8_t temperature;
extern int32_t massa_int;
extern volatile uint8_t uart_rx_buf [20];
extern volatile uint8_t uart_tx_buf [15];

uint32_t delay_ms_t = 0;
uint16_t tim_to_update_ds18b20 = 0;
uint8_t  count_buzer = 0;
uint8_t tim_to_transmit = 0;
uint8_t tim_to_led = 0;

struct flags {                                      // флаги 
  uint8_t start_conv :1;
  uint8_t read_sensor :1;
  uint8_t pin_but_reset :1;
  uint8_t beep_flag :1;
  uint8_t buzzer_on :1;
  uint8_t beep_start :1;
};

struct flags flag = {0,0,0,0,0,0};                 // инициализация флагов

int main( void )
{
  init_rcc ();                                    // инициализация клоков
  init_gpio();                                    // инициализация кнопки и светика
  init_tim2();                                    // инициализация таймера 2
  init_tim1();
  init_DS18B20_pin();                             // инициализация пинов датчика DS18B20 один на вход другой на выход
  beep_init(0x54);                                // инициализация частоты бипера
  asm("rim");                                     // Enable interrupt
  init_SPI_first();                               // инициализация SPI без включения 
  ADC_pin_init();                                 // пины АЦП HX711
  init_uarts();                                   // инициализация юарта
  
  SCK_PIN_OFF;                                    // пин клоков на землю
  DS_PIN_OUT_ON;                                  // пин данных датчика DS18B20 на землю

 
  
  if (!PC_IDR_IDR3) {                             // если нажата кнопка калибровки
    PIN_LED_ON;                                   // покажем светиком что ждем АЦП
    while (adc_flag.adc_work == 0);               // ждем пока АЦП отдаст данные
    adc_flag.adc_work = false;                    // сбросим флаг работы АЦП
    adc_set_pulse (2);                            // это чтоб АЦП нормально работало дополнительные два импульса
    delay_ms(1000);
    set_buzer (3, 5000);
    delay_ms(3000);
    set_buzer (1, 5000);
    PIN_LED_OFF;                                  // выключим светик                  
      
    while (1) {                                   // ждем калибровки нуля
      if (PC_IDR_IDR3) flag.pin_but_reset = true; // если кнопка до сих пор не была отпущена не пускать дальше
      adc_set_pulse (2);                          // это чтоб АЦП нормально работало дополнительные два импульса
      if (!PC_IDR_IDR3 && flag.pin_but_reset) {   // если нажали кнопку и она была уже отпущена до этого 
        flag.pin_but_reset = false;               // сбросим флаг что была отпущена
        zero_set ();                              // установим ноль
        delay_ms(1000);
        set_buzer (3, 5000);
        delay_ms(3000);
        set_buzer (1, 5000);
        delay_ms(1000);
        break;
      }
    }

    while (1) {                                       // ждем калибровки веса
       if (PC_IDR_IDR3) flag.pin_but_reset = true;    // если кнопка не была отпущена то не пускаем дальше
      adc_set_pulse (2);                              // это чтоб АЦП нормально работало дополнительные два импульса
      if (!PC_IDR_IDR3 && flag.pin_but_reset) {       // если нажали кнопку и кнопка была отпущена
        new_kalib_koef (flash.calib_massa);           // рассчитаем новый калибровочный коэф 
        set_buzer (5, 5000);
        flash.max_weight = 0;                         // сбросим в ноль показания максимального натяжения
        data_eeprom_write();                          // запишем новые данные в еепром после калибровки 
        delay_ms(3000);
        break;
      }
    }
  }
  
  data_eeprom_read();                             // считаем из памяти все значения  
  
  for (;;) {
    adc_set_pulse (2);                            // генерация импульсов для работы АЦП       
    if (flag.start_conv) {                        // флаг старта подготовки данных датчиком DS18B20
      flag.start_conv = false;                    // сброс флага
      start_conversion ();                        // отправим старт преобразования DS18B20
    }
      if (flag.read_sensor) {                     // если поднят флаг считывания данных
      flag.read_sensor = false;                   // сбросим флаг
      read_temp ();                               // запустим чтение с датчика DS18B20
    }
    transmit_uart ();                             // постоянно передаем по юарту данные о весе и температуре
    check_beep ();                                // проверим не достиг ли вес начала пиканья 
    blink_err_led();                              // 
  }
}

void transmit_uart (void) {                       // передача по юарту
  if (!tim_to_transmit) {                         // проверим не настало ли время отправки
    while (uart_flag.tx_busy);
    tim_to_transmit = TIM_UART_SEND;              // зададим новый временной диапазон
    uart_tx_buf [0] = 0x54;                       // "T"
    uart_tx_buf [1] = 0x3D;                       // "="
    uart_tx_buf [2] = (temperature / 10) + 0x30;  // значение температуры
    uart_tx_buf [3] = (temperature  % 10) + 0x30;
    uart_tx_buf [4] = 0x20;                       // " "
    uart_tx_buf [5] = 0x4D;                       // "M"
    uart_tx_buf [6] = 0x3D;                       // "="
    if (massa_int >= 0) uart_tx_buf [7] = 0x2B;   // "+"
    else uart_tx_buf [7] = 0x2D;                  // "-"
    uart_tx_buf [8] = (abs (massa_int / 1000) / 100) + 0x30;       // значение веса в кг
    uart_tx_buf [9] = (abs (massa_int / 1000) / 10) % 10 + 0x30;
    uart_tx_buf [10] = abs (massa_int / 1000) % 10 + 0x30;
    uart_tx_buf [11] = 0x6B;                      // "k"
    uart_tx_buf [12] = 0x67;                      // "g"
    uart_transmit_data (12);
  }
}

void adc_set_pulse (uint8_t pulse) {              // штука которая делает два дополнительных импкльса на линию SCK АЦП
  if(adc_flag.data_ok) {                          // если завершили преобразование и нужно формировать импульсы
    //DELAY_CYCLES(1);                            // подождем ??
    for (uint8_t i = 0; i < pulse; i ++) {        // посылаем оприделенное количество импульсов 
        SCK_PIN_ON;
        DELAY_CYCLES(1);
        SCK_PIN_OFF;
        DELAY_CYCLES(1);
    }
    ADC_pin_manual();                             // перестроим пины АЦП на ручной режим, для включеения прерываний по входу
    adc_flag.data_ok = false;                     // сбросим флаг
    massa_display();                              // преобразуем код АЦП в вес
  }
}

void check_beep (void){                           // проверка не нужно ли пикать
  if (abs (massa_int) > (flash.weight_push / DEVIDER_MASS)  &&  massa_int < flash.weight_push) {                      // если масса больше веса запуска деленного на делитель и меньше чем вес запуска
   set_buzer (1, map (abs (massa_int), (flash.weight_push / DEVIDER_MASS), flash.weight_push, MIN_FREQ, MAX_FREQ));   // пикать с частотой относительно минимальной и максимальной заданной
  } 
  if (massa_int >= flash.weight_push && !flag.beep_start) {  // если масса больше или равна весу отпускания и флаг единоразового запуска бипера опущен ( бипер не работает если его постоянно запускать) 
    TIM2_IER_UIE = 0;                                        // отключим прерывания
    flag.buzzer_on = false;                                  // сбросим флаг защиты нескольких вызовов бипера
    flag.beep_flag = false;                                  // сбросим флаг состояния бипера (включен - выключен)
    flag.beep_start = true;                                  // и поднимем флаг чтоб сюда не заходить 
    beep_set(true);                                          // включим бипер в ручную
  }
}

void set_buzer (uint8_t set_qualiti, uint32_t set_duty) {   // запустим бипер с оприделеным количеством повтором, и частотой повторения
  if (!flag.buzzer_on){                                     // если бузер не работает
    flag.beep_start = false;                                // сбрасываем флаг остановки входа в запуск бипера в постоянный режим 
    beep_set(false);                                        // а так же сбросим бипер руками для того чтоб запускать с нулевого положения
    flag.buzzer_on = true;                                  // поднимем флаг что мы используем бузер
    count_buzer = set_qualiti << 1;                         // умножим на два количество повторов, так как в прерывании мы включаем и выключаем бузер дважды для одного пика 
    TIM2_ARRH = set_duty >> 8;
    TIM2_ARRL = set_duty;                                   // set period
    TIM2_CNTRH = 0;
    TIM2_CNTRL = 0;                                         // обнулим счетчик таймера
    TIM2_SR1_UIF = 0;                                       // сбросим флаг прерывания 
    TIM2_IER_UIE = 1;                                       // interrupt enable
  }
}

#pragma vector=TIM1_OVR_UIF_vector                         // set interrupt funcion 
__interrupt void TIM1_OVR_UIF_Handler (void) {             // обработчик прерываний от таймера 1
    if (tim_to_led) tim_to_led --;                         // для отображения ошибок
    if (delay_ms_t) delay_ms_t --;                         // отнимаем переменную задержки
    if (tim_to_transmit) tim_to_transmit --;               // так же переменную опроса датчика DS18B20
    if (tim_to_update_ds18b20 == 0) flag.start_conv = true;                          // если ноль то запускаем преобразование
    if (tim_to_update_ds18b20 == TIM_18B20_READ >> 1) flag.read_sensor = true;       // если 32767 запускаем считывание
    tim_to_update_ds18b20 ++;                                                        // добавляем значения этой переменной
    if (tim_to_update_ds18b20 == TIM_18B20_READ) tim_to_update_ds18b20 = 0;          // ограничим переменную считывания датчика DS18B20
    TIM1_SR1_UIF = 0;                                                                // сбросим флаг прерывания
}

#pragma vector=TIM2_OVR_UIF_vector                        // set interrupt funcion 
__interrupt void TIM2_OVR_UIF_Handler (void) {            // обработчик прерываний от таймера 2
  if (count_buzer) {                                      // если есть счетчик бузера 
    flag.beep_flag = !flag.beep_flag;                     // изменяем состояние флага 
    beep_set(flag.beep_flag);                             // и по этому состоянию запускаем бузер
    count_buzer --;                                       // уменьшаем переменную счетчика бузера
  } else {
    beep_set(false);                                      // принудительно выключим бузер
    flag.buzzer_on = false;                               // освободим бузер 
    TIM2_IER_UIE = 0;                                     // interrupt disable
  }
    TIM2_SR1_UIF = 0;                                     // сброс флага прерывания
}

void delay_ms (uint32_t time) {                             // задержка на таймере
  delay_ms_t = time;                                        // задаем время и ждем 
  while (delay_ms_t != 0);
}


int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max){         // преобразование одного предела в другой 
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void blink_err_led (void){
  if (!tim_to_led) {
    tim_to_led = ERR_BLINK;
    if(!adc_flag.adc_work || !DS18B20_flag.presence){
          PC_ODR_ODR4 = !PC_ODR_ODR4;                           // моргаем светиком
    } else {
      PIN_LED_OFF;
    }
  }
}

/*
#pragma vector=TIM4_OVR_UIF_vector  // set interrupt funcion 
__interrupt void TIM4_OVR_UIF_Handler (void) {
    TIM4_SR_UIF = 0;
}
*/



