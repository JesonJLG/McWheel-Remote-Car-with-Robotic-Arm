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
    Uart1_init(115200); /*协议处理接口 */
    LED_Init();
    DBG_PRINTF("1111111111111111\r\n");
	PS2_Init();
	DBG_PRINTF("2222222222222222\r\n");

    while (1)
    {
#ifdef TEST0
        ////////////////////////////////////////////////////////
        //motor1:右侧小震动电机 0x00关，其他开
        //motor2:左侧大震动电机 0x40~0xFF 电机开，值越大 震动越大
        ////////////////////////////////////////////////////////
        //LED =! LED;
        PS2_KeyNum = PS2_DataKey();
        if(PS2_KeyNum!=0)                   //有按键按下
        {
            printf("  \r\n   %d  is  light  \r\n",Data[1]);//ID
            printf("  \r\n   %d  is  pressed  \r\n",PS2_KeyNum);
            if(PS2_KeyNum == 11)//左L1
            {
                PS2_Vibration(0x00,0x41);//moter2左侧大震动
                delay_ms(500);//发出震动后必须有延时  delay_ms(1000);
            }
            else if(PS2_KeyNum == 12)//右R1
            {
                PS2_Vibration(0x01,0x00);//moter1右侧小震动
                delay_ms(500);//发出震动后必须有延时  delay_ms(1000);
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
            break;  //按键按下，LED状态（PC13，低电平点亮）反转
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
        printf("PS2按键:%d		",PS2_KeyNum);   //打印按键的值
        printf("PS2左X轴:%d		",PS2_LX);   //打印左边X轴的值
        printf("PS2左Y轴:%d		",PS2_LY);   //打印右边Y轴的值
        printf("PS2右X轴:%d		",PS2_RX);   //打印左边X轴的值
        printf("PS2右Y轴:%d		",PS2_RY);   //打印右边Y轴的值
        printf("\r\n");   //换行

        delay_ms(100);										   //延时100m秒钟
#endif
    }

}
