#include "led.h"

/*********************************************************
Copyright (C), 2015-2025, YFRobot.
www.yfrobot.com
File��LED��������
Author��pinggai    Version:1.0     Data:2015/05/06
Description: Balance-STM32 LED��������
**********************************************************/	   

//��ʼ��PC13�����.��ʹ�˿�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
	RCC->APB2ENR|=1<<4; 		//ʹ�� PORT ʱ��  
	GPIOC->CRH&=0XFF0FFFFF;	//
	GPIOC->CRH|=0X00300000; //�������
	GPIOC->ODR|=1<<13; 			// �����	
}




