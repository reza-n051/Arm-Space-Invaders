/*
 * keypad.h
 *
 *  Created on: Jan 6, 2023
 *      Author: t3
 */

#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_

#include "stm32f3xx_hal.h"

typedef struct PKResult{
	int is_accepted;
	int is_deleted;
	char character[2];
} PKResult;

void Init_Keypad();
void Reset_PK();
uint8_t Handle_Keypad(uint16_t GPIO_PIN);
PKResult Handle_Phone_Keypad(uint8_t button_number);
#endif /* INC_KEYPAD_H_ */
