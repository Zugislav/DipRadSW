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
#include "ILI9341_touch.h"
#include "usart.h"

#define FS 50000000			//fs za formulu koja pretvara odabranu frekvenciju u oblic pogodan za prijenos USART-om na SDR
#define N 32

extern EncoderValue encoderValue;
extern osMessageQueueId_t mainQueueHandle;
extern osMessageQueueId_t encoderValueHandle;
extern UART_HandleTypeDef huart4;

static int16_t encoderDiff = 0;
static uint32_t buttonValue = 0;

static void LevelAllRightOfPointerToZero(){
	// Set all values to the right of the pointer to 0
	for(int i = encoderValue.pointerToValue; i < 10; i++){
		encoderValue.value[i] = 0;
	}
	printEncoderValue();
}
static uint32_t powersOfTen(uint32_t power){
	uint32_t powersOfTen = 1;
	for(int i = 0; i < power; i++){
		powersOfTen *= 10;
	}
	return powersOfTen;
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
			int8_t borrow = -1;  // Flag to indicate borrowing
			uint16_t value = encoderValue.value[encoderValue.pointerToValue - 1];  // Temporary variable to store the value
			for (int j = encoderValue.pointerToValue - 1; j >= 1; j--){
				// Perform subtraction and handle borrowing
				//find the first number that is bigger then zero
				if(-difference > value){
					//find the next significant value that is bigger then zero
					if(encoderValue.value[j - 1] > 0){
						value += encoderValue.value[j - 1] * powersOfTen((encoderValue.pointerToValue - j));
							if(value > -difference){
								value += difference;
								borrow = j - 1;
								break;
							}
					}
				}
				else if(-difference <= value){
					encoderValue.value[encoderValue.pointerToValue - 1] += difference;
					borrow = -2;
					break;
				}
			}
			if(borrow >= 0){
				//handle borrowing
				uint8_t tmpArr[10] = {0};
				uint8_t tmp = 0;
				for(int i = 0; i < 10; i++){
					tmpArr[i] = value % 10;
					value /= 10;
					if(value == 0) break;
					tmp++;
				}
				encoderValue.value[borrow]--;
				for(int j = borrow + 1; j <= encoderValue.pointerToValue - 1; j++){
					encoderValue.value[j] = tmpArr[tmp--];
				}
			}
			if(borrow == -1){
				//if there is no borrowing we will set the value to 0
				for(int i = 0; i < encoderValue.pointerToValue; i++){
					encoderValue.value[i] = 0;
				}
			}
		}

		// Ensure all values are within 0-9 range
		for (int i = 0; i < encoderValue.len; i++) {
			encoderValue.value[i] = (encoderValue.value[i] % 10 + 10) % 10; // Modulo 10 twice for positive values > 9
		}
}

void calculateSendFrequency(){
	//calculate the frequency to send to the radio
	uint32_t frequency = 0;
	if(encoderValue.value[0] > 4){
		ILI9341_fill_screen(COLOR_RED);
		ILI9341_print_text("Out of range", 85, 100, COLOR_WHITE, COLOR_RED, 2);
		HAL_Delay(5000);
		encoderValue.value[0] = 0;
		STM32_PLC_LCD_Show_Main_Frame(0);
		return;
	}
	//this is only to have control not to have more then max uint32_t value
	if(encoderValue.value[0] == 4 && encoderValue.value[1] >= 2 && encoderValue.value[2] >= 9 && encoderValue.value[3] >= 5){
		ILI9341_fill_screen(COLOR_RED);
		ILI9341_print_text("Out of range", 85, 100, COLOR_WHITE, COLOR_RED, 2);
		HAL_Delay(5000);
		encoderValue.value[0] = 0;
		STM32_PLC_LCD_Show_Main_Frame(0);
		return;
	}
	for(int i = 9; i >= 0; i--){
		frequency += encoderValue.value[i] * powersOfTen(9 - i);
	}
	//send the frequency to the radio
	uint8_t message[4] = {0};
	uint32_t delta = floor(frequency * pow(2,N) / FS + 0.5);

	for(int i = 0; i < 4; i++){
		message[i] = (delta >> (i * 8));
	}
	for(int i = 0; i < 4; i++){
		while (!(UART4->SR & 0x0080));
		UART4->DR = (message[i] & 0xFF);
	}
}

