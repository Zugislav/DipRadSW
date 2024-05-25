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
		|  0   |  1   |   2   |   3	  |  GHz  |
		|______|______|_______|_______|_______|
		|	   |      |		  |		  |       |
		|  .   |  4   |   5   |   6	  |  MHz  |
		|______|______|_______|_______|_______|
		|	   |      |		  |		  |       |
		|      |  7   | 8     |   9	  | kHz   |
		|______|______|_______|_______|_______|

	*/

	ILI9341_draw_horizontal_line(5, 5, 310, COLOR_GREEN);
	ILI9341_draw_horizontal_line(5, 70, 310, COLOR_GREEN);
	ILI9341_draw_horizontal_line(5, 125, 310, COLOR_GREEN);
	ILI9341_draw_horizontal_line(5, 180, 310, COLOR_GREEN);
	ILI9341_draw_horizontal_line(5, 235, 310, COLOR_GREEN);


	ILI9341_draw_vertical_line(5, 5, 230, COLOR_GREEN);
	ILI9341_draw_vertical_line(67, 70, 165, COLOR_GREEN);
	ILI9341_draw_vertical_line(129, 70, 165, COLOR_GREEN);
	ILI9341_draw_vertical_line(191, 70, 165, COLOR_GREEN);
	ILI9341_draw_vertical_line(253, 70, 165, COLOR_GREEN);
	ILI9341_draw_vertical_line(315, 5, 230, COLOR_GREEN);

	/* Print text */
	char str[40];
	sprintf(str, "xxxx.xxxxxx MHz");
	ILI9341_print_text(str, 70, 35, COLOR_WHITE, COLOR_BLACK, 2);

	/* Print buttons */
	ILI9341_print_text("0", 25, 85, COLOR_WHITE, COLOR_BLACK, 2);
	ILI9341_print_text(".", 25, 140, COLOR_WHITE, COLOR_BLACK, 2);
	//ILI9341_print_text("", 25, 195, COLOR_WHITE, COLOR_BLACK, 2);

	/* Print buttons */
	ILI9341_print_text("1", 87, 85, COLOR_WHITE, COLOR_BLACK, 2);
	ILI9341_print_text("2", 150, 85, COLOR_WHITE, COLOR_BLACK, 2);
	ILI9341_print_text("3", 211, 85, COLOR_WHITE, COLOR_BLACK, 2);

	ILI9341_print_text("4", 87, 140, COLOR_WHITE, COLOR_BLACK, 2);
	ILI9341_print_text("5", 150, 140, COLOR_WHITE, COLOR_BLACK, 2);
	ILI9341_print_text("6", 211, 140, COLOR_WHITE, COLOR_BLACK, 2);

	ILI9341_print_text("7", 87, 195, COLOR_WHITE, COLOR_BLACK, 2);
	ILI9341_print_text("8", 150, 195, COLOR_WHITE, COLOR_BLACK, 2);
	ILI9341_print_text("9", 211, 195, COLOR_WHITE, COLOR_BLACK, 2);


	ILI9341_print_text("GHz", 275, 85, COLOR_WHITE, COLOR_BLACK, 2);
	ILI9341_print_text("MHz", 275, 140, COLOR_WHITE, COLOR_BLACK, 2);
	ILI9341_print_text("kHz", 270, 195, COLOR_WHITE, COLOR_BLACK, 2);

	return STM32_PLC_LCD_Call_Numpad_Logic();
}
