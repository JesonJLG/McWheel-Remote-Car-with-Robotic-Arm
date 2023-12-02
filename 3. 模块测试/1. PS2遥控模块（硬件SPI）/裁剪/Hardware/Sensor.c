#include "Sensor.h"

/*
	SENSOR0(B1)  SENSOR1(B0)  SENSOR2(B4)  SENSOR3(B5)
*/
void Sensor_Init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

uint8_t Sensor0_Get_State()
{
    return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1);
}

uint8_t Sensor1_Get_State()
{
    return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0);
}

uint8_t Sensor2_Get_State()
{
    return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4);
}

uint8_t Sensor3_Get_State()
{
    return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5);
}

