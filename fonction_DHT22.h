/*
 * timer.h
 *
 *  Created on: Feb 15, 2021
 *      Author: KACEMI
 */

#include "stm32l1xx_hal.h"

#ifndef INC_HT_H_
#define INC_HT_H_

void Data_Output (GPIO_TypeDef *PORT, uint16_t PIN, int *testO);
void Data_Input (GPIO_TypeDef *PORT, uint16_t PIN, int *testI);
void Read_data (uint8_t *data);

#endif /* INC_HT_H_ */
