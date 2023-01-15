/*
 * game.c
 *
 *  Created on: Jan 7, 2023
 *      Author: t3
 */

#include "game.h"
#include "stdlib.h"

#define MAX_ROWS_IN_GAME 20
#define MAX_COLS_IN_GAME 20

int player_id=1;
int enemy_id =1;
int fire_id  =1;

Player player1;
//TODO game map for better implementation
//char game_map[MAX_ROWS_IN_GAME][MAX_COLS_IN_GAME];

int dead_enemies_count = 0;

UpdatedEntity ues[Max_Entity_In_Game];
int updated_counter = 0;

Enemy enemies[Max_Enemy_In_Game];
int enemy_count = 0;
Fire player1_fires[Max_Fire_In_Game];
Fire enemy_fires[Max_Fire_In_Game];

char game_level = 'E';

int rows_length;
int cols_length;

int enemy_fires_count = 0;
int handle_timer = 50;
int handle_method_counter = 0;

int is_game_end = 0;

/*
 * 1.movement of enemies and fires
 * 2.check is new enemy should created
 * 3.check should enemy shoot
 * 4.check is enemy or player shooted
 * 5.check is game ended or not
 */


int is_game_ended(){
/*
 * Game Ended When:
 * 1.player health become 0.
 * 2.enemies except boss reach to player
 * 3.player kills some enemies
 * (in normal level => 25, in easy level => 15 , in hard level => 35)
 *
 */
	if(player1.health == 0){
		return 1;
	}
	for (int i=0 ; i<enemy_count;i++){
		if(enemies[i].row == 1){
			return 1;
		}
	}
	if(
			(dead_enemies_count == 15 && game_level== 'E') ||
			(dead_enemies_count == 25 && game_level== 'N') ||
			(dead_enemies_count == 35 && game_level== 'H')
	){
		return 1;
	}
	return 0;

}



Fire is_enemy_shoot_player(){
	int player_col = player1.col;
	int player_row = player1.row;
	for(int i=0;i< Max_Fire_In_Game ;i++){
		if(enemy_fires[i].col == player_col && enemy_fires[i].row == player_row){
			return enemy_fires[i];
		}
	}
	return 0;
}

Fire is_player_shoot_enemy(){
	for(int i=0;i<enemy_count;i++){
		for(int j = 0 ; j < Max_Fire_In_Game ; j++){
			if(enemies[i].row == player1_fires[j].row && enemies[i].col == player1_fires[j].col){
				return player1_fires[j];
			}
		}
	}

	return (Fire) {-1,-1,-1,1};
}

Enemy get_enemy_with_col_and_row(int col,int row){
	Enemy e = (Enemy){-1,-1,-1,-1,-1,-1};
	for(int i=0;i<enemy_count;i++){
		if(enemies[i].col == col && enemies[i].row == row){
			e.id  = enemies[i].id ;
			e.col = enemies[i].col;
			e.row = enemies[i].row;
			e.fire_power = enemies[i].fire_power;
			e.enemy_type = enemies[i].enemy_type;
			e.health = enemies[i].health;
			return e;
		}
	}
	return (Fire) {-1,-1,-1,1};
}

void remove_dead_enemies(){
	for(int i=0;i<enemy_count;i++){
		if(enemies[i].health == 0){
			enemies[i].id = enemies[enemy_count - 1].id;
			enemies[i].col = enemies[enemy_count - 1].col;
			enemies[i].row = enemies[enemy_count - 1].row;
			enemies[i].enemy_type = enemies[enemy_count - 1].enemy_type;
			enemies[i].fire_power = enemies[enemy_count - 1].fire_power;
			enemies[i].health = enemies[enemy_count - 1].health;
			dead_enemies_count++;
		}
	}
}

Enemy select_enemy_for_fire(){
	int enemy_find = 0;
	Enemy result_enemy = (Enemy) {-1,-1,-1,0,0,0};
	int random_idx = rand() % enemy_count;
	result_enemy.col = enemies[random_idx].col;
	result_enemy.row = enemies[random_idx].row;
	result_enemy.id = enemies[random_idx].id;
	result_enemy.health = enemies[random_idx].health;
	result_enemy.fire_power = enemies[random_idx].fire_power;
	return result_enemy;

}

