#include "stm32f10x.h"                  // Device header
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "ps2.h"

uint8_t KeyNum;
uint8_t PS2_Mode, Last_PS2_Mode;
int main(void) 
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init();
    Uart1_init(115200); /*协议处理接口 */
    PS2_Init();
    DBG_PRINTF("NVIC分组完毕\r\nPS2初始化完毕\r\n电机初始化完毕\r\n舵机初始化完毕\r\n");

    while (1) 
	{

    }

}
