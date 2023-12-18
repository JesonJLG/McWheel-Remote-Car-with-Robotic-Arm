#include "stm32f10x.h"                  // Device header
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "ps2.h"
#include "led.h"
#include "motor.h"
#include "servo.h"

//#define TEST_PS2_HardSPI
//#define TEST_MOTOR_McWheel
#define TEST_SERVO_HardPWM
#define UNIT_PWM 3

uint8_t ps2_lx, ps2_ly, ps2_rx, ps2_ry,ps2_keynum;
uint8_t ps2_mode, last_ps2_mode;
uint8_t temp;

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init();
    Uart1_init(115200); /*Э�鴦��ӿ� */
    LED_Init();
    DBG_PRINTF("1111111111111111\r\n");
    PS2_Init();
    Motor_Init();
    DBG_PRINTF("2222222222222222\r\n");
	
    while (1)
    {
#ifdef TEST_PS2_HardSPI
        ////////////////////////////////////////////////////////
        //motor1:�Ҳ�С�𶯵�� 0x00�أ�������
        //motor2:�����𶯵�� 0x40~0xFF �������ֵԽ�� ��Խ��
        ////////////////////////////////////////////////////////
        PS2_KeyNum = PS2_DataKey();
        if(PS2_KeyNum!=0)                   //�а�������
        {
            printf("  \r\n   %d  is  light  \r\n",Data[1]);//ID
            printf("  \r\n   %d  is  pressed  \r\n",PS2_KeyNum);
            if(PS2_KeyNum == 11)//��L1
            {
                PS2_Vibration(0x00,0x41);//moter2������
                delay_ms(500);//�����𶯺��������ʱ  delay_ms(1000);
            }
            else if(PS2_KeyNum == 12)//��R1
            {
                PS2_Vibration(0x01,0x00);//moter1�Ҳ�С��
                delay_ms(500);//�����𶯺��������ʱ  delay_ms(1000);
            }
            else
                PS2_Vibration(0x00,0x00);
        }
        PS2_LX=ps2_get_anolog_data(PSS_LX);
        PS2_LY=ps2_get_anolog_data(PSS_LY);
        PS2_RX=ps2_get_anolog_data(PSS_RX);
        PS2_RY=ps2_get_anolog_data(PSS_RY);

        printf("PS2����:%d		",PS2_KeyNum);   //��ӡ������ֵ
        printf("PS2��X��:%d		",PS2_LX);   //��ӡ���X���ֵ
        printf("PS2��Y��:%d		",PS2_LY);   //��ӡ�ұ�Y���ֵ
        printf("PS2��X��:%d		",PS2_RX);   //��ӡ���X���ֵ
        printf("PS2��Y��:%d		",PS2_RY);   //��ӡ�ұ�Y���ֵ
        printf("\r\n");   //����
        delay_ms(50);
#endif

#ifdef TEST_MOTOR_McWheel
        ps2_keynum = ps2_key_serch();
        ps2_mode = ps2_mode_get();

        if (ps2_mode != last_ps2_mode)
        {
            Car_Forward(0);
        }

        if (ps2_mode == PSB_REDLIGHT_MODE)
        {
            PS2_LX = ps2_get_anolog_data(PSS_LX);
            PS2_LY = ps2_get_anolog_data(PSS_LY);

            if ( PS2_LY == 0x00)
            {
                Car_Forward(1000);
            }
            else if (PS2_LY == 0xff)
            {
                Car_Backward(1000);
            }
            else if (PS2_LX == 0x00)
            {
                Car_TransLeft(1000);
            }
            else if (PS2_LX == 0xff)
            {
                Car_TransRight(1000);
            }
            else
            {
                if (ps2_get_key_state(PSB_L1))
                {
                    Car_TurnLeft(1000);
                }
                else if (ps2_get_key_state(PSB_L2))
                {
                    Car_TurnRight(1000);
                }
                else
                {
                    Car_Forward(0);
                }
            }
        }
		delay_ms(20);
#endif
#ifdef TEST_SERVO_HardPWM
        ps2_keynum = ps2_key_serch();
        ps2_mode = ps2_mode_get();

        if (ps2_mode != last_ps2_mode) {
            DBG_PRINTF("PS2ģʽ�б䣬ͣ����\r\n");
            Car_Forward(0);
        }

        if (ps2_mode == PSB_REDLIGHT_MODE) {
            DBG_PRINTF("���ģʽ��ģ�������\r\n");
            unsigned char ps2_lx, ps2_ly, ps2_rx, ps2_ry;

            ps2_lx = ps2_get_anolog_data(PSS_LX);
            ps2_ly = ps2_get_anolog_data(PSS_LY);
            if (ps2_lx <= 0xFF) {
                DBG_PRINTF("ps2_lx:%d\r\n", ps2_lx);
            } else {
                DBG_PRINTF("ps2_lxֵ�쳣");
            }
            if (ps2_ly <= 0xFF) {
                DBG_PRINTF("ps2_ly:%d\r\n", ps2_lx);
            } else {
                DBG_PRINTF("ps2_lyֵ�쳣");
            }

            if (ps2_ly == 0x00) {
                Car_Forward(1000);
            } else if (ps2_ly == 0xff) {
                Car_Backward(1000);
            } else if (ps2_lx == 0x00) {
                Car_TurnLeft(1000);
            } else if (ps2_lx == 0xff) {
                Car_TurnRight(1000);
            } else {
                if (ps2_get_key_state(PSB_L1)) {
                    Car_TurnLeft(1000);
                } else if (ps2_get_key_state(PSB_L2)) {
                    Car_TurnRight(1000);
                } else {
                    Car_Forward(0);
                }
            }

            //��е�ۿ���
            ps2_rx = ps2_get_anolog_data(PSS_RX);
            if (ps2_rx == 0xFF) {
                RobotArm_RaiseHand(UNIT_PWM);
            } else if (ps2_rx == 0x00) {
                RobotArm_DropHand(UNIT_PWM);
            }

            ps2_ry = ps2_get_anolog_data(PSS_RY);
            if (ps2_ry == 0x00) {
                RobotArm_StrechHand(UNIT_PWM);
            } else if (ps2_ry == 0xFF) {
                RobotArm_ShinkHand(UNIT_PWM);
            }

            if (ps2_get_key_state(PSB_R1)) {
                RobotArm_ShakeHand(UNIT_PWM);
            } else if (ps2_get_key_state(PSB_R2)) {
                RobotArm_LetHand(UNIT_PWM);
            }
        } else if (ps2_mode == PSB_GREENLIGHT_MODE) {
            DBG_PRINTF("�̵�ģʽ\r\n");
            if (ps2_keynum) {
                if (ps2_get_key_state(PSB_PAD_UP)) {
                    DBG_PRINTF("PSB_PAD_UP Pressed!\r\n");
                    Car_Forward(1000);
                } else if (ps2_get_key_state(PSB_PAD_DOWN)) {
                    DBG_PRINTF("PSB_PAD_DOWN Pressed!\r\n");
                    Car_Backward(1000);
                } else if (ps2_get_key_state(PSB_PAD_LEFT)) {
                    DBG_PRINTF("PSB_PAD_LEFT Pressed!\r\n");
                    Car_TurnLeft(1000);
                } else if (ps2_get_key_state(PSB_PAD_RIGHT)) {
                    DBG_PRINTF("PSB_PAD_RIGHT Pressed!\r\n");
                    Car_TurnRight(1000);
                } else if (ps2_get_key_state(PSB_L1)) {
                    DBG_PRINTF("PSB_L1 Pressed!\r\n");
                    Car_TurnLeft(1000);
                } else if (ps2_get_key_state(PSB_L2)) {
                    DBG_PRINTF("PSB_L2 Pressed!\r\n");
                    Car_TurnRight(1000);
                } else {
                    DBG_PRINTF("Car_Forward(0)\r\n");
                    Car_Forward(0);
                }

                //��е�ۿ���
                if (ps2_get_key_state(PSB_RED)) {
                    DBG_PRINTF("��е�ۿ���--PSB_RED\r\n");
                    RobotArm_RaiseHand(UNIT_PWM);
                } else if (ps2_get_key_state(PSB_PINK)) {
                    RobotArm_DropHand(UNIT_PWM);
                } else if (ps2_get_key_state(PSB_GREEN)) {
                    RobotArm_StrechHand(UNIT_PWM);
                } else if (ps2_get_key_state(PSB_BLUE)) {
                    RobotArm_ShinkHand(UNIT_PWM);
                } else if (ps2_get_key_state(PSB_R1)) {
                    RobotArm_ShakeHand(UNIT_PWM);
                } else if (ps2_get_key_state(PSB_R2)) {
                    RobotArm_LetHand(UNIT_PWM);
                } else {
                }
            } else {
                Car_Forward(0);
            }
        } else {
            Car_Forward(0);
        }
        last_ps2_mode = ps2_mode;
        delay_ms(20);		
#endif
	}

}