int enemy_should_move_down(){
	if(
			((handle_method_counter * handle_timer ) % 5000 == 0 && game_level='E') ||
			((handle_method_counter * handle_timer ) % 3000 == 0 && game_level='N') ||
			((handle_method_counter * handle_timer ) % 1000 == 0 && game_level='H')
	){
		return 0;
	}
	return 1;

}


Fire create_fire_for_player(int row,int col,int power){
	int fire_id = -1;
	Fire f = {
		row,col,-1,power
	};
	int index = -1;
	for(int i=0 ; i< Max_Fire_In_Game ; i++){
		if(player1_fires[i].id != -1){
			index = i;
			break;
		}
	}
	f.id = index;
	return f;
}

void delete_fire_from_player_fires(int id){
	for(int i=0;i< Max_Fire_In_Game ;i++){
		if(player1_fires[i].id == id){
			player1_fires[i].id = -1;
		}
	}
}

int is_enemy_should_creat(){
	if(
			((handle_method_counter * handle_timer ) % 5000 == 0 && game_level='E') ||
			((handle_method_counter * handle_timer ) % 3000 == 0 && game_level='N') ||
			((handle_method_counter * handle_timer ) % 1000 == 0 && game_level='H')
	){
		return 0;
	}
	return 1;
}

Enemy create_enemy(int enemy_type){
	for(int i=0;i<cols_length;i++){
		if(enemy_count <= (rows_length * cols_length) - 8 ){
			enemies[enemy_count].id = player_id;
			enemies[enemy_count].col = i;
			enemies[enemy_count].row = 19;
			int enemy_type = rand() % 3;
			enemies[enemy_count].enemy_type = enemy_type;
			if(enemy_type == 0){
				enemies[enemy_count].fire_power = 1;
				enemies[enemy_count].health = 1;
			}else if(enemy_type == 1){
				enemies[enemy_count].fire_power = 1;
				enemies[enemy_count].health = 2;
			}else{
				enemies[enemy_count].fire_power = 2;
				enemies[enemy_count].health = 1;
			}
			enemy_count++;
		}
	}
	return enemies[enemy_count-1];

}

int is_enemy_should_fire(){
	if((handle_method_counter * handle_timer ) % 1000 == 0){
		return 0;
	}
	int enemy_fire_count = 0;
	for (int i=0 ; i<Max_Fire_In_Game ; i++){
		if(enemy_fires[i].id != -1){
			enemy_fire_count++;
		}
	}
	if(enemy_fire_count>Max_Fire_In_Game-1){
		return 0;
	}
	return 1;
}

void clear_updated_entities(){

	for(int i=0;i<Max_Entity_In_Game;i++){
		ues[i].id = -1;
		ues[i].action_type = -1;
		ues[i].col = -1;
		ues[i].row = -1;
		ues[i].type = -1;
		ues[i].entity_type = -1;
	}
	updated_counter = 0;
}

void add_entity_to_updated_entities(int id,int row,int col,int type,int entity_type,int action_type){
	ues[updated_counter] = (UpdatedEntity) {row,col,id,type,entity_type,action_type};
	updated_counter++;
}


UpdatedEntity* Get_Updated_Entities(){
	return ues;
}

