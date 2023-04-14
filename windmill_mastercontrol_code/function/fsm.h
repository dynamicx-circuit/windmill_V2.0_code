//
// Created by kade on 23-4-14.
//

#ifndef WINDMILL_MASTERCONTROL_CODE_FSM_H
#define WINDMILL_MASTERCONTROL_CODE_FSM_H

#include "main.h"

enum fsm_state { reset, waiting_hit, all_hit, test };

void FSM_Load(uint8_t *fsm_state);
#endif // WINDMILL_MASTERCONTROL_CODE_FSM_H
