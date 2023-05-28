/**
	Experiment 19:  Develop a Communication between two Raayan mini boards using UART1 protocol.
	Use USART1 Receiver interrupt. The switches (PC8 and PC9) are inputs. 
	LEDs (PB13, PB14) are outputs. 
	And the UART1 is used to communicate.
	
	SW_UP	-	PC8
	SW_DN	-	PC9
	
	USART1_TX	-	PA9
	USART1_RX	-	PA10
	
|-----------------------------|-----------------------------------|	
|					Board1							|							Board2								|
|-----------------------------|-----------------------------------|	
|PRESS SW_UP-->TX 'R'					|			RX 'R'-->Turn ON RED LED			|
|PRESS SW_DN-->TX 'G'					|			RX 'G'-->Turn ON GREEN LED		|
|RX 'R'-->Turn ON RED LED			|			PRESS SW_UP-->TX 'R'					|
|RX 'G'-->Turn ON GREEN LED		|			PRESS SW_DN-->TX 'G'					|
|-----------------------------|-----------------------------------|

Board:					Raayanmini Board V4.0/V4.1
Microcontroller:	STM32F401RBT6
**/

//RCC registers
#define		RCC_AHB1ENR								(*(volatile int *)0x40023830)						//system clock
#define		RCC_APB2ENR								(*(volatile int *)0x40023844)						//peripheral clock enable 
//GPIOA registers
#define		GPIO_PORTA_MODE						(*(volatile int *)0x40020000)						//GPIOA MODE register
#define		GPIO_PORTA_ODR						(*(volatile int *)0x40020014)						//GPIOA output data register
#define		GPIO_PORTA_AFRL						(*(volatile int *)0x40020020)						//GPIOA alternate function low register
#define		GPIO_PORTA_AFRH						(*(volatile int *)0x40020024)						//GPIOA alternate function high register
//GPIOB registers
#define		GPIO_PORTB_MODE						(*(volatile int *)0x40020400)						//GPIOB MODE register
#define		GPIO_PORTB_IDR						(*(volatile int *)0x40020410)						//GPIOB input data register
#define		GPIO_PORTB_ODR						(*(volatile int *)0x40020414)						//GPIOB output data register
//GPIOC registers
#define		GPIO_PORTC_MODE						(*(volatile int *)0x40020800)						//GPIOC MODE register
#define		GPIO_PORTC_PUPDR					(*(volatile int *)0x4002080C)						//GPIOC pull-up/pull-down register
#define		GPIO_PORTC_IDR						(*(volatile int *)0x40020810)						//GPIOC input data register
#define		GPIO_PORTC_ODR						(*(volatile int *)0x40020814)						//GPIOC output data register
#define		GPIO_PORTC_AFRL						(*(volatile int *)0x40020820)						//GPIOC alternate function low register
//USART1 registers
#define		USART1_SR									(*(volatile int *)0x40011000)						//USART1 Status register
#define		USART1_DR									(*(volatile int *)0x40011004)						//USART1 Data register   
#define		USART1_BRR								(*(volatile int *)0x40011008)						//USART1 Baud rate register
#define		USART1_CR1								(*(volatile int *)0x4001100C)						//USART1 Control register 1
#define		USART1_CR2								(*(volatile int *)0x40011010)						//USART1 Control register 2
//NVIC registers
#define		NVIC_ISER0								(*(volatile int *)0xE000E100)						//Interrupt Range is 31 to 0
#define		NVIC_ISER1								(*(volatile int *)0xE000E104)						//Interrupt Range is 63 to 32
#define		NVIC_ISER2								(*(volatile int *)0xE000E108)						//Interrupt Range is upto end to 64

/*Function prototypes*/
void GPIO_LED_Init(void);																				//Initialize Red and Green Leds
void GPIO_Switch_Init(void);																		//Initialize SW1(PC9) and SW2(PC8)
void USART1_Init_Config(void);																	//USART1 Enable and configure
void USART1_Interrupt_EN(void);																	//Enable interrupts for USART Receiver
void USART1_TX_Outchar(unsigned char out);											//function to transmit one byte
//unsigned char USART1_RX_Inchar(void);														//Blocking function to receive one byte
//unsigned char USART1_RX_InChar_NonBlock (void);									//Non blocking function to receive one byte
//int USART1_RX_InChar_TIMEOUT (unsigned int);										//Timeout function to receive one byte
void USART1_IRQHandler(void);																		//Usart1 interrupt handler function prototype


volatile char ch;
volatile int flag=0;