void Handle_Game(){
	//50 ms =>
	clear_updated_entities();

	for (int i=0 ; i< Max_Fire_In_Game ; i++){
		if(player1_fires[i].id != -1){
			player1_fires[i].row++;
			add_entity_to_updated_entities(
					player1_fires[i].id,
					player1_fires[i].row,
					player1_fires[i].col,
					EFire,1,UPDATE
			);

		}
	}
	for (int i=0 ; i< Max_Fire_In_Game ; i++){
		if(enemy_fires[i].id != -1){
			enemy_fires[i].row--;
			add_entity_to_updated_entities(
					enemy_fires[i].id,
					enemy_fires[i].row,
					enemy_fires[i].col,
					EFire,1,UPDATE
			);
		}
	}

	if(enemy_should_move_down()){
		for(int i=0 ; i<enemy_count ; i++){
			enemies[i].row--;
			add_entity_to_updated_entities(
					enemy_fires[i].id,
					enemy_fires[i].row,
					enemy_fires[i].col,
					EEnemy,1,UPDATE
			);

		}
	}

	if(is_enemy_should_creat()){
		int e_type = rand() % 3;
		Enemy e = create_enemy(e_type);
		add_entity_to_updated_entities(
				e.id,
				e.row,
				e.col,
				EEnemy,
				e_type,
				INSERT
		);
	}

	if(is_enemy_should_fire()){
		Enemy e = select_enemy_for_fire();
		for (int i=0 ; i< Max_Fire_In_Game ; i++){
			if(enemy_fires[i].id == -1){
				enemy_fires[i].id = i;
				enemy_fires[i].col = e.col;
				enemy_fires[i].row = e.row -1;
				enemy_fires[i].fire_power = e.fire_power;
				add_entity_to_updated_entities(
						enemy_fires[i].id,
						enemy_fires[i].row,
						enemy_fires[i].col,
						EFire,
						1,
						INSERT
				);
				break;
			}
		}
	}


	Fire shooted_fire1 = is_enemy_shoot_player();
	if(shooted_fire1.id != 0){
		player1.health--;
		add_entity_to_updated_entities(
				shooted_fire1.id,
				shooted_fire1.row,
				shooted_fire1.col,
				EFire,
				1,
				DELETE
		);
	}

	Fire shooted_fire2 = is_player_shoot_enemy();
	if(shooted_fire2.id != 0){
		Enemy e1 = get_enemy_with_col_and_row(shooted_fire2.col,shooted_fire2.row);
		for(int i=0 ; i < enemy_count ; i++){
			if(enemies[i].id == e1.id){
				enemies[i].health--;
				add_entity_to_updated_entities(
						shooted_fire2.id,
						shooted_fire2.row,
						shooted_fire2.col,
						EFire,
						1,
						DELETE
				);
				if(enemies[i].health == 0){
					add_entity_to_updated_entities(
							enemies[i].id,
							enemies[i].row,
							enemies[i].col,
							EEnemy,
							1,
							DELETE
					);
					enemies[i].id = enemies[enemy_count - 1].id;
					enemies[i].col = enemies[enemy_count - 1].col;
					enemies[i].row = enemies[enemy_count - 1].row;
					enemies[i].enemy_type = enemies[enemy_count - 1].enemy_type;
					enemies[i].fire_power = enemies[enemy_count - 1].fire_power;
					enemies[i].health = enemies[enemy_count - 1].health;
					dead_enemies_count++;
				}
			}
		}
	}

	if(is_game_ended()){
		is_game_end = 1;
	}else{
		Update_Entites();
	}

	handle_method_counter++;
}

int Get_Player_Row(){
	return player1.row;
}

int Get_Player_Col(){
	return player1.col;
}

void Move_Player(MoveDirection md){
	int dx = 0;
	if(md == RIGHT){
		dx += 1;
	}else if(md == LEFT){
		dx -= 1;
	}
	player1.row += dx;
}

void Shoot_Player(){
	Fire f = create_fire_for_player(player1.row, player1.col, 1);
	player1_fires[f.id] = f;
}


void Init_Game(char gl,int r,int c,int handle_timer){

	game_level = gl;
	handle_timer = handle_timer;
	rows_length = r;
	cols_length = c;

	for(int i=0 ; i < Max_Enemy_In_Game ; i++){
		enemies[i] =  (Enemy) {-1,-1,-1,0,0,0};
	}
	for(int i=0; i < Max_Entity_In_Game;i++){
		ues[i] = (UpdatedEntity) {-1, -1 ,-1,-1,-1,-1};
	}

	for(int i=0;i<Max_Fire_In_Game;i++){
		player1_fires[i] = (Fire) {-1,-1,-1,0};
		enemy_fires[i]   = (Fire) {-1,-1,-1,0};
	}
	int health_level = 7;
	if(game_level == 'H'){
		health_level = 3;
	}else if(game_level == 'N'){
		health_level = 5;
	}
	player1 = (Player) {
		0,0,1,health_level,1
	};
}


int SI_Is_Game_Ended(){
	return is_game_end;
}

void SI_Init_Game(){

}
void SI_Load_Game(){

}
