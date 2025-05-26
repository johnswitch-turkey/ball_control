#ifndef __SERVO_H
#define __SERVO_H

#define SERVOA_PIN GPIO_PIN_0
#define SERVOA_PORT GPIOA
#define SERVOB_PIN GPIO_PIN_1
#define SERVOB_PORT GPIOA

#include "stm32f4xx_hal.h"

void Set_servo_angle(uint16_t angle1,uint16_t angle2);

#endif
