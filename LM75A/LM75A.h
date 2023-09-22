#ifndef LM75A_H
#define LM75A_H

#include "main.h"

#define LM75A_DEV_ADDRESS			0x48

#define LM75A_TEMP_REG 				0x00
#define LM75A_CONFIG_REG 			0x01
#define LM75A_THYST_REG				0x02
#define LM75A_TOS_REG				0x03
#define LM75A_PRODID_REG			0x07

#define LM75A_I2C_TIMEOUT         	50 

typedef enum {
	LM75A_DEC_5, 
	LM75A_DEC_25, 
	LM75A_DEC_125,
} LM75A_DecimalMode;

typedef struct {
	I2C_HandleTypeDef* _hi2c;
	uint8_t _adress;
	LM75A_DecimalMode _mode;
} LM75A;

void LM75A_Init(LM75A* lm75a, I2C_HandleTypeDef* hi2c, uint8_t adress, LM75A_DecimalMode mode);

void LM75A_Shutdown(LM75A* lm75a);
void LM75A_Wakeup(LM75A* lm75a);

float LM75A_GetProductID(LM75A* lm75a);

float LM75A_GetTemperature(LM75A* lm75a);

float LM75A_GetHystTemperature(LM75A* lm75a);
void LM75A_SetHystTemperature(LM75A* lm75a, float temperature);

float LM75A_GetOSTemperature(LM75A* lm75a);
void LM75A_SetOSTemperature(LM75A* lm75a, float temperature);

uint8_t LM75A_GetConfig(LM75A* lm75a);
void LM75A_SetConfig(LM75A* lm75a, uint8_t data);

void LM75A_Write8bitReg(LM75A* lm75a, uint8_t reg, uint8_t data);
void LM75A_Write16bitReg(LM75A* lm75a, uint8_t reg, uint16_t data);

uint8_t LM75A_Read8bitReg(LM75A* lm75a, uint8_t reg);
uint16_t LM75A_Read16bitReg(LM75A* lm75a, uint8_t reg);

#endif
