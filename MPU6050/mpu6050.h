#ifndef MPU6050_H
#define MPU6050_H

#include "main.h"

#define MPU6050_DEV_ADRESS              0xD0

#define MPU6050_WHO_AM_I_REG            0x75
#define MPU6050_PWR_MGMT_1_REG          0x6B
#define MPU6050_SMPLRT_DIV_REG          0x19
#define MPU6050_ACCEL_CONFIG_REG        0x1C
#define MPU6050_ACCEL_XOUT_H_REG        0x3B
#define MPU6050_TEMP_OUT_H_REG          0x41
#define MPU6050_GYRO_CONFIG_REG         0x1B
#define MPU6050_GYRO_XOUT_H_REG         0x43

#define MPU6050_I2C_TIMEOUT             50

#define RAD_TO_DEG                      57.295779513

typedef struct
{
	I2C_HandleTypeDef* _hi2c;

    int16_t Accel_X_RAW;
    int16_t Accel_Y_RAW;
    int16_t Accel_Z_RAW;
    double Ax;
    double Ay;
    double Az;

    int16_t Gyro_X_RAW;
    int16_t Gyro_Y_RAW;
    int16_t Gyro_Z_RAW;
    double Gx;
    double Gy;
    double Gz;

    float Temperature;

    double KalmanAngleX;
    double KalmanAngleY;

    double roll;
    double pitch;
    double yaw;
} MPU6050;

typedef struct
{
    double Q_angle;
    double Q_bias;
    double R_measure;
    double angle;
    double bias;
    double P[2][2];
} Kalman;

uint8_t MPU6050_Init(MPU6050* mpu6050, I2C_HandleTypeDef* hi2c);

void MPU6050_ReadAcceleration(MPU6050* mpu6050);

void MPU6050_ReadGyroscope(MPU6050* mpu6050);

void MPU6050_ReadTemperature(MPU6050* mpu6050);

void MPU6050_ReadAll(MPU6050* mpu6050);

double MPU6050_GetKalmanAngle(Kalman* Kalman, double newAngle, double newRate, double dt);

#endif
