#define  STK_CTRL           (*(volatile int *)0xE000E010) 
#define  STK_LOAD           (*(volatile int *)0xE000E014)
#define  STK_VAL            (*(volatile int *)0xE000E018)
	
void Systick_Init(void);
void KM_Delay_ms(int Delay);
void KM_Delay_us(int Delay);

