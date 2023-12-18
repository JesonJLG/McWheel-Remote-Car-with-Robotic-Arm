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

void Robot_Arm_Gpio_Init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = SERVO_FRONT|SERVO_LIFT|SERVO_RIGHT;  //���� ǰ��� �Ҷ��
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SERVO_PORT, &GPIO_InitStructure);

    GPIO_SetBits(SERVO_PORT,SERVO_FRONT|SERVO_LIFT|SERVO_RIGHT);
}

void Servo_Time_Init()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    TIM_InternalClockConfig(SERVO_TIM);
	
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);			//��ʼ��Ĭ�Ͻṹ��
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;	//PSC
    TIM_TimeBaseInitStructure.TIM_Period = 2000 - 1;	//ARR
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//RDR
    TIM_TimeBaseInit(SERVO_TIM, &TIM_TimeBaseInitStructure);

    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);						//��ʼ��Ĭ�Ͻṹ��
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse =0;    //CCR
	
	TIM_OC3Init(SERVO_TIM,&TIM_OCInitStructure);
	TIM_OC2Init(SERVO_TIM,&TIM_OCInitStructure);
	TIM_OC1Init(SERVO_TIM,&TIM_OCInitStructure);

    TIM_Cmd(SERVO_TIM, ENABLE);
}


void Robot_UpPwm_Set(uint16_t value)	//ǰ���	TIM4_CH3
{
    //�޷�
    if (value < robotarm_up.left_limit)
    {
        value = robotarm_up.left_limit;
    }
    else if (value > robotarm_up.right_limit)
    {
        value = robotarm_up.right_limit;
    }

    robotarm_up.pwm = value;
	TIM_SetCompare1(SERVO_TIM, robotarm_up.pwm);
}

void Robot_LeftPwm_Set(uint16_t value)	//����	TIM4_CH2
{
    //�޷�
    if (value < robotarm_left.left_limit)
    {
        value = robotarm_left.left_limit;
    }
    else if (value > robotarm_left.right_limit)
    {
        value = robotarm_left.right_limit;
    }

    robotarm_left.pwm = value;
	TIM_SetCompare2(SERVO_TIM, robotarm_left.pwm);
}

void Robot_RightPwm_Set(uint16_t value)	//�Ҷ��	TIM4_CH1
{
	//�޷�
	if (value < robotarm_right.left_limit)
	{
		value = robotarm_right.left_limit;
	}
	else if (value > robotarm_right.right_limit)
	{
		value = robotarm_right.right_limit;
	}
	
	robotarm_right.pwm = value;
	TIM_SetCompare3(SERVO_TIM, robotarm_right.pwm);
}

/*------------���׻�е�ۿ����㷨-------------*/
void RobotArm_RaiseHand(uint8_t unit_pwm)	//̧��
{	
    unit_pwm = robotarm_left.pwm + unit_pwm;
    Robot_LeftPwm_Set(unit_pwm);
}

void RobotArm_DropHand(uint8_t unit_pwm)	//����
{
    unit_pwm = robotarm_left.pwm - unit_pwm;
    Robot_LeftPwm_Set(unit_pwm);
}

void RobotArm_StrechHand(uint8_t unit_pwm)	//����
{
    unit_pwm = robotarm_right.pwm + unit_pwm;
    Robot_RightPwm_Set(unit_pwm);
}

void RobotArm_ShinkHand(uint8_t unit_pwm)	//����
{
    unit_pwm = robotarm_right.pwm - unit_pwm;
    Robot_RightPwm_Set(unit_pwm);
}

void RobotArm_ShakeHand(uint8_t unit_pwm)	//��צ
{
    unit_pwm = robotarm_up.pwm + unit_pwm;
    Robot_UpPwm_Set(unit_pwm);
}

void RobotArm_LetHand(uint8_t unit_pwm)		//��צ
{
    unit_pwm = robotarm_up.pwm - unit_pwm;
    Robot_UpPwm_Set(unit_pwm);
}

