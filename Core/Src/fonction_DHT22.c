/*
 * timer.c
 *
 *  Created on: Feb 13, 2021
 *      Author:KACEMI
 */
#include "fonction_DHT22.h"
#include "timer.h"
#include "stm32l1xx_hal.h"

void Data_Output (GPIO_TypeDef *PORT, uint16_t PIN, int *testO)
  {
  	GPIO_InitTypeDef GPIO_InitStruct = {0};

  	GPIO_InitStruct.Pin = PIN;
  	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; //pushpull
  	//GPIO_InitStruct.Pull = GPIO_NOPULL;
  	//GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  	HAL_GPIO_Init(PORT, &GPIO_InitStruct);
  	//(*testO)++;
  }

void Data_Input (GPIO_TypeDef *PORT, uint16_t PIN, int *testI)
  {
  	GPIO_InitTypeDef GPIO_InitStruct = {0};

  	GPIO_InitStruct.Pin = PIN;
  	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  	//GPIO_InitStruct.Pull = GPIO_PULLUP;
  	HAL_GPIO_Init(PORT, &GPIO_InitStruct);
  	//(*testI)++;
  }

void Read_data (uint8_t *data)
  {
  	int i, k;
  	for (i=0;i<8;i++)
  	{
  		if (HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET)    // if the pin is low
  		{
  			(*data)&= ~(1<<(7-i));   // write 0
  			//data[i] = 0;
  			while(!(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)));
  			DWT_Delay_us(40);
  		}
  		else
  		{
  			(*data)|= (1<<(7-i));  // if the pin is high, write 1
  			//data[i] = 1;
  			for (k=0;k<100;k++)
  			{
  				if (HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET)
  				  {
  				  	break;
  				  }
  			}
  			while(!(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)));
  			DWT_Delay_us(40);
  		}
  	 }
  }
