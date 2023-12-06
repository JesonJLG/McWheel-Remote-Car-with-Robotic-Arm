#ifndef __MYSPI_H
#define __MYSPI_H

#include "stm32f10x.h"
/*------------������ź궨��-------------*/
#define PS2_PORT		GPIOB
#define PS2_CS_SEL		GPIO_Pin_12		//PB12
#define PS2_CLK			GPIO_Pin_13		//PB13
#define PS2_DO_CMD		GPIO_Pin_15		//PB15(MOSI)	STM32(MOSI) -> �ֱ�(DI)	
#define PS2_DI_DAT		GPIO_Pin_14		//PB14(MISO)	STM32(MISO) <- �ֱ�(DO)
/*------------Ӳ��SPI����һ����Ҫ�ӷ����мǣ�-------------*/
/*************************************
���﷢�������ҵ��������ʧ��
�ҵ�����������������
�޷�ͨѶ--PS2ת�������Ƿ��ز���ID�����ص�ȫ��0xFF������
����֣�����
Ӳ��SPI�Ľ��ߴ��ˣ�����
��ϸһ��
Ŷ~~
��STM32��MOSI����Ҫ����PS2ת�Ӱ��MISO���ţ������ͨ��
����
�����һ��������Ϊ
�������ӻ��Ľӷ�����MOSI--MOSI	MISO--MISO
��������ת�Ӱ�ı�ע���ˣ�Ҫô���Ǳ�ע�������Լ����뷨
*************************************/

void MySPI_Init(void);
void MySPI_Start(void);
void MySPI_Stop(void);
uint8_t MySPI_SwapByte(uint8_t ByteSend);

#endif
