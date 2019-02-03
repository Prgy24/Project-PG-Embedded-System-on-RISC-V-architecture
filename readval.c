#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
#include <string.h>
#include "plic/plic_driver.h"
#include "encoding.h"
#include <unistd.h>
#include "stdatomic.h"

#define SEND PIN_8_OFFSET

#define REC PIN_12_OFFSET

int pow(int x)
{
	int p = 1;
	while(x > 0)
	{
		p = p * 2;
		x--;
	}
	return p;
}



static void _putc(char c) {
  while ((int32_t) UART0_REG(UART_REG_TXFIFO) < 0);
  UART0_REG(UART_REG_TXFIFO) = c;
}

int _getc(char * c){
  int32_t val = (int32_t) UART0_REG(UART_REG_RXFIFO);
  if (val > 0) {
    *c =  val & 0xFF;
    return 1;
  }
  return 0;
}


static void _puts(const char * s) {
  while (*s != '\0'){
    _putc(*s++);
  }
}






//DELAY
void wait_ms(uint64_t ms)
{
	static const uint64_t ms_tick = RTC_FREQ/1000;
	volatile uint64_t * mtime  = (uint64_t*) (CLINT_CTRL_ADDR + CLINT_MTIME);
	uint64_t then = (ms_tick * ms) + *mtime;
	while(*mtime<then);
}

int main(int argc, char **argv)
{



  PRCI_REG(PRCI_HFROSCCFG) |= ROSC_EN(1);
  // Run off 16 MHz Crystal for accuracy. Note that the
  // first line is 
  PRCI_REG(PRCI_PLLCFG) = (PLL_REFSEL(1) | PLL_BYPASS(1));
  PRCI_REG(PRCI_PLLCFG) |= (PLL_SEL(1));
  // Turn off HFROSC to save power
  PRCI_REG(PRCI_HFROSCCFG) &= ~(ROSC_EN(1));

  // Configure UART to print
  GPIO_REG(GPIO_OUTPUT_VAL) |= IOF0_UART0_MASK;
  GPIO_REG(GPIO_OUTPUT_EN)  |= IOF0_UART0_MASK;
  GPIO_REG(GPIO_IOF_SEL)    &= ~IOF0_UART0_MASK;
  GPIO_REG(GPIO_IOF_EN)     |= IOF0_UART0_MASK;

  // 115200 Baud Rate
  UART0_REG(UART_REG_DIV) = 138;
  UART0_REG(UART_REG_TXCTRL) = UART_TXEN;
  UART0_REG(UART_REG_RXCTRL) = UART_RXEN;

  // Wait a bit to avoid corruption on the UART.
  // (In some cases, switching to the IOF can lead
  // to output glitches, so need to let the UART
  // reciever time out and 



	//uint64_t *  stime = (volatile uint64_t*)(CLINT_CTRL_ADDR + CLINT_MTIME);


	//LED INITIALIZING
	
	uint8_t z ,y;
	GPIO_REG(GPIO_OUTPUT_EN)   |=  (0x1 << SEND) ; 
//GPIO_REG(GPIO_OUTPUT_VAL)  |=  0x00 ;	
	
GPIO_REG(GPIO_OUTPUT_VAL)  |=  (0x0 << SEND) ;
	while(1)
	{
	
		
		GPIO_REG(GPIO_INPUT_EN)  |=  (0x1 << REC) ;
		GPIO_REG(GPIO_INPUT_VAL) = 0x00;
		y = REC ;
		printf("OFFSET: %d\n\r",y);
		z = GPIO_REG(GPIO_INPUT_VAL) & (pow(REC));
		printf("value: %d\n\r",z);		

		printf("value1: %d\n\r",GPIO_REG(GPIO_INPUT_VAL));
		/*z = GPIO_REG(GPIO_INPUT_VAL)  |=  (0x1 << REC) ;
		printf("value: %d\n\r",*z);
		if((GPIO_REG(GPIO_INPUT_VAL)  |=  (0x1 << REC)) == 0)
			printf("HIGH\n\r");
		else 
			printf("LOW\n\r");*/		
		wait_ms(1000) ;

	}
		
}
