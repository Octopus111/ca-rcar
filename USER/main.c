#include <stm32f10x.h>
#include "delay.h"
#include "oled.h"
#include "sys.h"
#include "usart.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "motor.h"
#include "pid.h"

extern float pitch, roll, yaw; // ŷ����
extern int velocity_measure;   // ת�ٲ���ֵ
extern float velocity_sum;     // �ٶȵ��ۼ�
extern char bluetooth;         // ����ָ��

int i = 0;
float zhongzhi = -2;           // ��е��ֵ��roll������ֵ��
int motor_flag;                // ���ʹ�ܱ�־���Զ����ƣ���1ʹ�� 0ʧ��
int motor = 1;                 // ���ʹ�ܱ�־���ֶ����ƣ���1ʹ�� 0ʧ��
int velocity_calcu = 5;        // �ٶ�����ֵ ���ڴ˶������ֱ�Ӹ��ģ�
float yaw_calcu = 0;           // yaw������ֵ���ڴ˶������ֱ�Ӹ��ģ�

// ֱ��������:
float Kp = 0, Ki = 0, Kd = 0;  // PID����
// �ٶȻ�����:
float VKp = 280, VKi = 1.4;
// ת�򻷲���:
float TKp = 0;

void safety_check() {
    if (velocity_calcu > 100 || velocity_calcu < -100) {
        velocity_calcu = 0; // ��������Χ�������ٶ�����
    }
}

void control_motor() {
    // �ٶ�PID����
    int velocity_pid_output = velocity_PID_value(velocity_measure, velocity_calcu);
    // ֱ��PID����
    int vertical_pid_output = vertical_PID_value(pitch, zhongzhi);
    // �ܿ������
    int control_output = vertical_pid_output + velocity_pid_output;

    // �������ƣ���ֹ�������
    if (control_output > 1000) control_output = 1000;
    if (control_output < -1000) control_output = -1000;

    if (motor_flag) {
        SETPWM(control_output, control_output); // ���õ���ٶ�
    }
    else {
        SETPWM(0, 0); // ֹͣ���
    }
}

int main() {
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // ����NVIC�жϷ���2: 2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    delay_init();
    OLED_Init();
    motor_init();                     // ����� IO pwm ������ ��ʼ��
    MPU_Init();                       // MPU6050��ʼ��
    DMP_Init();                       // DMP��ʼ��
    usart2_init(9600);                // ��������2��ʼ��
    MPU_exti_init();                  // mpu�ⲿ�жϳ�ʼ����һ��Ҫ�����

    /************************************************************
                     ��������Ӳ��׼�����
    *************************************************************/

    while (1) {
        if (pitch < -30 || pitch > 30) { // С�������Ѿ�����
            motor_flag = 0;               // �رյ��
            velocity_sum = 0;             // �ٶȻ�����0
        }
        else {
            motor_flag = 1;               // �������
        }

        safety_check();                  // ��ȫ���
        control_motor();                 // ���Ƶ��

        OLED_ShowAngle(pitch, roll, yaw); // ��ʾŷ����
        OLED_ShowPWM(velocity_measure);   // ��ʾת��
    }
}
