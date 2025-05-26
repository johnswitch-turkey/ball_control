#ifndef __LED_H
#define __LED_H

#define LED_PIN GPIO_PIN_13
#define LED_PORT GPIOC

void led_toggle(void);
void led_on(void);
void led_off(void);

#endif
