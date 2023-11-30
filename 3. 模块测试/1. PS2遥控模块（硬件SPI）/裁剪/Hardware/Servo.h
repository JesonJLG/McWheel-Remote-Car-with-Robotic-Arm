#ifndef _SERVO_H_
#define _SERVO_H_

#include "stm32f10x.h"
/*------------添加引脚宏定义-------------*/
//#define	


typedef enum
{
	SERVO_MIDDLE = 0,
	SERVO_LEFT_LIMIT,
	SERVO_RIGHT_LIMIT,
}servo_limit;

struct SERVO_PARAM
{
	uint8_t pwm;
	uint8_t left_limit;
	uint8_t middle;
	uint8_t right_limit;
};

void Servo_Init(void);
uint8_t Arc_Servo_Param_Get(uint8_t servo_param);
void Arc_ServoPwm_Set(uint16_t value);
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

