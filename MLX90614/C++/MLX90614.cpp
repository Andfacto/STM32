#include "MLX90614.h"

void MLX90614::Init(I2C_HandleTypeDef* hi2c, uint8_t adress)
{
	_hi2c = hi2c;
	_adress = adress;
}

float MLX90614::GetObjectTemperature()
{
	float temperature;
	
	uint16_t data = ReadReg(MLX90614_TOBJ1_REG);
	temperature = data * 0.02 - 273.15;
	
	return temperature;
}

float MLX90614::GetAmbientTemperature()
{	
	float temperature;
	
	uint16_t data = ReadReg(MLX90614_TA_REG);
	temperature = data * 0.02 - 273.15;
	
	return temperature;
}

uint16_t MLX90614::GetEmissivityReg()
{
	return ReadReg(MLX90614_EMISS_REG);
}

float MLX90614::GetEmissivity()
{	
	return GetEmissivityReg() / 65535.0;	
}

void MLX90614::SetEmissivityReg(uint16_t data)
{
	WriteReg(MLX90614_EMISS_REG, 0x00);
	HAL_Delay(10);
	WriteReg(MLX90614_EMISS_REG, data);
	HAL_Delay(10);
}

void MLX90614::SetEmissivity(float emissivity)
{
	uint16_t data = 0xFFFF * emissivity;
	
	SetEmissivityReg(data);
}

void MLX90614::WriteReg(uint8_t reg, uint16_t data)
{
	uint8_t buffer[3] = {reg, data >> 8 , data};
	HAL_I2C_Master_Transmit(_hi2c, _adress << 1, buffer, 3, MLX90614_I2C_TIMEOUT);	
}

uint16_t MLX90614::ReadReg(uint8_t reg)
{
	uint8_t buffer[3];
	
	HAL_I2C_Master_Transmit(_hi2c, _adress << 1, &reg, 1, MLX90614_I2C_TIMEOUT);
	HAL_I2C_Master_Receive(_hi2c, _adress << 1, buffer, 3, MLX90614_I2C_TIMEOUT);

	return buffer[1] << 8 | buffer[0];
}

