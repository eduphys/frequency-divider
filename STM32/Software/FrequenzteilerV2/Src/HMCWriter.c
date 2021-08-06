/*
 * SerialConnection.cpp
 *
 *  Created on: Nov 5, 2019
 *      Author: simon
 * License: Apache License, see https://www.apache.org/licenses/LICENSE-2.0
 */

#include <assert.h>

#include "stm32f0xx_hal.h"

#include "HMCWriter.h"

void writeHMCReg(int reg, int data)
{
	// check data size,  9 bit long
	assert(data < 512);
	// check reg size, 4 bit long
	assert(reg < 16);
	// send data, reg and 001 at the end
	int outputBuffer = (data << 7 ) | (reg << 3) | 0x07 ;

	extern SPI_HandleTypeDef hspi1; //defined in main

	HAL_SPI_Transmit(&hspi1, &outputBuffer, 1,1);
	HAL_Delay(500);

}
