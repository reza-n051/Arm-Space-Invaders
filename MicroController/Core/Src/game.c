/*
 * game.c
 *
 *  Created on: Jan 7, 2023
 *      Author: t3
 */

#include "game.h"
#include "stdlib.h"
#include "stdio.h"

Player player;
int is_player_fire = 0;

Boss boss;

int winner = 0; //enemy->-1 player->1

int e_id_gen = 0; //id generator for enemy id

int dead_enemies_count = 0;

int is_enemies_move_down;
UpdatedEntity ues[Max_Entity_In_Game];
int updated_counter = 0;

Enemy enemies[Max_Enemy_In_Game];
Fire player_fires[Max_Fire_In_Game];
Fire enemy_fires[Max_Fire_In_Game];
int enemy_count = 0;
int player_fires_count = 0;
int enemy_fires_count = 0;

Fire boss_fires[Max_Fire_In_Game];
int boss_fires_count=0;

char game_level = 'E';

int rows_length = 20;
int cols_length = 4;

int handle_timer = 50;
int handle_method_counter = 0;

int is_player_kill_an_enemy;

int is_game_end = 0;
int is_non_boss_game_end = 0;

/*
 * 1.movement of enemies and fires
 * 2.check is new enemy should created
 * 3.check should enemy shoot
 * 4.check is enemy or player shooted
 * 5.check is game ended or not
 */

int is_game_ended(){
	if(is_non_boss_game_end == 0 ){
		winner=0;
		return 0;
	}
	if(boss.health <= 0){
		dead_enemies_count++;
		winner = 1;
		return 1;
	}
	if( player.health <= 0){
		winner = -1;
		return 1;

	}
	winner=0;
	return 0;
}
/*
 * Game Ended When:
 * 1.player health become 0.
 * 2.enemies except boss reach to player
 * 3.player kills some enemies
 * (in normal level => 25, in easy level => 15 , in hard level => 35)
 * 4.boss killed
 */
