#include "timer5_capture.h"

u8 TIM5CH1_CAPTURE_STA = 0; //标志
u32 TIM5CH1_CAPTURE_VAL = 0;//捕获值

void Timer5_Capture_Init(u32 arr, u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;	 
	GPIO_Init(GPIOA, &GPIO_InitStructure);			   

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5); 

	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;				
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;			
	TIM_ICInitStructure.TIM_ICFilter = 0x00;						
	TIM_ICInit(TIM5, &TIM_ICInitStructure);

	TIM_ITConfig(TIM5, TIM_IT_Update|TIM_IT_CC1, ENABLE);

	TIM_Cmd(TIM5, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  
	NVIC_Init(&NVIC_InitStructure);							  
}



void TIM5_IRQHandler(void)
{
	if ((TIM5CH1_CAPTURE_STA & 0X80) == 0) 
	{
		if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)//定时器计数溢出
		{
			if (TIM5CH1_CAPTURE_STA & 0X40) 
			{
				if ((TIM5CH1_CAPTURE_STA & 0X3F) == 0X3F) 
				{
					TIM5CH1_CAPTURE_STA |= 0X80; 
					TIM5CH1_CAPTURE_VAL = 0XFFFFFFFF;
				}
				else
					TIM5CH1_CAPTURE_STA++;
			}
		}
		if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)//捕获事件
		{
			if (TIM5CH1_CAPTURE_STA & 0X40)
			{
				TIM5CH1_CAPTURE_STA |= 0X80;
				TIM5CH1_CAPTURE_VAL = TIM_GetCapture1(TIM5);
				TIM_OC1PolarityConfig(TIM5, TIM_ICPolarity_Rising);
			}
			else
			{
				TIM5CH1_CAPTURE_STA = 0;
				TIM5CH1_CAPTURE_VAL = 0;
				TIM5CH1_CAPTURE_STA |= 0X40; 
				TIM_Cmd(TIM5, DISABLE);
				TIM_SetCounter(TIM5, 0);
				TIM_OC1PolarityConfig(TIM5, TIM_ICPolarity_Falling);
				TIM_Cmd(TIM5, ENABLE);								
			}
		}
	}
	TIM_ClearITPendingBit(TIM5, TIM_IT_CC1 | TIM_IT_Update);
}

void Timer5_Capture()
{
	double temp = 0;
	Delay(10);
	if (TIM5CH1_CAPTURE_STA & 0X80)
	{
		GPIO_ToggleBits(GPIOB, GPIO_Pin_0);
		temp = TIM5CH1_CAPTURE_STA & 0X3F;
		temp *= 0XFFFFFFFF;
		temp += TIM5CH1_CAPTURE_VAL;
		printf("HIGH:%lf s\r\n", temp / 1000000);
		TIM5CH1_CAPTURE_STA = 0;
	}
}