void printEncoderValue(){
	// Print the encoder value
	ILI9341_fill_rect(10, 70, 310, 110, COLOR_BLACK);
	osDelay(10);
	char str[6] = {0};
	for(int i = 0; i < 4; i++){
		str[i] = encoderValue.value[i] + '0';
	}
	str[4] = '.';
	char str2[7] = {0};
	ILI9341_print_text(str, 50, 80, COLOR_WHITE, COLOR_BLACK, 3);
	osDelay(10);
	for(int i = 0; i < 6; i++){
		str2[i] = encoderValue.value[i + 4] + '0';
	}
	ILI9341_print_text(str2, 130, 80, COLOR_WHITE, COLOR_BLACK, 3);
	ILI9341_print_text("MHz", 240, 80, COLOR_WHITE, COLOR_BLACK, 3);
	osDelay(10);
}

void printPointerToValue(){
	// Print the pointer to the value
	if(encoderValue.pointerToValue < 4 && encoderValue.pointerToValue > 0){
		ILI9341_print_text("^", 105 - (4 - encoderValue.pointerToValue) * 20, 110, COLOR_RED, COLOR_BLACK, 3);
	}
	else if(encoderValue.pointerToValue >= 4 && encoderValue.pointerToValue <= 10){
		ILI9341_print_text("^", 105 + (encoderValue.pointerToValue - 4) * 20 , 110, COLOR_RED, COLOR_BLACK, 3);
	}
}

void STM32_PLC_LCD_Call_Main_Logic(uint8_t *frame_id) {
	uint16_t x, y;
	if (ILI9341_TouchGetCoordinates(&x, &y)) {
		/* Check which button we are pressing on */
		if (x >= 5 && x <= 83 && y >= 155 && y <= 235) {            /* First icon */
			//go left
			ILI9341_fill_rect(10, 110, 310, 130, COLOR_BLACK);
			encoderValue.pointerToValue--;
			printPointerToValue();
		} 
		else if (x >= 83 && x <= 160 && y >= 155 && y <= 235) {			
			//go right
			ILI9341_fill_rect(10, 110, 310, 130, COLOR_BLACK);
			encoderValue.pointerToValue++;
			printPointerToValue();
		}
		else if (x >= 160 && x <= 238 && y >= 155 && y <= 235) {
			//LEVEL all
			LevelAllRightOfPointerToZero();
			calculateSendFrequency();
		} 
		else if (x >= 238 && x <= 310 && y >= 155 && y <= 235) {
			//Keyboard
			STM32_PLC_LCD_Show_Numpad_Frame();
			//send new frequency
			calculateSendFrequency();
			STM32_PLC_LCD_Show_Main_Frame(frame_id);
		}
	}
	buttonValue = 0;
	//wait for the buttons to be pressed (left, right, keyboard and level)
	if(xTaskNotifyWait(0, ULONG_MAX, &buttonValue, 10) == pdTRUE)
    {
		switch(buttonValue){
			case BUTTON3_IRQ:
				//pointer to value goes to left
				ILI9341_fill_rect(10, 110, 310, 130, COLOR_BLACK);
				encoderValue.pointerToValue--;
				printPointerToValue();
				HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
				break;
			case BUTTON4_IRQ:
				//LEVEL all right of the pointer to 0
				LevelAllRightOfPointerToZero();
				calculateSendFrequency();
				HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);
				break;
			case BUTTON5_IRQ:
				//pointer goes to right
				ILI9341_fill_rect(10, 110, 310, 130, COLOR_BLACK);
				encoderValue.pointerToValue++;
				printPointerToValue();
				HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, GPIO_PIN_RESET);
				break;
			case BUTTON6_IRQ:
				//KEYBOARD call
				STM32_PLC_LCD_Show_Numpad_Frame();
				//send new frequency
				calculateSendFrequency();
				STM32_PLC_LCD_Show_Main_Frame(frame_id);
				break;
		}
    }
	encoderDiff = 0;
	if(xQueueReceive(encoderValueHandle, &encoderDiff, 1) == pdTRUE)
	{
		updateEncoderValue(encoderDiff);
		//now update the value on screen
		printEncoderValue();
		//send the new value to the radio
		calculateSendFrequency();
		//update value of the encoder
	}
	calculateSendFrequency();

}
