//
// Created by 86136 on 2023/3/13.
//

#include "HX711.h"
#include "main.h"
#include "delay.h"

#define delay_time 10

unsigned long HX711_ReadCount(void) {
    unsigned long Count;
    unsigned char i;
    HX711_SCK_LOW;
    Count = 0;

    while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_SET);

    for (i = 0; i < 24; i++) {
        HX711_SCK_HIGH;
        delayus(delay_time);
        Count = Count << 1;
        HX711_SCK_LOW;
        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_SET) Count++;
        delayus(delay_time);

    }
    HX711_SCK_HIGH;
    delayus(delay_time);

    Count = Count ^ 0x800000;
    HX711_SCK_LOW;
    delayus(delay_time);

    return (Count);
}