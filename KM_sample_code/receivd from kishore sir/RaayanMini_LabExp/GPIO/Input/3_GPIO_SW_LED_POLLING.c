/*Experiment 3: 
Whenever SW_UP is pressed RED LED is ON, whenever SW_DN is pressed GREEN LED is ON.
Assign break points after if condition and click run button in debugger window. 
Whenever SW1/SW2 presses program stops at any one of the breakpoint. 

GREEN LED 	- PB14
RED LED 		- PB13
SW_UP				- PC8
SW_DN				- PC9

Board						:	Raayanmini Board V4.0/V4.1
Microcontroller	:	STM32F401RBT6
*/

//RCC registers
#define		RCC_AHB1ENR								(*(volatile int *)0x40023830)						//RCC system clock
//GPIOB registers
#define		GPIO_PORTB_MODE						(*(volatile int *)0x40020400)						//GPIOB MODE register
#define		GPIO_PORTB_IDR						(*(volatile int *)0x40020410)						//GPIOB input data register
#define		GPIO_PORTB_ODR						(*(volatile int *)0x40020414)						//GPIOB output data register
//GPIOC registers
#define		GPIO_PORTC_MODE						(*(volatile int *)0x40020800)						//GPIOC MODE register
#define		GPIO_PORTC_PUPDR					(*(volatile int *)0x4002080C)						//GPIOC pull-up/pull-down register
#define		GPIO_PORTC_IDR						(*(volatile int *)0x40020810)						//GPIOC input data register
#define		GPIO_PORTC_ODR						(*(volatile int *)0x40020814)						//GPIOC output data register

//Function declarations
void LED_INIT(void);																	//Initialize and configure Red and Green Leds
void SWITCH_INIT(void);																//Initialize and configure sw_up and sw_dn


void LED_INIT(void)
{
	RCC_AHB1ENR |= 0x02;  																//Enable clock for Port B	
	while((RCC_AHB1ENR & 0x02) == 0);	   									//Wait till the bit is set
	GPIO_PORTB_MODE &= 0xC3FFFFFF;												//clear bits
	GPIO_PORTB_MODE |= 0x14000000;												//Enable OutPut Mode for Red(PB13) bit 26-high and Green(PB14) bit 28-high Leds
	GPIO_PORTB_ODR |= 0x1<<13;														//bit set to Turn off the Red Led
	GPIO_PORTB_ODR |= 0x1<<14;														//bit set to Turn off the Green Led
}
void SWITCH_INIT(void)
{
	RCC_AHB1ENR |= 0x04;																	//Enable clock for Port C
	while((RCC_AHB1ENR & 0x04) == 0);	   									//Wait till the bit is set
	GPIO_PORTC_MODE &= 0xFFF0FFF;													//configure switches as input mode
	GPIO_PORTC_PUPDR |= 0x00050000;												//Enable pull-up for the switches
}
int main()
{
	LED_INIT();
	SWITCH_INIT();
	while(1)
	{
		if(!(GPIO_PORTC_IDR & (0x1<<8)))										//check the SW_UP(PC8) bit
		{
			GPIO_PORTB_ODR &= ~(0x1<<13);											//clear 13th bit to Turn On the Red(PB13) Led
		}
		if(!(GPIO_PORTC_IDR & (0x1<<9)))										//check the SW_DN(PC9) bit
		{
			GPIO_PORTB_ODR &= ~(0x1<<14);											//clear 14th bit to Turn On the Green(PB13) Led
		}
		//GPIO_PORTB_ODR |= 0x1<<13;													//bit set to Turn off the Red Led
		//GPIO_PORTB_ODR |= 0x1<<14;													//bit set to Turn off the Green Led
	}
}
