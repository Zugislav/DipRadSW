/*
 * Touch_screen.h
 *
 *  Created on: 5 May 2024
 *      Author: Ivan Žugaj
 */

#ifndef SRC_STM32_PLC_TOUCH_SCREEN_TOUCH_SCREEN_H_
#define SRC_STM32_PLC_TOUCH_SCREEN_TOUCH_SCREEN_H_

#include "stdbool.h"
#include "gpio.h"
#include "stdio.h"
#include "stdint.h"

/* LCD */
void STM32_PLC_LCD(SPI_HandleTypeDef *lcdSpi, SPI_HandleTypeDef *touchSpi, GPIO_TypeDef *LCD_CS_PORT, uint16_t LCD_CS_PIN, GPIO_TypeDef *LCD_DC_PORT, uint16_t LCD_DC_PIN, GPIO_TypeDef *LCD_RST_PORT, uint16_t LCD_RST_PIN);

/* Frames with no frame id */
void  STM32_PLC_LCD_Show_Main_Frame(uint8_t *frame_id);

/* Numpad Frame */
uint8_t STM32_PLC_LCD_Show_Numpad_Frame();

/* Logics */
void STM32_PLC_LCD_Call_Main_Logic(uint8_t *frame_id);
uint8_t STM32_PLC_LCD_Call_Numpad_Logic();

#endif /* SRC_STM32_PLC_TOUCH_SCREEN_TOUCH_SCREEN_H_ */

void printEncoderValue();
void printPointerToValue();
