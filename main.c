#include <stm32f4xx.h>
#include "adc.h"
#include "key.h"
#include "led.h"
#include "systic.h"
#include "timer1_pwm.h"
#include "timer2_capture.h"
#include "timer3.h"
#include "timer5_capture.h"
#include "usart1.h"
#include "usart3.h"


int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//2位抢占 2位子优先
	Led_Init();										//LED初始化
//	Key_Init();										//按键初始化
	Delay_Init();									//延时初始化
	Usart1_Init();									//串口1 pc				中断1 1
//	Usart3_Init();									//串口3 wifi				中断1 2
	//Adc1_Init();									//ADC1 采集PA5电压		中断1 0
	Timer1_Pwm_Init(100 - 1, 90 - 1);				//定时器1 PWM
	//Tpad_Init(1);
//	Timer3_Init(5000-1,18000-1);					//定时器3 定时			中断1 3
//	Timer5_Capture_Init(0XFFFFFFFF, 90 - 1);		//定时器2 捕获key_up		中断0 3

	//u8 t = 0;
	u8 dir = 1;
	u16 led0pwmval = 0;
	
	while (1)
	{
		//if (Tpad_Scan(0))
		//{
			//GPIO_ToggleBits(GPIOB, GPIO_Pin_0);
		//}
		//t++;
		//if (t == 15)
		//{
			//t = 0;
			//GPIO_ToggleBits(GPIOB, GPIO_Pin_1);
		//}
		//Delay_ms(10);
			//Delay_ms(10);
			//if (dir)
				//led0pwmval++;
			//else
				//led0pwmval--;
			//if (led0pwmval > 100)
				//dir = 0;
			//if (led0pwmval == 0)
				//dir = 1;
		Delay_ms(100);
		TIM_SetCompare3(TIM1, 95);
		Delay_ms(100);
		TIM_SetCompare3(TIM1, 100);
	}
	return 0;
}





//wifi模块功能

//char *cmd = "LED1=TOG";

//		if (USART1_RX_LEN)
//		{
//			delay(5);//等待串口接收
//			printf("%s\n",USART1_RX_BUF);
//			memset(USART1_RX_BUF, '\0', USART1_RX_LEN);
//			USART1_RX_LEN = 0;
//		}
//		if (USART3_RX_LEN)
//		{
//			delay(3);
//			if (strcmp(USART3_RX_BUF, cmd) == 0)
//			{
//				GPIO_ToggleBits(GPIOB, GPIO_Pin_0);
//				printf("LED1 Toggled!\n");
//				printf3("LED1 Toggled!");
//			}
//			memset(USART3_RX_BUF, '\0', USART3_RX_LEN);
//			USART3_RX_LEN = 0;
//		}
