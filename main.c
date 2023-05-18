
#include "main.h"

int count = 0;
int main( void )
{
  void init_rcc ();
  while (1){
    count ++;
    
  }
 
}

void init_rcc (void) {
CLK_CKDIVR_HSIDIV = 0x00;
}