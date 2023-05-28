/**
	Write an Embedded C Program and implement USART Non-Blocking and USART TIMEOUT Functions.
	a. unsigned char USART1_RX_Inchar(void);
	b. unsigned char USART1_RX_InChar_NonBlock (void);
	c. int USART1_RX_InChar_TIMEOUT (unsigned int);
	
	USART1_TX	-	PA9
	USART1_RX	-	PA10

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

/*Function prototypes*/
void GPIO_PORTA_Init(void);																		//Port A Initialization
void USART1_Init_Config(void);																//USART1 Initialization
void USART1_TX_Outchar(unsigned char out);										//function to transmit one byte
unsigned char USART1_RX_Inchar(void);													//function to receive one byte
unsigned char USART1_RX_InChar_NonBlock (void);								//Non blocking function to receive one byte
int USART1_RX_InChar_TIMEOUT (unsigned int);									//Timeout function to receive one byte


volatile char ch = 'A';


void GPIO_PORTA_Init(void)
{
	RCC_AHB1ENR |= (0x01);															//Enable PORT A
	while(!(RCC_AHB1ENR & 0x01));
	GPIO_PORTA_MODE &= ~(0x3C0000);											//clear only the corresponding fields
	GPIO_PORTA_MODE |= 0x280000;												//Set PA9 and PA10 MODE to Alternate function mode
	GPIO_PORTA_AFRH &= ~(0x00000770);										//clear only the corresponding fields
	GPIO_PORTA_AFRH |= 0x00000770;											//Select the alternate functionality USART1
}
void USART1_Init_Config(void)
{
	RCC_APB2ENR |= (0x1<<4);														//Enable USART1
	while(!(RCC_APB2ENR & 0x10));
	USART1_BRR = 0x683;																	//Set baud rate 9600 =>Baud Rate = System clock/8*(2-0)*USARTDIV
																											//USARTDIV = System clock/8*(2-0)*BaudRate = 16000000/16*9600 =
																											//16000000/(16*9600)=104.1875 ==> 104+0.18==>68(integral part/Hex value of 104)
											
	USART1_CR1 &= ~(0x200C);														//clear the fields
	USART1_CR1 |= 0x200C;																//USART En-1, Tx En-1, Rx En-1
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
unsigned char USART1_RX_InChar_NonBlock(void)
{
	if((USART1_SR & 0x20))
	{
		return (USART1_DR & 0xFF);
	}
	return 0;
}
int USART1_RX_InChar_TIMEOUT (unsigned int time)
{
	while(time--)
	{
		if((USART1_SR & 0x20))
		{
			ch = (USART1_DR & 0xFF);
			return 0;
		}
	}
	return -1;
}

volatile int count;															//Incrementing count is the other task we do that is blocked/not blocked by the inchar() function

int main(){
	GPIO_PORTA_Init();																				//Initialize and configure PA9 and PA10
	USART1_Init_Config();																			//Initialize and configure USART1
	while(1)
	{
		
//************	BLOCKING	******************//
//		USART1_TX_Outchar(ch);																	//Transmit
//		count++;																								//count increment is blocked when nothing is received
//		ch = USART1_RX_Inchar();																//Blocking function-Receiver
		
		
//**********	NON BLOCKING	****************//
//		USART1_TX_Outchar(ch);																	//Transmit
//		count++;																								//count increment is happening without blocking
//		ch = USART1_RX_InChar_NonBlock();												//Non-Blocking function-receiver
	
		
//**********	TIMEOUT		********************//
		count++;																							//count increment is happening without blocking
		if(USART1_RX_InChar_TIMEOUT(10000)==0)								//Timeout function-receiver	//send a character from teraterm
		{
			USART1_TX_Outchar(ch);
		}
	}
}
