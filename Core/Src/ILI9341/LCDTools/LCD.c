/*
 * LCD.c
 *
 *  Created on: 5 May 2024
 *      Author: Ivan Žugaj
 */

#include "ILI9341.h"
#include "Touch_screen.h"
#include "message.h"
#include "string.h"

#define ILI9341_ROTATION (ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR)

ILI9341_SPI lcd;
EncoderValue encoderValue;
extern SPI_HandleTypeDef hspi1;

static void ILI9341_Select() {
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
}

static void ILI9341_Unselect() {
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
}
static void ILI9341_Reset() {
    HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(5);
    HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_SET);
}

static void ILI9341_WriteCommand(uint8_t cmd) {
    HAL_GPIO_WritePin(DC_RS_GPIO_Port, DC_RS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, &cmd, sizeof(cmd), HAL_MAX_DELAY);
}

static void ILI9341_WriteData(uint8_t* buff, size_t buff_size) {
    HAL_GPIO_WritePin(DC_RS_GPIO_Port, DC_RS_Pin, GPIO_PIN_SET);

    // split data in small chunks because HAL can't send more then 64K at once
    while(buff_size > 0) {
        uint16_t chunk_size = buff_size > 32768 ? 32768 : buff_size;
        HAL_SPI_Transmit(&hspi1, buff, chunk_size, HAL_MAX_DELAY);
        buff += chunk_size;
        buff_size -= chunk_size;
    }
}

static bool NewInit(){
	  ILI9341_Select();
    ILI9341_Reset();

    // command list is based on https://github.com/martnak/STM32-ILI9341

    // SOFTWARE RESET
    ILI9341_WriteCommand(0x01);
    HAL_Delay(1000);
        
    // POWER CONTROL A
    ILI9341_WriteCommand(0xCB);
    {
        uint8_t data[] = { 0x39, 0x2C, 0x00, 0x34, 0x02 };
        ILI9341_WriteData(data, sizeof(data));
    }

    // POWER CONTROL B
    ILI9341_WriteCommand(0xCF);
    {
        uint8_t data[] = { 0x00, 0xC1, 0x30 };
        ILI9341_WriteData(data, sizeof(data));
    }

    // DRIVER TIMING CONTROL A
    ILI9341_WriteCommand(0xE8);
    {
        uint8_t data[] = { 0x85, 0x00, 0x78 };
        ILI9341_WriteData(data, sizeof(data));
    }

    // DRIVER TIMING CONTROL B
    ILI9341_WriteCommand(0xEA);
    {
        uint8_t data[] = { 0x00, 0x00 };
        ILI9341_WriteData(data, sizeof(data));
    }

    // POWER ON SEQUENCE CONTROL
    ILI9341_WriteCommand(0xED);
    {
        uint8_t data[] = { 0x64, 0x03, 0x12, 0x81 };
        ILI9341_WriteData(data, sizeof(data));
    }

    // PUMP RATIO CONTROL
    ILI9341_WriteCommand(0xF7);
    {
        uint8_t data[] = { 0x20 };
        ILI9341_WriteData(data, sizeof(data));
    }

    // POWER CONTROL,VRH[5:0]
    ILI9341_WriteCommand(0xC0);
    {
        uint8_t data[] = { 0x23 };
        ILI9341_WriteData(data, sizeof(data));
    }

    // POWER CONTROL,SAP[2:0];BT[3:0]
    ILI9341_WriteCommand(0xC1);
    {
        uint8_t data[] = { 0x10 };
        ILI9341_WriteData(data, sizeof(data));
    }

    // VCM CONTROL
    ILI9341_WriteCommand(0xC5);
    {
        uint8_t data[] = { 0x3E, 0x28 };
        ILI9341_WriteData(data, sizeof(data));
    }

    // VCM CONTROL 2
    ILI9341_WriteCommand(0xC7);
    {
        uint8_t data[] = { 0x86 };
        ILI9341_WriteData(data, sizeof(data));
    }

    // MEMORY ACCESS CONTROL
    ILI9341_WriteCommand(0x36);
    {
        uint8_t data[] = { 0x48 };
        ILI9341_WriteData(data, sizeof(data));
    }

    // PIXEL FORMAT
    ILI9341_WriteCommand(0x3A);
    {
        uint8_t data[] = { 0x55 };
        ILI9341_WriteData(data, sizeof(data));
    }

    // FRAME RATIO CONTROL, STANDARD RGB COLOR
    ILI9341_WriteCommand(0xB1);
    {
        uint8_t data[] = { 0x00, 0x18 };
        ILI9341_WriteData(data, sizeof(data));
    }

    // DISPLAY FUNCTION CONTROL
    ILI9341_WriteCommand(0xB6);
    {
        uint8_t data[] = { 0x08, 0x82, 0x27 };
        ILI9341_WriteData(data, sizeof(data));
    }

    // 3GAMMA FUNCTION DISABLE
    ILI9341_WriteCommand(0xF2);
    {
        uint8_t data[] = { 0x00 };
        ILI9341_WriteData(data, sizeof(data));
    }

    // GAMMA CURVE SELECTED
    ILI9341_WriteCommand(0x26);
    {
        uint8_t data[] = { 0x01 };
        ILI9341_WriteData(data, sizeof(data));
    }

    // POSITIVE GAMMA CORRECTION
    ILI9341_WriteCommand(0xE0);
    {
        uint8_t data[] = { 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1,
                           0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00 };
        ILI9341_WriteData(data, sizeof(data));
    }

    // NEGATIVE GAMMA CORRECTION
    ILI9341_WriteCommand(0xE1);
    {
        uint8_t data[] = { 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1,
                           0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F };
        ILI9341_WriteData(data, sizeof(data));
    }

    // EXIT SLEEP
    ILI9341_WriteCommand(0x11);
    HAL_Delay(120);

    // TURN ON DISPLAY
    ILI9341_WriteCommand(0x29);

    // MADCTL
    ILI9341_WriteCommand(0x36);
    {
        uint8_t data[] = { ILI9341_ROTATION };
        ILI9341_WriteData(data, sizeof(data));
    }

    ILI9341_Unselect();
	return true;
}

