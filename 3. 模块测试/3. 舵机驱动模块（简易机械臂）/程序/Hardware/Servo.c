#include "Servo.h"
#include "usart.h"

struct SERVO_PARAM robotarm_left = {200, 130+20, 200, 250-20};
struct SERVO_PARAM robotarm_up = {150, 100+20, 150, 250-20};
struct SERVO_PARAM robotarm_right = {180, 120+20, 180, 250-20};

void Servo_Init()
{
    Robot_Arm_Gpio_Init();
    Servo_Time_Init();

    Robot_LeftPwm_Set(robotarm_left.middle);
    Robot_RightPwm_Set(robotarm_right.middle);
    Robot_UpPwm_Set(robotarm_up.middle);
}

void Robot_Arm_Gpio_Init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出！！！（md 把这漏掉了！！！）
    GPIO_InitStructure.GPIO_Pin = SERVO_FRONT|SERVO_LIFT|SERVO_RIGHT;  //左舵机 前舵机 右舵机
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SERVO_PORT, &GPIO_InitStructure);
}

void Servo_Time_Init()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    TIM_InternalClockConfig(SERVO_TIM);
	
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);			//初始化默认结构体
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;	//PSC	72M/720/2000 = 50Hz = 1/50s = 20ms
    TIM_TimeBaseInitStructure.TIM_Period = 2000 - 1;	//ARR
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//RDR
    TIM_TimeBaseInit(SERVO_TIM, &TIM_TimeBaseInitStructure);

    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);						//初始化默认结构体
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse =0;    //CCR
	
	TIM_OC3Init(SERVO_TIM,&TIM_OCInitStructure);
	TIM_OC2Init(SERVO_TIM,&TIM_OCInitStructure);
	TIM_OC1Init(SERVO_TIM,&TIM_OCInitStructure);

    TIM_Cmd(SERVO_TIM, ENABLE);
}


void Robot_UpPwm_Set(uint16_t pwm_value)	//前舵机	TIM4_CH3
{
    //限幅
    if (pwm_value < robotarm_up.left_limit)
    {
        pwm_value = robotarm_up.left_limit;
    }
    else if (pwm_value > robotarm_up.right_limit)
    {
        pwm_value = robotarm_up.right_limit;
    }

    robotarm_up.pwm = pwm_value;
	TIM_SetCompare3(SERVO_TIM, robotarm_up.pwm);
}

void Robot_LeftPwm_Set(uint16_t pwm_value)	//左舵机	TIM4_CH2
{
    //限幅
    if (pwm_value < robotarm_left.left_limit)
    {
        pwm_value = robotarm_left.left_limit;
    }
    else if (pwm_value > robotarm_left.right_limit)
    {
        pwm_value = robotarm_left.right_limit;
    }

    robotarm_left.pwm = pwm_value;	//更新到结构体中
	TIM_SetCompare2(SERVO_TIM, robotarm_left.pwm);
}

void Robot_RightPwm_Set(uint16_t pwm_value)	//右舵机	TIM4_CH1
{
	//限幅
	if (pwm_value < robotarm_right.left_limit)
	{
		pwm_value = robotarm_right.left_limit;
	}
	else if (pwm_value > robotarm_right.right_limit)
	{
		pwm_value = robotarm_right.right_limit;
	}
	
	robotarm_right.pwm = pwm_value;
	TIM_SetCompare1(SERVO_TIM, robotarm_right.pwm);
}

/*------------简易机械臂控制算法-------------*/
void RobotArm_RaiseHand(uint8_t unit_pwm)	//抬手
{	
//    unit_pwm = robotarm_left.pwm + unit_pwm;
//    Robot_LeftPwm_Set(unit_pwm);
//	DBG_PRINTF("unit_pwm:%d", unit_pwm)
	robotarm_left.pwm += unit_pwm;
	Robot_LeftPwm_Set(robotarm_left.pwm);
	DBG_PRINTF("robotarm_left.pwm:%d", robotarm_left.pwm)
}

void RobotArm_DropHand(uint8_t unit_pwm)	//放手
{
    robotarm_left.pwm -= unit_pwm;
    Robot_LeftPwm_Set(robotarm_left.pwm);
	DBG_PRINTF("robotarm_left.pwm:%d", robotarm_left.pwm)
}

void RobotArm_StrechHand(uint8_t unit_pwm)	//伸手
{
    robotarm_right.pwm += unit_pwm;
    Robot_RightPwm_Set(robotarm_right.pwm);
	DBG_PRINTF("robotarm_right.pwm:%d", robotarm_right.pwm)
}

void RobotArm_ShinkHand(uint8_t unit_pwm)	//缩手
{
    robotarm_right.pwm -= unit_pwm;
    Robot_RightPwm_Set(robotarm_right.pwm);
	DBG_PRINTF("robotarm_right.pwm:%d", robotarm_right.pwm)
}

void RobotArm_ShakeHand(uint8_t unit_pwm)	//夹爪
{
    robotarm_up.pwm += unit_pwm;
    Robot_UpPwm_Set(robotarm_up.pwm);
	DBG_PRINTF("robotarm_up.pwm:%d", robotarm_up.pwm)
}

void RobotArm_LetHand(uint8_t unit_pwm)		//松爪
{
    robotarm_up.pwm -= unit_pwm;
    Robot_UpPwm_Set(robotarm_up.pwm);
	DBG_PRINTF("robotarm_up.pwm:%d", robotarm_up.pwm)
}

