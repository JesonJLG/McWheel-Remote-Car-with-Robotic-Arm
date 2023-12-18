#include "ps2.h"
#include "delay.h"
#include "usart.h"
#include "MySPI.h"

/*------------SPIģʽ2  CPOL = 1, CPOH = 0-------------*/
unsigned int Handkey;	// ����ֵ��ȡ����ʱ�洢��
static uint8_t ps2_mode;
uint8_t Comd[9]= {0x01,0x42,0x00,0x00,0x00,0x00,0x00,0x00,0x00};	//��ʼ�����������
uint8_t Data[9]= {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; 	//���ݴ洢����
uint8_t key_search[12] = {0x00,0x00,0x00,0x00,
                          0x00,0x00,0x00,0x00,
                          0x00,0x00,0x00,0x00
                         };

unsigned int MASK[16][2]= {
    {PSB_SELECT,0},
    {PSB_L3,0},
    {PSB_R3,0},
    {PSB_START,0},
    {PSB_PAD_UP,0},
    {PSB_PAD_RIGHT,0},
    {PSB_PAD_DOWN,0},
    {PSB_PAD_LEFT,0},
    {PSB_L2,0},
    {PSB_R2,0},
    {PSB_L1,0},
    {PSB_R1,0},
    {PSB_GREEN,0},
    {PSB_RED,0},
    {PSB_BLUE,0},
    {PSB_PINK,0}
};	//����ֵ�밴����

void PS2_Init(void)
{
    MySPI_Init();

    PS2_ShortPoll();
    PS2_ShortPoll();
    PS2_ShortPoll();
	PS2_ShortPoll();

    PS2_EnterConfing();		//��������ģʽ
    PS2_TurnOnAnalogMode();	//�����̵ơ�����ģʽ����ѡ���Ƿ񱣴�
    PS2_VibrationMode();	//������ģʽ
    PS2_ExitConfing();		//��ɲ���������

}

//��ȡ�ֱ�����
void PS2_ReadData(void)
{
    uint8_t byte=0;
    MySPI_Start();
    for(byte = 0; byte < 9; byte++)
    {
        if(byte < 2)	//0,1�ֽ� ��ǰ�����ֽ�
        {
            Data[byte] = MySPI_SwapByte(Comd[byte]);	//�ٷ���0x01������������� �ڷ���0x42������ID��PS2��ʾ��ʼͨ�ţ�
        }
        else
        {
            Data[byte] = MySPI_SwapByte(0x00);	//
        }
    }
    MySPI_Stop();
}



/******************************************************
Function:    void PS2_Vibration(uint8_t motor1, uint8_t motor2)
Description: �ֱ��𶯺�����
Calls:		 void MySPI_SwapByte(uint8_t CMD);
Input: motor1:�Ҳ�С�𶯵�� 0x00�أ�������
	   motor2:�����𶯵�� 0x40~0xFF �������ֵԽ�� ��Խ��
******************************************************/
void PS2_Vibration(uint8_t motor1, uint8_t motor2)
{
    MySPI_Start();
    MySPI_SwapByte(0x01);  //��ʼ����
    MySPI_SwapByte(0x42);  //��������
    MySPI_SwapByte(0X00);
    MySPI_SwapByte(motor1);
    MySPI_SwapByte(motor2);
    MySPI_SwapByte(0X00);
    MySPI_SwapByte(0X00);
    MySPI_SwapByte(0X00);
    MySPI_SwapByte(0X00);
    MySPI_Stop();
}

//short poll
void PS2_ShortPoll(void)
{
    MySPI_Start();
    MySPI_SwapByte(0x01);	//����0x01,���������壬�൱�ڸ��߶Է�׼����ʼ
    MySPI_SwapByte(0x42);	//����0x42(��������)������PS2 ID��0x41=�̵�ģʽ��0x73=���ģʽ��
    MySPI_SwapByte(0X00);	//���������壬����PS2 0x5A������STM32�������ˣ�
    MySPI_SwapByte(0x00);	//���� 0X00~0XFF���Ҳ�С�𶯵���Ƿ�����������PS2 ���͵����ݣ�SELECT�� L3 �� R3�� START �� UP�� RIGHT�� DOWN�� LEFT �Ƿ񱻰��£��������¶�ӦλΪ0��
    MySPI_SwapByte(0x00);	//���� 0X00~0XFF�������𶯵���񶯷��ȣ�������PS2 ���͵����ݣ�L2 �� R2��L1 ��R1�������𡢨w���� �Ƿ񱻰��£��������¶�ӦλΪ0��
    MySPI_Stop();
}

//��������
void PS2_EnterConfing(void)
{
    MySPI_Start();			// ����CS
    MySPI_SwapByte(0x01);  	// ����һ�����0x01��-->�ֱ���ظ����� ID��0x41=ģ���̵ƣ�0x73=ģ���ơ�
    MySPI_SwapByte(0x43);  	//
    MySPI_SwapByte(0X00);
    MySPI_SwapByte(0x01);
    MySPI_SwapByte(0x00);
    MySPI_SwapByte(0X00);
    MySPI_SwapByte(0X00);
    MySPI_SwapByte(0X00);
    MySPI_SwapByte(0X00);
    MySPI_Stop();
}

//��ģ��ģʽ
void PS2_TurnOnAnalogMode(void)
{
    MySPI_Start();
    MySPI_SwapByte(0x01);
    MySPI_SwapByte(0x44);
    MySPI_SwapByte(0X00);
    MySPI_SwapByte(0x01); //analog=0x01;digital=0x00  ������÷���ģʽ
    MySPI_SwapByte(0x03); //0x03�������ã�������ͨ��������MODE������ģʽ��
    //0xEE������������ã���ͨ��������MODE������ģʽ��
    MySPI_SwapByte(0X00);
    MySPI_SwapByte(0X00);
    MySPI_SwapByte(0X00);
    MySPI_SwapByte(0X00);
    MySPI_Stop();
}

//������
void PS2_VibrationMode(void)
{
    MySPI_Start();
    MySPI_SwapByte(0x01);
    MySPI_SwapByte(0x4D);
    MySPI_SwapByte(0X00);
    MySPI_SwapByte(0x00);
    MySPI_SwapByte(0X01);
    MySPI_Stop();
}

//��ɲ���������
void PS2_ExitConfing(void)
{
    MySPI_Start();
    MySPI_SwapByte(0x01);
    MySPI_SwapByte(0x43);
    MySPI_SwapByte(0X00);
    MySPI_SwapByte(0x00);
    MySPI_SwapByte(0x5A);
    MySPI_SwapByte(0x5A);
    MySPI_SwapByte(0x5A);
    MySPI_SwapByte(0x5A);
    MySPI_SwapByte(0x5A);
    MySPI_Stop();
}

/*************************************
����Ϊ���ݽ�������
*************************************/
//�ж��Ƿ�Ϊ���ģʽ,0x41=ģ���̵ƣ�0x73=ģ����
//����ֵ��0�����ģʽ
//		  ����������ģʽ
uint8_t ps2_mode_get(void)
{
    static int getCnt = 0;
    if( Data[1] == 0X73)	//�ڶ��������ֽ� ID
    {
        ps2_mode = PSB_REDLIGHT_MODE;
    }
    else if ( Data[1] == 0X41)
    {
        ps2_mode = PSB_GREENLIGHT_MODE;
    }
    else
    {
        ps2_mode = PSB_LOSE;
        getCnt++;
        if(getCnt > 100) {
            getCnt = 0;
            DBG_PRINTF("�ֱ��źŶ�ʧ\r\n");
        }
    }
    return ps2_mode;
}

//�Զ�������PS2�����ݽ��д���,ֻ����������
//ֻ��һ����������ʱ����Ϊ0�� δ����Ϊ1
uint8_t ps2_key_serch()
{
    uint8_t index;
    uint8_t key_num = 0;
    PS2_ClearData();
    PS2_ReadData();

    Handkey=(Data[4]<<8)|Data[3];     //����16������  ����Ϊ0�� δ����Ϊ1
    for(index=4; index<16; index++)
    {
        if((Handkey&(1<<(MASK[index][0]-1)))==0)
        {
            MASK[index][1] = 1;
            key_num++;
        }
        else
        {
            MASK[index][1] = 0;
        }
    }
    return key_num;
}

uint8_t ps2_get_key_state(uint8_t key_id)
{
    if (key_id < PSB_SELECT)
        return 0;
    else
        return MASK[key_id - 1][1];
}

//�Զ�������PS2�����ݽ��д���,ֻ����������
//ֻ��һ����������ʱ����Ϊ0�� δ����Ϊ1
u8 PS2_DataKey()
{
    u8 index;

    PS2_ClearData();
    PS2_ReadData();

    Handkey=(Data[4]<<8)|Data[3];     //����16������  ����Ϊ0�� δ����Ϊ1
    for(index=0; index<16; index++)
    {
        if((Handkey&(1<<(MASK[index][0]-1)))==0)	//??��ע����??
            return index+1;
    }
    return 0;          //û���κΰ�������
}

//�õ�һ��ҡ�˵�ģ����	 ��Χ0~256
uint8_t ps2_get_anolog_data(uint8_t button)
{
    return Data[button];
}

//������ݻ�����
void PS2_ClearData()
{
    uint8_t a;
    for(a=0; a<9; a++)
        Data[a]=0x00;
}