static void ILI9341_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    // column address set
    ILI9341_WriteCommand(0x2A); // CASET
    {
        uint8_t data[] = { (x0 >> 8) & 0xFF, x0 & 0xFF, (x1 >> 8) & 0xFF, x1 & 0xFF };
        ILI9341_WriteData(data, sizeof(data));
    }

    // row address set
    ILI9341_WriteCommand(0x2B); // RASET
    {
        uint8_t data[] = { (y0 >> 8) & 0xFF, y0 & 0xFF, (y1 >> 8) & 0xFF, y1 & 0xFF };
        ILI9341_WriteData(data, sizeof(data));
    }

    // write to RAM
    ILI9341_WriteCommand(0x2C); // RAMWR
}

static void ILI9341_FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    // clipping
    if((x >= ILI9341_WIDTH) || (y >= ILI9341_HEIGHT)) return;
    if((x + w - 1) >= ILI9341_WIDTH) w = ILI9341_WIDTH - x;
    if((y + h - 1) >= ILI9341_HEIGHT) h = ILI9341_HEIGHT - y;

    ILI9341_Select();
    ILI9341_SetAddressWindow(x, y, x+w-1, y+h-1);

    uint8_t data[] = { color >> 8, color & 0xFF };
    HAL_GPIO_WritePin(DC_RS_GPIO_Port, DC_RS_Pin, GPIO_PIN_SET);
    for(y = h; y > 0; y--) {
        for(x = w; x > 0; x--) {
            HAL_SPI_Transmit(&hspi1, data, sizeof(data), HAL_MAX_DELAY);
        }
    }

    ILI9341_Unselect();
}

