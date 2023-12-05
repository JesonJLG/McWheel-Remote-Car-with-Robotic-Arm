#include "led.h"

/*********************************************************
Copyright (C), 2015-2025, YFRobot.
www.yfrobot.com
File：LED驱动代码
Author：pinggai    Version:1.0     Data:2015/05/06
Description: Balance-STM32 LED驱动代码
**********************************************************/

//初始化PC13输出口.并使端口时钟
//LED IO初始化
void LED_Init(void)
{
    RCC->APB2ENR|=1<<4; 		//使能 PORT 时钟
    GPIOC->CRH&=0XFF0FFFFF;	//
    GPIOC->CRH|=0X00300000; //推挽输出
    GPIOC->ODR|=1<<13; 			// 输出高
}




