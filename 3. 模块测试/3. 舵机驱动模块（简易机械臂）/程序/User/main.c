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
    Uart1_init(115200); /*协议处理接口 */
    LED_Init();
    DBG_PRINTF("1111111111111111");
    PS2_Init();
    Motor_Init();
	Servo_Init();	//搞了半天，搁这等着我呢，忘记初始化了我天！！！
    DBG_PRINTF("2222222222222222");
	//LED = 0;
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

        // 获取按下的按键值和模式
		ps2_mode = ps2_mode_get();
		ps2_keynum = ps2_key_serch();
        //ps2_keynum = PS2_DataKey();
		ps2_lx=ps2_get_anolog_data(PSS_LX);
        ps2_ly=ps2_get_anolog_data(PSS_LY);
        ps2_rx=ps2_get_anolog_data(PSS_RX);
        ps2_ry=ps2_get_anolog_data(PSS_RY);
		
//		printf("PS2模式:%d 	",ps2_mode);
//        printf("PS2按键:%d	",ps2_keynum);   //打印按键的值
//        printf("PS2左X轴:%d	",ps2_lx);   //打印左边X轴的值
//        printf("PS2左Y轴:%d	",ps2_ly);   //打印右边Y轴的值
//        printf("PS2右X轴:%d	",ps2_rx);   //打印左边X轴的值
//        printf("PS2右Y轴:%d	",ps2_ry);   //打印右边Y轴的值
//        printf("\r\n");   //换行
        

        // 检查模式是否发生变化
        if (ps2_mode != last_ps2_mode) {
			printf("检查模式是否发生变化");
            //Car_Forward(0);	//也就是说PWM占空比为0 及低电平 电机驱动控制引脚 二接1 就能转？
        }

        // 检查是否为红灯模式
        if (ps2_mode == PSB_REDLIGHT_MODE) {	LED = 0;
            // 获取PS2控制器的模拟数据
            ps2_lx = ps2_get_anolog_data(PSS_LX);
            ps2_ly = ps2_get_anolog_data(PSS_LY);

            // 根据模拟数据控制小车
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

            // 根据模拟数据控制机械臂
            ps2_rx = ps2_get_anolog_data(PSS_RX);
            if (ps2_rx == 0xFF) {
				DBG_PRINTF("抬起");
                RobotArm_RaiseHand(UNIT_PWM);	//抬起
            } else if (ps2_rx == 0x00) {
				DBG_PRINTF("放下");
                RobotArm_DropHand(UNIT_PWM);	//放下
            }

            ps2_ry = ps2_get_anolog_data(PSS_RY);
            if (ps2_ry == 0x00) {
				DBG_PRINTF("伸展");
                RobotArm_StrechHand(UNIT_PWM);	//伸展
            } else if (ps2_ry == 0xFF) {
				DBG_PRINTF("收缩");
                RobotArm_ShinkHand(UNIT_PWM);	//收缩
            }

            // 根据按键状态控制机械爪
            if (ps2_get_key_state(PSB_R1)) {
				DBG_PRINTF("夹爪");
                RobotArm_ShakeHand(UNIT_PWM);	//夹爪
            } else if (ps2_get_key_state(PSB_R2)) {
				DBG_PRINTF("松爪");
                RobotArm_LetHand(UNIT_PWM);		//松爪
            }
        } 
        else {	//这里都进不来
            // 如果不是红灯模式，停止小车
            //Car_Forward(0);
			LED = 0;	//奇怪这里不会跑
			delay_ms(1000);
			LED = 1;
			delay_ms(1000);
        }
		//LED = 0;
        // 更新上一次的PS2模式
        last_ps2_mode = ps2_mode;
		//Car_Forward(1000);	//这样的话，轮子不会自动转，奇了怪了
        // 延迟一段时间，以便PS2控制器响应
        delay_ms(10);
#endif
    }

}
