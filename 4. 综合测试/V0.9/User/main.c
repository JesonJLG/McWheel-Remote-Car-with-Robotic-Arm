#include "stm32f10x.h"                  // Device header
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "ps2.h"
#include "led.h"
#include "motor.h"
#include "servo.h"

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
    PS2_Init();
    Motor_Init();
	Servo_Init();	//���˰��죬����������أ����ǳ�ʼ�������죡����
	LED = 0;
    while (1)
    {
        // ��ȡ���µİ���ֵ��ģʽ
		ps2_mode = ps2_mode_get();
		ps2_keynum = ps2_key_serch();
        //ps2_keynum = PS2_DataKey();
		ps2_lx=ps2_get_anolog_data(PSS_LX);
        ps2_ly=ps2_get_anolog_data(PSS_LY);
        ps2_rx=ps2_get_anolog_data(PSS_RX);
        ps2_ry=ps2_get_anolog_data(PSS_RY);
		
//		  printf("PS2ģʽ:%d 	",ps2_mode);
//        printf("PS2����:%d	",ps2_keynum);   //��ӡ������ֵ
//        printf("PS2��X��:%d	",ps2_lx);   //��ӡ���X���ֵ
//        printf("PS2��Y��:%d	",ps2_ly);   //��ӡ�ұ�Y���ֵ
//        printf("PS2��X��:%d	",ps2_rx);   //��ӡ���X���ֵ
//        printf("PS2��Y��:%d	",ps2_ry);   //��ӡ�ұ�Y���ֵ
//        printf("\r\n");   //����
        

        // ���ģʽ�Ƿ����仯
        if (ps2_mode != last_ps2_mode) {
			printf("���ģʽ�Ƿ����仯");
            //Car_Forward(0);	//Ҳ����˵PWMռ�ձ�Ϊ0 ���͵�ƽ ��������������� ����1 ����ת��
        }

        // ����Ƿ�Ϊ���ģʽ
        if (ps2_mode == PSB_REDLIGHT_MODE) {	LED = 0;
            // ��ȡPS2��������ģ������
            ps2_lx = ps2_get_anolog_data(PSS_LX);
            ps2_ly = ps2_get_anolog_data(PSS_LY);

            // ����ģ�����ݿ���С��
            if (ps2_ly == 0x00) {
                Car_Forward(1000);
            } else if (ps2_ly == 0xff) {
                Car_Backward(1000);
            } else if (ps2_lx == 0x00) {
                Car_TransLeft(1000);
            } else if (ps2_lx == 0xff) {
                Car_TransRight(1000);
            } else {
                if (ps2_get_key_state(PSB_L1)) {
                    Car_TurnLeft(1000);
                } else if (ps2_get_key_state(PSB_L2)) {
                    Car_TurnRight(1000);
                } else {
                    //Car_Forward(0);
                }
            }

            // ����ģ�����ݿ��ƻ�е��
            ps2_rx = ps2_get_anolog_data(PSS_RX);
            if (ps2_rx == 0xFF) {
				DBG_PRINTF("̧��");
                RobotArm_RaiseHand(UNIT_PWM);	//̧��
            } else if (ps2_rx == 0x00) {
				DBG_PRINTF("����");
                RobotArm_DropHand(UNIT_PWM);	//����
            }

            ps2_ry = ps2_get_anolog_data(PSS_RY);
            if (ps2_ry == 0x00) {
				DBG_PRINTF("��չ");
                RobotArm_StrechHand(UNIT_PWM);	//��չ
            } else if (ps2_ry == 0xFF) {
				DBG_PRINTF("����");
                RobotArm_ShinkHand(UNIT_PWM);	//����
            }

            // ���ݰ���״̬���ƻ�еצ
            if (ps2_get_key_state(PSB_R1)) {
				DBG_PRINTF("��צ");
                RobotArm_ShakeHand(UNIT_PWM);	//��צ
            } else if (ps2_get_key_state(PSB_R2)) {
				DBG_PRINTF("��צ");
                RobotArm_LetHand(UNIT_PWM);		//��צ
            }
        } 
        else {
            // ������Ǻ��ģʽ��ֹͣС��
            //Car_Forward(0);
			LED = 0;
			delay_ms(1000);
			LED = 1;
			delay_ms(1000);
        }
        // ������һ�ε�PS2ģʽ
        last_ps2_mode = ps2_mode;
        // �ӳ�һ��ʱ�䣬�Ա�PS2��������Ӧ
        delay_ms(10);
    }

}
