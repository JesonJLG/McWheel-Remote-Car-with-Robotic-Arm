#ifndef __MYSPI_H
#define __MYSPI_H

#include "stm32f10x.h"
/*------------添加引脚宏定义-------------*/
#define PS2_PORT		GPIOB
#define PS2_CS_SEL		GPIO_Pin_12		//PB12
#define PS2_CLK			GPIO_Pin_13		//PB13
#define PS2_DO_CMD		GPIO_Pin_15		//PB15(MOSI)	STM32(MOSI) -> 手柄(DI)	
#define PS2_DI_DAT		GPIO_Pin_14		//PB14(MISO)	STM32(MISO) <- 手柄(DO)
/*------------硬件SPI引脚一定不要接反，切记！-------------*/
/*************************************
这里发生了让我调试两天的失误
我调了两天的软件？？？
无法通讯--PS2转接器就是返回不了ID，返回的全是0xFF！！！
最后发现！！！
硬件SPI的接线错了！！！
仔细一看
哦~~
我STM32的MOSI引脚要接在PS2转接板的MISO引脚，这才跑通的
纳尼
本来我还天真的以为
主机，从机的接法就是MOSI--MOSI	MISO--MISO
可能是这转接板的标注错了，要么就是标注的人有自己的想法
*************************************/

void MySPI_Init(void);
void MySPI_Start(void);
void MySPI_Stop(void);
uint8_t MySPI_SwapByte(uint8_t ByteSend);

#endif
