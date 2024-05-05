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

enum Frame_ID{
    MAIN_FRAME,
    ENCODER_FRAME,
    KEYBOARD_FRAME,
    FRAME3,
    FRAME4,
    FRAME5,
    FRAME6,
    ABOUT_STM32_PLC_FRAME
};

/* LCD */
void STM32_PLC_LCD(SPI_HandleTypeDef *lcdSpi, SPI_HandleTypeDef *touchSpi, GPIO_TypeDef *LCD_CS_PORT, uint16_t LCD_CS_PIN, GPIO_TypeDef *LCD_DC_PORT, uint16_t LCD_DC_PIN, GPIO_TypeDef *LCD_RST_PORT, uint16_t LCD_RST_PIN);
void STM32_PLC_LCD_Calibrate_Touch();
bool STM32_PLC_LCD_Is_Pressed();
void STM32_PLC_LCD_Get_Touch_Calibration_Parameters(float *Scale_X, float *Scale_Y, float *Bias_X, float *Bias_Y);
void STM32_PLC_LCD_Set_Touch_Calibration_Parameters(float *Scale_X, float *Scale_Y, float *Bias_X, float *Bias_Y);
void STM32_PLC_LCD_Get_Touch_Data();

/* Frames with no frame id */
void STM32_PLC_LCD_Show_Touch_Screen_Calibration_Message_Frame();
void STM32_PLC_LCD_Show_Plot_Frame();

void  STM32_PLC_LCD_Show_Main_Frame(uint8_t *frame_id);

/* Numpad Frame */
uint8_t STM32_PLC_LCD_Show_Numpad_Frame(bool decimalbutton_show, bool minusbutton_show, float *number_value, char title[]);

/* Keyboard Frame */
uint8_t STM32_PLC_LCD_Show_Keyboard_Frame(char word[], char title[]);

/* Encoder Frame*/
void STM32_PLC_LCD_Show_Encoder_Revolutions_Settings_Frame(uint8_t *frame_id);

/* Dialogs */
uint8_t STM32_PLC_LCD_Show_Question_Yes_No_Dialog(char question[]);
uint8_t STM32_PLC_LCD_Show_Information_OK_Dialog(char information[]);

/* Logics */
void STM32_PLC_LCD_Call_Main_Logic(uint8_t *frame_id);
uint8_t STM32_PLC_LCD_Call_Numpad_Logic(bool decimalbutton_show, bool minusbutton_show, float *number_value);
uint8_t STM32_PLC_LCD_Call_Keyboard_Logic(char word[]);
uint8_t STM32_PLC_LCD_Call_Two_Button_Logic(uint16_t b1_x1, uint16_t b1_y1, uint16_t b1_x2, uint16_t b1_y2, uint16_t b2_x1, uint16_t b2_y1, uint16_t b2_x2, uint16_t b2_y2);
uint8_t STM32_PLC_LCD_Call_One_Button_Logic(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

#endif /* SRC_STM32_PLC_TOUCH_SCREEN_TOUCH_SCREEN_H_ */
