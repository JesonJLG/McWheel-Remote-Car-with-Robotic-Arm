#ifndef __USART_H
#define __USART_H
#include "stm32f10x.h"                  // Device header
#include <stdio.h>

//V1.3�޸�˵��
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��


#define  DEBUG_USARTx                   USART1
#define USART_REC_LEN  			200  	//�����������ֽ��� 200

#define DEBUG   1

/*------------���ڵ������-------------*/
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

extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
extern u16 USART_RX_STA;         		//����״̬���

void Uart1_init(u32 bound);


#endif



