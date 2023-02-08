/*
 * melody.c
 *
 *  Created on: Jan 6, 2023
 *      Author: t3
 */
#include "melody.h"

#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof(*arr))
int wholenote_rest = 120000 / 200;
const Tone rest_melody[]={
  {REST     ,4},
};
int wholenote_fire = 120000 / 200;
const Tone fire_melody[]={
  {NOTE_E5, 8},
  {NOTE_D5, 8},
  {REST     ,4},
};
int wholenote_kill = 120000 / 85;
const Tone kill_melody[]={
  {NOTE_DS4,16},
  {NOTE_F4,16},
  {REST     ,4},
};
int wholenote_menu = 120000 / 50;
const Tone menu_melody[]={
  {NOTE_E5, 8},
  {NOTE_D5, 8},
  {NOTE_FS4,4},
  {NOTE_GS4,4},
  {NOTE_CS5,8},
  {NOTE_B4, 8},
  {NOTE_D4, 4},
  {NOTE_E4, 4},
  {NOTE_B4, 8},
  {NOTE_A4, 8},
  {NOTE_CS4,4},
  {NOTE_E4, 4},
  {NOTE_A4, 2},
  {REST     ,4},
};
int wholenote_boss = 120000 / 185;

const Tone boss_melody[]={
  {NOTE_G4,8},
  {NOTE_C4,8},
  {NOTE_DS4,16},
  {NOTE_F4,16},
  {NOTE_G4,8},
  {NOTE_C4,8},
  {NOTE_DS4,16},
  {NOTE_F4,16}, //1
  {NOTE_G4,8},
  {NOTE_C4,8},
  {NOTE_DS4,16},
  {NOTE_F4,16},
  {NOTE_G4,8},
  {NOTE_C4,8},
  {NOTE_DS4,16},
  {NOTE_F4,16},
  {NOTE_G4,8},
  {NOTE_C4,8},
  {NOTE_E4,16},
  {NOTE_F4,16},
  {NOTE_G4,8},
  {NOTE_C4,8},
  {NOTE_E4,16},
  {NOTE_F4,16},
  {NOTE_G4,8},
  {NOTE_C4,8},
  {NOTE_E4,16},
  {NOTE_F4,16},
  {NOTE_G4,8},
  {NOTE_C4,8},
  {NOTE_E4,16},
  {NOTE_F4,16},
  {NOTE_G4,-4},
  {NOTE_C4,-4},//5
  {NOTE_DS4,16},
  {NOTE_F4,16},
  {NOTE_G4,4},
  {NOTE_C4,4},
  {NOTE_DS4,16},
  {NOTE_F4,16}, //6
  {NOTE_D4,-1}, //7 and 8
  {NOTE_F4,-4},
  {NOTE_AS3,-4},
  {NOTE_DS4,16},
  {NOTE_D4,16},
  {NOTE_F4,4},
  {NOTE_AS3,-4},
  {NOTE_DS4,16},
  {NOTE_D4,16},
  {NOTE_C4,-1}, //11 and 12 repeats from 5
  {NOTE_G4,-4},
  {NOTE_C4,-4},//5
  {NOTE_DS4,16},
  {NOTE_F4,16},
  {NOTE_G4,4},
  {NOTE_C4,4},
  {NOTE_DS4,16},
  {NOTE_F4,16}, //6
  {NOTE_D4,-1}, //7 and 8
  {NOTE_F4,-4},
  {NOTE_AS3,-4},
  {NOTE_DS4,16},
  {NOTE_D4,16},
  {NOTE_F4,4},
  {NOTE_AS3,-4},
  {NOTE_DS4,16},
  {NOTE_D4,16},
  {NOTE_C4,-1}, //11 and 12
  {NOTE_G4,-4},
  {NOTE_C4,-4},
  {NOTE_DS4,16},
  {NOTE_F4,16},
  {NOTE_G4,4},
  {NOTE_C4,4},
  {NOTE_DS4,16},
  {NOTE_F4,16},
  {NOTE_D4,-2},//15
  {NOTE_F4,-4},
  {NOTE_AS3,-4},
  {NOTE_D4,-8},
  {NOTE_DS4,-8},
  {NOTE_D4,-8},
  {NOTE_AS3,-8},
  {NOTE_C4, -1},
  {NOTE_C5, -2},
  {NOTE_AS4,-2},
  {NOTE_C4, -2},
  {NOTE_G4, -2},
  {NOTE_DS4,-2},
  {NOTE_DS4,-4},
  {NOTE_F4, -4},
  {NOTE_G4, -1},
  {NOTE_C5, -2},//28
  {NOTE_AS4,-2},
  {NOTE_C4, -2},
  {NOTE_G4, -2},
  {NOTE_DS4,-2},
  {NOTE_DS4,-4},
  {NOTE_D4, -4},
  {NOTE_C5,  8},
  {NOTE_G4  ,8},
  {NOTE_GS4,16},
  {NOTE_AS4,16},
  {NOTE_C5,  8},
  {NOTE_G4,  8},
  {NOTE_GS4,16},
  {NOTE_AS4,16},
  {NOTE_C5,  8},
  {NOTE_G4,  8},
  {NOTE_GS4,16},
  {NOTE_AS4,16},
  {NOTE_C5,  8},
  {NOTE_G4,  8},
  {NOTE_GS4,16},
  {NOTE_AS4,16},
  {REST     ,4},
  {NOTE_GS5,16},
  {NOTE_AS5,16},
  {NOTE_C6,  8},
  {NOTE_G5,  8},
  {NOTE_GS5,16},
  {NOTE_AS5,16},
  {NOTE_C6,8},
  {NOTE_G5,16},
  {NOTE_GS5,16},
  {NOTE_AS5,16},
  {NOTE_C6,8},
  {NOTE_G5,8},
  {NOTE_GS5,16},
  {NOTE_AS5,16},
  {REST     ,4},
};

void Set_Rest_Melody(){
	Change_Melody(rest_melody, wholenote_rest, ARRAY_LENGTH(rest_melody),0);
}

void Set_Menu_Melody(){
	Change_Melody(menu_melody, wholenote_menu, ARRAY_LENGTH(menu_melody),1);
}
void Set_Boss_Melody(){
	int len = ARRAY_LENGTH(boss_melody);
	Change_Melody(boss_melody, wholenote_boss,  len,1);
}
void Set_Kill_Effect(){
	Change_Melody(kill_melody, wholenote_kill, ARRAY_LENGTH(kill_melody),0);
}
void Set_Fire_Effect(){
	Change_Melody(fire_melody, wholenote_fire, ARRAY_LENGTH(fire_melody),0);
}
