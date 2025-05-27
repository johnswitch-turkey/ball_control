#include "control_logic.h"
#include "control.h"
#include "uart.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1)
    {
//		if (Finded_flag == 1){
			control();
		}
//    }
		
}












