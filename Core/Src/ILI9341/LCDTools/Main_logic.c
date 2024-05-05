/*
 * Main_Logic.c
 *
 *  Created on: 5 May 2024
 *      Author: Ivan Žugaj
 */

#include "Touch_screen.h"
#include "ILI9341.h"

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
}
