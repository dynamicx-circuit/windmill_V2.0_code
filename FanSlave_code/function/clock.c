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
 * ��ü�ͷ������ʱ��
 * @return  ʱ��
 */
unsigned int Get_Times_Flow() {
    return times_flow;
}

/**
 * ���ü�ͷ������ʱ��
 * @param value Ҫ���õ�ʱ��
 */
void Set_Times_Flow(unsigned int value) {
    times_flow = value;
}

/**
 * �ú���ͨ����ʱ���������������������������ķ�ΧΪ 1~nums
 * @param nums ������������ķ�Χ
 * @return �����������
 */
unsigned int rands(unsigned char nums) {
    return __HAL_TIM_GET_COUNTER(&htim3) % nums + 1;
}