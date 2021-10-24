#include "timer2_capture.h"

int tpad_default_val = 0; //空载的时候

//定时器2 通道1 输入捕获
void Timer2_Capture_Init(u32 arr, u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM2_ICInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_TIM2);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM2_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM2_ICInitStructure.TIM_ICFilter = 0x00;
	TIM_ICInit(TIM2, &TIM2_ICInitStructure);

	TIM_Cmd(TIM2, ENABLE);
}

u8 Tpad_Init(u8 psc)
{
	u16 buf[10], temp;
	u8 j, i;
	Timer2_Capture_Init(TPAD_ARR_MAX_VAL, psc-1);
	for (i = 0; i < 10; i++)
	{
		buf[i] = Tpad_Get_Val();
		Delay_ms(10);
	}
	for (i = 0; i < 9; i++)
	{
		for (j = i + 1; j < 10; j++)
		{
			if (buf[i] > buf[j]) 
			{
				temp = buf[i];
				buf[i] = buf[j];
				buf[j] = temp;
			}
		}
	}
	temp = 0;
	for (i = 2; i < 8; i++)
		temp += buf[i];
	tpad_default_val = temp / 6;
	printf("tpad_default_val:%d\r\n", tpad_default_val);
	if (tpad_default_val > TPAD_ARR_MAX_VAL / 2)
		return 1;
	return 0;
}

void Tpad_Reset(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	  

	GPIO_ResetBits(GPIOA, GPIO_Pin_5); 

	Delay_ms(5);
	TIM_ClearITPendingBit(TIM2, TIM_IT_CC1 | TIM_IT_Update); 
	TIM_SetCounter(TIM2, 0);								 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;		   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;   
	GPIO_Init(GPIOA, &GPIO_InitStructure);			   
}

u16 Tpad_Get_Val(void)
{
	Tpad_Reset();
	while(TIM_GetFlagStatus(TIM2, TIM_IT_CC1) == RESET)
	{
		if(TIM_GetCounter(TIM2)>TPAD_ARR_MAX_VAL-500)return TIM_GetCounter(TIM2);
	}
	return TIM_GetCapture1(TIM2);	 
}


u16 Tpad_Get_Maxval(u8 n)
{
	u16 temp=0;
	u16 res=0; 
	while(n--)
	{
		temp=Tpad_Get_Val();
		if(temp>res)res=temp;
	}
	return res;
} 

#define TPAD_GATE_VAL 	100	
u8 Tpad_Scan(u8 mode)
{
	static u8 keyen=0;	
	u8 res=0;
	u8 sample=3;		
	u16 rval;
	if(mode)
	{
		sample=6;	
		keyen=0;	
	}
	rval=Tpad_Get_Maxval(sample); 
	if(rval>(tpad_default_val+TPAD_GATE_VAL)&&rval<(10*tpad_default_val))
	{							 
		if((keyen==0)&&(rval>(tpad_default_val+TPAD_GATE_VAL)))
		{
			res=1;
		}
		//printf("r:%d\r\n",rval);
		keyen=3;  
	} 
	if(keyen)keyen--;		   							   		     	    					   
	return res;
}	 