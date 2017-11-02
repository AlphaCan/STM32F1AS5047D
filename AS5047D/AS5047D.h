#ifndef __AS5047D_H
#define __AS5047D_H
#include "sys.h" 
#include "spi.h"
#include "usart.h"
#include "system_time.h"
// AS4047D Register Addresses

/** volatile **/
#define AS4047D_NOP 0x0000
#define AS4047D_ERRFL 0x0001
#define AS4047D_PROG 0x0003
#define AS4047D_DIAAGC 0x3FFC
#define AS4047D_CORDICMAG 0x3FFD
#define AS4047D_ANGLEUNC 0x3FFE
#define AS4047D_ANGLECOM 0x3FFF

/** non-volatile **/
#define AS4047D_ZPOSM 0x0016
#define AS4047D_ZPOSL 0x0017
#define AS4047D_SETTINGS1 0x0018
#define AS4047D_SETTINGS2 0x0019

#define AS4047D_RD 0x4000    // bit 14 = "1" is Read + parity even
#define AS4047D_WR 0x3FFF    // bit 14 = "0" is Write

#define	AS5047D_CSN		PBout(12)  		//片选信号
#define AS5047D_SCK     PBout(13)
#define AS5047D_MISO    PBin(14)
#define AS5047D_MOSI    PBout(15)

#define AS5047D_Offset_Num  1000     //采集多少个数据用于零点校准


#define LED1             PCout(0)
#define LED2             PCout(1)
#define LED3             PCout(2)
#define LED4             PCout(3)

#define HARD_SPI_MODE   1  //1：使能硬件SPI 0使能软模拟SPI

uint16_t parity(uint16_t x);
void AS5047D_Write(uint16_t address, uint16_t data);
uint16_t AS5047D_Read_16BIT(uint16_t address);
//uint16_t AS5047D_Read_8BIT(uint16_t address);
void AS5047D_Check_Transmission_Error(void);
void AS5047D_SetZero(void);
uint16_t AS5047D_GetZero(void);
void AS5047D_Init(void);
uint16_t AS5047D_Get_CORDICMAG_Value(void);
uint16_t AS5047D_Get_ANGLEUNC_Value(void);
uint16_t AS5047D_Get_ANGLECOM_Value(void);
float AS5047D_Get_True_Angle_Value(void);














#endif





















