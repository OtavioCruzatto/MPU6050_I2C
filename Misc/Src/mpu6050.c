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

Status mpu6050Init(I2C_HandleTypeDef *hi2c, Mpu6050DeviceData *mpu6050Device)
{
	mpu6050Device->i2cHandler	= hi2c;
	mpu6050Device->address		= 0x68 << 1;
	mpu6050Device->whoAmI		= 0x68;
	mpu6050Device->sampleRate	= 0x0000;

	for (uint8_t cont = 0; cont < 3; cont++)
	{
		mpu6050Device->accelTest[cont] = 0x00;
		mpu6050Device->gyroTest[cont] = 0x00;
	}

	if (mpu6050CheckCommunication(mpu6050Device) == NOK)
	{
		return NOK;
	}

	if (mpu6050WhoAmI(mpu6050Device) != mpu6050Device->whoAmI)
	{
		return NOK;
	}

	if (mpu6050SetPwrMode(mpu6050Device, NORMAL) == NOK)
	{
		return NOK;
	}

	if (mpu6050SetConfig(mpu6050Device, DISABLED, DLPF_ACCEL_1KHZ_184HZ_GYRO_1KHZ_188HZ) == NOK)
	{
		return NOK;
	}

	if (mpu6050SetSampleRate(mpu6050Device, 1000) == NOK)
	{
		return NOK;
	}

	mpu6050GetAccelAndGyroSelfTestParams(mpu6050Device);

	return OK;
}

Status mpu6050CheckCommunication(Mpu6050DeviceData *mpu6050Device)
{
	Status communicationStatus = NOK;
	uint32_t communicationTrials = 5;
	if (HAL_I2C_IsDeviceReady(mpu6050Device->i2cHandler, mpu6050Device->address, communicationTrials, timeoutI2C) == HAL_OK)
	{
		communicationStatus = OK;
	}
	return communicationStatus;
}

uint8_t mpu6050WhoAmI(Mpu6050DeviceData *mpu6050Device)
{
	uint8_t id = 0;
	HAL_I2C_Mem_Read(mpu6050Device->i2cHandler, mpu6050Device->address, mpu6050Reg.whoAmI, sizeof(mpu6050Reg.whoAmI), &id, sizeof(id), timeoutI2C);
	return id;
}

void mpu6050GetAccelAndGyroSelfTestParams(Mpu6050DeviceData *mpu6050Device)
{
	uint8_t qtyBytes = 4;
	uint8_t selfTestBytes[4] = {0, 0, 0, 0};

	HAL_I2C_Mem_Read(mpu6050Device->i2cHandler, mpu6050Device->address, mpu6050Reg.selfTestXYZ, sizeof(mpu6050Reg.selfTestXYZ), selfTestBytes, qtyBytes, timeoutI2C);

	mpu6050Device->gyroTest[X_AXIS] = selfTestBytes[X_AXIS] & 0x1F;
	mpu6050Device->gyroTest[Y_AXIS] = selfTestBytes[Y_AXIS] & 0x1F;
	mpu6050Device->gyroTest[Z_AXIS] = selfTestBytes[Z_AXIS] & 0x1F;

	mpu6050Device->accelTest[X_AXIS] = ((selfTestBytes[X_AXIS] & 0xE0) >> 3) | ((selfTestBytes[XYZ_AXIS] & 0x30) >> 4);
	mpu6050Device->accelTest[Y_AXIS] = ((selfTestBytes[Y_AXIS] & 0xE0) >> 3) | ((selfTestBytes[XYZ_AXIS] & 0x0C) >> 2);
	mpu6050Device->accelTest[Z_AXIS] = ((selfTestBytes[Z_AXIS] & 0xE0) >> 3) | (selfTestBytes[XYZ_AXIS] & 0x03);
}

uint8_t mpu6050GetConfig(Mpu6050DeviceData *mpu6050Device)
{
	uint8_t config = 0;
	HAL_I2C_Mem_Read(mpu6050Device->i2cHandler, mpu6050Device->address, mpu6050Reg.config, sizeof(mpu6050Reg.config), &config, 1, timeoutI2C);
	return config;
}

