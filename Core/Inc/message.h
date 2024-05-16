/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    message.h
  * @brief   This file contains all the function prototypes for
  *          the message.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 Ivan Žugaj.
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
#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

/* USER CODE BEGIN Prototypes */
enum IRQType{
    BUTTON1_IRQ,
    BUTTON2_IRQ,
    BUTTON3_IRQ,
    BUTTON4_IRQ,
    BUTTON5_IRQ,
    BUTTON6_IRQ,
    ENCODER_IRQ,
    TOUCHPAD_IRQ,
    LCD_IRQ
};
/* USER CODE END Prototypes */

typedef struct{
  uint8_t value[10];
  uint8_t len;
  uint8_t pointerToValue;
}EncoderValue;

#ifdef __cplusplus
}
#endif
#endif /*__ MESSAGE_H__ */

