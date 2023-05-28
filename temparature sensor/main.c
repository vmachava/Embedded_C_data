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
#define   GPIO_PORTC_MODE           (*(volatile int *)0x40020800)
#define   GPIO_PORTC_IDR            (*(volatile int *)0x40020810)
#define   GPIO_PORTC_ODR            (*(volatile int *)0x40020814)
#define		GPIO_PORTC_PUPDR					(*(volatile int *)0x4002080C)		

#define   ADC_SMPR1						(*(volatile int *)0x4001200C)
#define   ADC_SQR3						(*(volatile int *)0x40012034)
#define   ADC_CR1	   					(*(volatile int *)0x40012004)
#define   ADC_CR2	   					(*(volatile int *)0x40012008)
#define   ADC_SR              (*(volatile int *)0x40012000)
#define   ADC_DR							(*(volatile int *)0x4001204C)
	
volatile int  POT_Value = 0;
char str1[10];
void SysTick_Init(void);
void GPIO_PORT_B_Init(void);
void Vamshi_Adc_Read(void);




void KM_delay(int num)
{
	volatile int i=0;
		for(i=0;i<1600*num;i++)
		{
			;															//1600 iterations can give 1ms delay approximatly.
		}
}

void GPIO_PORT_C_Init(void)
{
	RCC_AHB1ENR |= 0x04;																	//Enable clock for Port C
	while((RCC_AHB1ENR & 0x04) == 0);	   									//Wait till the bit is set
	GPIO_PORTC_MODE &= ~(0x3<<0);													//clear te PC1 pin for Anlaog read
	GPIO_PORTC_MODE |= (0x3<<0);											//Enable PC1 for Analog read.
}

void Vamshi_Adc_init(void)
{
	RCC_APB2ENR |=(0x1<<8);												//enable ADC1 EN 
	while(!(RCC_APB2ENR & (0x1<<8)));							// wait untill ADC1 EN identified
	ADC_SMPR1 |=0x7;  														// sample time for 480 cycles
	ADC_SQR3  |= 0x0A;														//it is represents the (1011/11/B)value to set the ADC1_11 as 1st regular conversaion 
	ADC_CR1   &=~(0x3<<24);												//load the 00 in 24-25 psitions to set 12-bit.
	ADC_CR2   |= 0x1<<10;													//set 10th bit (EOC- end of the ADC conversion select for next ADC conversation )
	ADC_CR2   |= 0x1;     												//set 0th bit (ADON) to tun on ADC1 communnication

}
void Vamshi_Adc_Read(void)
{
	ADC_CR2 |= 0x1<<30;  //start conversion (Enable software trigger)
	while(!(ADC_SR & 0x2));
	POT_Value = ADC_DR;      //Collect data from ADC_DR data register
	ADC_CR2 &= ~(0x1<<30);   //clear trigger
	KM_delay(500);
}
void tostring(char str[], int num)
{
    int i, rem, len = 0, n;
 
    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}

int main()
{
		Systick_Init();
		GPIO_PORT_B_Init();
		GPIO_PORT_C_Init();
		Vamshi_Adc_init();
		
		KM_Lcd_Init();
		KM_LCD_Write_CMD(0x01);											//clear the screen
		KM_LCD_Write_CMD(0x80);											//Force cursor to the beginning of row 1

	KM_LCD_Write_STR("Temperaure :");
		while(1)
		{
			Vamshi_Adc_Read();
			tostring(str1,POT_Value);
			KM_LCD_Write_CMD(0xC0);											//Force cursor to the beginning of row 2	
			KM_LCD_Write_STR("     ");
		  KM_LCD_Write_STR(str1);
			KM_LCD_Write_CMD(0x0E);
		}

}

