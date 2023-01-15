/*
 * ggame.h
 *
 *  Created on: Jan 15, 2023
 *      Author: t3
 */

#ifndef INC_GGAME_H_
#define INC_GGAME_H_

typedef struct Entity{
	int health;
	int power;
	int entity_type; //ground=0, player=1, enemy=2 , fire=3
	int entity_sub_type; //enemy1=1, enemy2=2, enemy3=3, fire1=4, fire2=5,
} Entity;

typedef struct LoaderResult{
	char arr[2000];
} LoaderResult;

void SI_Init_Game();
LoaderResult SI_Save_Game();
void SI_Load_Game(LoaderResult lr);

#endif /* INC_GGAME_H_ */
