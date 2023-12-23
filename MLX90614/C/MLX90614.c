#include "MLX90614.h"

#ifdef __cplusplus
extern "C"{
#endif

void MLX90614_Init(MLX90614* mlx90614, I2C_HandleTypeDef* hi2c)
{
	mlx90614->_hi2c = hi2c;
}

float MLX90614_GetObjectTemperature(MLX90614* mlx90614)
{
	float temperature;

	uint16_t data =  MLX90614_ReadReg(mlx90614, MLX90614_TOBJ1_REG);
	temperature = data * 0.02 - 273.15;

	return temperature;
}

float MLX90614_GetAmbientTemperature(MLX90614* mlx90614)
{	
	float temperature;

	uint16_t data =  MLX90614_ReadReg(mlx90614, MLX90614_TA_REG);
	temperature = data * 0.02 - 273.15;

	return temperature;
}

uint16_t MLX90614_GetEmissivityReg(MLX90614* mlx90614)
{
	return  MLX90614_ReadReg(mlx90614, MLX90614_EMISS_REG);
}

float MLX90614_GetEmissivity(MLX90614* mlx90614)
{	
	return MLX90614_GetEmissivityReg(mlx90614) / 65535.0;
}

void MLX90614_SetEmissivityReg(MLX90614* mlx90614, uint16_t data)
{
	MLX90614_WriteReg(mlx90614, MLX90614_EMISS_REG, 0x00);
	HAL_Delay(10);
	MLX90614_WriteReg(mlx90614, MLX90614_EMISS_REG, data);
	HAL_Delay(10);
}

void MLX90614_SetEmissivity(MLX90614* mlx90614, float emissivity)
{
	uint16_t data = 0xFFFF * emissivity;

	MLX90614_SetEmissivityReg(mlx90614, data);
}

void MLX90614_WriteReg(MLX90614* mlx90614, uint8_t reg, uint16_t data)
{
	uint8_t buffer[3] = {reg, data >> 8 , data};
	HAL_I2C_Master_Transmit(mlx90614->_hi2c, MLX90614_DEV_ADRESS << 1, buffer, 3, MLX90614_I2C_TIMEOUT);
}

uint16_t MLX90614_ReadReg(MLX90614* mlx90614, uint8_t reg)
{
	uint8_t buffer[3];

	HAL_I2C_Master_Transmit(mlx90614->_hi2c, MLX90614_DEV_ADRESS << 1, &reg, 1, MLX90614_I2C_TIMEOUT);
	HAL_I2C_Master_Receive(mlx90614->_hi2c, MLX90614_DEV_ADRESS << 1, buffer, 3, MLX90614_I2C_TIMEOUT);

	return buffer[1] << 8 | buffer[0];
}

#ifdef __cplusplus
}
#endif
