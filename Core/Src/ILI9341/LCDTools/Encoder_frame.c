/*
 * Encoder_revolutions_settings_frame.c
 *
 *  Created on: 5 May 2024
 *      Author: Ivan Žugaj
 */

#include "Touch_screen.h"
#include "ILI9341.h"
#include "encoder.h"

#define ENCODER1_PULSES_PER_ROTATION	24

void STM32_PLC_LCD_Show_Encoder_Revolutions_Settings_Frame(uint8_t *frame_id) {
	/* Clear the screen , but not the icons */
	ILI9341_fill_rect(51, 6, 314, 234, COLOR_NAVY);

	/* Write the title */
	ILI9341_draw_horizontal_line(50, 30, 265, COLOR_GREEN);
	ILI9341_print_text("Set encoder revolution", 55, 10, COLOR_YELLOW, COLOR_NAVY, 1);

	/* Show all calibration inputs */
	ILI9341_print_text("Encoder counts between:", 55, 35, COLOR_YELLOW, COLOR_NAVY, 1);
	ILI9341_print_text("0 to 300.00MHz", 55, 50, COLOR_YELLOW, COLOR_NAVY, 1);

	/* Show selected frequency */
	char text[50];
	ILI9341_print_text("Current settings:", 55, 75, COLOR_YELLOW, COLOR_NAVY, 1);
	uint16_t pulses0 = ENCODER1_PULSES_PER_ROTATION;
	sprintf(text, "Encoder:%i", pulses0);
	ILI9341_print_text(text, 55, 85, COLOR_YELLOW, COLOR_NAVY, 1);

	/* Select settings button */
	ILI9341_fill_rect(90, 195, 273, 220, COLOR_GREEN);
	ILI9341_hollow_rect(90, 195, 273, 220, COLOR_BLACK);
	ILI9341_print_text("Select encoder", 100, 200, COLOR_BLACK, COLOR_GREEN, 2);

	/* Logic for request button */
	STM32_PLC_LCD_Call_One_Button_Logic(90, 195, 273, 220);

	/* Ask the user if */
	if(STM32_PLC_LCD_Show_Question_Yes_No_Dialog("Do you want to set encoder revolution?") == 0) {
		STM32_PLC_LCD_Show_Main_Frame(frame_id);
		return;
	}

	/* You entered choice 1 - Show numpad */
	bool minusbutton_show = false;
	bool decimalbutton_show = false;
	float number_value;
	if(STM32_PLC_LCD_Show_Numpad_Frame(decimalbutton_show, minusbutton_show, &number_value, "Enter input number between 1 and 65535") == 0) {
		STM32_PLC_LCD_Show_Main_Frame(frame_id);
		return;
	}

	/* Check 
	uint16_t pulses_per_revolution;
	if(number_value < 1)
		pulses_per_revolution = 1;
	else if(number_value > 65535)
		pulses_per_revolution = 65535;
	else
		pulses_per_revolution = (uint16_t) number_value;
	*/
	/* Ask for which PWM */
	if(STM32_PLC_LCD_Show_Numpad_Frame(decimalbutton_show, minusbutton_show, &number_value, "Set setting for Encoder0=0, Encoder1=1, Encoder2=2") == 0) {
		STM32_PLC_LCD_Show_Main_Frame(frame_id);
		return;
	}

	/* Check 
	uint8_t encoder_choice;
	if(number_value < 0)
		encoder_choice = 0;
	else if(number_value > 2)
		encoder_choice = 2;
	else
		encoder_choice = (uint8_t) number_value;
	*/
	/* Apply them */
	//STM32_PLC_Encoder_Set_Calibration(encoder_choice, pulses_per_revolution, true);

	/* Show main frame again */
	STM32_PLC_LCD_Show_Main_Frame(frame_id, false);
}
