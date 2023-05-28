#define GPIO_PORTB_MODE			*((volatile int *)0x40020400)     	//MODE register
#define GPIO_PORTB_ODR			*((volatile int *)0x40020414)     	//Output data register
#define RCC_AHB1ENR         *((volatile int *)0x40023830)      	//RCC AHB1 peripheral clock enable register
	
#define EN_ON()				    	GPIO_PORTB_ODR |= 0x100			      	//Enable pin high
#define EN_OFF()				    GPIO_PORTB_ODR &=~ (0x100)	      	//Enable pin low
#define LCD_RS_DATA_MODE		GPIO_PORTB_ODR |= (0x10)		      	//Register select high
#define LCD_RS_CMD_MODE			GPIO_PORTB_ODR &= ~(0x10)		      	//Register select low
  

void GPIO_PORT_B_Init(void);					                         	//Port-B init function
void KM_Lcd_Init(void);
void Enable(void);						                                 	//Enable select
void Higher_Nibble(char Data);
void Lower_Nibble(unsigned char Data);
void KM_LCD_Write_DATA(unsigned char Data);
void KM_LCD_Write_CMD(unsigned char cmd);
void KM_LCD_Write_STR(char *str);

