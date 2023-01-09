/*
 * buzzer.c
 *
 *  Created on: Jan 5, 2023
 *      Author: t3
 */
#include "buzzer.h"
#include <stdlib.h>

TIM_HandleTypeDef* buzzer_htim;
uint32_t buzzer_channel;

const Tone *volatile current_melody_ptr;
volatile uint16_t current_melody_tone_count;
volatile uint16_t current_tone_number;
volatile uint32_t current_melody_wholenote;
volatile uint32_t current_tone_end;
volatile uint16_t volume = 999;


int dividerToDuration(int divider,int wholenote){
	int d = 0;
	if(divider>0){
		d = wholenote / divider;
	}else if(divider<0){
		d = wholenote / abs(divider);
		d *= 1.5;
	}
	return d*0.9;
}


void Init_Buzzer_PWM(TIM_HandleTypeDef* htim,uint32_t channel){
	buzzer_channel = channel;
	buzzer_htim = htim;
}

void PWM_Play(){
	HAL_TIM_PWM_Start(buzzer_htim, buzzer_channel);
}
void PWM_Pause(){
	HAL_TIM_PWM_Stop(buzzer_htim, buzzer_channel);
}

void Change_Melody(const Tone* melody , const uint32_t wholenote, uint16_t melody_len){
	current_melody_ptr = melody;
	current_melody_wholenote = wholenote;
	current_melody_tone_count = melody_len;
	current_tone_number = 0;
}


void PWM_Change_Tone(uint16_t pwm_freq, uint16_t pwm_volume) // pwm_freq (1 - 20000), volume (0 - 1000)
{
    if (pwm_freq == 0 || pwm_freq > 20000)
    {
        __HAL_TIM_SET_COMPARE(buzzer_htim, buzzer_channel, 0);
    }
    else
    {
        const uint32_t internal_clock_freq = HAL_RCC_GetSysClockFreq();
        const uint16_t prescaler = 1 + internal_clock_freq / pwm_freq / 60000;
        const uint32_t timer_clock = internal_clock_freq / prescaler;
        const uint32_t period_cycles = timer_clock / pwm_freq;
        const uint32_t pulse_width = pwm_volume * period_cycles / 1000 / 2;
        TIM_HandleTypeDef* pwm = buzzer_htim;
        pwm->Instance->PSC = prescaler - 1;
        pwm->Instance->ARR = period_cycles - 1;
        pwm->Instance->EGR = TIM_EGR_UG;
        __HAL_TIM_SET_COMPARE(buzzer_htim, buzzer_channel, pulse_width); // pwm_timer->Instance->CCR2 = pulse_width;
    }
}

void PWM_Set_Volume(int v){
	if(v< 0){
		volume = 0;
	}else if(v > 25){
		volume = 25;
	}else{
		volume = v;
	}
}

void Update_Melody(){
	if((HAL_GetTick()>current_tone_end) && (current_tone_number<current_melody_tone_count)){
		const Tone active_tone = *(current_melody_ptr + current_tone_number);
		PWM_Change_Tone(active_tone.freq,volume);
		int duration = dividerToDuration(active_tone.devider, current_melody_wholenote);
		current_tone_end = HAL_GetTick() + duration;
		current_tone_number++;
	}
}
