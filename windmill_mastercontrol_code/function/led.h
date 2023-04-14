//
// Created by 86136 on 2023/3/2.
//

#ifndef WINDMILL_MASTERCONTROL_CODE_LED_H
#define WINDMILL_MASTERCONTROL_CODE_LED_H

#define LED1_PIN GPIO_PIN_15
#define LED2_PIN GPIO_PIN_14

/**
 * LE灯的开启或是关断
 */
#define LED_ON(LED_PIN) HAL_GPIO_WritePin(GPIOB, LED_PIN, GPIO_PIN_RESET)
#define LED_OFF(LED_PIN) HAL_GPIO_WritePin(GPIOB, LED_PIN, GPIO_PIN_SET)

/**
 * 翻转LED灯
 */
#define LED_TOGGLE(LED_PIN) HAL_GPIO_TogglePin(GPIOB, LED_PIN);

void led_twinkle(unsigned int GPIO_Pin);

#endif // WINDMILL_MASTERCONTROL_CODE_LED_H
