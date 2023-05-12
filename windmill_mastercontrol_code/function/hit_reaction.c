//
// Created by 86136 on 2023/3/2.
//

#include "hit_reaction.h"
#include "clock.h"
#include "communication.h"
#include "delay.h"
#include "led.h"
#include "oled.h"
#include "spi.h"
#include <stdio.h>

uint8_t blade_state = 0; /*该变量用来存储扇叶的亮灯状态*/
uint8_t blade_state_expect = 0; /*该变量用来存储扇叶下一时刻的亮灯状态*/

uint8_t rand_num_max = 5;

/**
 * 该函数为大风车的初始化函数。
 *  +其中的一个功能是确定第一叶要击打的扇叶。
 */
void Hit_Reaction_Init() {
    unsigned char flag = rands(rand_num_max);
    blade_state_expect |= (1 << flag);
}

/**
 * 判断大风车的扇叶是否被打击到，如果是则返回对应扇叶的编码。
 * @return
 */
uint8_t Hit_WindmillBlade() {
    uint8_t flag = 0;
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == GPIO_PIN_RESET) {
        flag = WINDMILL_BLADE_GPIO1;
    } else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == GPIO_PIN_RESET) {
        flag = WINDMILL_BLADE_GPIO2;
    } else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == GPIO_PIN_RESET) {
        flag = WINDMILL_BLADE_GPIO3;
    } else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8) == GPIO_PIN_RESET) {
        flag = WINDMILL_BLADE_GPIO4;
    } else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == GPIO_PIN_RESET) {
        flag = WINDMILL_BLADE_GPIO5;
    }
    return flag;
}

void Windmill_Resest() {
    // 关闭所有扇叶
    for (int i = 1; i <= rand_num_max; i++) {

        SPI_Send_Data(i, spi_data[off]);
    }
    HAL_Delay(100);
    // 重新设置随机数
    blade_state_expect = 0;
    blade_state = 0;
    unsigned char flag = rands(rand_num_max);
    blade_state_expect |= (1 << flag);

    for (int i = 1; i <= rand_num_max; i++) {
        if ((Get_blade_state_expect() & ~Get_blade_state()) & (1 << i)) {
            SPI_Send_Data(i, spi_data[readyhit]);
            break;
        }
    }
    // OLED_Fill(0x00);
}

/**
 * when all fns have been hit, they will off first and then blink.
 * And then resat.
 */
void All_Hit_Rection() {
    for (int i = 1; i <= rand_num_max; i++) {
        SPI_Send_Data(i, spi_data[off]);
    }
    HAL_Delay(200);
    {
        Spi_CS_LOW(1);
        Spi_CS_LOW(2);
        Spi_CS_LOW(3);
        Spi_CS_LOW(4);
        Spi_CS_LOW(5);

        delayus(10);
        HAL_SPI_Transmit(&hspi1, &spi_data[blink], 1, HAL_MAX_DELAY);
        Spi_CS_HIGH(1);
        Spi_CS_HIGH(2);
        Spi_CS_HIGH(3);
        Spi_CS_HIGH(4);
        Spi_CS_HIGH(5);

        delayus(100);
    }
    Windmill_Resest();
    HAL_Delay(100);
}

/**
 * 对击打做出反应
 * @return
 */

uint8_t Hit_Reaction() {
    uint8_t flag = Hit_WindmillBlade();
    if (flag) {
        if ((blade_state_expect & ~(blade_state)) & (1 << flag)) {
            // 打中了期望的扇叶，对应的扇叶编码会存到blade_state中。
            blade_state |= (1 << flag);
            // 重新产生随机数，设置期望被打中的扇叶。
            uint8_t nums = rands(rand_num_max);
            while (1) {
                // 如果全都击中了,将击中的信号发送给全部扇叶
                if (blade_state == 0x3E) {
                    break;
                } else if ((blade_state_expect & (1 << nums)) == 0) {
                    // 选中了期望的扇叶，将该扇叶设置为等待击打的状态。
                    SPI_Send_Data(nums, spi_data[readyhit]);
                    break;
                } else {
                    nums = rands(rand_num_max);

                    if (Get_Times() > 100) {

                        LED_TOGGLE(LED1_PIN);
                        Set_Times(0);
                    }
                }
            }
            blade_state_expect |= (1 << nums);
//      OLED_Fill(0x00);
//      OLED_ShowInt(0, 0, blade_state_expect);
//      OLED_ShowInt(48, 0, blade_state);
//      OLED_ShowInt(0, 2, nums);
            return HIT_CURRECT;
        } else {
            return HIT_FLASE;
        }
    } else {
        return HIT_NONE;
    }
}

/**
 * 返回扇叶的状态
 * @return
 */
uint8_t Get_blade_state() { return blade_state; }

/**
 * 返回扇叶的期望状态
 * @return
 */
uint8_t Get_blade_state_expect() { return blade_state_expect; }