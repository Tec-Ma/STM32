#pragma once
#include <stm32f4xx.h>
#include "adc.h"
#include "led.h"

void Timer3_Init(unsigned int period, unsigned short prescaler);