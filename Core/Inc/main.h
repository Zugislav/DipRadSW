/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Button1_Pin GPIO_PIN_2
#define Button1_GPIO_Port GPIOE
#define Button1_EXTI_IRQn EXTI2_IRQn
#define Button2_Pin GPIO_PIN_3
#define Button2_GPIO_Port GPIOE
#define Button2_EXTI_IRQn EXTI3_IRQn
#define Button3_Pin GPIO_PIN_4
#define Button3_GPIO_Port GPIOE
#define Button3_EXTI_IRQn EXTI4_IRQn
#define Button4_Pin GPIO_PIN_5
#define Button4_GPIO_Port GPIOE
#define Button4_EXTI_IRQn EXTI9_5_IRQn
#define Button5_Pin GPIO_PIN_6
#define Button5_GPIO_Port GPIOE
#define Button5_EXTI_IRQn EXTI9_5_IRQn
#define RST_Pin GPIO_PIN_3
#define RST_GPIO_Port GPIOA
#define LCD_CS_Pin GPIO_PIN_4
#define LCD_CS_GPIO_Port GPIOA
#define T_IRQ_Pin GPIO_PIN_4
#define T_IRQ_GPIO_Port GPIOC
#define BOOT1_Pin GPIO_PIN_2
#define BOOT1_GPIO_Port GPIOB
#define Button6_Pin GPIO_PIN_7
#define Button6_GPIO_Port GPIOE
#define Button6_EXTI_IRQn EXTI9_5_IRQn
#define LED1_Pin GPIO_PIN_8
#define LED1_GPIO_Port GPIOE
#define LED2_Pin GPIO_PIN_9
#define LED2_GPIO_Port GPIOE
#define LED3_Pin GPIO_PIN_10
#define LED3_GPIO_Port GPIOE
#define LED4_Pin GPIO_PIN_11
#define LED4_GPIO_Port GPIOE
#define LED5_Pin GPIO_PIN_12
#define LED5_GPIO_Port GPIOE
#define LED6_Pin GPIO_PIN_13
#define LED6_GPIO_Port GPIOE
#define EN_SW_Pin GPIO_PIN_11
#define EN_SW_GPIO_Port GPIOB
#define SD_NSS_Pin GPIO_PIN_12
#define SD_NSS_GPIO_Port GPIOB
#define SD_SCK_Pin GPIO_PIN_13
#define SD_SCK_GPIO_Port GPIOB
#define SD_MISO_Pin GPIO_PIN_14
#define SD_MISO_GPIO_Port GPIOB
#define SD_MOSI_Pin GPIO_PIN_15
#define SD_MOSI_GPIO_Port GPIOB
#define GPIO9_Pin GPIO_PIN_8
#define GPIO9_GPIO_Port GPIOD
#define GPIO10_Pin GPIO_PIN_9
#define GPIO10_GPIO_Port GPIOD
#define GPIO11_Pin GPIO_PIN_10
#define GPIO11_GPIO_Port GPIOD
#define GPIO12_Pin GPIO_PIN_11
#define GPIO12_GPIO_Port GPIOD
#define GPIO13_Pin GPIO_PIN_12
#define GPIO13_GPIO_Port GPIOD
#define GPIO14_Pin GPIO_PIN_13
#define GPIO14_GPIO_Port GPIOD
#define GPIO15_Pin GPIO_PIN_14
#define GPIO15_GPIO_Port GPIOD
#define GPIO16_Pin GPIO_PIN_15
#define GPIO16_GPIO_Port GPIOD
#define T_CLK_Pin GPIO_PIN_10
#define T_CLK_GPIO_Port GPIOC
#define T_MISO_Pin GPIO_PIN_11
#define T_MISO_GPIO_Port GPIOC
#define T_MOSI_Pin GPIO_PIN_12
#define T_MOSI_GPIO_Port GPIOC
#define GPIO1_Pin GPIO_PIN_0
#define GPIO1_GPIO_Port GPIOD
#define GPIO2_Pin GPIO_PIN_1
#define GPIO2_GPIO_Port GPIOD
#define GPIO3_Pin GPIO_PIN_2
#define GPIO3_GPIO_Port GPIOD
#define GPIO4_Pin GPIO_PIN_3
#define GPIO4_GPIO_Port GPIOD
#define GPIO5_Pin GPIO_PIN_4
#define GPIO5_GPIO_Port GPIOD
#define GPIO6_Pin GPIO_PIN_5
#define GPIO6_GPIO_Port GPIOD
#define GPIO7_Pin GPIO_PIN_6
#define GPIO7_GPIO_Port GPIOD
#define GPIO8_Pin GPIO_PIN_7
#define GPIO8_GPIO_Port GPIOD
#define DC_RS_Pin GPIO_PIN_0
#define DC_RS_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
