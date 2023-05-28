#include "LCD.h"
#include "SysTick.h"

void Enable(void)
{
	EN_ON();
	KM_Delay_ms(1);
	EN_OFF();
	KM_Delay_ms(1);
}

void Higher_Nibble(char Data)
{
	GPIO_PORTB_ODR &= ~(0x0F);
	GPIO_PORTB_ODR |= ((Data>>4)&0x0F);
	Enable();
}

void Lower_Nibble(unsigned char Data)
{
	GPIO_PORTB_ODR &= ~(0x0F);
	GPIO_PORTB_ODR |= ((Data)&0x0F);
	Enable();
}

void KM_LCD_Write_DATA(unsigned char Data)
{	
	LCD_RS_DATA_MODE; 
	Higher_Nibble(Data);
	Lower_Nibble(Data);		
}

void KM_LCD_Write_CMD(unsigned char cmd)
{
	LCD_RS_CMD_MODE;
	Higher_Nibble(cmd);
	Lower_Nibble(cmd);		
}

void GPIO_PORT_B_Init(void)
{
	RCC_AHB1ENR |= 0x02;						      	//clock enable for portB	
	while((RCC_AHB1ENR & 0x02) == 0);	   		//verify clock enabled or not 
	GPIO_PORTB_MODE &= ~(0x00030FFF);					
	GPIO_PORTB_MODE |= 0x00010555;	  			//LCD All Pin Set as a OutPut Mode (PB0-D4,PB1-D5,PB2-D6,PB3-D7,PB4-RS,PB5-RW,PB8-EN)
	GPIO_PORTB_ODR = 0x00;		            	//Clear o/p data register
}

void KM_Lcd_Init(void)
{
	KM_Delay_ms(20);
	KM_LCD_Write_CMD(0x33);
	KM_Delay_ms(1);
	KM_LCD_Write_CMD(0x32);
	KM_Delay_ms(1);
	KM_LCD_Write_CMD(0x0C);
	KM_LCD_Write_CMD(0x01);		//clear the screen
	KM_LCD_Write_CMD(0x80);		//force the cursor to the beginning of 1st line
}

void KM_LCD_Write_STR(char *str)
{
	int i;
	for(i=0; str[i]!='\0'; i++)
	{
		KM_LCD_Write_DATA(str[i]);
	}
}
