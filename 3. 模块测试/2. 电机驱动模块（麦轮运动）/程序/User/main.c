#include "stm32f10x.h"                  // Device header
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "ps2.h"
#include "led.h"
#include "motor.h"

//#define TEST_PS2_HardSPI
#define TEST_MOTOR_McWheel
int PS2_LX, PS2_LY, PS2_RX, PS2_RY, PS2_KEY;

uint8_t PS2_KeyNum;
uint8_t PS2_Mode, Last_PS2_Mode;
uint8_t temp;
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init();
    Uart1_init(115200); /*协议处理接口 */
    LED_Init();
    DBG_PRINTF("1111111111111111\r\n");
    PS2_Init();
    Motor_Init();
    DBG_PRINTF("2222222222222222\r\n");
	
//	PS2_ReadData();
//	for(int i=0; i<10; i++)
//	DBG_PRINTF("Data[%i]：%x\r\n", i, Data[i]);
	
//	{
//		delay_ms(1000);
//		delay_ms(1000);
//		delay_ms(1000);
//		LED=0;
//		Car_Forward(500);
//		delay_ms(1000);
//		delay_ms(1000);
//		delay_ms(1000);
//		Car_Backward(500);
//		delay_ms(1000);
//		delay_ms(1000);
//		delay_ms(1000);
//		Car_TurnLeft(500);
//		delay_ms(1000);
//		delay_ms(1000);
//		delay_ms(1000);
//		Car_TurnRight(500);
//		delay_ms(1000);
//		delay_ms(1000);
//		delay_ms(1000);
//		Car_TransLeft(500);
//		delay_ms(1000);
//		delay_ms(1000);
//		delay_ms(1000);
//		Car_TransRight(500);
//	}
	
//	MotorA_SetSpeed(1,500);
//	MotorB_SetSpeed(1,500);
//	MotorC_SetSpeed(1,500);
//	MotorD_SetSpeed(1,500);
    while (1)
    {
#ifdef TEST_PS2_HardSPI
        ////////////////////////////////////////////////////////
        //motor1:右侧小震动电机 0x00关，其他开
        //motor2:左侧大震动电机 0x40~0xFF 电机开，值越大 震动越大
        ////////////////////////////////////////////////////////
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
        PS2_LX=ps2_get_anolog_data(PSS_LX);
        PS2_LY=ps2_get_anolog_data(PSS_LY);
        PS2_RX=ps2_get_anolog_data(PSS_RX);
        PS2_RY=ps2_get_anolog_data(PSS_RY);

        printf("PS2按键:%d		",PS2_KeyNum);   //打印按键的值
        printf("PS2左X轴:%d		",PS2_LX);   //打印左边X轴的值
        printf("PS2左Y轴:%d		",PS2_LY);   //打印右边Y轴的值
        printf("PS2右X轴:%d		",PS2_RX);   //打印左边X轴的值
        printf("PS2右Y轴:%d		",PS2_RY);   //打印右边Y轴的值
        printf("\r\n");   //换行
        delay_ms(50);
#endif

#ifdef TEST_MOTOR_McWheel
        PS2_KEY = ps2_key_serch();
        PS2_Mode = ps2_mode_get();

        if (PS2_Mode != Last_PS2_Mode)
        {
            Car_Forward(0);
        }

        if (PS2_Mode == PSB_REDLIGHT_MODE)
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
    }

}
