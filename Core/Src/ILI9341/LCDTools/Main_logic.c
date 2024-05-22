/*
 * Main_Logic.c
 *
 *  Created on: 5 May 2024
 *      Author: Ivan Žugaj
 */

#include "Touch_screen.h"
#include "ILI9341.h"
#include "message.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "cmsis_os2.h"
#include "task.h"
#include "queue.h"
#include "limits.h"
#include "string.h"
#include "math.h"


extern EncoderValue encoderValue;
extern osMessageQueueId_t mainQueueHandle;
extern osMessageQueueId_t encoderValueHandle;

static int16_t encoderDiff = 0;
static uint32_t buttonValue = 0;

static void printEncoderValue(){
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

static void LevelAllRightOfPointerToZero(){
	// Set all values to the right of the pointer to 0
	for(int i = encoderValue.pointerToValue; i < 10; i++){
		encoderValue.value[i] = 0;
	}
	printEncoderValue();
}

static void updateEncoderValue(int16_t difference) {

	uint8_t full = 0;
	// Handle positive difference
	if (difference >= 0) {
		// Add the new value at the pointer position
		encoderValue.value[encoderValue.pointerToValue - 1] += difference;

		// Handle carry-over (if needed)
		for (int i = encoderValue.pointerToValue - 1; i >= 1; i--) {
		// Add carry-over
		encoderValue.value[i - 1] += encoderValue.value[i] / 10;
		if(i == 1){
			if(encoderValue.value[i - 1] >= 10){
				encoderValue.value[i - 1] = 9;
				full = 1;
			}
		}
		if(full == 1){
			//check if the second value is full
			for(int j = 1; j < encoderValue.pointerToValue; j++){
				//set all digits to 9
				encoderValue.value[j] = 9;
			}
		}
		encoderValue.value[i] %= 10;  // Limit previous digit to 0-9
		}
	} 
	else if(difference < 0){
			// Handle borrowing (if needed)
			uint8_t borrow = 0;  // Flag to indicate borrowing
			uint16_t value = encoderValue.value[encoderValue.pointerToValue - 1];  // Temporary variable to store the value
			for (int j = encoderValue.pointerToValue - 1; j >= 1; j--){
				// Perform subtraction and handle borrowing
				//find the first number that is bigger then zero
				if(-difference > value){
					//find the next significant value that is bigger then zero
					if(encoderValue.value[j - 1] > 0){
						value += encoderValue.value[j - 1] * 10;
							if(value > -difference){
								value += difference;
								borrow = j;
								break;
							}
					}
				}
				else if(-difference <= value){
					encoderValue.value[encoderValue.pointerToValue - 1] += difference;
					break;
				}
			}
			if(borrow > 0){
				//handle borrowing
				uint8_t tmpArr[10] = {0};
				for(int i = 0; i < 10; i++){
					tmpArr[i] = value % 10;
					value /= 10;
					if(value == 0) break;
				}
				uint8_t tmp = 0;
				for(int j = borrow; j >= 0; j--){
					encoderValue.value[j] = tmpArr[tmp++];
				}
			}
			else{
				//there where no bigger values so we will set all the values right of pointer to 9
				for(int i = 0; i < encoderValue.pointerToValue; i++){
					encoderValue.value[i] = 0;
				}
				for(int i = encoderValue.pointerToValue; i < 10; i++){
					encoderValue.value[i] = 9;
				}
			}
		}

		// Ensure all values are within 0-9 range
		for (int i = 0; i < encoderValue.len; i++) {
			encoderValue.value[i] = (encoderValue.value[i] % 10 + 10) % 10; // Modulo 10 twice for positive values > 9
		}
}

static void printPointerToValue(){
	// Print the pointer to the value
	if(encoderValue.pointerToValue < 4 && encoderValue.pointerToValue > 0){
		ILI9341_print_text("^", 105 - (4 - encoderValue.pointerToValue) * 20, 110, COLOR_RED, COLOR_BLACK, 3);
	}
	else if(encoderValue.pointerToValue >= 4 && encoderValue.pointerToValue <= 10){
		ILI9341_print_text("^", 105 + (encoderValue.pointerToValue - 4) * 20 , 110, COLOR_RED, COLOR_BLACK, 3);
	}
}
void STM32_PLC_LCD_Call_Main_Logic(uint8_t *frame_id) {
	if (TSC2046_isPressed()) {
		TSC2046_GetTouchData();
		uint16_t X = lcd.myTsData.X;
		uint16_t Y = lcd.myTsData.Y;
		/* Check which button we are pressing on */
		if (X >= 5 && X <= 83 && Y >= 5 && Y <= 35) {            /* First icon */
			//go left
			ILI9341_fill_rect(10, 110, 310, 130, COLOR_BLACK);
			encoderValue.pointerToValue--;
			printPointerToValue();
		} 
		else if (X >= 5 && X <= 315 && Y >= 35 && Y <= 53) {			
			//go right
			ILI9341_fill_rect(10, 110, 310, 130, COLOR_BLACK);
			encoderValue.pointerToValue++;
			printPointerToValue();
		}
		else if (X >= 5 && X <= 315 && Y >= 53 && Y <= 80) {
			//LEVEL all
			LevelAllRightOfPointerToZero();
		} 
		else if (X >= 5 && X <= 315 && Y >= 80 && Y <= 107) {
			//Keyboard
			STM32_PLC_LCD_Show_Numpad_Frame();
			STM32_PLC_LCD_Show_Main_Frame(frame_id);
		} 
	}
	buttonValue = 0;
	//wait for the buttons to be pressed (left, right, keyboard and level)
	if(xTaskNotifyWait(0, ULONG_MAX, &buttonValue, 100) == pdTRUE)
    {
		switch(buttonValue){
			case BUTTON3_IRQ:
				//pointer to value goes to left
				ILI9341_fill_rect(10, 110, 310, 130, COLOR_BLACK);
				encoderValue.pointerToValue--;
				printPointerToValue();
				break;
			case BUTTON4_IRQ:
				//LEVEL all right of the pointer to 0
				LevelAllRightOfPointerToZero();
				break;
			case BUTTON5_IRQ:
				//pointer goes to right
				ILI9341_fill_rect(10, 110, 310, 130, COLOR_BLACK);
				encoderValue.pointerToValue++;
				printPointerToValue();
				break;
			case BUTTON6_IRQ:
				//KEYBOARD call
				STM32_PLC_LCD_Show_Numpad_Frame();
				STM32_PLC_LCD_Show_Main_Frame(frame_id);
				break;
		}
    }
	encoderDiff = 0;
	if(xQueueReceive(encoderValueHandle, &encoderDiff, 100) == pdTRUE)
	{
		updateEncoderValue(encoderDiff);
		//now update the value on screen
		printEncoderValue();
		//update value of the encoder
	}
}
