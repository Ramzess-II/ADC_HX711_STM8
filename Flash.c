#include "Flash.h"

uint8_t *data_adrres = (uint8_t*)START_EEPROM_ADRESS;                   // если мы сделаем START_EEPROM_ADRESS ++  то мы сместимся на 1 ячейку 8 битную ( а если б у нас было *int32_t ++) то мы б переместились на 4 ячейки 

struct flash_data flash = { 8385000, 0.05, 2000, 0, 1000};              // инициализация переменных хранящихся в памяти  

void unlook_eeprom (void) {                                             // ключи для разблокировки ЕЕпром памяти
  FLASH_DUKR = 0xAE;
  FLASH_DUKR = 0x56;
}

void data_eeprom_write (void) {                                         // запись всех переменных в еепром
  unlook_eeprom();                                                      // разблокировать память 
  while(!FLASH_IAPSR_DUL);                                              // ждем установки бита DUL говорящего что память разблокирована
  devide_int (data_adrres, flash.zero_adc);                             // запишем в память по адрессу data_adrres flash.zero_adc
  while(!FLASH_IAPSR_EOP);                                              // ловим флаг завершения работы с ЕЕПром 
  devide_int (data_adrres + 4, float_to_int (flash.cal_coef));          // тут увеличиваем адрес на 4 так как у нас 32 битная переменная 
  while(!FLASH_IAPSR_EOP);
  devide_int (data_adrres + 8, flash.weight_push);
  while(!FLASH_IAPSR_EOP);
  devide_int (data_adrres + 12, flash.max_weight);
  while(!FLASH_IAPSR_EOP);
  devide_int (data_adrres + 16, flash.calib_massa);
  while(!FLASH_IAPSR_EOP);
  FLASH_IAPSR_PUL = 0;                                                   // включим защиту памяти
  FLASH_IAPSR_DUL = 0;                                                   // сбросим бит разблокированной памяти 
}

void data_eeprom_read (void) {                                           // считать данные из ЕЕпром
  flash.zero_adc = make_int (data_adrres);                               // по одному считываем и преобразовываем байты из памяти
  flash.cal_coef = int_to_float (make_int (data_adrres + 4));
  flash.weight_push = make_int (data_adrres + 8); 
  flash.max_weight = make_int (data_adrres + 12); 
  flash.calib_massa = make_int (data_adrres + 16); 
}

float int_to_float ( int32_t dat) {                                      // функция для преобразование инта из памяти в флоат
  union transmit {                                                       // с помощью объеденения создадим доступ к памяти как к флоат и как к инту
    int32_t in;
    float out;
  } code;
  code.in = dat;
  return code.out;                                                       // вернем флоат
}

int32_t float_to_int (float dat) {                                       // функция для преобразования из флоата в инт
    union transmit {
    int32_t in;
    float out;
  } code;
    code.out = dat;
    return code.in;
}

void devide_int (uint8_t *adr, int32_t data) {                           // функция для записи одного числа 32бит в память по 8 бит
  *adr = data >> 24;
   adr ++;
  *adr = data >> 16;
   adr ++;
  *adr = data >> 8; 
   adr ++;
  *adr = data;
}

int32_t make_int (uint8_t *adr) {                                        // обратная функция для сборки 32битного числа из 8 битных из памяти 
  int32_t mirror;
  mirror = (int32_t)*adr << 24;
  adr ++;
  mirror |= (int32_t) *adr << 16;
  adr ++;
  mirror |= (int32_t) *adr << 8;
  adr ++;
  mirror |= (int32_t) *adr;
  return mirror;
}

