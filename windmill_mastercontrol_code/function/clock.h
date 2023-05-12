//
// Created by 86136 on 2023/3/5.
//

#ifndef WINDMILL_MASTERCONTROL_CODE_CLOCK_H
#define WINDMILL_MASTERCONTROL_CODE_CLOCK_H

unsigned int rands(unsigned char nums);

unsigned int Get_Times_Can_Recive();

void Set_Times_Can_Recive(unsigned int value);

unsigned int Get_Times_Can_Tx();

void Set_Times_Can_Tx(unsigned int value);

unsigned int Get_Times();

void Set_Times(unsigned int value);


unsigned int Get_Times_Hit();


void Set_Times_Hit(unsigned int value);

unsigned int Get_Times_Reset();

void Set_Times_Reset(unsigned int value);

#endif //WINDMILL_MASTERCONTROL_CODE_CLOCK_H
