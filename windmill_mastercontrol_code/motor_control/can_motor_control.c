//
// Created by 86136 on 2023/3/5.
//

#include "can_motor_control.h"
#include "motor.h"
#include "pid.h"
#include "can.h"
#include "clock.h"
#include "math.h"
#include "key.h"


/**
 * can的滤波器的初始化设置
 */
void can_init(void) {
    CAN_FilterTypeDef can_filter_st;
    can_filter_st.FilterActivation = ENABLE;
    can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;
    can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;
    can_filter_st.FilterIdHigh = (CAN_ID) << 5;
    can_filter_st.FilterIdLow = 0;
    can_filter_st.FilterMaskIdHigh = 0xFFE0;
    can_filter_st.FilterMaskIdLow = 0x0000;
    can_filter_st.FilterBank = 1;
    can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0;
    HAL_CAN_ConfigFilter(&hcan, &can_filter_st);
    HAL_CAN_Start(&hcan);
    HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
}

/**
 * can中断
 * @param hcan
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {

    if (hcan->Instance == CAN1) {
        if (Get_Times_Can_Recive() > 200) {
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_15);
            Set_Times_Can_Recive(0);
        }
        CAN_RxHeaderTypeDef rx_header;
        HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, can_rx_data);
    }
}

pid_t_ pid_struct;

/**
 * 对使用can通讯的电机，以及控制电机的pid进行初始化
 */
void CAN_Motor_Pid_Init() {
    can_init();
    PID_struct_init(&pid_struct, POSITION_PID, 20000,
                    20000, 18.0f, 0.0002f, 13.0f);

    pid_struct.max_err = 20000.0f;

}

const double a = 1.0;
double w = 1.90;
double b = 2.090 - a;

/**
 * 设置速度为时间的正弦函数
 * @param speed
 */
void Set_Speed_Sin(double *speed) {

    *speed = 90 * a * (sin(w * 0.001 * Get_Times()) + b) * DECELERATION_RATIO_3508 / 3.1415926;;

}

/**
 * 设置电机的速度
 * @param set_speed
 */
void CAN_Motor_Set(double set_speed) {

    get_moto_measure(&moto_measure);
    pid_calc(&pid_struct, (float) moto_measure.speed_rpm, set_speed);
    set_moto_current((int16_t) pid_struct.pos_out);
}


double set_speed_temp = 10 * DECELERATION_RATIO_3508 * 3;
double set_speed = 0;
uint8_t key_flag_motor = 1;

/**
 * 使用按键对电机的速度状态进行切换。
 * 按一次为匀速、再按一次为正弦
 */
void CAN_Motor_Key_Set() {
    uint8_t key_flag = Key_Enter();
    if (key_flag == MOTOR_KEY) {
        key_flag_motor = -key_flag_motor;
    }
    if (key_flag_motor == 1) {
        set_speed = set_speed_temp;
    } else {
        Set_Speed_Sin(&set_speed);
    }
    CAN_Motor_Set(set_speed);

    //printf("%d\n", moto_measure.real_current);
}