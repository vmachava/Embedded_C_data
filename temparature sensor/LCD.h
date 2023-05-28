#include <stdint.h>

#define		RCC_APB2ENR								(*(volatile int *)0x40023844)						//peripheral clock enable 
#define   RCC_AHB1ENR         				(*(volatile int *)0x40023830)      	//RCC AHB1 peripheral clock enable register

#define IS_GPIO_PIN_ACTION(ACTION) (((ACTION) == GPIO_PIN_RESET) || ((ACTION) == GPIO_PIN_SET))
#define IS_GPIO_PIN(PIN)           ((((PIN) & GPIO_PIN_MASK ) != 0x00U) && (((PIN) & ~GPIO_PIN_MASK) == 0x00U))
#define GPIO_PIN_MASK              0x0000FFFFU /* PIN mask for assert test */

#define GPIO_PORTB_MODE			*((volatile int *)0x40020400)     	//MODE register
#define GPIO_PORTB_ODR			*((volatile int *)0x40020414)     	//Output data register

		
#define R1_GPIO_PORTB0		  (*(volatile int *)0x40020419)
#define R2_GPIO_PORTB1			(*((volatile int *)0x40020419)
#define R3_GPIO_PORTB2			(*(volatile int *)0x40020419)
#define R4_GPIO_PORTB3			(*(volatile int *)0x40020419)

#define C1_GPIO_PORTB4			(*(volatile int *)0x40020410)
#define C2_GPIO_PORTB5			(*(volatile int *)0x40020410)
#define C3_GPIO_PORTB6			(*(volatile int *)0x40020410)
#define C4_GPIO_PORTB7			(*(volatile int *)0x40020410)
	




#define GPIO_PIN_0                 ((uint16_t)0x0001)  /* Pin 0 selected    */
#define GPIO_PIN_1                 ((uint16_t)0x0002)  /* Pin 1 selected    */
#define GPIO_PIN_2                 ((uint16_t)0x0004)  /* Pin 2 selected    */
#define GPIO_PIN_3                 ((uint16_t)0x0008)  /* Pin 3 selected    */
#define GPIO_PIN_4                 ((uint16_t)0x0010)  /* Pin 4 selected    */
#define GPIO_PIN_5                 ((uint16_t)0x0020)  /* Pin 5 selected    */
#define GPIO_PIN_6                 ((uint16_t)0x0040)  /* Pin 6 selected    */
#define GPIO_PIN_7                 ((uint16_t)0x0080)  /* Pin 7 selected    */



#define C1_PORT GPIOA
#define C1_PIN GPIO_PIN_3

#define C2_PORT GPIOA
#define C2_PIN GPIO_PIN_2

#define C3_PORT GPIOA
#define C3_PIN GPIO_PIN_1

#define C4_PORT GPIOA
#define C4_PIN GPIO_PIN_0



#define EN_ON()				    	GPIO_PORTB_ODR |= 0x100			      	//Enable pin high
#define EN_OFF()				    GPIO_PORTB_ODR &=~ (0x100)	      	//Enable pin low
#define LCD_RS_DATA_MODE		GPIO_PORTB_ODR |= (0x10)		      	//Register select high
#define LCD_RS_CMD_MODE			GPIO_PORTB_ODR &= ~(0x10)		      	//Register select low
  
typedef struct
 {
    uint32_t CRL;
    uint32_t CRH;
   uint32_t IDR;
    uint32_t ODR;
   uint32_t BSRR;
    uint32_t BRR;
   uint32_t LCKR;
 } GPIO_TypeDef;

typedef enum
{
   GPIO_PIN_RESET = 0,GPIO_PIN_SET
}GPIO_PinState;

void GPIO_PORT_B_Init(void);					                         	//Port-B init function
void KM_Lcd_Init(void);
void Enable(void);						                                 	//Enable select
void Higher_Nibble(char Data);
void Lower_Nibble(unsigned char Data);
void KM_LCD_Write_DATA(unsigned char Data);
void KM_LCD_Write_CMD(unsigned char cmd);
void KM_LCD_Write_STR(char *str);


void HAL_GPIO_WritePin(GPIO_TypeDef * 	GPIOx,uint16_t 	GPIO_Pin,GPIO_PinState 	PinState);
