/*
 * Main_frame.c
 *
 *  Created on: 5 May 2024
 *      Author: Ivan Žugaj
 */

#include "Touch_screen.h"
#include "ILI9341.h"
#include "message.h"

extern EncoderValue encoderValue;

/* Create the main frame */
void  STM32_PLC_LCD_Show_Main_Frame(uint8_t *frame_id) {
	/* Set rotation */
	ILI9341_set_rotation(2);

	/* Set background */
	ILI9341_fill_screen(COLOR_BLACK);

	/* Print frame - large square */
	ILI9341_draw_horizontal_line(5, 235, 310, COLOR_GREEN);
	ILI9341_draw_horizontal_line(5, 5, 310, COLOR_GREEN);
	ILI9341_draw_vertical_line(5, 5, 230, COLOR_GREEN);
	ILI9341_draw_vertical_line(315, 5, 230, COLOR_GREEN);

	/* Button for left and right frame  and output text
		______________________________________________
		|	   |      |			|					  |
		|  <-  |  ->  | Level	| Keyboard			  |
		|______|______|_________|_____________________|
		|											  |
		|			1000.000000 MHz					  |
		|				^ (ON VALUE POINTER)		  |
		|_____________________________________________|

	*/
	ILI9341_draw_vertical_line(83, 155, 80, COLOR_GREEN);
	ILI9341_draw_vertical_line(160, 155, 80, COLOR_GREEN);
	ILI9341_draw_vertical_line(238, 155, 80, COLOR_GREEN);
	//ILI9341_draw_horizontal_line(5, 200, 100, COLOR_GREEN);
	ILI9341_draw_horizontal_line(5, 155, 310, COLOR_GREEN);

	ILI9341_print_text("<-", 45, 180, COLOR_ORANGE, COLOR_NAVY, 3);
	ILI9341_print_text("->", 115, 180, COLOR_GREEN, COLOR_NAVY, 3);

	/* Print title */
	ILI9341_print_text("Level", 205, 180, COLOR_YELLOW, COLOR_NAVY, 2);
	ILI9341_print_text("Keyboard", 250, 180, COLOR_YELLOW, COLOR_NAVY, 2);

	/* Print frequency */
	encoderValue.pointerToValue = 4;
	encoderValue.value[2] = 1;
	char str[40];
	sprintf(str, "0010.000000 MHz");
	ILI9341_print_text(str, 130, 80, COLOR_WHITE, COLOR_BLACK, 1);
	ILI9341_print_text("^", 150, 70, COLOR_RED, COLOR_BLACK, 3);
}
