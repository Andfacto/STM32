#ifndef MPU6050_H
#define MPU6050_H

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

#define MPU6050_DEV_ADRESS              0x68

#define MPU6050_WHO_AM_I_REG            0x75
#define MPU6050_PWR_MGMT_1_REG          0x6B
#define MPU6050_SMPLRT_DIV_REG          0x19
#define MPU6050_ACCEL_CONFIG_REG        0x1C
#define MPU6050_ACCEL_XOUT_H_REG        0x3B
#define MPU6050_TEMP_OUT_H_REG          0x41
#define MPU6050_GYRO_CONFIG_REG         0x1B
#define MPU6050_GYRO_XOUT_H_REG         0x43

#define MPU6050_I2C_TIMEOUT             50U

#define RAD_TO_DEG                      57.295779513f

typedef enum {
	MPU6050_OK,
	MPU6050_ERR_I2C,
	MPU6050_ERR_NULL_PTR,
} error_mpu6050;

typedef struct {
    FLOAT64 q_angle;
    FLOAT64 q_bias;
    FLOAT64 r_measure;
    FLOAT64 angle;
    FLOAT64 bias;
    FLOAT64 p[2][2];
} kalman_t;

typedef struct _mpu6050_t{
	I2C_HandleTypeDef* hi2c;
    UINT8 address;

    INT16 accel_x_raw;
    INT16 accel_y_raw;
    INT16 accel_z_raw;

    FLOAT64 ax;
    FLOAT64 ay;
    FLOAT64 az;

    INT16 gyro_x_raw;
    INT16 gyro_y_raw;
    INT16 gyro_z_raw;

    FLOAT64 gx;
    FLOAT64 gy;
    FLOAT64 gz;

    FLOAT64 temperature;

    FLOAT64 kalman_angle_x;
    FLOAT64 kalman_angle_y;

    FLOAT64 roll;
    FLOAT64 pitch;
    FLOAT64 yaw;

    UINT32 timer;

    error_mpu6050 (*get_acc)(struct _mpu6050_t* mpu6050);
    error_mpu6050 (*get_gyr)(struct _mpu6050_t* mpu6050);
    error_mpu6050 (*get_temp)(struct _mpu6050_t* mpu6050);
    error_mpu6050 (*get_all)(struct _mpu6050_t* mpu6050);
} mpu6050_t;

error_mpu6050 mpu6050_init(mpu6050_t* mpu6050, I2C_HandleTypeDef* hi2c, const UINT8 address);

error_mpu6050 mpu6050_get_acceleration(mpu6050_t* mpu6050);

error_mpu6050 mpu6050_get_gyroscope(mpu6050_t* mpu6050);

error_mpu6050 mpu6050_get_temperature(mpu6050_t* mpu6050);

error_mpu6050 mpu6050_get_all(mpu6050_t* mpu6050);

#endif
