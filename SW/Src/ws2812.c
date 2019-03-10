/*
 * ws2812.c
 *
 *  Created on: 05.03.2019
 *      Author: Mati
 */

#include "stm32f0xx_hal.h"
#include "ws2812.h"
#include "stm32f0xx_ll_hal_dma.h"
#include "string.h"
static uint8_t tip;
void WS2812_DmaHandler(void)
{

	/* USER CODE BEGIN DMA1_Channel2_3_IRQn 0 */
	DMA1->IFCR = DMA_IFCR_CGIF3;
	while ((SPI1->SR & SPI_SR_FTLVL) != 0)
	{
		;
	}
	while ((SPI1->SR & SPI_SR_BSY) == SPI_SR_BSY)
	{
		;
	}

	SPI1->CR2 &= ~(SPI_CR2_TXDMAEN);
	TIM3->CR1 &= ~(TIM_CR1_CEN);
	tip = 0;
}

void WS2812_SetBr(rgb_t* data, uint16_t len, uint8_t br)
{
	uint16_t i;
	for (i = 0; i < len; i++)
	{
		(data + i)->r = (uint8_t) ((uint16_t) (((data + i)->r) * br) / 0xff);
		(data + i)->g = (uint8_t) ((uint16_t) (((data + i)->g) * br) / 0xff);
		(data + i)->b = (uint8_t) ((uint16_t) (((data + i)->b) * br) / 0xff);
	}
}
void WS2812_SetUniColor(rgb_t* data, uint16_t len, led_t pixel)
{
	uint16_t i;
	rgb_t rgb_tmp;
	rgb_tmp.r = (uint8_t) ((uint16_t) (pixel.r * pixel.br) / 0xff);
	rgb_tmp.g = (uint8_t) ((uint16_t) (pixel.g * pixel.br) / 0xff);
	rgb_tmp.b = (uint8_t) ((uint16_t) (pixel.b * pixel.br) / 0xff);
	for (i = 0; i < len; i++)
	{
		memcpy((void*) (data + i), (void*) &rgb_tmp, sizeof(rgb_t));
	}
}


int8_t WS2812_SendData(rgb_t* data, uint16_t len, uint8_t row)
{
	if (tip == 0)
	{
		tip = 1;

		LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_3);
		DMA1_Channel3->CMAR = data;
		DMA1_Channel3->CPAR = (uint32_t*) &SPI1->DR;
		DMA1_Channel3->CNDTR = (uint16_t) (sizeof(rgb_t) * len);
		LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_3);
		SPI1->CR2 |= SPI_CR2_TXDMAEN;
		while ((SPI1->SR & SPI_SR_FTLVL) != (SPI_SR_FTLVL_1+SPI_SR_FTLVL_0))
		{
			;
		}
		TIM3->CR1 |= TIM_CR1_CEN;
		return 0;
	}
	else
		return -1;

}


void WS2812_Init(rgb_t* data, uint16_t len)
{
	GPIO_InitTypeDef gpio;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio.Pull = GPIO_NOPULL;
	gpio.Pin = GPIO_PIN_7;
	HAL_GPIO_Init(GPIOB, &gpio);
	GPIOB->BSRR = GPIO_BSRR_BS_6 |GPIO_BSRR_BR_7;
	DMA1_Channel3->CCR |= DMA_CCR_TCIE;
	DMA1_Channel3->CMAR = data;
	DMA1_Channel3->CPAR = (uint32_t*) &SPI1->DR;
	DMA1_Channel3->CNDTR = (uint16_t) (sizeof(rgb_t) * len);

	TIM3->BDTR |= TIM_BDTR_MOE;
	TIM3->CCER = TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E;

	SPI1->CR1 |= SPI_CR1_SPE;
	GPIOB->BSRR = GPIO_BSRR_BS_6 | GPIO_BSRR_BR_7;

}
