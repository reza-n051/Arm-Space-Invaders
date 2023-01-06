/*
 * buzzer.h
 *
 *  Created on: Jan 5, 2023
 *      Author: t3
 */

#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

#include "stm32f3xx_hal.h"

typedef struct {
	uint16_t freq;
	uint16_t devider;
} Tone;
void Init_Buzzer_PWM(TIM_HandleTypeDef* htim,uint32_t channel);
void PWM_Play();
void PWM_Pause();
void PWM_Change_Tone(uint16_t pwm_freq, uint16_t volume);
void PWM_Set_Volume(int v);
void Update_Melody();
void Change_Melody(const Tone* melody , const uint32_t wholenote, uint16_t melody_len);

#endif /* INC_BUZZER_H_ */
