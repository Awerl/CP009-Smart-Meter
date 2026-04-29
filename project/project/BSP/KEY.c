#include "KEY.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "OLED.h"
#include "DS1302.h"
extern TIM_HandleTypeDef htim1;
extern uint8_t mode, power_flag, time_set_flag;
extern  DS1302_Time_t set_time;
extern  DS1302_Time_t end_time;
struct keys key[KEY_MAX_NUMBER];
uint8_t time_i, key_flag;
void Key_Init(void)
{
	HAL_TIM_Base_Start_IT(&htim1);
}

void Key_Scanf(void)
{
	if (key[0].key_flag == 1)
	{
		OLED_Clear();
		if (mode == 1)
			mode = 0;
		else
			mode++;
		time_i = 0;
		key[0].key_flag = 0;
	}
	else if (key[1].key_flag == 1)
	{
		if (key_flag == 0)
		{
			if (mode == 1)
			{
				if (time_i == 2)
					time_i = 0;
				else 
					time_i++;
			}
		}
		key[1].key_flag = 0;
	}
	else if (key[2].key_flag == 1)
	{
		if (key_flag == 0)
		{
			if ((time_i == 0) && (mode == 1))
			{
				if (end_time.hour < 23)
					end_time.hour++;
				else
					end_time.hour = 0;
			}
			if ((time_i == 1) && (mode == 1))
			{
				if (end_time.minute < 59)
					end_time.minute++;
				else
					end_time.minute = 0;
			}
			if ((time_i == 2) && (mode == 1))
			{
				if  (end_time.second < 59)
					end_time.second++;
				else
					end_time.second = 0;
			}
		}
		else if (key_flag == 1)
			power_flag = 1;
		key[2].key_flag = 0;
	}
	else if (key[3].key_flag == 1) 
	{
		if (key_flag == 0)
		{
			if ((time_i == 0) && (mode == 1))
			{
				if (end_time.hour == 0)
					end_time.hour = 23;
				else
					end_time.hour--;
			}
			if ((time_i == 1) && (mode == 1))
			{
				if (end_time.minute == 0)
					end_time.minute = 59;
				else
					end_time.minute--;
			}
			if ((time_i == 2) && (mode == 1))
			{
				if  (end_time.second == 0)
					end_time.second = 59;
				else
					end_time.second--;
			}
		}
		else if (key_flag == 1)
			power_flag = 0;
		key[3].key_flag = 0;
	}
	else if ((key[4].key_flag == 1) && (mode == 1))
	{
		if(key_flag == 0)
		{
			key_flag = 1;
		}
		else if (key_flag == 1)
		{
			set_time.hour = end_time.hour;
			set_time.minute = end_time.minute;
			set_time.second = end_time.second;
			key_flag = 2;
			time_set_flag = 1;
		}
		else if (key_flag == 2)
		{
			end_time.hour = 0;
			end_time.minute = 0;
			end_time.second = 0;
			time_set_flag = 0;
			key_flag = 0;
			power_flag = 0;
			time_i = 0;
		}
		key[4].key_flag = 0;
	}
	else
	{
		key[0].key_flag = 0;
		key[1].key_flag = 0;
		key[2].key_flag = 0;
		key[3].key_flag = 0;
		key[4].key_flag = 0;
	}
	
}

