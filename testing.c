#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
#include <string.h>
#include "plic/plic_driver.h"
#include "encoding.h"
#include <unistd.h>
#include "stdatomic.h"

#define LED1 PIN_3_OFFSET

#define LED2 PIN_5_OFFSET

#define LED3 PIN_6_OFFSET



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

uint32_t a;
printf("Enter value\n\r");
scanf("%d",&a);
printf("the vcaue is %d",a);
/*
	unsigned long  stime = *(volatile unsigned long *)(CLINT_CTRL_ADDR + CLINT_MTIME);

printf("RTC FREQUENCY: %d\n\r",RTC_FREQ);
//printf("RTC FREQUENCY: %d\n\r",CORE_FREQ);

	//LED INITIALIZING
	GPIO_REG(GPIO_OUTPUT_EN)   |=  (0x1 << LED1) | (0X1 << LED2) | (0X1 << LED3); 
	
	while(1)
	{
	static const uint64_t ms_tick = RTC_FREQ/1000;
	
		GPIO_REG(GPIO_OUTPUT_VAL)  =  (0x1 << LED1) | (0X0 << LED2) | (0X0 << LED3) ;
	printf("RED ON\n\r");		
		wait_ms(1000) ;
	unsigned long   time = *(volatile unsigned long *)(CLINT_CTRL_ADDR + CLINT_MTIME);
	//uint32_t curtime = 
	printf("Time: %f",(time - stime) ;
		
	
		GPIO_REG(GPIO_OUTPUT_VAL)  =  (0x0 << LED1) | (0X1 << LED2) | (0X0 << LED3) ;
	printf("GREEN ON\n\r");
		wait_ms(1000) ;
	//printf("Time: %f",(((volatile uint64_t*)(CLINT_CTRL_ADDR + CLINT_MTIME) - *  stime)/263041843)) ;


		GPIO_REG(GPIO_OUTPUT_VAL)  =  (0x0 << LED1) | (0X0 << LED2) | (0X1 << LED3) ;
	printf("BLUE ON\n\r");
		wait_ms(1000) ;
	//printf("Time: %f",(- * ((volatile uint64_t*)(CLINT_CTRL_ADDR + CLINT_MTIME) - *  stime)/ms_tick)) ;

	}*/
		
}
