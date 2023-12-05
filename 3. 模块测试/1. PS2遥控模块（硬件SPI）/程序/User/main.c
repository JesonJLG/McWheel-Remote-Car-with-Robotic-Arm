#include "stm32f10x.h"                  // Device header
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "ps2.h"
#include "led.h"

#define TEST0
//#define TEST1
int PS2_LX,PS2_LY,PS2_RX,PS2_RY,PS2_KEY;

uint8_t PS2_KeyNum;
uint8_t PS2_Mode, Last_PS2_Mode;
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init();
    Uart1_init(115200); /*Э�鴦��ӿ� */
    LED_Init();
    DBG_PRINTF("1111111111111111\r\n");
	PS2_Init();
	DBG_PRINTF("2222222222222222\r\n");

    while (1)
    {
#ifdef TEST0
        ////////////////////////////////////////////////////////
        //motor1:�Ҳ�С�𶯵�� 0x00�أ�������
        //motor2:�����𶯵�� 0x40~0xFF �������ֵԽ�� ��Խ��
        ////////////////////////////////////////////////////////
        //LED =! LED;
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
        printf(" %5d %5d %5d %5d\r\n",ps2_get_anolog_data(PSS_LX),ps2_get_anolog_data(PSS_LY),
               ps2_get_anolog_data(PSS_RX),ps2_get_anolog_data(PSS_RY) );
        delay_ms(50);
#endif

#ifdef TEST1
        PS2_LX=ps2_get_anolog_data(PSS_LX);
        PS2_LY=ps2_get_anolog_data(PSS_LY);
        PS2_RX=ps2_get_anolog_data(PSS_RX);
        PS2_RY=ps2_get_anolog_data(PSS_RY);
        PS2_KeyNum=PS2_DataKey();
        switch(PS2_KeyNum)
        {
        case 1:
            LED = !LED;
            break;  //�������£�LED״̬��PC13���͵�ƽ��������ת
        case 2:
            LED = !LED;
            break;
        case 3:
            LED = !LED;
            break;
        case 4:
            LED = !LED;
            break;
        case 5:
            LED = !LED;
            break;
        case 6:
            LED = !LED;
            break;
        case 7:
            LED = !LED;
            break;
        case 8:
            LED = !LED;
            break;
        case 9:
            LED = !LED;
            break;
        case 10:
            LED = !LED;
            break;
        case 11:
            LED = !LED;
            break;
        case 12:
            LED = !LED;
            break;
        case 13:
            LED = !LED;
            break;
        case 14:
            LED = !LED;
            break;
        case 15:
            LED = !LED;
            break;
        case 16:
            LED = !LED;
            break;
        }
        printf("PS2����:%d		",PS2_KeyNum);   //��ӡ������ֵ
        printf("PS2��X��:%d		",PS2_LX);   //��ӡ���X���ֵ
        printf("PS2��Y��:%d		",PS2_LY);   //��ӡ�ұ�Y���ֵ
        printf("PS2��X��:%d		",PS2_RX);   //��ӡ���X���ֵ
        printf("PS2��Y��:%d		",PS2_RY);   //��ӡ�ұ�Y���ֵ
        printf("\r\n");   //����

        delay_ms(100);										   //��ʱ100m����
#endif
    }

}
