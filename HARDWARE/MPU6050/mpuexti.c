#include "mpuexti.h"
#include "sys.h"
#include "mpu6050.h"   
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "OLED.h"
#include "delay.h"
#include "pid.h"
#include "motor.h"
#include "timer.h"
extern int motor;          //电机使能标志（手动控制）
extern int motor_flag;     //电机使能标志（自动控制）
extern float zhongzhi;     //机械中值（roll角理论值）
extern int velocity_calcu; //速度理论值
extern float yaw_calcu;    //转向理论值
float pitch,roll,yaw;      //欧拉角
float measure,calcu;       //roll角测量值和理论值 
int velocity_measure;      //速度测量值
float yaw_measure;         //yaw角测量值
int PWM0,PWM1,PWM2;        //两个电机的PWM值 


void MPU_exti_init()
{
	EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
 	GPIO_InitTypeDef GPIO_InitStructure;
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);				  //使能复用功能时钟
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;								  //PA12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 					    //设置成上拉输入
 	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource12); //将PA12映射到中断线12
	EXTI_InitStructure.EXTI_Line=EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;     //下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	  	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;		
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 	//抢占优先级0 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			    //响应优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						
	NVIC_Init(&NVIC_InitStructure);
}

/**********************************************************

					外部中断线12服务程序，PID算法在此函数中

          注：必须严格保证数据接收和PID处理的同步

***********************************************************/ 
void EXTI15_10_IRQHandler(void)
{
	if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
	{ 	
	    //【1.直立环参数】
			measure = pitch;                            //roll测量值
			calcu = zhongzhi;                          //roll理论值
			
		  //【2.速度环参数】
		  //转速测量值 = 两电机转速平均值：
			velocity_measure = ( read_encoder8() + read_encoder1() ) /2;       
		
			//【3.转向环参数】                              
      yaw_measure = yaw;                         //yaw测量值  		
		
		
		  //PID计算：直立环 + 速度环 + 转向环：
		  //PWM0 = vertical_PID_value(measure,calcu) + velocity_PID_value(velocity_measure,velocity_calcu);
		   PWM0=velocity_PID_value(velocity_measure,velocity_calcu);
			//PWM1 = PWM0 + turn_PID_value(yaw_measure,yaw_calcu); 
		  //PWM2 = PWM0 - turn_PID_value(yaw_measure,yaw_calcu); 
			PWM_Xianfu(7000,&PWM0,&PWM0);              //PWM限幅
			
			if(motor_flag && motor) SETPWM(PWM0,PWM0); //给电机PWM
			else                    SETPWM(0,0);       //关闭电机		
	}
	
	EXTI_ClearITPendingBit(EXTI_Line12);           //清除LINE0上的中断标志位  
}
