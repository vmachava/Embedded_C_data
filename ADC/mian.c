/* 
Experiment 11: 
Toggle PB13 (RED LED) every 100msec delay using SysTick timer With Polling using 16MHZ system clock source. 
And also test minimum and maximum delays of 16MHZ timer. 

Red Led	:	PB13

Board						:	Raayanmini Board V4.0/V4.1
Microcontroller	:	STM32F401RBT6
*/
                            																																							
//RCC registers
#define		RCC_AHB1ENR								(*(volatile int *)0x40023830)						//RCC system clock
#define		RCC_APB2ENR								(*(volatile int *)0x40023844)						//peripheral clock enable 
//GPIOB registers
#define		GPIO_PORTB_MODE						(*(volatile int *)0x40020400)						//GPIOB MODE register
#define		GPIO_PORTB_IDR						(*(volatile int *)0x40020410)						//GPIOB input data register
#define		GPIO_PORTB_ODR						(*(volatile int *)0x40020414)						//GPIOB output data register
#define		STK_CTRL            			(*(volatile int *)0xE000E010) 
#define		STK_LOAD            			(*(volatile int *)0xE000E014)
#define		STK_VAL             			(*(volatile int *)0xE000E018)

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
void SysTick_Init(void);
void GPIO_PORT_B_Init(void);
void Vamshi_Adc_Read(void);

void GPIO_PORT_B_Init(void)
{
  	RCC_AHB1ENR         |= 0x02;              //Enable clock for GPIO_PORT_A 
		while(!(RCC_AHB1ENR & 0x02));    //Wait untill Clock Enable 
	  GPIO_PORTB_MODE     &= ~(0x03<<26);       //Clear 26 and 27 bits for setting Output mode 
	  GPIO_PORTB_MODE     |= (0x01<<26);        //Set 26 bit for PB13 Output mode for RED LED
}

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
	GPIO_PORTC_MODE &= ~(0x3<<2);													//clear te PC1 pin for Anlaog read
	GPIO_PORTC_MODE |= (0x3<<2);											//Enable PC1 for Analog read.
}

void Vamshi_Adc_init(void)
{
	RCC_APB2ENR |=(0x1<<8);												//enable ADC1 EN 
	while(!(RCC_APB2ENR & (0x1<<8)));							// wait untill ADC1 EN identified
	ADC_SMPR1 |=0x7;  														// sample time for 480 cycles
	ADC_SQR3  |= 0x0B;														//it is represents the (1011/11/B)value to set the ADC1_11 as 1st regular conversaion 
	ADC_CR1   &=~(0x3<<24);												//load the 00 in 24-25 psitions to set 12-bit.
	ADC_CR2   |= 0x1<<10;													//set 10th bit (EOC- end of conversion select)
	ADC_CR2   |= 0x1;     												//set 0th bit (ADON) to tun on ADC1

}
void Vamshi_Adc_Read(void)
{
	ADC_CR2 |= 0x1<<30;  //start conversion (Enable software trigger)
	while(!(ADC_SR & 0x2));
	POT_Value = ADC_DR;      //Collect data from ADC_DR data register
	ADC_CR2 &= ~(0x1<<30);   //clear trigger
	KM_delay(500);
}

 
int main(void)
{
GPIO_PORT_C_Init();
Vamshi_Adc_init();
	
while(1)
{
	Vamshi_Adc_Read();
}
}
