/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ESP8266.h"
#include "delay.h"
#include "OLED.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "DHT11.h"
#include "DS1302.h"
#include "JQ8900.h"
#include "KEY.h"
#include "POWER.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern struct Record_Info rec;
extern uint8_t key_flag;
extern uint8_t time_i;
uint8_t time_set_flag;
DS1302_Time_t time;
DS1302_Time_t set_time;
DS1302_Time_t end_time;

float U, I, P;
uint8_t humi_value, temp_value, temp_max = 40, humi_max = 80;
char s[100];
uint8_t mode;
uint8_t power_flag;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/*
*	功能：设置电源是否导通
*	参数：1开   0关
*	返回值：
*/
void Power_Set(uint8_t x)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, (GPIO_PinState)x);
}


/*
*	功能：控制逻辑
*	参数：无
*	返回值：无
*/
void Ctrl(void)
{
	switch(time_set_flag)
	{
		case 1:
		{
			end_time.hour = 0;
			end_time.minute = 0;
			end_time.second = 0;
			uint16_t sec_time = (set_time.second + time.second);
			if (sec_time > 59)
			{
				end_time.minute += 1;
				end_time.second = sec_time - 60;
			}
			else
				end_time.second = sec_time;
			uint16_t minute_temp = (set_time.minute + time.minute);
			if (minute_temp > 59)
			{
				end_time.hour += 1;
				end_time.minute += minute_temp - 60;
			}
			else
				end_time.minute += minute_temp;
			uint16_t hour_temp = (set_time.hour + time.hour);
			if (hour_temp > 23)
			{
				end_time.date += 1;
				end_time.hour += hour_temp - 24;
			}
			else
				end_time.hour += hour_temp;
			time_set_flag = 2;
		}
		break;
		case 2:
			if ((end_time.hour == time.hour) && (end_time.minute == time.minute) && (end_time.second == time.second))
			{
				end_time.hour = 0;
				end_time.minute = 0;
				end_time.second = 0;
				Power_Set(power_flag);
				time_set_flag = 0;
				key_flag = 0;
				time_i = 0;
			}
			break;
	}
	
	if (temp_value > temp_max)
	{
		if (!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1))
		{
			rec.index = 1;
			strcpy(rec.path, "\"00001\"");
			rec.volume = 15;
			JQ8900_Play_Recording(&rec);
		}
		Power_Set(0);
	}
	
	if (humi_value > humi_max)
	{
		if (!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1))
		{
			rec.index = 2;
			strcpy(rec.path, "\"00002\"");
			rec.volume = 15;
			JQ8900_Play_Recording(&rec);
		}
		Power_Set(0);
	}
}
/*
*	功能：页面显示
*	参数：无
*	返回值：无
*/
void Page(void)
{
	OLED_ShowChinese(1, 1, (mode == 0) ? "自动" : "设置");
	sprintf(s, "%02d:%02d:%02d", time.hour, time.minute, time.second);
	OLED_ShowString(50, 1, s, 8);
	if (mode == 0)
	{
		OLED_ShowChinese(1, 16, "温度：");
		OLED_ShowNum(40, 16, temp_value, 2, 8);
		OLED_ShowChinese(60, 16, "湿度：");
		OLED_ShowNum(100, 16, humi_value, 2, 8);
		OLED_ShowChinese(1, 32, "电流：");
		sprintf(s, "%.4fA", I);
		OLED_ShowString(40, 32, s, 8);
		OLED_ShowChinese(1, 48, "电压：");
		sprintf(s, "%fV", U);
		OLED_ShowString(40, 48, s, 8);
	}
	else if (mode == 1)
	{
		if (key_flag == 0)
		{
			OLED_ClearArea(85, 32, 16, 16);
			if (time_i == 0)
			{
				OLED_ShowChinese(3, 32, "↑");
				OLED_ClearArea(43, 32, 16, 16);
			}
			else if (time_i == 1)
			{
				OLED_ShowChinese(23, 32, "↑");
				OLED_ClearArea(3, 32, 16, 16);
			}
			else if (time_i == 2)
			{
				OLED_ShowChinese(43, 32, "↑");
				OLED_ClearArea(23, 32, 16, 16);
			}
		}
		else if (key_flag == 1)
		{
			OLED_ClearArea(1, 32, 55, 16);
			OLED_ShowChinese(83, 32, "↑");
		}
		
		
		OLED_ShowChinese(1, 48, "温度：");
		sprintf(s, "%d ", temp_max);
		OLED_ShowString(40, 48, s, 8);
		OLED_ShowChinese(65, 48, "湿度：");
		sprintf(s, "%d ", humi_max);
		OLED_ShowString(105, 48, s, 8);
		sprintf(s, "%02d:%02d:%02d", end_time.hour, end_time.minute, end_time.second);
		OLED_ShowString(1, 16, s, 8);
		OLED_ShowChinese(80, 16, (power_flag == 1) ? "开" : "关");
	}
}

/*
*	功能：数据发送
*	参数：无
*	返回值：无
*/
void Data_Send(void)
{
	sprintf(s, "temp:%d,humi:%d,U:%.1f,I:%.4f,P:%.1f,\r\n", temp_value, humi_value, U, I, P);
	ESP8266_SendData(s, strlen(s));
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  OLED_Init();
  ESP8266_Init();
	DS1302_Init();
	Key_Init();
	POWER_Init();
	Power_Set(1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		ESP8266_GetIPD(100);
		Ctrl();
		Data_Send();
		Key_Scanf();
		DS1302_ReadTime(&time);
		DHT11_Read_Data(&humi_value, &temp_value);
		Page();
		OLED_Update();
		POWER_Get_Data(&U, &I, &P);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
