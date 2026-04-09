/*
 * sensor_utils.c
 *
 *  Created on: 20 мар. 2026 г.
 *      Author: AVA
 */

#include "sensor_utils.h"
#include "main.h"
#include "Common.h"
#include "w25q_spi.h"
#include "protocol_parser.h"
#include "SPI_Connection.h"
#include "protocol_common.h"
#include <stdbool.h>
#include <stdint.h>

bool need_selfcheck = 0;

/* Выполняет проверку работоспособности датчика
 * Возврат: 1 - датчик работоспособен
 * 			0 - датчик не работоспособен */
bool sensorSelfCheck() {

#ifdef TEST_VERSION
	return true;
#endif

	// устанавливаем блокировку от перезаписи последнего полученного значения АЦП
	need_selfcheck = 1;

	uint8_t ovflw_ADC_level_cnt, low_ADC_level_cnt = 0; // счетчики недопустимых значений АЦП

	// проверка на работоспособность АЦП
	for (uint8_t i = 0; i < 10; i++) { // анализ полученных последних значений АЦП
		if (ADC_data_safe[i] == 4095) {
			ovflw_ADC_level_cnt++;
		}
		if (ADC_data_safe[i] == 0) {
			low_ADC_level_cnt++;
		}
	}

	// проверка счетчиков недопустимых значений
	if (low_ADC_level_cnt == 10 || ovflw_ADC_level_cnt == 10) { // если последние значения АЦП - нули или максимум
		need_selfcheck = 0;
		return false;
	}

	need_selfcheck = 0;
	return true;
}

void sensorChipInit() {

	return;
}

void resetSensorChip() {

	return;
}

void stopSensorChip() {
	return;
}

void enableSensorChip() {
	return;
}

/* Выполняет сброс датчика */
void resetSensor() {

	stopMeasurement();

    // сброс cостояния протокола
    resetFSMProtocol();

    // сброс переменных
#ifndef MULTICHANNEL_VERSION
    page_pos_ptr = 0;
    page_ptr = 0;

#endif

#ifdef MULTICHANNEL_VERSION
    page_ptr = 0;
    write_cycle_closed = 0;
#endif
    // сброс микросхемы датчика
    resetSensorChip();

	// очистка флеш-памяти
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
    W25_Erase_Chip();

    reset_ready = true;

	return;
}
/* Запуск измерения */
void startMeasurement() {
	need_save = 1;
	return;
}

/* Остановка измерения */
void stopMeasurement() {
	need_save = 0;
	return;
}

