//
// Created by 86136 on 2023/3/2.
//

#include "key.h"

/**
 * �԰���������Ӧ
 * ����key1���ı��������ɫ
 * ����key2�����Ƶ�ǵ�ת�١�
 * @return
 */
unsigned char Key_Enter() {
    unsigned char flag = 0;
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == GPIO_PIN_RESET) {
        HAL_Delay(10);
        while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == GPIO_PIN_RESET);
        HAL_Delay(10);
        flag = LED_KEY;
    } else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) == GPIO_PIN_RESET) {
        HAL_Delay(10);
        while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) == GPIO_PIN_RESET);
        HAL_Delay(10);
        flag = MOTOR_KEY;
    }
    return flag;
}