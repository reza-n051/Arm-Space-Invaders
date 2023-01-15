/*
 * lcd.h
 *
 *  Created on: Jan 7, 2023
 *      Author: t3
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_
#include "game.h"


//void Command_Menu_Next_Option();
//void Command_Menu_Prev_Option();
//void Command_Menu_Select_Option_In_Main_Menu();
//void Command_ReDispaly_Date_And_Time();
//void Command_Display_Page_Menu();
//void Set_LCD_Time(char* time_src,char* date_src);
//int Get_Current_Page();

void LCD_Display_Page_Game();
void LCD_Display_Page_Menu();
void LCD_Display_Page_Entering_Name();
void LCD_Display_Page_Setting_Level();
void LCD_Display_Page_Game_Menu();
void LCD_Display_Page_About();
void LCD_Display_Page_Load();

void LCD_Clear_Page_Game();
void LCD_Clear_Page_Menu();
void LCD_Clear_Page_Entering_Name();
void LCD_Clear_Page_Setting_Level();
void LCD_Clear_Page_Game_Menu();
void LCD_Clear_Page_About();
void LCD_Clear_Page_End();
void LCD_Clear_Page_Load();

void LCD_Update_Game(UpdatedEntity* ues);
void LCD_Update_Game_Player(int col,int prev_col);
void LCD_Update_Time(char* time_str,char* date_str);
void LCD_Update_Selected_Option_In_Page_Menu(int option);
void LCD_Update_Selected_Option_In_Page_Game_Menu(int option);
void LCD_Update_Selected_Option_In_Page_Saved_Games(int option);
void LCD_Update_Name(char* name);
void LCD_Update_Games_Info_In_End(char* name,char* score);


#endif /* INC_LCD_H_ */
