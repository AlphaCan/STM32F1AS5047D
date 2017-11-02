#ifndef __SPI_SOFT_H
#define __SPI_SOFT_H
#include "AS5047D.h"

void SOFT_SPI_GPIO_Init(void);
void SPI_SOFT_WRITE(uint8_t data);
uint8_t SPI_SOFT_READ(void);
uint8_t SPI_SOFT_READ_WARITE(uint8_t data);

#endif













