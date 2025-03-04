#ifndef __MOTOR_H
#define __MOTOR_H

void motor_init(void);
void motor_gpio_init(void);
void PWM_Xianfu(int max,int *PWM1,int *PWM2);
void SETPWM(int PWM1,int PWM2);
int read_encoder8(void);
int read_encoder1(void);
void motor_enable(float pitch); //根据角度判断是否使能 or 关闭电机
void left_positive(void);       //左轮正传
void left_negative(void);       //左轮反转
void right_positive(void);      //右轮正转
void right_negative(void);      //右轮反转

#endif
