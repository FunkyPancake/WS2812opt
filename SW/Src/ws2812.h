/*
 * ws2812.h
 *
 *  Created on: 05.03.2019
 *      Author: Mati
 */

#ifndef WS2812_H_
#define WS2812_H_

typedef struct
{
	uint8_t r, g, b, br;
} led_t;
typedef struct
{
	uint8_t g,r,b;
}rgb_t;
void WS2812_SetUniColor(rgb_t* data, uint16_t len, led_t pixel);
void WS2812_DmaHandler();
void WS2812_SetBr(rgb_t* data, uint16_t len,uint8_t br);
int8_t WS2812_SendData(rgb_t* data, uint16_t len, uint8_t row);
void WS2812_Init(rgb_t* data, uint16_t len);
#endif /* WS2812_H_ */
