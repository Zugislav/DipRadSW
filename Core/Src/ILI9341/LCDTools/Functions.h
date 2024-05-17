/* C standard library */
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* Project includes */
#include "main.h"
#include "stm32f4xx_hal.h"

/* LCD */
void STM32_PLC_LCD(SPI_HandleTypeDef *lcdSpi, SPI_HandleTypeDef *touchSpi, GPIO_TypeDef *LCD_CS_PORT, uint16_t LCD_CS_PIN, GPIO_TypeDef *LCD_DC_PORT, uint16_t LCD_DC_PIN, GPIO_TypeDef *TOUCH_CS_PORT, uint16_t TOUCH_CS_PIN);
void STM32_PLC_LCD_Calibrate_Touch();
bool STM32_PLC_LCD_Is_Pressed();
void STM32_PLC_LCD_Get_Touch_Calibration_Parameters(float *Scale_X, float *Scale_Y, float *Bias_X, float *Bias_Y);
void STM32_PLC_LCD_Set_Touch_Calibration_Parameters(float Scale_X, float Scale_Y, float Bias_X, float Bias_Y);
void STM32_PLC_LCD_Get_Touch_Data();