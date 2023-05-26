#ifndef __DELAY_H
#define __DELAY_H

#define true            1
#define false           0

#define DELAY_CYCLES(n) \
do { \
  unsigned int _delay_counter = (n); \
  for (; _delay_counter > 0; _delay_counter--) { \
    asm("nop"); \
  } \
} while(0)

// функция задержки основана на макросах define 


#endif /* __DELAY_H */