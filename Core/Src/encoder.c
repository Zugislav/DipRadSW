/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "encoder.h"
#include "message.h"

/* Defines ------------------------------------------------------------------*/
#define ENCODER_MAX_VALUE 1024

extern TIM_HandleTypeDef htim1;

uint16_t m_uValueOld;         //Stores the previous timer counter value
uint16_t m_uValueNew;         //Stores the current timer counter value
int16_t m_iValue;             //Stores the difference in between the previous and current timer counter values
uint16_t m_uAccel;            //Stores the acceleration value of the encoder (how fast the encoder is being rotated)

void encoderUpdate(){

		uint16_t uTicks = HAL_GetTick();
		m_uValueOld = m_uValueNew;                        //Store previous current value into old value
		m_uValueNew = __HAL_TIM_GET_COUNTER(&htim1);  //Update current value from Timer's counter register

		//The following code is used to determine if the encoder has been moved in a clockwise or counter-clockwise rotation,
		//while also taking into account whether the counter register value has wrapped around
		uint16_t uDiffA;
		uint16_t uDiffB;
		bool bValComp = (m_uValueNew < m_uValueOld);
		if (bValComp) {
			uDiffA = m_uValueOld - m_uValueNew;
			uDiffB = m_uValueNew + (ENCODER_MAX_VALUE - m_uValueOld);
		} else {
			uDiffA = m_uValueNew - m_uValueOld;
			uDiffB = m_uValueOld + (ENCODER_MAX_VALUE - m_uValueNew);
		}
		uint16_t uDiff = (uDiffA < uDiffB) ? uDiffA : uDiffB;
		m_iValue += (bValComp ? 0-uDiff : uDiff);

		//Calculate encoder acceleration value
		m_uAccel = uDiff * 1000 / uTicks;
}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
    //value koji trebamo htim->Instance->CNT;
    return;
}
