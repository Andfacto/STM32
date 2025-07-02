#ifndef SHARP_H
#define SHARP_H

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

#define SHARP_ADC_TIMEOUT 	50U

typedef enum {
	SHARP_GP2Y0A21,	//10-80 cm
	SHARP_GP2Y0A02,	//20-150 cm
} sharp_model_t;

typedef enum {
	SHARP_OK,
	SHARP_ERR_ADC,
	SHARP_ERR_NULL_PTR,
} error_sharp;

typedef struct _sharp_t{
	ADC_HandleTypeDef* hadc;
	UINT32 channel;
	sharp_model_t model;

	error_sharp (*get_distance)(struct _sharp_t* sharp, FLOAT32* buf);
} sharp_t;

error_sharp sharp_init(sharp_t* sharp, ADC_HandleTypeDef* hadc, UINT32 channel, sharp_model_t model);
error_sharp sharp_get_distance(sharp_t* sharp, FLOAT32* buf);

#endif