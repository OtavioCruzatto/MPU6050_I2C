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
	.intPinCfg 		 = 0x37,
	.intEnable 		 = 0x38,
	.intStatus 		 = 0x3A,
	.accelOutXYZ 	 = 0x3B,
	.tempOutH 		 = 0x41,
	.tempOutL 		 = 0x42,
	.gyroXoutH 		 = 0x43,
	.gyroXoutL 		 = 0x44,
	.gyroYoutH 		 = 0x45,
	.gyroYoutL 		 = 0x46,
	.gyroZoutH 		 = 0x47,
	.gyroZoutL 		 = 0x48,
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
	mpu6050Device->i2cHandler		= hi2c;
	mpu6050Device->address			= 0x68 << 1;
	mpu6050Device->whoAmI			= 0x68;
	mpu6050Device->sampleRate		= 0x0000;
	mpu6050Device->accelSensitivity	= 0x00;

	for (uint8_t cont = 0; cont < 3; cont++)
	{
		mpu6050Device->accelTest[cont] = 0x00;
		mpu6050Device->gyroTest[cont] = 0x00;
		mpu6050Device->accel[cont] = 0x00;
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

	if (mpu6050SetGyroFullScaleConfig(mpu6050Device, PLUS_MINUS_500_DEGREE_PER_SECOND) == NOK)
	{
		return NOK;
	}

	if (mpu6050SetAccelFullScaleConfig(mpu6050Device, PLUS_MINUS_2_G) == NOK)
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
	HAL_I2C_Mem_Read(mpu6050Device->i2cHandler, mpu6050Device->address, mpu6050Reg.config, sizeof(mpu6050Reg.config), &config, sizeof(config), timeoutI2C);
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

	HAL_I2C_Mem_Read(mpu6050Device->i2cHandler, mpu6050Device->address, mpu6050Reg.smplrtDiv, sizeof(mpu6050Reg.smplrtDiv), &smplrtDiv, sizeof(smplrtDiv), timeoutI2C);

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

uint8_t mpu6050GetGyroFullScaleConfig(Mpu6050DeviceData *mpu6050Device)
{
	uint8_t fullScaleConfig = 0xAA;
	uint8_t fs_sel = 0;

	HAL_I2C_Mem_Read(mpu6050Device->i2cHandler, mpu6050Device->address, mpu6050Reg.gyroConfig, sizeof(mpu6050Reg.gyroConfig), &fs_sel, sizeof(fs_sel), timeoutI2C);

	fullScaleConfig = (fs_sel & 0x18) >> 3;

	return fullScaleConfig;
}

Status mpu6050SetGyroFullScaleConfig(Mpu6050DeviceData *mpu6050Device, GyroFullScaleRange gyroFullScaleRange)
{
	uint8_t fs_sel = gyroFullScaleRange << 3;

	HAL_I2C_Mem_Write(mpu6050Device->i2cHandler, mpu6050Device->address, mpu6050Reg.gyroConfig, sizeof(mpu6050Reg.gyroConfig), &fs_sel, sizeof(fs_sel), timeoutI2C);

	if (mpu6050GetGyroFullScaleConfig(mpu6050Device) != gyroFullScaleRange)
	{
		return NOK;
	}

	return OK;
}

uint8_t mpu6050GetAccelFullScaleConfig(Mpu6050DeviceData *mpu6050Device)
{
	uint8_t fullScaleConfig = 0xAA;
	uint8_t afs_sel = 0;

	HAL_I2C_Mem_Read(mpu6050Device->i2cHandler, mpu6050Device->address, mpu6050Reg.accelConfig, sizeof(mpu6050Reg.accelConfig), &afs_sel, sizeof(afs_sel), timeoutI2C);

	fullScaleConfig = (afs_sel & 0x18) >> 3;

	return fullScaleConfig;
}

Status mpu6050SetAccelFullScaleConfig(Mpu6050DeviceData *mpu6050Device, AccelFullScaleRange accelFullScaleRange)
{
	uint8_t afs_sel = accelFullScaleRange << 3;

	HAL_I2C_Mem_Write(mpu6050Device->i2cHandler, mpu6050Device->address, mpu6050Reg.accelConfig, sizeof(mpu6050Reg.accelConfig), &afs_sel, sizeof(afs_sel), timeoutI2C);

	if (mpu6050GetAccelFullScaleConfig(mpu6050Device) != accelFullScaleRange)
	{
		return NOK;
	}

	mpu6050SetAccelSensivity(mpu6050Device, accelFullScaleRange);
	return OK;
}

void mpu6050SetAccelSensivity(Mpu6050DeviceData *mpu6050Device, AccelFullScaleRange accelFullScaleRange)
{
	switch (accelFullScaleRange)
	{
		case PLUS_MINUS_2_G:
			mpu6050Device->accelSensitivity = 16384;
			break;

		case PLUS_MINUS_4_G:
			mpu6050Device->accelSensitivity = 8192;
			break;

		case PLUS_MINUS_8_G:
			mpu6050Device->accelSensitivity = 4096;
			break;

		case PLUS_MINUS_16_G:
			mpu6050Device->accelSensitivity = 2048;
			break;

		default:
			break;
	}
}

void mpu6050GetAccel(Mpu6050DeviceData *mpu6050Device)
{
	uint8_t qtyBytes = 6;
	uint8_t accelBytesXYZ[6] = {0, 0, 0, 0, 0, 0};

	HAL_I2C_Mem_Read(mpu6050Device->i2cHandler, mpu6050Device->address, mpu6050Reg.accelOutXYZ, sizeof(mpu6050Reg.accelOutXYZ), accelBytesXYZ, qtyBytes, timeoutI2C);

	int16_t accel_x_axis = ((accelBytesXYZ[0] << 8) | accelBytesXYZ[1]);
	int16_t accel_y_axis = ((accelBytesXYZ[2] << 8) | accelBytesXYZ[3]);
	int16_t accel_z_axis = ((accelBytesXYZ[4] << 8) | accelBytesXYZ[5]);

	mpu6050Device->accel[X_AXIS] = accel_x_axis / ((float) mpu6050Device->accelSensitivity);
	mpu6050Device->accel[Y_AXIS] = accel_y_axis / ((float) mpu6050Device->accelSensitivity);
	mpu6050Device->accel[Z_AXIS] = accel_z_axis / ((float) mpu6050Device->accelSensitivity);
}






