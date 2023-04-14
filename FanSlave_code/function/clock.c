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