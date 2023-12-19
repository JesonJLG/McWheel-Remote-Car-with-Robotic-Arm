#ifndef __USART_H
#define __USART_H
#include "stm32f10x.h"                  // Device header
#include <stdio.h>

//V1.3修改说明
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持


#define  DEBUG_USARTx                   USART1
#define USART_REC_LEN  			200  	//定义最大接收字节数 200

#define DEBUG   1

/*------------串口调试输出-------------*/
#if DEBUG
#define DBG_PRINTF(fmt, args...)  \
		{\
			printf("<<File:%s  Line:%d  Function:%s>> ", __FILE__, __LINE__, __FUNCTION__);\
			printf(fmt, ##args);\
			printf("\r\n");\
		}
#else

#endif
/*------------END-------------*/

extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern u16 USART_RX_STA;         		//接收状态标记

void Uart1_init(u32 bound);


#endif



