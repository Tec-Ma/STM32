#include "timer1_pwm.h"

void Timer1_Pwm_Init(u32 arr, u32 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM1);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Prescaler = psc;	//预分频 90M/90 =  1M
	TIM_TimeBaseStructure.TIM_Period = arr;	//周期 1M/100 = 10kHz	能量 n/100
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCNPolarity_Low;
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCxN_Enable);//使用的是CH3N通道，为CH3的反向通道，需使能
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable); //使能实时更新
	TIM_CtrlPWMOutputs(TIM1, ENABLE);//高级定时器需使能控制PWM输出
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	TIM_Cmd(TIM1, ENABLE);
}

u8 dir = 1;
u16 led0pwmval = 0;
void Pwm_Led0()
{
	Delay_ms(10);
	if (dir)
		led0pwmval++;
	else
		led0pwmval--;
	if (led0pwmval > 100)
		dir = 0;
	if (led0pwmval == 0)
		dir = 1;
	TIM_SetCompare3(TIM1, led0pwmval);
}