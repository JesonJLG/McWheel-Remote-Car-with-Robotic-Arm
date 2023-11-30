#include "stm32f10x.h"                  // Device header
#include "sys.h"  


#define UNIT_PWM 3
#define SHOW_RESULT(str_before, dest, size, test_num) \
    {                                                 \
        if (memcmp(str_before, dest, size) == 0)      \
            test_results[test_num] = TRUE;            \
        else                                          \
            test_results[test_num] = FALSE;           \
    }

uint8_t KeyNum;
uint8_t PS2_Mode, Last_PS2_Mode;
int main(void) {
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();
    Uart1_init(9600); /*协议处理接口 */
    PS2_Init();
    Motor_Init();
    Servo_Init();
    DBG_PRINTF("NVIC分组完毕\r\nPS2初始化完毕\r\n电机初始化完毕\r\n舵机初始化完毕\r\n");

    while (1) {
        KeyNum = ps2_key_serch();
        PS2_Mode = ps2_mode_get();

        if (PS2_Mode != Last_PS2_Mode) {
            DBG_PRINTF("PS2模式有变，停车！\r\n");
            Car_Forward(0);
        }

        if (PS2_Mode == PSB_REDLIGHT_MODE) {
            DBG_PRINTF("红灯模式（模拟输出）\r\n");
            unsigned char ps2_lx, ps2_ly, ps2_rx, ps2_ry;

            ps2_lx = ps2_get_anolog_data(PSS_LX);
            ps2_ly = ps2_get_anolog_data(PSS_LY);
            if (ps2_lx <= 0xFF) {
                DBG_PRINTF("ps2_lx:%d\r\n", ps2_lx);
            } else {
                DBG_PRINTF("ps2_lx值异常");
            }
            if (ps2_ly <= 0xFF) {
                DBG_PRINTF("ps2_ly:%d\r\n", ps2_lx);
            } else {
                DBG_PRINTF("ps2_ly值异常");
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

            //机械臂控制
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
        } else if (PS2_Mode == PSB_GREENLIGHT_MODE) {
            DBG_PRINTF("绿灯模式\r\n");
            if (KeyNum) {
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

                //机械臂控制
                if (ps2_get_key_state(PSB_RED)) {
                    DBG_PRINTF("机械臂控制--PSB_RED\r\n");
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
        Last_PS2_Mode = PS2_Mode;
        delay_ms(20);
    }
}
