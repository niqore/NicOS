#ifndef TIMER_H
#define TIMER_H

#include "types.h"

uint32_t get_current_tick();

void init_timer(uint32_t freq);

void sleep(uint32_t ms);

#endif