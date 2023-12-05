//这里采用SPI模式0，CPOL时钟极性为0（SCK空闲时电平为0），CPHA时钟相位为0（SCK第一个边沿移入）
//改为SPI模式2		CPOL = 1, CPOH = 0
#include "MySPI.h"
#include "delay.h"

#define	DELAY_TIME	{delay_us(16);}		//SCK一定要有延时，否则PS2跟不上

void MySPI_W_SS(uint8_t BitValue)
{
    GPIO_WriteBit(PS2_PORT, PS2_CS_SEL, (BitAction)BitValue);
	//DELAY_TIME;
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
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出
    GPIO_InitStructure.GPIO_Pin = PS2_CS_SEL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PS2_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//复用输出
	GPIO_InitStructure.GPIO_Pin = PS2_CLK | PS2_DO_CMD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PS2_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//上拉输入
    GPIO_InitStructure.GPIO_Pin = PS2_DI_DAT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PS2_PORT, &GPIO_InitStructure);

	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;	//LSB先行
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;	//36MHz/128 = 281KHz
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;			//CPOL=1
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;		//CPOH=0	模式2
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;			//CS片选 软件实现
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);
	
	SPI_Cmd(SPI2, ENABLE);
	
    MySPI_W_SS(1);	//初始默认未选中
}


//SPI交换字节	LSB在前
uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
//    uint8_t i, ByteReceive = 0x00;

//    for (i = 0; i < 8; i ++)
//    {	
//        MySPI_W_MOSI(ByteSend & (0x01<<i));	//LSB在前  每次写入要发送字节对应1位，循环八次写入一字节
//        MySPI_W_SCK(0);						//下降沿（或低电平）读取
//        if (MySPI_R_MISO() == 1) {
//            ByteReceive |= (0x01<<i);   	//根据接收到的1位有效数据，存入字节接收缓冲区的对应位
//        }
//        MySPI_W_SCK(1);						//上升沿（或高电平）写入，运转时序
//    }

//    return ByteReceive;
	
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) != SET){	//等待TDR为空
		//DELAY_TIME
	}
	SPI_I2S_SendData(SPI2, ByteSend);	//写TDR
	
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) != SET){	//等待RDR非空
		//DELAY_TIME
	}
	return SPI_I2S_ReceiveData(SPI2);	//读RDR
}
