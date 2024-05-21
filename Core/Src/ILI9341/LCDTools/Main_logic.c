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

  // Ensure length is valid (avoid out-of-bounds access)
  if (encoderValue.len > 10 || encoderValue.len <= 0) {
    return;  // Handle invalid length (optional)
  }

  // Handle negative difference by subtracting its absolute value
  if (difference < 0) {
    difference = -difference;  // Get absolute value

    // Combine digits left of the pointer into a uint32_t
    uint32_t leftValue = 0;
    for (int i = encoderValue.pointerToValue - 1; i > 0; i--) {
      leftValue = leftValue * pow(10,i) + encoderValue.value[i];
    }

    // Perform subtraction on the combined value
    leftValue -= difference;

    // Update the array elements from the combined value
    int j = 0;
    while (leftValue > 0) {
      encoderValue.value[j++] = leftValue % 10;
      leftValue /= 10;
    }

    // Update remaining elements with 0 (optional)
    for (; j < encoderValue.pointerToValue - 1; j++) {
      encoderValue.value[j] = 0;
    }
  } else {
    // Existing logic for adding positive difference (unchanged)
    encoderValue.value[encoderValue.pointerToValue - 1] += difference;
  }

  // Handle carry-over (if needed)
  for (int i = encoderValue.pointerToValue - 1; i >= 1; i--) {
    // Handle carry-over based on sign of difference
    if (difference >= 0) {
      encoderValue.value[i - 1] += encoderValue.value[i] / 10; // Add carry-over (positive)
    } else {
      encoderValue.value[i - 1] += 10 - (encoderValue.value[i] % 10); // Borrow & subtract digit (negative)
    }
    encoderValue.value[i] %= 10;  // Limit previous digit to 0-9
  }

  // Ensure all values are within 0-9 range (consider sign)
  for (int i = 0; i < encoderValue.len; i++) {
    if (difference >= 0) {
      encoderValue.value[i] = (encoderValue.value[i] % 10 + 10) % 10; // Modulo 10 twice for positive values > 9
    } else {
      encoderValue.value[i] = (encoderValue.value[i] - 10 + 10) % 10; // Adjust for negative values > 9
    }
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
