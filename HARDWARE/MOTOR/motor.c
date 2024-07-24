#include "motor.h"
#include "delay.h"
#include "timer.h"
#include "oled.h"

void SETPWM(int PWM1, int PWM2)
{
    // ��ӱ������ƣ���ֹ�������
    if (PWM1 > 1000) PWM1 = 1000;
    if (PWM1 < -1000) PWM1 = -1000;
    if (PWM2 > 1000) PWM2 = 1000;
    if (PWM2 < -1000) PWM2 = -1000;

    if (PWM1 > 0) // ��ת��ǰ����
    {
        left_positive();
        TIM_SetCompare1(TIM3, PWM1);
    }
    else // ��ת�����ˣ�
    {
        left_negative();
        TIM_SetCompare1(TIM3, -PWM1);
    }

    if (PWM2 > 0) // ��ת��ǰ����
    {
        right_positive();
        TIM_SetCompare2(TIM3, PWM2);
    }
    else // ��ת�����ˣ�
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

void left_positive(void) // ������ת
{
    GPIO_SetBits(GPIOB, GPIO_Pin_13);
    GPIO_ResetBits(GPIOB, GPIO_Pin_12);
}

void left_negative(void) // ���ַ�ת
{
    GPIO_SetBits(GPIOB, GPIO_Pin_12);
    GPIO_ResetBits(GPIOB, GPIO_Pin_13);
}

void right_positive(void) // ������ת
{
    GPIO_SetBits(GPIOB, GPIO_Pin_1);
    GPIO_ResetBits(GPIOB, GPIO_Pin_0);
}

void right_negative(void) // ���ַ�ת
{
    GPIO_SetBits(GPIOB, GPIO_Pin_0);
    GPIO_ResetBits(GPIOB, GPIO_Pin_1);
}

void motor_init()
{
    motor_gpio_init();         // ���pwm��gpio�Ϳ���ת���gpio��ʼ��
    TIM3_PWM_Init(7200 - 1, 1 - 1); // TIM1��pwmģʽ��ʼ��
    TIM8_encoder_init();       // TIM8�ı�����ģʽ��ʼ��
    TIM1_encoder_init();       // TIM2�ı�����ģʽ��ʼ��
}

// PWM��   ��PA6
//        ��PA7
// ����ת�� ��PB13��12
//        ��PB1��0
void motor_gpio_init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // PWM�ڳ�ʼ����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // TIM3_CH1  TIM3_CH2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;        // �����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // ����ת���ƿڳ�ʼ����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_12; // ������ת
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       // �������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0; // ������ת
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       // �������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

int read_encoder8(void)
{
    int speed;
    speed = (short)TIM_GetCounter(TIM8); // 1.�ɼ��������ļ���ֵ������
    TIM_SetCounter(TIM8, 0);              // 2.����ʱ���ļ���ֵ����

    return speed;
}

int read_encoder1(void)
{
    int speed;
    speed = (short)TIM_GetCounter(TIM1); // 1.�ɼ��������ļ���ֵ������
    TIM_SetCounter(TIM1, 0);              // 2.����ʱ���ļ���ֵ����

    return speed;
}
