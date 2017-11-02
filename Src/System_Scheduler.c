#include "System_Scheduler.h"
#include "usart.h"
#include "AS5047D.h"

task_t task;


uint16_t ERRFL = 0;
uint16_t PROG = 0;
uint16_t DIAAGC = 0;
uint16_t ANGLEUNC = 0;

uint16_t NOP = 0;
uint16_t CORDICMAG = 0;
uint16_t ANGLECOM = 0;
uint16_t ZPOSM = 0;
uint16_t ZPOSL = 0;
uint16_t SETTINGS1 = 0;
uint16_t SETTINGS2 = 0;
float true_angle = 0.0f;


void Duty_1ms(void)
{
	
}


void Duty_2ms(void)
{
	
}

void Duty_5ms(void)
{
	
}

void Duty_10ms(void)
{
//	printf("正在运行10ms任务\r\n");
}

void Duty_20ms(void)
{
	
}

void Duty_50ms(void)
{
//	printf("正在运行50ms任务\r\n");
}

void Duty_100ms(void)
{
//		AS5047D_Write(AS4047D_SETTINGS1,0x03);
//	    Delay_ms(500);
//		ERRFL = (AS5047D_Read_16BIT(AS4047D_ERRFL)&0x07);
//		 printf("ERRFL=     0x%x\r\n",ERRFL);
//		PROG = AS5047D_Read_16BIT(AS4047D_PROG);
//		 printf("PROG=      0x%x\r\n",PROG);
//		DIAAGC = AS5047D_Read_16BIT(AS4047D_DIAAGC);
//		 printf("DIAAGC=    0x%x\r\n",DIAAGC);
//		ANGLEUNC = AS5047D_Read_16BIT(AS4047D_ANGLEUNC);
//		 printf("ANGLEUNC=  0x%x\r\n",ANGLEUNC);
//		NOP = AS5047D_Read_8BIT(AS4047D_NOP);
//		 printf("NOP=       0x%x\r\n",NOP);
//		CORDICMAG = AS5047D_Read_16BIT(AS4047D_CORDICMAG);
//		 printf("CORDICMAG= 0x%x\r\n",CORDICMAG);
//		ANGLECOM = AS5047D_Read_16BIT(AS4047D_ANGLECOM);
//		 printf("ANGLECOM=  0x%x\r\n",ANGLECOM);
//		ZPOSM = AS5047D_Read_8BIT(AS4047D_ZPOSM);
//		 printf("ZPOSM=     0x%x\r\n",ZPOSM);
//		ZPOSL = AS5047D_Read_16BIT(AS4047D_ZPOSL);
//		 printf("ZPOSL=     0x%x\r\n",ZPOSL)
//    	SETTINGS1 = AS5047D_Read_16BIT(AS4047D_SETTINGS1);
//		 printf("SETTINGS1= 0x%x\r\n\r\n",SETTINGS1);
//		SETTINGS2 = AS5047D_Read_16BIT(AS4047D_SETTINGS2);
//        printf("SETTINGS2= 0x%x\r\n",SETTINGS2);
		true_angle = AS5047D_Get_True_Angle_Value();
    	printf("true_angle=%5.2f\r\n\r\n",true_angle);
//		if(true_angle<5)LED1 =1;
//		else LED1 = 0;
//		if(true_angle>5&&true_angle<10)LED2 = 1;
//		else LED2 = 0 ;
//		if(true_angle>10&&true_angle<15)LED3 = 1;
//		else LED3 = 0 ;
//		if(true_angle>15&&true_angle<20)LED4 = 1;
//		else LED4 = 0 ;
//		if(true_angle>20)
//		{
//			LED1 = 1;
//			LED2 = 1;
//			LED3 = 1;
//			LED4 = 1;
//		}
}
void main_task(void)
{
	if(task.check_flag == 1)
	{
		Duty_1ms();
		if(task.period_cycle_2ms >= 2)
		{
			task.period_cycle_2ms = 0;
			Duty_2ms();
		}
		if(task.period_cycle_5ms >= 5)
		{
			task.period_cycle_5ms = 0;
			Duty_5ms();
		}
		if(task.period_cycle_10ms >= 10)
		{
			task.period_cycle_10ms = 0;
			Duty_10ms();
		}
		if(task.period_cycle_20ms >= 20)
		{
			task.period_cycle_20ms = 0;
			Duty_20ms();
		}
		if(task.period_cycle_50ms >= 50)
		{
			task.period_cycle_50ms = 0;
			Duty_50ms();
		}
		if(task.period_cycle_100ms >= 100)
		{
			task.period_cycle_100ms = 0;
			Duty_100ms();
		}
		task.check_flag = 0;//运行完毕标志清零
	}
}


void call_task_timer(void)
{
	task.period_cycle_2ms++;
	task.period_cycle_5ms++;
	task.period_cycle_10ms++;
	task.period_cycle_20ms++;
	task.period_cycle_50ms++;
	task.period_cycle_100ms++;
	
	if(task.check_flag == 1)
		task.err_flag++;
	else
		task.check_flag = 1;
	
}











