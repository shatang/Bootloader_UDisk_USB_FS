#include "Timer_Driver.h"
#include "stm32f4xx.h"


/****************************** PRIVATE VARIABLES ******************************/
static volatile uint32_t vu32_TimerTick = 0;
/********************************************************************************
* ������ :  TIM3_Init
* ��  �� :  ��ʱ��3��ʼ��
* ˵  �� :  ��ʱ��3ʱ��84M����Ƶϵ��8400������84M/8400=10Khz�ļ���Ƶ�ʣ�����10�Σ�
            ��ʱ��3�ṩ��׼��ʱ��1ms
* ��  �� :  none
* ��  �� :  none
* ��  �� :  Shatang                   ʱ  �� :  2020.06.18
* ��  �� :  none                      ʱ  �� :  none
********************************************************************************/
void TIM3_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
	
    TIM_TimeBaseInitStructure.TIM_Period = 9; 	      //�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler= 8399;    //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;//��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;  //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03;         //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//��ʱ��3�жϷ�������1ms��׼
void TIM3_IRQHandler(void)
{
    uint8_t i = 0;
    
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) !=RESET) //����ж�
	{
        vu32_TimerTick++;
        
        TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
	}
}
/********************************************************************************
* ������ : SysTimeGet
* ��  �� : ��ȡϵͳʱ��
* ˵  �� : none
* ��  �� : none
* ��  �� : none
* ��  �� : Shatang                    ʱ  �� : 2020.09.21
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
uint32_t SysTimeGet(void)
{
    return vu32_TimerTick;
}
