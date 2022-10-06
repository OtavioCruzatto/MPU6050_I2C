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
	uint8_t selfTestX;
	uint8_t selfTestY;
	uint8_t selfTestZ;
	uint8_t selfTestA;
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


#endif /* INC_MPU6050_H_ */