static void ILI9341_FillScreen(uint16_t color) {
    ILI9341_FillRectangle(0, 0, ILI9341_WIDTH, ILI9341_HEIGHT, color);
}
/*
 * Initialize LCD and touch
 * SPI settings:
 * CPHA = 1 Edge
 * Prescaler = 8 (be careful for this. Lower than 8 can cause trouble with the touch)
 * First bit = MBS first
 * CPOL = Low
 */
void STM32_PLC_LCD(SPI_HandleTypeDef *lcdSpi, SPI_HandleTypeDef *touchSpi, GPIO_TypeDef *LCD_CS_PORT, uint16_t LCD_CS_PIN, GPIO_TypeDef *LCD_DC_PORT, uint16_t LCD_DC_PIN, GPIO_TypeDef *LCD_RST_PORT, uint16_t LCD_RST_PIN){
	
	memset(&encoderValue, 0, sizeof(EncoderValue));
	encoderValue.len = 10;
	/* Copy SPI settings */
	lcd.lcdSpi = lcdSpi;
	lcd.LCD_CS_PORT = LCD_CS_PORT;
	lcd.LCD_CS_PIN = LCD_CS_PIN;
	lcd.LCD_DC_PORT = LCD_DC_PORT;
	lcd.LCD_DC_PIN = LCD_DC_PIN;
	HAL_GPIO_WritePin(lcd.LCD_DC_PORT, lcd.LCD_DC_PIN, GPIO_PIN_SET);
	lcd.LCD_RESET_PORT = LCD_RST_PORT;
	lcd.LCD_RESET_PIN = LCD_RST_PIN;
	HAL_GPIO_WritePin(lcd.LCD_RESET_PORT, lcd.LCD_RESET_PIN, GPIO_PIN_SET); /* Turn LCD ON */
	lcd._cp437 = 0; // False
	lcd.rotationNum = 1;
	// if(NewInit()){
	// 	ILI9341_FillScreen(COLOR_BLACK);
	// 	return;
	// }
	/* Reset */
	ILI9341_send_command(ILI9341_RESET);                                     /* software reset command */
	HAL_Delay(100);
	ILI9341_send_command(ILI9341_DISPLAY_OFF);                               /* display off */

	/* Power Control */
	ILI9341_send_command(ILI9341_POWER1);                                    /* power control */
	ILI9341_send_data(0x26);                                                 /* GVDD = 4.75v */
	ILI9341_send_command(ILI9341_POWER2);                                    /* power control */
	ILI9341_send_data(0x11);                                                 /* AVDD = VCIx2, VGH = VCIx7, VGL =-VCIx3 */

	/* Read CHIP ID*/
	uint8_t data[10] = {0};
	ILI9341_send_read_command(ILI9341_READ_ID1, data, 10);
	HAL_Delay(100);
	ILI9341_send_read_command(ILI9341_READ_ID2, data, 10);
	HAL_Delay(100);
	ILI9341_send_read_command(ILI9341_READ_ID3, data, 10);
	HAL_Delay(100);

	/* VCOM */
	ILI9341_send_command(ILI9341_VCOM1);                                     /* VCOM control */
	ILI9341_send_data(0x17);                                                 /* Set the VCOMH voltage (0x35 = 4.025v) */
	ILI9341_send_data(0x40);                                                 /* Set the VCOML voltage (0x3E = -0.950v) */
	ILI9341_send_command(ILI9341_VCOM2);                                     /* VCOM control */
	ILI9341_send_data(0x4a);

	/* Memory Access Control */
	ILI9341_send_command(ILI9341_MAC);
	ILI9341_send_data(0x48);

	/* Pixel Format Set - 16-bit per pixel */
	ILI9341_send_command(ILI9341_PIXEL_FORMAT);
	ILI9341_send_data(0x55);

	ILI9341_send_command(ILI9341_FRC);
	ILI9341_send_data(0);
	ILI9341_send_data(0x1F);

	/* DDRAM */
	ILI9341_send_command(ILI9341_COLUMN_ADDR);                               /* column set */
	ILI9341_send_data(0x00);                                                 /* x0_HIGH---0 */
	ILI9341_send_data(0x00);                                                 /* x0_LOW----0 */
	ILI9341_send_data(0xEF);  //i guess                                               /* x1_HIGH---240 */
	ILI9341_send_data(0xEF);                                                 /* x1_LOW----240 */
	ILI9341_send_command(ILI9341_PAGE_ADDR);                                 /* page address set */
	ILI9341_send_data(0x00);                                                 /* y0_HIGH---0 */
	ILI9341_send_data(0x00);                                                 /* y0_LOW----0 */
	ILI9341_send_data(0x01);                                                 /* y1_HIGH---320 */
	ILI9341_send_data(0x3F);                                                 /* y1_LOW----320 */

	ILI9341_send_command(ILI9341_TEARING_OFF);                               /* tearing effect off */
	//LCD_write_cmd(ILI9341_TEARING_ON);                                    /* tearing effect on */
	//LCD_write_cmd(ILI9341_DISPLAY_INVERSION);                             /* display inversion */
	ILI9341_send_command(ILI9341_Entry_Mode_Set);                            /* entry mode set */
	/* Deep Standby Mode: OFF
	 * Set the output level of gate driver G1-G320: Normal display
	 * Low voltage detection: Disable
	 */
	ILI9341_send_data(0x07);

	/* Display */
	ILI9341_send_command(ILI9341_DFC);                                       /* display function control */
	/* Set the scan mode in non-display area
	 * Determine source/VCOM output in a non-display area in the partial display mode
	 */
	ILI9341_send_data(0x0a);
	/* Select whether the liquid crystal type is normally white type or normally black type
	 * Sets the direction of scan by the gate driver in the range determined by SCN and NL
	 * Select the shift direction of outputs from the source driver
	 * Sets the gate driver pin arrangement in combination with the GS bit to select the optimal scan mode for the module
	 * Specify the scan cycle interval of gate driver in non-display area when PTG to select interval scan
	 */
	ILI9341_send_data(0x82);
	/* Sets the number of lines to drive the LCD at an interval of 8 lines */
	ILI9341_send_data(0x27);
	ILI9341_send_data(0x00);                                                 /* clock divisor */

	ILI9341_send_command(ILI9341_SLEEP_OUT);                                 /* sleep out */
	HAL_Delay(100);
	ILI9341_send_command(ILI9341_DISPLAY_ON);                                /* display on */
	HAL_Delay(100);
	ILI9341_send_command(ILI9341_GRAM);                                      /* memory write */
	HAL_Delay(5);

	/* Touch Screen SPI */
	// TS_TOUCH_RAW_Def myRawTouchDef = {0};
	// TS_TOUCH_RAW_Def localRawTouch = {0};
	// TS_CALIBRATE_Def myTS_Calibrate = {0};
	// lcd.myTS_Calibrate = myTS_Calibrate;
	// lcd.localRawTouch = localRawTouch;
	// lcd.myRawTouchDef = myRawTouchDef;
	// lcd.touchSpi = touchSpi;
	// lcd.TOUCH_CS_PORT = LCD_CS_PORT;
	// lcd.TOUCH_CS_PIN = LCD_CS_PIN;
	// lcd.CMD_Default = 0x84;
}

void STM32_PLC_LCD_Calibrate_Touch() {
	TSC2046_Calibrate();
}

void STM32_PLC_LCD_Get_Touch_Calibration_Parameters(float *Scale_X, float *Scale_Y, float *Bias_X, float *Bias_Y){
	TSC2046_GetTouchCalibrationParameters(Scale_X, Scale_Y, Bias_X, Bias_Y);
}

void STM32_PLC_LCD_Set_Touch_Calibration_Parameters(float *Scale_X, float *Scale_Y, float *Bias_X, float *Bias_Y){
	TSC2046_SetTouchCalibrationParameters(*Scale_X, *Scale_Y, *Bias_X, *Bias_Y);
}

void STM32_PLC_LCD_Get_Touch_Data() {
	TSC2046_GetTouchData();
}