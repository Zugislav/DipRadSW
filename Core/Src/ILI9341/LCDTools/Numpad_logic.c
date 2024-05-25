/*
 * Numpad_frame.c
 *
 *  Created on: 5 May 2024
 *      Author: Ivan Žugaj
 */

#include "Touch_screen.h"
#include "ILI9341.h"
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
	ILI9341_fill_rect(10, 20, 300, 100, COLOR_BLACK);
	ILI9341_print_text("Invalid input", 15, 25, COLOR_WHITE, COLOR_BLACK, 3);
}

static void printEncoderUpdate(){
	// Print the encoder value
	ILI9341_fill_rect(10, 70, 310, 110, COLOR_BLACK);
	char str[5];
	for(int i = 0; i < 4; i++){
		str[i] = encoderValue.value[i] + '0';
	}
	str[4] = '.';
	char str2[7];
	ILI9341_print_text(str, 50, 80, COLOR_WHITE, COLOR_BLACK, 3);
	for(int i = 0; i < 6; i++){
		str2[i] = encoderValue.value[i + 4] + '0';
	}
	ILI9341_print_text(str2, 130, 80, COLOR_WHITE, COLOR_BLACK, 3);
	ILI9341_print_text("MHz", 240, 80, COLOR_WHITE, COLOR_BLACK, 3);
}

static void InputNumber(bool hasDot, uint8_t k, uint8_t freqPointer, uint8_t value){
	if (hasDot == false) {
		// If we don't have a dot
		if(freqPointer == 0){
			if(k > 0){
				//print out text that input is invalid
				printInvalidInput();
			}
			else{
				encoderValue.value[freqPointer] = value;
			}
		}
		if(freqPointer == 3){
			if(k > 3){
				//print out text that input is invalid
				printInvalidInput();
			}
			else{
				//shift all the values to the left
				for(int i = freqPointer; i >= 1; i++){
					encoderValue.value[i - 1] = encoderValue.value[i];
				}
				encoderValue.value[freqPointer] = value;
			}
		}
		if(freqPointer == 6){
			if(k > 6){
				//print out text that input is invalid
				printInvalidInput();
			}
			else{
				//shift all the values to the left
				for(int i = freqPointer; i >= 1; i++){
					encoderValue.value[i - 1] = encoderValue.value[i];
				}
				encoderValue.value[freqPointer] = value;
			}
		}
	} else {
		if(freqPointer == 0){
			if(k > 9){
				//print out text that input is invalid
				printInvalidInput();
			}
			else{
				for(int i = 9; i > freqPointer + 1; i--){
					encoderValue.value[i] = encoderValue.value[i - 1];
				}
				encoderValue.value[freqPointer + 1] = value;
			}
		}
		if(freqPointer == 3){
			if(k > 7){
				//print out text that input is invalid
				printInvalidInput();
			}
			else{
				//shift all the values to the right from dot to the end
				for(int i = 9; i > freqPointer + 1; i--){
					encoderValue.value[i] = encoderValue.value[i - 1];
				}
				encoderValue.value[freqPointer + 1] = value;
			}
		}
		if(freqPointer == 6){
			if(k > 3){
				//print out text that input is invalid
				printInvalidInput();
			}
			else{
				//shift all the values to the right from dot to the end
				for(int i = 9; i > freqPointer + 1; i--){
					encoderValue.value[i] = encoderValue.value[i - 1];
				}
				encoderValue.value[freqPointer + 1] = value;
			}
		}
	}
}

uint8_t STM32_PLC_LCD_Call_Numpad_Logic() {

	bool haveDot = false;
	uint8_t k = 0;
	uint8_t freqPointer = 3;
	while (1) {
		if (TSC2046_isPressed(true)) {
			TSC2046_GetTouchData();
			uint16_t X = lcd.myTsData.X;
			uint16_t Y = lcd.myTsData.Y;

			/* Check which button we are pressing on */
			if (X >= 5 && X <= 67 && Y >= 115 && Y <= 165){
				//GHz button
				memset(encoderValue.value, 0, sizeof(encoderValue.value));
				freqPointer = GHZ_POINTER;
				k = 0;
			}
			else if (X >= 5 && X <= 67 && Y >= 60 && Y <= 110){
				//MHz button
				memset(encoderValue.value, 0, sizeof(encoderValue.value));
				freqPointer = MHZ_POINTER;
				k = 0;
			}
			else if (X >= 5 && X <= 67 && Y >= 5 && Y <= 60){
				//kHz button
				memset(encoderValue.value, 0, sizeof(encoderValue.value));
				freqPointer = KHZ_POINTER;
				k = 0;
			}
			else if (X >= 67 && X <= 129 && Y >= 115 && Y <= 165){
				//1 button
				InputNumber(haveDot, k, freqPointer, 1);
				printEncoderUpdate();
				k++;
			}
			else if (X >= 67 && X <= 129 && Y >= 60 && Y <= 115){
				//4 button
				InputNumber(haveDot, k, freqPointer, 4);
				printEncoderUpdate();
				k++;
			}
			else if (X >= 67 && X <= 129 && Y >= 5 && Y <= 60){
				//7 button
				InputNumber(haveDot, k, freqPointer, 7);
				printEncoderUpdate();
				k++;
			}
			else if (X >= 129 && X <= 191 && Y >= 115 && Y <= 165){
				//2 button
				InputNumber(haveDot, k, freqPointer, 2);
				printEncoderUpdate();
				k++;
			}
			else if (X >= 129 && X <= 191 && Y >= 60 && Y <= 115){
				//5 button
				InputNumber(haveDot, k, freqPointer, 5);
				printEncoderUpdate();
				k++;
			}
			else if (X >= 129 && X <= 191 && Y >= 5 && Y <= 60){
				//8 button
				InputNumber(haveDot, k, freqPointer, 8);
				printEncoderUpdate();
				k++;
			}
			else if (X >= 191 && X <= 253 && Y >= 115 && Y <= 165){
				//3 button
				InputNumber(haveDot, k, freqPointer, 3);
				printEncoderUpdate();
				k++;
			}
			else if (X >= 191 && X <= 253 && Y >= 60 && Y <= 115){
				//6 button
				InputNumber(haveDot, k, freqPointer, 6);
				printEncoderUpdate();
				k++;
			}
			else if (X >= 191 && X <= 253 && Y >= 5 && Y <= 60){
				//9 button
				InputNumber(haveDot, k, freqPointer, 9);
				printEncoderUpdate();
				k++;
			}
			else if (X >= 191 && X <= 253 && Y >= 115 && Y <= 165){
				//zero button
				InputNumber(haveDot, k, freqPointer, 0);
				printEncoderUpdate();
				k++;
			}
			else if (X >= 191 && X <= 253 && Y >= 60 && Y <= 115){
				//point button
				if(haveDot == false){
					haveDot = true;
					k = 0;
				}
			}
			else if (X >= 191 && X <= 253 && Y >= 5 && Y <= 60){
				//ENTER button
				return 0;
			}
		}
	}
}
