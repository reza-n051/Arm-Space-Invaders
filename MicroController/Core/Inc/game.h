/*
 * game.h
 *
 *  Created on: Jan 7, 2023
 *      Author: t3
 */

#ifndef INC_GAME_H_
#define INC_GAME_H_

typedef int bool;

enum EntityType{
	EEnemy=1,
	EPlayer=0,
	EFire=2
};
typedef enum MoveDirection{
	RIGHT=1,
	LEFT=2,
	UP=3,
	DOWN=4
}MoveDirection;
typedef struct Enemy{
	int row;
	int col;
	int id;
	int health;
	int fire_power;
	int enemy_type;
} Enemy;

typedef struct Player{
	int id;
	int row;
	int col;
	int health;
	int fire_power;
} Player;


typedef struct Fire{
	int row;
	int col;
	int id;
	int fire_power;
} Fire;

typedef struct Entity{
	int row;
	int col;
	int id;
	int entity_type; //boss=0, enemy1=1, enemy2=2, enemy3=3, fire1=4, fire2=5, player1=6
} Entity;

void Init_Game();

void Move_Player(MoveDirection md);

void Shoot_Player();

void Handle_Game_AI();

#endif /* INC_GAME_H_ */
