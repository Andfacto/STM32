#ifndef MLX90614_H
#define MLX90614_H

#include "main.h"

#define MLX90614_DEV_ADRESS 	0x5A

#define MLX90614_TA_REG		 	0x06
#define MLX90614_TOBJ1_REG 		0x07
#define MLX90614_TOBJ2_REG 		0x08

#define MLX90614_TOMAX_REG 		0x20
#define MLX90614_TOMIN_REG 		0x21
#define MLX90614_PWMCTRL_REG 	0x22
#define MLX90614_TARANGE_REG 	0x23
#define MLX90614_EMISS_REG 		0x24
#define MLX90614_CONFIG_REG 	0x25
#define MLX90614_ADDR_REG		0x2E
#define MLX90614_ID1_REG 		0x3C
#define MLX90614_ID2_REG 		0x3D
#define MLX90614_ID3_REG 		0x3E
#define MLX90614_ID4_REG 		0x3F

#define MLX90614_I2C_TIMEOUT	50

class MLX90614 {
private:
	I2C_HandleTypeDef* _hi2c;
	uint8_t _adress;
	
	void WriteReg(uint8_t reg, uint16_t data);	
	uint16_t ReadReg(uint8_t reg);
	
public:
	void Init(I2C_HandleTypeDef* hi2c, uint8_t adress = MLX90614_DEV_ADRESS);
	
	float GetObjectTemperature();
	float GetAmbientTemperature();
	
	float GetEmissivity();
	uint16_t GetEmissivityReg();
	
	void SetEmissivity(float emissivity);
	void SetEmissivityReg(uint16_t data);
		
};

#endif

