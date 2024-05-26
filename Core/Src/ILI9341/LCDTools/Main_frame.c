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
	ILI9341_set_rotation(4);

	/* Set background */
	ILI9341_fill_screen(COLOR_BLACK);

	/* Print frame - large square */
	ILI9341_draw_horizontal_line(5, 235, 310, COLOR_GREEN);
	ILI9341_draw_horizontal_line(5, 5, 310, COLOR_GREEN);
	ILI9341_draw_vertical_line(5, 5, 230, COLOR_GREEN);
	ILI9341_draw_vertical_line(315, 5, 230, COLOR_GREEN);

	/* Button for left and right frame  and output text
		______________________________________________
		|											  |
		|			1000.000000 MHz					  |
		|				^ (ON VALUE POINTER)		  |
		|_____________________________________________|
		|	   |      |			|					  |
		|  <-  |  ->  | Level	| Keyboard			  |
		|______|______|_________|_____________________|


	*/
	//left button
	ILI9341_draw_vertical_line(83, 155, 80, COLOR_GREEN);
	ILI9341_fill_rect(5, 155, 83, 235, COLOR_RED);
	//right button
	ILI9341_draw_vertical_line(160, 155, 80, COLOR_GREEN);
	ILI9341_fill_rect(83, 155, 160, 235, COLOR_GREEN);
	//Level button
	ILI9341_draw_vertical_line(238, 155, 80, COLOR_GREEN);
	ILI9341_fill_rect(160, 155, 238, 235, COLOR_YELLOW);
	//keyboard button
	//ILI9341_draw_horizontal_line(5, 200, 100, COLOR_GREEN);
	//ILI9341_draw_horizontal_line(5, 155, 310, COLOR_GREEN);
	ILI9341_fill_rect(238, 155, 315, 235, COLOR_BLUE);


	ILI9341_print_text("<-", 35, 185, COLOR_BLACK, COLOR_RED, 3);
	ILI9341_print_text("->", 105, 185, COLOR_BLACK, COLOR_GREEN, 3);

	/* Print title */
	ILI9341_print_text("Level", 170, 185, COLOR_BLACK, COLOR_YELLOW, 2);
	ILI9341_print_text("Input", 250, 185, COLOR_BLACK, COLOR_BLUE, 2);

	/* Print frequency */
	encoderValue.pointerToValue = 4;
	//EXTREMNO BITNO
	printEncoderValue();
	// ILI9341_print_text("0100.", 50, 80, COLOR_WHITE, COLOR_BLACK, 3);
	// ILI9341_print_text("000000", 130, 80, COLOR_WHITE, COLOR_BLACK, 3);
	// ILI9341_print_text("MHz", 240, 80, COLOR_WHITE, COLOR_BLACK, 3);
	//ILI9341_print_text(str, 100, 80, COLOR_WHITE, COLOR_BLACK, 2);
	// ILI9341_print_text("^", 105, 110, COLOR_RED, COLOR_BLACK, 3);
	printPointerToValue();
}
