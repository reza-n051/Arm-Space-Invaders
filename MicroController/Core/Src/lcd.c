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

typedef unsigned char byte;

typedef struct Area{
	int col;
	int row;
	int len;
} Area;

byte player_character[] = {
		  0x12,
		  0x0C,
		  0x17,
		  0x17,
		  0x17,
		  0x17,
		  0x0C,
		  0x12
};
byte e1_character[] = {
  0x1D,
  0x1E,
  0x03,
  0x0B,
  0x0B,
  0x03,
  0x1E,
  0x1D
};
byte e3_character[] = {
  0x01,
  0x1D,
  0x02,
  0x09,
  0x09,
  0x02,
  0x1D,
  0x01
};
byte e2_character[] = {
  0x00,
  0x1E,
  0x02,
  0x09,
  0x09,
  0x02,
  0x1E,
  0x00
};
byte fire_character[] = {
  0x00,
  0x00,
  0x00,
  0x1F,
  0x1F,
  0x00,
  0x00,
  0x00
};
byte b1_character[] = {
  0x14,
  0x1A,
  0x15,
  0x12,
  0x13,
  0x11,
  0x15,
  0x11
};
byte b2_character[] = {
  0x11,
  0x15,
  0x11,
  0x13,
  0x12,
  0x15,
  0x1A,
  0x14
};
byte b3_character[] = {
  0x00,
  0x00,
  0x1F,
  0x00,
  0x00,
  0x1F,
  0x00,
  0x00
};

void LCD_Init(){
	createChar(0, e1_character);
	createChar(1, e2_character);
	createChar(2, e3_character);
	createChar(3, player_character);
	createChar(4, fire_character);
	createChar(5, b1_character);
	createChar(6, b2_character);
	createChar(7, b3_character);
}

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
		(Area) {2,1,12},
		(Area) {2,2,12},
		(Area) {2,3,12},
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


Area page_menu_option_selector1 = (Area) {1,0,1};
Area page_menu_option_selector2 = (Area) {1,0,1};
Area page_menu_option_selector3 = (Area) {1,0,1};
Area page_load_option =  (Area) {2,2,10};
Area page_entering_name = (Area) {2,2,8};
Area page_entering_name_pointer = (Area) {2,3,8};
Area page_about_date = (Area) {2,2,8};
Area page_about_time = (Area) {2,3,8};
Area page_end_name = (Area)  {10,1,10};
Area page_end_score = (Area) {10,2,10};



