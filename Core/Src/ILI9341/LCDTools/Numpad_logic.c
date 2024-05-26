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

static void checkInputValue(uint32_t inputValue[], uint8_t dotPlace, uint8_t freq, uint8_t numbers){
	if(freq == GHZ_POINTER){
		encoderValue.value[0] = inputValue[0];
		if(dotPlace != 1 && numbers > 1){

		}
	}
	if(freq == MHZ_POINTER){

	}
	if(freq == KHZ_POINTER){

	}
}

uint8_t STM32_PLC_LCD_Call_Numpad_Logic() {

	uint8_t haveDot = 0;
	uint8_t k = 0;
	uint16_t x, y;
	uint32_t inputVariable[10] = {0};
	memset(encoderValue.value, 0, sizeof(encoderValue.value));
	while (1) {
		if (ILI9341_TouchGetCoordinates(&x, &y)) {
			/* Check which button we are pressing on */
			if (x >= 253 && x <= 315 && y >= 115 && y <= 165){
				//GHz button
				checkInputValue(inputVariable, haveDot, GHZ_POINTER, k);
				printEncoderUpdate();	
			}
			else if (x >= 5 && x <= 67 && y >= 60 && y <= 115){
				//MHz button
				checkInputValue(inputVariable, haveDot, MHZ_POINTER, k);
				printEncoderUpdate();
			}
			else if (x >= 5 && x <= 67 && y >= 5 && y <= 60){
				//kHz button
				checkInputValue(inputVariable, haveDot, KHZ_POINTER, k);
				printEncoderUpdate();
			}
			else if (x >= 67 && x <= 129 && y >= 115 && y <= 165){
				//1 button
				inputVariable[k++] = 1;
			}
			else if (x >= 67 && x <= 129 && y >= 60 && y <= 115){
				//4 button
				inputVariable[k++] = 4;
			}
			else if (x >= 67 && x <= 129 && y >= 5 && y <= 60){
				//7 button
				inputVariable[k++] = 7;
			}
			else if (x >= 129 && x <= 191 && y >= 115 && y <= 165){
				//2 button
				inputVariable[k++] = 2;
			}
			else if (x >= 129 && x <= 191 && y >= 60 && y <= 115){
				//5 button
				inputVariable[k++] = 5;
			}
			else if (x >= 129 && x <= 191 && y >= 5 && y <= 60){
				//8 button
				inputVariable[k++] = 8;
			}
			else if (x >= 191 && x <= 253 && y >= 115 && y <= 165){
				//3 button
				inputVariable[k++] = 3;
			}
			else if (x >= 191 && x <= 253 && y >= 60 && y <= 115){
				//6 button
				inputVariable[k++] = 6;
			}
			else if (x >= 191 && x <= 253 && y >= 5 && y <= 60){
				//9 button
				inputVariable[k++] = 9;
			}
			else if (x >= 5 && x <= 67 && y >= 115 && y <= 165){
				//zero button
				inputVariable[k++] = 0;
			}
			else if (x >= 191 && x <= 253 && y >= 60 && y <= 115){
				//point button
				haveDot = k;
			}
		}
	}
}
