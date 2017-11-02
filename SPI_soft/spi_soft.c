#include "spi_soft.h"

void SOFT_SPI_GPIO_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStruct;
    __HAL_RCC_SPI2_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**SPI2 GPIO Configuration    
    PB13     ------> SPI2_SCK
    PB14     ------> SPI2_MISO
    PB15     ------> SPI2_MOSI 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}





void SPI_SOFT_WRITE(uint8_t data)
{
	uint8_t i ;
	AS5047D_SCK = 0;
	for(i= 0;i<8;i++)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
		Delay_us(100);
		if(data&0x80)
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
		else 
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);	
       HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);	
 		data<<=1;
		Delay_us(100);
		
	}
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);	 			
}

uint8_t SPI_SOFT_READ(void)
{
	uint8_t i,spi_data;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);	   		
	for(i = 0;i<8;i++)
	{	
			   
		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14)) 
			spi_data|=0x01;
		else 
			spi_data&=0xfe;			
	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
        Delay_us(100);
		spi_data<<=1;	
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
		Delay_us(100);
	}
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
	
	return spi_data;
}

uint8_t SPI_SOFT_READ_WARITE(uint8_t data)
{
	uint8_t i,spi_data;
//	AS5047D_SCK = 0;
//	for(i = 0;i<8;i++)
//	{
//		if(AS5047D_MISO) 
//			spi_data|=0x01;
//		else 
//			spi_data&=0xfe;	
//		
//		AS5047D_SCK = 1;
//		spi_data<<=1;
//		Delay_us(100);
//		
//		if(data&0x80)
//			AS5047D_MOSI = 1;
//		else 	
//			AS5047D_MOSI = 0 ; 
//			
//        AS5047D_SCK = 0;		
//	    Delay_us(100);
//		data<<=1;		
//		
//	}
//	AS5047D_SCK = 0;



//    for(i= 0;i<8;i++)
//	  {
//		  GPIO_SetBits(GPIOB, GPIO_Pin_13);
//		  if(data&0x80)
//		  	GPIO_SetBits(GPIOB, GPIO_Pin_15);
//		  else 
//		  	GPIO_ResetBits(GPIOB, GPIO_Pin_15);	
//			data<<=1;
//        GPIO_ResetBits(GPIOB, GPIO_Pin_13); 
//			  spi_data<<=1;
//			if(AS5047D_MISO)
//				spi_data++;
//			GPIO_SetBits(GPIOB, GPIO_Pin_13);
   for(i = 0;i<8;i++)
  {
 //   AS5047D_SCK = 1;
		spi_data|=AS5047D_MISO;
		AS5047D_SCK = 1;
        AS5047D_MOSI = (data&0x80);
		data<<=1;
		AS5047D_SCK = 0;
	}

	return spi_data;
}



