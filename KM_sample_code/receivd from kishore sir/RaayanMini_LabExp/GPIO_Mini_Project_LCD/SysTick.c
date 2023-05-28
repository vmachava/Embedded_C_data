#include "SysTick.h"

void Systick_Init(void)
{
		STK_CTRL |= 0x05; 				//Select Clock source as 16mhz( Processor clock (AHB))  |  SysTick exception request enable   |   Systick Counter enable  
		STK_VAL = 0x123;  				//write of any value clears the field to 0, and also clears the COUNTFLAG bit in the STK_CTRL register to 0.
}

void KM_Delay_ms(int Delay)
{
		int i;
  	STK_VAL = 0x123;											//write of any value clears the field to 0, and also clears the COUNTFLAG bit in the STK_CTRL register to 0.
		STK_LOAD=16000;  											//16000 for 1ms delay
		for(i=0;i<Delay;i++)
	  {
		   while((STK_CTRL&0x1<<16)==0);
		}
}	

void KM_Delay_us(int Delay)
{
		int i;
	  STK_VAL = 0x123;											//write of any value clears the field to 0, and also clears the COUNTFLAG bit in the STK_CTRL register to 0.
		STK_LOAD = 16; 												//16 for 1us delay
		for(i=0;i<Delay;i++)
	  {
		  while((STK_CTRL&0x1<<16)==0);
		}
}

