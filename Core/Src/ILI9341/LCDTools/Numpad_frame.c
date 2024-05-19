/*
 * Numpad_frame.c
 *
 *  Created on: Jul 23, 2021
 *      Author: Daniel Mårtensson
 */

#include "Touch_screen.h"
#include "ILI9341.h"

uint8_t STM32_PLC_LCD_Show_Numpad_Frame() {
	/* Begin with black page */
	ILI9341_fill_screen(COLOR_BLACK);

	/* 
		_______________________________________
		|									  |
		|			xxxx.xxxxxx MHz			  |
		|_____________________________________|
		|	   |      |		  |		  |       |
		| GHz  |  1   |   2   |   3	  |   0   |
		|______|______|_______|_______|_______|
		|	   |      |		  |		  |       |
		| MHz  |  4   |   5   |   6	  |   .   |
		|______|______|_______|_______|_______|
		|	   |      |		  |		  |       |
		| kHz  |  7   | 8     |   9	  | Enter |
		|______|______|_______|_______|_______|

	*/

	ILI9341_draw_horizontal_line(5, 5, 310, COLOR_GREEN);
	ILI9341_draw_horizontal_line(5, 60, 310, COLOR_GREEN);
	ILI9341_draw_horizontal_line(5, 115, 310, COLOR_GREEN);
	ILI9341_draw_horizontal_line(5, 170, 310, COLOR_GREEN);
	ILI9341_draw_horizontal_line(5, 235, 310, COLOR_GREEN);


	ILI9341_draw_vertical_line(5, 5, 230, COLOR_GREEN);
	ILI9341_draw_vertical_line(5, 67, 160, COLOR_GREEN);
	ILI9341_draw_vertical_line(5, 129, 160, COLOR_GREEN);
	ILI9341_draw_vertical_line(5, 191, 160, COLOR_GREEN);
	ILI9341_draw_vertical_line(5, 253, 160, COLOR_GREEN);
	ILI9341_draw_vertical_line(5, 315, 230, COLOR_GREEN);

	/* Print text */
	char str[40];
	sprintf(str, "xxxx.xxxxxx MHz");
	ILI9341_print_text(str, 130, 190, COLOR_WHITE, COLOR_BLACK, 2);

	/* Print buttons */
	ILI9341_print_text("GHz", 25, 140, COLOR_WHITE, COLOR_BLACK, 2);
	ILI9341_print_text("MHz", 25, 85, COLOR_WHITE, COLOR_BLACK, 2);
	ILI9341_print_text("kHz", 25, 30, COLOR_WHITE, COLOR_BLACK, 2);

	/* Print buttons */
	ILI9341_print_text("1", 87, 140, COLOR_WHITE, COLOR_BLACK, 2);
	ILI9341_print_text("2", 150, 140, COLOR_WHITE, COLOR_BLACK, 2);
	ILI9341_print_text("3", 211, 140, COLOR_WHITE, COLOR_BLACK, 2);

	ILI9341_print_text("4", 87, 85, COLOR_WHITE, COLOR_BLACK, 2);
	ILI9341_print_text("5", 150, 85, COLOR_WHITE, COLOR_BLACK, 2);
	ILI9341_print_text("6", 211, 85, COLOR_WHITE, COLOR_BLACK, 2);

	ILI9341_print_text("7", 87, 30, COLOR_WHITE, COLOR_BLACK, 2);
	ILI9341_print_text("8", 150, 30, COLOR_WHITE, COLOR_BLACK, 2);
	ILI9341_print_text("9", 211, 30, COLOR_WHITE, COLOR_BLACK, 2);

	ILI9341_print_text("0", 275, 140, COLOR_WHITE, COLOR_BLACK, 2);
	ILI9341_print_text(".", 275, 85, COLOR_WHITE, COLOR_BLACK, 2);
	ILI9341_print_text("ENTER", 275, 30, COLOR_WHITE, COLOR_BLACK, 2);

	return STM32_PLC_LCD_Call_Numpad_Logic();
}
