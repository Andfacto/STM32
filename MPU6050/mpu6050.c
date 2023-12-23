#include <math.h>
#include "mpu6050.h"

#ifdef __cplusplus
extern "C"{
#endif

uint32_t timer;

Kalman KalmanX = {
	.Q_angle = 0.001f,
	.Q_bias = 0.003f,
	.R_measure = 0.03f
};

Kalman KalmanY = {
	.Q_angle = 0.001f,
	.Q_bias = 0.003f,
	.R_measure = 0.03f,
};

uint8_t MPU6050_Init(MPU6050* mpu6050, I2C_HandleTypeDef* hi2c)
{
	uint8_t check;

	mpu6050->_hi2c = hi2c;

	HAL_I2C_Mem_Write(mpu6050->_hi2c, MPU6050_DEV_ADRESS, MPU6050_PWR_MGMT_1_REG, 1, 0x00, 1, MPU6050_I2C_TIMEOUT);
	HAL_I2C_Mem_Write(mpu6050->_hi2c, MPU6050_DEV_ADRESS, MPU6050_SMPLRT_DIV_REG, 1, 0x07, 1, MPU6050_I2C_TIMEOUT);
	HAL_I2C_Mem_Write(mpu6050->_hi2c, MPU6050_DEV_ADRESS, MPU6050_ACCEL_CONFIG_REG, 1, 0x00, 1, MPU6050_I2C_TIMEOUT);
	HAL_I2C_Mem_Write(mpu6050->_hi2c, MPU6050_DEV_ADRESS, MPU6050_GYRO_CONFIG_REG, 1, 0x00, 1, MPU6050_I2C_TIMEOUT);
}

void MPU6050_ReadAcceleration(MPU6050* mpu6050)
{
	uint8_t data[6];

	HAL_I2C_Mem_Read(mpu6050->_hi2c, MPU6050_DEV_ADRESS, MPU6050_ACCEL_XOUT_H_REG, 1, data, 6, MPU6050_I2C_TIMEOUT);

	mpu6050->Accel_X_RAW = (int16_t)(data[0] << 8 | data[1]);
	mpu6050->Accel_Y_RAW = (int16_t)(data[2] << 8 | data[3]);
	mpu6050->Accel_Z_RAW = (int16_t)(data[4] << 8 | data[5]);

	mpu6050->Ax = mpu6050->Accel_X_RAW / 16384.0;
	mpu6050->Ay = mpu6050->Accel_Y_RAW / 16384.0;
	mpu6050->Az = mpu6050->Accel_Z_RAW / 14418.0;
}

void MPU6050_ReadGyroscope(MPU6050* mpu6050)
{
	uint8_t data[6];

	HAL_I2C_Mem_Read(mpu6050->_hi2c, MPU6050_DEV_ADRESS, MPU6050_GYRO_XOUT_H_REG, 1, data, 6, MPU6050_I2C_TIMEOUT);

	mpu6050->Gyro_X_RAW = (int16_t)(data[0] << 8 | data[1]);
	mpu6050->Gyro_Y_RAW = (int16_t)(data[2] << 8 | data[3]);
	mpu6050->Gyro_Z_RAW = (int16_t)(data[4] << 8 | data[5]);

	mpu6050->Gx = mpu6050->Gyro_X_RAW / 131.0;
	mpu6050->Gy = mpu6050->Gyro_Y_RAW / 131.0;
	mpu6050->Gz = mpu6050->Gyro_Z_RAW / 131.0;
}

void MPU6050_ReadTemperature(MPU6050* mpu6050)
{
	uint8_t data[2];
	int16_t temp;

	HAL_I2C_Mem_Read(mpu6050->_hi2c, MPU6050_DEV_ADRESS, MPU6050_TEMP_OUT_H_REG, 1, data, 2, MPU6050_I2C_TIMEOUT);

	temp = (int16_t)(data[0] << 8 | data[1]);

	mpu6050->Temperature = (float)((int16_t)temp / (float)340.0 + (float)36.53);
}

void MPU6050_ReadAll(MPU6050* mpu6050)
{
	MPU6050_ReadAcceleration(mpu6050);
	MPU6050_ReadGyroscope(mpu6050);
	MPU6050_ReadTemperature(mpu6050);

	double dt = (double)(HAL_GetTick() - timer) / 1000;
	timer = HAL_GetTick();

	double roll_sqrt = sqrt(mpu6050->Accel_X_RAW * mpu6050->Accel_X_RAW + mpu6050->Accel_Z_RAW * mpu6050->Accel_Z_RAW);

	if (roll_sqrt != 0.0)
	{
		mpu6050->roll = atan(mpu6050->Accel_Y_RAW / roll_sqrt) * RAD_TO_DEG;
	}
	else
	{
		mpu6050->roll = 0.0;
	}

	mpu6050->pitch = atan2(-mpu6050->Accel_X_RAW, mpu6050->Accel_Z_RAW) * RAD_TO_DEG;
	mpu6050->yaw += mpu6050->Gz * dt;

	if ((mpu6050->pitch < -90 && mpu6050->KalmanAngleY > 90) || (mpu6050->pitch > 90 && mpu6050->KalmanAngleY < -90))
	{
		KalmanY.angle = mpu6050->pitch;
		mpu6050->KalmanAngleY = mpu6050->pitch;
	}
	else
	{
		mpu6050->KalmanAngleY = MPU6050_GetKalmanAngle(&KalmanY, mpu6050->pitch, mpu6050->Gy, dt);
	}

	if (fabs(mpu6050->KalmanAngleY) > 90)
		mpu6050->Gx = -mpu6050->Gx;

	mpu6050->KalmanAngleX = MPU6050_GetKalmanAngle(&KalmanX, mpu6050->roll, mpu6050->Gx, dt);
}

double MPU6050_GetKalmanAngle(Kalman* Kalman, double newAngle, double newRate, double dt)
{
	double rate = newRate - Kalman->bias;
	Kalman->angle += dt * rate;

	Kalman->P[0][0] += dt * (dt * Kalman->P[1][1] - Kalman->P[0][1] - Kalman->P[1][0] + Kalman->Q_angle);
	Kalman->P[0][1] -= dt * Kalman->P[1][1];
	Kalman->P[1][0] -= dt * Kalman->P[1][1];
	Kalman->P[1][1] += Kalman->Q_bias * dt;

	double S = Kalman->P[0][0] + Kalman->R_measure;
	double K[2];
	K[0] = Kalman->P[0][0] / S;
	K[1] = Kalman->P[1][0] / S;

	double y = newAngle - Kalman->angle;
	Kalman->angle += K[0] * y;
	Kalman->bias += K[1] * y;

	double P00_temp = Kalman->P[0][0];
	double P01_temp = Kalman->P[0][1];

	Kalman->P[0][0] -= K[0] * P00_temp;
	Kalman->P[0][1] -= K[0] * P01_temp;
	Kalman->P[1][0] -= K[1] * P00_temp;
	Kalman->P[1][1] -= K[1] * P01_temp;

	return Kalman->angle;
};

#ifdef __cplusplus
}
#endif
