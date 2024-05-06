/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "button.h"
#include "message.h"
#include "cmsis_os2.h"

extern osThreadId_t ButtonHandleHandle;
extern osThreadId_t LCDTaskHandleHandle;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart1;

void ButtonProcess(uint16_t GPIO_Pin){
  switch(GPIO_Pin){
	case BUTTON1_IRQ:
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
		break;
	case BUTTON2_IRQ:
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
		break;
	case BUTTON3_IRQ:
		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
		break;
	case BUTTON4_IRQ:
		HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);
		break;
	case BUTTON5_IRQ:
		HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, GPIO_PIN_SET);
		break;
	case BUTTON6_IRQ:
		HAL_GPIO_WritePin(LED6_GPIO_Port, LED6_Pin, GPIO_PIN_SET);
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
  int button = 0;
  switch(GPIO_Pin){
	case Button1_Pin:
		button = BUTTON1_IRQ;
		HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
		xTaskNotifyFromISR(ButtonHandleHandle, button, eSetValueWithOverwrite, &hptw);
		break;
	case Button2_Pin:
		button = BUTTON2_IRQ;
		HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
		xTaskNotifyFromISR(ButtonHandleHandle, button, eSetValueWithOverwrite, &hptw);
		break;
	case Button3_Pin:
		button = BUTTON3_IRQ;
		HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
		xTaskNotifyFromISR(ButtonHandleHandle, button, eSetValueWithOverwrite, &hptw);
		break;
	case Button4_Pin:
		button = BUTTON4_IRQ;
		HAL_GPIO_TogglePin(LED4_GPIO_Port, LED4_Pin);
		xTaskNotifyFromISR(ButtonHandleHandle, button, eSetValueWithOverwrite, &hptw);
		break;
	case Button5_Pin:
		button = BUTTON5_IRQ;
		HAL_GPIO_TogglePin(LED5_GPIO_Port, LED5_Pin);
		xTaskNotifyFromISR(ButtonHandleHandle, button, eSetValueWithOverwrite, &hptw);
		break;
	case Button6_Pin:
		button = BUTTON6_IRQ;
		HAL_GPIO_TogglePin(LED6_GPIO_Port, LED6_Pin);
		xTaskNotifyFromISR(ButtonHandleHandle, button, eSetValueWithOverwrite, &hptw);
		break;
	case EN_SW_Pin:
		//button = ENCODER_IRQ;
		xTaskNotifyFromISR(LCDTaskHandleHandle, NULL, NULL, &hptw);	
		break; 
	default:
		return;
	}

}