//�������SPIģʽ0��CPOLʱ�Ӽ���Ϊ0��SCK����ʱ��ƽΪ0����CPHAʱ����λΪ0��SCK��һ���������룩
//��ΪSPIģʽ2		CPOL = 1, CPOH = 0
#include "MySPI.h"
#include "delay.h"

#define	DELAY_TIME	{delay_us(16);}		//SCKһ��Ҫ����ʱ������PS2������

void MySPI_W_SS(uint8_t BitValue)
{
    GPIO_WriteBit(PS2_PORT, PS2_CS_SEL, (BitAction)BitValue);
	//DELAY_TIME;
}

void MySPI_Start(void)
{
    MySPI_W_SS(0);	//CSƬѡ ѡ�п�ʼ
}

void MySPI_Stop(void)
{
    MySPI_W_SS(1);	//ȡ��ѡ�н���
}

void MySPI_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�������
    GPIO_InitStructure.GPIO_Pin = PS2_CS_SEL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PS2_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//�������
	GPIO_InitStructure.GPIO_Pin = PS2_CLK | PS2_DO_CMD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PS2_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//��������
    GPIO_InitStructure.GPIO_Pin = PS2_DI_DAT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PS2_PORT, &GPIO_InitStructure);

	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;	//LSB����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;	//36MHz/128 = 281KHz
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;			//CPOL=1
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;		//CPOH=0	ģʽ2
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;			//CSƬѡ ���ʵ��
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);
	
	SPI_Cmd(SPI2, ENABLE);
	
    MySPI_W_SS(1);	//��ʼĬ��δѡ��
}


//SPI�����ֽ�	LSB��ǰ
uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
//    uint8_t i, ByteReceive = 0x00;

//    for (i = 0; i < 8; i ++)
//    {	
//        MySPI_W_MOSI(ByteSend & (0x01<<i));	//LSB��ǰ  ÿ��д��Ҫ�����ֽڶ�Ӧ1λ��ѭ���˴�д��һ�ֽ�
//        MySPI_W_SCK(0);						//�½��أ���͵�ƽ����ȡ
//        if (MySPI_R_MISO() == 1) {
//            ByteReceive |= (0x01<<i);   	//���ݽ��յ���1λ��Ч���ݣ������ֽڽ��ջ������Ķ�Ӧλ
//        }
//        MySPI_W_SCK(1);						//�����أ���ߵ�ƽ��д�룬��תʱ��
//    }

//    return ByteReceive;
	
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) != SET){	//�ȴ�TDRΪ��
		//DELAY_TIME
	}
	SPI_I2S_SendData(SPI2, ByteSend);	//дTDR
	
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) != SET){	//�ȴ�RDR�ǿ�
		//DELAY_TIME
	}
	return SPI_I2S_ReceiveData(SPI2);	//��RDR
}
