#include "sharp.h"

#ifdef __cplusplus
extern "C"{
#endif

error_sharp sharp_init(      sharp_t*           sharp  ,
	                         ADC_HandleTypeDef* hadc   , 
					   const UINT32             channel, 
					   const sharp_model_t      model  )
{
	sharp->hadc = hadc;
	sharp->channel = channel;
	sharp->model = model;

	sharp->get_distance = &sharp_get_distance;

	return SHARP_OK;
}

static error_sharp sharp_get_adc(sharp_t* sharp, 
	                             UINT32*  buf  )
{
	HAL_StatusTypeDef err;
	ADC_ChannelConfTypeDef config = {0};

	config.Rank = 1;
	config.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	config.Channel = sharp->channel;

	HAL_ADC_ConfigChannel(sharp->hadc, &config);
	HAL_ADC_Start(sharp->hadc);

	err = HAL_ADC_PollForConversion(sharp->hadc, SHARP_ADC_TIMEOUT);
	if(err != HAL_OK) return SHARP_ERR_ADC;

	*buf = HAL_ADC_GetValue(sharp->hadc);
	HAL_ADC_Stop(sharp->hadc);

	return SHARP_OK;
}

error_sharp sharp_get_distance(sharp_t* sharp, 
	                           FLOAT32* buf  )
{
	error_sharp err;
	UINT32 adc_val;
	FLOAT32 analog;

	err = sharp_get_adc(sharp, &adc_val);
	if(err != SHARP_OK) return err;

	analog = (adc_val * 3.3) / 4095.0f;

	if(sharp->model == SHARP_GP2Y0A21)
		*buf = 29.988f*powf(analog, -1.173f);
	else
		*buf = 65*powf(analog, -1.10f);

	return err;
}

#ifdef __cplusplus
}
#endif
