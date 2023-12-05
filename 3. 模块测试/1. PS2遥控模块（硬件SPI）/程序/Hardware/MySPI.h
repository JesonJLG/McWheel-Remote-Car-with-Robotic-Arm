#ifndef __MYSPI_H
#define __MYSPI_H

#include "stm32f10x.h"
/*------------添加引脚宏定义-------------*/
#define PS2_PORT		GPIOB
#define PS2_CS_SEL		GPIO_Pin_12		//PB12
#define PS2_CLK			GPIO_Pin_13		//PB13
#define PS2_DO_CMD		GPIO_Pin_15		//PB14	STM32(MOSI) -> 手柄(DO)
#define PS2_DI_DAT		GPIO_Pin_14		//PB15	STM32(MISO) <- 手柄(DI)

void MySPI_Init(void);
void MySPI_Start(void);
void MySPI_Stop(void);
uint8_t MySPI_SwapByte(uint8_t ByteSend);

#endif
