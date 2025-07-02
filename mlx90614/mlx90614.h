#ifndef MLX90614_H
#define MLX90614_H

#include "main.h"

typedef unsigned char      UINT8;
typedef 		 char      INT8;
typedef unsigned short     UINT16;
typedef 		 short     INT16;
typedef unsigned int       UINT32;
typedef 		 int       INT32;
typedef unsigned long long UINT64;
typedef 		 long long INT64;
typedef 		 float     FLOAT32;
typedef			 double    FLOAT64;

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

#define MLX90614_I2C_TIMEOUT		50U

typedef enum {
	MLX90614_OK,
	MLX90614_ERR_I2C,
	MLX90614_ERR_NULL_PTR,
} error_mlx90614;

typedef struct _mlx90614_t{
	I2C_HandleTypeDef* hi2c;
	UINT8 address;

	error_mlx90614 (*get_object)(struct _mlx90614_t* mlx90614, FLOAT32* buf);
	error_mlx90614 (*get_ambient)(struct _mlx90614_t* mlx90614, FLOAT32* buf);

	error_mlx90614 (*get_emissivity)(struct _mlx90614_t* mlx90614, FLOAT32* buf);
	error_mlx90614 (*set_emissivity)(struct _mlx90614_t* mlx90614, const FLOAT32 emissivity);
} mlx90614_t;

error_mlx90614 mlx90614_init(mlx90614_t* mlx90614, I2C_HandleTypeDef* hi2c, const UINT8 address);

error_mlx90614 mlx90614_get_object(mlx90614_t* mlx90614, FLOAT32* buf);
error_mlx90614 mlx90614_get_ambient(mlx90614_t* mlx90614, FLOAT32* buf);

error_mlx90614 mlx90614_get_emissivity(mlx90614_t* mlx90614, FLOAT32* buf);
error_mlx90614 mlx90614_set_emissivity(mlx90614_t* mlx90614, const FLOAT32 emissivity);

#endif