Status mpu6050SetConfig(Mpu6050DeviceData *mpu6050Device, ExternalSyncSet externalSyncSet, DigitalLowPassFilter digitalLowPassFilter)
{
	uint8_t config = (externalSyncSet << 3) | digitalLowPassFilter;

	HAL_I2C_Mem_Write(mpu6050Device->i2cHandler, mpu6050Device->address, mpu6050Reg.config, sizeof(mpu6050Reg.config), &config, sizeof(config), timeoutI2C);

	if (mpu6050GetConfig(mpu6050Device) != config)
	{
		return NOK;
	}

	return OK;
}

uint8_t mpu6050GetPwrMode(Mpu6050DeviceData *mpu6050Device)
{
	uint8_t pwrMode = 0;
	HAL_I2C_Mem_Read(mpu6050Device->i2cHandler, mpu6050Device->address, mpu6050Reg.pwrMgmt1, sizeof(mpu6050Reg.pwrMgmt1), &pwrMode, sizeof(pwrMode), timeoutI2C);
	return pwrMode;
}

Status mpu6050SetPwrMode(Mpu6050DeviceData *mpu6050Device, PowerMode powerMode)
{
	uint8_t pwrMode = 0xFF;

	if (powerMode == NORMAL)
	{
		pwrMode = mpu6050GetPwrMode(mpu6050Device) & 0xBF;
	}
	else if (powerMode == SLEEP)
	{
		pwrMode = mpu6050GetPwrMode(mpu6050Device) | 0x40;
	}

	HAL_I2C_Mem_Write(mpu6050Device->i2cHandler, mpu6050Device->address, mpu6050Reg.pwrMgmt1, sizeof(mpu6050Reg.pwrMgmt1), &pwrMode, sizeof(pwrMode), timeoutI2C);

	if (mpu6050GetPwrMode(mpu6050Device) != pwrMode)
	{
		return NOK;
	}

	return OK;
}

uint16_t mpu6050GetSampleRate(Mpu6050DeviceData *mpu6050Device)
{
	uint16_t sampleRate = 0;
	uint8_t smplrtDiv = 0;
	uint8_t config = mpu6050GetConfig(mpu6050Device);
	uint8_t dlpfCfg = config & 0x07;

	HAL_I2C_Mem_Read(mpu6050Device->i2cHandler, mpu6050Device->address, mpu6050Reg.smplrtDiv, sizeof(mpu6050Reg.smplrtDiv), &smplrtDiv, 1, timeoutI2C);

	if ((dlpfCfg == DLPF_ACCEL_1KHZ_260HZ_GYRO_8KHZ_256HZ) || (dlpfCfg == DLPF_ACCEL_1KHZ_RES_GYRO_8KHZ_RES))
	{
		sampleRate = 8000 / (1 + smplrtDiv);
	}
	else
	{
		sampleRate = 1000 / (1 + smplrtDiv);
	}

	mpu6050Device->sampleRate = sampleRate;

	return sampleRate;
}

Status mpu6050SetSampleRate(Mpu6050DeviceData *mpu6050Device, uint16_t sampleRate)
{
	uint8_t config = mpu6050GetConfig(mpu6050Device);
	uint8_t dlpfCfg = config & 0x07;
	uint8_t smplrtDiv = 0;

	if (sampleRate > 8000)
	{
		return NOK;
	}

	if ((dlpfCfg != DLPF_ACCEL_1KHZ_260HZ_GYRO_8KHZ_256HZ) || (dlpfCfg != DLPF_ACCEL_1KHZ_RES_GYRO_8KHZ_RES))
	{
		if (sampleRate > 1000)
		{
			return NOK;
		}

		smplrtDiv = (1000 / sampleRate) - 1;
	}
	else
	{
		smplrtDiv = (8000 / sampleRate) - 1;
	}

	HAL_I2C_Mem_Write(mpu6050Device->i2cHandler, mpu6050Device->address, mpu6050Reg.smplrtDiv, sizeof(mpu6050Reg.smplrtDiv), &smplrtDiv, sizeof(smplrtDiv), timeoutI2C);

	if (mpu6050GetSampleRate(mpu6050Device) != sampleRate)
	{
		return NOK;
	}

	return OK;
}
