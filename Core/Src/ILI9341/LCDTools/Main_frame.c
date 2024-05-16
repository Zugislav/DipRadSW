/*
 * Main_frame.c
 *
 *  Created on: 5 May 2024
 *      Author: Ivan Žugaj
 */

#include "Touch_screen.h"
#include "ILI9341.h"

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
		|	   |      |								  |
		|  <-  |  ->  | Main frame (text)			  |
		|______|______|_______________________________|
		|__	G ________________________________________|
		|__	F ________________________________________|
		|__	E ________________________________________|
		|__	D ________________________________________|
		|__	C ________________________________________|
		|__	B ________________________________________|
		|	A										  |
		|_____________________________________________|

	*/
	ILI9341_draw_vertical_line(5, 200, 35, COLOR_GREEN);
	ILI9341_draw_vertical_line(55, 200, 35, COLOR_GREEN);
	ILI9341_draw_vertical_line(105, 200, 35, COLOR_GREEN);
	//ILI9341_draw_horizontal_line(5, 200, 100, COLOR_GREEN);
	ILI9341_draw_horizontal_line(5, 200, 310, COLOR_GREEN);

	//printing lines for options B - G since option A is on bottom of the screen
	ILI9341_draw_horizontal_line(5, 35, 310, COLOR_GREEN);
	ILI9341_draw_horizontal_line(5, 53, 310, COLOR_GREEN);
	ILI9341_draw_horizontal_line(5, 80, 310, COLOR_GREEN);
	ILI9341_draw_horizontal_line(5, 107, 230, COLOR_GREEN);
	ILI9341_draw_horizontal_line(5, 134, 230, COLOR_GREEN);
	ILI9341_draw_horizontal_line(5, 171, 230, COLOR_GREEN);

	/* This line is for the input and output text */
	//ILI9341_draw_horizontal_line(5, 30, 310, COLOR_GREEN);

	ILI9341_print_text("<-", 25, 238, COLOR_ORANGE, COLOR_NAVY, 3);
	ILI9341_print_text("->", 75, 238, COLOR_GREEN, COLOR_NAVY, 3);

	/* Print title */
	ILI9341_print_text("Main frame", 115, 210, COLOR_YELLOW, COLOR_NAVY, 2);

	char text[40];

	/* Write out letters */
	//WE NEED ONLY A-F (like buttons f1-f6) and G like about STM32 PLC
	sprintf(text, "A:This is f1 function");
	ILI9341_print_text(text, 15, 15, COLOR_YELLOW, COLOR_NAVY, 1);
	sprintf(text, "B:This is f2 function");
	ILI9341_print_text(text, 15, 45, COLOR_YELLOW, COLOR_NAVY, 1);
	sprintf(text, "C:This is f3 function");
	ILI9341_print_text(text, 15, 63, COLOR_YELLOW, COLOR_NAVY, 1);
	sprintf(text, "D:This is f4 function");
	ILI9341_print_text(text, 15, 90, COLOR_YELLOW, COLOR_NAVY, 1);
	sprintf(text, "E:This is f5 function");
	ILI9341_print_text(text, 15, 117, COLOR_YELLOW, COLOR_NAVY, 1);
	sprintf(text, "F:This is f6 function");
	ILI9341_print_text(text, 15, 145, COLOR_YELLOW, COLOR_NAVY, 1);
	sprintf(text, "G:Info about STM32 PLC");
	ILI9341_print_text(text, 15, 182, COLOR_YELLOW, COLOR_NAVY, 1);
}
