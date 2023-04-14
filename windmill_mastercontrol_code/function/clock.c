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
 * �ú���ͨ����ʱ���������������������������ķ�ΧΪ 1~nums
 * @param nums ������������ķ�Χ
 * @return �����������
 */
unsigned int rands(unsigned char nums) {
    return __HAL_TIM_GET_COUNTER(&htim4) % nums + 1;
}

/**
 * ���can�����ź�ʱ��ָʾ�Ƶ���˸ʱ��
 * @return ����CAN����ʱ��ָʾ�Ƶ���˸ʱ��
 */
unsigned int Get_Times_Can_Recive() {
    return times_can_recive;
}

/**
 * ����ʱ��
 * @param value
 */
void Set_Times_Can_Recive(unsigned int value) {
    times_can_recive = value;
}

/**
 * ���can�����ź�ʱ��ָʾ�Ƶ���˸ʱ��
 * @return ����CAN����ʱ��ָʾ�Ƶ���˸ʱ��
 */
unsigned int Get_Times_Can_Tx() {
    return times_can_tx;
}

/**
 * ����ʱ��
 * @param value
 */
void Set_Times_Can_Tx(unsigned int value) {
    times_can_tx = value;
}

/**
 * ���can�����ź�ʱ��ָʾ�Ƶ���˸ʱ��
 * @return ����CAN����ʱ��ָʾ�Ƶ���˸ʱ��
 */
unsigned int Get_Times() {
    return times;
}

/**
 * ����ʱ��
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
 * ����ʱ��
 * @param value
 */
void Set_Times_Hit(unsigned int value) {
    times_hit = value;
}