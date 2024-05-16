/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "button.h"
#include "message.h"
#include "cmsis_os2.h"
#include "task.h"

extern osThreadId_t ButtonHandleHandle;
extern osThreadId_t LCDTaskHandleHandle;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim2;
bool state = true;

void ButtonProcess(uint16_t GPIO_Pin){
  switch(GPIO_Pin){
	case BUTTON1_IRQ:
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
		break;
	case BUTTON2_IRQ:
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
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
	case Button3_Pin:
		if(state){
			HAL_TIM_Base_Start_IT(&htim2);
			state = false;
			button = BUTTON3_IRQ;
			HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
			xTaskNotifyFromISR(LCDTaskHandleHandle, button, eSetValueWithOverwrite, &hptw);
			break;
		}
		else return;
	case Button4_Pin:
		if(state){
			HAL_TIM_Base_Start_IT(&htim2);
			state = false;
			button = BUTTON4_IRQ;
			HAL_GPIO_TogglePin(LED4_GPIO_Port, LED4_Pin);
			xTaskNotifyFromISR(LCDTaskHandleHandle, button, eSetValueWithOverwrite, &hptw);
			break;
		}
		else return;
	case Button5_Pin:
		if(state){
			HAL_TIM_Base_Start_IT(&htim2);
			state = false;
			button = BUTTON5_IRQ;
			HAL_GPIO_TogglePin(LED5_GPIO_Port, LED5_Pin);
			xTaskNotifyFromISR(LCDTaskHandleHandle, button, eSetValueWithOverwrite, &hptw);
			break;
		}
		else return;
	case Button6_Pin:
		if(state){
			HAL_TIM_Base_Start_IT(&htim2);
			state = false;
			button = BUTTON6_IRQ;
			HAL_GPIO_TogglePin(LED6_GPIO_Port, LED6_Pin);
			xTaskNotifyFromISR(LCDTaskHandleHandle, button, eSetValueWithOverwrite, &hptw);
			break;
		}
		else return;
	case Button1_Pin:
		if(state){
			HAL_TIM_Base_Start_IT(&htim2);
			state = false;
			button = BUTTON1_IRQ;
			//HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
			xTaskNotifyFromISR(ButtonHandleHandle, button, eSetValueWithOverwrite, &hptw);
			break;
		}
		else return;
	case Button2_Pin:
		if(state){
			HAL_TIM_Base_Start_IT(&htim2);
			state = false;
			button = BUTTON2_IRQ;
			//HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
			xTaskNotifyFromISR(ButtonHandleHandle, button, eSetValueWithOverwrite, &hptw);
			break;
		}
		else return;

	default:
		return;
	}

}