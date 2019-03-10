/*
 * ws2812_if.c
 *
 *  Created on: 10.03.2019
 *      Author: Mati
 */

#include "stm32f0xx_hal.h"
#include "ws2812_if.h"
#include "string.h"
led_t tmp_led;
uint8_t update_flag;
rgb_t myLedString[MY_LED_STRING_LEN];
void WS2812_Cfg(void)
{
	memset(&tmp_led,0,sizeof(led_t));
	memset(myLedString,0,sizeof(rgb_t)*MY_LED_STRING_LEN);
	WS2812_SetUniColor(myLedString, MY_LED_STRING_LEN, tmp_led);
	WS2812_Init(myLedString, MY_LED_STRING_LEN);
	update_flag = 1;
}
void WS2812_Main(void)
{
	uint16_t i;
	if (update_flag == 1)
	{
		update_flag = 0;
		WS2812_SetUniColor(myLedString, MY_LED_STRING_LEN, tmp_led);
		WS2812_SendData(myLedString, MY_LED_STRING_LEN, 0);
		for (i = 0; i < 0xfff; i++)
		{
			;
		}

	}
}
