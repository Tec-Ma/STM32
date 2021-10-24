#pragma once
#include <stm32f4xx.h>
#include "usart1.h"
extern u16 ADC1Value;

void Adc1_Init();

u16 Get_Adc();