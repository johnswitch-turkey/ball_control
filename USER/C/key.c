#include "key.h"

#include "stm32f4xx_hal.h"

#include "control.h"

uint8_t key_state = 0;



/**
 * @brief 使用按键判断小球目标位置
 * @param GPIO_Pin: 触发中断的GPIO引脚
 * @retval None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_3) {
        target_position.target_position_x = ball_site[0][0];
        target_position.target_position_y = ball_site[0][1];
    }
    else if (GPIO_Pin == GPIO_PIN_4) {
        target_position.target_position_x = ball_site[1][0];
        target_position.target_position_x = ball_site[1][1];
    }
    else if (GPIO_Pin == GPIO_PIN_5) {
        target_position.target_position_x = ball_site[2][0];
        target_position.target_position_x = ball_site[2][1];
    }
    else if (GPIO_Pin == GPIO_PIN_6) {
        target_position.target_position_x = ball_site[3][0];
        target_position.target_position_x = ball_site[3][1];
    }
   
}
