#include <stdio.h>
#include "platform.h"
#include <string.h>
#include "plic/plic_driver.h"
#include "encoding.h"
#include <unistd.h>
#include "stdatomic.h"

#define DPIN PIN_12_OFFSET								// DPIN = DHT-11 Connnected PIN

#define GOT_VALUES 0
#define CHECKSUM_ERROR -1
#define TIMEOUT_ERROR -2
int humidity, temperature ;

uint8_t bits[5] ;
uint8_t cnt = 7 ;
uint8_t idx = 0 ;

static const uint64_t us_tick = RTC_FREQ/1000000 ;

int pow11(int x)
{
	int p = 1;
	while(x > 0)
	{
		p = p * 2;
		x--;
	}
	return p;
}

uint8_t z;

// DELAY IN MILLISECONDS
void wait_us (uint64_t us)
{
	//static const uint64_t us_tick = RTC_FREQ/1000000 ;
	volatile uint64_t * mtime = (uint64_t*) (CLINT_CTRL_ADDR + CLINT_MTIME) ;
	uint64_t then = (us_tick * us) + * mtime ;
	while(* mtime < then);
}

int main (int argc, char **argv)
{
	uint32_t p, z;
	p = pow11(DPIN);
printf("p val %d\n\r",p);
	// EMPTY BUFFER
	for (int i = 0; i < 5; i++)
		bits[i] = 0 ;
	
	// REQUEST SAMPLE FROM PROCESSOR
	GPIO_REG(GPIO_OUTPUT_EN)  |= (0X1 << DPIN) ;
	//GPIO_REG(GPIO_PULLUP_EN)  |= (0X1 << DPIN) ;
	GPIO_REG(GPIO_OUTPUT_VAL) |= (0X0 << DPIN) ;
	wait_us(18000) ;									// 18 milliseconds delay
	GPIO_REG(GPIO_OUTPUT_VAL) |= (0X1 << DPIN) ;
	wait_us(40) ;								// 40 microseconds delay
	
	/*GPIO_REG(GPIO_INPUT_EN)   |= (0X1 << DPIN) ;
GPIO_REG(GPIO_PULLUP_EN)  |= (0X1 << DPIN) ;*/




	GPIO_REG(GPIO_OUTPUT_EN)  &= ~(0x1 << DPIN);
	GPIO_REG(GPIO_PULLUP_EN)  |= (0x1 << DPIN);
	GPIO_REG(GPIO_INPUT_EN)   |= (0X1 << DPIN) ;




	//DHT11 RESPONSE
	uint32_t loopcount = 10000 ;
z = GPIO_REG(GPIO_INPUT_VAL) & (pow11(DPIN));
	while(z == 0x0)	
	{
printf("1");
//z = GPIO_REG(GPIO_INPUT_VAL) & (pow11(DPIN));
//printf("value: %d\n\r",z);
		if(loopcount-- == 0)
		{
			printf("TIMEOUT ERROR") ;
			return TIMEOUT_ERROR ;
		}
z = GPIO_REG(GPIO_INPUT_VAL) & (pow11(DPIN));
	}

	loopcount = 10000 ;
	while((GPIO_REG(GPIO_INPUT_VAL) & (pow11(DPIN)) != 0x0))
	{
printf("2");
//z = GPIO_REG(GPIO_INPUT_VAL) & (pow11(DPIN));
//printf("value: %d\n\r",z);
		if(loopcount-- == 0)
		{
			printf("TIMEOUT ERROR") ;
			return TIMEOUT_ERROR ;
		}
	}


	// GETTING VALUES
	uint32_t mtime , ntime;

	for(int i = 0; i < 40; i++)
	{
		loopcount = 10000 ;
printf("vvvvvvv %ld\n\r",(GPIO_REG(GPIO_INPUT_VAL) & (pow11(DPIN))));
		while((GPIO_REG(GPIO_INPUT_VAL) & (pow11(DPIN)) ==0x0))
		{
printf("3");
//z = GPIO_REG(GPIO_INPUT_VAL) & (pow11(DPIN));
//printf("value: %d\n\r",z);
			if(loopcount-- == 0)
			{
				printf("TIMEOUT ERROR") ;
				return TIMEOUT_ERROR ;
			}
		}
	
		mtime = *((uint64_t*) (CLINT_CTRL_ADDR + CLINT_MTIME)) ;
		//unsigned long t =  mtime ;

		loopcount = 10000 ;
		
		while((GPIO_REG(GPIO_INPUT_VAL) & (pow11(DPIN)) != 0x0))
		{
printf("4");
//z = GPIO_REG(GPIO_INPUT_VAL) & (pow11(DPIN));
//printf("value: %d\n\r",z);
			if(loopcount-- == 0)
			{
			printf("TIMEOUT ERROR") ;
			return TIMEOUT_ERROR ;
			}
		}
		
		ntime = *((uint64_t*) (CLINT_CTRL_ADDR + CLINT_MTIME)) ;

		if ((ntime - mtime) > (40 * us_tick)){
			bits[idx] |= (0x1 << cnt) ;
		if (cnt == 0)
		{
			cnt = 7 ;
			idx++ ;
		}
		else
			cnt-- ;}
	}
	
	// ASSIGNING VALUES
	// bits[1] and bits[3] have zeros
	humidity    = bits[0]; 
	temperature = bits[2]; 
	
	uint8_t sum = bits[0] + bits[2];  

	if (bits[4] != sum)
	{
printf("GOT VALUES\n") ;
		printf("HUMIDITY: %d RH \t TEMPERATURE: %d *c\n",bits[0], bits[2]) ;
		printf("CHECKSUM ERROR\n") ;
		return CHECKSUM_ERROR ;
	}
	else
	{
		printf("GOT VALUES\n") ;
		printf("HUMIDITY: %d RH \t TEMPERATURE: %d *c\n",bits[0], bits[2]) ;
		return GOT_VALUES ;
	}
	wait_us(3000000);
}


	
