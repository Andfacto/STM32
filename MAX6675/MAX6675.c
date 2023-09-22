#include "MAX6675.h"

#ifdef __cplusplus
extern "C"{
#endif

void MAX6675_Init(MAX6675* max6675, SPI_HandleTypeDef* hspi, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	max6675->_hspi = hspi;
    max6675->_GPIOx = GPIOx;
	max6675->_GPIO_Pin = GPIO_Pin;
}

float MAX6675_GetTemperature(MAX6675* max6675)
{
	float temperature;
	
	uint16_t data = MAX6675_Read(max6675);
	temperature = (data >> 3) * 0.25;
	HAL_Delay(220);
	
	return temperature;
}

uint16_t MAX6675_Read(MAX6675* max6675) 
{	
	uint8_t buffer[2];
	
	HAL_GPIO_WritePin(max6675->_GPIOx, max6675->_GPIO_Pin, GPIO_PIN_RESET);
	HAL_SPI_Receive(max6675->_hspi, buffer, 2, MAX6675_SPI_TIMEOUT);
	HAL_GPIO_WritePin(max6675->_GPIOx, max6675->_GPIO_Pin, GPIO_PIN_SET);
	
	return buffer[0] | (buffer[1] << 8);
}

#ifdef __cplusplus
}
#endif
