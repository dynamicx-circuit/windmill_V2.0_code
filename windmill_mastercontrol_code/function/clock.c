//
// Created by 86136 on 2023/3/5.
//

#include "clock.h"
#include "main.h"
#include "tim.h"
#include "can_motor_control.h"

unsigned int times = 0;
unsigned int times_can_recive = 0;
unsigned int times_can_tx = 0;
unsigned int times_rand;
unsigned int times_hit = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == &htim3) {
        times++;
        times_can_recive++;
        times_can_tx++;
        times_hit++;
    }

}

/**
 * 该函数通过定时器来产生随机数，产生的随机数的范围为 1~nums
 * @param nums 产生的随机数的范围
 * @return 产生的随机数
 */
unsigned int rands(unsigned char nums) {
    return __HAL_TIM_GET_COUNTER(&htim4) % nums + 1;
}

/**
 * 获得can接收信号时，指示灯的闪烁时间
 * @return 返回CAN接受时，指示灯的闪烁时间
 */
unsigned int Get_Times_Can_Recive() {
    return times_can_recive;
}

/**
 * 设置时间
 * @param value
 */
void Set_Times_Can_Recive(unsigned int value) {
    times_can_recive = value;
}

/**
 * 获得can发送信号时，指示灯的闪烁时间
 * @return 返回CAN接受时，指示灯的闪烁时间
 */
unsigned int Get_Times_Can_Tx() {
    return times_can_tx;
}

/**
 * 设置时间
 * @param value
 */
void Set_Times_Can_Tx(unsigned int value) {
    times_can_tx = value;
}

/**
 * 获得can接收信号时，指示灯的闪烁时间
 * @return 返回CAN接受时，指示灯的闪烁时间
 */
unsigned int Get_Times() {
    return times;
}

/**
 * 设置时间
 * @param value
 */
void Set_Times(unsigned int value) {
    times = value;
}

/**

 */
unsigned int Get_Times_Hit() {
    return times_hit;
}

/**
 * 设置时间
 * @param value
 */
void Set_Times_Hit(unsigned int value) {
    times_hit = value;
}