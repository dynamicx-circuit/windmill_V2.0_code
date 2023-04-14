//
// Created by kade on 23-4-14.
//
/**
 * The windmill has several state
 * + reset
 * + waiting hit
 * + all fans have been hit
 */
#include "fsm.h"
#include "clock.h"
#include "hit_reaction.h"
uint8_t flag_hit = 0;
extern uint8_t rand_num_max;

void FSM_Load(uint8_t *fsm_state) {
  switch (*fsm_state) {
  case reset:
    Windmill_Resest();
    flag_hit = 0;
    Set_Times_Hit(0);
    *fsm_state = waiting_hit;
    break;
  case waiting_hit:
    if (Hit_Reaction() == HIT_CURRECT) {
      flag_hit++;
      Set_Times_Hit(0);
    }
    break;
  case all_hit:
    flag_hit = 0;
    All_Hit_Rection();
    *fsm_state = waiting_hit;
    break;
  default:
    break;
  }

  if (Get_blade_state() == 0x3E) {
    *fsm_state = all_hit;
  }
  if (flag_hit > 0 && Get_Times_Hit() > 5000) {
    *fsm_state = reset;
  }
}