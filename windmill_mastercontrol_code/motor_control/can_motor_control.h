//
// Created by 86136 on 2023/3/5.
//

#ifndef WINDMILL_MASTERCONTROL_CODE_CAN_MOTOR_CONTROL_H
#define WINDMILL_MASTERCONTROL_CODE_CAN_MOTOR_CONTROL_H

void CAN_Motor_Init();

void CAN_Motor_Pid_Init();

void Set_Speed_Sin(double *speed);

void CAN_Motor_Key_Set();

#endif //WINDMILL_MASTERCONTROL_CODE_CAN_MOTOR_CONTROL_H
