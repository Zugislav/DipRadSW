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


extern EncoderValue encoderValue;
extern osMessageQueueId_t mainQueueHandle;

static uint32_t encoderDiff = 0;
static uint32_t buttonValue = 0;

static void updateEncoderValue(uint8_t difference){
	
	// Add the new value at the pointer position
	encoderValue.value[encoderValue.pointerToValue] += difference;

	// Handle carry-over (if needed)
	for(int i = encoderValue.pointerToValue; i >= 1; i--){
		encoderValue.value[i - 1 ] += encoderValue.value[i] / 10; // Add carry-over from previous digit
		encoderValue.value[i] %= 10;						  // Limit previous digit to 0-9
	}
	// Ensure all values are within 0-9 range
	for (int i = 0; i < 10; i++) {
		encoderValue.value[i] = (encoderValue.value[i] % 10 + 10) % 10;  // Modulo 10 twice for values > 9
	}
}

static void printPointerToValue(){
	// Print the pointer to the value
	if(encoderValue.pointerToValue < 4){
		ILI9341_print_text("^", 150 - (4 - encoderValue.pointerToValue) * 10, 70, COLOR_RED, COLOR_BLACK, 3);
	}
	else{
		ILI9341_print_text("^", 150 + (encoderValue.pointerToValue - 4) * 10 , 70, COLOR_RED, COLOR_BLACK, 3);
	}
}
void STM32_PLC_LCD_Call_Main_Logic(uint8_t *frame_id) {
	if (TSC2046_isPressed()) {
		TSC2046_GetTouchData();
		uint16_t X = lcd.myTsData.X;
		uint16_t Y = lcd.myTsData.Y;
		/* Check which button we are pressing on */
		if (X >= 5 && X <= 315 && Y >= 5 && Y <= 35) {            /* First icon */
			//option number 1
		} 
		else if (X >= 5 && X <= 315 && Y >= 35 && Y <= 53) {			
			//option number 2
		}
		else if (X >= 5 && X <= 315 && Y >= 53 && Y <= 80) {
			//option number 3
		} 
		else if (X >= 5 && X <= 315 && Y >= 80 && Y <= 107) {
			//option number 4
		} 
		else if (X >= 5 && X <= 315 && Y >= 107 && Y <= 134) {
			//option number 5
		} 
		else if (X >= 5 && X <= 315 && Y >= 134 && Y <= 171) {
			//option number 6
		} 
		else if (X >= 5 && X <= 315 && Y >= 171 && Y <= 200) {
			//option number 7
		}
		else if (X >= 5 && X <= 55 && Y >= 200 && Y <= 235) {  /* Fourth icon */
			if (*frame_id > 0)
				*frame_id = *frame_id - 1; /* Got to previous frame */
			else
				*frame_id = 6; /* Go to last frame */
			STM32_PLC_LCD_Show_Main_Frame(frame_id);
		} 
		else if (X >= 55 && X <= 105 && Y >= 200 && Y <= 235) {  /* Fifth icon */
			if (*frame_id < 6)
				*frame_id = *frame_id + 1; /* Go to next frame */
			else
				*frame_id = 0; /* Go to start frame */
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
				encoderValue.pointerToValue--;
				printPointerToValue();
				break;
			case BUTTON4_IRQ:
				//LEVEL all right of the pointer to 0
				break;
			case BUTTON5_IRQ:
				//pointer goes to right
				encoderValue.pointerToValue++;
				printPointerToValue();
				break;
			case BUTTON6_IRQ:
				//KEYBOARD call
				break;
		}
    }
	encoderDiff = 0;
	if(xQueueReceive(mainQueueHandle, &encoderDiff, 100) == pdTRUE)
	{
		updateEncoderValue(encoderDiff);
		//now update the value on screen
		char str[40];
		memcpy(str, encoderValue.value, 10);
		ILI9341_print_text(str, 130, 80, COLOR_WHITE, COLOR_BLACK, 1);
		//update value of the encoder
	}
}
