#include "AS5047D.h"
#include "spi_soft.h"
 
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

}

void AS5047D_Write(uint16_t address, uint16_t data)
{
	
	if (parity(address & AS4047D_WR) == 1) 
		address = address | 0x8000; // set parity bit
	AS5047D_CSN = 0;
	Delay_us(500);
//	HAL_SPI_Transmit(&hspi2, (uint8_t*) &address, 2, 100);
	SPI2_ReadWriteByte(address>>8);
	SPI2_ReadWriteByte(address);
	Delay_us(500);
	AS5047D_CSN = 1;
	
    if (parity(data & AS4047D_WR) == 1)
		data = data | 0x8000; // set parity bit
	AS5047D_CSN = 0;
	Delay_us(500);		
//	HAL_SPI_Transmit(&hspi2, (uint8_t*) &data, 2, 100);
	SPI2_ReadWriteByte(data>>8);
	SPI2_ReadWriteByte(data);
	Delay_us(500);
	AS5047D_CSN = 1;		
}



uint16_t AS5047D_Read_16BIT(uint16_t address)
{
	
	if (parity(address | AS4047D_RD) == 1) 
		address = address | 0x8000; // set parity bit
	address = address | AS4047D_RD; // it's a read command
	AS5047D_CSN = 0;
	Delay_us(500);
//	HAL_SPI_Transmit(&hspi2, (uint8_t*) &address, 2, 100);
	SPI2_ReadWriteByte(address>>8);
	SPI2_ReadWriteByte(address);
	
	Delay_ms(1);
	AS5047D_CSN = 1;
	Delay_us(500);
	
	AS5047D_CSN = 0;
	Delay_us(500);
	uint16_t data = 0;
	uint8_t temp1 = 0;
	uint8_t temp2 = 0;
//	HAL_SPI_Receive(&hspi2, (uint8_t*) &data, 2, 100);
	temp1 = SPI2_ReadWriteByte(0xff);
	temp2 = SPI2_ReadWriteByte(0xff);
    Delay_us(500);
	AS5047D_CSN = 1;
	data = (uint16_t)temp1<<8|temp2;
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
	AS5047D_Write(AS4047D_SETTINGS1,0x20);
//	AS5047D_Check_Transmission_Error();
	AS5047D_Write(AS4047D_SETTINGS2,0x18);
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

uint8_t AS5047D_offset = 1;
uint16_t AS5047D_offset_reslut = 0;
float AS5047D_Get_True_Angle_Value(void)
{
//	
//	if(AS5047D_offset )
//	{
//		uint16_t AS5047D_offset_temp = 0;
//		
//		for(uint16_t i = 0 ; i < AS5047D_Offset_Num ;i++)
//		{
//			AS5047D_offset_temp += (float)AS5047D_Get_ANGLECOM_Value() * 360.0f / 16383.0f;
//			LED1=~LED1;
//		}
//		AS5047D_offset_reslut = AS5047D_offset_temp/AS5047D_Offset_Num;

//		AS5047D_offset = 0;
//	}
//	return((float)AS5047D_Get_ANGLEUNC_Value() * 360.0f / 16383.0f );
	return((float)AS5047D_Get_ANGLECOM_Value() * 360.0f / 16383.0f  );
}







