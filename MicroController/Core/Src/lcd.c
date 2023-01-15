/*
 * lcd.c
 *
 *  Created on: Jan 7, 2023
 *      Author: t3
 */

#include "lcd.h"
#include "LiquidCrystal.h"
#include "string.h"
//page static squares
#define MENU_PAGE_OPTIONS_COUNT 3
#define PAGE_MENU_STATIC_SQUARES_COUNT 13
#define PAGE_ABOUT_STATIC_SQUARES_COUNT 41
#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof(arr[0]))


typedef struct Area{
	int col;
	int row;
	int len;
} Area;

Area page_menu_static_squares[3] = {
		(Area) {2,0,4},
		(Area) {2,1,5},
		(Area) {2,2,4},
};
Area page_about_static_squares[4] = {
		(Area) {2,0,16},
		(Area) {2,1,11},
		(Area) {2,2,8},
		(Area) {2,2,8},
};
Area page_entering_name_static_squares[2] = {
		(Area) {2,0,15},
		(Area) {2,1,18},
};
Area page_setting_level_squares[4] = {
		(Area) {2,0,16},
		(Area) {2,1,9},
		(Area) {2,2,9},
		(Area) {2,3,9},
};
Area page_game_menu_static_squares[2] = {
		(Area) {2,0,4},//play
		(Area) {2,1,4},//save
};
Area page_end_static_squares[2] = {
		(Area) {2,0,8},
		(Area) {2,1,8},
};
Area page_load_static_squares[2] = {
		(Area) {0,0,20},
		(Area) {0,1,15},
};


Area page_menu_option_selector = (Area) {1,0,1};
Area page_game_menu_option_selector = (Area) {1,0,1};
Area page_load_option =  (Area) {2,2,10};
Area page_entering_name = (Area) {2,2,8};
Area page_about_date = (Area) {2,2,8};
Area page_about_time = (Area) {2,3,8};
Area page_end_name = (Area)  {10,1,8};
Area page_end_score = (Area) {10,2,3};



//player1 => p
//enemy1  => q
//enemy2  => w
//enemy3  => e
//fire1   => f
//fire2   => d
/*
 * page 0 : menu
 *
 * page 1 : game
 * page 2 : enter name
 * page 3 : enter game level
 * page 4 : saved games
 * page 5 : about
 * page 6 : scores
 * page 7 : end
 * page 8 : game_menu (when player stop game)
 *
 */


void clear_cell(int col,int row){
	setCursor(col,row);
	print(" ");
}

int get_lcd_col_from_game_col(int col){
	return col;
}
int get_lcd_row_from_game_row(int row){
	return row;
}

void lcd_clear_area(Area a){
	int row = a.row;
	int col = a.col;
	int len = a.len;
	for (int i=col;i<len;i++) {
		setCursor(i, row);
		print(" ");
	}
}

void lcd_clear_static_squares(Area* areas){
	int static_length = ARRAY_LENGTH(areas);
	for(int i=0;i<static_length;i++){
		Area a = *(areas + i);
		for(int j=a.col;j<a.len;j++){
			setCursor(j, i);
			print(" ");
		}
	}
}

void LCD_Display_Page_Game(){
	setCursor(0, 0);
	print("P");
}

void LCD_Display_Page_Load(){
	setCursor(0,0);
	print("To Continue The Last");
	setCursor(0, 1);
	print("Game Press A");
}


void LCD_Display_Page_Menu(){
	setCursor(2, 0);
	print("Game");
	setCursor(2, 1);
	print("About");
	setCursor(2, 2);
	print("Load");
}
void LCD_Display_Page_Entering_Name(){
	setCursor(2, 0);
	print("Enter Your Name");
	setCursor(2, 1);
	print("Maximum 8 characters");
}
void LCD_Display_Page_Setting_Level(){
	setCursor(2,0);
	print("Select Game Level");
	setCursor(4,1);
	print("Easy   : 1");
	setCursor(4,2);
	print("Normal : 2");
	setCursor(4,3);
	print("Hard   : 3");
}
void LCD_Display_Page_Game_Menu(){
	setCursor(2, 0);
	print("Play");
	setCursor(2, 1);
	print("Save");

}
void LCD_Display_Page_About(){
	setCursor(2, 0);
	print("Ahmadreza Hafizi");
	setCursor(2, 1);
	print("Reza Nasiri");
}
void LCD_Display_Page_End(){
	setCursor(5,0);
	print("END");
	setCursor(2, 1);
	print("Name  : ");
	setCursor(2, 2);
	print("Score : ");
}

