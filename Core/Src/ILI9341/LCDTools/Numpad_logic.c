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

extern EncoderValue enc,oderValue;

#define GHZ_POINTER 0
#define MHZ_POINTER 3
#define KHZ_POINTER 6

static InputNumber(bool hasDot, uint8_t inputValue[], uint8_t k, uint8_t freqPointer){
	//not so sure about this code-------------------------------------
	if (hasDot == false) {
		// If we don't have a dot
		if (k < 3) {
			// If we are before the dot
			inputValue[k] = freqPointer;
		} else if (k > 3) {
			// If we are after the dot
			inputValue[k - 1] = freqPointer;
		}
	} else {
		// If we have a dot
		if (k < 3) {
			// If we are before the dot
			inputValue[k] = freqPointer;
		} else if (k > 3) {
			// If we are after the dot
			inputValue[k - 1] = freqPointer;
		}
	}
}

uint8_t STM32_PLC_LCD_Call_Numpad_Logic() {

	bool haveDot = false;
	char text[17] = "xxxx.xxxxxx MHz";
	uint8_t inputValue[10];
	uint8_t k = 0;
	uint8_t freqPointer = 3;
	while (1) {
		if (TSC2046_isPressed()) {
			TSC2046_GetTouchData();
			uint16_t X = lcd.myTsData.X;
			uint16_t Y = lcd.myTsData.Y;

			/* Check which button we are pressing on */
			if (X >= 5 && X <= 67 && Y >= 115 && Y <= 165){
				//GHz button
				memset(text, 0, sizeof(text));
				freqPointer = GHZ_POINTER;
			}
			else if (X >= 5 && X <= 67 && Y >= 60 && Y <= 110){
				//MHz button
				memset(text, 0, sizeof(text));
				freqPointer = MHZ_POINTER;
			}
			else if (X >= 5 && X <= 67 && Y >= 5 && Y <= 60){
				//kHz button
				memset(text, 0, sizeof(text));
				freqPointer = KHZ_POINTER;
			}
			else if (X >= 67 && X <= 129 && Y >= 115 && Y <= 165){
				//1 button
			}
			else if (X >= 67 && X <= 129 && Y >= 60 && Y <= 115){
				//4 button
			}
			else if (X >= 67 && X <= 129 && Y >= 5 && Y <= 60){
				//7 button
			}
			else if (X >= 129 && X <= 191 && Y >= 115 && Y <= 165){
				//2 button
			}
			else if (X >= 129 && X <= 191 && Y >= 60 && Y <= 115){
				//5 button
			}
			else if (X >= 129 && X <= 191 && Y >= 5 && Y <= 60){
				//8 button
			}
			else if (X >= 191 && X <= 253 && Y >= 115 && Y <= 165){
				//3 button
			}
			else if (X >= 191 && X <= 253 && Y >= 60 && Y <= 115){
				//6 button
			}
			else if (X >= 191 && X <= 253 && Y >= 5 && Y <= 60){
				//9 button
			}
			else if (X >= 191 && X <= 253 && Y >= 115 && Y <= 165){
				//zero button
			}
			else if (X >= 191 && X <= 253 && Y >= 60 && Y <= 115){
				//point button
				if(haveDot == false){
					haveDot = true;
				}
			}
			else if (X >= 191 && X <= 253 && Y >= 5 && Y <= 60){
				//ENTER button
				//store the value in encoderValue and then exit the loop and enter main frame
			}

			/* Display the selected number text */
			ILI9341_print_text(text, 15, 25, COLOR_BLACK, COLOR_WHITE, 3);
		}
	}
}
