/*
 * ggame.c
 *
 *  Created on: Jan 15, 2023
 *      Author: t3
 */

#define GAME_MAP_SIZE 80
#define GAME_MAP_ROW 20
#define GAME_MAP_COL 4
#define GAME_MAP_JSON_SIZE 2000
#define GAME_MAP_CELL_JSON_SIZE 20

Entity game_map[GAME_MAP_ROW][GAME_MAP_COL];
int game_level;
int game_handler_timer;



int str_to_int(char* string){

}
void SI_Init_Game_Map(){
	for(int i=0;i<GAME_MAP_ROW;i++){
		for(int j=0;j<GAME_MAP_COL;j++){
			game_map[i][j] = (Entity) {1,-1,-1,-1};
		}
	}
}
void SI_Load_Game(LoaderResult lr){
	char json_str[GAME_MAP_CELL_JSON_SIZE];
	strcpy(json_str,lr.arr);
	char copy_string[strlen(lr.arr)];
	strcpy(copy_string, lr.arr);
	char* json_section[GAME_MAP_SIZE];
	int i=0;

	//split with ",\n"
	char* res = strtok(copy_string,",\n");
	while(res != NULL && i < GAME_MAP_SIZE){
		json_section[i] = res;
		i += 1;
		res = strtok(NULL, ",\n");
	}
	for(int j=0;j<GAME_MAP_SIZE;j++){
		char* cell_string_sections[6];
		//split with ":"
		int i=0;
		char* res0 = strtok(copy_string,":");
		while(res0 != NULL && i < 6){
			cell_string_sections[i] = res0;
			i += 1;
			res0 = strtok(NULL, ":");
		}
		int row = str_to_int(cell_string_sections[0]);
		int col = str_to_int(cell_string_sections[1]);
		int health = str_to_int(cell_string_sections[2]);
		int power = str_to_int(cell_string_sections[3]);
		int entity_type = str_to_int(cell_string_sections[4]);
		int entity_sub_type = str_to_int(cell_string_sections[5]);
		game_map[row][col] = (Entity){health,power,entity_type,entity_sub_type};
	}
}


//row:col:health:power:entity_type:entity_sub_type

LoaderResult SI_Save_Game(){
	LoaderResult json_loader = {""};
	for(int i=0;i<GAME_MAP_ROW;i++){
		for (int j=0;j< GAME_MAP_COL ;j++){
			char cell_res[130];
			//row:col:health:power:entity_type:entity_sub_type
			sprintf(cell_res,"%d:%d:%d:%d:%d:%d"
					,i , j ,game_map[i][j].col,game_map[i][j].row
					,game_map[i][j].health,game_map[i][j].power,game_map[i][j].entity_type,game_map[i][j].entity_sub_type
			);
			strcat(json_loader.arr,cell_res);
			if(!(i == GAME_MAP_ROW - 1 && j == GAME_MAP_COL - 1)){
				strcat(json_loader.arr,",\n");
			}
		}
	}
	return json_loader;

}

void SI_Init_Game(int gl,int handle_timer){
	game_level = gl;
	game_handler_timer = handle_timer;

}



























