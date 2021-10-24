#pragma once
#include <stm32f4xx.h>
#include "systic.h"
#include "usart1.h"

#define TPAD_ARR_MAX_VAL	0xFFFFFFFF


void Timer2_Capture_Init(u32 arr,u16 psc);
u8 Tpad_Init(u8 psc);
void Tpad_Reset(void);
u16 Tpad_Get_Val(void);
u16 Tpad_Get_Maxval(u8 n);
u8 Tpad_Scan(u8 mode);