GPIO MINI PROJECT
-----------------
Write a program to Initialize and configure LCD. And also print GPIO MINI PROJECT on the LCD.
Configure the below pins.
PB0	-	D4
PB1	-	D5
PB2	-	D6
PB3	-	D7
PB4	-	RS
PB5	-	RW
PB8	-	EN
Use seperate files for each peripherals required for the project. 
------------------------------------------------------------------------------------------------------------
Systick.c	-	Systick timer provides the time base. This file consists of the systick timer 
			initialization functions and systick timer based delay functions only. eg:
	void Systick_Init(void) - Initialization
	void KM_Delay_ms(int Delay) - Delay in milliseconds
	void KM_Delay_us(int Delay) - Delay in microseconds

LCD.c		-	This file consists of some functions required to operate LCD module. We use a 16x2
			monochrome LCD device. It has 8 data pins and 3 control pins. We need to cofigure 
			it to a 4 bit mode. So we need to define certain functions to communicate the LCD
			fairly well.
	void Higher_Nibble(char Data);		- sends higher nibble of the data.
	void Lower_Nibble(unsigned char Data);	- sends lower nibble of the data.
	void KM_LCD_Write_DATA(unsigned char Data); - Passes one byte data to print on the LCD.
	void KM_LCD_Write_CMD(unsigned char cmd); - Passes one byte commands to configure LCD as per the need.
	void KM_LCD_Write_STR(char *str);	- write a string on the LCD
	KM_Lcd_Init(void);			- Initialize and configure LCD using GPIO configuration

-------------------------------------------------------------------------------------------------------------						  
PROGRAM: 
- In the main() function, initialize the respective ports, systick timer and LCD.
- In the infinite loop, clear the screen, force the cursor to the respective position and print the message 
  to print on the LCD using KM_LCD_Write_CMD(), KM_LCD_Write_DATA() and KM_LCD_Write_STR() functions.
 
-------------------------------------------------------------------------------------------------------------

