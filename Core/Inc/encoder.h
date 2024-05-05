/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
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
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "cmsis_os.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "cmsis_os2.h"
#include "task.h"
#include "queue.h"
#include "usart.h"
#include "usb_device.h"
#include "stdbool.h"
/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
#define ENCODER_PULSES_PER_ROTATION 24

/* USER CODE END Private defines */
/* USER CODE BEGIN Prototypes */
typedef struct{
	uint8_t dir;
	uint16_t cnt1, cnt2, diff, speed, one_rotation_pulses;
	uint32_t tick;
	TIM_HandleTypeDef* htim;
}encoderHandle;

void Encoder_init(encoderHandle *encoder, TIM_HandleTypeDef *htim, uint16_t one_rotation_pulses);
void Encoder_count(encoderHandle *encoder);
float Encoder_getSpeed(encoderHandle *encoder);
uint16_t Encoder_getDifference(encoderHandle *encoder);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

