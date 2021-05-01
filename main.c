/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "mbed.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "ventilo_led.c"
#include "servo.c"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lib_lcd.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
static rgb_lcd lcdData;
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

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
        HAL_StatusTypeDef ret;
        // saves return variable to handle errors aproppiately
        uint16_t ST, SRH;
        //values to store raw temperature and humidity
        static const uint8_t SHT31_ADDRS = 0x44 << 1,
                LCD_ADDRS = 0x7c,
                RGB_ADDRS = 0xc4;
        uint16_t SHT31_MEAS_HIGHREP = 0x062c;
        uint8_t com;
        float humidity;
        int position;
        int verif_servo, verif_led, verif_ventil; 

        uint8_t buf[12], serial_text[12], serial_hum[12];//general use buffer
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
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

  lcd_init(&hi2c1, &lcdData); // initialise le lcd
  lcd_position(&hi2c1,0,0);//cositas
  lcd_print(&hi2c1,"Starting");
  int i, delay=100;
  HAL_Delay(delay);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
        ret = HAL_I2C_Master_Transmit(
                &hi2c1,
                SHT31_ADDRS,
                &SHT31_MEAS_HIGHREP,
                2,
                HAL_MAX_DELAY);

        if(ret != HAL_OK)
        {
            strcpy((char*)serial_text, "Error Tx\r\n");
        }
         else 
        {
            ret = HAL_I2C_Master_Receive(&hi2c1, SHT31_ADDRS, &buf, 2, HAL_MAX_DELAY);
            if( ret != HAL_OK){strcpy((char*)buf, "Error Tx2\r\n");}
            else 
            {
                ST = buf[0];
                ST <<= 8;
                ST |= buf[1];
                double stemp = ST;
                stemp = ST;
                stemp *= 175;
                stemp /= 0xffff;
                stemp = -45 + stemp;
                sprintf((char*)serial_text,"%u.%0.u",((unsigned int) stemp),(((unsigned int) stemp  % 100)*100));
                humidity = (((buf[3] * 256) + buf[4])) * 1000.0 / 65535.0;
                sprintf((char*)serial_hum,"%u.%0.u",((unsigned int) humidity),(((unsigned int) humidity  % 100)*100));
            }
        }

        HAL_UART_Transmit(&huart2, serial_text, strlen((char*)serial_text), HAL_MAX_DELAY);
        HAL_Delay(500);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    clearlcd();
          lcd_position(&hi2c1,0,0);
          lcd_print(&hi2c1,"Temp:    Hum:");
          lcd_position(&hi2c1,0,1);
          lcd_print(&hi2c1, serial_text);
          lcd_position(&hi2c1,5,1);
          lcd_print(&hi2c1," C");
          lcd_position(&hi2c1,9,1);
          lcd_print(&hi2c1, serial_hum);
          lcd_position(&hi2c1,13,1);
          lcd_print(&hi2c1," %");
          reglagecouleur(255,255,255);
          lcd_position(&hi2c1,15,1);
          lcd_print(&hi2c1," ");
          HAL_Delay(500);

          htim2.Instance->CCR1 = 75;
          verif_servo = servoinit(verif_servo);
          verif_led = ledinit(verif_led);
          verif_ventil = ventilinit(verif_ventil);
          
      if(serial_text > 23)//cas température trop élevé
      {
          if(verif_led==1)//on éteint les leds
          {
              verif_led = ledOnOff(verif_led);
          }
          if(verif_servo==0)// on ouvre le trappe
          {
              verif_servo = servoinit(verif_servo);
          }
          if(verif_ventil==0)//
          {
              verif_ventil = ventiloOnOff(verif_ventil);
          }      
      }
      else 
      {
            if(serial_text < 24)//cas température trop faible
            {
                if(verif_led==0)
              {
                   verif_led = ledOnOff(verif_led);
              }
                if(verif_servo==1)
              {
              verif_servo = servoinit(verif_servo);
             }
             if(verif_ventil==1)
             {
              verif_led = ventiloOnOff(verif_ventil);
              }    
            }
            else
            {
             if(verif_led==1)
              {
                   verif_led = ledOnOff(verif_led);
              }
                if(verif_led==1)
              {
              verif_servo = servoinit(verif_servo);
             }
             if(verif_ventil==1)
             {
              verif_ventil = ventiloOnOff(verif_ventil);
              }  
            }
        }
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV3;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
