/**
	Experiment 18: USART RX interrupts
	Write a Pseudo code and Embedded C program to USART6 Configuration using the below conditions.
	a. System Clock Initialization to 16MHZ
	b. 9600 8N1  (1+8+0+1=10 bits)
	Use USART Receiver interrupt.
	Assign a Hardware breakpoint in USART1_IRQHandler and run program. 
	Program execution time sends ‘A’ character from host system using Tera term application. 
	Verify results in ISR.

Platform:					Raayanmini Board V4.0/V4.1
Microcontroller:	STM32F401RBT6
**/

//RCC registers
#define		RCC_AHB1ENR					(*(volatile int *)0x40023830)				//system clock
#define		RCC_APB2ENR					(*(volatile int *)0x40023844)				//peripheral clock enable   
//GPIOA registers
#define		GPIO_PORTA_MODE			(*(volatile int *)0x40020000)     	//GPIOA MODE register
#define		GPIO_PORTA_ODR			(*(volatile int *)0x40020014)     	//GPIOA output data register
#define		GPIO_PORTA_AFRH			(*(volatile int *)0x40020024)				//GPIOA alternate function high register
//USART1 registers
#define		USART1_SR						(*(volatile int *)0x40011000)				//USART1 Status register
#define		USART1_DR						(*(volatile int *)0x40011004)				//USART1 Data register   
#define		USART1_BRR					(*(volatile int *)0x40011008)				//USART1 Baud rate register
#define		USART1_CR1					(*(volatile int *)0x4001100C)				//USART1 Control register 1
#define		USART1_CR2					(*(volatile int *)0x40011010)				//USART1 Control register 2
//NVIC registers
#define		NVIC_ISER0					(*(volatile int *)0xE000E100)				//Interrupt Range is 31 to 0
#define		NVIC_ISER1					(*(volatile int *)0xE000E104)				//Interrupt Range is 63 to 32
#define		NVIC_ISER2					(*(volatile int *)0xE000E108)				//Interrupt Range is upto end to 64

/*Function prototypes*/
void GPIO_PORTA_Init(void);																			//Port A Initialization
void USART1_Init_Config(void);																	//USART1 Initialization
void USART1_Interrupt_EN(void);																	//Enable interrupts for USART Receiver
void USART1_TX_Outchar(unsigned char out);											//function to transmit one byte
unsigned char USART1_RX_Inchar(void);														//function to receive one byte
void USART1_IRQHandler(void);																		//USART1 IRQ handler

void GPIO_PORTA_Init(void)
{
	RCC_AHB1ENR |= (0x01);																	//Enable PORT A
	while(!(RCC_AHB1ENR & 0x01));
	GPIO_PORTA_MODE &= ~(0x003C0000);												//clear only the corresponding fields
	GPIO_PORTA_MODE |= 0x00280000;													//Set PA9 and PA10 MODE to Alternate function mode
	GPIO_PORTA_AFRH &= ~(0x00000770);												//clear only the corresponding fields
	GPIO_PORTA_AFRH |= 0x00000770;													//Select the alternate functionality USART1
}
void USART1_Init_Config(void)
{
	RCC_APB2ENR |= (0x1<<4);																//Enable USART1
	while(!(RCC_APB2ENR & 0x10));
	USART1_BRR = 0x683;																			//Set baud rate 9600 =>Baud Rate = System clock/8*(2-0)*USARTDIV
																														//USARTDIV = System clock/8*(2-0)*BaudRate = 16000000/16*9600 =
																														//16000000/(16*9600)=104.1875 ==> 104+0.18==>68(integral part)											
	USART1_CR1 &= ~(0x0000200C);														//clear the fields
	USART1_CR1 |= 0x0000200C;																//USART En-1, Tx En-1, Rx En-1
}
void USART1_Interrupt_EN(void)
{
	USART1_CR1 &= ~0x00000020;
	USART1_CR1 |= 0x00000020;																//Enable USART1 Receiver interrupt RXNEIE->1
	NVIC_ISER1 |= 0x00000020;																//Enable UART1 Interrupt in NVIC													
}
void USART1_TX_Outchar(unsigned char out)
{
	while(!(USART1_SR & 0x80));
	USART1_DR = out;
}
unsigned char USART1_RX_Inchar(void)
{
	while(!(USART1_SR & 0x20));
	return (USART1_DR & 0xFF);
}

volatile char ch;
int flag = 0;

int main(){
//initialisation and configuration
	GPIO_PORTA_Init();																			//Initialize and configure PA9 and PA10
	USART1_Init_Config();																		//Initialize and configure USART1
	USART1_Interrupt_EN();																	//Initialize Interrupt for USART receiver
//infinite loop begin
	while(1)
	{
		if(flag == 1)
		{
			USART1_TX_Outchar(ch);
			flag = 0;
		}
	}
}

void USART1_IRQHandler(void)
{
	if(USART1_SR & 0x20)
	{
		ch = (USART1_DR & 0xFF);
		flag = 1;
	}
}
