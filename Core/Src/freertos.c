/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "encoder.h"
#include "button.h"
#include "queue.h"
#include "stm32f407xx.h"
#include "gpio.h"
#include "usart.h"
#include "message.h"
#include "limits.h"
#include "Touch_screen.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart1;

extern TIM_HandleTypeDef htim1;
static encoderHandle encoder = {0};

extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi3;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for LCDTaskHandle */
osThreadId_t LCDTaskHandleHandle;
const osThreadAttr_t LCDTaskHandle_attributes = {
  .name = "LCDTaskHandle",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for EncoderHandle */
osThreadId_t EncoderHandleHandle;
const osThreadAttr_t EncoderHandle_attributes = {
  .name = "EncoderHandle",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ButtonHandle */
osThreadId_t ButtonHandleHandle;
const osThreadAttr_t ButtonHandle_attributes = {
  .name = "ButtonHandle",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for mainQueue */
osMessageQueueId_t mainQueueHandle;
const osMessageQueueAttr_t mainQueue_attributes = {
  .name = "mainQueue"
};
/* Definitions for printMutex */
osMutexId_t printMutexHandle;
const osMutexAttr_t printMutex_attributes = {
  .name = "printMutex"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void LCDTask(void *argument);
void EncoderTask(void *argument);
void ButtonTask(void *argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of printMutex */
  printMutexHandle = osMutexNew(&printMutex_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of mainQueue */
  mainQueueHandle = osMessageQueueNew (128, sizeof(uint8_t), &mainQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of LCDTaskHandle */
  LCDTaskHandleHandle = osThreadNew(LCDTask, NULL, &LCDTaskHandle_attributes);

  /* creation of EncoderHandle */
  EncoderHandleHandle = osThreadNew(EncoderTask, NULL, &EncoderHandle_attributes);

  /* creation of ButtonHandle */
  ButtonHandleHandle = osThreadNew(ButtonTask, NULL, &ButtonHandle_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartDefaultTask */
  // Create the container (queue) for data
  /* Infinite loop */
  for(;;)
  {
    /* USER CODE END WHILE */
 /*   if(xQueueReceive(mainQueueHandle, &buffer, 200) == pdTRUE ){
      if(buffer >= BUTTON1_IRQ && buffer <= BUTTON6_IRQ){
        printSerial("Button IRQ\r\n");
        xTaskNotify(ButtonTask, (uint32_t)buffer, eSetValueWithOverwrite);
      }
      else if(buffer == ENCODER_IRQ){
        xTaskNotify(EncoderTask, 0, eNoAction);
        printSerial("Encoder IRQ\r\n");
      }
      else if(buffer == LCD_IRQ){
        //LCDHandle();
        printSerial("LCD IRQ\r\n");
      }
      else if (buffer == TOUCHPAD_IRQ){
        //TouchpadHandle(buffer[0]);
        printSerial("Touchpad IRQ\r\n");
      }
      else {
        printSerial("Invalid IRQ\r\n");
      }
    }
    */
    /* USER CODE BEGIN 3
    HAL_UART_Transmit(&huart4, "Hello world1!\r\n", 14, 200);
    osDelay(100);
    HAL_UART_Transmit(&huart1, "Hello world2!\r\n", 15, 200);
    osDelay(100);
     */
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_LCDTask */
/**
* @brief Function implementing the LCDTaskHandle thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LCDTask */
void LCDTask(void *argument)
{
  /* USER CODE BEGIN LCDTask */

  /* Check if we touch at the screen */
  STM32_PLC_LCD(&hspi1, &hspi3, LCD_CS_GPIO_Port, LCD_CS_Pin, DC_RS_GPIO_Port, DC_RS_Pin, RST_GPIO_Port, RST_Pin);

  /* Infinite loop */

  uint8_t frame_id = 0;
  STM32_PLC_LCD_Show_Main_Frame(&frame_id);

  for(;;)
  {
    STM32_PLC_LCD_Call_Main_Logic(&frame_id);
    osDelay(50);
  }
  /* USER CODE END LCDTask */
}

/* USER CODE BEGIN Header_EncoderTask */
/**
* @brief Function implementing the EncoderTaskHand thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_EncoderTask */
void EncoderTask(void *argument)
{
  /* USER CODE BEGIN EncoderTask */
  /* Infinite loop */
  Encoder_init(&encoder, &htim1, ENCODER_PULSES_PER_ROTATION);
	float speed = 0;
	float difference = 0;
	for(;;)
	{
		Encoder_count(&encoder);
		speed = Encoder_getSpeed(&encoder)/ENCODER_PULSES_PER_ROTATION; //Unit: RPM
		difference = Encoder_getDifference(&encoder);
    //We need to update the value off the encoder on the screen that will be a shared variable between this task and the LCDTask

    //do it with queue,now go to sleep
    //difference = 10;
    osDelay(1);
	}
  /* USER CODE END EncoderTask */
}

/* USER CODE BEGIN Header_ButtonTask */
/**
* @brief Function implementing the ButtonTaskHandl thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ButtonTask */
void ButtonTask(void *argument)
{
  /* USER CODE BEGIN ButtonTask */
  uint32_t uValue;
  /* Infinite loop */
  for(;;)
  {
        // Wait for a notification and get the notification value
    if(xTaskNotifyWait(0, ULONG_MAX, &uValue, 100) == pdPASS)
    {
      printSerial("Button %d IRQ\r\n", uValue);
      ButtonProcess(uValue);
      //in buttonHandle also set some thing to have it displaied on the LCD
    }
    osDelay(1);
  }
  /* USER CODE END ButtonTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

