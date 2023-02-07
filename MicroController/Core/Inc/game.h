/*
 * game.h
 *
 *  Created on: Jan 7, 2023
 *      Author: t3
 */

#ifndef INC_GAME_H_
#define INC_GAME_H_

#define Max_Entity_In_Game 100
#define Max_Enemy_In_Game 76
#define Max_Fire_In_Game 8

typedef struct Row{
	int col;
} Row;


enum EntityType{
	EPlayer=0,
	EEnemy=1,
	EPlayerFire=3,
	EFire=2
};
typedef enum ActionType{
	DELETE=1,
	UPDATE=2,
	INSERT=3
}ActionType;

typedef enum MoveDirection{
	RIGHT=1,
	LEFT=2,
}MoveDirection;

typedef struct Enemy{
	int row;
	int col;
	int id;
	int health;
	int fire_power;
	int enemy_type;
} Enemy;

typedef struct Fire{
	int row;
	int col;
	int id;
	int fire_power;
} Fire;

typedef struct Player{
	int id;
	int row;
	int col;
	int health;
	int fire_power;
} Player;

typedef struct UpdatedEntity{
	int row;
	int col;
	int id;
	int type;//1=enemy 2=fire
	int entity_type; //1=e1=f1 2=e2=f2 3=e3=f3
	int action_type;//delete=1 update=2 insert=3
} UpdatedEntity;

typedef struct LoaderResult{

	char arr[2000];
} LoaderResult;

void SI_Init_Game(char gl,int handle_timer);

void SI_Move_Player(MoveDirection md);

void SI_Shoot_Player();

void SI_Handle_Game();

UpdatedEntity* SI_Get_Updated_Entities();

int SI_Get_Player_Col();

int SI_Is_Game_Ended();
int SI_Get_Dead_Enemies();
int SI_Get_Remained_Enemies_Killings_For_Winnig();
int SI_Get_Player_Health();
int SI_Is_Player_Kill_Enemy();
int SI_Get_Winner();
int SI_Is_Enemies_Move_Down();
#endif /* INC_GAME_H_ */
