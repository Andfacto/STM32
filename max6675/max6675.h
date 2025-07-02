#ifndef MAX6675_H
#define MAX6675_H

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

#define MAX6675_SPI_TIMEOUT 50U 

typedef enum {
	MAX6675_OK,
	MAX6675_ERR_SPI,
	MAX6675_ERR_NULL_PTR,
} error_max6675;

typedef struct _max6675_t{
	SPI_HandleTypeDef* hspi;
	GPIO_TypeDef* gpiox;
	UINT16 gpio_pin;

	error_max6675 (*get_temperature)(struct _max6675_t* max6675, FLOAT32* buf);
} max6675_t;

error_max6675 max6675_init(max6675_t* max6675, SPI_HandleTypeDef* hspi, const GPIO_TypeDef* gpiox, const UINT16 gpio_pin);

error_max6675 max6675_get_temperature(max6675_t* max6675, FLOAT32* buf);

#endif
