#pragma once
#include <stm32f4xx.h>
#include "systic.h"
#include "usart1.h"

extern u8 TIM5CH1_CAPTURE_STA;//标志
extern u32 TIM5CH1_CAPTURE_VAL; //捕获值

void Timer5_Capture_Init(u32 arr,u16 psc);