//player1 => p
//enemy1  => Q
//enemy2  => W
//enemy3  => E
//fire1   => F
//fire2   => D
/*
 * page 0 : menu
 *
 * page 1 : game
 * page 2 : enter name
 * page 3 : enter game level
 * page 5 : about
 * page 7 : end
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
	for (int i=col;i<col + len;i++) {
		setCursor(i, row);
		print(" ");
	}
}

void lcd_clear_static_squares(Area* areas,int arr_len){
	for(int i=0;i<arr_len;i++){
		Area a = *(areas + i);
		for(int j=a.col;j<a.col+a.len;j++){
			setCursor(j, a.row);
			print(" ");
		}
	}
}

void LCD_Display_Page_Game(){
	setCursor(0, 0);
	write(3);
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
}
void LCD_Display_Page_Entering_Name(){
	setCursor(2, 0);
	print("Enter Your Name");
	setCursor(2, 1);
	print("Max: 8 characters");
}
void LCD_Display_Page_Setting_Level(){
	setCursor(2,0);
	print("Select Game Level");
	setCursor(4,1);
	print("Easy   : 4");
	setCursor(4,2);
	print("Normal : 5");
	setCursor(4,3);
	print("Hard   : 6");
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
	lcd_clear_static_squares(page_menu_static_squares,3);
	lcd_clear_area(page_menu_option_selector1);
	lcd_clear_area(page_menu_option_selector2);
	lcd_clear_area(page_menu_option_selector3);
}
void LCD_Clear_Page_Entering_Name(){
	clear();
}
void LCD_Clear_Page_Setting_Level(){
	clear();
//	lcd_clear_static_squares(page_setting_level_squares,4);
}
void LCD_Clear_Page_About(){
	lcd_clear_static_squares(page_about_static_squares,4);
	lcd_clear_area(page_about_date);
	lcd_clear_area(page_about_time);
}
void LCD_Clear_Page_End(){
	clear();
}
void LCD_Clear_Page_Load(){
	lcd_clear_static_squares(page_load_static_squares,2);

}
void LCD_Update_Game_Boss(UpdatedEntity* ues){
	int ue_id = -1;
	if(ues[0].id < 0){
		return;
	}

	int index = 0;
	do{
		ue_id = ues[index].id;
		int col = get_lcd_col_from_game_col(ues[index].col);
		int row = get_lcd_row_from_game_row(ues[index].row);
		int type = ues[index].type;
		int entity_type = ues[index].entity_type;
		int action_type = ues[index].action_type;
		if(action_type == INSERT){
			if(type == EBoss){
				setCursor(row, col);
				if(entity_type == 1){
					write(5);
				}else if(entity_type == 2){
					write(6);
				}else if(entity_type == 3){
					write(7);
				}else if(entity_type == 4){
					write(7);
				}

			}else if(type == EFire || type == EPlayerFire){
				setCursor(row, col);
				write(4);
			}
		}else if(action_type == DELETE){
			setCursor(row, col); //map of game is vertical but map of LCD is corizontal.
			print(" ");
		}else if(action_type == UPDATE){
			if(type == EBoss){
				setCursor(row, col);
				if(entity_type == 1){
					write(5);
					setCursor(row+1,col);
					print(" ");
					if(col !=0){
						setCursor(row,col-1);
						print(" ");
					}
				}else if(entity_type == 2){
					write(6);
					setCursor(row+1,col);
					print(" ");
					if(col !=3){
						setCursor(row,col+1);
						print(" ");
					}
				}else if(entity_type == 3){
					write(7);
					setCursor(row-1,col);
					print(" ");
					if(col !=0){
						setCursor(row,col-1);
						print(" ");
					}
				}else if(entity_type == 4){
					write(7);
					setCursor(row-1,col);
					print(" ");
					if(col !=3){
						setCursor(row,col+1);
						print(" ");
					}
				}

			}else if(type == EFire){
				setCursor(row, col); //map of game is vertical but map of LCD is corizontal.
				write(4);
				setCursor(row+1,col);
				print(" ");
			}else if(type == EPlayerFire){
				setCursor(row, col); //map of game is vertical but map of LCD is corizontal.
				write(4);
				setCursor(row-1,col);
				print(" ");

			}
		}
		index++;
	}while(ue_id != -1 && index < Max_Entity_In_Game);

}
void LCD_Update_Game_With_Enemy_Move(UpdatedEntity* ues){
	if(ues[0].id < 0){
		return;
	}

	int col_0_entities[20]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
	int col_1_entities[20]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
	int col_2_entities[20]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
	int col_3_entities[20]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
	int ue_id = -1;
	int index = 0;
	do{
		ue_id = ues[index].id;
		int col = get_lcd_col_from_game_col(ues[index].col);
		int row = get_lcd_row_from_game_row(ues[index].row);
		int type = ues[index].type;
		int entity_type = ues[index].entity_type;
		int action_type = ues[index].action_type;
		if(action_type == INSERT){
			if(type == EEnemy){
				if(col == 0){
					col_0_entities[row] = entity_type;
				}else if(col == 1){
					col_1_entities[row] = entity_type;
				}else if(col == 2){
					col_2_entities[row] = entity_type;
				}else if(col == 3){
					col_3_entities[row] = entity_type;
				}
			}else if(type == EFire || type == EPlayerFire){
				setCursor(row, col);
				write(4);
			}
		}else if(action_type == DELETE){
			setCursor(row, col); //map of game is vertical but map of LCD is corizontal.
			print(" ");
		}else if(action_type == UPDATE){
			if(type== EEnemy){
				if(col == 0){
					col_0_entities[row] = entity_type;
				}else if(col == 1){
					col_1_entities[row] = entity_type;
				}else if(col == 2){
					col_2_entities[row] = entity_type;
				}else if(col == 3){
					col_3_entities[row] = entity_type;
				}

			}else if(type == EFire){
				setCursor(row, col); //map of game is vertical but map of LCD is corizontal.
				write(4);
				setCursor(row+1,col);
				print(" ");
			}else if(type == EPlayerFire){
				setCursor(row, col); //map of game is vertical but map of LCD is corizontal.
				write(4);
				setCursor(row-1,col);
				print(" ");

			}
		}
		index++;
	}while(ue_id != -1 && index < Max_Entity_In_Game);
	int min_rows[4] = {0,0,0,0};
	int is_finded[4] = {0,0,0,0};
	for(int i=0;i<20;i++){
		if(col_0_entities[i] != -1 && is_finded[0] == 0){
			min_rows[0] = i;
			is_finded[0] = 1;
		}
		if(col_1_entities[i] != -1 && is_finded[1] == 0){
			min_rows[1] = i;
			is_finded[1] = 1;
		}
		if(col_2_entities[i] != -1 && is_finded[2] == 0){
			min_rows[2] = i;
			is_finded[2] = 1;
		}
		if(col_3_entities[i] != -1 && is_finded[3] == 0){
			min_rows[3] = i;
			is_finded[3] = 1;
		}
	}
	setCursor(min_rows[0], 0);
	for(int i=0;i<20;i++){
		if(col_0_entities[i] != -1){
			write(col_0_entities[i]);
		}
	}
	setCursor(min_rows[1], 1);
	for(int i=0;i<20;i++){
		if(col_1_entities[i] != -1){
			write(col_1_entities[i]);
		}

	}
	setCursor(min_rows[2], 2);
	for(int i=0;i<20;i++){
		if(col_2_entities[i] != -1){
			write(col_2_entities[i]);
		}
	}
	setCursor(min_rows[3], 3);
	for(int i=0;i<20;i++){
		if(col_3_entities[i] != -1){
			write(col_3_entities[i]);
		}
	}

}


void LCD_Update_Game_Without_Enemy_Move(UpdatedEntity* ues){
	int ue_id = -1;
	int index = 0;
	if(ues[0].id < 0){
		return;
	}
	do{
		int action_type = ues[index].action_type;
		int col = get_lcd_col_from_game_col(ues[index].col);
		int row = get_lcd_row_from_game_row(ues[index].row);
		int type = ues[index].type;
		int entity_type = ues[index].entity_type;
		ue_id = ues[index].id;
		setCursor(row, col); //map of game is vertical but map of LCD is corizontal.
		if(action_type == INSERT){

			if(type == EEnemy){
				if(entity_type == 1){
					write(1);
				}else if(entity_type == 2){
					write(2);
				}else if(entity_type == 0){
					write(0);
				}
			}else if(type == EFire || type == EPlayerFire){
				write(4);
			}
		}else if(action_type == DELETE){
			print(" ");
		}else if(action_type == UPDATE){
			if(type == EEnemy){
				write(entity_type);
				setCursor(row+1,col);
				print(" ");
			}else if(type == EFire){
				write(4);
				setCursor(row+1,col);
				print(" ");

			}else if(type == EPlayerFire){
				write(4);
				setCursor(row-1,col);
				print(" ");
			}
		}
		index++;
	}while(ue_id != -1 && index < Max_Entity_In_Game);
}
void LCD_Update_Time(char* time_str,char* date_str){
	setCursor(2, 2);
	print(date_str);
	setCursor(2, 3);
	print(time_str);
}
void LCD_Update_Selected_Option_In_Page_Menu(int option,int prev_option){
	int prev_col = 1;
	int prev_row = prev_option-1;
	setCursor(prev_col, prev_row);
	print(" ");
	int cur_col = 1;
	int cur_row = option-1;
	setCursor(cur_col, cur_row);
	print(">");
}
void LCD_Update_Name(char* name){
	setCursor(2, 2);
	print(name);
}

void LCD_Update_Char_Name(char* character,int col){
	setCursor(col, 2);
	print(character);
	setCursor(col, 3);
	print("^");
}
void LCD_Update_Char_Name_Pointer(int col){
	setCursor(col, 3);
	print("^");
}



void LCD_Update_Games_Info_In_End(char* name,char* score){
	setCursor(10, 1);
	print(name);
	setCursor(10, 2);
	print(score);
}

void LCD_Update_Game_Player(int col,int prev_col){
	setCursor(0,prev_col);
	print(" ");
	setCursor(0, col);
	write(3);
}

void LCD_Clear_Player_Name(){
	lcd_clear_area(page_entering_name);
	lcd_clear_area(page_entering_name_pointer);
}

