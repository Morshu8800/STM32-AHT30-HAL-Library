/*
 * AHT30.h
 *
 *  Created on: 9 июн. 2026 г.
 *      Author: Morshu8800
 */

#pragma once

#include "main.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdint.h"

#define I2C_ADDRESS 0x38<<1 //for HAL address left shifting by 1

#define CMD_READ_STATUS 0x71

#define MASK_BUSY 0x80
#define MASK_MODE 0x60
#define MASK_CRC_FLAG 0x10
#define MASK_CALIBRATION 0x08
#define MASK_CMP_INTERRUPT 0x04

extern I2C_HandleTypeDef *_aht_ui2c;

void AHT30_Init(I2C_HandleTypeDef *hi2c);
bool AHT30_Force_Measure(void);
uint8_t AHT30_IsBusy(void);
bool AHT30_Get_Data(float *Humidity, float *Temperature);

