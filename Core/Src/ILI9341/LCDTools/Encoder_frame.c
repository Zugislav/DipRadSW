/*
 * Encoder_revolutions_settings_frame.c
 *
 *  Created on: 5 May 2024
 *      Author: Ivan Žugaj
 */

#include "Touch_screen.h"
#include "ILI9341.h"
#include "encoder.h"

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "queue.h"

extern osMessageQueueId_t mainQueueHandle;

#define ENCODER1_PULSES_PER_ROTATION	24

void STM32_PLC_LCD_Show_Encoder_Revolutions_Settings_Frame(uint8_t *frame_id) {
	/* Clear the screen , but not the icons */
	ILI9341_fill_rect(5, 5, 310, 200, COLOR_NAVY);

	/* Write the title */
	//ILI9341_draw_horizontal_line(50, 30, 265, COLOR_GREEN);
	ILI9341_print_text("Encoder frame", 115, 210, COLOR_YELLOW, COLOR_NAVY, 2);

	/* Show all calibration inputs */
	ILI9341_print_text("Encoder counts between:", 55, 35, COLOR_YELLOW, COLOR_NAVY, 1);
	ILI9341_print_text("0 to 500.00MHz", 55, 50, COLOR_YELLOW, COLOR_NAVY, 1);

	//we will show encoder value
	/* You entered choice 1 - Show numpad */
	bool minusbutton_show = false;
	bool decimalbutton_show = false;
	float number_value;
	if(STM32_PLC_LCD_Show_Numpad_Frame(decimalbutton_show, minusbutton_show, &number_value, "Enter input number between 1 and 300") == 0) {
		return;
	}

	uint8_t doubleTouch = 0;
	do{
		//wait for the touch off the encoder switch
		if(xTaskNotifyWait(0, NULL, NULL , 200) == pdPASS)
		{
			doubleTouch++;
			if(doubleTouch == 2) break;
		}
		if(doubleTouch){
			//show encoder value and update the value and print it on the screen
			
		}
	} while(1);
	/* Show main frame again */
	STM32_PLC_LCD_Show_Main_Frame(frame_id);
}
