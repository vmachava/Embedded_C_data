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
#define		RCC_AHB1ENR	 (*(volatile int *)0x40023830)
#define   RCC_APB2ENR  (*(volatile int *)0x40023844) 
//RCC system clock
//GPIOB registers
#define		GPIO_PORTB_MODE						(*(volatile int *)0x40020400)						//GPIOB MODE register
#define		GPIO_PORTB_ODR						(*(volatile int *)0x40020414)						//GPIOB output data register
#define   GPIO_PORTB_IDR            (*(volatile int *)0x40020410)
//GPIOC regsters
#define   GPIO_PORTC_MODE           (*(volatile int *)0x40020800)           //GPIOC MODE register
#define   GPIO_POTTC_PUPDR          (*(volatile int *)0x4002080C)           //GPIOC PULL UP register
#define		GPIO_PORTC_IDR						(*(volatile int *)0x40020810)						//GPIOB input data register
#define		GPIO_PORTC_ODR						(*(volatile int *)0x40020814)	
//STSCTG regisers
#define  SYSCTG_EXTICR1   (*(volatile int *)0x40013808)   //syscfg extenal interrupt configure 1
#define  SYSCTG_EXTICR2		(*(volatile int *)0x4001380C)		//2
#define  SYSCTG_EXTICR3		(*(volatile int *)0x40013810)		//3
#define  SYSCTG_EXTICR4		(*(volatile int *)0x40013814)		//4
//EXT1 reister
#define  EXTI_IMR		(*(volatile int *)0x40013C00)			//EXT1 Interrupt mask register
#define  EXTI_ENR		(*(volatile int *)0x40013C04)`		//EXT1 Event mask register
#define  EXTI_RTSR		(*(volatile int *)0x40013C08)   // EXT1 Rising trigger selection
#define  EXTI_FTSR		(*(volatile int *)0x40013C0C)		//EXT1 falling trigger selection
#define  EXTI_SWIER		(*(volatile int *)0x40013C10)		//EXT1 software interrupt event
#define  EXTI_PR			(*(volatile int *)0x40013C14)		//EXT1 pending register
//NVIC registers
#define  NVIC_ISER0		(*(volatile int *)0xE000E100)  //Interrupt Range is 31 to 0
#define  NVIC_ISER1		(*(volatile int *)0xE000E104)  //Interrput Range is 63 to 32
#define  NVIC_ISER2		(*(volatile int *)0xE000E108)  //Interrput Range is upto end to 
//Timers registers

#define  STK_CTRL (*(volatile int *)0xE000E010)
#define STK_LOAD	(*(volatile int *)0xE000E014)
#define STK_VAL		(*(volatile int *)0xE000E018)

//Function declarations
void Delay(int n);															//Software delay Function
void switch_ent_init(void);
void switch_interrupt_en(void);
void EXTI15_10_IRQHandler(void);
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
void switch_ent_init(void)
{
	RCC_AHB1ENR |= (0x1<<2);												//Enable PORT B clock
	while(!(RCC_AHB1ENR & 0x04));
	
	GPIO_POTTC_PUPDR |= 0x00100000;
	//GPIO_PORTC_MODE &= 0xFFCFFFFF;									//by default the mode of GPIO port 10 is input mode only.so not nessary
	//GPIO_PORTC_MODE |= 0x00100000;									//Enable OutPut Mode for PC10 
	
}
void switch_interrupt_en(void)
{
	RCC_APB2ENR |= 0x00004000;        //enable system configuratio enable block. with 14 bit for externa  interrupt 
	SYSCTG_EXTICR3 |= 0x0000200;     //select the source input for the EXTI10 external interrupt
	EXTI_IMR |= 0x0000400;					// interupt rquest from line 10 is not masked.
	EXTI_FTSR |=0x0000400;   				//falling rigger enable (for event and interrupt)for input line
	
	//Enable NVIC interrupt
	NVIC_ISER1 |=0x0000100;					//enable interrupt for EXTI15_10 position 40
}
void SysTick_Init(void)
{
	STK_CTRL |= 0x05;			//enable the timer fr HW interrupt
	STK_VAL = 0x124;			// dummy value wrter for clearring all te bits under 
	STK_LOAD = 1600;  //100 ms /16MHZ clock handler
}

void SysTick_Handler(void)
{
			GPIO_PORTB_ODR ^= (0x1<<13);   //exampe 2
}

int  counter=0,flag=0;
int status = 1;
int main()
{
	red_led_init();
	switch_ent_init();
	switch_interrupt_en();
	SysTick_Init();
	/******************************************************************************
												    LED ON/OFF program
	*******************************************************************************/

	while(1)
	{
		//GPIO_PORTB_ODR ^= (0x1<<13);  //exampe 1
		while((STK_CTRL&(1<<16)) == 0);  //exampe 1
	}
	
}


