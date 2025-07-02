#include "mpu6050.h"

#ifdef __cplusplus
extern "C"{
#endif

static kalman_t kalman_x = {
	.q_angle = 0.001f,
	.q_bias = 0.003f,
	.r_measure = 0.03f
};

static kalman_t kalman_y = {
	.q_angle = 0.001f,
	.q_bias = 0.003f,
	.r_measure = 0.03f,
};

error_mpu6050 mpu6050_init(      mpu6050_t*         mpu6050, 
	                             I2C_HandleTypeDef* hi2c   , 
						   const UINT8              address)
{
	HAL_StatusTypeDef err;

	mpu6050->hi2c = hi2c;
	mpu6050->address = address << 1;

	mpu6050->get_acc = &mpu6050_get_acceleration;
	mpu6050->get_gyr = &mpu6050_get_gyroscope;
	mpu6050->get_temp = &mpu6050_get_temperature;
	mpu6050->get_all = &mpu6050_get_all;

	err = HAL_I2C_Mem_Write(mpu6050->hi2c, mpu6050->address, MPU6050_PWR_MGMT_1_REG, 1, 0x00, 1, MPU6050_I2C_TIMEOUT);
	if(err != HAL_OK) return MPU6050_ERR_I2C;

	err = HAL_I2C_Mem_Write(mpu6050->hi2c, mpu6050->address, MPU6050_SMPLRT_DIV_REG, 1, 0x07, 1, MPU6050_I2C_TIMEOUT);
	if(err != HAL_OK) return MPU6050_ERR_I2C;

	err = HAL_I2C_Mem_Write(mpu6050->hi2c, mpu6050->address, MPU6050_ACCEL_CONFIG_REG, 1, 0x00, 1, MPU6050_I2C_TIMEOUT);
	if(err != HAL_OK) return MPU6050_ERR_I2C;

	err = HAL_I2C_Mem_Write(mpu6050->hi2c, mpu6050->address, MPU6050_GYRO_CONFIG_REG, 1, 0x00, 1, MPU6050_I2C_TIMEOUT);
	if(err != HAL_OK) return MPU6050_ERR_I2C;

	return MPU6050_OK;
}

static error_mpu6050 mpu6050_kalman_angle(      kalman_t* kalman   , 
	                                      const FLOAT64   new_angle, 
										  const FLOAT64   new_rate , 
										  const FLOAT64   dt       ,
										        FLOAT64*  buf      )
{
	FLOAT64 rate = new_rate - kalman->bias;
	kalman->angle += dt * rate;

	kalman->p[0][0] += dt * (dt * kalman->p[1][1] - kalman->p[0][1] - kalman->p[1][0] + kalman->q_angle);
	kalman->p[0][1] -= dt * kalman->p[1][1];
	kalman->p[1][0] -= dt * kalman->p[1][1];
	kalman->p[1][1] += kalman->q_bias * dt;

	FLOAT64 S = kalman->p[0][0] + kalman->r_measure;
	FLOAT64 K[2];
	K[0] = kalman->p[0][0] / S;
	K[1] = kalman->p[1][0] / S;

	FLOAT64 y = new_angle - kalman->angle;
	*buf += K[0] * y;
	kalman->bias += K[1] * y;

	FLOAT64 p00_temp = kalman->p[0][0];
	FLOAT64 p01_temp = kalman->p[0][1];

	kalman->p[0][0] -= K[0] * p00_temp;
	kalman->p[0][1] -= K[0] * p01_temp;
	kalman->p[1][0] -= K[1] * p00_temp;
	kalman->p[1][1] -= K[1] * p01_temp;

	return MPU6050_OK;
}

error_mpu6050 mpu6050_get_acceleration(mpu6050_t* mpu6050)
{
	error_mpu6050 err;
	UINT8 data[6];

	err = HAL_I2C_Mem_Read(mpu6050->hi2c, mpu6050->address, MPU6050_ACCEL_XOUT_H_REG, 1, data, 6, MPU6050_I2C_TIMEOUT);
	if(err != MPU6050_OK) return err;

	mpu6050->accel_x_raw = (INT16)(data[0] << 8 | data[1]);
	mpu6050->accel_y_raw = (INT16)(data[2] << 8 | data[3]);
	mpu6050->accel_z_raw = (INT16)(data[4] << 8 | data[5]);

	mpu6050->ax = mpu6050->accel_x_raw / 16384.0f;
	mpu6050->ay = mpu6050->accel_y_raw / 16384.0f;
	mpu6050->az = mpu6050->accel_z_raw / 14418.0f;

	return MPU6050_OK;
}

error_mpu6050 mpu6050_get_gyroscope(mpu6050_t* mpu6050)
{
	error_mpu6050 err;
	UINT8 data[6];

	err = HAL_I2C_Mem_Read(mpu6050->hi2c, mpu6050->address, MPU6050_GYRO_XOUT_H_REG, 1, data, 6, MPU6050_I2C_TIMEOUT);
	if(err != MPU6050_OK) return err;

	mpu6050->gyro_x_raw = (int16_t)(data[0] << 8 | data[1]);
	mpu6050->gyro_y_raw = (int16_t)(data[2] << 8 | data[3]);
	mpu6050->gyro_z_raw = (int16_t)(data[4] << 8 | data[5]);

	mpu6050->gx = mpu6050->gyro_x_raw / 131.0f;
	mpu6050->gy = mpu6050->gyro_y_raw / 131.0f;
	mpu6050->gz = mpu6050->gyro_z_raw / 131.0f;

	return MPU6050_OK;
}

error_mpu6050 mpu6050_get_temperature(mpu6050_t* mpu6050)
{
	error_mpu6050 err;
	UINT8 data[2];
	INT16 temp;

	err = HAL_I2C_Mem_Read(mpu6050->hi2c, mpu6050->address, MPU6050_TEMP_OUT_H_REG, 1, data, 2, MPU6050_I2C_TIMEOUT);
	if(err != MPU6050_OK) return err;

	temp = (int16_t)(data[0] << 8 | data[1]);
	mpu6050->temperature = (float)((int16_t)temp / (float)340.0 + (float)36.53);

	return MPU6050_OK;
}

error_mpu6050 mpu6050_get_all(mpu6050_t* mpu6050)
{
	error_mpu6050 err;

	err = mpu6050_get_acceleration(mpu6050);
	if(err != MPU6050_OK) return err;

	err = mpu6050_get_gyroscope(mpu6050);
	if(err != MPU6050_OK) return err;

	err = mpu6050_get_temperature(mpu6050);
	if(err != MPU6050_OK) return err;

	FLOAT64 dt = (FLOAT64)(HAL_GetTick() - mpu6050->timer) / 1000;
	mpu6050->timer = HAL_GetTick();

	FLOAT64 roll_sqrt = sqrt(mpu6050->accel_x_raw * mpu6050->accel_x_raw + mpu6050->accel_z_raw * mpu6050->accel_z_raw);

	if (roll_sqrt != 0.0) {
		mpu6050->roll = atan(mpu6050->accel_y_raw / roll_sqrt) * RAD_TO_DEG;
	} else {
		mpu6050->roll = 0.0;
	}

	mpu6050->pitch = atan2(-mpu6050->accel_x_raw, mpu6050->accel_z_raw) * RAD_TO_DEG;
	mpu6050->yaw += mpu6050->gz * dt;

	if ((mpu6050->pitch < -90 && mpu6050->kalman_angle_y > 90) || (mpu6050->pitch > 90 && mpu6050->kalman_angle_y < -90)) {
		kalman_y.angle = mpu6050->pitch;
		mpu6050->kalman_angle_y = mpu6050->pitch;
	} else {
		mpu6050_kalman_angle(&kalman_y, mpu6050->pitch, mpu6050->gy, dt, &mpu6050->kalman_angle_y);
	}

	if (fabs(mpu6050->kalman_angle_y) > 90) {
		mpu6050->gx = -mpu6050->gx;
	}

	mpu6050_kalman_angle(&kalman_x, mpu6050->roll, mpu6050->gx, dt, &mpu6050->kalman_angle_x);

	return MPU6050_OK;
}

#ifdef __cplusplus
}
#endif
