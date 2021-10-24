#pragma once
#include <stm32f4xx.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define MAXSIZE 1000
extern unsigned int USART3_RX_LEN; //接收缓冲区字符长度
extern char USART3_RX_BUF[MAXSIZE];
extern unsigned char USART3_TX_BUF[MAXSIZE];

void Usart3_Init();

void printf3(char *s, ...);