/*
 * keypad.c
 *
 *  Created on: Jan 6, 2023
 *      Author: t3
 */


#include "keypad.h"
// Input pull down rising edge trigger interrupt pins:
// Row1 PD0, Row2 PD2, Row3 PD4, Row4 PD6
GPIO_TypeDef *const Row_ports[] = {GPIOD, GPIOD, GPIOD, GPIOD};
const uint16_t Row_pins[] = {GPIO_PIN_0, GPIO_PIN_2, GPIO_PIN_4, GPIO_PIN_6};
// Output pins: Column1 PD1, Column2 PD3, Column3 PD5, Column4 PD7
GPIO_TypeDef *const Column_ports[] = {GPIOD, GPIOD, GPIOD, GPIOD};
const uint16_t Column_pins[] = {GPIO_PIN_1, GPIO_PIN_3, GPIO_PIN_5, GPIO_PIN_7};

void Init_Keypad(){
	for (int i=0;i<4;i++){
		HAL_GPIO_WritePin(Column_ports[i], Column_pins[i], 1);
	}
}

uint8_t Handle_Keypad(uint16_t GPIO_Pin){
	  int8_t row_number = -1;
	  int8_t column_number = -1;

//	  if (GPIO_Pin == GPIO_PIN_0)
//	  {
//	    // blue_button_pressed = 1;
////	     return 0;
//	  }

	  for (uint8_t row = 0; row < 4; row++) // Loop through Rows
	  {
	    if (GPIO_Pin == Row_pins[row])
	    {
	      row_number = row;
	    }
	  }
	  HAL_GPIO_WritePin(Column_ports[0], Column_pins[0], 0);
	  HAL_GPIO_WritePin(Column_ports[1], Column_pins[1], 0);
	  HAL_GPIO_WritePin(Column_ports[2], Column_pins[2], 0);
	  HAL_GPIO_WritePin(Column_ports[3], Column_pins[3], 0);

	  for (uint8_t col = 0; col < 4; col++) // Loop through Columns
	  {
	    HAL_GPIO_WritePin(Column_ports[col], Column_pins[col], 1);
	    GPIO_PinState x = HAL_GPIO_ReadPin(Row_ports[row_number], Row_pins[row_number]);
	    if (x)
	    {
	      column_number = col;
	    }
	    HAL_GPIO_WritePin(Column_ports[col], Column_pins[col], 0);
	  }

	  HAL_GPIO_WritePin(Column_ports[0], Column_pins[0], 1);
	  HAL_GPIO_WritePin(Column_ports[1], Column_pins[1], 1);
	  HAL_GPIO_WritePin(Column_ports[2], Column_pins[2], 1);
	  HAL_GPIO_WritePin(Column_ports[3], Column_pins[3], 1);

	  if (row_number == -1 || column_number == -1)
	  {
	    return 1; // Reject invalid scan
	  }
	  //   C0   C1   C2   C3
	  // +----+----+----+----+
	  // | 1  | 2  | 3  | 4  |  R0
	  // +----+----+----+----+
	  // | 5  | 6  | 7  | 8  |  R1
	  // +----+----+----+----+
	  // | 9  | 10 | 11 | 12 |  R2
	  // +----+----+----+----+
	  // | 13 | 14 | 15 | 16 |  R3
	  // +----+----+----+----+
	  const uint8_t button_number = row_number * 4 + column_number + 1;
	  return button_number;
}
