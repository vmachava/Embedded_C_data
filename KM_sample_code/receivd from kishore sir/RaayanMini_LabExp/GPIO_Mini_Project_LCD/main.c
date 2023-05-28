/*
GPIO MINI PROJECT: 
Initialize, configure and print GPIO MINI PROJECT on the LCD
PB0		-	D4
PB1		-	D5
PB2		-	D6
PB3		-	D7
PB4		-	RS
PB5		-	RW
PB8		-	EN

Board						:	Raayanmini Board V4.0/V4.1
Microcontroller	:	STM32F401RBT6
*/


#include "LCD.h"
#include "SysTick.h"
#include <stdio.h>

int main()
{
		Systick_Init();
		GPIO_PORT_B_Init();
		KM_Lcd_Init();
	
		KM_LCD_Write_CMD(0x01);											//clear the screen
		KM_LCD_Write_CMD(0x80);											//Force cursor to the beginning of row 1
//		KM_LCD_Write_DATA('A');
	  KM_LCD_Write_STR("    GPIO-LCD    ");
		KM_LCD_Write_CMD(0xC0);											//Force cursor to the beginning of row 2
		KM_LCD_Write_STR("  MINI PROJECT  ");
}

