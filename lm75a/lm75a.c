#include "lm75a.h"

#ifdef __cplusplus
extern "C"{
#endif

error_lm75a lm75a_init(      lm75a_t*           lm75a  ,
	                         I2C_HandleTypeDef* hi2c   ,
	                   const UINT8              address,
				       const lm75a_decmode_t    mode   )
{
	lm75a->hi2c = hi2c;
	lm75a->address = address << 1;
	lm75a->mode = mode;

	lm75a->get_productid = &lm75a_get_productid;

	lm75a->shutdown = &lm75a_shutdown;
	lm75a->wakeup = &lm75a_wakeup;

	lm75a->get_temperature = &lm75a_get_temperature;

	lm75a->get_hyst = &lm75a_get_hyst;
	lm75a->set_hyst = &lm75a_set_hyst;

	lm75a->get_os = &lm75a_get_os;
	lm75a->set_os = &lm75a_set_os;

	lm75a->get_config = &lm75a_get_config;
	lm75a->set_config = &lm75a_set_config;

	return LM75A_OK;
}

static error_lm75a lm75a_write_reg(      lm75a_t* lm75a,
	                               const UINT8    reg  ,
								   const UINT8*   buf  ,
								   const UINT32   size )
{
	HAL_StatusTypeDef err;

	err = HAL_I2C_Master_Transmit(lm75a->hi2c, lm75a->address, &reg, 1, LM75A_I2C_TIMEOUT);
	if(err != HAL_OK) return LM75A_ERR_I2C;

	err = HAL_I2C_Master_Transmit(lm75a->hi2c, lm75a->address, buf, size, LM75A_I2C_TIMEOUT);
	if(err != HAL_OK) return LM75A_ERR_I2C;

	return LM75A_OK;
}

static error_lm75a lm75a_read_reg(      lm75a_t* lm75a,
	                              const UINT8    reg  ,
								        UINT8*   buf  ,
								  const UINT32   size )
{
	HAL_StatusTypeDef err;

	err = HAL_I2C_Master_Transmit(lm75a->hi2c, lm75a->address, &reg, 1, LM75A_I2C_TIMEOUT);
	if(err != HAL_OK) return LM75A_ERR_I2C;

	err = HAL_I2C_Master_Receive(lm75a->hi2c, lm75a->address, buf, size, LM75A_I2C_TIMEOUT);
	if(err != HAL_OK) return LM75A_ERR_I2C;

	return LM75A_OK;
}

error_lm75a lm75a_shutdown(lm75a_t* lm75a)
{
	error_lm75a err;
	UINT8 data;

	err = lm75a_read_reg(lm75a, LM75A_CONFIG_REG, &data, 1);
	if(err != LM75A_OK) return err;

	err = lm75a_write_reg(lm75a, LM75A_CONFIG_REG, data & 0xFF, 1);
	if(err != LM75A_OK) return err;

	return LM75A_OK;
}

error_lm75a lm75a_wakeup(lm75a_t* lm75a)
{
	error_lm75a err;
	UINT8 data;

	err = lm75a_read_reg(lm75a, LM75A_CONFIG_REG, &data, 1);
	if(err != LM75A_OK) return err;

	err = lm75a_write_reg(lm75a, LM75A_CONFIG_REG, data & 0xFE, 1);
	if(err != LM75A_OK) return err;

	return LM75A_OK;
}

error_lm75a lm75a_get_productid(lm75a_t* lm75a,
                                UINT8*   buf  )
{
	error_lm75a err;

	err = lm75a_read_reg(lm75a, LM75A_PRODID_REG, buf, 1);
	if(err != LM75A_OK) return err;

	return LM75A_OK;
}

error_lm75a lm75a_get_temperature(lm75a_t* lm75a,
                                  FLOAT32* buf  )
{
	error_lm75a err;
	UINT8 data[2];

	err = lm75a_read_reg(lm75a, LM75A_TEMP_REG, data, 2);
	if(err != LM75A_OK) return err;

	if(lm75a->mode == LM75A_DEC_5)
		*buf = (INT8)data[0] + (data[1] >> 7) * 0.5f;
	else if(lm75a->mode == LM75A_DEC_25)
		*buf = (INT8)data[0] + (data[1] >> 6) * 0.25f;
	else
		*buf = (INT8)data[0] + (data[1] >> 5) * 0.125f;

	return LM75A_OK;
}

error_lm75a lm75a_get_hyst(lm75a_t* lm75a,
                           FLOAT32* buf  )
{
	error_lm75a err;
	UINT8 data[2];

	err = lm75a_read_reg(lm75a, LM75A_THYST_REG, data, 2);
	if(err != LM75A_OK) return err;

	*buf = (INT8)data[0] + (data[1] >> 7) * 0.5f;

	return LM75A_OK;
}

error_lm75a lm75a_set_hyst(lm75a_t* lm75a,
	                       FLOAT32  data )
{
	error_lm75a err;
	INT16 temp;

	data = roundf(data * 2.0f);
    temp = ((INT16)(data)) << 7;

	UINT8 buf[2] = {temp >> 8, temp};

	err = lm75a_write_reg(lm75a, LM75A_THYST_REG, buf, 2);
	if(err != LM75A_OK) return err;

	return LM75A_OK;
}

error_lm75a lm75a_get_os(lm75a_t* lm75a,
                         FLOAT32* buf  )
{
	error_lm75a err;
	UINT8 data[2];

	err = lm75a_read_reg(lm75a, LM75A_TOS_REG, data, 2);
	if(err != LM75A_OK) return err;

	*buf = (INT8)data[0] + (data[1] >> 7) * 0.5f;

	return LM75A_OK;
}

error_lm75a lm75a_set_os(lm75a_t* lm75a,
	                     FLOAT32  data )
{
	error_lm75a err;
	INT16 temp;

	data = roundf(data * 2.0f);
    temp = ((INT16)(data)) << 7;

	UINT8 buf[2] = {temp >> 8, temp};

	err = lm75a_write_reg(lm75a, LM75A_TOS_REG, buf, 2);
	if(err != LM75A_OK) return err;

	return LM75A_OK;
}

error_lm75a lm75a_get_config(lm75a_t* lm75a,
                             UINT8*   buf  )
{
	error_lm75a err;

	err = lm75a_read_reg(lm75a, LM75A_CONFIG_REG, buf, 1);
	if(err != LM75A_OK) return err;

	return LM75A_OK;
}

error_lm75a lm75a_set_config(      lm75a_t* lm75a,
	                         const UINT8    data )
{
	error_lm75a err;

	err = lm75a_write_reg(lm75a, LM75A_CONFIG_REG, &data, 1);
	if(err != LM75A_OK) return err;

	return LM75A_OK;
}

#ifdef __cplusplus
}
#endif
