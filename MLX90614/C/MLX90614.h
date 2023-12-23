#ifndef MLX90614_H
#define MLX90614_H

#include "main.h"

#define MLX90614_DEV_ADRESS 		0x5A

#define MLX90614_TA_REG				0x06
#define MLX90614_TOBJ1_REG 			0x07
#define MLX90614_TOBJ2_REG 			0x08

#define MLX90614_TOMAX_REG 			0x20
#define MLX90614_TOMIN_REG 			0x21
#define MLX90614_PWMCTRL_REG 		0x22
#define MLX90614_TARANGE_REG 		0x23
#define MLX90614_EMISS_REG 			0x24
#define MLX90614_CONFIG_REG 		0x25
#define MLX90614_ADDR_REG			0x2E
#define MLX90614_ID1_REG 			0x3C
#define MLX90614_ID2_REG 			0x3D
#define MLX90614_ID3_REG 			0x3E
#define MLX90614_ID4_REG 			0x3F

#define MLX90614_I2C_TIMEOUT		50

typedef struct {
	I2C_HandleTypeDef* _hi2c;
} MLX90614;

void MLX90614_WriteReg(MLX90614* mlx90614, uint8_t reg, uint16_t data);
uint16_t MLX90614_ReadReg(MLX90614* mlx90614, uint8_t reg);

void MLX90614_Init(MLX90614* mlx90614, I2C_HandleTypeDef* hi2c);

float MLX90614_GetObjectTemperature(MLX90614* mlx90614);
float MLX90614_GetAmbientTemperature(MLX90614* mlx90614);

float MLX90614_GetEmissivity(MLX90614* mlx90614);
uint16_t MLX90614_GetEmissivityReg(MLX90614* mlx90614);

void MLX90614_SetEmissivity(MLX90614* mlx90614, float emissivity);
void MLX90614_SetEmissivityReg(MLX90614* mlx90614, uint16_t data);

#endif