int is_non_boss_game_ended(){
	if(player.health == 0){
		winner = -1;
		return 1;
	}
	for (int i=0 ; i<enemy_count;i++){
		if(enemies[i].row == 1){
			winner = -1;
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
	winner=0;
	return 0;

}



Fire is_enemy_shoot_player(){
	int player_col = player.col;
	int player_row = player.row;
	for(int i=0;i< Max_Fire_In_Game ;i++){
		if(enemy_fires[i].col == player_col && enemy_fires[i].row-1 == player_row){
			return enemy_fires[i];
		}
	}
	return (Fire) {-1,-1,-1,1};
}

Fire is_player_shoot_enemy(){
	for(int i=0;i<enemy_count;i++){
		for(int j = 0 ; j < Max_Fire_In_Game ; j++){
			if( enemies[i].col == player_fires[j].col && enemies[i].row == player_fires[j].row+1 ){
				return player_fires[j];
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
	return (Enemy) {-1,-1,-1,-1,-1,-1};
}

Enemy select_enemy_for_fire(){
	Enemy result_enemy = (Enemy) {-1,-1,-1,-1,-1,-1};
	int selected_idxs[4] = {-1,-1,-1,-1};
	int selected_rows[4] = {rows_length,rows_length,rows_length,rows_length};
	for(int i=0;i<enemy_count;i++){
		if(enemies[i].col==0 && enemies[i].row<selected_rows[0]){
			selected_idxs[0] = i;
			selected_rows[0] = enemies[i].row;
		}else if(enemies[i].col==1 && enemies[i].row<selected_rows[1]){
			selected_idxs[1] = i;
			selected_rows[1] = enemies[i].row;
		}else if(enemies[i].col==2 && enemies[i].row<selected_rows[2]){
			selected_idxs[2] = i;
			selected_rows[2] = enemies[i].row;
		}else if(enemies[i].col==3 && enemies[i].row<selected_rows[3]){
			selected_idxs[3] = i;
			selected_rows[3] = enemies[i].row;
		}
	}
	int random_idx = -1;
	while(random_idx  == -1){
		random_idx = rand() % 4;
		if(selected_rows[random_idx] < 0){
			random_idx = -1;
		}
	}
	result_enemy.col = enemies[selected_idxs[random_idx]].col;
	result_enemy.row = enemies[selected_idxs[random_idx]].row;
	result_enemy.id = enemies[selected_idxs[random_idx]].id;
	result_enemy.health = enemies[selected_idxs[random_idx]].health;
	result_enemy.fire_power = enemies[selected_idxs[random_idx]].fire_power;
	return result_enemy;

}

int enemy_should_move_down(){
	if(
			((handle_method_counter * handle_timer ) % 5000 == 0 && game_level=='E') ||
			((handle_method_counter * handle_timer ) % 3000 == 0 && game_level=='N') ||
			((handle_method_counter * handle_timer ) % 1000 == 0 && game_level=='H')
	){
		return 1;
	}
	return 0;

}


//Fire create_fire_for_player(int row,int col,int power){
//	Fire f = {
//		row,col,-1,power
//	};
//	int index = -1;
//	for(int i=0 ; i< Max_Fire_In_Game ; i++){
//		if(player_fires[i].id != -1){
//			index = i;
//			break;
//		}
//	}
//	f.id = index;
//	return f;
//}


int is_enemy_should_creat(){
	if(
			((handle_method_counter * handle_timer ) % 5000 == 0 && game_level=='E') ||
			((handle_method_counter * handle_timer ) % 3000 == 0 && game_level=='N') ||
			((handle_method_counter * handle_timer ) % 1000 == 0 && game_level=='H')
	){
		return 1;
	}
	return 0;
}

Enemy create_enemy(int enemy_type,int col){
	if(enemy_count <= (rows_length * cols_length) - 8 ){
		enemies[enemy_count].id = e_id_gen++;
			enemies[enemy_count].col = col;
			enemies[enemy_count].row = 19;
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
	return enemies[enemy_count-1];

}

int is_enemy_should_fire(){
	if(
			(game_level == 'H' && (handle_method_counter * handle_timer ) % 1100 != 0 )||
			(game_level == 'N' && (handle_method_counter * handle_timer ) % 3100 != 0 )||
			(game_level == 'E' && (handle_method_counter * handle_timer ) % 5100 != 0 )
	){
		return 0;
	}

	if(
			(handle_method_counter * handle_timer ) % 1000 == 0
	){
		return 0;
	}

	if(enemy_fires_count>Max_Fire_In_Game-1){
		return 0;
	}
	if(enemy_count == 0){
		return 0;
	}
	enemy_fires_count++;
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
	int is_duplicate_delete_update = 0;
	if(action_type == DELETE){
		for(int i=0 ; i<updated_counter ; i++){
			if(ues[i].id == id && ues[i].type == type && ues[i].action_type ==UPDATE){
				ues[i] = (UpdatedEntity) {row,col,id,type,entity_type,action_type};
				is_duplicate_delete_update = 1;
			}
		}
	}
	if(is_duplicate_delete_update == 0 ){
		ues[updated_counter] = (UpdatedEntity) {row,col,id,type,entity_type,action_type};
		updated_counter++;
	}

}

void delete_enemy_from_enemies(int i){

	if(i == enemy_count - 1){
		enemies[i] = (Enemy) {-1,-1,-1,-1,-1,-1};
	}else{
		int col = enemies[enemy_count - 1].col;
		int row = enemies[enemy_count - 1].row;
		int et  = enemies[enemy_count - 1].enemy_type;
		int fp  = enemies[enemy_count - 1].fire_power;
		int id  = enemies[enemy_count - 1].id;
		int hlt = enemies[enemy_count - 1].health;
		enemies[i] = (Enemy) {row,col,id,hlt,fp,et};
		enemies[enemy_count - 1] = (Enemy) {-1,-1,-1,-1,-1,-1};

	}
	enemy_count--;
}
void delete_fire_from_enemy_fires(int i){

	if(i == enemy_fires_count - 1){
		enemy_fires[i] = (Fire) {-1,-1,-1,-1};
	}else{
		int col = enemy_fires[enemy_fires_count - 1].col;
		int row = enemy_fires[enemy_fires_count - 1].row;
		int fp  = enemy_fires[enemy_fires_count - 1].fire_power;
		int id  = enemy_fires[enemy_fires_count - 1].id;
		enemy_fires[i] = (Fire) {row,col,id,fp};
		enemy_fires[enemy_fires_count - 1] = (Fire) {-1,-1,-1,-1};

	}
	enemy_fires_count--;
}
void delete_fire_from_player_fires(int i){

	if(i == player_fires_count - 1){
		player_fires[i] = (Fire) {-1,-1,-1,-1};
	}else{
		int col = player_fires[player_fires_count - 1].col;
		int row = player_fires[player_fires_count - 1].row;
		int fp  = player_fires[player_fires_count - 1].fire_power;
		int id  = player_fires[player_fires_count - 1].id;
		player_fires[i] = (Fire) {row,col,id,fp};
		player_fires[player_fires_count - 1] = (Fire) {-1,-1,-1,-1};

	}
	player_fires_count--;
}
Fire is_player_shoot_boss(){
	for(int i=0;i< Max_Fire_In_Game ;i++){
		if((player_fires[i].col == boss.col || player_fires[i].col == boss.col+1) && player_fires[i].row+2 == boss.row){
			return player_fires[i];
		}
	}
	return (Fire) {-1,-1,-1,1};

}
Fire is_boss_shoot_player(){
	int player_col = player.col;
	int player_row = player.row;
	for(int i=0;i< Max_Fire_In_Game ;i++){
		if(boss_fires[i].col == player_col && boss_fires[i].row-1 == player_row){
			return boss_fires[i];
		}
	}
	return (Fire) {-1,-1,-1,1};

}
void delete_fire_from_boss_fires(int i){
	if(i == boss_fires_count - 1){
		boss_fires[i] = (Fire) {-1,-1,-1,-1};
	}else{
		int col = boss_fires[boss_fires_count - 1].col;
		int row = boss_fires[boss_fires_count - 1].row;
		int fp  = boss_fires[boss_fires_count - 1].fire_power;
		int id  = boss_fires[boss_fires_count - 1].id;
		boss_fires[i] = (Fire) {row,col,id,fp};
		boss_fires[boss_fires_count - 1] = (Fire) {-1,-1,-1,-1};

	}
	boss_fires_count--;

}
int is_boss_should_fire(){
	if(
			(game_level == 'H' && (handle_method_counter * handle_timer ) % 1210 != 0 )||
			(game_level == 'N' && (handle_method_counter * handle_timer ) % 1420 != 0 )||
			(game_level == 'E' && (handle_method_counter * handle_timer ) % 1850 != 0 )
	){
		return 0;
	}

	if(
			(handle_method_counter * handle_timer ) % 1000 == 0
	){
		return 0;
	}

	if(boss_fires_count>Max_Fire_In_Game-1){
		return 0;
	}
	boss_fires_count++;
	return 1;

}

int is_boss_should_move(){
	if(
			((handle_method_counter * handle_timer ) % 1200 == 0 && game_level=='E') ||
			((handle_method_counter * handle_timer ) % 900 == 0 && game_level=='N') ||
			((handle_method_counter * handle_timer ) % 400 == 0 && game_level=='H')
	){
		int r = rand() % 4;
		return r+1;
	}
	return 0;
}
void SI_Handle_Boss_Game(){
	//50 ms
	if((handle_method_counter * handle_timer ) % 10000 == 0){
		int xx=10;
		xx=5;
	}
	clear_updated_entities();
	Fire shooted_fire = is_boss_shoot_player();
	if(shooted_fire.id != -1){
		for(int i=0;i<boss_fires_count;i++){
			if(boss_fires[i].id == shooted_fire.id){
				player.health-=2;
				delete_fire_from_boss_fires(i);
				add_entity_to_updated_entities(
						shooted_fire.id,
						shooted_fire.row,
						shooted_fire.col,
						EFire, 1, DELETE
				);
			}
		}
	}

	for(int i=0;i< boss_fires_count;i++){
		if( boss_fires[i].row <= 0){
			int g_row = boss_fires[i].row;
			int g_col = boss_fires[i].col;
			int g_id =  boss_fires[i].id;
			delete_fire_from_boss_fires(i);
			add_entity_to_updated_entities(g_id,g_row,g_col, EFire,1,DELETE);
		}
	}
	Fire shooted_fire2 = is_player_shoot_boss();
	if(shooted_fire2.id != -1){
		for(int i=0;i<player_fires_count;i++){
			if(player_fires[i].id == shooted_fire2.id){
				delete_fire_from_player_fires(i);
				add_entity_to_updated_entities(
						shooted_fire2.id,
						shooted_fire2.row,
						shooted_fire2.col,
						EPlayerFire,1,DELETE
				);
			}
		}
		boss.health--;
	}
	for(int i=0;i<player_fires_count;i++){
		if(player_fires[i].row >= 19){
			int g_row = player_fires[i].row;
			int g_col = player_fires[i].col;
			int g_id = player_fires[i].id;
			delete_fire_from_player_fires(i);
			add_entity_to_updated_entities(g_id ,g_row ,g_col ,EPlayerFire,1,DELETE);
		}
	}

	if((handle_method_counter * handle_timer ) % 100 == 0){
		for (int i=0 ; i< Max_Fire_In_Game ; i++){
			if(player_fires[i].id != -1){
				player_fires[i].row++;
				int id = player_fires[i].id;
				int col = player_fires[i].col;
				int row = player_fires[i].row;
				add_entity_to_updated_entities(id,row,col,EPlayerFire,1,UPDATE);

			}
		}
	}

	if((handle_method_counter * handle_timer ) % 100 == 0){
		for (int i=0 ; i< Max_Fire_In_Game ; i++){
			if(boss_fires[i].id != -1){
				boss_fires[i].row--;
				int id = boss_fires[i].id;
				int col = boss_fires[i].col;
				int row = boss_fires[i].row;
				add_entity_to_updated_entities(id,row,col,EFire,1,UPDATE);
			}
		}
	}


	if(is_boss_should_fire()){
		for (int i=0 ; i< Max_Fire_In_Game ; i++){
			if(boss_fires[i].id == -1){
				int dir = rand() % 2;
				if(dir == 0 ){
					boss_fires[i] = (Fire) {boss.row -2,boss.col+1,i,2};
				}else{
					boss_fires[i] = (Fire) {boss.row -2,boss.col,i,2};
				}
				boss_fires_count++;
				add_entity_to_updated_entities(
						boss_fires[i].id,
						boss_fires[i].row,
						boss_fires[i].col,
						EFire,1,INSERT
				);
				break;
			}
		}
	}

	if(is_player_fire == 1){
		is_player_fire = 0;
		for (int i=0 ; i< Max_Fire_In_Game ; i++){
			if(player_fires[i].id == -1){
				player_fires[i].id = i;
				player_fires[i].col = player.col;
				player_fires[i].row = player.row +1;
				player_fires[i].fire_power = player.fire_power;
				player_fires_count++;
				add_entity_to_updated_entities(
						player_fires[i].id,
						player_fires[i].row,
						player_fires[i].col,
						EPlayerFire, 1, INSERT
				);
				break;
			}
		}
	}
	int move = is_boss_should_move();
    if(move == 1){
    	if(boss.col<2){
        	boss.col++;
    	}
    }else if(move == 2){
    	if(boss.col>0){
			boss.col--;
    	}
    }else if(move == 3){
    	if(boss.row<16){
    		boss.row++;
    	}
    }else if(move == 4){
    	if(boss.row>10){
    		boss.row--;
    	}
    }
    if(move ==1 ||move ==2 ||move ==3 ||move ==4){
		add_entity_to_updated_entities(
				1,
				boss.row,
				boss.col,
				EBoss,1,UPDATE
		);
		add_entity_to_updated_entities(
				1,
				boss.row,
				boss.col+1,
				EBoss,2,UPDATE
		);
		add_entity_to_updated_entities(
				1,
				boss.row-1,
				boss.col,
				EBoss,3,UPDATE
		);
		add_entity_to_updated_entities(
				1,
				boss.row-1,
				boss.col+1,
				EBoss,4,UPDATE
		);

    }
	if(is_game_ended()){
		is_game_end = 1;
	}
	handle_method_counter++;

}
void SI_Handle_Game(){
	//50 ms =>




	is_player_kill_an_enemy= 0;
	is_enemies_move_down = 0;
	clear_updated_entities();

	Fire shooted_fire1 = is_enemy_shoot_player();
	if(shooted_fire1.id != -1){
		for(int i=0;i<enemy_fires_count;i++){
			if(enemy_fires[i].id == shooted_fire1.id){
				player.health--;
				delete_fire_from_enemy_fires(i);
				add_entity_to_updated_entities(
						shooted_fire1.id,
						shooted_fire1.row,
						shooted_fire1.col,
						EFire, 1, DELETE
				);
			}
		}
	}

	for(int i=0;i<enemy_fires_count;i++){
		if(enemy_fires[i].row <= 0){
			int g_row = enemy_fires[i].row;
			int g_col = enemy_fires[i].col;
			int g_id = enemy_fires[i].id;
			delete_fire_from_enemy_fires(i);
			add_entity_to_updated_entities(g_id,g_row,g_col, EFire,1,DELETE);

		}
	}

	Fire shooted_fire2 = is_player_shoot_enemy();
	if(shooted_fire2.id != -1){
		for(int i=0;i<player_fires_count;i++){
			if(player_fires[i].id == shooted_fire2.id){
				delete_fire_from_player_fires(i);
				add_entity_to_updated_entities(
						shooted_fire2.id,
						shooted_fire2.row,
						shooted_fire2.col,
						EPlayerFire,1,DELETE
				);
			}
		}

		Enemy e1 = get_enemy_with_col_and_row(shooted_fire2.col,shooted_fire2.row+1);
		for(int i=0 ; i < enemy_count ; i++){
			if(enemies[i].id == e1.id){
				enemies[i].health--;
				if(enemies[i].health <= 0){
					is_player_kill_an_enemy = 1;
					int id = enemies[i].id;
					int row = enemies[i].row;
					int col = enemies[i].col;
					int et = enemies[i].enemy_type;
					add_entity_to_updated_entities(id, row, col,EEnemy, et, DELETE);
					delete_enemy_from_enemies(i);
					dead_enemies_count++;
				}
			}
		}
	}
	for(int i=0;i<player_fires_count;i++){
		if(player_fires[i].row >= 19){
			int g_row = player_fires[i].row;
			int g_col = player_fires[i].col;
			int g_id = player_fires[i].id;
			delete_fire_from_player_fires(i);
			add_entity_to_updated_entities(g_id ,g_row ,g_col ,EPlayerFire,1,DELETE);
		}
	}

	if((handle_method_counter * handle_timer ) % 100 == 0){
		for (int i=0 ; i< Max_Fire_In_Game ; i++){
			if(player_fires[i].id != -1){
				player_fires[i].row++;
				int id = player_fires[i].id;
				int col = player_fires[i].col;
				int row = player_fires[i].row;
				add_entity_to_updated_entities(id,row,col,EPlayerFire,1,UPDATE);

			}
		}
	}

	if((handle_method_counter * handle_timer ) % 100 == 0){
		for (int i=0 ; i< Max_Fire_In_Game ; i++){
			if(enemy_fires[i].id != -1){
				enemy_fires[i].row--;
				int id = enemy_fires[i].id;
				int col = enemy_fires[i].col;
				int row = enemy_fires[i].row;
				add_entity_to_updated_entities(id,row,col,EFire,1,UPDATE);
			}
		}
	}
	if(is_enemy_should_fire()){
		Enemy e = select_enemy_for_fire();
		for (int i=0 ; i< Max_Fire_In_Game ; i++){
			if(enemy_fires[i].id == -1){
				enemy_fires[i] = (Fire) {e.row -1,e.col,i,e.fire_power};
				enemy_fires_count++;
				add_entity_to_updated_entities(
						enemy_fires[i].id,
						enemy_fires[i].row,
						enemy_fires[i].col,
						EFire,1,INSERT
				);
				break;
			}
		}
	}

	if(is_player_fire == 1){
		is_player_fire = 0;
		for (int i=0 ; i< Max_Fire_In_Game ; i++){
			if(player_fires[i].id == -1){
				player_fires[i].id = i;
				player_fires[i].col = player.col;
				player_fires[i].row = player.row +1;
				player_fires[i].fire_power = player.fire_power;
				player_fires_count++;
				add_entity_to_updated_entities(
						player_fires[i].id,
						player_fires[i].row,
						player_fires[i].col,
						EPlayerFire, 1, INSERT
				);
				break;
			}
		}
	}

	if(enemy_should_move_down()){
		is_enemies_move_down = 1;
		for(int i=0 ; i<enemy_count ; i++){
			enemies[i].row--;
			add_entity_to_updated_entities(
					enemies[i].id,
					enemies[i].row,
					enemies[i].col,
					EEnemy,enemies[i].enemy_type,UPDATE
			);

		}
	}

	if(is_enemy_should_creat()){
		is_enemies_move_down = 1;
		int e1_type = rand() % 3;
		Enemy e1 = create_enemy(e1_type,0);
		add_entity_to_updated_entities(e1.id,e1.row,e1.col,EEnemy,e1_type,INSERT);

		int e2_type = rand() % 3;
		Enemy e2 = create_enemy(e2_type,1);
		add_entity_to_updated_entities(e2.id,e2.row,e2.col,EEnemy,e2_type,INSERT);

		int e3_type = rand() % 3;
		Enemy e3 = create_enemy(e3_type,2);
		add_entity_to_updated_entities(e3.id,e3.row,e3.col,EEnemy,e3_type,INSERT);

		int e4_type = rand() % 3;
		Enemy e4 = create_enemy(e4_type,3);
		add_entity_to_updated_entities(e4.id,e4.row,e4.col,EEnemy,e4_type,INSERT);
	}

	if(is_non_boss_game_ended()){
		is_non_boss_game_end = 1;
	}

	handle_method_counter++;
}

int SI_Get_Player_Col(){
	return player.col;
}

void SI_Move_Player(MoveDirection md){
	int dx = 0;
	if(md == RIGHT){
		dx += 1;
	}else if(md == LEFT){
		dx -= 1;
	}
	player.col += dx;
	if(player.col == 4 ){
		player.col = 0;
	}
	if(player.col == -1 ){
		player.col = 3;
	}

}

void SI_Shoot_Player(){
	is_player_fire = 1;
}


void SI_Init_Game(char gl,int handle_timer){

	game_level = gl;
	handle_timer = handle_timer;

	for(int i=0 ; i < Max_Enemy_In_Game ; i++){
		enemies[i] =  (Enemy) {-1,-1,-1,0,0,0};
	}
	for(int i=0; i < Max_Entity_In_Game;i++){
		ues[i] = (UpdatedEntity) {-1, -1 ,-1,-1,-1,-1};
	}

	for(int i=0;i<Max_Fire_In_Game;i++){
		player_fires[i] = (Fire) {-1,-1,-1,0};
		enemy_fires[i]   = (Fire) {-1,-1,-1,0};
		boss_fires[i] = (Fire) {-1,-1,-1,0};
	}
	int health_level = 7;
	if(game_level == 'H'){
		health_level = 3;
	}else if(game_level == 'N'){
		health_level = 5;
	}
	player = (Player) {
		0,0,1,health_level,1
	};
	boss = (Boss){
		15,1,30
	};
}


int SI_Is_Game_Ended(){
	return is_game_end;
}
int SI_Is_Non_Boss_Game_Ended(){
	return is_non_boss_game_end;
}
int SI_Get_Winner(){
	return winner;
}
int SI_Get_Remained_Enemies_Killings_For_Winnig(){
	if(game_level == 'H'){
		return 35-dead_enemies_count;
	}else if(game_level == 'E'){
		return 15-dead_enemies_count;
	}else if(game_level == 'N'){
		return 25-dead_enemies_count;
	}
}
int SI_Get_Player_Health(){
	return player.health;
}
int SI_Is_Player_Kill_Enemy(){
	return is_player_kill_an_enemy;
}
int SI_Get_Dead_Enemies(){
	return dead_enemies_count;
}
int SI_Is_Enemies_Move_Down(){
	return is_enemies_move_down;
}

UpdatedEntity* SI_Get_Updated_Entities(){
	return ues;
}

void SI_Reset_Game(){
	is_player_fire = 0;

	winner = 0; //enemy->-1 player->1

	e_id_gen = 0; //id generator for enemy id

	dead_enemies_count = 0;

	updated_counter = 0;
	enemy_count = 0;
	player_fires_count = 0;
	enemy_fires_count = 0;

	boss_fires_count=0;

	game_level = 'E';

	rows_length = 20;
	cols_length = 4;

	handle_timer = 50;
	handle_method_counter = 0;

	is_player_kill_an_enemy = 0;

	is_game_end = 0;
	is_non_boss_game_end = 0;

}
