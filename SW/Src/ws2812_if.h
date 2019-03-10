/*
 * ws2812_if.h
 *
 *  Created on: 10.03.2019
 *      Author: Mati
 */

#ifndef WS2812_IF_H_
#define WS2812_IF_H_
#include "ws2812.h"

#define MY_LED_STRING_LEN 25

extern led_t tmp_led;
extern uint8_t update_flag;
void WS2812_Main(void);
void WS2812_Cfg(void);
#endif /* WS2812_IF_H_ */
