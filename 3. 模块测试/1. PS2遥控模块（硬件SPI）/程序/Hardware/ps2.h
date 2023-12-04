#ifndef SPI_H
#define SPI_H

#include "stm32f10x.h"
/*------------������ź궨��-------------*/
//#define PS2_PORT		GPIOB
//#define PS2_CS_SEL		GPIO_Pin_12		//PB12
//#define PS2_CLK			GPIO_Pin_13		//PB13
//#define PS2_DO_CMD		GPIO_Pin_14		//PB14	STM32(MOSI) -> �ֱ�(DO)
//#define PS2_DI_DAT		GPIO_Pin_15		//PB15	STM32(MISO) -> �ֱ�(DI)


#define PSB_LOSE				0
#define PSB_REDLIGHT_MODE		1
#define PSB_GREENLIGHT_MODE 	2

#define PSB_SELECT      1
#define PSB_L3          2
#define PSB_R3          3
#define PSB_START       4
#define PSB_PAD_UP      5
#define PSB_PAD_RIGHT   6
#define PSB_PAD_DOWN    7
#define PSB_PAD_LEFT    8
#define PSB_L2          9
#define PSB_R2          10
#define PSB_L1          11
#define PSB_R1          12
#define PSB_GREEN       13
#define PSB_RED         14
#define PSB_BLUE        15
#define PSB_PINK        16

#define PSB_TRIANGLE    13
#define PSB_CIRCLE      14
#define PSB_CROSS       15
#define PSB_SQUARE      16

//#define WHAMMY_BAR		8

//These are stick values
#define PSS_RX			5                //��ҡ��X������
#define PSS_RY			6
#define PSS_LX			7
#define PSS_LY			8

#define PSS_RX_MID	   0X7F
#define PSS_RY_MID     0X80

extern uint8_t Data[9];
extern unsigned int MASK[16][2];
extern unsigned int Handkey;

#ifdef __cplusplus
extern "C" {
#endif

static void PS2_ShortPoll(void);			//����������  �����ͻָ�����
void PS2_Init(void);
void PS2_Vibration(uint8_t motor1, uint8_t motor2);//������motor1  0xFF���������أ�motor2  0x40~0xFF
void PS2_EnterConfing(void);	 			//��������
void PS2_TurnOnAnalogMode(void); 			//��ģ��ģʽ
void PS2_VibrationMode(void);    			//��ģʽ����
void PS2_ExitConfing(void);	     			//��������˳�

uint8_t ps2_mode_get(void);   				//��ȡPS2�ֱ�ģʽ�����/�̵ƣ�
void PS2_ReadData(void); 					//���ֱ�����
uint8_t ps2_key_serch(void);		  		//��ȡ����������
uint8_t ps2_get_key_state(uint8_t key_id);	//��ȡ����״̬��
u8 PS2_DataKey(void);						//��ȡ������Ӧ��ֵ
uint8_t ps2_get_anolog_data(uint8_t button);//��ȡҡ��ģ����
void PS2_ClearData(void);					//���Data[]

#ifdef __cplusplus
}
#endif

#endif
