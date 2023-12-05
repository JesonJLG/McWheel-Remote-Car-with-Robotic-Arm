//这里采用SPI模式0，CPOL时钟极性为0（SCK空闲时电平为0），CPHA时钟相位为0（SCK第一个边沿移入）
//改为SPI模式2		CPOL = 1, CPOH = 0
#include "MySPI.h"
#include "delay.h"

#define	DELAY_TIME	{delay_us(16);}		//SCK一定要有延时，否则PS2跟不上

void MySPI_W_SS(uint8_t BitValue)
{
    GPIO_WriteBit(PS2_PORT, PS2_CS_SEL, (BitAction)BitValue);
}

void MySPI_W_SCK(uint8_t BitValue)
{
    GPIO_WriteBit(PS2_PORT, PS2_CLK, (BitAction)BitValue);
	DELAY_TIME;
}

void MySPI_W_MOSI(uint8_t BitValue)
{
    GPIO_WriteBit(PS2_PORT, PS2_DO_CMD, (BitAction)BitValue);
}

uint8_t MySPI_R_MISO(void)
{
    return GPIO_ReadInputDataBit(PS2_PORT, PS2_DI_DAT);
}

void MySPI_Start(void)
{
    MySPI_W_SS(0);	//CS片选 选中开始
}

void MySPI_Stop(void)
{
    MySPI_W_SS(1);	//取消选中结束
}

void MySPI_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出
    GPIO_InitStructure.GPIO_Pin = PS2_CS_SEL|PS2_CLK|PS2_DO_CMD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PS2_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//上拉输入
    GPIO_InitStructure.GPIO_Pin = PS2_DI_DAT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PS2_PORT, &GPIO_InitStructure);

    MySPI_W_SS(1);	//初始默认未选中
    MySPI_W_SCK(1);	//初始默认空闲为1	CPOL=1
}


//SPI交换字节	LSB在前
uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
    uint8_t i, ByteReceive = 0x00;

    for (i = 0; i < 8; i ++)
    {	
        MySPI_W_MOSI(ByteSend & (0x01<<i));	//LSB在前  每次写入要发送字节对应1位，循环八次写入一字节
        MySPI_W_SCK(0);						//下降沿（或低电平）读取
        if (MySPI_R_MISO() == 1) {
            ByteReceive |= (0x01<<i);   	//根据接收到的1位有效数据，存入字节接收缓冲区的对应位
        }
        MySPI_W_SCK(1);						//上升沿（或高电平）写入，运转时序
    }

    return ByteReceive;
}
