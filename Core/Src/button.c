/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "button.h"
#include "message.h"

extern osMessageQueueId_t mainQueueHandle;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart1;

void ButtonHandle(uint16_t GPIO_Pin){
  switch(GPIO_Pin){
	case Button1_Pin:
		HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
		break;
	case Button2_Pin:
		HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
		break;
	case Button3_Pin:
		HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
		break;
	case Button4_Pin:
		HAL_GPIO_TogglePin(LED4_GPIO_Port, LED4_Pin);
		break;
	case Button5_Pin:
		HAL_GPIO_TogglePin(LED5_GPIO_Port, LED5_Pin);
		break;
	case Button6_Pin:
		HAL_GPIO_TogglePin(LED6_GPIO_Port, LED6_Pin);
		break;
	default:
		return;
	}
}

/* USER CODE BEGIN 4 */
/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  BaseType_t hptw = pdFALSE;
  int button;
  switch(GPIO_Pin){
	case Button1_Pin:
		HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
		button = BUTTON1_IRQ;
		xQueueSendFromISR(mainQueueHandle, &button, &hptw);
		break;
	case Button2_Pin:
		HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
		button = BUTTON2_IRQ;
		xQueueSendFromISR(mainQueueHandle,&button, &hptw);
		break;
	case Button3_Pin:
		HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
		button = BUTTON3_IRQ;
		xQueueSendFromISR(mainQueueHandle, &button, &hptw);
		break;
	case Button4_Pin:
		HAL_GPIO_TogglePin(LED4_GPIO_Port, LED4_Pin);
		button = BUTTON4_IRQ;
		xQueueSendFromISR(mainQueueHandle, &button, &hptw);
		break;
	case Button5_Pin:
		HAL_GPIO_TogglePin(LED5_GPIO_Port, LED5_Pin);
		button = BUTTON5_IRQ;
		xQueueSendFromISR(mainQueueHandle, &button, &hptw);
		break;
	case Button6_Pin:
		HAL_GPIO_TogglePin(LED6_GPIO_Port, LED6_Pin);
		button = BUTTON6_IRQ;
		xQueueSendFromISR(mainQueueHandle, &button, &hptw);
		break;
	default:
		return;
	}
}