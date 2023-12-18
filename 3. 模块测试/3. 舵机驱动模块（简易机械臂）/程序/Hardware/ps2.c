#include "ps2.h"
#include "delay.h"
#include "usart.h"
#include "MySPI.h"

/*------------SPI模式2  CPOL = 1, CPOH = 0-------------*/
unsigned int Handkey;	// 按键值读取，零时存储。
static uint8_t ps2_mode;
uint8_t Comd[9]= {0x01,0x42,0x00,0x00,0x00,0x00,0x00,0x00,0x00};	//开始命令。请求数据
uint8_t Data[9]= {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; 	//数据存储数组
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
};	//按键值与按键明

void PS2_Init(void)
{
    MySPI_Init();

    PS2_ShortPoll();
    PS2_ShortPoll();
    PS2_ShortPoll();
	PS2_ShortPoll();

    PS2_EnterConfing();		//进入配置模式
    PS2_TurnOnAnalogMode();	//“红绿灯”配置模式，并选择是否保存
    PS2_VibrationMode();	//开启震动模式
    PS2_ExitConfing();		//完成并保存配置

}

//读取手柄数据
void PS2_ReadData(void)
{
    uint8_t byte=0;
    MySPI_Start();
    for(byte = 0; byte < 9; byte++)
    {
        if(byte < 2)	//0,1字节 即前两个字节
        {
            Data[byte] = MySPI_SwapByte(Comd[byte]);	//①发送0x01，请求接受数据 ②发送0x42，接受ID（PS2表示开始通信）
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
Description: 手柄震动函数，
Calls:		 void MySPI_SwapByte(uint8_t CMD);
Input: motor1:右侧小震动电机 0x00关，其他开
	   motor2:左侧大震动电机 0x40~0xFF 电机开，值越大 震动越大
******************************************************/
void PS2_Vibration(uint8_t motor1, uint8_t motor2)
{
    MySPI_Start();
    MySPI_SwapByte(0x01);  //开始命令
    MySPI_SwapByte(0x42);  //请求数据
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
    MySPI_SwapByte(0x01);	//发送0x01,接收无意义，相当于告诉对方准备开始
    MySPI_SwapByte(0x42);	//发送0x42(请求数据)，接收PS2 ID（0x41=绿灯模式，0x73=红灯模式）
    MySPI_SwapByte(0X00);	//发送无意义，接收PS2 0x5A（告诉STM32数据来了）
    MySPI_SwapByte(0x00);	//发送 0X00~0XFF（右侧小震动电机是否开启），接收PS2 发送的数据（SELECT、 L3 、 R3、 START 、 UP、 RIGHT、 DOWN、 LEFT 是否被按下，若被按下对应位为0）
    MySPI_SwapByte(0x00);	//发送 0X00~0XFF（左侧大震动电机振动幅度），接收PS2 发送的数据（L2 、 R2、L1 、R1、△、○、╳、□ 是否被按下，若被按下对应位为0）
    MySPI_Stop();
}

//进入配置
void PS2_EnterConfing(void)
{
    MySPI_Start();			// 拉低CS
    MySPI_SwapByte(0x01);  	// 发出一个命令“0x01”-->手柄会回复它的 ID“0x41=模拟绿灯，0x73=模拟红灯”
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

//打开模拟模式
void PS2_TurnOnAnalogMode(void)
{
    MySPI_Start();
    MySPI_SwapByte(0x01);
    MySPI_SwapByte(0x44);
    MySPI_SwapByte(0X00);
    MySPI_SwapByte(0x01); //analog=0x01;digital=0x00  软件设置发送模式
    MySPI_SwapByte(0x03); //0x03锁存设置，即不可通过按键“MODE”设置模式。
    //0xEE不锁存软件设置，可通过按键“MODE”设置模式。
    MySPI_SwapByte(0X00);
    MySPI_SwapByte(0X00);
    MySPI_SwapByte(0X00);
    MySPI_SwapByte(0X00);
    MySPI_Stop();
}

//振动设置
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

//完成并保存配置
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
以下为数据解析部分
*************************************/
//判断是否为红灯模式,0x41=模拟绿灯，0x73=模拟红灯
//返回值；0，红灯模式
//		  其他，其他模式
uint8_t ps2_mode_get(void)
{
    static int getCnt = 0;
    if( Data[1] == 0X73)	//第二个接受字节 ID
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
            DBG_PRINTF("手柄信号丢失\r\n");
        }
    }
    return ps2_mode;
}

//对读出来的PS2的数据进行处理,只处理按键部分
//只有一个按键按下时按下为0， 未按下为1
uint8_t ps2_key_serch()
{
    uint8_t index;
    uint8_t key_num = 0;
    PS2_ClearData();
    PS2_ReadData();

    Handkey=(Data[4]<<8)|Data[3];     //这是16个按键  按下为0， 未按下为1
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

//对读出来的PS2的数据进行处理,只处理按键部分
//只有一个按键按下时按下为0， 未按下为1
u8 PS2_DataKey()
{
    u8 index;

    PS2_ClearData();
    PS2_ReadData();

    Handkey=(Data[4]<<8)|Data[3];     //这是16个按键  按下为0， 未按下为1
    for(index=0; index<16; index++)
    {
        if((Handkey&(1<<(MASK[index][0]-1)))==0)	//??关注这里??
            return index+1;
    }
    return 0;          //没有任何按键按下
}

//得到一个摇杆的模拟量	 范围0~256
uint8_t ps2_get_anolog_data(uint8_t button)
{
    return Data[button];
}

//清除数据缓冲区
void PS2_ClearData()
{
    uint8_t a;
    for(a=0; a<9; a++)
        Data[a]=0x00;
}
