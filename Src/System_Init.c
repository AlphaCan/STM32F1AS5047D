#include "System_Init.h"

uint8_t InitFinish;	//初始化完成标志


void AS5047D_system_Init(void)
{
	
	MX_GPIO_Init();
	Interval_Time_Init();
    MX_SPI2_Init();
    MX_USART1_UART_Init();
	Delay_ms(100);
	AS5047D_Init();
	AS5047D_SetZero();
	InitFinish = 1;
	
}










