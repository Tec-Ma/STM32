#pragma once
#include <stm32f4xx.h>
#include "systic.h"

void Timer1_Pwm_Init(u32 arr, u32 psc);
void Pwm_Led0();