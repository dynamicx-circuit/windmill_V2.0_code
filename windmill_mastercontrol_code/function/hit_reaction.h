//
// Created by 86136 on 2023/3/2.
//

#ifndef WINDMILL_MASTERCONTROL_CODE_HIT_REACTION_H
#define WINDMILL_MASTERCONTROL_CODE_HIT_REACTION_H

#include "main.h"

#define WINDMILL_BLADE_GPIO1 1
#define WINDMILL_BLADE_GPIO2 2
#define WINDMILL_BLADE_GPIO3 3
#define WINDMILL_BLADE_GPIO4 4
#define WINDMILL_BLADE_GPIO5 5

#define HIT_FLASE 0
#define HIT_CURRECT 1
#define HIT_NONE 2

void Hit_Reaction_Init();

uint8_t Hit_WindmillBlade();

uint8_t Hit_Reaction();
void All_Hit_Rection();
void Windmill_Resest();

uint8_t Get_blade_state();

uint8_t Get_blade_state_expect();

#endif // WINDMILL_MASTERCONTROL_CODE_HIT_REACTION_H
