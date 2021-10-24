#include "systic.h"

void Delay_Init()
{
	//主频 180Mhz	每数一次 1/180 us
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
}

void Delay_us(u32 us)
{
	u32 temp;
	SysTick->LOAD = us * 180;
	SysTick->VAL = 0x00;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	do
	{
		temp = SysTick->CTRL;
	} while ((temp & 0x01) && !(temp & 1 << 16));//CTRL第0位为1使能，最高位不为1(1表示数到)
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;//关闭
	SysTick->VAL = 0x00;//清零
}

void Delay_ms(u32 ms)
{
	u32 i;
	for (i = 0; i < ms; i++)
	{
		Delay_us(1000);
	}
}

//软件延时
void Delay(u32 ms)
{
	int i, j;
	for (i = 0; i < ms; i++)
	{
		for (j = 0; j < 12000; j++)
		{
			asm("nop");
		}
	}
}