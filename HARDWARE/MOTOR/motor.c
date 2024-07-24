#include "motor.h"
#include "delay.h"
#include "timer.h"
#include "oled.h"

void SETPWM(int PWM1, int PWM2)
{
    // 添加保护机制，防止过大电流
    if (PWM1 > 1000) PWM1 = 1000;
    if (PWM1 < -1000) PWM1 = -1000;
    if (PWM2 > 1000) PWM2 = 1000;
    if (PWM2 < -1000) PWM2 = -1000;

    if (PWM1 > 0) // 正转（前进）
    {
        left_positive();
        TIM_SetCompare1(TIM3, PWM1);
    }
    else // 反转（倒退）
    {
        left_negative();
        TIM_SetCompare1(TIM3, -PWM1);
    }

    if (PWM2 > 0) // 正转（前进）
    {
        right_positive();
        TIM_SetCompare2(TIM3, PWM2);
    }
    else // 反转（倒退）
    {
        right_negative();
        TIM_SetCompare2(TIM3, -PWM2);
    }
}

void PWM_Xianfu(int max, int* PWM1, int* PWM2)
{
    if (*PWM1 > max)  *PWM1 = max;
    if (*PWM1 < -max) *PWM1 = -max;

    if (*PWM2 > max)  *PWM2 = max;
    if (*PWM2 < -max) *PWM2 = -max;
}

void left_positive(void) // 左轮正转
{
    GPIO_SetBits(GPIOB, GPIO_Pin_13);
    GPIO_ResetBits(GPIOB, GPIO_Pin_12);
}

void left_negative(void) // 左轮反转
{
    GPIO_SetBits(GPIOB, GPIO_Pin_12);
    GPIO_ResetBits(GPIOB, GPIO_Pin_13);
}

void right_positive(void) // 右轮正转
{
    GPIO_SetBits(GPIOB, GPIO_Pin_1);
    GPIO_ResetBits(GPIOB, GPIO_Pin_0);
}

void right_negative(void) // 右轮反转
{
    GPIO_SetBits(GPIOB, GPIO_Pin_0);
    GPIO_ResetBits(GPIOB, GPIO_Pin_1);
}

void motor_init()
{
    motor_gpio_init();         // 输出pwm的gpio和控制转向的gpio初始化
    TIM3_PWM_Init(7200 - 1, 1 - 1); // TIM1的pwm模式初始化
    TIM8_encoder_init();       // TIM8的编码器模式初始化
    TIM1_encoder_init();       // TIM2的编码器模式初始化
}

// PWM：   左PA6
//        右PA7
// 正反转： 左PB13、12
//        右PB1、0
void motor_gpio_init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // PWM口初始化：
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // TIM3_CH1  TIM3_CH2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;        // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 正反转控制口初始化：
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_12; // 右正反转
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0; // 左正反转
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

int read_encoder8(void)
{
    int speed;
    speed = (short)TIM_GetCounter(TIM8); // 1.采集编码器的计数值并保存
    TIM_SetCounter(TIM8, 0);              // 2.将定时器的计数值清零

    return speed;
}

int read_encoder1(void)
{
    int speed;
    speed = (short)TIM_GetCounter(TIM1); // 1.采集编码器的计数值并保存
    TIM_SetCounter(TIM1, 0);              // 2.将定时器的计数值清零

    return speed;
}
