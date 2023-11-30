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
    Uart1_init(115200); /*Э�鴦��ӿ� */
    PS2_Init();
    DBG_PRINTF("NVIC�������\r\nPS2��ʼ�����\r\n�����ʼ�����\r\n�����ʼ�����\r\n");

    while (1) 
	{

    }

}
