/*
 * game.c
 *
 *  Created on: Jan 7, 2023
 *      Author: t3
 */

#include "game.h"
#include "stdlib.h"

#define Max_Entity_In_Game 100
#define Max_Enemy_In_Game 76
#define Max_Fire_In_Game 8


int player_id=1;
int enemy_id =1;
int fire_id  =1;

Player player1;
Entity entities[Max_Entity_In_Game];
int entity_count = 0;
Enemy enemies[Max_Enemy_In_Game];
int enemy_count = 0;
Fire player1_fires[Max_Fire_In_Game];
Fire enemy_fires[Max_Fire_In_Game];
int dead_enemies_count = 0;
char game_level = 'E';
int rows_length;
int cols_length;
int enemy_fires_count = 0;
int handle_timer = 10;
int handle_method_counter = 0;
int is_game_end = 0;

Enemy enemy_changes[Max_Enemy_In_Game];

Fire* get_player_fire(){
	return player1_fires;
}
Fire* get_enemy_fire(){
	return enemy_fires;
}
Enemy* get_enemy(){
	return enemies;
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
		entities[i] = (Entity) {-1, -1 ,-1,-1};
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

void create_enemy(int enemy_type){
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

int get_is_game_ended(){
	return is_game_end;
}

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

int is_enemy_shoot_player(){
	int player_col = player1.col;
	int player_row = player1.row;
	for(int i=0;i< Max_Fire_In_Game ;i++){
		if(enemy_fires[i].col == player_col && enemy_fires[i].row == player_row){
			return 1;
		}
	}
	return 0;
}

int is_player_shoot_enemy(){
	for(int i=0;i<enemy_count;i++){
		for(int j = 0 ; j < Max_Fire_In_Game ; j++){
			if(enemies[i].row == player1_fires[j].row && enemies[i].col == player1_fires[j].col){
				return enemies[i].id;
			}
		}
	}
	return 0;
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

/*
 * 1.movement of enemies and fires
 * 2.check is new enemy should created
 * 3.check should enemy shoot
 * 4.check is enemy or player shooted
 * 5.check is game ended or not
 */
Entity* Handle_Game(){
	//50 ms =>

	for (int i=0 ; i< Max_Fire_In_Game ; i++){
		if(enemy_fires[i].id != -1){
			enemy_fires[i].row--;
		}
	}

	if(enemy_should_move_down()){
		for(int i=0 ; i<enemy_count ; i++){
			enemies[i].row--;
			enemy_changes[i].row--;
		}
	}

	if(is_enemy_should_creat()){
		create_enemy(rand() % 3);
	}

	if(is_enemy_should_fire()){
		Enemy e = select_enemy_for_fire();
		for (int i=0 ; i< Max_Fire_In_Game ; i++){
			if(enemy_fires[i].id == -1){
				enemy_fires[i].id = i;
				enemy_fires[i].col = e.col;
				enemy_fires[i].row = e.row;
				enemy_fires[i].fire_power = e.fire_power;
				break;
			}
		}
	}

	if(is_enemy_shoot_player()){
		player1.health--;
	}

	int e_id = is_player_shoot_enemy();
	if(e_id != 0){
		for(int i=0 ;i<enemy_count;i++){
			if(enemies[i].id == e_id){
				enemies[i].health--;
			}
		}
	}

	if(is_game_ended()){
		is_game_end = 1;
	}
	handle_method_counter++;
}

void Move_Player(MoveDirection md){
	int dx = 0;
	int dy = 0;
	if(md == RIGHT){
		dx += 1;
	}else if(md == LEFT){
		dx -= 1;
	}else if(md == UP){
		dy += 1;
	}else if(md == DOWN){
		dy -= 1;
	}
	player1.col += dy;
	player1.row += dx;
}

void Shoot_Player(){
	Fire f = create_fire_for_player(player1.row, player1.col, 1);
	player1_fires[f.id] = f;
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
