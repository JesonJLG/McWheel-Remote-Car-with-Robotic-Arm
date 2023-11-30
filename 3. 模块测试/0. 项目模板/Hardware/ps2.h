#ifndef SPI_H
#define SPI_H

#include "stm32f10x.h"
/*------------������ź궨��-------------*/
//#define	


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

extern unsigned char Data[9];
extern unsigned int MASK[16][2];
extern unsigned int Handkey;

#ifdef __cplusplus
extern "C" {
#endif
	
    void PS2_Init(void);  
	
     unsigned char ps2_mode_get(void);   //�ж��Ƿ�Ϊ���ģʽ
    
    void PS2_ReadData(void); //���ֱ�����
    
    unsigned char PS2_Cmd(unsigned char CMD);		  //���ֱ���������
    
   unsigned char ps2_key_serch(void);		  //����ֵ��ȡ
    
	unsigned char ps2_get_key_state(unsigned char key_id);
	
  unsigned char ps2_get_anolog_data(unsigned char button); //�õ�һ��ҡ�˵�ģ����
    
    void PS2_ClearData(void);	  //������ݻ�����
    
    void PS2_Vibration(unsigned char motor1, unsigned char motor2);//������motor1  0xFF���������أ�motor2  0x40~0xFF

    void PS2_EnterConfing(void);	 //��������
    
    void PS2_TurnOnAnalogMode(void); //����ģ����
    
    void PS2_VibrationMode(void);    //������
    
    void PS2_ExitConfing(void);	     //�������
    
    void PS2_SetInit(void);		     //���ó�ʼ��

    void PS2_ShortPoll(void);

	void PS2_CS_H(void);

	void PS2_CS_L(void);

	void PS2_SCK_H(void);

	void PS2_SCK_L(void);

	void PS2_DO_H(void);

	void PS2_DO_L(void);

	void PS2_DI_H(void);

	void PS2_DI_L(void);
	
	uint8_t PS2_DI(void);
	
	void PS2_Delay_US(uint32_t nCount);
	
	void PS2_GPIO_Init(void);
	
#ifdef __cplusplus
}
#endif

#endif
