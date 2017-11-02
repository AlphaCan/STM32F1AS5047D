#include "AS5047D.h"
#include "spi_soft.h"

uint16_t parit = 0;
 
uint16_t parity(uint16_t x)
{
	uint8_t cnt = 0;
	uint8_t i;

	for (i = 0; i < 16; i++)
	{
		if (x & 0x1)
		{
			cnt++;
		}
		x >>= 1;
	}
return cnt & 0x1;
//	  int parity=0;
//  while (x > 0) 
//  {
//    parity = (parity + (x & 1)) % 2;
//    x >>= 1;
//  }
//  return(parity);

}

void AS5047D_Write(uint16_t address, uint16_t data)
{
	uint8_t address_H = 0;
	uint8_t address_L = 0;
	address_H = (uint8_t)((address&0x3f00)>>8);
	address_L = (uint8_t)(address&0x00ff);
		
	if (parity(address & AS4047D_WR) == 1) 
		address_H = address_H | 0x80; // set parity bit
	Delay_us(500);
	AS5047D_CSN = 0;
	Delay_us(500);
	SPI2_ReadWriteByte(address_H);
	SPI2_ReadWriteByte(address_L);
//	HAL_SPI_Transmit(&hspi2, (uint8_t*) &address_H, 1, 100);
//    HAL_SPI_Transmit(&hspi2, (uint8_t*) &address_L, 1, 100);
	
	Delay_us(500);
	AS5047D_CSN = 1;
	 
    uint8_t data_H = 0;
	uint8_t data_L = 0;
	data_H = (uint8_t)((data&0x3f00)>>8);
	data_L = (uint8_t)(data&0x00ff);  
	
    if (parity(data & AS4047D_WR) == 1)
		data_H = data_H | 0x80; // set parity bit
	Delay_us(500);
	AS5047D_CSN = 0;
	Delay_us(500);		
	SPI2_ReadWriteByte(data_H);
	SPI2_ReadWriteByte(data_L);
//	HAL_SPI_Transmit(&hspi2, (uint8_t*) &data_H, 1, 100);
//    HAL_SPI_Transmit(&hspi2, (uint8_t*) &data_L, 1, 100);

	Delay_us(500);
	AS5047D_CSN = 1;		
}



uint16_t AS5047D_Read_16BIT(uint16_t address)
{
	uint16_t data = 0;
	uint16_t read_H = 0;
	uint16_t read_L = 0;
	uint8_t address_H = 0;
	uint8_t address_L = 0;
	address_H = (uint8_t)((address&0x3f00)>>8);
	address_L = (uint8_t)(address&0x00ff);
	
	if (parity(address | AS4047D_RD) == 1) 
		address_H = address_H | 0x80; // set parity bit
	address_H = address_H | AS4047D_RD; // it's a read command
	AS5047D_CSN = 0;
	Delay_us(500);
//	SPI2_ReadWriteByte(address_H);
//	SPI2_ReadWriteByte(address_L);
	HAL_SPI_Transmit(&hspi2, (uint8_t*) &address_H, 1, 100);
    HAL_SPI_Transmit(&hspi2, (uint8_t*) &address_L, 1, 100);

	Delay_ms(1);
	AS5047D_CSN = 1;
	Delay_us(500);
	
	AS5047D_CSN = 0;
	Delay_us(500);
	HAL_SPI_Receive(&hspi2, (uint8_t*) &data, 2, 100);

//    read_H = SPI2_ReadWriteByte(0xff);	
//	read_L = SPI2_ReadWriteByte(0xff);	
//	data = read_H<<8|read_L;
    Delay_us(500);
	AS5047D_CSN = 1;
	data = data & 0x3FFF;  // filter bits outside data, strip bit 14..15
	return data;
}


void AS5047D_Check_Transmission_Error(void)
{
	/** Check if transmission error **/
	if(AS5047D_Read_16BIT( AS4047D_ERRFL) != 0)
	{
		while(1)
		{}	
	}
}

void AS5047D_SetZero(void)
{
	/** Check diagnostics reg **/
	uint16_t DIAAGC = AS5047D_Read_16BIT(AS4047D_DIAAGC);

	/** Get uncompensated angle reg value **/
	uint16_t ANGLEUNC = AS5047D_Read_16BIT(AS4047D_ANGLEUNC);

	/** Write to zero pos regs **/
	AS5047D_Write(AS4047D_ZPOSM, (ANGLEUNC >> 6) & 0x00FF);
	AS5047D_Write(AS4047D_ZPOSL, ANGLEUNC & 0x003F);
	
//	AS5047D_Write(AS4047D_PROG, 0X01);
//	AS5047D_Write(AS4047D_PROG, 0X08);
//	
//	AS5047D_Write(AS4047D_ZPOSM, 0X00);
//	AS5047D_Write(AS4047D_ZPOSL, 0X00);
//	AS5047D_Write(AS4047D_SETTINGS1, 0X00);
//	AS5047D_Write(AS4047D_SETTINGS2, 0X00);
//	
//	AS5047D_Write(AS4047D_PROG, 0X08);
//	AS5047D_Write(AS4047D_PROG, 0X40);
//	AS5047D_Write(AS4047D_PROG, 0X04);
}

uint16_t AS5047D_GetZero(void)
{
	uint16_t ZPOSM = 0;
	uint16_t ZPOSL = 0;

	ZPOSM = AS5047D_Read_16BIT(AS4047D_ZPOSM);
	ZPOSL = AS5047D_Read_16BIT(AS4047D_ZPOSL);

	return (((ZPOSM << 6) & 0x3FC0) | (ZPOSL & 0x003F));
}

uint8_t AS5047D_Get_AGC_Value(void)
{
	/** Read diagnostics reg **/
	uint16_t DIAAGC = AS5047D_Read_16BIT(AS4047D_DIAAGC);
	return (uint8_t)((DIAAGC >> 8) & 0x00FF);
}

void AS5047D_Init(void)
{	
	MX_SPI2_Init();		   	//初始化SPI
	AS5047D_CSN=1;				//SPI FLASH不选中
	/* Initiaize AS4047D */
	AS5047D_Write(AS4047D_SETTINGS1,0xD8);
//	AS5047D_Check_Transmission_Error();
	AS5047D_Write(AS4047D_SETTINGS2,0xEB);
//	AS5047D_Check_Transmission_Error();
}

uint16_t AS5047D_Get_CORDICMAG_Value(void)
{
	uint16_t CORDICMAG = AS5047D_Read_16BIT(AS4047D_CORDICMAG);
//	AS5047D_Check_Transmission_Error();
	return CORDICMAG;
}

uint16_t AS5047D_Get_ANGLEUNC_Value(void)
{
	uint16_t ANGLEUNC = AS5047D_Read_16BIT(AS4047D_ANGLEUNC);
//	AS5047D_Check_Transmission_Error();
	return ANGLEUNC;
}

uint16_t AS5047D_Get_ANGLECOM_Value(void)
{
	uint16_t ANGLECOM = AS5047D_Read_16BIT(AS4047D_ANGLECOM);
//	AS5047D_Check_Transmission_Error();
	return ANGLECOM;
}

float AS5047D_Get_True_Angle_Value(void)
{
	return((float)AS5047D_Get_ANGLEUNC_Value() * 360.0f / 16383.0f);
//	return((float)AS5047D_Get_ANGLECOM_Value() * 360.0f / 16383.0f);
}







