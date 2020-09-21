#include "Timer_Driver.h"
#include "stm32f4xx.h"


/****************************** PRIVATE VARIABLES ******************************/
static volatile uint32_t vu32_TimerTick = 0;
/********************************************************************************
* 函数名 :  TIM3_Init
* 功  能 :  定时器3初始化
* 说  明 :  定时器3时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数10次；
            定时器3提供基准定时：1ms
* 入  参 :  none
* 返  回 :  none
* 设  计 :  Shatang                   时  间 :  2020.06.18
* 修  改 :  none                      时  间 :  none
********************************************************************************/
void TIM3_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	
    TIM_TimeBaseInitStructure.TIM_Period = 9; 	      //自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler= 8399;    //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;//定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;  //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03;         //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//定时器3中断服务函数：1ms基准
void TIM3_IRQHandler(void)
{
    uint8_t i = 0;
    
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) !=RESET) //溢出中断
	{
        vu32_TimerTick++;
        
        TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
	}
}
/********************************************************************************
* 函数名 : SysTimeGet
* 功  能 : 获取系统时间
* 说  明 : none
* 入  参 : none
* 返  回 : none
* 设  计 : Shatang                    时  间 : 2020.09.21
* 修  改 : none                       时  间 : none
********************************************************************************/
uint32_t SysTimeGet(void)
{
    return vu32_TimerTick;
}
