/*
 * lcd.h
 *
 *  Created on: Jan 7, 2023
 *      Author: t3
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

typedef struct Position{
	int row;
	int col;
} Position;

void Init_LCD();
void Set_Page(int page);
int  Get_Page();



#endif /* INC_LCD_H_ */
