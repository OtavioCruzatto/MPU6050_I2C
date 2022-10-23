/*
 * mpu6050.h
 *
 *  Created on: Oct 5, 2022
 *      Author: Otavio
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#include "main.h"

typedef struct
{
	uint8_t selfTestXYZ;
	uint8_t smplrtDiv;
	uint8_t config;
	uint8_t gyroConfig;
	uint8_t accelConfig;
	uint8_t fifoEn;
	uint8_t intPinCfg;
	uint8_t intEnable;
	uint8_t intStatus;
	uint8_t accelOutXYZ;
	uint8_t tempOutH;
	uint8_t tempOutL;
	uint8_t gyroXoutH;
	uint8_t gyroXoutL;
	uint8_t gyroYoutH;
	uint8_t gyroYoutL;
	uint8_t gyroZoutH;
	uint8_t gyroZoutL;
	uint8_t signalPathReset;
	uint8_t userCtrl;
	uint8_t pwrMgmt1;
	uint8_t pwrMgmt2;
	uint8_t fifoCountH;
	uint8_t fifoCountL;
	uint8_t fifoRW;
	uint8_t whoAmI;
} Mpu6050Reg;

typedef struct
{
	I2C_HandleTypeDef *i2cHandler;
	uint8_t address;
	uint8_t whoAmI;
	uint8_t accelTest[3];
	uint8_t gyroTest[3];
	uint16_t sampleRate;
	float accel[3];
	int16_t accelSensitivity;
} Mpu6050DeviceData;

typedef enum STATUS
{
	NOK	= 0x00,
	OK
} Status;

typedef enum AXIS
{
	X_AXIS = 0x00,
	Y_AXIS,
	Z_AXIS,
	XYZ_AXIS
} Axis;

typedef enum EXT_SYNC_SET
{
	DISABLED	= 0x00,
	TEMP_OUT_L_BIT0,
	GYRO_XOUT_L_BIT0,
	GYRO_YOUT_L_BIT0,
	GYRO_ZOUT_L_BIT0,
	ACCEL_XOUT_L_BIT0,
	ACCEL_YOUT_L_BIT0,
	ACCEL_ZOUT_L_BIT0
} ExternalSyncSet;

typedef enum DIGITAL_LPF
{
	DLPF_ACCEL_1KHZ_260HZ_GYRO_8KHZ_256HZ	= 0x00,
	DLPF_ACCEL_1KHZ_184HZ_GYRO_1KHZ_188HZ,
	DLPF_ACCEL_1KHZ_94HZ_GYRO_1KHZ_98HZ,
	DLPF_ACCEL_1KHZ_44HZ_GYRO_1KHZ_42HZ,
	DLPF_ACCEL_1KHZ_21HZ_GYRO_1KHZ_20HZ,
	DLPF_ACCEL_1KHZ_10HZ_GYRO_1KHZ_10HZ,
	DLPF_ACCEL_1KHZ_5HZ_GYRO_1KHZ_5HZ,
	DLPF_ACCEL_1KHZ_RES_GYRO_8KHZ_RES
} DigitalLowPassFilter;

typedef enum POWER_MODE
{
	NORMAL	= 0x00,
	SLEEP
} PowerMode;

typedef enum GYRO_FULL_SCALE_RANGE
{
	PLUS_MINUS_250_DEGREE_PER_SECOND	= 0x00,
	PLUS_MINUS_500_DEGREE_PER_SECOND,
	PLUS_MINUS_1000_DEGREE_PER_SECOND,
	PLUS_MINUS_2000_DEGREE_PER_SECOND
} GyroFullScaleRange;

typedef enum ACCEL_FULL_SCALE_RANGE
{
	PLUS_MINUS_2_G	= 0x00,
	PLUS_MINUS_4_G,
	PLUS_MINUS_8_G,
	PLUS_MINUS_16_G
} AccelFullScaleRange;

Status mpu6050Init(I2C_HandleTypeDef *hi2c, Mpu6050DeviceData *mpu6050Device);
Status mpu6050CheckCommunication(Mpu6050DeviceData *mpu6050Device);
uint8_t mpu6050WhoAmI(Mpu6050DeviceData *mpu6050Device);
void mpu6050GetAccelAndGyroSelfTestParams(Mpu6050DeviceData *mpu6050Device);
Status mpu6050SetConfig(Mpu6050DeviceData *mpu6050Device, ExternalSyncSet externalSyncSet, DigitalLowPassFilter digitalLowPassFilter);
uint8_t mpu6050GetConfig(Mpu6050DeviceData *mpu6050Device);
Status mpu6050SetPwrMode(Mpu6050DeviceData *mpu6050Device, PowerMode powerMode);
uint8_t mpu6050GetPwrMode(Mpu6050DeviceData *mpu6050Device);
uint16_t mpu6050GetSampleRate(Mpu6050DeviceData *mpu6050Device);
Status mpu6050SetSampleRate(Mpu6050DeviceData *mpu6050Device, uint16_t sampleRate);
uint8_t mpu6050GetGyroFullScaleConfig(Mpu6050DeviceData *mpu6050Device);
Status mpu6050SetGyroFullScaleConfig(Mpu6050DeviceData *mpu6050Device, GyroFullScaleRange gyroFullScaleRange);
uint8_t mpu6050GetAccelFullScaleConfig(Mpu6050DeviceData *mpu6050Device);
Status mpu6050SetAccelFullScaleConfig(Mpu6050DeviceData *mpu6050Device, AccelFullScaleRange accelFullScaleRange);
void mpu6050SetAccelSensivity(Mpu6050DeviceData *mpu6050Device, AccelFullScaleRange accelFullScaleRange);
void mpu6050GetAccel(Mpu6050DeviceData *mpu6050Device);

#endif /* INC_MPU6050_H_ */
