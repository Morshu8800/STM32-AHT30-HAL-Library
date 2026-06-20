/*
 * AHT30.c
 *
 *  Created on: 9 июн. 2026 г.
 *      Author: Morshu8800
 */

#include "AHT30.h"

I2C_HandleTypeDef *_aht_ui2c;
const uint8_t CMD_MEASURE_SEQ[3] = { 0xAC, 0x33, 0x00 }; // Measurement command from page 13

void AHT30_Init(I2C_HandleTypeDef *hi2c) {
	_aht_ui2c = hi2c;
	HAL_Delay(6); // Wait until sensor init as said in datasheet on page 12
	//If this is not necessary, comment out this line above.
}

bool AHT30_Force_Measure(void) {
	if (HAL_I2C_Master_Transmit(_aht_ui2c, CMD_FORCE_MEASURE, (uint8_t*) CMD_MEASURE_SEQ, 3,
			1000) != HAL_OK) {
		return false;// bus error
	}
	HAL_Delay(80); // Wait until sensor measure data as said in datasheet on page 13
	//If this is not necessary, comment out this line above.
	return true; // it's ok!
}

uint8_t AHT30_IsBusy(void) {
	uint8_t buf;
	if (HAL_I2C_Master_Receive(_aht_ui2c, CMD_READ_STATUS, &buf, 1,
			1000) != HAL_OK) {
		return 255; //bus error
	}
	if ((buf & MASK_BUSY)) {
		return 1; //busy
	} else {
		return 0; //not busy
	}
}

bool AHT30_Get_Data(float *Humidity, float *Temperature) {
	uint8_t buf[7] = { 0 };
	uint8_t tempTemp[3] = { 0 };
	uint8_t humiTemp[3] = { 0 };
	uint32_t raw_humidity, raw_temperature;
	if (HAL_I2C_Master_Receive(_aht_ui2c, CMD_READ_STATUS, buf, sizeof(buf),
			1000) != HAL_OK) {
		return 1; //bus error
	}

	humiTemp[0] = buf[1];
	humiTemp[1] = buf[2];
	humiTemp[2] = buf[3] & 0xF0;  //cutting and shifting the bytes
	for (uint8_t i = 0; i < 3; i++) {
		raw_humidity += humiTemp[i];
		raw_humidity <<= 8;
	}
	raw_humidity >>= 12;
	*Humidity = (raw_humidity * 0.00000095367431640625f) * 100.0f;
// I use multiplication because it is faster than division, especially on the Cortex M0+
// Я использую умножение, т.к. оно быстрее деления, особенно на Cortex M0+

	tempTemp[0] = (buf[3] & 0x0F) << 4;  //cutting and shifting the bytes
	tempTemp[0] += (buf[4] & 0xF0) >> 4; //делим делим байтики
	tempTemp[1] = (buf[4] & 0x0F) << 4;
	tempTemp[1] += (buf[5] & 0xF0) >> 4;
	tempTemp[2] = (buf[5] & 0x0F) << 4;
	tempTemp[2] += (buf[6] & 0xF0) >> 4;
	for (uint8_t i = 0; i < 3; i++) {
		raw_temperature += tempTemp[i];
		raw_temperature <<= 8;
	}
	raw_temperature >>= 12;
	*Temperature = ((raw_temperature * 0.00000095367431640625f) * 200.0f) - 50.0f;
// I use multiplication because it is faster than division, especially on the Cortex M0+
// Я использую умножение, т.к. оно быстрее деления, особенно на Cortex M0+
	return 0; // it's ok!
}

