//
// Created by 86136 on 2023/3/14.
//

#include "clock.h"
#include "main.h"
#include "tim.h"

unsigned int times_flow = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

    times_flow++;
}

/**
 * 获得箭头流动的时间
 * @return  时间
 */
unsigned int Get_Times_Flow() {
    return times_flow;
}

/**
 * 重置箭头流动的时间
 * @param value 要设置的时间
 */
void Set_Times_Flow(unsigned int value) {
    times_flow = value;
}

/**
 * 该函数通过定时器来产生随机数，产生的随机数的范围为 1~nums
 * @param nums 产生的随机数的范围
 * @return 产生的随机数
 */
unsigned int rands(unsigned char nums) {
    return __HAL_TIM_GET_COUNTER(&htim3) % nums + 1;
}