/*
 * Numpad_frame.c
 *
 *  Created on: 5 May 2024
 *      Author: Ivan Žugaj
 */

#include "Touch_screen.h"
#include "ILI9341.h"
#include "ili9341_touch.h"
#include "string.h"
#include "message.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "cmsis_os2.h"
#include "task.h"
#include "queue.h"
#include "limits.h"

extern EncoderValue encoderValue;

#define GHZ_POINTER 0
#define MHZ_POINTER 3
#define KHZ_POINTER 6

static void printInvalidInput(){
	ILI9341_fill_rect(10, 20, 300, 65, COLOR_BLACK);
	ILI9341_print_text("Invalid input", 85, 25, COLOR_WHITE, COLOR_BLACK, 2);
}

// Print the encoder value
static void printNewEncoderValue(){
	ILI9341_fill_rect(10, 10, 310, 70, COLOR_BLACK);
	HAL_Delay(30);
	char str[6] = {0};
	for(int i = 0; i < 4; i++){
		str[i] = encoderValue.value[i] + '0';
	}
	str[4] = '.';
	char str2[7] = {0};
	ILI9341_print_text(str, 50, 20, COLOR_WHITE, COLOR_BLACK, 3);
	HAL_Delay(30);
	for(int i = 0; i < 6; i++){
		str2[i] = encoderValue.value[i + 4] + '0';
	}
	ILI9341_print_text(str2, 130, 20, COLOR_WHITE, COLOR_BLACK, 3);
	ILI9341_print_text("MHz", 240, 20, COLOR_WHITE, COLOR_BLACK, 3);
	HAL_Delay(30);
}

static bool checkInputValue(uint32_t inputValue[], uint8_t dotPlace, uint8_t freq, uint8_t numbers){
	if(freq == GHZ_POINTER){
		if(dotPlace > 1)
		{
			printInvalidInput();
			return false;
		}
		for(int i = 0; i < numbers; i++){
			encoderValue.value[i] = inputValue[i];
		}
		return true;
	}
	if(freq == MHZ_POINTER){
		if(dotPlace >= 4){
			printInvalidInput();
			return false;
		}
		if(dotPlace > 0){
			for(int i = 0; i < dotPlace; i++){
				encoderValue.value[4 - 1 - i] = *(inputValue + dotPlace - i - 1);
			}
			for(int i = 0; i < numbers - dotPlace; i++){
				encoderValue.value[4 + i] = *(inputValue + i + dotPlace);
			}
			return true;
		}
		else if (numbers < 4 && dotPlace == 0){
			for(int i = 0; i < numbers; i++){
				encoderValue.value[4 - 1 - i] = *(inputValue + numbers - i - 1);
			}
			return true;
		}
		else printInvalidInput();
		return false;
	}
	if(freq == KHZ_POINTER){
		if(dotPlace >= 7){
			printInvalidInput();
			return false;
		} 
		if(dotPlace > 0){
			for(int i = 0; i < dotPlace; i++){
				encoderValue.value[7 - 1 - i] = *(inputValue + dotPlace - i - 1);
			}
			for(int i = 0; i < numbers - dotPlace; i++){
				encoderValue.value[7 + i] = *(inputValue + i + dotPlace);
			}
			return true;
		}
		else if (numbers < 4 && dotPlace == 0){
			for(int i = 0; i < numbers; i++){
				encoderValue.value[7 - 1 - i] = *(inputValue + numbers - i - 1);
			}
			return true;
		}
		else printInvalidInput();
		return false;
	}
	return false;
}

uint8_t STM32_PLC_LCD_Call_Numpad_Logic() {

	uint8_t haveDot = 0;
	uint8_t k = 0;
	uint16_t x, y;
	uint32_t inputVariable[10] = {0};
	memset(encoderValue.value, 0, sizeof(encoderValue.value));
	while (1) {
		if (ILI9341_TouchGetCoordinates(&x, &y)) {
			HAL_Delay(150);
			/* Check which button we are pressing on */
			if (x >= 253 && x <= 315 && y >= 70 && y <= 125){
				//GHz button
				if(!checkInputValue(inputVariable, haveDot, GHZ_POINTER, k)){
					memset(inputVariable, 0, 10);
					k = 0;
					haveDot = 0;
					continue;
				}
				printNewEncoderValue();
				osDelay(2000);
				return 0;
			}
			else if (x >= 253 && x <= 315 && y >= 125 && y <= 180){
				//MHz button
				if(!checkInputValue(inputVariable, haveDot, MHZ_POINTER, k)){
					memset(inputVariable, 0, 10);
					k = 0;
					haveDot = 0;
					continue;
				}
				printNewEncoderValue();
				osDelay(2000);
				return 0;
			}
			else if (x >= 253 && x <= 315 && y >= 180 && y <= 235){
				//kHz button
				if(!checkInputValue(inputVariable, haveDot, KHZ_POINTER, k)){
					memset(inputVariable, 0, 10);
					k = 0;
					haveDot = 0;
					continue;
				}
				printNewEncoderValue();
				osDelay(2000);
				return 0;
			}
			else if (x >= 67 && x <= 129 && y >= 70 && y <= 125){
				//1 button
				inputVariable[k] = 1;
				k >= 9 ? printInvalidInput() : k++;
			}
			else if (x >= 67 && x <= 129 && y >= 125 && y <= 180){
				//4 button
				inputVariable[k] = 4;
				k >= 9 ? printInvalidInput() : k++;
			}
			else if (x >= 67 && x <= 129 && y >= 180 && y <= 235){
				//7 button
				inputVariable[k] = 7;
				k >= 9 ? printInvalidInput() : k++;
			}
			else if (x >= 129 && x <= 191 && y >= 70 && y <= 125){
				//2 button
				inputVariable[k] = 2;
				k >= 9 ? printInvalidInput() : k++;
			}
			else if (x >= 129 && x <= 191 && y >= 125 && y <= 180){
				//5 button
				inputVariable[k] = 5;
				k >= 9 ? printInvalidInput() : k++;
			}
			else if (x >= 129 && x <= 191 && y >= 180 && y <= 235){
				//8 button
				inputVariable[k] = 8;
				k >= 9 ? printInvalidInput() : k++;
			}
			else if (x >= 191 && x <= 253 && y >= 70 && y <= 125){
				//3 button
				inputVariable[k] = 3;
				k >= 9 ? printInvalidInput() : k++;
			}
			else if (x >= 191 && x <= 253 && y >= 125 && y <= 180){
				//6 button
				inputVariable[k] = 6;
				k >= 9 ? printInvalidInput() : k++;
			}
			else if (x >= 191 && x <= 253 && y >= 180 && y <= 235){
				//9 button
				inputVariable[k] = 9;
				k >= 9 ? printInvalidInput() : k++;
			}
			else if (x >= 5 && x <= 67 && y >= 70 && y <= 125){
				//zero button
				inputVariable[k] = 0;
				k >= 9 ? printInvalidInput() : k++;
			}
 			else if (x >= 5 && x <= 67 && y >= 125 && y <= 180){
				//point button
				haveDot = k;
			}
		}
	}
}
