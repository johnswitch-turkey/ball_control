#include "control_logic.h"
#include "control.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1)
    {
        control();
    }
}












