/*
 * lcd.h
 *
 *  Created on: Jan 7, 2023
 *      Author: t3
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_
#include "game.h"

void LCD_Init();

void LCD_Display_Page_Game();
void LCD_Display_Page_Menu();
void LCD_Display_Page_Entering_Name();
void LCD_Display_Page_Setting_Level();
void LCD_Display_Page_Game_Menu();
void LCD_Display_Page_About();
void LCD_Display_Page_End();

void LCD_Clear_Page_Game();
void LCD_Clear_Page_Menu();
void LCD_Clear_Page_Entering_Name();
void LCD_Clear_Page_Setting_Level();
void LCD_Clear_Page_Game_Menu();
void LCD_Clear_Page_About();
void LCD_Clear_Page_End();
void LCD_Clear_Player_Name();

void LCD_Update_Game(UpdatedEntity* ues);
void LCD_Update_Game_Boss(UpdatedEntity* ues);
void LCD_Update_Game_With_Enemy_Move(UpdatedEntity* ues);
void LCD_Update_Game_Without_Enemy_Move(UpdatedEntity* ues);
void LCD_Update_Game_Player(int col,int prev_col);
void LCD_Update_Time(char* time_str,char* date_str);
void LCD_Update_Selected_Option_In_Page_Menu(int option,int prev_option);
void LCD_Update_Name(char* name);
void LCD_Update_Char_Name_Pointer(int col);
void LCD_Update_Char_Name(char* character,int col);
void LCD_Update_Games_Info_In_End(char* name,char* score);


#endif /* INC_LCD_H_ */
