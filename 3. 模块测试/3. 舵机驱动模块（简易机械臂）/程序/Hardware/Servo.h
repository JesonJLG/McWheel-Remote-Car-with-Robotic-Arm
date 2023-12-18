#ifndef _SERVO_H_
#define _SERVO_H_

#include "stm32f10x.h"
/*------------添加引脚宏定义-------------*/
/*
前舵机（夹爪开合）		PB8 	TIM4_CH3
左舵机（水平轴运动）	PB7		TIM4_CH2
右舵机（竖直轴运动）	PB6		TIM4_CH1
*/
#define SERVO_PORT		GPIOB
#define SERVO_TIM		TIM4

#define SERVO_FRONT		GPIO_Pin_8		//PB8
#define SERVO_LIFT		GPIO_Pin_7		//PB7
#define SERVO_RIGHT		GPIO_Pin_6		//PB6

#define SERVO_ACMAN		GPIO_Pin_9		//PB9




typedef enum
{
    SERVO_MIDDLE = 0,
    SERVO_LEFT_LIMIT,
    SERVO_RIGHT_LIMIT,
} servo_limit;

struct SERVO_PARAM
{
    uint8_t pwm;
    uint8_t left_limit;
    uint8_t middle;
    uint8_t right_limit;
};

void Servo_Init(void);

void Servo_Gpio_Init(void);
void Servo_Time_Init(void);
void Robot_Arm_Gpio_Init(void);

void Robot_LeftPwm_Set(uint16_t value);
void Robot_UpPwm_Set(uint16_t value);
void Robot_RightPwm_Set(uint16_t value);

void RobotArm_RaiseHand(uint8_t unit_pwm);
void RobotArm_DropHand(uint8_t unit_pwm);
void RobotArm_StrechHand(uint8_t unit_pwm);
void RobotArm_ShinkHand(uint8_t unit_pwm);
void RobotArm_ShakeHand(uint8_t unit_pwm);
void RobotArm_LetHand(uint8_t unit_pwm);
#endif

