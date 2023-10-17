#include "LM75A.h"

#ifdef __cplusplus
extern "C"{
#endif

void LM75A_Init(LM75A* lm75a, I2C_HandleTypeDef* hi2c, uint8_t adress, LM75A_DecimalMode mode)
{
	lm75a->_hi2c = hi2c;
	lm75a->_adress = adress;
	lm75a->_mode = mode;
}

void LM75A_Shutdown(LM75A* lm75a)
{
	uint8_t data = LM75A_Read8bitReg(lm75a, LM75A_CONFIG_REG);
	LM75A_Write8bitReg(lm75a, LM75A_CONFIG_REG, (data & 0b11111110) | 0x01);
}

void LM75A_Wakeup(LM75A* lm75a)
{
	uint8_t data = LM75A_Read8bitReg(lm75a, LM75A_CONFIG_REG);
	LM75A_Write8bitReg(lm75a, LM75A_CONFIG_REG, data & 0b11111110);
}

float LM75A_GetProductID(LM75A* lm75a)
{
	uint8_t data = LM75A_Read8bitReg(lm75a, LM75A_PRODID_REG);
	return (data >> 4) + (data & 0x0F) * 0.1;
} 

float LM75A_GetTemperature(LM75A* lm75a)
{
	uint16_t data = LM75A_Read16bitReg(lm75a, LM75A_TEMP_REG);
	
	if(lm75a->_mode == LM75A_DEC_5)
		data &= ~((1 << 5) | (1 << 6));
	else if(lm75a->_mode == LM75A_DEC_25) 
		data &= ~(1 << 5);
	
	return data / 256.0;
}

float LM75A_GetHystTemperature(LM75A* lm75a)
{
	uint16_t data = LM75A_Read16bitReg(lm75a, LM75A_THYST_REG);
	return data / 256.0;
}

float LM75A_GetOSTemperature(LM75A* lm75a)
{
	uint16_t data = LM75A_Read16bitReg(lm75a, LM75A_TOS_REG);
	return data / 256.0;
}

void LM75A_SetHystTemperature(LM75A* lm75a, float temperature)
{
	LM75A_Write16bitReg(lm75a, LM75A_THYST_REG, temperature * 256);
}

void LM75A_SetOSTemperature(LM75A* lm75a, float temperature)
{
	LM75A_Write16bitReg(lm75a, LM75A_TOS_REG, temperature * 256);
}

uint8_t LM75A_GetConfig(LM75A* lm75a)
{
	return LM75A_Read8bitReg(lm75a, LM75A_CONFIG_REG);
}

void LM75A_SetConfig(LM75A* lm75a, uint8_t data)
{
	LM75A_Write8bitReg(lm75a, LM75A_CONFIG_REG, data);
}

void LM75A_Write8bitReg(LM75A* lm75a, uint8_t reg, uint8_t data) 
{
	uint8_t buffer[2] = {reg, data};	
	HAL_I2C_Master_Transmit(lm75a->_hi2c, lm75a->_adress << 1, buffer, 2, LM75A_I2C_TIMEOUT);
}

void LM75A_Write16bitReg(LM75A* lm75a, uint8_t reg, uint16_t data) 
{
	uint8_t buffer[3] = {reg, data >> 8, data};	
	HAL_I2C_Master_Transmit(lm75a->_hi2c, lm75a->_adress << 1, buffer, 3, LM75A_I2C_TIMEOUT);
}

uint8_t LM75A_Read8bitReg(LM75A* lm75a, uint8_t reg) 
{
	uint8_t data;
	
	HAL_I2C_Master_Transmit(lm75a->_hi2c, lm75a->_adress << 1, &reg, 1, LM75A_I2C_TIMEOUT);
	HAL_I2C_Master_Receive(lm75a->_hi2c, lm75a->_adress << 1, data, 1, LM75A_I2C_TIMEOUT);
	
	return data;
}

uint16_t LM75A_Read16bitReg(LM75A* lm75a, uint8_t reg) 
{
	uint8_t buffer[2];
	
	HAL_I2C_Master_Transmit(lm75a->_hi2c, lm75a->_adress << 1, &reg, 1, LM75A_I2C_TIMEOUT);
	HAL_I2C_Master_Receive(lm75a->_hi2c, lm75a->_adress << 1, buffer, 2, LM75A_I2C_TIMEOUT);

	return buffer[0] << 8 | buffer[1];
}

#ifdef __cplusplus
}
#endif
