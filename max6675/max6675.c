#include "max6675.h"

#ifdef __cplusplus
extern "C"{
#endif

error_max6675 max6675_init(      max6675_t*         max6675 , 
	                             SPI_HandleTypeDef* hspi    , 
						   const GPIO_TypeDef*      gpiox   , 
						   const UINT16             gpio_pin)
{
	max6675->hspi = hspi;
	max6675->gpiox = gpiox;
	max6675->gpio_pin = gpio_pin;

	max6675->get_temperature = &max6675_get_temperature;

	return MAX6675_OK;
}

static error_max6675 max6675_read_reg(      max6675_t* max6675, 
	                                        UINT8*     buf    ,
									  const UINT32     size   ) 
{	
	HAL_StatusTypeDef err;

	HAL_GPIO_WritePin(max6675->gpiox, max6675->gpio_pin, GPIO_PIN_RESET);
	
	err = HAL_SPI_Receive(max6675->hspi, buf, size, MAX6675_SPI_TIMEOUT);
	if(err != HAL_OK) return MAX6675_ERR_SPI;

	HAL_GPIO_WritePin(max6675->gpiox, max6675->gpio_pin, GPIO_PIN_SET);
	
	return MAX6675_OK;
}

error_max6675 max6675_get_temperature(max6675_t* max6675,
                                      FLOAT32*   buf    )
{
	error_max6675 err;
	UINT8 data[2];
	UINT16 temp;
	
	err = max6675_read_reg(max6675, data, 2);
	if(err != MAX6675_OK) return err;

	temp = data[0] | (data[1] << 8);
	*buf = (temp >> 3) * 0.25;

	HAL_Delay(220);
	
	return MAX6675_OK;
}

#ifdef __cplusplus
}
#endif
