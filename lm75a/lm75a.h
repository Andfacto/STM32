#ifndef LM75A_H
#define LM75A_H

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

#define LM75A_DEV_ADDRESS			0x48

#define LM75A_TEMP_REG 				0x00
#define LM75A_CONFIG_REG 			0x01
#define LM75A_THYST_REG				0x02
#define LM75A_TOS_REG				0x03
#define LM75A_PRODID_REG			0x07

#define LM75A_I2C_TIMEOUT         	50U

typedef enum {
	LM75A_DEC_5, 
	LM75A_DEC_25, 
	LM75A_DEC_125,
} lm75a_decmode_t;

typedef enum {
	LM75A_OK,
	LM75A_ERR_I2C,
	LM75A_ERR_NULL_PTR,
} error_lm75a;

typedef struct _lm75a_t{
	I2C_HandleTypeDef* hi2c;
	UINT8 address;
	lm75a_decmode_t mode;

	error_lm75a (*shutdown)(struct _lm75a_t* lm75a);
	error_lm75a (*wakeup)(struct _lm75a_t* lm75a);
	error_lm75a (*get_productid)(struct _lm75a_t* lm75a, UINT8* buf);
	error_lm75a (*get_temperature)(struct _lm75a_t* lm75a, FLOAT32* buf);
	error_lm75a (*get_hyst)(struct _lm75a_t* lm75a, FLOAT32* buf);
	error_lm75a (*set_hyst)(struct _lm75a_t* lm75a, FLOAT32 data);
	error_lm75a (*get_os)(struct _lm75a_t* lm75a, FLOAT32* buf);
	error_lm75a (*set_os)(struct _lm75a_t* lm75a, FLOAT32 data);
	error_lm75a (*get_config)(struct _lm75a_t* lm75a, UINT8* buf);
	error_lm75a (*set_config)(struct _lm75a_t* lm75a, const UINT8 data);
} lm75a_t;

error_lm75a lm75a_init(lm75a_t* lm75a, I2C_HandleTypeDef* hi2c, const UINT8 address, const lm75a_decmode_t mode);

error_lm75a lm75a_shutdown(lm75a_t* lm75a);
error_lm75a lm75a_wakeup(lm75a_t* lm75a);

error_lm75a lm75a_get_productid(lm75a_t* lm75a, UINT8* buf);

error_lm75a lm75a_get_temperature(lm75a_t* lm75a, FLOAT32* buf);

error_lm75a lm75a_get_hyst(lm75a_t* lm75a, FLOAT32* buf);
error_lm75a lm75a_set_hyst(lm75a_t* lm75a, FLOAT32 data);

error_lm75a lm75a_get_os(lm75a_t* lm75a, FLOAT32* buf);
error_lm75a lm75a_set_os(lm75a_t* lm75a, FLOAT32 data);

error_lm75a lm75a_get_config(lm75a_t* lm75a, UINT8* buf);
error_lm75a lm75a_set_config(lm75a_t* lm75a, const UINT8 data);

#endif
