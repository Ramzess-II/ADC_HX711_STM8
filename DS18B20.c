#include "DS18B20.h" 

int8_t temperature;                       // переменная для хранения температуры 

struct DS18B20_union DS18B20_flag = {0};  // инициализация 

void init_DS18B20_pin (void) {
  // OUT_DATA
  PD_DDR_DDR2 = 1;       // direction port 0 - input, 1 - output 
  PD_CR1_C12 = 0;        // control 0 - open drein, 1 - push-pull
  PD_CR2_C22 = 1;        // output speed 0 - 2MHz, 1 - 10MHz
  PD_ODR_ODR2 = 1;       // output data registr 
  // INPT_DATA
  PD_DDR_DDR3 = 0;       // direction port 0 - input, 1 - output
  PD_CR1_C13 = 0;        // control 0 - floating, 1 - Input with pull-up
  PD_CR2_C23 = 0;        // External interrupt enabled
}

void init_sensor_DS18B20 (void) {     // инициализация старта обмена данными с датчиком DS18B20
  DS_PIN_OUT_OFF;                     // пин на землю       
  DELAY_CYCLES(1248);                 // ждем 480us * на 2,6
  DS_PIN_OUT_ON;                      // отпускаем пин
  DELAY_CYCLES(208);                  // ждем 80 us 
  if (!DS_PIN_READ) {                  // читаем пин
    DS18B20_flag.presence = true;     // если датчик ответил поднимем флаг
  }
  DELAY_CYCLES(1000);                 // подождем еще чтоб закончить старт 
}

uint8_t read_DS18B20 (void) {         // чтение байта с датчика DS18B20
  uint8_t data;
  for (int i = 0; i < 8; i++) {
        DS_PIN_OUT_OFF;
        DELAY_CYCLES(39);
        DS_PIN_OUT_ON;
        DELAY_CYCLES(2);
        data = data >> 1;
        if (DS_PIN_READ) {
         data  |= 0x80;
        }
        DELAY_CYCLES(120);
  }
  return data;
}

void write_DS18B20 (uint8_t data) {
  for (int i = 0; i < 8; i++) {
    if (data & 0x01){
        DS_PIN_OUT_OFF;
        DELAY_CYCLES(39);
        DS_PIN_OUT_ON;
        DELAY_CYCLES(117);
    } else {
        DS_PIN_OUT_OFF;
        DELAY_CYCLES(300);
        DS_PIN_OUT_ON;
        DELAY_CYCLES(2);
    }
    data >>= 1;
  }
}

void read_temp (void) {              // считать регистры с датчика DS18B20 (можно читать или все 9 бит или остановить в любой момент, допустим после 2х ) 
  uint8_t lsb = 0;                   // переменная для младшего байта 
  uint8_t msb = 0;                   // переменная для старшего байта 
  init_sensor_DS18B20();             // инициализируем обмен с датчиком 
  write_DS18B20 (NOID);              // отправим команду пропуска проверки серийного номера
  write_DS18B20 (READ_DATA);         // отправим команду чтения с датчика 
  lsb = read_DS18B20();              // считаем младший байт
  msb = read_DS18B20();              // считаем старший байт
  temperature = (uint8_t) (lsb >> 4) | (msb << 4);    // удалим лишние биты, и дробную часть. получим температуру в градусах 
}

void start_conversion (void){        // запустить преобразование датчика 
  init_sensor_DS18B20();             // инициализируем обмен с датчиком  
  write_DS18B20 (NOID);              // отправим команду пропуска проверки серийного номера
  write_DS18B20 (T_CONVERT);         // отправим команду начать преобразование температуры 
}


// Алгоритм для вычисления CRC-8 для DALLAS.
// Для первого байта CRC = 0, для остальных - то, что получилось от предыдущего.
// В расчёт CRC входят первые 8 байт DS18B20.
// 1. Нахождение логического исключающего ИЛИ между младшим битом CRC и младшим битом данных. 
// 2. Если результат равен 0, то:
//    - Сдвиг вправо CRC.
// 3. Если результат равен 1, то:
//    - Поиск нового значения CRC путем вычисления логического исключающего ИЛИ между CRC и полиномом CRC.
//    - Сдвиг вправо CRC.
//    - Установка старшего бита CRC в 1. 
//    - Сдвиг вправо данных. 
//    - Повтор данной последовательности 8 раз на 1 байт данных.
uint8_t DS18B20_crc (uint16_t adress)
{
	uint8_t crc = 0;			// Переменная для накопления CRC
	for (uint8_t i=0; i<8; i++)	// Считаю CRC 8-ми байт, 9-й байт это CRC
	{
		// Расчитываю CRC одного байта
		crc = crc ^ (*(uint16_t*)(adress+i));
		for (uint8_t j=0; j<8; j++)
		{
			if (crc & 0x01) crc = (crc >> 1) ^ 0x8C;
			else crc >>= 1;
		}
	}
	return crc;	// Возвращаю CRC
}
        
        