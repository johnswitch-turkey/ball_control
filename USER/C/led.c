/*负责LED的翻转、点亮和熄灭*/

#include "led.h"

#include "stm32f4xx_hal.h"

void led_toggle(void){
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13); // 翻转LED灯的状态
}
void led_on(void){
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET); // 点亮LED灯
}
void led_off(void){
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // 熄灭LED灯
}
