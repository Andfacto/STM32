#ifndef MAX6675_H
#define MAX6675_H

#include "main.h"

#define MAX6675_SPI_TIMEOUT 50 

typedef struct MAX6675 {
	SPI_HandleTypeDef* hspi;
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
} MAX6675;

void MAX6675_Init(MAX6675* max6675, SPI_HandleTypeDef* hspi, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

float MAX6675_GetTemperature(MAX6675* max6675);

uint16_t MAX6675_Read(MAX6675* max6675);

#endif
