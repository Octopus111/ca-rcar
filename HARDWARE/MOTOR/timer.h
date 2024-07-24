#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"



void TIM3_PWM_Init(u16 arr,u16 psc); //TIM1的pwm模式初始化
void TIM8_encoder_init(void);        //TIM8的编码器模式初始化
void TIM1_encoder_init(void);        //TIM2的编码器模式初始化
#endif