void GPIO_LED_Init(void)
{
	RCC_AHB1ENR |= (0x01<<1);																				//Enable PORT B clock
	while(!(RCC_AHB1ENR & 0x02));
	GPIO_PORTB_MODE &= 0xC3FFFFFF;
	GPIO_PORTB_MODE |= 0x14000000;																	//Set both LEDs (PB13 and PB14) mode as output mode-01
	GPIO_PORTB_ODR |= (0x1<<13) | (0x01<<14);												//Turn OFF both the LEDs
	
}
void GPIO_Switch_Init(void)
{
/*	Port C switches Initialization	*/
	RCC_AHB1ENR |= (0x1<<2);																		//Enable Port C
	while(!(RCC_AHB1ENR & 0x04));
	GPIO_PORTC_MODE &= ~(0x0000F000);														//Set mode as input mode -00
	GPIO_PORTC_PUPDR &= ~(0x0000F000);
	GPIO_PORTC_PUPDR |= 0x00050000;															//Enable Pull up resistors for both switches
}
void USART1_Init_Config(void)
{
	RCC_AHB1ENR |= (0x01);																			//Enable PORT A
	while(!(RCC_AHB1ENR & 0x01));
	GPIO_PORTA_MODE &= ~(0x003C0000);														//clear only the corresponding fields
	GPIO_PORTA_MODE |= 0x00280000;															//Set PA9 and PA10 MODE to Alternate function mode
	GPIO_PORTA_AFRH &= ~(0x00000770);														//clear only the corresponding fields
	GPIO_PORTA_AFRH |= 0x00000770;															//Select the alternate functionality USART1
	RCC_APB2ENR |= (0x1<<4);																		//Enable USART1
	while(!(RCC_APB2ENR & 0x10));
	USART1_BRR = 0x683;																					//Set baud rate 9600 =>Baud Rate = System clock/8*(2-0)*USARTDIV
																																	//USARTDIV = System clock/8*(2-0)*BaudRate = 16000000/16*9600 =
																																	//16000000/(16*9600)=104.1875 ==> 104+0.18==>68(integral part)
	USART1_CR1 &= ~(0x0000200C);																//clear the fields
	USART1_CR1 |= 0x0000200C;																		//USART En-1, Tx En-1, Rx En-1
}
void USART1_Interrupt_EN(void)
{
	USART1_CR1 &= ~0x00000020;
	USART1_CR1 |= 0x00000020;																//Enable USART1 Receiver interrupt RXNEIE-1
	NVIC_ISER1 |= 0x00000020;																//Enable UART1 Interrupt in NVIC													
}
void USART1_TX_Outchar(unsigned char out)
{
	while(!(USART1_SR & 0x80));																	//wait till TXE bit in usart status register is set
	USART1_DR = out;																						//transmit the byte
}
//unsigned char USART1_RX_Inchar(void)
//{
//	while(!(USART1_SR & 0x20));																	//wait till RXNE bit in usart status register is set
//	return (USART1_DR & 0xFF);																	//receive the byte
//}
//unsigned char USART1_RX_InChar_NonBlock(void)
//{
//	if((USART1_SR & 0x20))																			//check if RXNE bit in usart status register is set
//	{
//		return (USART1_DR & 0xFF);																//receive the byte
//	}
//	return 0;
//}
//int USART1_RX_InChar_TIMEOUT (unsigned int time)
//{
//	while(time--)
//	{
//		if((USART1_SR & 0x20))																		//check if RXNE bit in usart status register is set
//		{
//			ch = (USART1_DR & 0xFF);																//receive the byte
//			return 0;
//		}
//	}
//	return -1;
//}


int main(){
	GPIO_LED_Init();																						//Initialize Green(PA8) and Red(PC5) Leds
	GPIO_Switch_Init();																					//Initialize SW1(PC9) and SW2(PC8) switches
	USART1_Init_Config();																				//Initialize USART and configure PA9 and PA10	
	USART1_Interrupt_EN();																			//Enable interrupt for USART receiver
	while(1)
	{
		if(!(GPIO_PORTC_IDR & (0x01<<8)))														//Check if SW2(PC8) is pressed
		{
			USART1_TX_Outchar('R');																		//Transmit 'R' to the other board/host via USART Tx
			while(!(GPIO_PORTC_IDR & (0x01<<8)));											//wait till the SW value is high again
		}
		else if(!(GPIO_PORTC_IDR & (0x01<<9)))											//Check if SW1(PC9) is pressed
		{
			USART1_TX_Outchar('G');																		//Transmit 'G' to the other board/host via USART Tx
			while(!(GPIO_PORTC_IDR & (0x01<<9)));											//wait till the SW value is high again
		}
		else if(flag == 1)																					//Check if the receiver received any data 
		{
			flag = 0;
			if(ch=='R' || ch=='r')																		//check if the received data is 'R' or 'r'
			{
				USART1_TX_Outchar(ch);																	//Transmit the received byte to the host/Teraterm for the user to confirm
				GPIO_PORTB_ODR ^= (0x1<<13);															//Toggle the RED LED
			}
			if(ch=='G' || ch=='g')																		//check if the received data is 'G' or 'g'
			{
				USART1_TX_Outchar(ch);																	//Transmit the received byte to the host/Teraterm for the user to confirm
				GPIO_PORTB_ODR ^= (0x1<<14);														//Toggle the GREEN LED
			}
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
