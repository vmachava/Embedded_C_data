/*Experiment 1: 
Toggle RED LED. ON time is 50ms and OFF time is 1300ms.

RED LED -> PB13

Board						:	Raayanmini Board V4.0/V4.1
Microcontroller	:	STM32F401RBT6
*/

/**************************************************************************************************
RCC_AHB1ENR physical address = 0x4002 3800  of Base address + 0x30 offset register address of RCC AHB1 peripheral clock enable 
GPIO_PORTB_MoODE physical address = 0x4002 0400 of Base adddress register boundary + 0x00 offeset register of GPIO_PORTB GPIO port mode register
GPIO_PORTB OUTput Driver regiser for hisical address = 0x4002 0400 of Base adddress register boundary + 0x04 offset register got GPIO_PORTB.
***************************************************************************************************/
//RCC registers
#define		RCC_AHB1ENR								(*(volatile int *)0x40023830)						//RCC system clock
//GPIOB registers
#define		GPIO_PORTB_MODE						(*(volatile int *)0x40020400)						//GPIOB MODE register
#define		GPIO_PORTB_ODR						(*(volatile int *)0x40020414)						//GPIOB output data register

#define   GPIO_PORTC_MODE           (*(volatile int *)0x40020800)           //GPIOC MODE register
#define   GPIO_PORTC_PUPDR          (*(volatile int *)0x4002080C)           //GPIOC PULL UP register
#define		GPIO_PORTC_IDR						(*(volatile int *)0x40020810)						//GPIOB input data register
//Function declarations
void Delay(int n);															//Software delay Function

void Delay(int n)
{
	volatile int i;
	for(i=0; i < 1600*n; i++);
}
void red_led_init(void)
{
	RCC_AHB1ENR |= (0x1<<1);												//Enable PORT B clock
	while(!(RCC_AHB1ENR & 0x02));
	GPIO_PORTB_MODE &= 0xF3FFFFFF;									//clear the respective bits
	GPIO_PORTB_MODE |= 0x04000000;									//Enable OutPut Mode for PB13 
	
}
void switch_init(void)
{
	RCC_AHB1ENR |= (0x1<<2);												//Enable PORT B clock
	while(!(RCC_AHB1ENR & 0x04));
	
	GPIO_PORTC_PUPDR |= 0x00100000;
	//GPIO_PORTC_MODE &= 0xFFCFFFFF;									//by default the mode of GPIO port 10 is input mode only.so not nessary
	//GPIO_PORTC_MODE |= 0x00100000;									//Enable OutPut Mode for PC10 
	
}
int switch_status(int pin_no_status)
{
		int ret = GPIO_PORTC_IDR&(0x1<<pin_no_status);
	
return ret;	
}

int status = 1;
int main()
{
	red_led_init();
	switch_init();
	
	/******************************************************************************
												    LED ON/OFF program
	*******************************************************************************
	GPIO_PORTB_ODR |= 0x1 << 13;										//Turn off the Red Led
	while(1)
	{
		GPIO_PORTB_ODR &= ~(0x1 << 13);								//Turn on the Red Led
		Delay(600);																		//Delay of 50ms
		GPIO_PORTB_ODR |= (0x1 << 13);								//Turn off the Red Led
		Delay(1300);																	//Delay of 1300ms
	}
	*******************************************************************************/
	
	GPIO_PORTB_ODR |= 0x1 << 13;										//Turn off the Red Led
	while(1)
	{	
		if(!(GPIO_PORTC_IDR&(0x1<<10)))
		{
			if(status == 1)
			{
				status = 0;
				GPIO_PORTB_ODR &= ~(0x1 << 13);								//Turn on the Red Led
			}		
			else if(status == 0)
			{
				GPIO_PORTB_ODR |= (0x1 << 13);								//Turn off the Red Led
				status = 1;
			}
			else
			{
				;
			}
		}													
	}
	
}

