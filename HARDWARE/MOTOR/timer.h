#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"



void TIM3_PWM_Init(u16 arr,u16 psc); //TIM1��pwmģʽ��ʼ��
void TIM8_encoder_init(void);        //TIM8�ı�����ģʽ��ʼ��
void TIM1_encoder_init(void);        //TIM2�ı�����ģʽ��ʼ��
#endif
