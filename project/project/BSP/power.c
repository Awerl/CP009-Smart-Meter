#include "POWER.h"
#include "UART.h"
#include "string.h"
#include "delay.h"
#include "OLED.h"
#include "stdio.h"
extern UART_HandleTypeDef huart2;
#define uart							huart2
extern uint8_t uart2_Buff[RXBUFF_MAX_SIZEOF];
extern uint16_t uart2_i;
extern uint8_t rxdata2;
//01 03 00 48 00 01 04 1C				电压
//01 03 00 49 00 01 55 DC				电流
//01 03 00 4A 00 01 A5 DC				功率
uint8_t rx_len;
void POWER_Init(void)
{
	HAL_UART_Receive_IT(&huart2, &rxdata2, 1);
}

void POWER_Send_String(uint8_t *s)
{
	HAL_UART_Transmit(&huart2, s, 8, 0xffff);
}

void POWER_Clear_Buff(void)
{
	memset(uart2_Buff, 0, sizeof((char *)uart2_Buff));
}


uint8_t POWER_WaitRecive(void)
{
	if(uart2_i == 0) 							
		return 0;
		
	if(uart2_i == rx_len)
	{
		uart2_i = 0;							
		return 1;								
	}
	rx_len = uart2_i;			
	return 0;								
}



void POWER_Get_Data(float *U, float *I,float *P)
{
	uint8_t timeout = 10;
	uint8_t data[9] = {0x01, 0x03, 0x00, 0x48, 0x00, 0x03, 0x85, 0xDD};
	char ss[100];
	uint32_t u_temp = 0, i_temp = 0, p_temp = 0;
	POWER_Send_String(data);
	while(timeout--)
	{
		if (POWER_WaitRecive())
		{
			u_temp = uart2_Buff[3] << 24 | uart2_Buff[4] << 16 | uart2_Buff[5] << 8 | uart2_Buff[6];
				*U = u_temp * 0.0001;

			i_temp = uart2_Buff[7] << 24 | uart2_Buff[8] << 16 | uart2_Buff[9] << 8 | uart2_Buff[10];
				*I = i_temp * 0.0001;
			
			p_temp = uart2_Buff[11] << 24 | uart2_Buff[12] << 16 | uart2_Buff[13] << 8 | uart2_Buff[14];

				*P = p_temp * 0.0001;
			POWER_Clear_Buff();
			return ;
		}
		Delay_ms(1);
	}
}

