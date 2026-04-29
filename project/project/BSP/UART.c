#include "UART.h"
#include "OLED.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

uint8_t uart1_Buff[RXBUFF_MAX_SIZEOF];
uint16_t uart1_i;
uint8_t rxdata1;

uint8_t uart2_Buff[RXBUFF_MAX_SIZEOF];
uint16_t uart2_i;
uint8_t rxdata2;
uint8_t uart2_flag;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    uart1_Buff[uart1_i++] = rxdata1;
    HAL_UART_Receive_IT(&huart1, &rxdata1, 1);
    if (uart1_i >= RXBUFF_MAX_SIZEOF)
    {
      uart1_i = 0;
    }
  }
	if (huart->Instance == USART2)
  {
		uart2_Buff[uart2_i++] = rxdata2;
		HAL_UART_Receive_IT(&huart2, &rxdata2, 1);
  }
}





