/*
 * mpu6050.c
 *
 *  Created on: Oct 5, 2022
 *      Author: Otavio
 */

#include "mpu6050.h"

static const uint32_t timeoutI2C = 100;
static const Mpu6050Reg mpu6050Reg =
{
	.selfTestXYZ	 = 0x0D,
	.selfTestA 		 = 0x10,
	.smplrtDiv 		 = 0x19,
	.config 		 = 0x1A,
	.gyroConfig 	 = 0x1B,
	.accelConfig 	 = 0x1C,
	.fifoEn 		 = 0x23,
	.i2cMstCtrl 	 = 0x24,
	.i2cSlv0Addr 	 = 0x25,
	.i2cSlv0Reg 	 = 0x26,
	.i2cSlv0Ctrl 	 = 0x27,
	.i2cSlv1Addr 	 = 0x28,
	.i2cSlv1Reg 	 = 0x29,
	.i2cSlv1Ctrl 	 = 0x2A,
	.i2cSlv2Addr 	 = 0x2B,
	.i2cSlv2Reg 	 = 0x2C,
	.i2cSlv2Ctrl 	 = 0x2D,
	.i2cSlv3Addr 	 = 0x2E,
	.i2cSlv3Reg 	 = 0x2F,
	.i2cSlv3Ctrl 	 = 0x30,
	.i2cSlv4Addr 	 = 0x31,
	.i2cSlv4Reg 	 = 0x32,
	.i2cSlv4Do 		 = 0x33,
	.i2cSlv4Ctrl 	 = 0x34,
	.i2cSlv4Di 		 = 0x35,
	.i2cMstStatus 	 = 0x36,
	.intPinCfg 		 = 0x37,
	.intEnable 		 = 0x38,
	.intStatus 		 = 0x3A,
	.accelXoutH 	 = 0x3B,
	.accelXoutL 	 = 0x3C,
	.accelYoutH 	 = 0x3D,
	.accelYoutL 	 = 0x3E,
	.accelZoutH 	 = 0x3F,
	.accelZoutL 	 = 0x40,
	.tempOutH 		 = 0x41,
	.tempOutL 		 = 0x42,
	.gyroXoutH 		 = 0x43,
	.gyroXoutL 		 = 0x44,
	.gyroYoutH 		 = 0x45,
	.gyroYoutL 		 = 0x46,
	.gyroZoutH 		 = 0x47,
	.gyroZoutL 		 = 0x48,
	.extSensData00 	 = 0x49,
	.extSensData01 	 = 0x4A,
	.extSensData02 	 = 0x4B,
	.extSensData03 	 = 0x4C,
	.extSensData04 	 = 0x4D,
	.extSensData05 	 = 0x4E,
	.extSensData06 	 = 0x4F,
	.extSensData07 	 = 0x50,
	.extSensData08 	 = 0x51,
	.extSensData09 	 = 0x52,
	.extSensData10 	 = 0x53,
	.extSensData11 	 = 0x54,
	.extSensData12   = 0x55,
	.extSensData13 	 = 0x56,
	.extSensData14 	 = 0x57,
	.extSensData15 	 = 0x58,
	.extSensData16 	 = 0x59,
	.extSensData17 	 = 0x5A,
	.extSensData18 	 = 0x5B,
	.extSensData19	 = 0x5C,
	.extSensData20 	 = 0x5D,
	.extSensData21 	 = 0x5E,
	.extSensData22 	 = 0x5F,
	.extSensData23 	 = 0x60,
	.i2cSlv0Do 	   	 = 0x63,
	.i2cSlv1Do 	   	 = 0x64,
	.i2cSlv2Do		 = 0x65,
	.i2cSlv3Do		 = 0x66,
	.i2cMstDelayCtrl = 0x67,
	.signalPathReset = 0x68,
	.userCtrl		 = 0x6A,
	.pwrMgmt1		 = 0x6B,
	.pwrMgmt2		 = 0x6C,
	.fifoCountH		 = 0x72,
	.fifoCountL		 = 0x73,
	.fifoRW			 = 0x74,
	.whoAmI			 = 0x75
};

CommStatus mpu6050Init(I2C_HandleTypeDef *hi2c, Mpu6050DeviceData *mpu6050Device)
{
	CommStatus communicationStatus = NOK;

	mpu6050Device->address		= 0x68 << 1;
	mpu6050Device->whoAmI		= 0x68;

	uint8_t cont = 0;
	for (cont = 0; cont < 3; cont++)
	{
		mpu6050Device->accelTest[cont] = 0x00;
		mpu6050Device->gyroTest[cont] = 0x00;
	}

	if (mpu6050CheckCommunication(hi2c, mpu6050Device) == NOK)
	{
		return NOK;
	}

	if (mpu6050WhoAmI(hi2c, mpu6050Device) == mpu6050Device->whoAmI)
	{
		communicationStatus = OK;
	}

	return communicationStatus;
}

CommStatus mpu6050CheckCommunication(I2C_HandleTypeDef *hi2c, Mpu6050DeviceData *mpu6050Device)
{
	CommStatus communicationStatus = NOK;
	uint32_t communicationTrials = 5;
	if (HAL_I2C_IsDeviceReady(hi2c, mpu6050Device->address, communicationTrials, timeoutI2C) == HAL_OK)
	{
		communicationStatus = OK;
	}
	return communicationStatus;
}

uint8_t mpu6050WhoAmI(I2C_HandleTypeDef *hi2c, Mpu6050DeviceData *mpu6050Device)
{
	uint8_t id = 0;
	HAL_I2C_Mem_Read(hi2c, mpu6050Device->address, mpu6050Reg.whoAmI, sizeof(mpu6050Reg.whoAmI), &id, sizeof(id), timeoutI2C);
	return id;
}

void mpu6050GetAccelAndGyroSelfTestParams(I2C_HandleTypeDef *hi2c, Mpu6050DeviceData *mpu6050Device)
{
	uint8_t selfTestBytes[3] = {0, 0, 0};
	uint8_t qtyBytes = 3;

	HAL_I2C_Mem_Read(hi2c, mpu6050Device->address, mpu6050Reg.selfTestXYZ, sizeof(mpu6050Reg.selfTestXYZ), selfTestBytes, qtyBytes, timeoutI2C);

	mpu6050Device->gyroTest[X_AXIS] = selfTestBytes[X_AXIS] & 0x1F;
	mpu6050Device->gyroTest[Y_AXIS] = selfTestBytes[Y_AXIS] & 0x1F;
	mpu6050Device->gyroTest[Z_AXIS] = selfTestBytes[Z_AXIS] & 0x1F;

	mpu6050Device->accelTest[X_AXIS] = (selfTestBytes[X_AXIS] & 0xE0) >> 5;
	mpu6050Device->accelTest[Y_AXIS] = (selfTestBytes[Y_AXIS] & 0xE0) >> 5;
	mpu6050Device->accelTest[Z_AXIS] = (selfTestBytes[Z_AXIS] & 0xE0) >> 5;
}
