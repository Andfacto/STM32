#include "mlx90614.h"

#ifdef __cplusplus
extern "C"{
#endif

error_mlx90614 mlx90614_init(      mlx90614_t*        mlx90614,
	                               I2C_HandleTypeDef* hi2c    ,
	                         const UINT8              address )
{
	mlx90614->hi2c = hi2c;
	mlx90614->address = address << 1;

	mlx90614->get_object = &mlx90614_get_object;
	mlx90614->get_ambient = &mlx90614_get_ambient;

	mlx90614->get_emissivity = &mlx90614_get_emissivity;
	mlx90614->set_emissivity = &mlx90614_set_emissivity;

	return MLX90614_OK;
}

static error_mlx90614 mlx90614_write_reg(      mlx90614_t* mlx90614,
	                                     const UINT8       reg     ,
								         const UINT8*      buf     ,
								         const UINT32      size    )
{
	HAL_StatusTypeDef err;

	err = HAL_I2C_Master_Transmit(mlx90614->hi2c, mlx90614->address, &reg, 1, MLX90614_I2C_TIMEOUT);
	if(err != HAL_OK) return MLX90614_ERR_I2C;

	err = HAL_I2C_Master_Transmit(mlx90614->hi2c, mlx90614->address, buf, size, MLX90614_I2C_TIMEOUT);
	if(err != HAL_OK) return MLX90614_ERR_I2C;

	return MLX90614_OK;
}

static error_mlx90614 mlx90614_read_reg(      mlx90614_t* mlx90614,
	                                    const UINT8       reg     ,
								              UINT8*      buf     ,
								        const UINT32      size    )
{
	HAL_StatusTypeDef err;

	err = HAL_I2C_Master_Transmit(mlx90614->hi2c, mlx90614->address, &reg, 1, MLX90614_I2C_TIMEOUT);
	if(err != HAL_OK) return MLX90614_ERR_I2C;

	err = HAL_I2C_Master_Receive(mlx90614->hi2c, mlx90614->address, buf, size, MLX90614_I2C_TIMEOUT);
	if(err != HAL_OK) return MLX90614_ERR_I2C;

	return MLX90614_OK;
}

error_mlx90614 mlx90614_get_object(mlx90614_t* mlx90614,
                                   FLOAT32*    buf     )
{
	error_mlx90614 err;
	UINT8 data[2];
	INT16 temp;

	err = mlx90614_read_reg(mlx90614, MLX90614_TOBJ1_REG, data, 2);
	if(err != MLX90614_OK) return err;

	temp = (data[1] << 8 | data[0]);

	*buf = temp * 0.02 - 273.15;

	return MLX90614_OK;
}

error_mlx90614 mlx90614_get_ambient(mlx90614_t* mlx90614,
                                    FLOAT32*    buf     )
{
	error_mlx90614 err;
	UINT8 data[2];
	INT16 temp;

	err = mlx90614_read_reg(mlx90614, MLX90614_TA_REG, data, 2);
	if(err != MLX90614_OK) return err;

	temp = (data[1] << 8 | data[0]);

	*buf = temp * 0.02 - 273.15;

	return MLX90614_OK;
}

error_mlx90614 mlx90614_get_emissivity(mlx90614_t* mlx90614,
                                       FLOAT32*    buf     )
{
	error_mlx90614 err;
	UINT8 data[2];

	err = mlx90614_read_reg(mlx90614, MLX90614_EMISS_REG, data, 2);
	if(err != MLX90614_OK) return err;

	*buf = (data[1] << 8 | data[0]) / 65535.0f;

	return MLX90614_OK;
}

error_mlx90614 mlx90614_set_emissivity(      mlx90614_t* mlx90614  ,
	                                   const FLOAT32     emissivity)
{
	error_mlx90614 err;
	UINT16 data;

	data = 0xFFFF * emissivity;

	UINT8 buf[2] = {data >> 8, data};

	err = mlx90614_write_reg(mlx90614, MLX90614_EMISS_REG, 0x00, 1);
	if(err != MLX90614_OK) return err;

	HAL_Delay(10);

	err = mlx90614_write_reg(mlx90614, MLX90614_EMISS_REG, buf, 2);
	if(err != MLX90614_OK) return err;

	HAL_Delay(10);

	return MLX90614_OK;
}

#ifdef __cplusplus
}
#endif
