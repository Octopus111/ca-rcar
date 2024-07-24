#include <stm32f10x.h>
#include "delay.h"
#include "sys.h"
#include "timer.h"
 
//TIM1的PWM模式初始化（TIM3_CH1--PA6  TIM3_CH2--PA7）
//arr：自动重装值
//psc：时钟预分频数
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);	//使能定时器3时钟
	
  //初始化TIM3,设置ARR和RCC:
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM3的PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	
	TIM_OC1Init(TIM3,&TIM_OCInitStructure);           //初始化TIM1_CH1  
	TIM_OC2Init(TIM3,&TIM_OCInitStructure);           //初始化TIM1_CH4  
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_Cmd(TIM3, ENABLE);

	
	TIM_SetCompare1(TIM3,0); //先置零占空比
  TIM_SetCompare2(TIM3,0);
}

//编码器初始化TIM8
void TIM8_encoder_init(void) //TIM8的编码器模式初始化
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_ICInitTypeDef TIM_ICInitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;//初始化GPIO--PC6、PC7
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6 |GPIO_Pin_7;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);//初始化定时器。
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=65535;
	TIM_TimeBaseInitStruct.TIM_Prescaler=0;
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseInitStruct);
	
	TIM_EncoderInterfaceConfig(TIM8,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//配置编码器模式
	
	TIM_ICStructInit(&TIM_ICInitStruct);//初始化输入捕获
	TIM_ICInitStruct.TIM_ICFilter=10;
	TIM_ICInit(TIM8,&TIM_ICInitStruct);
	
	TIM_ITConfig(TIM8,TIM_IT_Update,ENABLE);//配置溢出更新中断标志位
	
	TIM_SetCounter(TIM8,0);//清零定时器计数值
	
	TIM_Cmd(TIM8,ENABLE);//开启定时器	
	TIM_CtrlPWMOutputs(TIM8, ENABLE);//TIM8是高级定时器，需要总输出使能
}
	
void TIM8_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM8,TIM_IT_Update)!=0)
	{
		TIM_ClearITPendingBit(TIM8,TIM_IT_Update);
	}
}


void TIM1_encoder_init(void) //TIM1的编码器模式初始化
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_ICInitTypeDef TIM_ICInitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;//初始化GPIO--PA8、PA9
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8 |GPIO_Pin_9;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);//初始化定时器。
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=65535;
	TIM_TimeBaseInitStruct.TIM_Prescaler=0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
	
	TIM_EncoderInterfaceConfig(TIM1,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//配置编码器模式
	
	TIM_ICStructInit(&TIM_ICInitStruct);//初始化输入捕获
	TIM_ICInitStruct.TIM_ICFilter=10;
	TIM_ICInit(TIM1,&TIM_ICInitStruct);
	
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);//配置溢出更新中断标志位
	
	TIM_SetCounter(TIM1,0);//清零定时器计数值
	
	TIM_Cmd(TIM1,ENABLE);//开启定时器	
	TIM_CtrlPWMOutputs(TIM1, ENABLE);//TIM1是高级定时器，需要总输出使能
}
	
void TIM1_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)!=0)
	{
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
	}
}
