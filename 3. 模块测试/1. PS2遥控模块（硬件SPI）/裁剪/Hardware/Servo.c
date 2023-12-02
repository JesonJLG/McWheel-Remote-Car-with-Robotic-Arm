#include "Servo.h"

struct SERVO_PARAM robotarm_left = {200, 130, 200, 250};
struct SERVO_PARAM robotarm_up = {150, 100, 150, 250};
struct SERVO_PARAM robotarm_right = {180, 120, 180, 250};
void Servo_Init()
{
    Robot_Arm_Gpio_Init();
    Servo_Time_Init();

    Robot_LeftPwm_Set(robotarm_left.middle);
    Robot_RightPwm_Set(robotarm_right.middle);
    Robot_UpPwm_Set(robotarm_up.middle);
}

void RobotArm_RaiseHand(uint8_t unit_pwm)
{
    unit_pwm = robotarm_left.pwm + unit_pwm;
    Robot_LeftPwm_Set(unit_pwm);
}

void RobotArm_DropHand(uint8_t unit_pwm)
{
    unit_pwm = robotarm_left.pwm - unit_pwm;
    Robot_LeftPwm_Set(unit_pwm);
}

void RobotArm_StrechHand(uint8_t unit_pwm)
{
    unit_pwm = robotarm_right.pwm + unit_pwm;
    Robot_RightPwm_Set(unit_pwm);
}

void RobotArm_ShinkHand(uint8_t unit_pwm)
{
    unit_pwm = robotarm_right.pwm - unit_pwm;
    Robot_RightPwm_Set(unit_pwm);
}

void RobotArm_ShakeHand(uint8_t unit_pwm)
{
    unit_pwm = robotarm_up.pwm + unit_pwm;
    Robot_UpPwm_Set(unit_pwm);
}

void RobotArm_LetHand(uint8_t unit_pwm)
{
    unit_pwm = robotarm_up.pwm - unit_pwm;
    Robot_UpPwm_Set(unit_pwm);
}
/*************************************/
void Robot_Arm_Gpio_Init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;  //左舵机 前舵机 右舵机
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
}

void Servo_Time_Init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    TIM_InternalClockConfig(TIM1);
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 10-1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);

    TIM_ClearFlag(TIM1, TIM_FLAG_Update);
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM1, ENABLE);
}

void Robot_LeftPwm_Set(uint16_t value)
{
    //限幅
    if (value < robotarm_left.left_limit)
    {
        value = robotarm_left.left_limit;
    }
    else if (value > robotarm_left.right_limit)
    {
        value = robotarm_left.right_limit;
    }

    robotarm_left.pwm = value;
}

void Robot_UpPwm_Set(uint16_t value)
{
    //限幅
    if (value < robotarm_up.left_limit)
    {
        value = robotarm_up.left_limit;
    }
    else if (value > robotarm_up.right_limit)
    {
        value = robotarm_up.right_limit;
    }

    robotarm_up.pwm = value;
}

void Robot_RightPwm_Set(uint16_t value)
{
    //限幅
    if (value < robotarm_right.left_limit)
    {
        value = robotarm_right.left_limit;
    }
    else if (value > robotarm_right.right_limit)
    {
        value = robotarm_right.right_limit;
    }

    robotarm_right.pwm = value;
}

uint16_t counter;
void TIM1_UP_IRQHandler()
{
    if (TIM_GetITStatus(TIM1,TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM1,TIM_IT_Update);

        counter++;
        if (counter > 2000)
        {
            counter = 0;
        }

        //机械臂左舵机
        if (counter < robotarm_left.pwm)
        {
            GPIO_SetBits(GPIOB,GPIO_Pin_13);
        }
        else
        {
            GPIO_ResetBits(GPIOB,GPIO_Pin_13);
        }

        //机械臂前舵机
        if (counter < robotarm_up.pwm)
        {
            GPIO_SetBits(GPIOB,GPIO_Pin_14);
        }
        else
        {
            GPIO_ResetBits(GPIOB,GPIO_Pin_14);
        }

        //机械臂右舵机
        if (counter < robotarm_right.pwm)
        {
            GPIO_SetBits(GPIOB,GPIO_Pin_15);
        }
        else
        {
            GPIO_ResetBits(GPIOB,GPIO_Pin_15);
        }


    }
}
