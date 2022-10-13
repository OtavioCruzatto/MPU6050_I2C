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
	uint8_t i2cMstCtrl;
	uint8_t i2cSlv0Addr;
	uint8_t i2cSlv0Reg;
	uint8_t i2cSlv0Ctrl;
	uint8_t i2cSlv1Addr;
	uint8_t i2cSlv1Reg;
	uint8_t i2cSlv1Ctrl;
	uint8_t i2cSlv2Addr;
	uint8_t i2cSlv2Reg;
	uint8_t i2cSlv2Ctrl;
	uint8_t i2cSlv3Addr;
	uint8_t i2cSlv3Reg;
	uint8_t i2cSlv3Ctrl;
	uint8_t i2cSlv4Addr;
	uint8_t i2cSlv4Reg;
	uint8_t i2cSlv4Do;
	uint8_t i2cSlv4Ctrl;
	uint8_t i2cSlv4Di;
	uint8_t i2cMstStatus;
	uint8_t intPinCfg;
	uint8_t intEnable;
	uint8_t intStatus;
	uint8_t accelXoutH;
	uint8_t accelXoutL;
	uint8_t accelYoutH;
	uint8_t accelYoutL;
	uint8_t accelZoutH;
	uint8_t accelZoutL;
	uint8_t tempOutH;
	uint8_t tempOutL;
	uint8_t gyroXoutH;
	uint8_t gyroXoutL;
	uint8_t gyroYoutH;
	uint8_t gyroYoutL;
	uint8_t gyroZoutH;
	uint8_t gyroZoutL;
	uint8_t extSensData00;
	uint8_t extSensData01;
	uint8_t extSensData02;
	uint8_t extSensData03;
	uint8_t extSensData04;
	uint8_t extSensData05;
	uint8_t extSensData06;
	uint8_t extSensData07;
	uint8_t extSensData08;
	uint8_t extSensData09;
	uint8_t extSensData10;
	uint8_t extSensData11;
	uint8_t extSensData12;
	uint8_t extSensData13;
	uint8_t extSensData14;
	uint8_t extSensData15;
	uint8_t extSensData16;
	uint8_t extSensData17;
	uint8_t extSensData18;
	uint8_t extSensData19;
	uint8_t extSensData20;
	uint8_t extSensData21;
	uint8_t extSensData22;
	uint8_t extSensData23;
	uint8_t i2cSlv0Do;
	uint8_t i2cSlv1Do;
	uint8_t i2cSlv2Do;
	uint8_t i2cSlv3Do;
	uint8_t i2cMstDelayCtrl;
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

Status mpu6050Init(I2C_HandleTypeDef *hi2c, Mpu6050DeviceData *mpu6050Device);
Status mpu6050CheckCommunication(Mpu6050DeviceData *mpu6050Device);
uint8_t mpu6050WhoAmI(Mpu6050DeviceData *mpu6050Device);
void mpu6050GetAccelAndGyroSelfTestParams(Mpu6050DeviceData *mpu6050Device);
Status mpu6050SetConfig(Mpu6050DeviceData *mpu6050Device, ExternalSyncSet externalSyncSet, DigitalLowPassFilter digitalLowPassFilter);
uint8_t mpu6050GetConfig(Mpu6050DeviceData *mpu6050Device);
Status mpu6050SetPwrMode(Mpu6050DeviceData *mpu6050Device, PowerMode powerMode);
uint8_t mpu6050GetPwrMode(Mpu6050DeviceData *mpu6050Device);



#endif /* INC_MPU6050_H_ */