void LCD_Clear_Page_Game(){
	clear();
}
void LCD_Clear_Page_Menu(){
	lcd_clear_static_squares(page_menu_static_squares);
	lcd_clear_area(page_menu_option_selector);
}
void LCD_Clear_Page_Entering_Name(){
	lcd_clear_static_squares(page_entering_name_static_squares);
	lcd_clear_area(page_entering_name);
}
void LCD_Clear_Page_Setting_Level(){
	lcd_clear_static_squares(page_setting_level_squares);
}
void LCD_Clear_Page_Game_Menu(){
	lcd_clear_static_squares(page_game_menu_static_squares);
	lcd_clear_area(page_game_menu_option_selector);

}
void LCD_Clear_Page_About(){
	lcd_clear_static_squares(page_about_static_squares);
	lcd_clear_area(page_about_date);
	lcd_clear_area(page_about_time);
}
void LCD_Clear_Page_End(){
	lcd_clear_static_squares(page_end_static_squares);
	lcd_clear_area(page_end_score);
	lcd_clear_area(page_end_name);
}
void LCD_Clear_Page_Load(){
	lcd_clear_static_squares(page_load_static_squares);

}

void LCD_Update_Game(UpdatedEntity* ues){
	int ue_id = -1;
	int index = 0;
	do{
		int action_type = ues[index].action_type;
		int col = get_lcd_col_from_game_col(ues[index].col);
		int row = get_lcd_row_from_game_row(ues[index].row);
		int type = ues[index].type;
		int entity_type = ues[index].entity_type;
		ue_id = ues[index].id;
		setCursor(col, row);
		if(action_type == INSERT){
			if(type == EEnemy){
				if(entity_type == 1){
					print('q');
				}else if(entity_type == 2){
					print('w');
				}else if(entity_type == 3){
					print('e');
				}
			}else if(type == EFire || type == EPlayerFire){
				if(entity_type == 1){
					print('f');
				}else if(entity_type == 2){
					print('d');
				}
			}
		}else if(action_type == DELETE){
			print(' ');
		}else if(action_type == UPDATE){
			if(type == EEnemy){
				if(entity_type == 1){
					print('q');
				}else if(entity_type == 2){
					print('w');
				}else if(entity_type == 3){
					print('e');
				}
				setCursor(col,row+1);
				print(' ');
			}else if(type == EFire){
				if(entity_type == 1){
					print('f');
				}else if(entity_type == 2){
					print('d');
				}
				setCursor(col,row+1);
				print(' ');

			}else if(type == EPlayerFire){
				if(entity_type == 1){
					print('f');
				}else if(entity_type == 2){
					print('d');
				}
				setCursor(col,row-1);
				print(' ');

			}
		}
	}while(ue_id != -1 && index < Max_Entity_In_Game);
}
void LCD_Update_Time(char* time_str,char* date_str){
	setCursor(2, 2);
	print(date_str);
	setCursor(2, 3);
	print(time_str);
}
void LCD_Update_Selected_Option_In_Page_Menu(int option){
	int cur_col = 1;
	int cur_row = option;
	setCursor(cur_col, cur_row);
	print(">");
}
void LCD_Update_Selected_Option_In_Page_Game_Menu(int option){
	int cur_col = 1;
	int cur_row = option;
	setCursor(cur_col, cur_row);
	print(">");
}
void LCD_Update_Selected_Option_In_Page_Saved_Games(int option){
	int cur_col = 1;
	int cur_row = option;
	setCursor(cur_col, cur_row);
	print(">");
}
void LCD_Update_Name(char* name){
	setCursor(2, 3);
	print(name);
}

void LCD_Update_Games_Info_In_End(char* name,char* score){
	setCursor(10, 1);
	print(name);
	setCursor(10, 2);
	print(score);
}

void LCD_Update_Game_Player(int col,int prev_col){
	setCursor(prev_col, 0);
	print(" ");
	setCursor(col, row);
	print("P")
}


























