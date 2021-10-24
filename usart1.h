#pragma once
#include <stm32f4xx.h>
#include <stdio.h>

#define MAXSIZE 1000
extern unsigned int USART1_RX_LEN; //接收缓冲区字符长度
extern char USART1_RX_BUF[MAXSIZE];
extern unsigned char USART1_TX_BUF[MAXSIZE];

void Usart1_Init();

