//
// Created by 86136 on 2023/3/2.
//

#include "led.h"
#include "clock.h"
#include "gpio.h"
#include "usart.h"

void led_twinkle(unsigned int GPIO_Pin) {
    if (Get_Times() > 1000) {
        LED_TOGGLE(GPIO_Pin);
        Set_Times(0);
    }
}
