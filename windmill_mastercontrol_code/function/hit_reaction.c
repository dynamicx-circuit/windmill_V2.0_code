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

uint8_t blade_state = 0; /*�ñ��������洢��Ҷ������״̬*/
uint8_t blade_state_expect = 0; /*�ñ��������洢��Ҷ��һʱ�̵�����״̬*/

uint8_t rand_num_max = 5;

/**
 * �ú���Ϊ��糵�ĳ�ʼ��������
 *  +���е�һ��������ȷ����һҶҪ�������Ҷ��
 */
void Hit_Reaction_Init() {
    unsigned char flag = rands(rand_num_max);
    blade_state_expect |= (1 << flag);
}

/**
 * �жϴ�糵����Ҷ�Ƿ񱻴������������򷵻ض�Ӧ��Ҷ�ı��롣
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
    // �ر�������Ҷ
    for (int i = 1; i <= rand_num_max; i++) {

        SPI_Send_Data(i, spi_data[off]);
    }
    HAL_Delay(100);
    // �������������
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
 * �Ի���������Ӧ
 * @return
 */

uint8_t Hit_Reaction() {
    uint8_t flag = Hit_WindmillBlade();
    if (flag) {
        if ((blade_state_expect & ~(blade_state)) & (1 << flag)) {
            // ��������������Ҷ����Ӧ����Ҷ�����浽blade_state�С�
            blade_state |= (1 << flag);
            // ���²�����������������������е���Ҷ��
            uint8_t nums = rands(rand_num_max);
            while (1) {
                // ���ȫ��������,�����е��źŷ��͸�ȫ����Ҷ
                if (blade_state == 0x3E) {
                    break;
                } else if ((blade_state_expect & (1 << nums)) == 0) {
                    // ѡ������������Ҷ��������Ҷ����Ϊ�ȴ������״̬��
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
 * ������Ҷ��״̬
 * @return
 */
uint8_t Get_blade_state() { return blade_state; }

/**
 * ������Ҷ������״̬
 * @return
 */
uint8_t Get_blade_state_expect() { return blade_state_expect; }